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

declare_fit_module(FatTextToHtmlFixture);

using namespace CEEFIT;

namespace CEEFAT
{
  begin_namespaced_fit_fixture(CEEFAT, TEXTTOHTMLFIXTURE, COLUMNFIXTURE, fat.TextToHtmlFixture)

    public:
      fit_var(STRING, Text);
	    
    private:
	    STRING UnescapeAscii(const STRING& text) 
      {
        STRING temp(text);
		    temp = temp.SimplePatternReplaceAll("\\n", "\n");
		    temp = temp.SimplePatternReplaceAll("\\r", "\r");
		    return(temp);
	    }
	    
	    STRING GenerateOutput(PTR<PARSE>& parse) 
      {
        STRINGWRITER result;
		    parse->Print(&result);
		    return(result.ToString());
	    }

    public:
	    fit_test(HTML, STRING) 
      {
		    Text = UnescapeAscii(Text);
        return(this->FIXTURE::Escape(Text));
	    }

  end_namespaced_fit_fixture(CEEFAT, TEXTTOHTMLFIXTURE)
};
