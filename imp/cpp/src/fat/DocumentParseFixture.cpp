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

declare_fit_module(FatDocumentParseFixture);

using namespace CEEFIT;

namespace CEEFAT
{
  begin_namespaced_fit_fixture(CEEFAT, DOCUMENTPARSEFIXTURE, COLUMNFIXTURE, fat.DocumentParseFixture);
    
    public:
      fit_var(STRING, HTML);
      fit_var(STRING, Note);    // non-functional

    private:
	    inline STRING ceefit_call_spec GenerateOutput(PTR<PARSE>& parse) 
      {
        STRINGWRITER result;
        parse->Print(&result);
		    return result.ToString();
	    }
		    
	    inline STRING ceefit_call_spec DumpTables(PTR<PARSE>& table) 
      {
        PTR<PARSE> temp(table);
		    STRING result("");
		    STRING separator("");

		    while(temp != null) 
        {
			    result += separator;
			    result += DumpRows(temp->Parts);
			    separator = "\n----\n";
			    temp = temp->More;
		    }
		    return result;
	    }
	    
	    inline STRING ceefit_call_spec DumpRows(PTR<PARSE>& row) 
      {
		    PTR<PARSE> temp(row);
        STRING result("");
		    STRING separator("");

		    while(temp != null) 
        {
			    result += separator;
			    result += DumpCells(temp->Parts);
			    separator = "\n";
			    temp = temp->More;
		    }
		    return result;
	    }
	    
	    inline STRING ceefit_call_spec DumpCells(PTR<PARSE>& cell) 
      {
		    PTR<PARSE> temp(cell);
        STRING result("");
		    STRING separator("");

		    while(temp != null) 
        {
			    result += separator;
			    result += STRING("[") + temp->Body + "]";
			    separator = " ";
			    temp = temp->More;
		    }
		    return result;
	    }

      fit_test(Output, STRING) 
      {
        PTR<PARSE> temp(new PARSE(HTML));
		    
        return GenerateOutput(temp);
	    }

	    fit_test(Structure, STRING)
      {
        PTR<PARSE> temp(new PARSE(HTML));
		    
        return DumpTables(temp);		
	    }
	  
  end_namespaced_fit_fixture(CEEFAT, DOCUMENTPARSEFIXTURE);

};
