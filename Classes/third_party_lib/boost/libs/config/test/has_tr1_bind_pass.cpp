//  This file was automatically generated on Sat Jul 12 12:39:31 2008
//  by libs/config/tools/generate.cpp
//  Copyright John Maddock 2002-4.
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/config for the most recent version.//
//  Revision $Id: has_tr1_bind_pass.cpp 47351 2008-07-12 12:41:52Z johnmaddock $
//


// Test file for macro BOOST_HAS_TR1_BIND
// This file should compile, if it does not then
// BOOST_HAS_TR1_BIND should not be defined.
// See file boost_has_tr1_bind.ipp for details

// Must not have BOOST_ASSERT_CONFIG set; it defeats
// the objective of this file:
#ifdef BOOST_ASSERT_CONFIG
#  undef BOOST_ASSERT_CONFIG
#endif

#include <boost/config.hpp>
#include <boost/tr1/detail/config.hpp>
#include "test.hpp"

#ifdef BOOST_HAS_TR1_BIND
#include "boost_has_tr1_bind.ipp"
#else
namespace boost_has_tr1_bind = empty_boost;
#endif

int main( int, char *[] )
{
   return boost_has_tr1_bind::test();
}

