#ifndef __TOOLS_EQUALITY_H__
#define __TOOLS_EQUALITY_H__

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

  inline bool IsEqual(const bool aValue, const bool bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(const bool& aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  inline bool IsEqual(const unsigned char aValue, const unsigned char bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(const unsigned char& aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  inline bool IsEqual(const signed char aValue, const signed char bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(const signed char& aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  inline bool IsEqual(const char aValue, const char bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(const char& aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  inline bool IsEqual(const short aValue, const short bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(const short& aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  inline bool IsEqual(const unsigned short aValue, const unsigned short bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(const unsigned short& aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

#if defined(__GNUC__) || (1200 < _MSC_VER)  /* dw 05/30/06 - VC > 6.0 apparently also supports wchar_t as a type */
  inline bool IsEqual(const wchar_t aValue, const wchar_t bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(const wchar_t& aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }
#endif

  inline bool IsEqual(const unsigned int aValue, const unsigned int bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(const unsigned int& aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  inline bool IsEqual(const signed int aValue, const signed int bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(const signed int& aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  inline bool IsEqual(const unsigned long aValue, const unsigned long bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(const unsigned long& aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  inline bool IsEqual(const signed long aValue, const signed long bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(const signed long& aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  inline bool IsEqual(const fitINT64 aValue, const fitINT64 bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(const fitINT64& aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  inline bool IsEqual(const UfitINT64 aValue, const UfitINT64 bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(const UfitINT64& aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  inline bool IsEqual(const float aValue, const float bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(const float& aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  inline bool IsEqual(const double aValue, const double bValue)
  {
    return(aValue == bValue);
  }

  template<class ANYTYPE> inline bool IsEqual(const double& aValue, const ANYTYPE& bValue)
  {
    return(aValue == bValue);
  }

  inline bool IsEqual(const char* aValue, const char* bValue)
  {
    if(aValue == null)
    {
      return(bValue == null);
    }
    if(bValue == null)
    {
      return(false);
    }
    return(strcmp(aValue, bValue) == 0);
  }

  inline bool IsEqual(const wchar_t* aValue, const wchar_t* bValue)
  {
    if(aValue == null)
    {
      return(bValue == null);
    }
    if(bValue == null)
    {
      return(false);
    }
    return(wcscmp(aValue, bValue) == 0);
  }

  template<class ANYTYPE1, class ANYTYPE2> inline bool IsEqual(const ANYTYPE1& aValue, const ANYTYPE2& bValue)
  {
    return(aValue.IsEqual(bValue));
  }

  template<class ANYTYPE1, class ANYTYPE2> inline bool IsEqual(const ANYTYPE1* aValue, const ANYTYPE2* bValue)
  {
    return(aValue->IsEqual(*bValue));
  }

};

#endif // __TOOLS_EQUALITY_H__
