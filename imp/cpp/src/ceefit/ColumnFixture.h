#ifndef __CEEFIT_COLUMNFIXTURE_H__
#define __CEEFIT_COLUMNFIXTURE_H__

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

  class COLUMNFIXTURE : public FIXTURE
  {
    protected:
      bool HasExecuted;
      DYNARRAY<CELLADAPTER*> ColumnBindings;

    public:
      ceefit_init_spec COLUMNFIXTURE(void);
      virtual ceefit_init_spec ~COLUMNFIXTURE(void);

    public:
      // Traversal ////////////////////////////////

      virtual void ceefit_call_spec DoRows(PTR<PARSE>& rows);
      virtual void ceefit_call_spec DoRow(PTR<PARSE>& row);

      virtual void ceefit_call_spec DoCell(PTR<PARSE>& cell, int column);
      virtual void ceefit_call_spec Check(PTR<PARSE>& cell, CELLADAPTER* a);

      virtual void ceefit_call_spec Reset(void);
      virtual void ceefit_call_spec Execute(void);

    protected:
      // Utility //////////////////////////////////
      virtual void ceefit_call_spec Bind(PTR<PARSE>& heads);

      virtual CELLADAPTER* ceefit_call_spec BindMethod(const STRING& name);
      virtual CELLADAPTER* ceefit_call_spec BindField(const STRING& name);

    protected:
      COLUMNFIXTURE& ceefit_call_spec operator=(const COLUMNFIXTURE&);
      ceefit_init_spec COLUMNFIXTURE(const COLUMNFIXTURE&);
  };

};

#endif // __CEEFIT_COLUMNFIXTURE_H__
