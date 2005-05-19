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

#include "ceefit/mandatory.h"

#include <new>
#include <assert.h>

extern "C"
{
  typedef void* (ceefit_call_spec * CEEFITALLOCFUNC)(size_t numBytes);
  typedef void (ceefit_call_spec * CEEFITFREEFUNC)(void* objPtr);
};

/**
 * <p>This function must be defined by the user, and must return a non-null alloc function</p>
 */
extern ::CEEFITALLOCFUNC ceefit_call_spec GetCeeFitAllocFunc(void);

/**
 * <p>This function must be defined by the user, and must return a non-null free function</p>
 */
extern ::CEEFITFREEFUNC ceefit_call_spec GetCeeFitFreeFunc(void);

extern "C++"
{
# ifdef __GNUC__
    extern void* operator new(std::size_t) throw (std::bad_alloc);
# else
    inline static void* ceefit_call_spec operator new(size_t size)
    {
      ::CEEFITALLOCFUNC allocFunc = GetCeeFitAllocFunc();

      void* retVal = allocFunc(size);
      return(retVal);
    }
# endif

# ifdef __GNUC__
    extern void operator delete(void*) throw();
# else
    inline static void ceefit_call_spec operator delete(void* obj)
    {
      if(obj != NULL)
      {
      ::CEEFITFREEFUNC freeFunc = GetCeeFitFreeFunc();

        freeFunc(obj);
      }
    }
# endif

# ifdef __GNUC__
    extern void* operator new[](std::size_t) throw (std::bad_alloc);
# else
    inline static void* ceefit_call_spec operator new[](size_t size)
    {
      ::CEEFITALLOCFUNC allocFunc = GetCeeFitAllocFunc();

      void* retVal = allocFunc(size);
      return(retVal);
    }
# endif

# ifdef __GNUC__
    extern void operator delete[](void*) throw();
# else
    inline static void ceefit_call_spec operator delete[](void* obj)
    {
      if(obj != NULL)
      {
      ::CEEFITFREEFUNC freeFunc = GetCeeFitFreeFunc();

        freeFunc(obj);
      }
    }
# endif

};

#endif // __TOOLS_ALLOC_H__
