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
 *  Copyright 2017, Kuylen E, Willem L, Broeckhove J
 */
/**
 * @file
 * Implementation of ptree utils.
 */

#include "RunConfigManager.h"

#include "util/ConfigInfo.h"
#include "util/FileSys.h"

#include <boost/property_tree/xml_parser.hpp>
#include <initializer_list>
#include <map>
#include <sha1.h>
#include <sstream>
#include <string>

using namespace boost::property_tree;
using namespace boost::property_tree::xml_parser;
using namespace std;

namespace stride {
namespace util {

void RunConfigManager::CleanConfigFile(ptree pt)
{
        pt.sort();
        const string sha1  = RunConfigManager::ToShortSha1(pt);
        const string fName = sha1 + ".xml";
        cerr << "Rewriting config to file " << fName << " in current directory." << endl;
        FileSys::WritePtreeFile(fName, pt);
}

ptree RunConfigManager::Create(const std::string& configName)
{
        static map<string, string (*)()> creators{
            // clang-format off
                make_pair("BenchInfluenza",  &CreateBenchInfluenza),
                make_pair("BenchMeasles",    &CreateBenchMeasles),
                make_pair("Default",         &CreateDefault),
                make_pair("TestsInfluenza",  &CreateTestsInfluenza),
                make_pair("TestsMeasles",    &CreateTestsMeasles)
        }; // clang-format on
        return FromString(creators.at(configName)());
}

string RunConfigManager::CreateBenchInfluenza()
{
        return R"###(
<?xml version="1.0" encoding="utf-8"?>
<run>
        <age_contact_matrix_file>contact_matrix_flanders_subpop.xml</age_contact_matrix_file>
        <behaviour_policy>NoBehaviour</behaviour_policy>
        <belief_policy>
                <name>NoBelief</name>
        </belief_policy>
        <contact_log_level>None</contact_log_level>
        <contact_output_file>false</contact_output_file>
        <disease_config_file>disease_influenza.xml</disease_config_file>
        <global_information_policy>NoGlobalInformation</global_information_policy>
        <holidays_file>holidays_none.json</holidays_file>
        <immunity_link_probability>0</immunity_link_probability>
        <immunity_profile>Random</immunity_profile>
        <immunity_rate>0</immunity_rate>
        <local_information_policy>NoLocalInformation</local_information_policy>
        <num_days>30</num_days>
        <num_participants_survey>10</num_participants_survey>
        <num_threads>1</num_threads>
        <output_prefix>BenchRuns</output_prefix>
        <population_file>pop_double_flanders.csv</population_file>
        <region_file>region_double_flanders.csv</region_file>
        <poolsys_file>poolsys_double_flanders.csv</poolsys_file>
        <rng_seed>343869</rng_seed>
        <rng_type>mrg2</rng_type>
        <r0>3</r0>
        <seeding_rate>0.001</seeding_rate>
        <seeding_age_min>1</seeding_age_min>
        <seeding_age_max>99</seeding_age_max>
        <start_date>2017-01-01</start_date>
        <stride_log_level>info</stride_log_level>
        <track_index_case>false</track_index_case>
        <use_install_dirs>true</use_install_dirs>
        <vaccine_profile>None</vaccine_profile>
</run>
        )###";
}

string RunConfigManager::CreateDefault()
{
        return R"###(
<?xml version="1.0" encoding="utf-8"?>
<run>
        <age_contact_matrix_file>contact_matrix_flanders_subpop.xml</age_contact_matrix_file>
        <behaviour_policy>NoBehaviour</behaviour_policy>
        <belief_policy>
                <name>NoBelief</name>
        </belief_policy>
        <contact_log_level>Transmissions</contact_log_level>
        <contact_output_file>true</contact_output_file>
        <disease_config_file>disease_measles.xml</disease_config_file>
        <global_information_policy>NoGlobalInformation</global_information_policy>
        <holidays_file>holidays_none.json</holidays_file>
        <immunity_profile>None</immunity_profile>
        <immunity_rate>0.8</immunity_rate>
        <local_information_policy>NoLocalInformation</local_information_policy>
        <num_days>30</num_days>
        <num_participants_survey>10</num_participants_survey>
        <num_threads>1</num_threads>
        <output_adopted>false</output_adopted>
        <output_cases>true</output_cases>
        <output_persons>false</output_persons>
        <output_prefix></output_prefix>
        <output_summary>false</output_summary>
        <population_file>pop_double_flanders.csv</population_file>
        <region_file>region_double_flanders.csv</region_file>
        <poolsys_file>poolsys_double_flanders.csv</poolsys_file>
        <rng_seed>1</rng_seed>
        <rng_type>mrg2</rng_type>
        <r0>11</r0>
        <seeding_age_max>99</seeding_age_max>
        <seeding_age_min>1</seeding_age_min>
        <seeding_rate>0.003</seeding_rate>
        <start_date>2017-01-01</start_date>
        <stride_log_level>critical</stride_log_level>
        <track_index_case>false</track_index_case>
        <use_install_dirs>true</use_install_dirs>
        <vaccine_link_probability>0</vaccine_link_probability>
        <vaccine_profile>Random</vaccine_profile>
        <vaccine_rate>0.8</vaccine_rate>
</run>
        )###";
}

