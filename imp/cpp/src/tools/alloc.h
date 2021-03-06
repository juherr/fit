#ifndef __TOOLS_ALLOC_H__
#define __TOOLS_ALLOC_H__

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

#include "tools/mandatory.h"

#include <new>
#include <assert.h>
#include <memory.h>

#ifndef CEEFIT_ALLOC_FUNCS
# define CEEFIT_ALLOC_FUNCS

# if !(defined(_MSC_VER) || defined(__BORLANDC__))
#   define fit_size_t std::size_t
# else
#   define fit_size_t size_t
# endif

  extern "C"
  {
    typedef void* (ceefit_call_spec * CEEFITALLOCFUNC)(fit_size_t numBytes);
    typedef void (ceefit_call_spec * CEEFITFREEFUNC)(void* objPtr);
  }
#endif

/**
 * <p>This function must be defined by the user, and must return a non-null alloc function</p>
 */
extern ::CEEFITALLOCFUNC ceefit_call_spec GetCeeFitAllocFunc(void);

/**
 * <p>This function must be defined by the user, and must return a non-null free function</p>
 */
extern ::CEEFITFREEFUNC ceefit_call_spec GetCeeFitFreeFunc(void);

namespace CEEFIT 
{
  extern ::CEEFITALLOCFUNC OverriddenAllocFunc;
  extern ::CEEFITFREEFUNC OverriddenFreeFunc;
};

extern "C++"
{
# if !(defined(_MSC_VER) || defined(__BORLANDC__))
    extern void* operator new(fit_size_t) throw (std::bad_alloc);
# else
    inline static void* ceefit_call_spec operator new(fit_size_t size)
    {
      void* retVal = 0;      
      if(CEEFIT::OverriddenAllocFunc != 0) 
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
# endif

# if !(defined(_MSC_VER) || defined(__BORLANDC__))
    extern void operator delete(void*) throw();
# else
    inline static void ceefit_call_spec operator delete(void* obj)
    {
      if(obj != 0)
      {
        ::CEEFITFREEFUNC freeFunc = ((CEEFIT::OverriddenFreeFunc != 0) ? CEEFIT::OverriddenFreeFunc : GetCeeFitFreeFunc());

        freeFunc(obj);
      }
    }
# endif

# if !(defined(_MSC_VER) || defined(__BORLANDC__))
    extern void* operator new[](fit_size_t) throw (std::bad_alloc);
# else
    inline static void* ceefit_call_spec operator new[](fit_size_t size)
    {
      void* retVal = 0;      
      if(CEEFIT::OverriddenAllocFunc != 0) 
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
# endif

# if !(defined(_MSC_VER) || defined(__BORLANDC__))
    extern void operator delete[](void*) throw();
# else
    inline static void ceefit_call_spec operator delete[](void* obj)
    {
      if(obj != 0)
      {
        ::CEEFITFREEFUNC freeFunc = ((CEEFIT::OverriddenFreeFunc != 0) ? CEEFIT::OverriddenFreeFunc : GetCeeFitFreeFunc());

        freeFunc(obj);
      }
    }
# endif

};

#endif // __TOOLS_ALLOC_H__
