//  (C) Copyright Gennadiy Rozental 2001-2004.
//  (C) Copyright Beman Dawes 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile: test_exec_fail4.cpp,v $
//
//  Version     : $Revision: 1.1 $
//
//  Description : test failures reported by throwing the exception.
//  Should fail during run.
// ***************************************************************************

// Boost.Test
#include <boost/test/test_tools.hpp>

int test_main( int argc, char* [] )  // note the name
{
    BOOST_ERROR( "Msg" );

    if( argc > 0 ) // to prevent the unreachable return warning
        throw "Test error by throwing C-style string exception";

    return 0;
}

//____________________________________________________________________________//

// ***************************************************************************
//  Revision History :
//  
//  $Log: test_exec_fail4.cpp,v $
//  Revision 1.1  2005/02/02 03:47:28  davewoldrich
//  *** empty log message ***
//
//  Revision 1.13  2004/05/21 06:26:11  rogeeff
//  licence update
//
//  Revision 1.12  2003/12/01 00:42:38  rogeeff
//  prerelease cleaning
//

// ***************************************************************************

// EOF