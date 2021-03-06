#pragma once
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
 * Header file of base class for config that needs to be read from a file.
 */

#include "CSVRow.h"

#include "util/StringUtils.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <fstream>
#include <vector>

namespace stride {
namespace util {

/**
 * A collection of CSVRow elements. Iterate with begin and end like STL containers.
 */
class CSV : protected std::vector<CSVRow>
{
public:
        using std::vector<CSVRow>::begin;
        using std::vector<CSVRow>::end;
        using std::vector<CSVRow>::size;

public:
        /// Initialize from file. If optLabels not specied, the file is required. Otherwise initialize like second
        /// constructor.
        explicit CSV(const boost::filesystem::path& path, std::initializer_list<std::string> optLabels = {});

        /// Initialize with header.
        CSV(std::initializer_list<std::string> labels);

        /// Default constructor. Mainly used for swig.
        CSV() = default;

        /// Compare operator.
        bool operator==(const CSV& other) const;

        /// Add row of values. Will all be converted to string with StringUtils::ToString
        template <typename... T>
        void AddRow(const T&... values);

        /// Add row of string values.
        void AddRow(std::vector<std::string> values);

        /// Amount of columns in the CSV.
        size_t GetColumnCount() const { return columnCount; }

        /// Convert label to index for more user friendly and robuust implementation. This level of indirection does
        /// introduce a perfomance tradeoff.
        size_t GetIndexForLabel(const std::string& label) const;

        /// Write CSV to file.
        void Write(const boost::filesystem::path& path) const;

protected:
        std::vector<std::string> labels;
        size_t                   columnCount = 0;
};

template <typename... T>
inline void CSV::AddRow(const T&... values)
{
        AddRow({ToString(values)...});
}

} // namespace util
} // namespace stride
