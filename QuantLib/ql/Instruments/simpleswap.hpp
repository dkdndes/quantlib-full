/*
 Copyright (C) 2000, 2001, 2002 RiskMap srl

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it under the
 terms of the QuantLib license.  You should have received a copy of the
 license along with this program; if not, please email ferdinando@ametrano.net
 The license is also available online at http://quantlib.org/html/license.html

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/
/*! \file simpleswap.hpp
    \brief Simple fixed-rate vs Libor swap

    \fullpath
    ql/Instruments/%simpleswap.hpp
*/

// $Id$

#ifndef quantlib_simple_swap_h
#define quantlib_simple_swap_h

#include <ql/Instruments/swap.hpp>
#include <ql/Indexes/xibor.hpp>
#include <ql/CashFlows/cashflowvectors.hpp>

namespace QuantLib {

    namespace Instruments {

        //! Simple fixed-rate vs Libor swap
        class SimpleSwap : public Swap {
          public:
            SimpleSwap(bool payFixedRate,
                // dates
                const Date& startDate, int n, TimeUnit units,
                const Calendar& calendar,
                RollingConvention rollingConvention,
                /* nominals (if the vector length is lower than the number
                   of coupons, the last nominal will prevail for the
                   remaining coupons)
                */
                const std::vector<double>& nominals,
                // fixed leg
                int fixedFrequency,
                /* fixed coupon rates (if the vector length is lower than
                   the number of coupons, the last rate will prevail for
                   the remaining coupons)
                */
                const std::vector<Rate>& couponRates,
                bool fixedIsAdjusted,
                const DayCounter& fixedDayCount,
                // floating leg
                int floatingFrequency,
                const Handle<Indexes::Xibor>& index,
                int indexFixingDays,
                const std::vector<Spread>& spreads,
                // hook to term structure
                const RelinkableHandle<TermStructure>& termStructure,
                // description
                const std::string& isinCode = "",
                const std::string& description = "");
            double fixedLegBPS() const;
            double floatingLegBPS() const;
            const Date& maturity() const { return maturity_; }
            //added by Sad
            bool payFixedRate() const {
                return payFixedRate_;
            }
            const CashFlows::FixedRateCouponVector& fixedLeg() const {
                return fixedLeg_;
            }
            const CashFlows::FloatingRateCouponVector& floatingLeg() const {
                return floatingLeg_;
            }   
          private:
            bool payFixedRate_;
            Date maturity_;
            CashFlows::FixedRateCouponVector fixedLeg_;
            CashFlows::FloatingRateCouponVector floatingLeg_;
        };

        inline double SimpleSwap::fixedLegBPS() const {
            return (payFixedRate_ ? firstLegBPS() : secondLegBPS());
        }

        inline double SimpleSwap::floatingLegBPS() const {
            return (payFixedRate_ ? secondLegBPS() : firstLegBPS());
        }

        class VanillaSwap : public SimpleSwap {
          public:
            VanillaSwap(
                bool payFixedRate,
                // dates
                const Date& startDate, int n, TimeUnit units,
                const Calendar& calendar,
                RollingConvention rollingConvention,
                double nominal,
                // fixed leg
                int fixedFrequency,
                Rate couponRate,
                bool fixedIsAdjusted,
                const DayCounter& fixedDayCount,
                // floating leg
                int floatingFrequency,
                const Handle<Indexes::Xibor>& index,
                int indexFixingDays,
                const std::vector<Spread>& spreads,
                // hook to term structure
                const RelinkableHandle<TermStructure>& termStructure,
                // description
                const std::string& isinCode = "",
                const std::string& description = "")
            : SimpleSwap(payFixedRate, startDate, n, units, calendar,
                         rollingConvention, std::vector<double>(1, nominal), 
                         fixedFrequency, std::vector<Rate>(1, couponRate),
                         fixedIsAdjusted, fixedDayCount, floatingFrequency,
                         index, indexFixingDays, spreads, termStructure, 
                         isinCode, description),
              fixedRate_(couponRate), nominal_(nominal) {}

            Rate fixedRate() const { return fixedRate_; }
            Rate fairRate() const {
                return fixedRate_ - NPV()/fixedLegBPS();
            }
            double nominal() const { return nominal_; }

          private:
            Rate fixedRate_;
            double nominal_;
        };
    }

}


#endif
