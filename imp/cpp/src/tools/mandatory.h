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

#if defined(__BORLANDC__)
# define ceefit_init_spec __stdcall
# define ceefit_dtor_spec __stdcall
# define ceefit_call_spec __cdecl
#elif defined(_MSC_VER)
# define ceefit_init_spec __stdcall
# define ceefit_dtor_spec
# define ceefit_call_spec __cdecl
#else
# define ceefit_init_spec
# define ceefit_dtor_spec
# define ceefit_call_spec
#endif

#ifdef __cplusplus

namespace CEEFIT
{
  // Drastic measures ...
  // We have to deal with gcc's annoying "warning: passing NULL used for non-pointer converting" by completely removing any
  // mention of NULL from the CeeFIT code.  From now on, use only CEEFIT::null when representing the NULL pointer (except
  // in modified 3rd party code, etc...)

#if defined(_MSC_VER) && _MSC_VER < 1300 && !defined(__BORLANDC__)
  // this is sufficient for MSVC6, it doesn't seem to like the NULLTYPE business below...
  static const int null = 0;
#else
  class NULLTYPE
  {
    public:
      inline ceefit_init_spec NULLTYPE(void) {}
      inline virtual ceefit_dtor_spec ~NULLTYPE(void) {}

      template<class T> inline ceefit_call_spec operator T*(void) const
      {
        return((T*) (void*) 0);
      }

#     ifndef __BORLANDC__    // Borland C++ chokes on this caster
        template<class T> inline ceefit_call_spec operator const T*(void) const
        {
          return((const T*) (const void*) 0);
        }
#     endif

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

  // Note:  11/12/05:  This declaration of null used to be a reference rather than a static object.  This change was made to be more friendly with 
  // Borland C++.  Not sure if this change is going to be a problem on other platforms.
  static const NULLTYPE null;

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

// There was a requirement to use using declarations to bring inherited methods into scope on strict/funky compilers like Solaris CC where inheritance
// trees got particularly bushy and interesting.
//
// On VC6 the using declarations didn't seem to matter, but on Borland C++ Builder it was failing with errors.  On Borland C++ Builder and VC6, the using
// declaration macro will default to a noop
#if defined(__BORLANDC__)
  // gotta eat the spurious semicolon after fit_using_decl(x) on BCC or else we get goofy errors, so just put out an anoymous struct with nothing in it to take the semicolon, cross your fingers and toes on this one ...
# define fit_using_decl(decl) /##/
#elif defined(_MSC_VER)
  // on VC6, we don't need nor care about the using thing
# define fit_using_decl(decl)
#else
  // on all other platforms, it's ok to emit the using declaration expression - I think the compilers that required this (Solaris CC) were just being cute, totally not necessary
# define fit_using_decl(decl) using decl
#endif

#endif // __cplusplus

#endif // __TOOLS_MANDATORY_H__
