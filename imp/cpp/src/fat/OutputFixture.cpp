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

declare_fit_module(OutputFixture);

using namespace CEEFIT;

namespace CEEFAT
{

  begin_fit_fixture(FAT_OUTPUTFIXTURE, COLUMNFIXTURE, fat.OutputFixture);

    public:
      fit_var(STRING, Text);

    private:
	    STRING Unescape(const STRING& text) 
      {
        STRING temp(text); 

		    temp = temp.SimplePatternReplaceAll("\\\\n", "\n");
		    temp = temp.SimplePatternReplaceAll("\\\\r", "\r");

		    return temp;
	    }
	    
	    STRING GenerateOutput(PTR<PARSE>& parse) 
      {
        STRINGWRITER stringWriter;

		    parse->Print(&stringWriter);

		    return stringWriter.ToString();
	    }

    public:
	    fit_test(CellOutput, STRING)
      {
        STRING tag("td");
        STRING body("");
        PTR<PARSE> nullParts(NULL);
        PTR<PARSE> nullMore(NULL);

		    PTR<PARSE> cell(new PARSE(tag, body, nullParts, nullMore));
		    
        cell->Leader = "";
		    cell->Body = FIXTURE::Escape(Unescape(Text));
		    
        return GenerateOutput(cell);
	    }

  end_fit_fixture(FAT_OUTPUTFIXTURE);    

};
