#include "ceefit.h"
#include "eg/dlltest/FoodFixture.h"

using namespace CEEFIT;

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

// ====== Program main entry point

int ceefit_call_spec main(int argc, char** argv)
{
  int exitCode = 0;

  // This gets the FIXTUREFACTORY classes out of the DLL and into the EXE prior to a call to CEEFIT::Run()
  DoRegisterFoodFixtureDll(RUNNER::GetFixtureFactoryList(), GetCeeFitAllocFunc(), GetCeeFitFreeFunc());

  int retVal;
  RESULTS runResults;
  if((retVal = CEEFIT::Run(argc, argv, runResults)) > 0) 
  {
    printf("%S\n", runResults.GetCountsSummary());
  }
  else 
  {
    printf("\nUsage:  %s -CeeFIT <inputFile> <outputFile>\n", argc > 0 ? argv[0] : "rundllfixture");
    printf("Description:  Tests calling FIXTURE's living in DLL's.\n\n");
  }
  return(exitCode);
}