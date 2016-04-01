
// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: is_placeholder.cpp 49240 2008-10-10 09:21:07Z agurtovoy $
// $Date: 2008-10-10 17:21:07 +0800 (周五, 2008-10-10) $
// $Revision: 49240 $

#include <boost/mpl/is_placeholder.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/aux_/test.hpp>

#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/inc.hpp>
#include <boost/preprocessor/cat.hpp>
    
#define AUX_IS_PLACEHOLDER_TEST(unused1, n, unused2) \
    { MPL_ASSERT(( is_placeholder< \
          BOOST_PP_CAT(_,BOOST_PP_INC(n)) \
        > )); } \
/**/

MPL_TEST_CASE()
{
    MPL_ASSERT_NOT(( is_placeholder<int> ));
    MPL_ASSERT_NOT(( is_placeholder<UDT> ));
    MPL_ASSERT_NOT(( is_placeholder<incomplete> ));
    MPL_ASSERT_NOT(( is_placeholder<abstract> ));
    MPL_ASSERT_NOT(( is_placeholder<noncopyable> ));
    MPL_ASSERT(( is_placeholder<_> ));
    
    BOOST_PP_REPEAT(
          BOOST_MPL_LIMIT_METAFUNCTION_ARITY
        , AUX_IS_PLACEHOLDER_TEST
        , unused
        )
}
