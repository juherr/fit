#include "ceefit.h"

/**
 * <p>CeeFIT runner for the Fit Specification fixtures</p>
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

// Force the linkage of the "fat" example CeeFIT FIXTURES
force_link_fit_module(FatEquals);
force_link_fit_module(FatDivide);
force_link_fit_module(FatTable);
force_link_fit_module(ParseFixture);
force_link_fit_module(OutputFixture);
force_link_fit_module(ReferenceFixture);
force_link_fit_module(Summary);
force_link_fit_module(FatFixtureNameFixture);
force_link_fit_module(FatAnnotationFixture);
force_link_fit_module(FatColor);
force_link_fit_module(FatDocumentParseFixture);
force_link_fit_module(FatHtmlToTextFixture);
force_link_fit_module(FatTextToHtmlFixture);
force_link_fit_module(TableParseFixture);
force_link_fit_module(FatFixtureLoadFixture);
force_link_fit_module(FatNotAFixture);
force_link_fit_module(FatExampleFixture);
force_link_fit_module(FatStandardAnnotationFixture);

// Define the Alloc and Free functions that CeeFIT should use

static void* ceefit_call_spec FatAlloc(fit_size_t numBytes)
{
  return(malloc(numBytes));
}

static void ceefit_call_spec FatFree(void* objPtr)
{
  free(objPtr);
}

::CEEFITALLOCFUNC ceefit_call_spec GetCeeFitAllocFunc()
{
  return(FatAlloc);
}

::CEEFITFREEFUNC ceefit_call_spec GetCeeFitFreeFunc()
{
  return(FatFree);
}

/**
 * <p>Main function for the Fit Specification runner</p>
 *
 * <p>By nature of the fact that all the "fat" library modules were forcibly linked above, the CEEFIT::Run call below will
 * have all of the tools needed to successfully run the Fit Specification html files.</p>
 */
int ceefit_call_spec main(int argc, char** argv)
{
# ifdef _DEBUG
#   ifdef DUMP_MSVC_MEMORY_LEAKS
      _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#   endif
# endif

  int i = 0;
  while(++i < argc) 
  {
    if(strcmp(argv[i], "-CeeFIT"))
    {
      printf("fitspec runner:  Detected parameter before seeing \"-CeeFIT\" that will be ignored:  %s\n", argv[i]);
    }
    else 
    {
      break;
    }
  }

  int retVal = CEEFIT::Run(argc, argv);

  if(retVal == 0)
  {
    printf("\nUsage:  %s -CeeFIT <inputFile> <outputFile>\n", argc > 0 ? argv[0] : "fitspec");
    printf("Description:  Runs the CeeFIT libraries with all \"fat\" modules linked in.  This\n");
    printf("              program is suitable for running the Fit Specification.  Failures in\n");
    printf("              running the spec indicate that this version of CeeFIT is out of date.\n\n");
  }

  return(0);
}
