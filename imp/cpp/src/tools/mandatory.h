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
#else
// GCC gets pissy if we try to get cute with the initializer call specs, leave as __thiscall for now
# define ceefit_init_spec
#endif

#define ceefit_call_spec __cdecl

namespace CEEFIT
{
  // Drastic measures ...
  // We have to deal with gcc's annoying "warning: passing NULL used for non-pointer converting" by completely removing any 
  // mention of NULL from the CeeFIT code.  From now on, use only CEEFIT::null when representing the NULL pointer.
  static const int null=0;
};

#endif // __TOOLS_MANDATORY_H__
