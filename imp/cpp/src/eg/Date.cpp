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
#include "icu/include/unicode/smpdtfmt.h"

using namespace CEEFIT;

namespace EG
{
  ceefit_init_spec DATE::DATE() 
  {
    Value = (fitINT64) 0;
  }

  ceefit_dtor_spec DATE::~DATE()
  {
  }

  DATE& ceefit_call_spec DATE::operator=(const DATE& aDate)
  {
    Value = aDate.Value;

    return(*this);
  }

  ceefit_init_spec DATE::DATE(const DATE& aDate)
  {
    Value = aDate.Value;
  }

  fitINT64 DATE::GetTime() const
  {
    return(Value);
  }

  STRING DATE::ToString() const 
  {
    UErrorCode errorCode = U_ZERO_ERROR;

    UnicodeString formatSpec(L"d/M/yy K:mm a");
    SimpleDateFormat dateFormat(formatSpec, errorCode);
    if(U_FAILURE(errorCode))
    {
      STRING reason;
      SafeSprintf(reason, L"Failed to get a SimpleDateFormat for format() in DATE::ToString, reason (%i):  %S", errorCode, u_errorName(errorCode));
      throw new PARSEEXCEPTION(reason);
    }

    UnicodeString unicodeString;

    const UChar* uChar = dateFormat.format((UDate) Value, unicodeString).getBuffer();
    STRING retVal;
    int i = -1;
    while(++i < unicodeString.length())
    {
      retVal += uChar[i];
    }

    return(retVal);        
  }

  void DATE::Parse(const STRING& aString) 
  {
    UErrorCode errorCode = U_ZERO_ERROR;

    UnicodeString formatSpec(L"d/M/yy K:mm a");
    SimpleDateFormat dateFormat(formatSpec, errorCode);
    if(U_FAILURE(errorCode))
    {
      STRING reason;
      SafeSprintf(reason, L"Failed to get a SimpleDateFormat for parse() in Date::Parse, reason (%i):  %S", errorCode, u_errorName(errorCode));
      throw new PARSEEXCEPTION(reason);
    }

    UnicodeString unicodeString(aString.GetBuffer());
    UDate uDate = dateFormat.parse(unicodeString, errorCode);
    if(U_FAILURE(errorCode))
    {
      STRING reason;
      SafeSprintf(reason, L"Parse exception in MUSIC::Parse, reason (%i):  %S", errorCode, u_errorName(errorCode));
      throw new PARSEEXCEPTION("Parse exception in MUSIC::Parse");
    }

    //delete dateFormat;
    Value = (fitINT64) uDate;
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