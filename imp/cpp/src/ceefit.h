#ifndef __CEEFIT_H__
#define __CEEFIT_H__

/**
 * <p>CeeFIT : A C++ version of <a href="http://fit.c2.com">FIT</a>, compliant with the <a href="http://fit.c2.com/wiki.cgi?FitSpecification">Fit Specification</a></p>
 *
 * <p>Design goals include:</p>
 *
 * <p><ul>
 * <li>Run statically linked inside host application rather than as a separate shared object or runtime.</p>
 * <li>Automatic registration of fixtures, their member variables, and member functions using macro assists.</li>
 * <li>Able to accomodate user's memory allocator/deallocator.</p>
 * <li>Explicitly declare __cdecl or __stdcall on all functions for maximum portability and to override user's default
 * calling convention.  (08/17/05 - modified this goal to only Microsoft environments to be friendlier to Solaris compiler...)</li>
 * <li>Allow user to declare specializations of FITFIELD so that CeeFIT will parse and emit strings for custom data types.</li>
 * <li>Provide a single &quot;drop-in&quot; header for the user that provides everything needed to build and run CeeFIT tests.</li>
 * <li>Build and run with VC6.0 SP6</li>
 * <li>Build and run with GCC 3.4.3 (and higher, hopefully)</li>
 * </ul></p>
 *
 * <p>This file is part of CeeFIT.</p>
 *
 * <p>CeeFIT is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.</p>
 *
 * <p>CeeFIT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.</p>
 *
 * <p>You should have received a copy of the GNU General Public License
 * along with CeeFIT; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA</p>
 *
 * <p>(c)2005 Woldrich, Inc.  <a href="http://www.woldrich.com">http://www.woldrich.com</a></p>
 *
 * @cite [Cunn 04] Cunningham, Ward. Welcome Visitors. <i>Framework for Integrated Test.</i><br>December 10, 2004. <a href="http://fit.c2.com/?WelcomeVisitors">http://fit.c2.com/?WelcomeVisitors</a>
 * @author David Woldrich
 */

// This ceremonial good juju hand waving is dedicated to my homey:  Bill Gates ...
#if (defined(_MSC_VER) && !defined(__BORLANDC__))
# pragma inline_recursion(on)
# pragma inline_depth(255)
# pragma warning(disable : 4786)

# ifdef _DEBUG
//#   ifdef DUMP_MSVC_MEMORY_LEAKS
#     define CRTDBG_MAP_ALLOC
#     include <stdlib.h>
#     include <crtdbg.h>
#   endif
//# endif
#endif

// Standard C headers
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <fcntl.h>
#include <math.h>
#if (defined(_MSC_VER) || defined(__BORLANDC__))
# include <direct.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <malloc.h>
#include <memory.h>
#include <stddef.h>
#include <assert.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>
#include <limits.h>

#include <typeinfo>
#include <new>

// Define up-front, mandatory symbols
#include "tools/mandatory.h"

// Declare what the alloc/free functions should look like
#ifndef CEEFIT_ALLOC_FUNCS                                  // tombstoned so as not to conflict with alloc.h
# define CEEFIT_ALLOC_FUNCS

# if !(defined(_MSC_VER) || defined(__BORLANDC__))
#   define fit_size_t std::size_t
# else
#   define fit_size_t size_t
# endif

  extern "C"
  {
    typedef void* (ceefit_call_spec * CEEFITALLOCFUNC)(fit_size_t numBytes);
    typedef void (ceefit_call_spec * CEEFITFREEFUNC)(void* objPtr);
  }
#endif

// Tools and Java-like classes
#include "tools/object.h"
#include "tools/misc.h"
#include "tools/cfstring.h"     // renamed from string.h to work with Borland C++ Builder 11/21/05
#include "tools/wcs.h"          // a module taken from winehq and added to CeeFIT to obtain a working _vsnwprintf for GCC ....
#include "tools/exception.h"
#include "tools/failure.h"
#include "tools/equality.h"
#include "tools/dynarray.h"
#include "tools/stringdata.h"
#include "tools/slink.h"
#include "tools/value.h"
#include "tools/ptr.h"
#include "tools/refcounted.h"
#include "tools/writer.h"
#include "tools/BufferedFileReader.h"
#include "tools/hashmap.h"
#include "tools/findfiles.h"
#include "tools/splitpath.h"

