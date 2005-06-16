#ifndef __EG_PAGE_H__
#define __EG_PAGE_H__

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

namespace EG
{
  class PAGE : public CEEFIT::ROWFIXTURE
  {
    public:
      static CEEFIT::STRING Url;
      static CEEFIT::STRING Text;

      // actions //////////////////////////////////

      virtual void ceefit_call_spec Location(const CEEFIT::STRING& url);

      virtual CEEFIT::STRING ceefit_call_spec Title(void); 

      virtual void ceefit_call_spec Link(const CEEFIT::STRING& label);

      // rows /////////////////////////////////////

      virtual void ceefit_call_spec Query(CEEFIT::DYNARRAY< CEEFIT::PTR<CEEFIT::CELLADAPTER> >& out); 

      /*
      class Row 
      {
        Parse cells;

        Row(Parse row) 
        {
          this.cells = row.parts;
        }

        STRING Numerator() 
        {
          return Cells->At(0)->Text();
        }

        STRING Denominator() 
        {
          return Cells->At(1)->Text();
        }

        public String quotient() {
          return cells.at(2).text();
        }
      }
      */

      // utilities ////////////////////////////////
  
    private:
      CEEFIT::STRING ceefit_call_spec Get(const CEEFIT::STRING& url);
  };
};

#endif // __EG_PAGE_H__
