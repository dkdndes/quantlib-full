/*
 * Copyright (C) 2001
 * Ferdinando Ametrano, Luigi Ballabio, Adolfo Benin, Marco Marchioro
 *
 * This file is part of QuantLib.
 * QuantLib is a C++ open source library for financial quantitative
 * analysts and developers --- http://quantlib.sourceforge.net/
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
 * if not, contact ferdinando@ametrano.net
 *
 * QuantLib license is also available at http://quantlib.sourceforge.net/LICENSE.TXT
*/
/*! \file standardmultipathgenerator.h
    \brief Generates multi paths from random points

    $Source$
    $Name$
    $Log$
    Revision 1.1  2001/01/25 16:11:17  marmar
    MultiPath, the first step for a multi-dimensional Monte Carlo

*/


#ifndef quantlib_montecarlo_standard_multi_path_generator_h
#define quantlib_montecarlo_standard_multi_path_generator_h

#include "gaussianrandomgenerator.h"
#include "multipathgenerator.h"

namespace QuantLib {

    namespace MonteCarlo {

    /*! \typedef StandardMultiPathGenerator
        The following is the default choice for the multi-path generator. 
        See the corresponding classes for more documentation.
    */
        typedef MultiPathGenerator<GaussianRandomGenerator > 
                                              StandardMultiPathGenerator;
    }
}

#endif
