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

#ifdef _WIN32
# include <windows.h>
#endif

namespace CEEFIT 
{
  ::CEEFITALLOCFUNC OverriddenAllocFunc = null;     // useful for overriding the heap functions in a DLL with those of an exe
  ::CEEFITFREEFUNC OverriddenFreeFunc = null;       // useful for overriding the heap functions in a DLL with those of an exe
}

extern "C++"
{
# if !(defined(_MSC_VER) || defined(__BORLANDC__))
    void* operator new(fit_size_t size) throw (std::bad_alloc)
    {
      void* retVal = CEEFIT::null;      
      if(CEEFIT::OverriddenAllocFunc != CEEFIT::null) 
      {
        retVal = CEEFIT::OverriddenAllocFunc(size);
      }
      else 
      {
        retVal = GetCeeFitAllocFunc()(size);
      }

      if(retVal != 0) 
      {
        memset(retVal, 0, size);
      }

      return(retVal);
    }

    void operator delete(void* obj) throw()
    {
      if(obj != CEEFIT::null)
      {
        if(CEEFIT::OverriddenFreeFunc != CEEFIT::null) 
        {
          CEEFIT::OverriddenFreeFunc(obj);
        }
        else 
        {
          GetCeeFitFreeFunc()(obj);
        }
      }
    }

    void* operator new[](fit_size_t size) throw (std::bad_alloc)
    {
      void* retVal = CEEFIT::null;      
      if(CEEFIT::OverriddenAllocFunc != CEEFIT::null) 
      {
        retVal = CEEFIT::OverriddenAllocFunc(size);
      }
      else 
      {
        retVal = GetCeeFitAllocFunc()(size);
      }

      if(retVal != 0) 
      {
        memset(retVal, 0, size);
      }

      return(retVal);
    }

    void operator delete[](void* obj) throw()
    {
      if(obj != CEEFIT::null)
      {
        if(CEEFIT::OverriddenFreeFunc != CEEFIT::null) 
        {
          CEEFIT::OverriddenFreeFunc(obj);
        }
        else 
        {
          GetCeeFitFreeFunc()(obj);
        }
      }
    }
# endif  
};
