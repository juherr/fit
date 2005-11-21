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
  class ROWFIXTURE : public COLUMNFIXTURE
  {
    public:
      DYNARRAY< PTR<CELLADAPTER> > Results;
      DYNARRAY< PTR<PARSE> > Missing;
      DYNARRAY< PTR<CELLADAPTER> > Surplus;

      typedef NODEIMPL< DYNARRAY< PTR<PARSE> >, PTR<CELLADAPTER> > PARSE_KEYNODE;
      typedef NODEIMPL< DYNARRAY< PTR<CELLADAPTER> >, PTR<CELLADAPTER> > CELL_KEYNODE;

    public:
      virtual void ceefit_call_spec DoRows(PTR<PARSE>& rows);

      virtual void ceefit_call_spec Query(DYNARRAY< PTR<CELLADAPTER> >& out)=0;     // get rows to be compared
      virtual VALUE<FIXTURE> ceefit_call_spec GetTargetClass(void)=0;               // get expected type of row

    protected:
      virtual void ceefit_call_spec Match(DYNARRAY< PTR<PARSE> >& expected, DYNARRAY< PTR<CELLADAPTER> >& computed, int col);
      virtual void ceefit_call_spec List(DYNARRAY< PTR<PARSE> >& out, PTR<PARSE>& rows);
      virtual void ceefit_call_spec List(DYNARRAY< PTR<CELLADAPTER> >& out, DYNARRAY< PTR<CELLADAPTER> >& rows);

      virtual void ceefit_call_spec eSort(HASHMAP< DYNARRAY< PTR<PARSE> > >& out, DYNARRAY< PTR<PARSE> >& list, int col);
      virtual void ceefit_call_spec cSort(HASHMAP< DYNARRAY< PTR<CELLADAPTER> > >& out, DYNARRAY< PTR<CELLADAPTER> >& list, int col);

      virtual void ceefit_call_spec Bin(HASHMAP< DYNARRAY< PTR<PARSE> > >& map, PTR<CELLADAPTER>& key, PTR<PARSE>& row);
      virtual void ceefit_call_spec Bin(HASHMAP< DYNARRAY< PTR<CELLADAPTER> > >& map, PTR<CELLADAPTER>& key, PTR<CELLADAPTER>& row);
      virtual void ceefit_call_spec Union(DYNARRAY< PTR<HASHMAPNODEBASE> >& out, DYNARRAY< PTR<PARSE_KEYNODE> >& a, DYNARRAY< PTR<CELL_KEYNODE> >& b);

      fit_using_decl(COLUMNFIXTURE::Check);

      virtual void ceefit_call_spec Check (DYNARRAY< PTR<PARSE> >& eList, DYNARRAY< PTR<CELLADAPTER> >& cList);

      virtual void ceefit_call_spec Mark(PTR<PARSE>& rows, const STRING& message);
      virtual void ceefit_call_spec Mark(DYNARRAY< PTR<PARSE> >& rows, const STRING& message);

      virtual void ceefit_call_spec BuildRows(PTR<PARSE>& out, DYNARRAY< PTR<CELLADAPTER> >& rows);
      virtual void ceefit_call_spec BuildCells(PTR<PARSE>& out, PTR<CELLADAPTER>& row);

    public:
      ceefit_init_spec ROWFIXTURE(void);
      virtual ceefit_dtor_spec ~ROWFIXTURE(void);

    private:
      ceefit_init_spec ROWFIXTURE(const ROWFIXTURE&);                   /**< Not implemented, do not call */
      ROWFIXTURE& ceefit_call_spec operator=(const ROWFIXTURE&);        /**< Not implemented, do not call */
  };
};
