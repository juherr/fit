/**
 * <p>This file is part of CeeFIT.</p>
 *
 * <p>CeeFIT is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.</p>
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
 * @author David Woldrich
 */

#include "ceefit.h"

// Give main.cpp something to latch onto when linking ...
// If you can command your linker to force a linkage to multiply.obj or it
// is linking it in automatically, then this statement is not-required.
declare_fit_module(ExampleMultiply);

namespace SIMPLE
{
  // Start a Fixture class called SIMPLE::MULTIPLY that extends CEEFIT::COLUMNFIXTURE
  // It has a name alias for a matching Java Fixture somewhere called
  // "fitexample.Multiply"
  begin_namespaced_fit_fixture(SIMPLE, MULTIPLY, CEEFIT::COLUMNFIXTURE, fitexample.Multiply)

    // This is the int column variable called "x"
    fit_var(int, x);

    // This is the int column variable called "y"
    fit_var(int, y);

    // This is the multiply() fixture test, it returns an int
    fit_test(multiply, int)
    {
      return (x * y);
    }

  // Close up the Fixture class, notice I had to match the SIMPLE::MULTIPLY
  // namespace name and class name from begin_namespaced_fit_fixture as parameters here
  end_namespaced_fit_fixture(SIMPLE, MULTIPLY)
};