// CeeFIT system files
#include "ceefit/celladapter.h"
#include "ceefit/Runner.h"
#include "ceefit/fitfield.h"
#include "ceefit/fittest.h"
#include "ceefit/registerstatic.h"
#include "ceefit/Fixture.h"
#include "ceefit/ActionFixture.h"
#include "ceefit/ColumnFixture.h"
#include "ceefit/PrimitiveFixture.h"
#include "ceefit/RowFixture.h"
#include "ceefit/TimedActionFixture.h"
#include "ceefit/macros.h"
#include "ceefit/Parse.h"
#include "ceefit/ScientificDouble.h"
#include "ceefit/FileRunner.h"

/**
 * <p>This function must be defined by the user, and must return a non-null alloc function</p>
 * 
 * <p>In CeeFIT DLL's this can return NULL (the EXE will provide our heap functions)</p>
 */
extern ::CEEFITALLOCFUNC ceefit_call_spec GetCeeFitAllocFunc(void);

/**
 * <p>This function must be defined by the user, and must return a non-null free function</p>
 * 
 * <p>In CeeFIT DLL's this can return NULL (the EXE will provide our heap functions)</p>
 */
extern ::CEEFITFREEFUNC ceefit_call_spec GetCeeFitFreeFunc(void);

namespace CEEFIT
{
  /**
   * <p>CEEFIT::Run() RESULTS
   */
  class RESULTS
  {
    private:
      int Right;                  /**< Number of CeeFIT tests that were right */
      int Wrong;                  /**< Number of CeeFIT tests that were wrong */
      int Ignores;                /**< Number of CeeFIT tests that were ignored */
      int Exceptions;             /**< Number of CeeFIT tests that threw uncaught exceptions */
      wchar_t CountsSummary[256]; /**< Counts summary string as reported by the FILERUNNER's FIXTURE object */

    public:
      inline RESULTS(void) { Right = 0; Wrong = 0; Ignores = 0; Exceptions = 0; CountsSummary[0] = L'\0'; }
      inline ~RESULTS(void) {}

      inline void SetRight(int right) { Right = right; }                                /**< Set the number of CeeFIT tests that were right */
      inline void SetWrong(int wrong) { Wrong = wrong; }                                /**< Set the number of CeeFIT tests that were wrong */
      inline void SetIgnores(int ignores) { Ignores = ignores; }                        /**< Set the number of CeeFIT tests that were ignored */
      inline void SetExceptions(int exceptions) { Exceptions = exceptions; }            /**< Set the number of CeeFIT tests that threw exceptions */
      void SetCountsSummary(const wchar_t* resultsStr);                                 /**< Set the CEEFIT::Run() counts string summary */

      inline int GetRight(void) const { return(Right); }                                /**< Get the number of CeeFIT tests that were right */
      inline int GetWrong(void) const { return(Wrong); }                                /**< Get the number of CeeFIT tests that were wrong */
      inline int GetIgnores(void) const { return(Ignores); }                            /**< Get the number of CeeFIT tests that were ignored */
      inline int GetExceptions(void) const { return(Exceptions); }                      /**< Get the number of CeeFIT tests that threw exceptions */
      inline const wchar_t* GetCountsSummary(void) const { return(&CountsSummary[0]); } /**< Get the CEEFIT::Run() counts string summary */

    private:
      RESULTS(const RESULTS&);                 /**< Not implemented, do not call */
      RESULTS& operator=(const RESULTS&);      /**< Not implemented, do not call */
  };

  /**
   * <p>Run the CeeFIT tests on an input file and write to an output file.</p>
   *
   * <p>The first parameter of argv must exactly match -CeeFIT in order for FIT tests to run.  Following -CeeFIT,
   * the next two parameters are expected to be an input file and an output file.  When Run() completes, it will
   * return a 1 if all tests pass and a 2 if one or more tests failed.  The calling program should exit
   * immediately, returning the exit code that Run() returns.</p>
   *
   * <p>If the -CeeFIT parameter is not detected on the passed command line, Run() will return 0 and the program should be allowed
   * to execute normally.<p>
   *
   * <p>Upon successfully opening the input file, CeeFIT will attempt to create an output file.  If the output file's specified folder
   * path does not already exist, CeeFIT attempts to create the folder tree.</p>
   *
   * @param argc The argc passed from main()
   * @param argv The argv passed from main().  Pathspecs that contain spaces are expected to be surrounded with double quotes
   *             ('\"') by the caller.
   * @param doReleaseStatics If true, static references in FIXTURE's will be released at the end of the CeeFIT run.
   * @return 0 if no -CeeFIT parameter was detected and no action was taken, 1 if -CeeFIT parameter was detected and all tests pass,
   *         2 if -CeeFIT parameter was detected and one or more tests had a failure or error.
   */
  extern int ceefit_call_spec Run(int argc, char** argv, bool doReleaseStatics=true);

