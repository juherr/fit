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

declare_fit_module(FatAnnotationFixture);

using namespace CEEFIT;

namespace CEEFAT
{
  class FAT_ANNOTATIONFIXTURE : public COLUMNFIXTURE
  {
    public:
      STRING Type;
      STRING Text;
      STRING OriginalCell;

	  private:
  	  // code smell note: copied from ParseFixture
      STRING GenerateOutput(PTR<PARSE>& parse)
      {
        STRINGWRITER writer;

        parse->Print(&writer);

		    return writer.ToString().Trim();
	    }

    public:
      virtual STRING Output()
      {
        PTR<PARSE> nullPointer;
		    PTR<PARSE> parse(new PARSE(STRING("td"), OriginalCell, nullPointer, nullPointer));
		    FIXTURE hack;

		    if (Type.IsEqual("none")) {
			    // do nothing
		    }
		    else if (Type.IsEqual("right"))
        {
			    hack.Right(parse);
		    }
		    else if (Type.IsEqual("wrong"))
        {
			    hack.Wrong(parse, Text);
		    }
		    else if (Type.IsEqual("error"))
        {
			    return "not implemented";
		    }
		    else if (Type.IsEqual("ignore"))
        {
			    hack.Ignore(parse);
		    }
		    else if (Type.IsEqual("unchecked"))
        {
			    return "not implemented";
		    }
		    else
        {
			    return STRING("unknown type: ") + Type;
		    }

		    return GenerateOutput(parse);
	    }
      
      FAT_ANNOTATIONFIXTURE(VOID) 
      {
        OriginalCell = "Text";

        RegisterCeefitField(this, "Type", Type);
        RegisterCeefitField(this, "Text", Text);
        RegisterCeefitField(this, "OriginalCell", OriginalCell);

        RegisterCeefitTest(this, "Output", &FAT_ANNOTATIONFIXTURE::Output);
      }

      virtual ~FAT_ANNOTATIONFIXTURE(VOID) 
      {
      }
  };

  static ::CEEFIT::REGISTERFIXTURECLASS< FAT_ANNOTATIONFIXTURE > AnnotationFixtureRegistration("FAT_ANNOTATIONFIXTURE", "fat.AnnotationFixture");
};
