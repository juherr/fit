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

#include "ceefit.h"
#include "fat/Money.h"

extern "C"
{
  // defined in ceefit.lib
  typedef unsigned int unicode_char_t;
  extern int unicode_isdigit(unicode_char_t c);
}

using namespace CEEFIT;

namespace CEEFAT
{
  MONEY ceefit_call_spec MONEY::ValueOf(const STRING& s)
  {
    MONEY aMoney(s);

    return(aMoney);
  }

  ceefit_init_spec MONEY::MONEY() 
  {
    cents = 0;
  }

  ceefit_init_spec MONEY::MONEY(const STRING& s) 
  {
    STRING stripped("");
    for (int i=0; i < s.Length(); i++) 
    {
      wchar_t c = s.CharAt(i);
      if(unicode_isdigit(c) || c == '.') 
      {
        stripped += c;
      }
    }
  
    errno = 0;
    wchar_t* endChar;
    float temp = (float) wcstod(stripped.GetBuffer(), &endChar);

    if(errno == ERANGE)
    {
      throw new PARSEEXCEPTION("Failed to parse double");
    }

    cents = (long) (100 * temp);
  }

  ceefit_dtor_spec MONEY::~MONEY(void) 
  {
  }

  MONEY& ceefit_call_spec MONEY::operator=(const MONEY& obj)
  {
    cents = obj.cents;
    return(*this);
  }

  ceefit_init_spec MONEY::MONEY(const MONEY& obj)
  {
    cents = obj.cents;
  }

  bool ceefit_call_spec MONEY::IsEqual(const MONEY& value) const
  {
    return cents == value.cents;
  }

  int ceefit_call_spec MONEY::GetHashCode() 
  {
    return (int)cents;
  }

  STRING ceefit_call_spec MONEY::ToString() const
  {
    STRING out;
    SafeSprintf(out, L"%li", cents);
    
    return out + " cents";
  }
};
