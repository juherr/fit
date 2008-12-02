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
 * <p>(c)2006 Woldrich, Inc.  <a href="http://www.woldrich.com">http://www.woldrich.com</a></p>
 *
 * @author David Woldrich
 */

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
    printf("Exit code will be:  %i\n", retVal);

    exitCode = retVal;
  }
  else 
  {
    printf("\nUsage:  %s -CeeFIT <inputFile> <outputFile>\n", argc > 0 ? argv[0] : "rundllfixture");
    printf("Description:  Tests calling FIXTURE's living in DLL's.\n\n");
  }
  return(exitCode);
}