
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

/*! \file segmentintegral.hpp
    \fullpath Include/ql/Math/%integral.hpp
    \brief Integral of a one-dimensional function

    $Id$
*/

// $Source$
// $Log$
// Revision 1.1  2001/08/08 15:47:11  marmar
// Class SegmentIntegral computes the integral of a function over an interval
//

#include "ql/dataformatters.hpp"
#include "ql/solver1d.hpp"
#include <functional>

#ifndef quantlib_math_segmant_integral_h
#define quantlib_math_segmant_integral_h

namespace QuantLib {

    namespace Math {
        
        class SegmentIntegral{
          public:
            SegmentIntegral(long intervals);
            double operator()(const ObjectiveFunction &f, 
                             double a, double b) const;
          private:
            long intervals_;
       };

    }

}


#endif
