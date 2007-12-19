/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2006 Ferdinando Ametrano

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

#ifndef quantlib_autolink_hpp
#define quantlib_autolink_hpp

#include <boost/config.hpp>

// select toolset:
#if (_MSC_VER < 1310)
#  error "unsupported Microsoft compiler"
#elif (_MSC_VER == 1310)
#  define QL_LIB_TOOLSET "vc71"
#elif (_MSC_VER == 1400)
#  define QL_LIB_TOOLSET "vc80"
//#elif (_MSC_VER == 1500)
//#  define QL_LIB_TOOLSET "vc90"
#else
#  error "unknown Microsoft compiler"
#endif


/*** libraries to be linked ***/

// select thread opt:
#ifdef _MT
#  define QL_LIB_THREAD_OPT "-mt"
#else
#  define QL_LIB_THREAD_OPT
#endif

// select linkage opt:
#ifdef _DLL
#  if defined(_DEBUG)
#    define QL_LIB_RT_OPT "-gd"
#  else
#    define QL_LIB_RT_OPT
#  endif
#else
#  if defined(_DEBUG)
#    define QL_LIB_RT_OPT "-sgd"
#  else
#    define QL_LIB_RT_OPT "-s"
#  endif
#endif

#define QL_LIB_NAME "QuantLib-" QL_LIB_TOOLSET QL_LIB_THREAD_OPT QL_LIB_RT_OPT "-" QL_LIB_VERSION ".lib"

#pragma comment(lib, QL_LIB_NAME)
#ifdef BOOST_LIB_DIAGNOSTIC
#  pragma message("Will (need to) link to lib file: " QL_LIB_NAME)
#endif

#endif
