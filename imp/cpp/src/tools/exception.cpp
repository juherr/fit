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

  const STRING& ceefit_call_spec EXCEPTION::GetExceptionTypeName()
  {
    static STRING typeName(L"EXCEPTION");

    return(typeName);
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

  const STRING& ceefit_call_spec PARSEEXCEPTION::GetExceptionTypeName()
  {
    static STRING typeName(L"PARSEEXCEPTION");

    return(typeName);
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

  const STRING& ceefit_call_spec BOUNDSEXCEPTION::GetExceptionTypeName()
  {
    static STRING typeName(L"BOUNDSEXCEPTION");

    return(typeName);
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

  const STRING& ceefit_call_spec IOEXCEPTION::GetExceptionTypeName()
  {
    static STRING typeName(L"IOEXCEPTION");

    return(typeName);
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

  const STRING& ceefit_call_spec CLASSNOTFOUNDEXCEPTION::GetExceptionTypeName()
  {
    static STRING typeName(L"CLASSNOTFOUNDEXCEPTION");

    return(typeName);
  }
};
