Release notes:  CeeFIT version 1.1.2
David Woldrich
11/21/2005

This version of CeeFIT has the following changes:

  * Borland C++ Builder on Windows builder is now a supported platform.  The Ant
    script was updated and BCB project files have been included to build with Borland
    C++ Builder 6.0 and higher.

  * Callspec macros were slightly cleaned up for Borland C++ and Visual C++ 
    6.0 compilers (callspec macros are disabled on all other platforms and compilers 
    currently.)
    
  * There are now three callspec types that CeeFIT defines:
    
      - ceefit_init_spec : all constructors should get this callspec
      - ceefit_dtor_spec : all destructors should get this callspec (except 
        for OBJECT::~OBJECT)
      - ceefit_call_spec : operator= and all other methods should get this callspec
    
    ceefit_dtor_spec was added to reconcile a problem with VC6 getting confused 
    about destructor callspecs on some classes that originally derived from OBJECT
    (which is all classes in Core CeeFIT.)  ceefit_dtor_spec macro defines 
    to blank on VC6 to eliminate the confusion.
    
    As a general reminder, if you intend your C++ code to be portable across compilers 
    that include Borland or Microsoft, please make sure to implement one of the 
    three callspecs mentioned above on all Fit-related classes you create.
  
  * using declarations in some classes were causing internal compiler 
    errors in Borland C++ Builder.  These using declarations were added 
    to make Solaris CC builds happy, and whereas VC6 tended to ignore these, they 
    needed to be commented completely on BCC for builds of CeeFIT to complete.
    A new macro has been added to CeeFIT.h in mandatory.h called ceefit_using_decl(a)
    which is equates to "using a" on all platforms other than VC and 
    BCC.
    
  * Renamed string.cpp/.h to cfstring.cpp/.h so that builds would succeed with 
    Borland C++ Builder compiler.  It was seeing #include "tools/string.h"
    and getting that confused with the standard library string.h.

Please visit the official CeeFIT website for The latest information and news
about CeeFIT:

	http://ceefit.woldrich.com

Thank you for using CeeFIT.


============================================================================


Release notes:  CeeFIT version 1.1.1
David Woldrich
11/03/2005

This version of CeeFIT resolves the following issues:

  * HTML comments are now filtered out of input HTML past the first 
    encountered <body> tag.  Input files that contain html comments in 
    <script>, <style>, or other tags found past the first <body> tag will be
    negatively affected by this change.
  * SCIENTFICDOUBLE was not working properly and some spec tests were 
    failing.
  * Custom value objects for fixtures (created using FITFIELD<> 
    specializations) required an IsEqual() method on CeeFIT 1.1.  Even so,
    this method was never used where it should have been; when table cells 
    needed to be compared by the CeeFIT engine, table cells would be 
    converted to string and those strings would be compared to perform 
    equality tests.  As of CeeFIT 1.1.1, IsEqual() is no longer required on
    custom value objects, and you may now specify how to perform an equality
    test on your custom value objects by extending your corresponding 
    FITFIELD<> specialization with CELLEQUITABLE<>.  (See scientificdouble.h
    for an example of how this is done.)  Implementing CELLEQUITABLE<> on 
    your FITFIELD<> specialization classes will improve overall test 
    performance.
  * Minor code fixes to build CeeFIT on Solaris CC and Linux PPC gcc.

Please visit the official CeeFIT website for The latest information and news
about CeeFIT:

	http://ceefit.woldrich.com

Thank you for using CeeFIT.


============================================================================


Release notes:  CeeFIT version 1.1
David Woldrich
07/20/2005

Please visit the official CeeFIT website for the latest information and news
about CeeFIT:

	http://ceefit.woldrich.com


LICENSE AND COPYRIGHT:
---------------------

This file is part of CeeFIT.

CeeFIT is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

CeeFIT is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with CeeFIT; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

(c)2005 Woldrich, Inc.  http://www.woldrich.com


END - DW
