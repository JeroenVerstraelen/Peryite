#include "CommunitiesBuilder.h"
#include "trng/fast_discrete_dist.hpp"

namespace stride {
namespace gen {
namespace geogen {
namespace builder {

using namespace std;
using namespace util;
using namespace trng;

vector<shared_ptr<Community>> BuildCommunities(const GenConfiguration& config, GeoGrid& grid, shared_ptr<Population>& population, unsigned int firstPerson)
{
    vector<shared_ptr<Community>> communities = vector<shared_ptr<Community>>();
    auto total_population = config.GetTree().get<unsigned int>("population_size");
    // Every community has an average of 2000 members.
    auto community_count =  uint((population->size() - firstPerson) / 2000);
    // Create the discrete distribution to sample from.
    vector<double> fractions;
    for(const auto& center : grid) {
        fractions.push_back(double(center->population) / double(total_population));
    }

    // The RNManager allows for parallelization.
    auto rn_manager = config.GetRNManager();
    auto generator = rn_manager->GetGenerator(trng::fast_discrete_dist(fractions.begin(), fractions.end()));

    // Create and map the communities to their samples.
    for (unsigned int i = 0; i < community_count; i++) {
        auto center = grid.at(uint(generator()));
        auto coords = center->coordinate;
        if (center->is_fragmented) {
            // Select one of the fragments
            vector<double> f_fractions;
            for(const auto& center_population : center->fragmented_populations)
                f_fractions.push_back(double(center_population) / double(center->population));
            auto frag_gen = rn_manager->GetGenerator(trng::fast_discrete_dist(f_fractions.begin(), f_fractions.end()));
            coords = center->fragmented_coords.at(uint(frag_gen()));
        }
        auto community           = make_shared<Community>(Community(i, true, coords));
        auto community_secondary = make_shared<Community>(Community(i+community_count, false, coords));
        communities.push_back(community);
        communities.push_back(community_secondary);
    }

    return communities;
}

} // namespace builder
} // namespace geogen
} // namespace gen
} // namespace stride
