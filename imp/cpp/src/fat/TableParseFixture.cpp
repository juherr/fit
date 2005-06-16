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

declare_fit_module(TableParseFixture);

using namespace CEEFIT;

namespace CEEFAT
{
  begin_namespaced_fit_fixture(CEEFAT, TABLEPARSEFIXTURE, COLUMNFIXTURE, fat.TableParseFixture)

    public:
	    fit_var(STRING, HTML);
      fit_var(int, Row);
      fit_var(int, Column);

    private:
	    void GetTable(PTR<PARSE>& out)
      {
		    out = new PARSE(HTML);
	    }
	    
	    void GetRow(PTR<PARSE>& out)
      {
        PTR<PARSE> temp;
        GetTable(temp);

		    out = temp->At(0, Row - 1);
	    }
	    
	    void GetCell(PTR<PARSE>& out) 
      {
        PTR<PARSE> temp;
        GetRow(temp);

		    out = temp->At(0, Column - 1);
	    }
      
    public:
	    fit_test(CellBody, STRING)
      {
        PTR<PARSE> temp;
        
        GetCell(temp);
		    return(temp->Body);
	    }
	    
	    fit_test(CellTag, STRING) 
      {
        PTR<PARSE> temp;
        
        GetCell(temp);
		    return(temp->Tag);
	    }
	    
	    fit_test(RowTag, STRING)
      {
        PTR<PARSE> temp;
        
        GetRow(temp);
		    return(temp->Tag);
      }
	    
	    fit_test(TableTag, STRING)
      {
        PTR<PARSE> temp;

        GetTable(temp);
		    return(temp->Tag);
	    }

  end_namespaced_fit_fixture(CEEFAT, TABLEPARSEFIXTURE)
};
