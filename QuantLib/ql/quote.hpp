/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2000, 2001, 2002, 2003 RiskMap srl

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

/*! \file quote.hpp
    \brief purely virtual base class for market observables
*/

#ifndef quantlib_quote_hpp
#define quantlib_quote_hpp

#include <ql/types.hpp>
#include <ql/handle.hpp>
#include <ql/errors.hpp>

namespace QuantLib {

    //! purely virtual base class for market observables
    /*! \test the observability of class instances is tested.
     */
    class Quote : public Observable {
      public:
        virtual ~Quote() {}
        //! returns the current value
        virtual Real value() const = 0;
    };


    //! market element returning a stored value
    class SimpleQuote : public Quote {
      public:
        SimpleQuote(Real value);
        //! \name Quote interface
        //@{
        Real value() const;
        //@}
        //! \name Modifiers
        //@{
        void setValue(Real value);
        //@}
      private:
        Real value_;
    };

    //! interval quote
    class IntervalQuote : public Quote {
    public:
        enum price_type {Open, Close, High, Low};
        IntervalQuote(Real o, Real c, Real h, Real l,
                      price_type r = Close) :
            open_(o), close_(c), high_(h), low_(l),
        return_type_(r) {};
        void setReturnType(price_type p) {
            return_type_ = p;
        }
        Real value() const { return value(return_type_);};
        Real value(price_type p) const {
            switch(p) {
            case Open:
                return open_;
            case Close:
                return close_;
            case High:
                return high_;
            case Low:
                return low_;
            default:
                QL_FAIL("unknown price_type in interval quote");
            }
        }
        void setValue(Real o, Real c, Real h, Real l) {
            open_ = o; close_ = c;
            high_ = h; low_ = l;
        }
    private:
        Real open_, close_, high_, low_;
        price_type return_type_;
    };


    //! market element whose value depends on another market element
    /*! \test the correctness of the returned values is tested by
              checking them against numerical calculations.
    */
    template <class UnaryFunction>
    class DerivedQuote : public Quote, public Observer {
      public:
        DerivedQuote(const Handle<Quote>& element,
                     const UnaryFunction& f);
        //! \name Market element interface
        //@{
        Real value() const;
        //@}
        //! \name Observer interface
        //@{
        void update();
        //@}
      private:
        Handle<Quote> element_;
        UnaryFunction f_;
    };


    //! market element whose value depends on two other market element
    /*! \test the correctness of the returned values is tested by
              checking them against numerical calculations.
    */
    template <class BinaryFunction>
    class CompositeQuote : public Quote, public Observer {
      public:
        CompositeQuote(
            const Handle<Quote>& element1,
            const Handle<Quote>& element2,
            const BinaryFunction& f);
        //! \name Quote interface
        //@{
        Real value() const;
        //@}
        //! \name Observer interface
        //@{
        void update();
        //@}
      private:
        Handle<Quote> element1_, element2_;
        BinaryFunction f_;
    };


    // inline definitions

    // simple quote

    inline SimpleQuote::SimpleQuote(Real value)
    : value_(value) {}

    inline Real SimpleQuote::value() const {
        return value_;
    }

    inline void SimpleQuote::setValue(Real value) {
        if (value_ == value)
            return;
        value_ = value;
        notifyObservers();
    }


    // derived quote

    template <class UnaryFunction>
    inline DerivedQuote<UnaryFunction>::DerivedQuote(
                                                 const Handle<Quote>& element,
                                                 const UnaryFunction& f)
    : element_(element), f_(f) {
        registerWith(element_);
    }

    template <class UnaryFunction>
    inline Real DerivedQuote<UnaryFunction>::value() const {
        QL_REQUIRE(!element_.empty(), "null market element set");
        return f_(element_->value());
    }

    template <class UnaryFunction>
    inline void DerivedQuote<UnaryFunction>::update() {
        notifyObservers();
    }


    // composite quote

    template <class BinaryFunction>
    inline CompositeQuote<BinaryFunction>::CompositeQuote(
                                                const Handle<Quote>& element1,
                                                const Handle<Quote>& element2,
                                                const BinaryFunction& f)
    : element1_(element1), element2_(element2), f_(f) {
        registerWith(element1_);
        registerWith(element2_);
    }

    template <class BinaryFunction>
    inline Real CompositeQuote<BinaryFunction>::value() const {
        QL_REQUIRE(!element1_.empty() && !element2_.empty(),
                   "null quote set");
        return f_(element1_->value(),element2_->value());
    }

    template <class BinaryFunction>
    inline void CompositeQuote<BinaryFunction>::update() {
        notifyObservers();
    }

}


#endif

