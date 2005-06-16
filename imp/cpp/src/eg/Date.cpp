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
#include "eg/eg.h"

// this requires the ICU binaries to be downloaded and installed in the lib folder

#include "icu/include/unicode/utypes.h"
#include "icu/include/unicode/datefmt.h"

using namespace CEEFIT;

namespace EG
{
  DATE::DATE() 
  {
    Value = (fitINT64) 0;
  }

  DATE::~DATE()
  {
  }

  DATE& DATE::operator=(const DATE& aDate)
  {
    Value = aDate.Value;

    return(*this);
  }

  DATE::DATE(const DATE& aDate)
  {
    Value = aDate.Value;
  }

  fitINT64 DATE::GetTime() const
  {
    return(Value);
  }

  STRING DATE::ToString() const 
  {
    DateFormat* dateFormat = DateFormat::createDateTimeInstance(DateFormat::SHORT, DateFormat::SHORT);
    UnicodeString unicodeString;

    const UChar* uChar = dateFormat->format((UDate) Value, unicodeString).getBuffer();
    STRING retVal;
    int i = -1;
    while(++i < unicodeString.length())
    {
      retVal += uChar[i];
    }

    delete dateFormat;

    return(retVal);        
  }

  void DATE::Parse(const STRING& aString) 
  {
    DateFormat* dateFormat = DateFormat::createDateTimeInstance(DateFormat::SHORT, DateFormat::SHORT);
    UnicodeString unicodeString(aString.GetBuffer());
    UErrorCode errorCode;
    UDate uDate = dateFormat->parse(unicodeString, errorCode);
    if(U_FAILURE(errorCode))
    {
      throw new PARSEEXCEPTION("Parse exceptionm in MUSIC::Parse");
    }

    delete dateFormat;
    Value = (fitINT64) (long) uDate;
  }

  bool DATE::IsEqual(const DATE& aDate) const
  {
    return(Value == aDate.Value);
  }

  int DATE::GetHashCode(void) const
  {
    return((int) Value);
  }
};