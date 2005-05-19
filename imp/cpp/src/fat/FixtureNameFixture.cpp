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

declare_fit_module(FatFixtureNameFixture);

using namespace CEEFIT;

namespace CEEFAT
{
  static const char* RegexSpaceCharacters = "[ \t\n\x0B\f\r]";

  begin_fit_fixture(FAT_FIXTURENAMEFIXTURE, COLUMNFIXTURE, fat.FixtureNameFixture)
  
    public:
	    fit_var(STRING, Table);

    private:
	    VALUE<PARSE> GenerateTableParse(const STRING& table) 
      {
        DYNARRAY<STRING> patternStrArray;
        patternStrArray.Add(STRING("\n"));

        DYNARRAY<STRING> output;
        table.ArrayRegexPatternSplit(output, patternStrArray, false);

        PTR<PARSE> temp(GenerateRowParses(output, 0));
		    PTR<PARSE> nullPtr;
        return(VALUE<PARSE>(new PARSE(STRING("table"), STRING(""), temp, nullPtr)));
	    }

	    VALUE<PARSE> GenerateCellParses(const DYNARRAY<STRING>& cells, int cellIndex) 
      {
		    if (cellIndex >= cells.GetSize()) 
        {
          return(VALUE<PARSE>(NULL));
        }
		    
        PTR<PARSE> cellParses(GenerateCellParses(cells, cellIndex + 1));
        PTR<PARSE> nullParse;
		    return(VALUE<PARSE>(new PARSE("td", cells[cellIndex], nullParse, cellParses)));
	    }
    
	    VALUE<PARSE> GenerateRowParses(const DYNARRAY<STRING>& rows, int rowIndex) 
      {
		    if (rowIndex >= rows.GetSize()) 
        {
          return(VALUE<PARSE>(NULL));
        }

        DYNARRAY<STRING> patternStrArray;
        patternStrArray.Add(STRING("\\]"));
        patternStrArray.Add(STRING(RegexSpaceCharacters) + "*");
        patternStrArray.Add(STRING("\\["));
        
		    DYNARRAY<STRING> cells;
        rows[rowIndex].ArrayRegexPatternSplit(cells, patternStrArray, false);

		    if (cells.GetSize() != 0) 
        {
          cells[0] = cells[0].Substring(1); // strip beginning '['
			    int lastCell = cells.GetSize() - 1;

          DYNARRAY<STRING> patternStrArray2;
          patternStrArray2.Add(STRING("\\]"));
          patternStrArray2.Add(STRING("$"));
          cells[lastCell] = cells[lastCell].ArrayRegexPatternReplaceAll(patternStrArray2, "");  // strip ending ']' 
		    }
		    
        PTR<PARSE> cellParses(GenerateCellParses(cells, 0));
        PTR<PARSE> rowParses(GenerateRowParses(rows, rowIndex+1));
        return(VALUE<PARSE>(new PARSE("tr", STRING(""), cellParses, rowParses)));
	    }		

    public:
      fit_test(FixtureName, STRING)
      {
		    PTR<PARSE> tableParse(GenerateTableParse(Table));		    
        PTR<PARSE> fixtureNameParse(this->COLUMNFIXTURE::FixtureName(tableParse));

        STRING result(fixtureNameParse->Text());
		    if (result.IsEqual("")) 
        {
          return STRING("(missing)");
        }
		    return result;
	    }	
	    
  end_fit_fixture(FAT_FIXTURENAMEFIXTURE)

};