  /**
   * <p>Same as Run() above that takes unicode char main style command line.</p>
   */
  extern int ceefit_call_spec Run(int argc, wchar_t** argv, bool doReleaseStatics=true);

  /**
   * <p>Same as Run() above that takes WinMain style command line.</p>
   */
  extern int ceefit_call_spec Run(const char* cmdLine, bool doReleaseStatics=true);

  /**
   * <p>Same as Run() above that takes WinMainW style command line (unicode string.)</p>
   */
  extern int ceefit_call_spec Run(const wchar_t* wideCmdLine, bool doReleaseStatics=true);

  /**
   * <p>Run the CeeFIT tests on an input file and write to an output file.</p>
   *
   * <p>The first parameter of argv must exactly match -CeeFIT in order for FIT tests to run.  Following -CeeFIT,
   * the next two parameters are expected to be an input file and an output file.  When Run() completes, it will
   * return a 1 if all tests pass and a 2 if one or more tests failed.  The calling program should exit
   * immediately, returning the exit code that Run() returns.</p>
   *
   * <p>If the -CeeFIT parameter is not detected on the passed command line, Run() will return 0 and the program should be allowed
   * to execute normally.<p>
   *
   * <p>Upon successfully opening the input file, CeeFIT will attempt to create an output file.  If the output file's specified folder
   * path does not already exist, CeeFIT attempts to create the folder tree.</p>
   *
   * @param argc The argc passed from main()
   * @param argv The argv passed from main().  Pathspecs that contain spaces are expected to be surrounded with double quotes
   *             ('\"') by the caller.
   * @param outResults A RESULTS object that will be populated with the outputs of the CeeFIT run.
   * @param doReleaseStatics If true, static references in FIXTURE's will be released at the end of the CeeFIT run.
   * @return 0 if no -CeeFIT parameter was detected and no action was taken, 1 if -CeeFIT parameter was detected and all tests pass,
   *         2 if -CeeFIT parameter was detected and one or more tests had a failure or error.
   */
  extern int ceefit_call_spec Run(int argc, char** argv, RESULTS& outResults, bool doReleaseStatics=true);

  /**
   * <p>Same as Run() above that returns RESULTS that takes unicode char main style command line.</p>
   */
  extern int ceefit_call_spec Run(int argc, wchar_t** argv, RESULTS& outResults, bool doReleaseStatics=true);

  /**
   * <p>Same as Run() above that returns RESULTS that takes WinMain style command line.</p>
   */
  extern int ceefit_call_spec Run(const char* cmdLine, RESULTS& outResults, bool doReleaseStatics=true);

  /**
   * <p>Same as Run() above that returns RESULTS that takes WinMainW style command line (unicode string.)</p>
   */
  extern int ceefit_call_spec Run(const wchar_t* wideCmdLine, RESULTS& outResults, bool doReleaseStatics=true);

  /**
   * <p>Called from the main executable's runtime, communicates main program's heap access functions to a DLL and adds the DLL's custom FIXTURE classes
   * to the main EXE's list of FIXTURE's.</p>
   * 
   * <p>This function takes a little explanation in order to use properly.  CeeFIT relies on static linking in order to get FIXTURE classes registered 
   * into the system.  When FIXTURE classes are defined in DLL's, the factories for those FIXTURE's do not automatically register with CeeFIT, but they 
   * do register with a list that lives in the DLL.  RegisterDll transfers those autoregistered FIXTUREFACTORY's to the EXE so that they will be 
   * recognized.</p>
   *
   * <p>To make DLL-borne FIXTURES callable from your EXE, wrap a call to RegisterDll in a dllexport'ed function residing in your DLL.  Then, from 
   * your exe, and before calling the CEEFIT::Run() function, invoke your dllexport'ed function wrapper for RegisterDll.</p>
   *
   * @param fixtureFactoryListFromExe This must contain the result of a call, performed by the EXE, to RUNNER::GetFixtureFactoryList().
   * @param allocFuncFromExe This must contain the result of a call, performed by the EXE, to ::GetCeeFitAllocFunc().
   * @param freeFuncFromExe This must contain the result of a call, performed by the EXE, to ::GetCeeFitFreeFunc().
   */
  extern void ceefit_call_spec RegisterDll(SLINKLIST<FIXTUREFACTORY>& fixtureFactoryListFromExe, ::CEEFITALLOCFUNC allocFuncFromExe, ::CEEFITFREEFUNC freeFuncFromExe);

};

#endif // __CEEFIT_H__
