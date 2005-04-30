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
  ceefit_init_spec PRIMITIVEFIXTURE::PRIMITIVEFIXTURE(void)
  {
  }

  ceefit_init_spec PRIMITIVEFIXTURE::~PRIMITIVEFIXTURE(void)
  {
  }

  // format converters ////////////////////////

  long ceefit_call_spec PRIMITIVEFIXTURE::ParseLong(PTR<PARSE>& cell) 
  {
    long retVal;
    int returnCount;

    STRING cellText(cell->Text());
    returnCount = swscanf(cellText.GetBuffer(), L"%li", &retVal);
    if(returnCount == 0 || returnCount == EOF)
    {
      throw new PARSEEXCEPTION("PRIMITIVEFIXTURE::ParseLong failed");
    }
  
    return(retVal);
  }

  double ceefit_call_spec PRIMITIVEFIXTURE::ParseDouble(PTR<PARSE>& cell) 
  {
    double retVal;
    int returnCount;

    STRING cellText(cell->Text());
    returnCount = swscanf(cellText.GetBuffer(), L"%g", &retVal);
    if(returnCount == 0 || returnCount == EOF)
    {
      throw new PARSEEXCEPTION("PRIMITIVEFIXTURE::ParseDouble failed");
    }
  
    return(retVal);
  }

  bool ceefit_call_spec PRIMITIVEFIXTURE::ParseBoolean(PTR<PARSE>& cell) 
  {
    STRING cellText(cell->Text());

    if(cellText.IsEqual("true") || cellText.IsEqual("yes") || cellText.IsEqual("1"))
    {
      return(true);
    }
    else if(cellText.IsEqual("false") || cellText.IsEqual("no") || cellText.IsEqual("0"))
    {
      return(false);
    }
    else
    {
      throw new PARSEEXCEPTION("PRIMITIVEFIXTURE::ParseBoolean failed");
    }
  }

  // answer comparisons ///////////////////////

  void ceefit_call_spec PRIMITIVEFIXTURE::Check(PTR<PARSE>& cell, const STRING& value) 
  {
    if(cell->Text().IsEqual(value)) 
    {
			Right(cell);
		} 
    else 
    {
      Wrong(cell, value);
    }
  }

  void ceefit_call_spec PRIMITIVEFIXTURE::Check(PTR<PARSE>& cell, long value) 
  {
    if(ParseLong(cell) == value) 
    {
			Right(cell);
		} 
    else 
    {
      STRING tempLong;

      SafeSprintf(tempLong, L"%li", value);
      Wrong(cell, tempLong);
    }
  }

  void ceefit_call_spec PRIMITIVEFIXTURE::Check(PTR<PARSE>& cell, double value) 
  {
    if(ParseDouble(cell) == value) 
    {
			Right(cell);
		} 
    else 
    {
      STRING tempDouble;

      SafeSprintf(tempDouble, L"%g", value);
      Wrong(cell, tempDouble);
    }
  }

  void ceefit_call_spec PRIMITIVEFIXTURE::Check(PTR<PARSE>& cell, bool value) 
  {
    if(ParseBoolean(cell) == value) 
    {
			Right(cell);
		} 
    else 
    {
      Wrong(cell, value ? "true" : "false");
    }
  }

};
