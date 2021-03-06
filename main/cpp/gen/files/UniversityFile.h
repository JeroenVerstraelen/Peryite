#pragma once
#include "GenFile.h"
#include "../structs/University.h"

namespace stride {
namespace gen {
namespace files {

/**
 * A class that can read and write university data files.
 */
class UniversityFile : public GenFile
{
public:
    /// Constructor. Constructs the UniversityFile object using the config.
    /// @param config           The geopop configuration to be used.
    /// @param suffix           The suffix to use for the file name.
    UniversityFile(GenConfiguration& config, std::string suffix) : GenFile(config, suffix) {}

    /// Constructor. Constructs the UniversityFile object with an existing vector of universities.
    /// @param config           The geopop configuration to be used.
    /// @param structs          The universities that the file will contain.
    /// @param geo              The geogrid that was used to build the universities.
    /// @param suffix           The suffix to use for the file name.
    UniversityFile(GenConfiguration& config, std::vector<std::shared_ptr<GenStruct>> structs, GeoGrid& geo, std::string suffix)
    : GenFile(config, structs, geo, suffix)
    {
        m_file_name = "Universities";
        m_labels = {"id", "urban_id", "latitude","longitude","band"};
    }

private:
    /// Returns the university associated with a row in the csv data.
    /// @param row              The row that describes the university.
    /// @return                 The university associated with the csv row.
    std::shared_ptr<GenStruct> GetStruct(util::CSVRow const & row)
    {
        auto university = std::make_shared<University>(University(
            row.GetValue<unsigned int>("id"),
            row.GetValue<unsigned int>("urban_id"),
            util::spherical_point(
                row.GetValue<double>("latitude"),
                row.GetValue<double>("longitude")
                )
            )
        );
        return university;
    }

    /// Returns the values of a university in string format.
    /// @param g_struct         The university that contains the values.
    /// @return                 Vector of the values of the university in string format.
    std::vector<std::string> GetValues(std::shared_ptr<GenStruct> g_struct)
    {
        std::shared_ptr<University> university = std::static_pointer_cast<University>(g_struct);
        std::vector<std::string> values = {
            std::to_string(university->id),
            std::to_string(university->urban_id),
            std::to_string(university->coordinate.get<0>()),
            std::to_string(university->coordinate.get<1>())
        };
        return values;
    }
};

typedef std::shared_ptr<UniversityFile> UniversityFilePtr;

} // namespace files
} // namespace gen
} // namespace stride
