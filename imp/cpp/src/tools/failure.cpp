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
  ceefit_init_spec FAILURE::FAILURE()
  {
  }

  ceefit_dtor_spec FAILURE::~FAILURE()
  {
  }

  STRING& ceefit_call_spec FAILURE::GetReason()
  {
    return(Reason);
  }

  ceefit_init_spec FITASSERTIONFAILED::FITASSERTIONFAILED(const char* aFile, int aLineNumber)
  {
    Reason = STRING("Assertion failed");
    if(aLineNumber > 0)
    {
      Reason += STRING(": ") + aFile + "(" + aLineNumber + ")";
    }
  }

  ceefit_dtor_spec FITASSERTIONFAILED::~FITASSERTIONFAILED()
  {
  }

  ceefit_init_spec FITFAILED::FITFAILED(const char* aFile, int aLineNumber)
  {
    Reason = STRING("Failed");
    if(aLineNumber > 0)
    {
      Reason += STRING(": ") + aFile + "(" + aLineNumber + ")";
    }
  }

  ceefit_dtor_spec FITFAILED::~FITFAILED()
  {
  }
};
