#ifndef __TOOLS_OBJECT_H__
#define __TOOLS_OBJECT_H__

#include "ceefit/mandatory.h"

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
  /**
   * <p>The base class for all CeeFIT objects</p>
   *
   * <p>OBJECT lets us automatically install a operator new/delete for all derived classes.</p>
   */
  class OBJECT
  {
    public:
      static void* ceefit_call_spec operator new(size_t numBytes);
      static void ceefit_call_spec operator delete(void* aObj);

      OBJECT(void);
      virtual ceefit_init_spec ~OBJECT(void);
      ceefit_init_spec OBJECT(const OBJECT&);
      OBJECT& ceefit_call_spec operator=(const OBJECT&);
  };
};

#endif // __TOOLS_OBJECT_H__