string RunConfigManager::CreateBenchMeasles()
{
        return R"###(
<?xml version="1.0" encoding="utf-8"?>
<run>
        <age_contact_matrix_file>contact_matrix_flanders_subpop.xml</age_contact_matrix_file>
        <behaviour_policy>NoBehaviour</behaviour_policy>
        <belief_policy>
                <name>NoBelief</name>
        </belief_policy>
        <contact_log_level>None</contact_log_level>
        <contact_output_file>false</contact_output_file>
        <disease_config_file>disease_measles.xml</disease_config_file>
        <global_information_policy>NoGlobalInformation</global_information_policy>
        <holidays_file>holidays_none.json</holidays_file>
        <immunity_profile>None</immunity_profile>
        <immunity_rate>0.01</immunity_rate>
        <local_information_policy>NoLocalInformation</local_information_policy>
        <num_days>30</num_days>
        <num_participants_survey>10</num_participants_survey>
        <num_threads>1</num_threads>
        <output_prefix>bench</output_prefix>
        <population_file>pop_double_flanders.csv</population_file>
        <region_file>region_double_flanders.csv</region_file>
        <poolsys_file>poolsys_double_flanders.csv</poolsys_file>
        <rng_seed>1</rng_seed>
        <rng_type>mrg2</rng_type>
        <r0>15</r0>
        <seeding_age_max>99</seeding_age_max>
        <seeding_age_min>1</seeding_age_min>
        <seeding_rate>0.02</seeding_rate>
        <start_date>2017-01-01</start_date>
        <stride_log_level>critical</stride_log_level>
        <track_index_case>false</track_index_case>
        <use_install_dirs>true</use_install_dirs>
        <vaccine_link_probability>0</vaccine_link_probability>
        <vaccine_profile>Random</vaccine_profile>
        <vaccine_rate>0</vaccine_rate>
</run>
        )###";
}

vector<unsigned int> RunConfigManager::CreateNumThreads(unsigned int max)
{
        max = std::max(max, ConfigInfo::NumberAvailableThreads());
        initializer_list<unsigned int> num{1U};

        if (max >= 2) {
                num = {1U, 2U};
        }
        if (max >= 4) {
                num = {1U, 2U, 3U, 4U};
        }
        if (max >= 8) {
                num = {1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U};
        }
        if (max >= 12) {
                num = {1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 10U, 12U};
        }
        if (max >= 16) {
                num = {1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 10U, 12U, 16U};
        }

        return num;
}

