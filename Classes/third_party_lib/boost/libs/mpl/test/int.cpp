
// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: int.cpp 49240 2008-10-10 09:21:07Z agurtovoy $
// $Date: 2008-10-10 17:21:07 +0800 (周五, 2008-10-10) $
// $Revision: 49240 $

#include <boost/mpl/int.hpp>
#include <boost/preprocessor/repeat.hpp>

#include "integral_wrapper_test.hpp"


MPL_TEST_CASE()
{
#   define WRAPPER(T, i) int_<i>
    BOOST_PP_REPEAT(10, INTEGRAL_WRAPPER_TEST, int)
}
