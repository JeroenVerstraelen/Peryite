#pragma once
#include "../../GeoGrid.h"
#include "../../GenConfiguration.h"
#include "../../structs/School.h"
#include <vector>
#include "pop/Population.h"


namespace stride {
namespace gen {
namespace geogen {
namespace builder {

/// Builds a list of schools using the Geogrid and some configuration values.
/// @param config       The configuration file that specifies parameters used in building schools.
/// @param locations    The geogrid that the schools will be mapped to.
/// @return             A vector of newly created schools.
std::vector<std::shared_ptr<School>> BuildSchools(const GenConfiguration& config, GeoGrid& locations, std::shared_ptr<Population> population, unsigned int firstPerson);

} // namespace builder
} // namespace geogen
} // namespace gen
} // namespace stride
