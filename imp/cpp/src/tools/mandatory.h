/**
 * <p>Mandatory typedefs and #defines for all CeeFIT classes</p>
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

#ifndef __TOOLS_MANDATORY_H__
#define __TOOLS_MANDATORY_H__

#ifdef _MSC_VER
# define ceefit_init_spec __stdcall
# define ceefit_call_spec __cdecl
#else
# define ceefit_init_spec
# define ceefit_call_spec
#endif
namespace CEEFIT
{
  // Drastic measures ...
  // We have to deal with gcc's annoying "warning: passing NULL used for non-pointer converting" by completely removing any 
  // mention of NULL from the CeeFIT code.  From now on, use only CEEFIT::null when representing the NULL pointer (except
  // in modified 3rd party code, etc...)

#if defined(_MSC_VER) && _MSC_VER < 1300
  // this is sufficient for MSVC6, it doesn't like the NULLTYPE business below...
  static const int null = 0;
#else
  class NULLTYPE 
  {
    public:
      static NULLTYPE GlobalNull;

      inline ceefit_init_spec NULLTYPE(void) {}
      inline virtual ceefit_init_spec ~NULLTYPE(void) {}

      template<class T> inline ceefit_call_spec operator T*(void) const
      {
        return((T*) (void*) 0);
      }

      template<class T> inline ceefit_call_spec operator const T*(void) const
      {
        return((const T*) (const void*) 0);
      }

      inline ceefit_call_spec operator unsigned int(void) const
      {
        return(0);
      }

      inline bool ceefit_call_spec operator==(const NULLTYPE& anyPtr) const
      {
        return(true);
      }

      inline bool ceefit_call_spec operator!=(const NULLTYPE& anyPtr) const
      {
        return(false);
      }

      template<class T> inline bool ceefit_call_spec operator==(T anyPtr) const
      {
        return(anyPtr == 0);
      }

      template<class T> inline bool ceefit_call_spec operator!=(T anyPtr) const
      {
        return(anyPtr != 0);
      }

    private:
      ceefit_init_spec NULLTYPE(const NULLTYPE&);            /**< Not implemented, do not call. */
      NULLTYPE& ceefit_call_spec operator=(const NULLTYPE&); /**< Not implemented, do not call. */
  };
  
  static const NULLTYPE& null = NULLTYPE::GlobalNull;

  template<class T> inline bool ceefit_call_spec operator==(T& anyPtr, const NULLTYPE& aNull) 
  {
    return(anyPtr == 0);
  }

  template<class T> inline bool ceefit_call_spec operator!=(T& anyPtr, const NULLTYPE& aNull) 
  {
    return(anyPtr != 0);
  }
#endif
};

#endif // __TOOLS_MANDATORY_H__
