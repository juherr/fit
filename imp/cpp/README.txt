Release notes:  CeeFIT version 1.1.4
David Woldrich
04/05/2006

  * Based on a user request, added 3 new overloads for CEEFIT::Run() to support 
    returning run results to the caller, modified fitspec.cpp and
    fitexamples.cpp to print out the run results string.  See ceefit.h for more
    details.

    With these changes, you can now print out the "x right, y wrong, z ignored, 
    w exceptions" string in the same format that Java's fit.FileRunner does.
    You should also use the results numeric values to calculate an exit code for
    your program (for example, if exceptions + wrong > 0, you should return a
    non-zero error code from your test executable.)  This will make it possible
    to use CeeFIT programs in an automated testing environment, where the exit
    code is used to help compute a pass/fail result for the overall test run.
 
  * Extended CELLEQUITABLE<T> on out-of-the-box FITFIELD<T> specializations 
    where T is bool, unsigned char, signed char, char, unsigned short, signed 
    short, wchar_t, unsigned int, signed int, unsigned long, signed long, float,
    double, CEEFIT::UfitINT64, CEEFIT::fitINT64, and CEEFIT::STRING.  

    In fixtures that use native types for their fields, this change should yield
    a preciptious decrease in run times of fit tests without affecting the 
    outcome of the tests.  Making this change showed fitspec.exe ran 24 seconds
    faster (on WinXP laptop running on batteries with a Release build on MSVC6,
    run went from 1:16 to 0:52 after the change - 32% improvement in run time!)
    This change emphasizes dynamic_cast-driven direct field comparions versus 
    ToString()/IsEqual() comparisons for the majority of fields.  I find it 
    interesting how much time all of the dynamic_cast's actually take!  I would
    have liked to have seen a greater difference in run times when switching
    from string comparisons to direct comparisons since less dynamic memory
    management is involved in the direct comparison method.  (Just goes to show
    how costly dynamic_cast can be.)

    With this change, the CeeFIT build now semi-accurately approximates how the
    Java build works and it is now safe to compare the run times of Java Fit's
    FileRunner running the Fit Specification tests versus that of the CeeFIT's
    fitspec.exe.  Here are the unscientific test results on WinXP laptop running
    on batteries with Release builds of CeeFIT versus Java Fit running on 
    JDK 1.4.2_10:

      - Java 1.4.2_10 Client VM - 0:03:61
      - Java 1.4.2_10 Server VM - 0:03:73
      - MSVC6 Release Build - 0:20.45 (after a bit of profiling/tuning)
      - MinGW GCC 3.4.5 Release Build - 0:38.95 (after a bit of profiling/
        tuning)

    Clearly, I have besmirched the good name of C++ with this pitiful defeat!
    Note: these numbers are intentionally exaggerated by the fact that I'm not
    running the tests on wall power and the laptop CPU is in power-save mode.
    Had I plugged in, MSVC6's times would be something like 6 seconds, GCC
    something like 11 and Java's floats around 1 second...  Disgrace and shame!

    I ran a profiling/performance tuning session on the fitspec.exe.  Looking
    at the profiling numbers, about 85% of the time is spent working in and
    around CeeFIT's STRING implementation.  This is quite a bit higher than I
    see in typical Java applications, which are often String-intensive 
    applications themselves.  I think CeeFIT's numbers are totally blown out of
    proportion by the slow C++ exception handling (which CeeFIT makes liberal
    use of to be like Java), and the ramshackle STRING class implementation
    made by yours truly (that was written to behave similarly to Java's String
    class, but clearly not to be fast.)  Sigh, CeeFIT is a paragon of 
    sloth!  :(

  * Experimented with the Music Example to try to get it to pass 100%.  The
    remaining issue was that double's that are parsed are not directly 
    comparable to double's that are computed (minor accumulated error made
    them sightly different floating point numbers.)  Since I had implemented
    CELLEQUITABLE<T> for FITFIELD<double>, this comparison went from 
    ToString/IsEqual to direct field value comparisons and was thus failing.
    In order to make the comparisons pass, I created a MUSIC_DOUBLE class that
    wraps a double value and does NOT implement CELLEQUITABLE<T>.  By not
    implementing CELLEQUITABLE<T>, CeeFIT defaults to the ToString/IsEqual 
    method of comparing MUSIC_DOUBLE fields, and the string representation of
    MUSIC_DOUBLE reflects the style from the Java version, where there are 
    minimum two digits after the decimal point - which allows it to compare 
    exactly to the expected values in the HTML.

    This serves as an example of where not extending CELLEQUITABLE<T> can be
    advantageous for a custom FITFIELD<T> specialization.

  * No longer claiming on the CeeFIT website that I will ship pre-built binaries
    on the SourceForge downloads page.  (It's way too much trouble to get
    binaries for various platforms together for every release, sorry.)

  * Minor cleanup and content updates to the CeeFIT website overall. 

