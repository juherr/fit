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

declare_fit_module(FatFixtureNameFixture);

using namespace CEEFIT;

namespace CEEFAT
{

  begin_fit_fixture(FAT_FIXTURENAMEFIXTURE, COLUMNFIXTURE, fat.FixtureNameFixture)
  
    public:
      fit_var(STRING, Table);

    private:  
	    STRING DumpCells(PTR<PARSE>& cell) 
      {
		    PTR<PARSE> temp(cell);
        STRING result;
		    STRING separator;
		    
        while(temp != NULL) 
        {
			    result += separator;
			    result += STRING("[") + temp->Text() + "]";
			    separator = " ";
			    temp = temp->More;
		    }
		    return(result);
	    }

	    STRING DumpRows(PTR<PARSE>& row) 
      {
        PTR<PARSE> temp(row);
		    STRING result;
		    STRING separator;

		    while(temp != NULL) 
        {
			    result += separator;
			    result += DumpCells(temp->Parts);
			    separator = "\n";
			    temp = temp->More;
		    }
		    return(result);
	    }

	    STRING DumpTables(PTR<PARSE>& table) 
      {
        PTR<PARSE> temp(table);
		    STRING result;
		    STRING separator;

		    while(temp != NULL) 
        {
			    result += separator;
			    result += DumpRows(temp->Parts);
			    separator = "\n----\n";
			    temp = temp->More;
		    }
		    return(result);
	    }
	  
	    VALUE<PARSE> GenerateTableParse(const STRING& table) 
      {
        DYNARRAY<STRING> rows;
		    table.Split(rows, "\n");
		    
        STRING nullString;
        PTR<PARSE> rowParses(GenerateRowParses(rows, 0));
        PTR<PARSE> nullPtr;
        return(VALUE<PARSE>(new PARSE(STRING("table"), nullString, rowParses, nullPtr)));
	    }

	    VALUE<PARSE> GenerateRowParses(DYNARRAY<STRING>& rows, int rowIndex) 
      {
		    if (rowIndex >= rows.GetSize()) 
        {
          return VALUE<PARSE>(NULL);
        }
		    
        DYNARRAY<STRING> temp;
        DYNARRAY<STRING> cells;
        rows[rowIndex].CapturingGroups(temp, "\\[(.*?)\\]");
            
        if(temp.GetSize() == 1)
        {
          cells.Reserve(temp.GetSize());
        }
        else
        {
          cells.Reserve(temp.GetSize()-1);
        }

		    for (int i = 0; i < temp.GetSize(); i++) 
        {
          if((i + 1) < temp.GetSize())
          {
			      cells[i] = temp[i + 1];     // don't copy the 0 group
          }
		    }
		    
        STRING nullString;
        PTR<PARSE> cellParses(GenerateCellParses(cells, 0));
        PTR<PARSE> rowParses(GenerateRowParses(rows, rowIndex+1));
		    
        return(VALUE<PARSE>(new PARSE(STRING("tr"), nullString, cellParses, rowParses)));
	    }		

	    VALUE<PARSE> GenerateCellParses(DYNARRAY<STRING>& cells, int cellIndex) 
      {
		    if (cellIndex >= cells.GetSize()) 
        {
          return(VALUE<PARSE>(NULL));
        }

        PTR<PARSE> nullParse;		  
        PTR<PARSE> cellParses(GenerateCellParses(cells, cellIndex + 1));		  
		    return(VALUE<PARSE>(new PARSE(STRING("td"), cells[cellIndex], nullParse, cellParses)));
	    }
    
    public:
      fit_test(FixtureName, STRING)
      {
		    PTR<PARSE> tableParse(GenerateTableParse(Table));

        STRING retVal = DumpTables(tableParse);
		    
        return retVal;
      }

	    fit_test(ValidFixture, STRING) 
      {
		    return("not implemented");
	    }
	    
	    fit_test(Error, STRING) 
      {
		    return("not implemented");
	    }
	    
  end_fit_fixture(FAT_FIXTURENAMEFIXTURE);

};
