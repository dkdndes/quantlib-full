
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

/*! \file mcpricer.hpp
    \brief base class for Monte Carlo pricers
     
    \fullpath
    ql/Pricers/%mcpricer.hpp
*/

// $Id$

#ifndef quantlib_montecarlo_pricer_h
#define quantlib_montecarlo_pricer_h

#include "ql/MonteCarlo/montecarlomodel.hpp"

namespace QuantLib {

    namespace Pricers {

        //! base class for Monte Carlo pricers
        /*! Eventually this class might be linked to the general tree of 
            pricers, in order to have tools like impliedVolatility available.
            Also, it could, eventually, offer greeks methods.
            Deriving a class from McPricer gives an easy way to write
            a Monte Carlo Pricer.
            See McEuropean as example of one factor pricer,
            Basket as example of multi factor pricer.
        */

        template<class S, class PG, class PP>
        class McPricer {
          public:
            virtual ~McPricer() {}
            double value(double tolerance,
                         unsigned int maxSample = QL_MAX_INT) const;
          protected:
            McPricer() {}
            mutable Handle<MonteCarlo::MonteCarloModel<S, PG, PP> > mcModel_;
            static const unsigned int minSample_;
        };


        template<class S, class PG, class PP>
        const unsigned int McPricer<S, PG, PP>::minSample_ = 10000;

        // inline definitions
        template<class S, class PG, class PP>
        inline double McPricer<S, PG, PP>::value(double tolerance,
            unsigned int maxSamples) const {

            unsigned int sampleNumber =
                mcModel_->sampleAccumulator().samples();
            if (sampleNumber<minSample_) {
                mcModel_->addSamples(minSample_-sampleNumber);
                sampleNumber = mcModel_->sampleAccumulator().samples();
            }

            unsigned int nextBatch;
            double order;
            double result = mcModel_->sampleAccumulator().mean();
            double accuracy = mcModel_->sampleAccumulator().errorEstimate()/
                result;
            while (accuracy > tolerance) {
                order = accuracy*accuracy/tolerance/tolerance;
                nextBatch = sampleNumber*order-sampleNumber+10.0;
                sampleNumber += nextBatch;
                QL_REQUIRE(sampleNumber<maxSamples,
                    "max number of samples exceeded");
                mcModel_->addSamples(nextBatch);
                result = mcModel_->sampleAccumulator().mean();
                accuracy = mcModel_->sampleAccumulator().errorEstimate()/
                    result;
            }

            return result;
        }

    }

}

#endif
