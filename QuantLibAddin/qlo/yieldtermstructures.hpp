/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2005, 2006, 2007 Eric Ehlers
 Copyright (C) 2006, 2007 Ferdinando Ametrano
 Copyright (C) 2005 Plamen Neykov
 Copyright (C) 2005 Aurelien Chanudet

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#ifndef qla_yieldtermstructures_hpp
#define qla_yieldtermstructures_hpp

#include <qlo/termstructures.hpp>

#include <ql/time/frequency.hpp>
#include <ql/compounding.hpp>
#include <ql/types.hpp>

namespace QuantLib {
    class Calendar;
    class DayCounter;
    class Date;
    class Quote;

    class YieldTermStructure;

    template<class TS>
    class BootstrapHelper;

    typedef BootstrapHelper<YieldTermStructure> RateHelper;

    template <class T>
    class Handle;
}

namespace QuantLibAddin {
     
    OH_OBJ_CLASS(YieldTermStructure, TermStructure);

    class DiscountCurve : public YieldTermStructure {
      public:
        DiscountCurve(
            const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
            const std::vector<QuantLib::Date>& dates,
            const std::vector<QuantLib::DiscountFactor>& dfs,
            const QuantLib::DayCounter& dayCounter,
            bool permanent);
    };

    class ZeroCurve : public YieldTermStructure {
      public:
        ZeroCurve(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                  const std::vector<QuantLib::Date>& dates,
                  const std::vector <QuantLib::Rate>& zeroRates,
                  const QuantLib::DayCounter& dayCounter,
                  bool permanent);
    };

    class ForwardCurve : public YieldTermStructure {
      public:
        ForwardCurve(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                     const std::vector<QuantLib::Date>& dates,
                     const std::vector <QuantLib::Rate>& forwardRates,
                     const QuantLib::DayCounter& dayCounter,
                     bool permanent);
    };

    class FlatForward : public YieldTermStructure {
      public:
        FlatForward(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                    QuantLib::Size nDays,
                    const QuantLib::Calendar& calendar,
                    QuantLib::Rate forward,
                    const QuantLib::DayCounter& dayCounter,
                    QuantLib::Compounding compounding,
                    QuantLib::Frequency frequency,
                    bool permanent);
    };

    class ForwardSpreadedTermStructure : public YieldTermStructure {
      public:
        ForwardSpreadedTermStructure(
            const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
            const QuantLib::Handle<QuantLib::YieldTermStructure>& hYTS,
            const QuantLib::Handle<QuantLib::Quote>& spread,
            bool permanent);
    };

    class ImpliedTermStructure : public YieldTermStructure {
      public:
        ImpliedTermStructure(
            const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
            const QuantLib::Handle<QuantLib::YieldTermStructure>& hYTS,
            const QuantLib::Date& referenceDate,
            bool permanent);
    };

}

#endif