Please visit the official CeeFIT website for The latest information and news
about CeeFIT:

	http://ceefit.woldrich.com

Thank you for using CeeFIT.


================================================================================


Release notes:  CeeFIT version 1.1.3
David Woldrich
03/11/2006

This version of CeeFIT has the following changes:

  * Antfit has been updated to include support for CeeFIT as an alternative 
    to the default (Java).  The sources to antfit are available separately 
    via SourceForge CVS and are not provided as part of the CeeFIT source 
    package.

  * CeeFIT sources now ship with pre-built fit.jar and antfit.jar to make 
    running CeeFIT with Ant scripts easier.

  * Upgraded build.xml to exercise Antfit with new CeeFIT extensions.  Minor
    changes to fitspec.cpp to facilitate antfit testing.

  * The CeeFIT website has two new pages:
      - Added an Antfit help page to the website to provide usage information 
        for CeeFIT users.
      - Started a Support Email Transcripts page to share email support 
        information.  Once there are enough of these, a FAQ section will be 
        developed.

Please visit the official CeeFIT website for The latest information and news
about CeeFIT:

	http://ceefit.woldrich.com

Thank you for using CeeFIT.


================================================================================


Release notes:  CeeFIT version 1.1.2
David Woldrich
11/21/2005

This version of CeeFIT has the following changes:

  * Borland C++ Builder on Windows is now a supported platform (and perhaps on
    Linux as well?).  The Ant script was updated and BCB project files
    were included to build with Borland C++ Builder 6.0 and higher.

  * Callspec macros were slightly cleaned up for Borland C++ and Visual C++ 
    6.0 compilers (callspec macros are disabled on all other platforms and
    compilers currently.)
    
  * There are now three callspec types that CeeFIT defines:
    
      - ceefit_init_spec : all constructors should get this callspec
      - ceefit_dtor_spec : all destructors should get this callspec (except for
                           OBJECT::~OBJECT)
      - ceefit_call_spec : operator= and all other methods should get this 
                           callspec
    
    ceefit_dtor_spec was added to reconcile a problem with VC6 getting confused 
    about destructor callspecs on some classes that originally derived from 
    OBJECT (which is all classes in Core CeeFIT.)  ceefit_dtor_spec macro defines
    to blank on VC6 to eliminate the confusion.
    
    As a general reminder, if you intend your C++ code to be portable across 
    compilers that include Borland or Microsoft, please make sure to implement
    one of the three callspecs mentioned above on all Fit-related classes you
    create.
  
  * using declarations in some classes were causing internal compiler 
    errors in Borland C++ Builder.  These using declarations were added 
    to make Solaris CC builds happy, and whereas VC6 tended to ignore these, 
    they needed to be commented completely on BCC for builds of CeeFIT to 
    complete.  A new macro has been added to CeeFIT.h in mandatory.h called 
    ceefit_using_decl(a) which is equates to "using a" on all platforms other 
    than VC and BCC.
    
  * Renamed string.cpp/.h to cfstring.cpp/.h so that builds would succeed with 
    Borland C++ Builder compiler.  It was seeing #include "tools/string.h"
    and getting that confused with the standard library string.h.

  * Broke the UNIX build of CeeFIT on 1.1.1?!  All file searches died with an 
    AssertTrueFailed.  (*sigh*)  This is now fixed, Linux/unix builds should now
    work and pass spec tests.

Please visit the official CeeFIT website for The latest information and news
about CeeFIT:

	http://ceefit.woldrich.com

Thank you for using CeeFIT.


================================================================================


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
