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
  // don't call this
  ceefit_init_spec REFCOUNTED::REFCOUNTED(const REFCOUNTED&)
  {
    assert(false);
  }

  // don't call this
  REFCOUNTED& ceefit_call_spec REFCOUNTED::operator=(const REFCOUNTED&)
  {
    assert(false);
    return(*this);
  }

  ceefit_init_spec REFCOUNTED::REFCOUNTED()
  {
	  RefCount = 0;
  }

  ceefit_dtor_spec REFCOUNTED::~REFCOUNTED()
  {
  }

  int ceefit_call_spec REFCOUNTED::GetRefCount()
  {
	  return RefCount;
  }

  void ceefit_call_spec REFCOUNTED::AddRef()
  {
	  RefCount++;
  }

  bool ceefit_call_spec REFCOUNTED::ReleaseRef(bool DoDestroy)
  {
	  bool RetVal = false;

	  RefCount--;
	  if(RefCount == 0) {
		  if(DoDestroy)
		  {
        delete this;
		  }
		  RetVal = true;
	  }

	  return(RetVal);
  }
};
