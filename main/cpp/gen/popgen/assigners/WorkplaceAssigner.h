#include "pop/Population.h"
#include "../../GeoGrid.h"
#include "../../GenConfiguration.h"
#include "util/Regions.h"
#include <vector>

namespace stride {
namespace gen {
namespace popgen {
namespace assigner {

/// Builds a set of households for the population.
/// @param workplaces               The workplaces that persons can be asssigned to.
/// @param population               The persons that will be assigned to workplaces.
/// @param region                   the region that the workplaces will be assigned in.
/// @param grid                     The geogrid that was used to build the schools.
/// @param total_commuting_students The total number of students that are commuting to schools/universities.
void AssignWorkplaces(
    std::vector<std::vector<std::shared_ptr<GenStruct>>>& workplaces,
    const std::shared_ptr<Population> population,
    std::shared_ptr<util::Region> region, const GeoGrid& grid,
    unsigned int total_commuting_students);

} // assigner
} // popgen
} // namespace gen
} // namespace stride
