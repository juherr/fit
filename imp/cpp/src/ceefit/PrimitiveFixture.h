#ifndef __CEEFIT_PRIMITIVEFIXTURE_H__
#define __CEEFIT_PRIMITIVEFIXTURE_H__

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

  class PRIMITIVEFIXTURE : public FIXTURE
  {
    public:
      // format converters ////////////////////////
      static long ceefit_call_spec ParseLong(PTR<PARSE>& cell);
      static double ceefit_call_spec ParseDouble(PTR<PARSE>& cell);
      static bool ceefit_call_spec ParseBoolean(PTR<PARSE>& cell);

      // answer comparisons ///////////////////////
      virtual void ceefit_call_spec Check(PTR<PARSE>& cell, const STRING& value);
      virtual void ceefit_call_spec Check(PTR<PARSE>& cell, long value);
      virtual void ceefit_call_spec Check(PTR<PARSE>& cell, double value);
      virtual void ceefit_call_spec Check(PTR<PARSE>& cell, bool value);

      ceefit_init_spec PRIMITIVEFIXTURE(void);
      virtual ceefit_init_spec ~PRIMITIVEFIXTURE(void);

    protected:
      ceefit_init_spec PRIMITIVEFIXTURE(const PRIMITIVEFIXTURE&);              /**< not implemented, do not call */
      PRIMITIVEFIXTURE& ceefit_call_spec operator=(const PRIMITIVEFIXTURE&);   /**< not implemented, do not call */
  };

};

#endif // __CEEFIT_PRIMITIVEFIXTURE_H__
