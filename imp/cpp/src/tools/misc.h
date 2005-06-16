#ifndef __TOOLS_MISC_H__
#define __TOOLS_MISC_H__

/**
 * <p>Miscallaneous typedefs and function prototypes that do not fit anywhere else</p>
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

namespace CEEFIT
{
  // Forward declare STRING class
  class STRING;

  extern void ceefit_call_spec SafeSprintf(STRING& out, const wchar_t* format, ...);
  extern void ceefit_call_spec SafeFprintf(FILE* out, const wchar_t* format, ...);  /**< @throws IOEXCEPTION* if write fails */

  class VOIDTYPE
  {
    public:
      inline ceefit_init_spec VOIDTYPE(void) {}
      virtual ceefit_init_spec ~VOIDTYPE(void) {}

    private:
      ceefit_init_spec VOIDTYPE(const VOIDTYPE&);
      VOIDTYPE& ceefit_call_spec operator=(const VOIDTYPE&);
  };

# ifdef _MSC_VER
    typedef __int64 fitINT64;
    typedef unsigned __int64 UfitINT64;
# else
    typedef long long int fitINT64;
    typedef unsigned long long int UfitINT64;
# endif
};

#endif // __TOOLS_MISC_H__
