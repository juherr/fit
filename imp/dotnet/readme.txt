FIT for .NET
by Jim Shore and Ward Cunningham
based on FIT for Java
by Ward Cunningham

-------------

Quick Start:

Open a Visual Studio command prompt, change to the 'imp\dotnet'
directory of the Fit distribution, and type 'build'.  Fit will
build and run tests.  You should see it say "build successful."

There is a VS.NET solution file that includes the source code
and examples.  It's in the same 'dotnet' directory.

For more information about FIT, please see the FIT Wiki:
fit.c2.com

-------------

Differences in the C# version of FIT:

* Type parsing is done automatically.  There's no need to
create a "parse" method in your fixture.  In fact, that's not
supported.  If a type is mentioned in your fixture, make sure
it implements these three methods:
  - string ToString()
  - void Parse(string)
  - bool Equals(object)
All of the basic C# types implement those methods.  The
ScientificDouble class that comes with FIT has been augmented
to implement those methods.

* In order to load fixtures, C# has to know the location of
the fixture assembly.  The test runners have an extra
"fixture path" parameter that you can use to specify which
directory your fixture assembly is in.  This path is configured
to support common C# assembly paths by default.  See the
command-line help for more information.

-------------

Many thanks to Ward Cunningham for inventing FIT and asking me
to spearhead the C# version.

-------------

Jim Shore
Portland, OR
19 Oct 2002

(updated, 21 July 2004)