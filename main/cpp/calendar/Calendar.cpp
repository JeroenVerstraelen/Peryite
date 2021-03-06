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
 * Implementation file for the Calendar class.
 */

#include "Calendar.h"

#include "util/FileSys.h"

#include <boost/property_tree/json_parser.hpp>

namespace stride {

using namespace std;
using namespace boost::filesystem;
using namespace boost::property_tree::json_parser;
using namespace stride::util;
using boost::property_tree::ptree;

Calendar::Calendar(const ptree& configPt) : m_date(), m_day(0U), m_holidays(), m_school_holidays()
{
        // Set start date
        m_date = boost::gregorian::from_simple_string(configPt.get<string>("run.start_date", "2016-01-01"));

        // Set holidays & school holidays
        InitializeHolidays(configPt);
}

void Calendar::AdvanceDay()
{
        m_day++;
        m_date = m_date + boost::gregorian::date_duration(1);
}

void Calendar::InitializeHolidays(const ptree& configPt)
{
        // Load json file
        ptree holidaysPt;
        {
                const string fName{configPt.get<string>("run.holidays_file", "holidays_flanders_2017.json")};
                const path   fPath{FileSys::GetDataDir() /= fName};
                if (!is_regular_file(fPath)) {
                        throw runtime_error(string(__func__) + "Holidays file " + fPath.string() + " not present.");
                }
                read_json(fPath.string(), holidaysPt);
        }

        // Read in holidays
        for (int i = 1; i < 13; i++) {
                const auto month = to_string(i);
                const auto year  = holidaysPt.get<string>("year", "2017");
                const auto lead  = string(year).append("-").append(month).append("-");

                // read in general holidays
                for (const auto& date : holidaysPt.get_child("general." + month)) {
                        const auto d = string(lead).append(date.second.get_value<string>());
                        m_holidays.push_back(boost::gregorian::from_simple_string(d));
                }

                // read in school holidays
                for (const auto& date : holidaysPt.get_child("school." + month)) {
                        const string d = string(lead).append(date.second.get_value<string>());
                        m_school_holidays.push_back(boost::gregorian::from_simple_string(d));
                }
        }
}

} // namespace stride
