#include "UniversityAssigner.h"
#include "../../structs/University.h"
#include "../PopGenerator.h"
#include "trng/fast_discrete_dist.hpp"

namespace stride {
namespace gen {
namespace popgen {
namespace assigner {

using namespace std;
using namespace gen;
using namespace util;

unsigned int AssignUniversities(
        vector<vector<shared_ptr<GenStruct>>> &universities, const shared_ptr<Population> population,
        shared_ptr<Region> region, const GeoGrid &grid) {
    // -------------
    // Contactpools
    // -------------
    auto config                             = region->config;
    const auto university_size      = config.GetTree().get<unsigned int>("university_size");
    const auto university_cp_size   = config.GetTree().get<unsigned int>("university_cp_size");
    unsigned int cp_id                      = region->last_cps[ContactPoolType::Id::School];
    map<unsigned int, vector<shared_ptr<University>>> cities;
    for (const auto &band : universities) {
        for (const auto &g_struct : band) {
            auto university = std::static_pointer_cast<University>(g_struct);
            auto coord      = university->coordinate;
            // Create the contactpools for every university
            for (unsigned int size = 0; size < university_size; size += university_cp_size) {
                auto pool = make_shared<ContactPool>(cp_id, ContactPoolType::Id::School, coord);
                university->pools.push_back(pool);
                cp_id++;
            }
            // Get the different university locations (urban_id = row_index in commuting data).
            if (cities.find(university->urban_id) == cities.end()) {
                cities[university->urban_id] = vector<shared_ptr<University>>({university});
            } else {
                cities[university->urban_id].push_back(university);
            }
        }
    }
    region->last_cps[ContactPoolType::Id::School] = cp_id-1;

    // -------------
    // Distributions
    // -------------
    auto rn_manager = config.GetRNManager();

    // Create two distributions, one to select if the person is a student and one to select if the student commutes.
    auto student_fraction   = config.GetTree().get<double>("university.student_fraction");
    auto commute_fraction   = config.GetTree().get<double>("university.commute_fraction");
    auto student_fractions  = vector<double>{student_fraction, 1.0 - student_fraction};
    auto commute_fractions  = vector<double>{commute_fraction, 1.0 - commute_fraction};
    auto student_gen        = rn_manager->GetGenerator(
            trng::fast_discrete_dist(student_fractions.begin(), student_fractions.end()));
    auto commute_gen        = rn_manager->GetGenerator(
            trng::fast_discrete_dist(commute_fractions.begin(), commute_fractions.end()));

    // create a uniform distribution to select a contactpool from a university
    auto cp_gen = rn_manager->GetGenerator(
            trng::fast_discrete_dist(uint(floor(university_size / university_cp_size))));

    // Commuting distributions
    util::CSV commuting_data(config.GetTree().get<string>("geoprofile.commuters"));

    map<unsigned int, std::function<int()>> city_generators;
    vector<unsigned int> commute_towards;
    unsigned int commute_towards_total = 0;
    if (commuting_data.size() > 1) {
        for (auto const &city : cities) {
            std::stringstream templabel;
            templabel << "id_" << city.first;
            string label = templabel.str();
            // For every university city, calculate the fraction commuting towards it.
            auto row = *(commuting_data.begin() + commuting_data.GetIndexForLabel(label));
            unsigned int city_total = 0;
            for (unsigned int col_index = 0; col_index < commuting_data.GetColumnCount(); col_index++) {
                // Ignore commuting towards itself
                if (city.first == col_index)
                    continue;
                auto commuting_towards = row.GetValue<unsigned int>(col_index);
                // Count the total number of people commuting towards this university city
                city_total += commuting_towards;
                // Count the total number of people commuting towards all university cities
                commute_towards_total += commuting_towards;
            }
            commute_towards.push_back(city_total);
            // For every university city, create a uniform distribution to select a university
            auto uni_gen = rn_manager->GetGenerator(trng::fast_discrete_dist(uint(city.second.size())));
            city_generators[city.first] = uni_gen;
        }
    }

    // Create a distribution to select a university city when the student commutes.
    vector<double> city_fractions;
    for (auto const &commute_towards_city : commute_towards)
        city_fractions.push_back(commute_towards_city / commute_towards_total);
    auto city_gen = rn_manager->GetGenerator(
            trng::fast_discrete_dist(city_fractions.begin(), city_fractions.end()));

    // --------------------------------
    // Assign students to universities.
    // --------------------------------
    unsigned int total_commuting_students = 0;
    for (unsigned int i = region->first_person_id; i <= region->last_person_id; i++) {
        auto &person = population->at(i);
        auto age = person.GetAge();
        if (age >= 18 && age < 26 && student_gen() == 0) {
            shared_ptr<ContactPool> pool;
            if (commute_gen() == 0) {
                /// Commuting student
                total_commuting_students++;
                auto city_index = uint(city_gen());
                auto it = cities.begin();
                std::advance(it, city_index);
                auto city = *it;
                auto university = city.second[city_generators[city.first]()];
                pool = university->pools[cp_gen()];
            } else {
                /// Non-commuting student
                auto home_coord = person.GetCoordinate();
                // Find the closest schools
                std::vector<shared_ptr<GenStruct>> closest_universities = GetClosestStructs(home_coord,
                                                                                            universities,
                                                                                            grid);
                if (closest_universities.empty()) {
                    continue;
                }
                // Create a uniform distribution to select a university
                //auto rn_manager = config.GetRNManager();
                auto uni_gen = rn_manager->GetGenerator(
                        trng::fast_discrete_dist(uint(closest_universities.size())));
                auto university = static_pointer_cast<University>(closest_universities[uni_gen()]);
                // Create a uniform distribution to select a contactpool in the selected university
                auto cp_generator = rn_manager->GetGenerator(
                        trng::fast_discrete_dist(uint(university->pools.size())));
                pool = university->pools.at(uint(cp_generator()));
            }
            person.setPoolId(ContactPoolType::Id::School, uint(pool->GetId()));
            pool->AddMember(&person);
        }
    }
    return total_commuting_students;
}

} // assigner
} // popgen
} // namespace gen
} // namespace stride

