Release notes:  CeeFIT version 0.5
David Woldrich
01/31/2005


IN A NUTSHELL:
-------------

* CeeFIT is a Java Fit-like FileRunner written for C++ supported by a heavy
  dose of macros and (hopefully universally compatible) templates.
* Code compiles and links successfully with MSVC 6.0 SP6.
* Code compiles BUT DOES NOT LINK with g++, please help.
* Java, Ant, Ant-Contrib, CppTasks used for build system.
* Make a build.properties to configure Ant for your build environment.
* CeeFIT is released under the GNU GPL.


INTRODUCTION (elevator pitch for Fit in general):
------------------------------------------------

[from the main site:  http://fit.c2.com]

Fit: Framework for Integrated Test

Great software requires collaboration and communication. Fit is a tool for 
enhancing collaboration in software development. It's an invaluable way to 
collaborate on complicated problems--and get them right--early in development.

Fit allows customers, testers, and programmers to learn what their software
should do and what it does do. It automatically compares customers' 
expectations to actual results.

Learn how Fit does it by reading our 
IntroductionToFit (http://fit.c2.com/wiki.cgi?IntroductionToFit). 
Then DownloadNow (http://fit.c2.com/wiki.cgi?DownloadNow). Fit is freely 
available under the GNU General Public License. 


CeeFIT OVERVIEW:
---------------

CeeFIT is an attempt to mimic the capabilities and behavior of the Java Fit
with C++.  Initial goals of CeeFIT are:

  * Link statically to an application rather than as a DLL to simplify things.
  * In the absence of reflection in C++ make CeeFIT somehow automagically 
    register fixtures with the engine.
  * Make ColumnFixtures as easy to write for CeeFIT as they are in the Java 
    version (used macros and template classes to do this.)
  * Get CeeFIT to run with both MinGW (g++) and Microsoft Visual C++ 6.0 SP6.
    (I failed to complete this goal.)
  * Pass and fail the Fit Specification documents in the same exact way that
    the Java version does (for the most part, CeeFIT can do this ... still
    more work to do here to implement all Fixtures and all Fat examples.)

Because I perceived ColumnFixture as one of the better uses of Fit, my main 
goal was to get all the same kind of ease of use and power out of that one 
part of Fit in CeeFIT.  What I arrived at was a system of macros and 
supporting template functions that automatically register Fixture classes
that does ColumnFixture very well.  Other complex fixture types like 
ActionFixture can probably be reproduced in CeeFIT with a little effort, but
I have not yet had the chance to get that part done.

I plan in the future to allow for manual registration of Fixture classes so
that users who find macros distasteful can handle everything themselves. For
now, please suffer with the macros until I figure out a way to make manually
registered Fixture classes that can extend from macro ones (and vice versa.)

C++ does not have the same packaging system as Java, and I did not see any
easy way to map C++ Fixture class names to the fully qualified Java class 
names you see listed in most Fit specification documents.  Therefore, the 
Fixture classes you create in CeeFIT may specify a string alias as an 
alternate match for Fixture table names.  Other Fit implementations have 
used the same method for matching fixture tables to Fixture classes.


IMPLEMENTING YOUR APPLICATION WITH CeeFIT:
-----------------------------------------

1. Your main() function with CeeFIT

CeeFIT is a static library that you link into your application.  In your 
main() function, after you've called all of your initialization routines, 
invoke the CEEFIT::Run() method with the list of command line parameters
passed by the user.  The Run method searches the command line for the 
following three parameters:

  -CeeFIT <source HTML filepath> <output HTML filepath>

If CEEFIT::Run() finds the parameters it is looking for, it will create a
FileRunner class and process the source HTML file, saving the output to the
output HTML filepath.  

In addition, you must specify to CeeFIT the heap you use in your program.  
If, for example, you are using the garbage collecting Boehm GC heap in your 
program, you must specify to CeeFIT to use that heap.  If you are using the
default new and delete, you must specify function calls to those as well.
The two functions you will need to create that CeeFIT requires are 
GetCeeFitAllocFunc(), which returns a function pointer to an allocator, and
GetCeeFitFreeFunc(), which returns a function pointer to a deallocator.

See the documentation in src/ceefit.h for more details on CEEFIT::Run().  
src/fitspec/fitspec.cpp has an example of what a simple main() looks like 
(including the definitions of GetCeeFitAllocFunc() and GetCeeFitFreeFunc().)

In general, you should only need to include src/ceefit.h at the top of any
module that needs to use CeeFIT.

2. Defining C++ FIXTURE classes

You must have C++ classes to match to each of the Fixtures named in the
tables in the source HTML files that your SME's will produce.  These C++ 
classes are generated using macros supplied with CeeFIT, and typically they
are not accompanied by any header files.  See the src/fat sample .cpp files
for ideas on how to write Fixtures.  

fit_test() and fit_field() are the two macros you will see inside of the
fixtures you create.  begin_fit_fixture() and end_fit_fixture() mark the
beginning and end of the the C++ class that is created for your fixture. All
of the macros are defined in src/ceefit/macros.h, and they are a bit crazy.

3. Leveraging your own datatypes in CeeFIT (as needed)

CeeFIT requires a template definition to handle the serialization and 
deserialization of Fixture fields to strings.  This is how CeeFIT knows how
to read and write HTML files containing your Fixture data.  The template
class is FITFIELD. 

CeeFIT supports the following types "out-of-the-box" for fields in your 
fixtures:

  bool
  unsigned char
  signed char
  char
  unsigned short
  signed short
  wchar_t
  unsigned int
  signed int
  unsigned long
  signed long
  UINT64
  INT64
  float
  double
  CEEFIT::STRING
  
if those types do not cover your needs, or if you have a custom struct
or class you wish to register for your Fixture, you may register additional
FITFIELD specializations for your special types.  See src/ceefit/fitfield.h
for ideas on how to implement your own custom FITFIELD specialization.  The
Fat examples library also has some examples of additional FITFIELD 
specializations (see the src/fat/testtypes.h file.)

4. Telling the linker to link in your Fixtures

Linkers for C++ require that all modules map into a dependency graph that 
ultimately maps back to the module containing the main() method.  If a module
is not referenced, it is usually not linked in to the final executable.  
Because of that, you will likely need to force a dependency to be created 
between the main() method's module and each of your Fixture's modules.  

If you find that your Fixtures' modules are compiling but not linking into the
final executable, use the supplied macros to force the link to occur.  Use the 
declare_fit_module() macro in your Fixture module with a unique module name.  
Use the force_link_fit_module() macro with the same module name in the main() 
method's module.  This will force the linker to link in the Fixture's module 
into the final executable.

See examples of declare_fit_module() and force_link_fit_module() in the src/fat
examples and the src/fitspec/fitspec.cpp.


GETTING CeeFIT TO BUILD EVERYWHERE:
----------------------------------

CeeFIT is one of those libraries that should build everywhere on most every 
C++ compiler in order to be useful and relevant to the C++ community.  That
may be an unattainable goal for a multitude of reasons, but mostly because of
varying template support.  I did want to support Microsoft Visual C++ 6.0 SP6
and GCC just so that I could cover both the Microsoft and UNIX/Linux worlds.

In order to automate the build on the multitude of platforms, I used ant,
ant-contrib, and cpptasks to script the build.  Cpptasks has not seen very
many updates in the last couple of years, but it is still very good and covers
many different compilers and linkers.  

  * You will need to have Java 1.4.2 installed (http://java.sun.com)
  * You will need a JAVA_HOME environment variable set to the Java home dir.

I use environment variables to find where Visual C++ or GCC are installed.

  * If you have MSVC, you will need to set MSVCDir to c:\msdev\VC98 (or 
    wherever you have Visual C++ installed
  * If you have GCC, you will need to set GCC_HOME to c:\mingw (or wherever 
    you have GCC installed.)

The build.xml ant script provided with CeeFIT can load a build.properties file
to set default properties.  Copy build.properties.example to build.properties
and edit it to suit your environment.

The ant command line is:  ant [switches] <target name>

The common targets are:  clean, build, test, all

Run ant without any targets to get help.  Use the -msvc switch to force Visual
C++ builds.  Use the -g++ switch to force GCC build.

When you run "ant test" or "ant all", it will place the CeeFIT output from
running the 
Fit Specification in the ./output folder.

Currently GCC is able to compile the CeeFIT code, but I have been unable to 
get it to link successfully.  GCC uses the Boost libraries to provide Unicode
regular expression support, and that seems to be part of the problems.  I
think having IBM's ICU instead of Boost for Unicode support and regular 
expressions in the long term is the best way to go especially as they follow
my philosophy of not requiring STL.  Boost requires STL, and that is the core
of the linking issues I had with GCC.  Unfortunately, ICU does not currently
build to static libraries on all platforms, so I did not opt to use it for 
this version of CeeFIT.

Please help me figure this out!  Please help me get GCC builds to work, I feel
like I've tried everything, but I'm no expert at GCC and it's probably 
something very simple.  Also, please help me convert ant.bat to a shell 
script and make a UNIX/Linux version.  (Thanks!)


CeeFIT NAMING CONVENTION:
------------------------

CeeFIT uses the following naming conventions for symbols:

  * Class : All uppercase, no underscores (unless namespaces are unavailable)
  * Namespace : All uppercase, no underscores
  * Macros, enums : All lowercase, underscores separating words
  * Local variables, parameters : Camelback, first letter small
  * Global, static, members : Camelback, first letter big

Sorry if that hurts your eyes, but I like it this way.  If I messed up 
anywhere, it needs to be fixed.


IMPORTANT:
---------

While CeeFIT is compatible functionally with the Java version for the most 
part, it's internals are still subject to change while some of the more 
challenging aspects of CeeFIT are worked out.  When new versions come
out, I may change some CeeFIT specifics that may break your builds in order
to complete some of the todos below.  (Hey, it's only version 0.5, only half
way done ... whaddaya expect??  ;)


TODOS:
-----

  * Get doxygen doc generation working in the ant build script
  * Get GCC/MinGW to link successfully
  * Write an ant.sh shell script for UNIX/Linux
  * Create a manual Fixture registration scheme that is compatible with the
    macro one.


EXAMPLE COLUMNFIXTURE PROJECT:
-----------------------------

What follows is a little example program demonstrating CeeFIT and
ColumnFixture.  In a nutshell, CeeFIT expects to be statically linked (along
with any fixtures it will need) into a host program.  (This example is checked
into CVS as a MSVC6 project called "simple".  In addition to "simple", there 
is a "fitspec" project which builds and runs the Fit Specification documents 
through CeeFIT.)

To activate CeeFIT within the host program, it requires command line
parameters as a trigger.  For this example program the command line would be:

  simple.exe -CeeFIT simple.html simpleOut.html

If the CeeFIT command line parameters are not found, the host program will be
allowed to run its normal main loop method.

Build the code below as a console application with RTTI and Exception Handling
enabled, and link with the ceefit.lib.  The resulting simple.exe should be 
sufficient for running the simple.html test file.


[[[[[[[[[[[[[[multiply.cpp]]]]]]]]]]]]]]]]

#include "ceefit.h"

// Give main.cpp something to latch onto when linking ...
// If you can command your linker to force a linkage to multiply.o, then 
// this statement is not-required.
declare_fit_module(ExampleMultiply);


// Start a Fixture class called MULTIPLY that extends CEEFIT::COLUMNFIXTURE
// It has a name alias for a matching Java Fixture somewhere called
// "fitexample.Multiply"
begin_fit_fixture(MULTIPLY, CEEFIT::COLUMNFIXTURE, fitexample.Multiply);

  // This is the int column variable called "x"
  fit_var(int, x);

  // This is the int column variable called "y"
  fit_var(int, y);

  // This is the multiply() fixture test, it returns an int
  fit_test(multiply, int)
  {
    return (x * y);
  }

// Close up the Fixture class, notice I had to match the MULTIPLY
// class name from begin_fit_fixture as a parameter here
end_fit_fixture(MULTIPLY);






[[[[[[[[[[[[[[[[main.cpp]]]]]]]]]]]]]]]]]]

#include "ceefit.h"

// ====== Init Release and MainLoop

void InitOpenGL()         { /* this inits something flashy */ }
void InitDirectSound()    { /* this inits something noisy */ }
void SynchronizeClocks()  { /* this inits something networky */ }
void ReleaseClocks()      { /* world time stops */ }
void ReleaseDirectSound() { /* volume == 0 */ }
void ReleaseOpenGL()      { /* fade to black */ }

int MainLoop()           
{ 
  printf("CeeFIT didn't run, forget command line params?\n");
  return(0); 
}


// ====== CeeFIT support routines

/**
 * <p>CeeFIT needs to know what heap alloc function your program uses.</p>
 *
 * <p>If yours isn't malloc, make a CEEFITALLOCFUNC that calls your allocator
 * and return that function pointer here.<p>
 */
CEEFITALLOCFUNC ceefit_call_spec GetCeeFitAllocFunc()
{
  return(malloc);
}

/**
 * <p>CeeFIT needs to know what heap free function your program uses.</p>
 *
 * <p>If yours isn't free, make a CEEFITFREEFUNC that calls your allocator
 * and return that function pointer here.<p>
 */
CEEFITFREEFUNC ceefit_call_spec GetCeeFitFreeFunc()
{
  return(free);
}

// Latch onto the ExampleMultiply variable from multiply.cpp
// If you can command your linker to force a linkage to multiply.o, then 
// this statement is not-required.
force_link_fit_module(ExampleMultiply);


// ====== Program main entry point

/**
 * <p>The main function for your existing program ...</p>
 */
int __cdecl main(int argc, char** argv)
{
  int exitCode = 0;

  InitOpenGL();
  InitDirectSound();
  SynchronizeClocks();

  int retVal;
  if((retVal = CEEFIT::Run(argc, argv)) > 0) 
  {
    printf("\nCeeFIT:  %s\n", retVal == 1 ? "pass" : "fail");
  }
  else
  {
    exitCode = MainLoop(); /* run the program normally if CeeFIT did nothing */
  }

  ReleaseClocks();
  ReleaseDirectSound();
  ReleaseOpenGL();

  return(exitCode);
}





[[[[[[[[[[[[[[[simple.html]]]]]]]]]]]]]]]

<html>
<body>
<p>This exercises the MULTIPLY fixture using the C++ class name</p>
<table width="100%" border="1">
  <tr> 
    <td colspan="3">MULTIPLY</td>
  </tr>
  <tr> 
    <td>x</td>
    <td>y</td>
    <td>multiply()</td>
  </tr>
  <tr> 
    <td>0</td>
    <td>0</td>
    <td>0</td>
  </tr>
  <tr> 
    <td>800</td>
    <td>2</td>
    <td>1600</td>
  </tr>
</table>
<p>&nbsp;</p>
<p>This exercises the MULTIPLY fixture using the Java alias.</p>
<table width="100%" border="1">
  <tr> 
    <td colspan="3">fitexample.Multiply</td>
  </tr>
  <tr> 
    <td>x</td>
    <td>y</td>
    <td>multiply()</td>
  </tr>
  <tr> 
    <td>0</td>
    <td>10</td>
    <td>0</td>
  </tr>
  <tr> 
    <td>5</td>
    <td>5</td>
    <td>25</td>
  </tr>
  <tr> 
    <td>6</td>
    <td>9</td>
    <td>54</td>
  </tr>
  <tr> 
    <td>-3</td>
    <td>6</td>
    <td>-18</td>
  </tr>
  <tr>
    <td>-4</td>
    <td>-2</td>
    <td>8</td>
  </tr>
</table>
<p>&nbsp;</p>
</body>
</html>


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
