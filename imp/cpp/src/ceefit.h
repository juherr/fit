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
 * calling convention.</li>
 * <li>Allow user to declare specializations of FITFIELD so that CeeFIT will parse and emit strings for custom data types.</li>
 * <li>Provide a single &quot;drop-in&quot; header for the user that provides everything needed to build and run CeeFIT tests.</li>
 * <li>Build and run with VC6.0 SP6</li>
 * <li>Build and run with GCC 3.4.1 (and higher, hopefully)</li>
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
#ifdef _MSC_VER
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
//#include <sys/utime.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <fcntl.h>
#include <io.h>
#include <math.h>

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

// Define up-front, mandatory symbols
#include "tools/mandatory.h"

// Tools and Java-like classes
#include "tools/object.h"
#include "tools/misc.h"
#include "tools/string.h"
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

extern "C"
{
  typedef void* (ceefit_call_spec * CEEFITALLOCFUNC)(size_t numBytes);
  typedef void (ceefit_call_spec * CEEFITFREEFUNC)(void* objPtr);
};

/**
 * <p>This function must be defined by the user, and must return a non-null alloc function</p>
 */
extern ::CEEFITALLOCFUNC ceefit_call_spec GetCeeFitAllocFunc(void);

/**
 * <p>This function must be defined by the user, and must return a non-null free function</p>
 */
extern ::CEEFITFREEFUNC ceefit_call_spec GetCeeFitFreeFunc(void);

namespace CEEFIT
{
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
   * <p>Same as Run() above that takes WinMain style command line.</p>
   */
  extern int ceefit_call_spec Run(const char* cmdLine, bool doReleaseStatics=true);

  /**
   * <p>Same as Run() above that takes WinMainW style command line (unicode string.)</p>
   */
  extern int ceefit_call_spec Run(const wchar_t* wideCmdLine, bool doReleaseStatics=true);
};

#endif // __CEEFIT_H__
