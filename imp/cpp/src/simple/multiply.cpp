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