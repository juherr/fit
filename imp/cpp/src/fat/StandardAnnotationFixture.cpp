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

declare_fit_module(FatStandardAnnotationFixture);

using namespace CEEFIT;

namespace CEEFAT
{
  class FAT_STANDARDANNOTATIONFIXTURE : public COLUMNFIXTURE
  {
    public:
      STRING OriginalHTML;
  	  STRING Annotation;
	    STRING Text;

    private:
      // code smell note: copied from ParseFixture	
	    virtual inline STRING ceefit_call_spec GenerateOutput(PTR<PARSE>& parse) 
      {
		    STRINGWRITER result;
		    parse->Print(&result);
		    return(result.ToString().Trim());
	    }

    public:
	    virtual inline STRING ceefit_call_spec Output() 
      {
		    DYNARRAY<STRING> tags;
        tags.Add("td");
        PTR<PARSE> parse(new PARSE(OriginalHTML, tags));
		    
        PTR<FIXTURE> testbed(new FIXTURE());		    
		    if (Annotation.IsEqual("right")) 
        { 
          testbed->Right(parse);
        }
		    if (Annotation.IsEqual("wrong")) 
        {
          testbed->Wrong(parse, Text);
        }
		    if (Annotation.IsEqual("error")) 
        {
          testbed->Error(parse, Text);
        }
		    if (Annotation.IsEqual("info"))
        {
          testbed->Info(parse, Text); 
        }
		    if (Annotation.IsEqual("ignore"))
        {
          testbed->Ignore(parse);
        }
				    
        return(GenerateOutput(parse));
	    }
	
	    virtual inline void ceefit_call_spec DoCell(PTR<PARSE>& cell, int column) 
      {
		    try 
        {
			    if (column == 4) 
          {
				    cell->Body = RenderedOutput();
			    }
			    else 
          {
            COLUMNFIXTURE::DoCell(cell, column);
			    }
		    }
		    catch(EXCEPTION* e) 
        {
			    Exception(cell, e);
		    }	
	    }
	    
	    virtual inline STRING ceefit_call_spec RenderedOutput() 
      {
		    return STRING("<table border='1'><tr>") + Output() + "</tr></table>";		
	    }
	
      inline ceefit_init_spec FAT_STANDARDANNOTATIONFIXTURE(void)
      {   
        OriginalHTML = "Text";

        RegisterCeefitField(this, "OriginalHTML", OriginalHTML);
        RegisterCeefitField(this, "Annotation", Annotation);
        RegisterCeefitField(this, "Text", Text);

        RegisterCeefitTest(this, "Output", &FAT_STANDARDANNOTATIONFIXTURE::Output);
        RegisterCeefitTest(this, "RenderedOutput", &FAT_STANDARDANNOTATIONFIXTURE::RenderedOutput);
      }

      virtual inline ceefit_init_spec ~FAT_STANDARDANNOTATIONFIXTURE(void)
      {
      }

    private:
      ceefit_init_spec FAT_STANDARDANNOTATIONFIXTURE(FAT_STANDARDANNOTATIONFIXTURE& afixture);    /**< Not implemented, do not call */
      FAT_STANDARDANNOTATIONFIXTURE& ceefit_init_spec operator=(FAT_STANDARDANNOTATIONFIXTURE& afixture);   /**< Not implemented, do not call */
  };

  static ::CEEFIT::REGISTERFIXTURECLASS< FAT_STANDARDANNOTATIONFIXTURE > StandardAnnotationFixtureRegistration("FAT_STANDARDANNOTATIONFIXTURE", "fat.StandardAnnotationFixture");
};
