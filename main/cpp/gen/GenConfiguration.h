#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/filesystem/path.hpp>
#include "util/CSV.h"
#include "util/RNManager.h"

namespace stride {
namespace gen {

#define AMOUNTOFBANDS 50

/**
 * The configuration for the generator component.
 *
 **/
class GenConfiguration
{
public:
    /// Default constructor
    GenConfiguration() {}

    /// Constructor. Constructs the GenConfiguration object using the config.
    /// file.
    /// @param config_pt       The configuration property tree to use.
    /// @param thread_count    The amount of threads to be used for parallelization.
    /// @param output_prefix   The prefix used for output files.
    /// @param rn_manager      The random number manager to be used by the geopop module.
    GenConfiguration(
        boost::property_tree::ptree config_pt, unsigned int thread_count,
        std::string output_prefix, std::shared_ptr<util::RNManager> rn_manager);

    /// Returns the property tree that this class wraps.
    /// @return                 The ptree wrapped by this class.
    boost::property_tree::ptree GetTree() const;

    /// Returns the output prefix that the general configuration describes.
    /// The outputprefix is the prefix (directory name or prefix to filename)
    /// to be used in the filepath of all output files.
    /// @return                 The output prefix.
    std::string GetOutputPrefix() const;

    /// Returns the amount of threads to be used.
    /// @return                 The amount of threads.
    unsigned int GetNumThreads() const;

    /// Returns the RNManager that manages the generation of random numbers.
    /// @return                 The rn manager.
    std::shared_ptr<util::RNManager> GetRNManager() const;
private:
    /// The configuration in a tree structure
    boost::property_tree::ptree m_config;
    /// The prefix for the output files
    std::string m_output_prefix;
    /// The number of threads to be used
    unsigned int m_num_threads;
    /// Manages the parallel generation of random numbers
    std::shared_ptr<util::RNManager> m_rn_manager;

    /// Checks if the configuration property tree is valid.
    void CheckValidConfig() const;
};

} // namespace gen
} // namespace stride

