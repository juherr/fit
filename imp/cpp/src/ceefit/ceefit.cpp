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

#include "tools/alloc.h"
#include "ceefit.h"

force_link_fit_module(FitActionFixture);

namespace CEEFIT
{
  /**
   * <p>Releases any references to any allocated static members at the end of a call to CEEFIT::Run</p>
   */
  void ceefit_call_spec ReleaseStatics()
  {
    SLINKLIST<FIXTUREFACTORY>& factoryList = RUNNER::GetFixtureFactoryList();

    FIXTUREFACTORY* aFixtureFactory = factoryList.GetHead();
    while(aFixtureFactory != null) 
    {
      PTR<FIXTURE> aFixture;
      aFixtureFactory->CreateFixture(aFixture);

      aFixture->ReleaseStatics();

      aFixtureFactory = aFixtureFactory->GetNext();
    }
  }

  void RESULTS::SetCountsSummary(const wchar_t* resultsStr)
  {
    if(resultsStr != null)
    {
      wcsncpy(&CountsSummary[0], resultsStr, 255);
      CountsSummary[255] = L'\0';
    }
  }

  int ceefit_call_spec Run(const STRING& cmdLine, RESULTS& outResults, bool doReleaseStatics)
  {
    DYNARRAY<STRING> argList;
    STRING delimitList(" ");

    TokenizeRespectQuotes(argList, cmdLine, delimitList);

    //printf("%S %S %S %S\n", argList[0].GetBuffer(), argList[1].GetBuffer(), argList[2].GetBuffer(), argList[3].GetBuffer());

    if(argList.GetSize() >= 4)
    {
      int retVal = 0;

      int i = 0;
      while(++i < argList.GetSize())
      {
        if(IsEqual(argList[i], "-CeeFIT"))
        {
          if(argList.GetSize() - i < 3)
          {
            printf("usage:  %S -CeeFIT <input_file> <output_file>", argList[0].GetBuffer());
            retVal = 2;
          }

          if(retVal <= 1)
          {
            DYNARRAY<STRING> subList;
            subList.Add(argList[0]);
            subList.Add(argList[i]);
            subList.Add(argList[i+1]);
            subList.Add(argList[i+2]);

            PTR<FILERUNNER> fileRunner;
            try 
            {
              fileRunner = new FILERUNNER();
              retVal = fileRunner->Run(subList);
              
              if(doReleaseStatics)
              {
                ReleaseStatics();
              }
            }
            catch(EXCEPTION* e) 
            {
              STRING message;

              message = STRING("An unhandled exception occurred:  ") + (e != null ? e->GetReason() : STRING("<unknown reason>"));
              printf("%S\n", message.GetBuffer());

              delete e;              
            }
            catch(FAILURE* f) 
            {
              STRING message;

              message = STRING("A failure occurred:  ") + (f != null ? f->GetReason() : STRING("<unknown reason>"));
              printf("%S\n", message.GetBuffer());

              delete f;              
            }
            catch(...) 
            {
              printf("An unknown exception occurred.\n");
            }

            // New 03/25/06 DW - Communicate results to caller before fileRunner is dereferenced
            if(fileRunner != null && fileRunner->Fixture != null && fileRunner->Fixture->CountsObj != null)
            {              
              outResults.SetRight(fileRunner->Fixture->CountsObj->right);
              outResults.SetWrong(fileRunner->Fixture->CountsObj->wrong);
              outResults.SetIgnores(fileRunner->Fixture->CountsObj->ignores);
              outResults.SetExceptions(fileRunner->Fixture->CountsObj->exceptions);

              STRING countsString(fileRunner->Fixture->Counts());
              outResults.SetCountsSummary(countsString.GetBuffer());
            }

            i += 2;   // the while loop increments i for a total of 3...
          }
        }
      }

      return(retVal);
    }
    return(0);
  }

  int ceefit_call_spec Run(const wchar_t* wideCmdLine, bool doReleaseStatics)
  {
    RESULTS dummyResults;

    return(Run(STRING(wideCmdLine), dummyResults, doReleaseStatics));
  }

  int ceefit_call_spec Run(const wchar_t* wideCmdLine, RESULTS& outResults, bool doReleaseStatics)
  {
    return(Run(STRING(wideCmdLine), outResults, doReleaseStatics));
  }

  int ceefit_call_spec Run(const char* cmdLine, bool doReleaseStatics)
  {
    RESULTS dummyResults;

    return(Run(STRING(cmdLine), dummyResults, doReleaseStatics));
  }

  int ceefit_call_spec Run(const char* cmdLine, RESULTS& outResults, bool doReleaseStatics)
  {
    return(Run(STRING(cmdLine), outResults, doReleaseStatics));
  }

  int ceefit_call_spec Run(int argc, char** argv, bool doReleaseStatics)
  {
    RESULTS dummyResults;

    return(Run(argc, argv, dummyResults, doReleaseStatics));
  }

  int ceefit_call_spec Run(int argc, char** argv, RESULTS& outResults, bool doReleaseStatics)
  {
    STRING temp;
    int i = -1;

    while(++i < argc)
    {
      if(i != 0)
      {
        temp += STRING(" ");
      }

      STRING aArg(argv[i]);
      if(aArg.IndexOf(" ") >= 0 && !aArg.StartsWith("\""))
      {
        temp += STRING("\"") + aArg + "\"";
      }
      else
      {
        temp += aArg;
      }
    }

    RESULTS dummyResults;

    return(Run(temp, outResults, doReleaseStatics));
  }

  int ceefit_call_spec Run(int argc, wchar_t** argv, bool doReleaseStatics)
  {
    RESULTS dummyResults;

    return(Run(argc, argv, dummyResults, doReleaseStatics));
  }

  int ceefit_call_spec Run(int argc, wchar_t** argv, RESULTS& outResults, bool doReleaseStatics)
  {
    STRING temp;
    int i = -1;

    while(++i < argc)
    {
      if(i != 0)
      {
        temp += STRING(" ");
      }

      STRING aArg(argv[i]);
      if(aArg.IndexOf(" ") >= 0 && !aArg.StartsWith("\""))
      {
        temp += STRING("\"") + aArg + "\"";
      }
    }

    return(Run(temp, outResults, doReleaseStatics));
  }

  void ceefit_call_spec RegisterDll(SLINKLIST<FIXTUREFACTORY>& fixtureFactoryListFromExe, ::CEEFITALLOCFUNC allocFuncFromExe, ::CEEFITFREEFUNC freeFuncFromExe)
  {
    SLINKLIST<FIXTUREFACTORY>& dllFixtureFactoryList = RUNNER::GetFixtureFactoryList();

    OverriddenAllocFunc = allocFuncFromExe;
    OverriddenFreeFunc = freeFuncFromExe;

    FIXTUREFACTORY* aDllFixtureFactory = null;
    while((aDllFixtureFactory = dllFixtureFactoryList.GetHead()) != null) 
    {      
      // move each FIXTUREFACTORY from the DLL's FixtureFactory list to the EXE's
      dllFixtureFactoryList.RemoveObject(aDllFixtureFactory);
      fixtureFactoryListFromExe.AddTail(aDllFixtureFactory);
    }
  }
};
