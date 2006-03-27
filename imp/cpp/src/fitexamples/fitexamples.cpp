/**
 * <p>CeeFIT runner for the Fit Example fixtures</p>
 *
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
#include "eg/eg.h"

using namespace CEEFIT;

force_link_fit_module(Summary);
force_link_fit_module(BrowserFixture);
force_link_fit_module(MusicDialog);
force_link_fit_module(MusicDisplay);
force_link_fit_module(MusicClass);
force_link_fit_module(MusicLibrary);
force_link_fit_module(MusicPlayer);
force_link_fit_module(MusicRealtime);
force_link_fit_module(MusicSimulator);
force_link_fit_module(SqrtFixture);
force_link_fit_module(DivisionFixture);
force_link_fit_module(CalculatorFixture);
force_link_fit_module(ExampleTestsFixture);

// Define the Alloc and Free functions that CeeFIT should use

static void* ceefit_call_spec FitExamplesAlloc(fit_size_t numBytes)
{
  return(new unsigned char[numBytes]);
}

static void ceefit_call_spec FitExamplesFree(void* objPtr)
{
  if(objPtr != null)
  {
    delete [] ((unsigned char*) objPtr);
  }
}

::CEEFITALLOCFUNC ceefit_call_spec GetCeeFitAllocFunc()
{
  return(FitExamplesAlloc);
}

::CEEFITFREEFUNC ceefit_call_spec GetCeeFitFreeFunc()
{
  return(FitExamplesFree);
}

/**
 * <p>Main function for the Fit Examples runner</p>
 */
int ceefit_call_spec main(int argc, char** argv)
{
# ifdef _DEBUG
#   ifdef DUMP_MSVC_MEMORY_LEAKS
      _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#   endif
# endif

  CEEFIT::RESULTS runResults;
  int retVal = CEEFIT::Run(argc, argv, runResults);

  if(retVal == 0)
  {
    printf("\nUsage:  %s -CeeFIT <inputFile> <outputFile>\n", argc > 0 ? argv[0] : "fitspec");
    printf("Description:  Runs the CeeFIT libraries with all \"example\" modules linked in.  This\n");
    printf("              program is suitable for running the Fit Examples.\n\n");
  }
  else
  {
    printf("%S\n", runResults.GetCountsSummary());
  }

  return(0);
}
