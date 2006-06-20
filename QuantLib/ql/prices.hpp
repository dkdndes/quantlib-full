/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2006 Joseph Wang

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

/*! \file prices.hpp
    \brief price classes
*/

#ifndef quantlib_prices_hpp
#define quantlib_prices_hpp

#include <ql/timeseries.hpp>

namespace QuantLib {

    //! interval price
    class IntervalPrice {
      public:
        enum Type { Open, Close, High, Low };

        IntervalPrice();
        IntervalPrice(Real open, Real close, Real high, Real low);

        //! \name Inspectors
        //@{
        Real open() const { return open_; }
        Real close() const { return close_; }
        Real high() const { return high_; }
        Real low() const { return low_; }
        Real value(IntervalPrice::Type) const;
        //@}

        //! \name Modifiers
        //@{
        void setValue(Real value, IntervalPrice::Type);
        void setValues(Real open, Real close, Real high, Real low);
        //@}

        //! \name Helper functions
        //@{
        static TimeSeries<IntervalPrice> makeSeries(
                                               const std::vector<Date>& d,
                                               const std::vector<Real>& open,
                                               const std::vector<Real>& close,
                                               const std::vector<Real>& high,
                                               const std::vector<Real>& low);
        static std::vector<Real> extractValues(
                                          const TimeSeries<IntervalPrice>&,
                                          IntervalPrice::Type);
        static TimeSeries<Real> extractComponent(
                                          const TimeSeries<IntervalPrice>&,
                                          enum IntervalPrice::Type);
        //@}
      private:
        Real open_, close_, high_, low_;
    };

}


#endif
