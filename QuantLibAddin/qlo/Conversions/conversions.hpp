
/*
 Copyright (C) 2005 Plamen Neykov
 Copyright (C) 2004, 2005, 2006 Eric Ehlers

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

#ifndef qlo_conversions_hpp
#define qlo_conversions_hpp

#include <qlo/qladdindefines.hpp>
#include <ql/types.hpp>
#include <ql/math/matrix.hpp>
#include <vector>

namespace QuantLib {
    class InterestRate;
    class Date;
    class Period;
}

namespace QuantLibAddin {

    double libraryToScalar(const QuantLib::InterestRate&);
    double libraryToScalar(const QuantLib::Rate&);
    long libraryToScalar(const QuantLib::Date&);
    std::string libraryToScalar(const QuantLib::Period&);

    std::vector<long> libraryToVector(const std::vector<QuantLib::Date>&);
    std::vector<std::string> libraryToVector(const std::vector<QuantLib::Period>&);
    std::vector<long> libraryToVector(const std::vector<QuantLib::Size>&);

    void cppToLibrary(const std::string &in, QuantLib::Period &ret);
    void cppToLibrary(const long &in, QuantLib::Size &ret);

    template <class T>
    std::vector<T> cppToLibrary(const std::vector<long>& v) {
        return std::vector<T>(v.begin(), v.end());
    }

    QuantLib::Matrix vvToQlMatrix(const std::vector<std::vector<double> > &vv);

}

#endif
