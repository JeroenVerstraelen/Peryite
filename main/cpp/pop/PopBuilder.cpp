/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2017, 2018, Kuylen E, Willem L, Broeckhove J
 */

/**
 * @file
 * Initialize populations: implementation.
 */

#include "PopBuilder.h"

#include "pop/Population.h"
#include "pop/SurveySeeder.h"
#include "util/FileSys.h"
#include "util/RNManager.h"
#include "util/StringUtils.h"
#include "gen/geogen/GeoGenerator.h"
#include "gen/popgen/PopGenerator.h"
#include "gen/files/GenDirectory.h"
#include "gen/files/PopulationFile.h"

#include <boost/property_tree/ptree.hpp>

namespace stride {

using namespace std;
using namespace util;
using namespace boost::property_tree;

PopBuilder::PopBuilder(const ptree& configPt, RNManager& rnManager) : m_config_pt(configPt), m_rn_manager(rnManager) {}

shared_ptr<Population> PopBuilder::MakePoolSys(std::shared_ptr<Population> pop)
{
        using namespace ContactPoolType;
        auto& population = *pop;
        auto& poolSys    = population.GetContactPoolSys();
        // --------------------------------------------------------------
        // Read the ContactPoolSys from a file.
        // --------------------------------------------------------------
        gen::files::PopulationFile::ReadPoolSys(m_config_pt, poolSys);

        // --------------------------------------------------------------
        // Insert persons (pointers) in their contactpools. Having Id 0
        // means "not belonging pool of that type" (e.g. school/ work -
        // cannot belong to both, or e.g. out-of-work).
        //
        // Pools are uniquely identified by (typ, subscript) and a Person
        // belongs, for typ, to pool with subscrip p.GetPoolId(typ).
        // Defensive measure: we have a pool for Id 0 and leave it empty.
        // --------------------------------------------------------------
        for (auto& p : population) {
                for (Id typ : IdList) {
                        const auto poolId = p.GetPoolId(typ);
                        if (poolId > 0) {
                                poolSys[typ][poolId].AddMember(&p);
                        }
                }
        }

        return pop;
}

shared_ptr<Population> PopBuilder::MakePersons(std::shared_ptr<Population> pop)
{
        //------------------------------------------------
        // Read persosns from file.
        //------------------------------------------------
        const auto file_name        = m_config_pt.get<string>("run.population_file");
        const auto use_install_dirs = m_config_pt.get<bool>("run.use_install_dirs");
        const auto file_path        = (use_install_dirs) ? FileSys::GetDataDir() /= file_name : file_name;
        if (!is_regular_file(file_path)) {
                throw runtime_error(string(__func__) + "> Population file " + file_path.string() + " not present.");
        }

        boost::filesystem::ifstream pop_file;
        pop_file.open(file_path.string());
        if (!pop_file.is_open()) {
                throw runtime_error(string(__func__) + "> Error opening population file " + file_path.string());
        }

        string line;
        getline(pop_file, line); // step over file header
        unsigned int person_id = 0U;

        while (getline(pop_file, line)) {
                const auto values                   = Split(line, ",");
                const auto age                      = FromString<unsigned int>(values[0]);
                const auto household_id             = FromString<unsigned int>(values[1]);
                const auto school_id                = FromString<unsigned int>(values[2]);
                const auto work_id                  = FromString<unsigned int>(values[3]);
                const auto primary_community_id     = FromString<unsigned int>(values[4]);
                const auto secondary_community_id   = FromString<unsigned int>(values[5]);
                auto latitude = 0.0;
                auto longitude = 0.0;
                unsigned int tourismPrimid= 0;
                unsigned int tourismSecid= 0;
                unsigned int travelWorkid = 0;
                unsigned int startDayTravel = 0;
                unsigned int endDayTravel = 0;
                if (values.size() >= 8) {
                        latitude = FromString<double>(values[6]);
                        longitude = FromString<double>(values[7]);
                        tourismPrimid = FromString<unsigned int>(values[8]);
                        tourismSecid = FromString<unsigned int>(values[9]);
                        travelWorkid = FromString<unsigned int>(values[10]);
                        startDayTravel = FromString<unsigned int>(values[11]);
                        endDayTravel = FromString<unsigned int>(values[12]);
                }
                pop->CreatePerson(person_id, age, household_id, school_id, work_id, primary_community_id,
                                  secondary_community_id, latitude, longitude, tourismPrimid, tourismSecid, travelWorkid, startDayTravel, endDayTravel);
                ++person_id;
        }

        pop_file.close();

        //------------------------------------------------
        // Read regions from file.
        //------------------------------------------------
        pop->SetRegions(gen::files::PopulationFile::ReadRegions(m_config_pt));

        return pop;
}

shared_ptr<Population> PopBuilder::Build(std::shared_ptr<Population> pop)
{
        //------------------------------------------------
        // Check validity of input data.
        //------------------------------------------------
        const auto seeding_rate = m_config_pt.get<double>("run.seeding_rate");
        if (seeding_rate > 1.0) {
                throw runtime_error(string(__func__) + "> Bad input data for seeding_rate.");
        }
        //------------------------------------------------
        // Add persons & fill pools & surveyseeding.
        //------------------------------------------------
        auto pop_config_pt = m_config_pt.get_child_optional("run.pop_config");
        if (pop_config_pt) {
            gen::files::GenDirectory dir(m_config_pt, m_rn_manager);
            gen::geogen::Generate(dir, pop);
            gen::popgen::Generate(dir, pop);
        } else {
            SurveySeeder(m_config_pt, m_rn_manager).Seed(MakePoolSys(MakePersons(pop)));
        }

        return pop;
}

} // namespace stride