string RunConfigManager::CreateTestsInfluenza()
{
        return R"###(
<?xml version="1.0" encoding="utf-8"?>
<run>
        <age_contact_matrix_file>contact_matrix_flanders_subpop.xml</age_contact_matrix_file>
        <behaviour_policy>NoBehaviour</behaviour_policy>
        <belief_policy>
                <name>NoBelief</name>
        </belief_policy>
        <contact_log_level>None</contact_log_level>
        <contact_output_file>false</contact_output_file>
        <disease_config_file>disease_influenza.xml</disease_config_file>
        <global_information_policy>NoGlobalInformation</global_information_policy>
        <holidays_file>holidays_none.json</holidays_file>
        <immunity_link_probability>0</immunity_link_probability>
        <immunity_profile>Random</immunity_profile>
        <immunity_rate>0</immunity_rate>
        <local_information_policy>NoLocalInformation</local_information_policy>
        <num_days>30</num_days>
        <num_participants_survey>10</num_participants_survey>
        <num_threads>1</num_threads>
        <output_prefix>BenchRuns</output_prefix>
        <population_file>pop_double_flanders.csv</population_file>
        <region_file>region_double_flanders.csv</region_file>
        <poolsys_file>poolsys_double_flanders.csv</poolsys_file>
        <rng_seed>2015</rng_seed>
        <rng_type>mrg2</rng_type>
        <r0>3</r0>
        <seeding_rate>0.00089999999999999998</seeding_rate>
        <seeding_age_min>1</seeding_age_min>
        <seeding_age_max>99</seeding_age_max>
        <start_date>2017-01-01</start_date>
        <stride_log_level>info</stride_log_level>
        <track_index_case>false</track_index_case>
        <use_install_dirs>true</use_install_dirs>
        <vaccine_profile>None</vaccine_profile>
</run>
        )###";
}

string RunConfigManager::CreateTestsMeasles()
{
        return R"###(
<?xml version="1.0" encoding="utf-8"?>
<run>
        <age_contact_matrix_file>contact_matrix_flanders_subpop.xml</age_contact_matrix_file>
        <behaviour_policy>NoBehaviour</behaviour_policy>
        <belief_policy>
                <name>NoBelief</name>
        </belief_policy>
        <contact_log_level>Transmissions</contact_log_level>
        <contact_output_file>false</contact_output_file>
        <disease_config_file>disease_measles.xml</disease_config_file>
        <global_information_policy>NoGlobalInformation</global_information_policy>
        <holidays_file>holidays_none.json</holidays_file>
        <immunity_rate>0.80000000000000004</immunity_rate>
        <immunity_profile>None</immunity_profile>
        <local_information_policy>NoLocalInformation</local_information_policy>
        <num_days>50</num_days>
        <num_participants_survey>10</num_participants_survey>
        <num_threads>1</num_threads>
        <output_prefix>BenchRuns</output_prefix>
        <population_file>pop_double_flanders.csv</population_file>
        <region_file>region_double_flanders.csv</region_file>
        <poolsys_file>poolsys_double_flanders.csv</poolsys_file>
        <rng_seed>1</rng_seed>
        <rng_type>mrg2</rng_type>
        <seeding_age_max>99</seeding_age_max>
        <seeding_age_min>1</seeding_age_min>
        <seeding_rate>0.002</seeding_rate>
        <start_date>2017-01-01</start_date>
        <stride_log_level>info</stride_log_level>
        <track_index_case>false</track_index_case>
        <use_install_dirs>true</use_install_dirs>
        <vaccine_link_probability>0</vaccine_link_probability>
        <vaccine_profile>Random</vaccine_profile>
        <vaccine_rate>0.80000000000000004</vaccine_rate>
</run>
        )###";
}

ptree RunConfigManager::FromString(const string& s)
{
        ptree         pt;
        istringstream is(s);
        read_xml(is, pt);
        return pt;
}

string RunConfigManager::ToString(const ptree& pt)
{
        ostringstream ss;
        write_xml(ss, pt, xml_writer_make_settings<ptree::key_type>(' ', 8));
        return ss.str();
}

std::string RunConfigManager::ToSha1(const boost::property_tree::ptree& pt) { return sha1(ToString(pt)); }

std::string RunConfigManager::ToShortSha1(const boost::property_tree::ptree& pt, unsigned int n)
{
        return ToSha1(pt).substr(0, n);
}

} // namespace util
} // namespace stride
