
/*
 * Copyright (C) 2000-2001 QuantLib Group
 *
 * This file is part of QuantLib.
 * QuantLib is a C++ open source library for financial quantitative
 * analysts and developers --- http://quantlib.org/
 *
 * QuantLib is free software and you are allowed to use, copy, modify, merge,
 * publish, distribute, and/or sell copies of it under the conditions stated
 * in the QuantLib License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the license for more details.
 *
 * You should have received a copy of the license along with this file;
 * if not, please email quantlib-users@lists.sourceforge.net
 * The license is also available at http://quantlib.org/LICENSE.TXT
 *
 * The members of the QuantLib Group are listed in the Authors.txt file, also
 * available at http://quantlib.org/group.html
*/

/*! \file target.hpp
    \brief TARGET calendar

    \fullpath
    ql/Calendars/%target.hpp
*/

// $Id$

#ifndef quantlib_target_calendar_h
#define quantlib_target_calendar_h

#include <ql/calendar.hpp>

namespace QuantLib {

    namespace Calendars {

        //! %TARGET calendar
        /*! Holidays:
            <ul>
            <li>Saturdays</li>
            <li>Sundays</li>
            <li>New Year's Day, January 1st</li>
            <li>Good Friday</li>
            <li>Easter Monday</li>
            <li>Labour Day, May 1st</li>
            <li>Christmas, December 25th</li>
            <li>Day of Goodwill, December 26th</li>
            </ul>
        */
        class TARGET : public Calendar {
          private:
            class EurCalendarFactory : public Calendar::factory {
              public:
                Calendar create() const { return TARGET(); }
            };
            class EurCalendarImpl : public Calendar::WesternCalendarImpl {
              public:
                std::string name() const { return "TARGET"; }
                bool isBusinessDay(const Date&) const;
            };
          public:
            TARGET()
            : Calendar(Handle<CalendarImpl>(new EurCalendarImpl)) {}
            //! returns a factory of %TARGET calendars
            Handle<factory> getFactory() const {
                return Handle<factory>(new EurCalendarFactory);
            }
        };

    }

}


#endif
