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
  ceefit_init_spec SCIENTIFICDOUBLE::SCIENTIFICDOUBLE(double value) 
  {
    Value = value;
    Precision = 0.0;
  }

  ceefit_init_spec SCIENTIFICDOUBLE::SCIENTIFICDOUBLE()
  {
    Value = 0.0;
    Precision = 0.0;
  }

  ceefit_init_spec SCIENTIFICDOUBLE::SCIENTIFICDOUBLE(const SCIENTIFICDOUBLE& obj)
  {
    Value = obj.Value;
    Precision = obj.Precision;
  }

  ceefit_init_spec SCIENTIFICDOUBLE::~SCIENTIFICDOUBLE()
  {
  }

  SCIENTIFICDOUBLE& ceefit_call_spec SCIENTIFICDOUBLE::operator=(const SCIENTIFICDOUBLE& obj)
  {
    Value = obj.Value;
    Precision = obj.Precision;

    return(*this);
  }

  SCIENTIFICDOUBLE& ceefit_call_spec SCIENTIFICDOUBLE::operator=(const STRING& obj)
  {
    (*this) = SCIENTIFICDOUBLE::ValueOf(obj);

    return(*this);
  }

  SCIENTIFICDOUBLE& ceefit_call_spec SCIENTIFICDOUBLE::operator=(const double& obj)
  {
    Value = obj;
    Precision = 0.0;

    return(*this);
  }

  double ceefit_call_spec SCIENTIFICDOUBLE::ParseDouble(const STRING& s) 
  {
    double aDouble = 0.0f;
    errno = 0;
    wchar_t* endChar;
    aDouble = wcstod(s.GetBuffer(), &endChar);
    if(errno == ERANGE)
    {
      throw new PARSEEXCEPTION(STRING("Could not parse string to a double:  ") + s);
    }

    return((double) aDouble);
  }

  bool ceefit_call_spec SCIENTIFICDOUBLE::IsNaN(double aDouble) 
  {
    return((aDouble != aDouble) == true);
  }

  SCIENTIFICDOUBLE ceefit_call_spec SCIENTIFICDOUBLE::ValueOf(const STRING& s) 
  {
    SCIENTIFICDOUBLE result(ParseDouble(s));
    result.Precision = CalcPrecision(s);

    return result;
  }

  double ceefit_call_spec SCIENTIFICDOUBLE::CalcPrecision(const STRING& s) 
  {
    double value = ParseDouble(s);
    double bound = ParseDouble(Tweak(s.Trim()));

    return fabs(bound - value);
  }

  STRING ceefit_call_spec SCIENTIFICDOUBLE::Tweak(const STRING& s) 
  {
    int pos;
    if((pos = s.ToLowercase().IndexOf("e")) >= 0) 
    {
      return Tweak(s.Substring(0, pos)) + s.Substring(pos);
    }
    if(s.IndexOf(".") >= 0) 
    {
      return s + "5";
    }
    return s + ".5";
  }

  bool ceefit_call_spec SCIENTIFICDOUBLE::IsEqual(const SCIENTIFICDOUBLE& obj) const 
  {
    return CompareTo(obj) == 0;
  }

  int ceefit_call_spec SCIENTIFICDOUBLE::CompareTo(const SCIENTIFICDOUBLE& obj) const 
  {
    double other = obj.Value;
    double diff = Value - other;

    if (diff < -Precision) return -1;
    if (diff > Precision) return 1;
    if (IsNaN(Value) && IsNaN(other)) return 0;
    if (IsNaN(Value)) return 1;
    if (IsNaN(other)) return -1;

    return 0;
  }

  STRING ceefit_call_spec SCIENTIFICDOUBLE::ToString() const 
  {
    STRING out;

    SafeSprintf(out, L"%g", Value);

    return out;
  }

  double ceefit_call_spec SCIENTIFICDOUBLE::DoubleValue() const 
  {
    return Value;
  }

  float ceefit_call_spec SCIENTIFICDOUBLE::FloatValue() const 
  {
    return((float) Value);
  }

  long ceefit_call_spec SCIENTIFICDOUBLE::LongValue() const 
  {
    return((long) Value);
  }

  int ceefit_call_spec SCIENTIFICDOUBLE::IntValue() const 
  {
    return((int) Value);
  }

  int ceefit_call_spec SCIENTIFICDOUBLE::GetHashCode(void) const
  {
    return((int) FloatValue());
  }
};
