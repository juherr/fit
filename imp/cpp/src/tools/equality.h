#ifndef __TOOLS_EQUALITY_H__
#define __TOOLS_EQUALITY_H__

#include "ceefit/mandatory.h"

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

namespace CEEFIT
{
  //--------------------------------------------------------------------------------------------------------
  // Equality Tests
  //--------------------------------------------------------------------------------------------------------

  template<class ANYTYPE> inline bool IsEqual(bool aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(unsigned char aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(signed char aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(char aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(unsigned short aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(signed short aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

# ifdef __GNUC__
    template<class ANYTYPE> inline bool IsEqual(wchar_t aValue, const ANYTYPE& bValue)
    {
      return(aValue == bValue);
    }
# endif

  template<class ANYTYPE> inline bool IsEqual(unsigned int aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(int aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(const INT64& aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(const UINT64& aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(float aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(const double& aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  inline bool IsEqual(const char* aValue, const char* bValue)
  {
    if(aValue == NULL)
    {
      return(bValue == NULL);
    }
    if(bValue == NULL)
    {
      return(false);
    }
    return(strcmp(aValue, bValue) == 0);
  }

  inline bool IsEqual(const wchar_t* aValue, const wchar_t* bValue)
  {
    if(aValue == NULL)
    {
      return(bValue == NULL);
    }
    if(bValue == NULL)
    {
      return(false);
    }
    return(wcscmp(aValue, bValue) == 0);
  }

  template<class ANYTYPE1, class ANYTYPE2> inline bool IsEqual(ANYTYPE1& aValue, const ANYTYPE2& bValue)
  {
    return(aValue.IsEqual(bValue));
  }

/*
  template<class ANYTYPE1, class ANYTYPE2> inline bool IsEqual(ANYTYPE1* aValue, const ANYTYPE2* bValue)
  {
    return(aValue->IsEqual(*bValue));
  }
*/

};

#endif // __TOOLS_EQUALITY_H__
