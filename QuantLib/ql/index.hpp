/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2000, 2001, 2002, 2003 RiskMap srl
 Copyright (C) 2003, 2004, 2005, 2006 StatPro Italia srl

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/reference/license.html>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file index.hpp
    \brief purely virtual base class for indexes
*/

#ifndef quantlib_index_hpp
#define quantlib_index_hpp

//#include <ql/calendar.hpp>
//#include <ql/currency.hpp>
//#include <ql/daycounter.hpp>
#include <ql/Indexes/indexmanager.hpp>

namespace QuantLib {

    //! purely virtual base class for indexes
    class Index : public Observable {
      public:
        virtual ~Index() {}
        //! Returns the name of the index.
        /*! \warning This method is used for output and comparison
                     between indexes. It is <b>not</b> meant to be
                     used for writing switch-on-type code.

            \todo add methods returning InterestRate
        */
        virtual std::string name() const = 0;
        //! returns TRUE if the fixing date is a valid one
        virtual bool isValidFixingDate(const Date& fixingDate) const = 0;
        //! returns the fixing at the given date
        /*! the date passed as arguments must be the actual calendar
            date of the fixing; no settlement days must be used.
        */
        virtual Real fixing(const Date& fixingDate,
                            bool forecastTodaysFixing = false) const = 0;
        //! stores the historical fixing at the given date
        /*! the date passed as arguments must be the actual calendar
            date of the fixing; no settlement days must be used.
        */
        void addFixing(const Date& fixingDate,
			           Real fixing);
        //! stores historical fixings at the given dates
        /*! the dates passed as arguments must be the actual calendar
            dates of the fixings; no settlement days must be used.
        */
        template <class DateIterator, class ValueIterator>
        void addFixings(DateIterator dBegin, DateIterator dEnd,
                        ValueIterator vBegin) {
            std::string tag = name();
            TimeSeries<Real> h = IndexManager::instance().getHistory(tag);
            while (dBegin != dEnd) {
                QL_REQUIRE(isValidFixingDate(*dBegin),
                           "Fixing date " << (*dBegin).weekday() << ", " <<
                           *dBegin << " is not valid");
                h[*(dBegin++)] = *(vBegin++);
            }
            IndexManager::instance().setHistory(tag,h);
        }
        //! clears all stored historical fixings
        void clearFixings();
    };

}

#endif
