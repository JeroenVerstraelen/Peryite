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
 * Implementation for the core ContcatPool class.
 */

#include "ContactPool.h"

#include "pop/Age.h"
#include "pop/Person.h"

namespace stride {

using namespace std;

ContactPool::ContactPool(std::size_t pool_id, ContactPoolType::Id type, util::spherical_point coord)
    : m_pool_id(pool_id), m_pool_type(type), m_index_immune(0), m_members(), m_coord(coord)
{
}

void ContactPool::AddMember(const Person* p)
{
        m_members.emplace_back(const_cast<Person*>(p));
        m_index_immune++;
}

std::tuple<bool, size_t> ContactPool::SortMembers()
{
        bool   infectious_cases = false;
        size_t num_cases        = 0;

        for (size_t i_member = 0; i_member < m_index_immune; i_member++) {
                // if immune, move to back
                if (m_members[i_member]->GetHealth().IsImmune()) {
                        bool   swapped   = false;
                        size_t new_place = m_index_immune - 1;
                        m_index_immune--;
                        while (!swapped && new_place > i_member) {
                                if (m_members[new_place]->GetHealth().IsImmune()) {
                                        m_index_immune--;
                                        new_place--;
                                } else {
                                        swap(m_members[i_member], m_members[new_place]);
                                        swapped = true;
                                }
                        }
                }
                // else, if not susceptible, move to front
                else if (!m_members[i_member]->GetHealth().IsSusceptible()) {
                        if (!infectious_cases && m_members[i_member]->GetHealth().IsInfectious()) {
                                infectious_cases = true;
                        }
                        if (i_member > num_cases) {
                                swap(m_members[i_member], m_members[num_cases]);
                        }
                        num_cases++;
                }
        }
        return std::make_tuple(infectious_cases, num_cases);
}

void ContactPool::AddTraveller(Person* person, unsigned int EndDate) {
        std::vector<unsigned int> ids;
        ids.push_back(person->GetId());
        std::pair<map<unsigned long, std::vector<unsigned int>>::iterator, bool> inMap = m_end_travel_dates.emplace(EndDate, ids);
        if(!std::get<1>(inMap)){
            m_end_travel_dates.at(EndDate).push_back(person->GetId());
        }
        m_members.push_back(person);
}

void ContactPool::UpdateTravel(unsigned int simDay) {
        std::vector<unsigned int> travel_enders = m_end_travel_dates.at(simDay);
        for(unsigned int id : travel_enders) {
                for (auto it = m_members.begin(); it < m_members.end(); it ++) {
                        Person* p = *it;
                        if (p->GetId() == id){
                                m_members.erase(it);

                        }
                }
        }
}

} // namespace stride

