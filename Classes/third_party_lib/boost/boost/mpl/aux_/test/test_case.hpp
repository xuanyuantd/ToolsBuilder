
#ifndef BOOST_MPL_AUX_TEST_TEST_CASE_HPP_INCLUDED
#define BOOST_MPL_AUX_TEST_TEST_CASE_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: test_case.hpp 49239 2008-10-10 09:10:26Z agurtovoy $
// $Date: 2008-10-10 17:10:26 +0800 (周五, 2008-10-10) $
// $Revision: 49239 $

#include <boost/preprocessor/cat.hpp>

#define MPL_TEST_CASE() void BOOST_PP_CAT(test,__LINE__)()

#endif // BOOST_MPL_AUX_TEST_TEST_CASE_HPP_INCLUDED
