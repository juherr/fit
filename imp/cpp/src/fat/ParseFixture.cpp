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

declare_fit_module(ParseFixture);

using namespace CEEFIT;

namespace CEEFAT
{

  begin_namespaced_fit_fixture(CEEFAT, PARSEFIXTURE, COLUMNFIXTURE, fat.ParseFixture)

    public:
      fit_var(STRING, Html);
      fit_var(STRING, TableCell);
      fit_var(STRING, Entity);
      fit_var(STRING, Note);

    private:
	    VALUE<PARSE> GenerateParse() 
      {
		    int inputColumns = 0;
		    STRING html;

		    if(Html.Length() != 0) 
        {
			    inputColumns++;
			    html = Html;
		    }
		    if(TableCell.Length() != 0) 
        {
			    inputColumns++;
			    html = STRING("<table><tr>") + TableCell + "</tr></table>";
		    }
		    if(Entity.Length() != 0) 
        {
			    inputColumns++;
			    html = STRING("<table><tr><td>") + Entity + "</td></tr></table>";
		    }
		    
		    if(inputColumns != 1) 
        {
			    throw new EXCEPTION(STRING("Exactly ONE of the following columns is needed: 'Html', 'TableCell', or 'Entity'"));
		    }

		    html = html.SimplePatternReplaceAll(L"\\\\u00a0", L"\x00a0");
		    return(VALUE<PARSE>(new PARSE(html)));
	    }
		  
	    STRING DumpTables(PTR<PARSE>& table) 
      {
        PTR<PARSE> temp(table);

		    STRING result;
		    STRING separator;
		    while(temp != null) 
        {
			    result += separator;
			    result += DumpRows(temp->Parts);
			    separator = "\n----\n";
			    temp = temp->More;
		    }
		    return result;
	    }
	    
	    STRING DumpRows(PTR<PARSE>& row) 
      {
        PTR<PARSE> temp(row);

		    STRING result;
		    STRING separator;
		    while (temp != null) 
        {
			    result += separator;
			    result += DumpCells(temp->Parts);
			    separator = "\n";
			    temp = temp->More;
		    }
		    return result;
	    }
	    
	    STRING DumpCells(PTR<PARSE>& cell) 
      {
        PTR<PARSE> temp(cell);

		    STRING result;
		    STRING separator;
		    while (temp != null) 
        {
          STRING tempText(temp->Text());

			    result += separator;
			    result += STRING("[") + EscapeAscii(tempText) + "]";
			    separator = " ";
			    temp = temp->More;
		    }
		    return result;
	    }

	    STRING EscapeAscii(const STRING& text) 
      {
        STRING temp(text);

		    temp = temp.SimplePatternReplaceAll(L"\x0a", L"\\\\n");
		    temp = temp.SimplePatternReplaceAll(L"\x0d", L"\\\\r");
		    temp = temp.SimplePatternReplaceAll(L"\xa0", L"\\\\x00a0");

		    return temp;
	    }

	    STRING GenerateOutput(PTR<PARSE>& parse) 
      {
        STRINGWRITER result;
		    
        parse->Print(&result);
		    
        return result.ToString();
	    }
	    
    public:
	    fit_test(Output, STRING)
      {
        PTR<PARSE> aParse(GenerateParse());
		    
        return GenerateOutput(aParse);
	    }

      using COLUMNFIXTURE::Parse;
  	  fit_test(Parse, STRING) 
      {
        PTR<PARSE> aParse(GenerateParse());
		    
		    return DumpTables(aParse);		
	    }

  end_namespaced_fit_fixture(CEEFAT, PARSEFIXTURE)

};
