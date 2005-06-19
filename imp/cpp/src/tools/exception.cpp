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

#ifdef _WIN32
# include <windows.h>  
#endif

namespace CEEFIT
{
  ceefit_init_spec EXCEPTION::EXCEPTION()
  {
  }

  ceefit_init_spec EXCEPTION::~EXCEPTION()
  {
  }

  STRING& ceefit_call_spec EXCEPTION::GetReason()
  {
    return(Reason);
  }

  void ceefit_call_spec EXCEPTION::SetReason(const wchar_t* aReason)
  {
    Reason = aReason;
  }

  void ceefit_call_spec EXCEPTION::SetReason(const STRING& aReason)
  {
    Reason = aReason;
  }

  const char* ceefit_call_spec EXCEPTION::GetExceptionTypeName()
  {
    return("EXCEPTION");
  }

  ceefit_init_spec EXCEPTION::EXCEPTION(const STRING& aReason)
  {
    SetReason(aReason);
  }

  ceefit_init_spec EXCEPTION::EXCEPTION(const wchar_t* aReason)
  {
    SetReason(aReason);
  }

  ceefit_init_spec PARSEEXCEPTION::PARSEEXCEPTION()
  {
    Offset = 0;
  }

  ceefit_init_spec PARSEEXCEPTION::PARSEEXCEPTION(const STRING& aString) : EXCEPTION(aString)
  {
    Offset = 0;
  }

  ceefit_init_spec PARSEEXCEPTION::PARSEEXCEPTION(const wchar_t* aString) : EXCEPTION(aString)
  {
    Offset = 0;
  }

  ceefit_init_spec PARSEEXCEPTION::~PARSEEXCEPTION()
  {
  }

  void ceefit_call_spec PARSEEXCEPTION::SetOffset(int aOffset)
  {
    Offset = aOffset;
  }

  int ceefit_call_spec PARSEEXCEPTION::GetOffset() const
  {
    return(Offset);
  }

  ceefit_init_spec PARSEEXCEPTION::PARSEEXCEPTION(const STRING& aString, int aOffset) : EXCEPTION(aString)
  {
    SetOffset(aOffset);
  }

  ceefit_init_spec PARSEEXCEPTION::PARSEEXCEPTION(const wchar_t* aString, int aOffset) : EXCEPTION(aString)
  {
    SetOffset(aOffset);
  }

  const char* ceefit_call_spec PARSEEXCEPTION::GetExceptionTypeName()
  {
    return("PARSEEXCEPTION");
  }

  ceefit_init_spec BOUNDSEXCEPTION::BOUNDSEXCEPTION()
  {
    Index = 0;
  }

  ceefit_init_spec BOUNDSEXCEPTION::BOUNDSEXCEPTION(const STRING& aString) : EXCEPTION(aString)
  {
    Index = 0;
  }

  ceefit_init_spec BOUNDSEXCEPTION::BOUNDSEXCEPTION(const wchar_t* aString) : EXCEPTION(aString)
  {
    Index = 0;
  }

  ceefit_init_spec BOUNDSEXCEPTION::~BOUNDSEXCEPTION()
  {
  }

  void ceefit_call_spec BOUNDSEXCEPTION::SetIndex(int aIndex)
  {
    Index = aIndex;
  }

  int ceefit_call_spec BOUNDSEXCEPTION::GetIndex() const
  {
    return(Index);
  }

  ceefit_init_spec BOUNDSEXCEPTION::BOUNDSEXCEPTION(const STRING& aString, int aIndex) : EXCEPTION(aString)
  {
    SetIndex(aIndex);
  }

  ceefit_init_spec BOUNDSEXCEPTION::BOUNDSEXCEPTION(const wchar_t* aString, int aIndex) : EXCEPTION(aString)
  {
    SetIndex(aIndex);
  }

  const char* ceefit_call_spec BOUNDSEXCEPTION::GetExceptionTypeName()
  {
    return("BOUNDSEXCEPTION");
  }

  ceefit_init_spec IOEXCEPTION::IOEXCEPTION()
  {
  }

  ceefit_init_spec IOEXCEPTION::IOEXCEPTION(const STRING& aString) : EXCEPTION(aString)
  {
  }

  ceefit_init_spec IOEXCEPTION::IOEXCEPTION(const wchar_t* aString) : EXCEPTION(aString)
  {
  }

  ceefit_init_spec IOEXCEPTION::~IOEXCEPTION()
  {
  }

  const char* ceefit_call_spec IOEXCEPTION::GetExceptionTypeName()
  {
    return("IOEXCEPTION");
  }

  ceefit_init_spec CLASSNOTFOUNDEXCEPTION::CLASSNOTFOUNDEXCEPTION()
  {
  }

  ceefit_init_spec CLASSNOTFOUNDEXCEPTION::CLASSNOTFOUNDEXCEPTION(const STRING& aString) : EXCEPTION(aString)
  {
  }

  ceefit_init_spec CLASSNOTFOUNDEXCEPTION::CLASSNOTFOUNDEXCEPTION(const wchar_t* aString) : EXCEPTION(aString)
  {
  }

  ceefit_init_spec CLASSNOTFOUNDEXCEPTION::~CLASSNOTFOUNDEXCEPTION()
  {
  }

  const char* ceefit_call_spec CLASSNOTFOUNDEXCEPTION::GetExceptionTypeName()
  {
    return("CLASSNOTFOUNDEXCEPTION");
  }

  ceefit_init_spec DIVIDEBYZEROEXCEPTION::DIVIDEBYZEROEXCEPTION()
  {
  }

  ceefit_init_spec DIVIDEBYZEROEXCEPTION::DIVIDEBYZEROEXCEPTION(const STRING& aString) : EXCEPTION(aString)
  {
  }

  ceefit_init_spec DIVIDEBYZEROEXCEPTION::DIVIDEBYZEROEXCEPTION(const wchar_t* aString) : EXCEPTION(aString)
  {
  }

  ceefit_init_spec DIVIDEBYZEROEXCEPTION::~DIVIDEBYZEROEXCEPTION()
  {
  }

  const char* ceefit_call_spec DIVIDEBYZEROEXCEPTION::GetExceptionTypeName()
  {
    return("DIVIDEBYZEROEXCEPTION");
  }

  void ceefit_call_spec ThrowAssertionNotNullFailed()
  {
    throw new EXCEPTION("AssertNotNull failed");
  }

  void ceefit_init_spec AssertIsTrueImpl(bool aExpr) 
  {
    if(aExpr != true)
    {
// uncomment if you want to have debug break in when an assertion fails
//#     ifdef _DEBUG
//#       ifdef WIN32
//          DebugBreak();
//#       endif        
//#     endif
      throw new EXCEPTION("AssertIsTrue failed");
    }
  }

};
