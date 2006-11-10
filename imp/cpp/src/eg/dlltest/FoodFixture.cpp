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

#include <windows.h>
#include <stdio.h>
#include "tools/alloc.h"      // THIS INCLUDE IS ABSOLUTELY VITAL:  without it, your DLL fit fixtures may use a different heap than the EXE and you'll get a crash
#include "ceefit.h"

using namespace CEEFIT;

::CEEFITALLOCFUNC ceefit_call_spec GetCeeFitAllocFunc() 
{
  return(NULL);
}

::CEEFITFREEFUNC ceefit_call_spec GetCeeFitFreeFunc()
{
  return(NULL);
}

namespace EG_DLLTEST
{
  begin_namespaced_fit_fixture(EG_DLLTEST, FOODFIXTURE, CEEFIT::COLUMNFIXTURE, "fit.eg_dlltest.FoodFixture")
    public:
      fit_var(CEEFIT::STRING, foodName);

      fit_test(goodProtein, bool)
      {
        if(foodName.IsEqual("beans")) 
        {
          return(true);
        }
        else if(foodName.IsEqual("dill pickles")) 
        {
          return(false);
        }
        else if(foodName.IsEqual("beef")) 
        {
          return(true);
        }
        else if(foodName.IsEqual("soda pop")) 
        {
          return(false);
        }
        else if(foodName.IsEqual("ice cream")) 
        {
          return(true);
        }
        return(false);
      }

      fit_test(highSugar, bool)
      {
        if(foodName.IsEqual("beans")) 
        {
          return(false);
        }
        else if(foodName.IsEqual("dill pickles")) 
        {
          return(false);
        }
        else if(foodName.IsEqual("beef")) 
        {
          return(false);
        }
        else if(foodName.IsEqual("soda pop")) 
        {
          return(true);
        }
        else if(foodName.IsEqual("ice cream")) 
        {
          return(true);
        }
        return(false);
      }
  end_namespaced_fit_fixture(EG_DLLTEST, FOODFIXTURE)
};

extern "C" 
{
  BOOL APIENTRY DllMain (HINSTANCE hInst, DWORD reason, LPVOID reserved /* Not used. */ ) 
  {
    switch (reason)
      {
      case DLL_PROCESS_ATTACH:
        break;

      case DLL_PROCESS_DETACH:
        break;

      case DLL_THREAD_ATTACH:
        break;

      case DLL_THREAD_DETACH:
        break;
      }
    return TRUE;
  }

  /**
   * <p>This is called by an exe running the fit tests</p>
   */
  __declspec(dllexport) void ceefit_call_spec DoRegisterFoodFixtureDll(SLINKLIST<FIXTUREFACTORY>& fixtureFactoryListFromExe, ::CEEFITALLOCFUNC allocFuncFromExe, ::CEEFITFREEFUNC freeFuncFromExe)
  {
    CEEFIT::RegisterDll(fixtureFactoryListFromExe, allocFuncFromExe, freeFuncFromExe);
  }
};
