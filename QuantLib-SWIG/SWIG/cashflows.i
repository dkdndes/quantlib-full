
/*
 Copyright (C) 2000, 2001, 2002, 2003 RiskMap srl

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it under the
 terms of the QuantLib license.  You should have received a copy of the
 license along with this program; if not, please email quantlib-dev@lists.sf.net
 The license is also available online at http://quantlib.org/html/license.html

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#ifndef quantlib_cash_flows_i
#define quantlib_cash_flows_i

%include date.i
%include types.i
%include calendars.i
%include daycounters.i
%include indexes.i
%include termstructures.i
%include scheduler.i
%include vectors.i

%{
using QuantLib::CashFlow;
%}

%ignore CashFlow;
class CashFlow {
  public:
    Real amount() const;
    Date date() const;
};

%template(CashFlow) boost::shared_ptr<CashFlow>;
IsObservable(boost::shared_ptr<CashFlow>);


// implementations

%{
using QuantLib::SimpleCashFlow;
using QuantLib::FixedRateCoupon;
using QuantLib::ParCoupon;
typedef boost::shared_ptr<CashFlow> SimpleCashFlowPtr;
typedef boost::shared_ptr<CashFlow> FixedRateCouponPtr;
typedef boost::shared_ptr<CashFlow> ParCouponPtr;
%}

%rename(SimpleCashFlow) SimpleCashFlowPtr;
class SimpleCashFlowPtr : public boost::shared_ptr<CashFlow> {
  public:
    %extend {
        SimpleCashFlowPtr(Real amount, const Date& date) {
            return new SimpleCashFlowPtr(new SimpleCashFlow(amount,date));
        }
    }
};

%rename(FixedRateCoupon) FixedRateCouponPtr;
class FixedRateCouponPtr : public boost::shared_ptr<CashFlow> {
  public:
    %extend {
        FixedRateCouponPtr(Real nominal, const Date& paymentDate,
                           Rate rate, const DayCounter& dayCounter,
                           const Date& startDate, const Date& endDate,
                           const Date& refPeriodStart = Date(),
                           const Date& refPeriodEnd = Date()) {
            return new FixedRateCouponPtr(
                new FixedRateCoupon(nominal, paymentDate, rate,
                                    dayCounter, startDate, endDate,
                                    refPeriodStart, refPeriodEnd));
        }
    }
};

%rename(ParCoupon) ParCouponPtr;
class ParCouponPtr : public boost::shared_ptr<CashFlow> {
  public:
    %extend {
        ParCouponPtr(Real nominal, const Date& paymentDate,
                     const XiborPtr& index,
                     const Date& startDate, const Date& endDate,
                     Integer fixingDays, Spread spread = 0.0,
                     const Date& refPeriodStart = Date(),
                     const Date& refPeriodEnd = Date()) {
            boost::shared_ptr<Xibor> libor =
                boost::dynamic_pointer_cast<Xibor>(index);
            return new ParCouponPtr(
                new ParCoupon(nominal, paymentDate, libor,
                              startDate, endDate, fixingDays, spread,
                              refPeriodStart, refPeriodEnd));
        }
        Date accrualStartDate() {
            return boost::dynamic_pointer_cast<ParCoupon>(*self)
                 ->accrualStartDate();
        }
        Date accrualEndDate() {
            return boost::dynamic_pointer_cast<ParCoupon>(*self)
                 ->accrualEndDate();
        }
        Rate rate() {
            return boost::dynamic_pointer_cast<ParCoupon>(*self)->rate();
        }
        Rate indexFixing() {
            return boost::dynamic_pointer_cast<ParCoupon>(*self)
                ->indexFixing();
        }
        Real nominal() {
            return boost::dynamic_pointer_cast<ParCoupon>(*self)->nominal();
        }
    }
};

namespace std {
    %template(CashFlowVector) vector<boost::shared_ptr<CashFlow> >;
}

// cash flow vector builders

%{
using QuantLib::FixedRateCouponVector;
%}

std::vector<boost::shared_ptr<CashFlow> >
FixedRateCouponVector(const Schedule& schedule,
                      BusinessDayConvention paymentAdjustment,
                      const std::vector<Real>& nominals,
                      const std::vector<Rate>& couponRates,
                      const DayCounter& dayCount,
                      const DayCounter& firstPeriodDayCount
                        = DayCounter());

%inline %{
std::vector<boost::shared_ptr<CashFlow> >
FloatingRateCouponVector(const Schedule& schedule,
                         BusinessDayConvention paymentAdjustment,
                         const std::vector<Real>& nominals,
                         const XiborPtr& index, Integer fixingDays,
                         const std::vector<Spread>& spreads =
                             std::vector<Spread>()) {
    boost::shared_ptr<Xibor> libor =
        boost::dynamic_pointer_cast<Xibor>(index);
    return QuantLib::FloatingRateCouponVector(schedule,paymentAdjustment,
                                              nominals,libor,
                                              fixingDays,spreads);
}
%}


// cash-flow analysis

%{
using QuantLib::Cashflows;
typedef QuantLib::Duration::Type DurationType;

DurationType durationFromString(std::string s) {
    s = QuantLib::lowercase(s);
    if (s == "simple")
        return QuantLib::Duration::Simple;
    else if (s == "macaulay")
        return QuantLib::Duration::Macaulay;
    else if (s == "modified")
        return QuantLib::Duration::Modified;
    else
        QL_FAIL("unknown duration type");
}

std::string stringFromDuration(DurationType d) {
    switch (d) {
      case QuantLib::Duration::Simple:      return "simple";
      case QuantLib::Duration::Macaulay:    return "Macaulay";
      case QuantLib::Duration::Modified:    return "modified";
      default:                              QL_FAIL("unknown duration type");
    }
}
%}

MapToString(DurationType,durationFromString,stringFromDuration);

class Cashflows {
  private:
    Cashflows();
    Cashflows(const Cashflows&);
  public:
    static Real npv(const std::vector<boost::shared_ptr<CashFlow> >&,
                    const Handle<YieldTermStructure>&);
    static Real npv(const std::vector<boost::shared_ptr<CashFlow> >&,
                    const InterestRate&,
                    Date settlementDate = Date());
    static Rate irr(const std::vector<boost::shared_ptr<CashFlow> >&,
                    Real marketPrice,
                    const DayCounter& dayCounter,
                    Compounding compounding,
                    Frequency frequency = QuantLib::NoFrequency,
                    Date settlementDate = Date(),
                    Real tolerance = 1.0e-10,
                    Size maxIterations = 10000,
                    Rate guess = 0.05);
    static Real convexity(const std::vector<boost::shared_ptr<CashFlow> >&,
                          const InterestRate&,
                          Date settlementDate = Date());
    static Time duration(const std::vector<boost::shared_ptr<CashFlow> >&,
                         Real marketPrice,
                         const InterestRate&,
                         DurationType type = QuantLib::Duration::Simple,
                         Date settlementDate = Date());
};


#endif
