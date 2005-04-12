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

extern "C"
{
  /**
   * <p>Calls the default system operator new on numBytes.</p>
   *
   * <p>::operator new(numBytes) is called.</p>
   *
   * @param numBytes number of bytes that should be allocated.
   */
  extern void* ceefit_call_spec DefaultAlloc(size_t numBytes);

  /**
   * <p>Calls the default system operator delete on objPtr.</p>
   *
   * <p>::operator delete(objPtr) is called.</p>
   *
   * @param objPtr pointer to the object that should be freed.
   */
  extern void ceefit_call_spec DefaultFree(void* objPtr);

  extern ::CEEFITALLOCFUNC CeeFitAllocFunc;     /**< Alloc function that either points to DefaultAlloc or user specified func */
  extern ::CEEFITFREEFUNC CeeFitFreeFunc;       /**< Free function that either points to DefaultFree or user specified func */
};

namespace CEEFIT
{
  int ceefit_call_spec Run(const STRING& cmdLine)
  {
    if(CeeFitAllocFunc == NULL)
    {
      CeeFitAllocFunc = DefaultAlloc;
    }

    if(CeeFitFreeFunc == NULL)
    {
      CeeFitFreeFunc = DefaultFree;
    }

    DYNARRAY<STRING> argList;
    static STRING delimitList(" ");

    TokenizeRespectQuotes(argList, cmdLine, delimitList);

    //wprintf(L"%s %s %s %s\n", argList[0].GetBuffer(), argList[1].GetBuffer(), argList[2].GetBuffer(), argList[3].GetBuffer());

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
            fwprintf(stderr, L"usage:  %s -CeeFIT <input_file> <output_file>", argList[0].GetBuffer());
            retVal = 2;
          }

          if(retVal <= 1)
          {
            DYNARRAY<STRING> subList;
            subList.Add(argList[0]);
            subList.Add(argList[i]);
            subList.Add(argList[i+1]);
            subList.Add(argList[i+2]);

            FILERUNNER* fileRunner = new FILERUNNER();
            retVal = fileRunner->Run(subList);
            delete fileRunner;

            i += 2;
          }
        }
      }

      return(retVal);
    }
    return(0);
  }

  int ceefit_call_spec Run(const wchar_t* wideCmdLine)
  {
    return(Run(STRING(wideCmdLine)));
  }

  int ceefit_call_spec Run(const char* cmdLine)
  {
    return(Run(STRING(cmdLine)));
  }

  int ceefit_call_spec Run(int argc, char** argv)
  {
    if(CeeFitAllocFunc == NULL)
    {
      CeeFitAllocFunc = DefaultAlloc;
    }

    if(CeeFitFreeFunc == NULL)
    {
      CeeFitFreeFunc = DefaultFree;
    }

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

    return(Run(temp));
  }

  int ceefit_call_spec Run(int argc, wchar_t** argv)
  {
    if(CeeFitAllocFunc == NULL)
    {
      CeeFitAllocFunc = DefaultAlloc;
    }

    if(CeeFitFreeFunc == NULL)
    {
      CeeFitFreeFunc = DefaultFree;
    }

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

    return(Run(temp));
  }
};
