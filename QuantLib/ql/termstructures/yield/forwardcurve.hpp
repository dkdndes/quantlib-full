/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2005, 2006, 2007, 2008, 2009 StatPro Italia srl

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

/*! \file forwardcurve.hpp
    \brief interpolated forward-rate structure
*/

#ifndef quantlib_forward_curve_hpp
#define quantlib_forward_curve_hpp

#include <ql/termstructures/yield/forwardstructure.hpp>
#include <ql/termstructures/interpolatedcurve.hpp>
#include <ql/math/interpolations/backwardflatinterpolation.hpp>
#include <ql/math/comparison.hpp>
#include <utility>

namespace QuantLib {

    //! Term structure based on interpolation of forward rates
    /*! \ingroup yieldtermstructures */
    template <class Interpolator>
    class InterpolatedForwardCurve : public ForwardRateStructure,
                                     protected InterpolatedCurve<Interpolator> {
      public:
        // constructor
        InterpolatedForwardCurve(const std::vector<Date>& dates,
                                 const std::vector<Rate>& forwards,
                                 const DayCounter& dayCounter,
                                 const Calendar& cal = Calendar(),
                                 const Interpolator& interpolator
                                                            = Interpolator());
        //! \name TermStructure interface
        //@{
        Date maxDate() const;
        //@}
        //! \name other inspectors
        //@{
        const std::vector<Time>& times() const;
        const std::vector<Date>& dates() const;
        const std::vector<Rate>& forwards() const;
        std::vector<std::pair<Date,Rate> > nodes() const;
        //@}
      protected:
        InterpolatedForwardCurve(const DayCounter&,
                                 const Interpolator& interpolator
                                                            = Interpolator());
        InterpolatedForwardCurve(const Date& referenceDate,
                                 const DayCounter&,
                                 const Interpolator& interpolator
                                                            = Interpolator());
        InterpolatedForwardCurve(Natural settlementDays,
                                 const Calendar&,
                                 const DayCounter&,
                                 const Interpolator& interpolator
                                                            = Interpolator());
        Rate forwardImpl(Time t) const;
        Rate zeroYieldImpl(Time t) const;
        mutable std::vector<Date> dates_;
    };

    //! Term structure based on flat interpolation of forward rates
    /*! \ingroup yieldtermstructures */

    typedef InterpolatedForwardCurve<BackwardFlat> ForwardCurve;


    // inline definitions

    template <class T>
    inline Date InterpolatedForwardCurve<T>::maxDate() const {
        return dates_.back();
    }

    template <class T>
    inline const std::vector<Time>&
    InterpolatedForwardCurve<T>::times() const {
        return this->times_;
    }

    template <class T>
    inline const std::vector<Date>&
    InterpolatedForwardCurve<T>::dates() const {
        return dates_;
    }

    template <class T>
    inline const std::vector<Rate>&
    InterpolatedForwardCurve<T>::forwards() const {
        return this->data_;
    }

    template <class T>
    inline std::vector<std::pair<Date,Rate> >
    InterpolatedForwardCurve<T>::nodes() const {
        std::vector<std::pair<Date,Rate> > results(dates_.size());
        for (Size i=0; i<dates_.size(); ++i)
            results[i] = std::make_pair(dates_[i],this->data_[i]);
        return results;
    }

    #ifndef __DOXYGEN__

    // template definitions

    template <class T>
    Rate InterpolatedForwardCurve<T>::forwardImpl(Time t) const {
        if (t <= this->times_.back()) {
            return this->interpolation_(t, true);
        } else {
            // flat extrapolation
            return this->data_.back();
        }
    }

    template <class T>
    Rate InterpolatedForwardCurve<T>::zeroYieldImpl(Time t) const {
        if (t == 0.0)
            return forwardImpl(0.0);

        Real integral;
        if (t <= this->times_.back()) {
            integral = this->interpolation_.primitive(t, true);
        } else {
            // flat extrapolation
            integral = this->interpolation_.primitive(this->times_.back(), true)
                     + this->data_.back()*(t - this->times_.back());
        }
        return integral/t;
    }

    template <class T>
    InterpolatedForwardCurve<T>::InterpolatedForwardCurve(
                                                 const DayCounter& dayCounter,
                                                 const T& interpolator)
    : ForwardRateStructure(dayCounter),
      InterpolatedCurve<T>(interpolator) {}

    template <class T>
    InterpolatedForwardCurve<T>::InterpolatedForwardCurve(
                                                 const Date& referenceDate,
                                                 const DayCounter& dayCounter,
                                                 const T& interpolator)
    : ForwardRateStructure(referenceDate, Calendar(), dayCounter),
      InterpolatedCurve<T>(interpolator) {}

    template <class T>
    InterpolatedForwardCurve<T>::InterpolatedForwardCurve(
                                                 Natural settlementDays,
                                                 const Calendar& calendar,
                                                 const DayCounter& dayCounter,
                                                 const T& interpolator)
    : ForwardRateStructure(settlementDays, calendar, dayCounter),
      InterpolatedCurve<T>(interpolator) {}

    template <class T>
    InterpolatedForwardCurve<T>::InterpolatedForwardCurve(
                                            const std::vector<Date>& dates,
                                            const std::vector<Rate>& forwards,
                                            const DayCounter& dayCounter,
                                            const Calendar& calendar,
                                            const T& interpolator)
    : ForwardRateStructure(dates.front(), calendar, dayCounter),
      InterpolatedCurve<T>(std::vector<Time>(), forwards, interpolator),
      dates_(dates)
    {
        QL_REQUIRE(dates_.size() >= T::requiredPoints,
                   "not enough input dates given");
        QL_REQUIRE(this->data_.size() == dates_.size(),
                   "dates/data count mismatch");

        this->times_.resize(dates_.size());
        this->times_[0]=0.0;
        for (Size i=1; i<dates_.size(); ++i) {
            QL_REQUIRE(dates_[i] > dates_[i-1],
                       "invalid date (" << dates_[i] << ", vs "
                       << dates_[i-1] << ")");
            #if !defined(QL_NEGATIVE_RATES)
            QL_REQUIRE(this->data_[i] >= 0.0, "negative forward");
            #endif
            this->times_[i] = dayCounter.yearFraction(dates_[0], dates_[i]);
            QL_REQUIRE(!close(this->times_[i],this->times_[i-1]),
                       "two dates correspond to the same time "
                       "under this curve's day count convention");
        }

        this->interpolation_ =
            this->interpolator_.interpolate(this->times_.begin(),
                                            this->times_.end(),
                                            this->data_.begin());
        this->interpolation_.update();
    }

    #endif
}

#endif
