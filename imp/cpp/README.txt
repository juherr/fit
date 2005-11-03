Release notes:  CeeFIT version 1.1.1
David Woldrich
11/03/2005

This version of CeeFIT resolves the following issues:

  * SCIENTFICDOUBLE was not working properly and some spec tests were 
    failing.
  * Custom value objects for fixtures (created using FITFIELD<> 
    specializations) required an IsEqual() method.  This is no longer 
    required, and you may now specify how to perform an equality test on your
    custom value object by extending FITFIELD<> specialization with 
    CELLEQUITABLE<>.  (See scientificdouble.h for an example of how this is
    done.)  The website will be updated soon on how this new equality test
    mechanism works.
  * HTML comments are now filtered out of input HTML past the first 
    encountered <body> tag.
  * Minor code fixes to compare Solaris CC and Linux PPC gcc.

Please visit the official CeeFIT website for The latest information and news
about CeeFIT:

	http://ceefit.woldrich.com

Thank you for using CeeFIT.


============================================================================


Release notes:  CeeFIT version 1.1
David Woldrich
07/20/2005

Please visit the official CeeFIT website for The latest information and news
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
