
// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot/fit/fit/imp/cpp/lib/boost_1_32_0/libs/mpl/test/Attic/min_max.cpp,v $
// $Date: 2005/02/02 03:29:27 $
// $Revision: 1.1 $

#include <boost/mpl/min_max.hpp>
#include <boost/mpl/int.hpp>

#include <boost/mpl/aux_/test.hpp>


MPL_TEST_CASE()
{
    MPL_ASSERT(( is_same< mpl::min< int_<5>,int_<7> >::type,int_<5> > ));
    MPL_ASSERT(( is_same< mpl::max< int_<5>,int_<7> >::type,int_<7> > ));

    MPL_ASSERT(( is_same< mpl::min< int_<-5>,int_<-7> >::type,int_<-7> > ));
    MPL_ASSERT(( is_same< mpl::max< int_<-5>,int_<-7> >::type,int_<-5> > ));
}
