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

declare_fit_module(FatAnnotationFixture);

using namespace CEEFIT;

namespace CEEFAT
{
  class ANNOTATIONFIXTURE : public COLUMNFIXTURE
  {
    public:
	    STRING OriginalHTML;
	    int Row;
	    int Column;
	    
	    STRING OverwriteCellBody;
	    STRING AddToCellBody;
	    
	    STRING OverwriteCellTag;
	    STRING OverwriteEndCellTag;
	    STRING AddToCellTag;
	    
	    STRING OverwriteRowTag;
	    STRING OverwriteEndRowTag;
	    STRING AddToRowTag;

	    STRING OverwriteTableTag;
	    STRING OverwriteEndTableTag;
	    STRING AddToTableTag;
	    
	    STRING AddCellFollowing;
	    STRING RemoveFollowingCell;
	    
	    STRING AddRowFollowing;
	    STRING RemoveFollowingRow;
	    
	    STRING AddTableFollowing;

	  private:
    	// code smell note: copied from DocumentParseFixture	
      STRING ceefit_call_spec GenerateOutput(PTR<PARSE>& document)
      {
        STRINGWRITER writer;

        document->Print(&writer);

		    return writer.ToString().Trim();
	    }

      void AddParse(PTR<PARSE>& parse, const STRING& newString, const char** tags) 
      {
        DYNARRAY<STRING> tagList;
        int i = 0;
        while(tags[i] != null) 
        {
          tagList.Add(tags[i]);
          i++;
        }

        PTR<PARSE> newParse(new PARSE(newString, tagList));
        newParse->More = parse->More;
        newParse->Trailer = parse->Trailer;
        parse->More = newParse;
        parse->Trailer.Reset();
      }

      void RemoveParse(PTR<PARSE>& parse) 
      {
        parse->Trailer = parse->More->Trailer;

        PTR<PARSE> temp(parse->More->More);   // this temp PTR<PARSE> will keep any orphan references alive 
        parse->More = temp;
      }

      STRING StripDelimiters(const STRING& s) 
      {
        STRING temp;
        
        DYNARRAY<STRING> firstPattern;
        firstPattern.Add("^");
        firstPattern.Add("\\[");
        temp = s.ArrayRegexPatternReplaceAll(firstPattern, STRING(""));

        DYNARRAY<STRING> secondPattern;
        secondPattern.Add("]");
        secondPattern.Add("$");
        temp = temp.ArrayRegexPatternReplaceAll(secondPattern, STRING(""));
        
        return temp;
          
        // this abomination is the equivalent of return s.replaceAll("^\\[", "").replaceAll("]$", ""); from the Java version
      }

    public:
      virtual STRING ceefit_call_spec ResultingHTML(void) 
      {
        PTR<PARSE> table(new PARSE(OriginalHTML));
        PTR<PARSE> row(table->At(0, Row - 1));
        PTR<PARSE> cell(row->At(0, Column - 1));

        if (OverwriteCellBody.IsAssigned()) cell->Body = OverwriteCellBody;
        if (AddToCellBody.IsAssigned()) cell->AddToBody(AddToCellBody);

        if (OverwriteCellTag.IsAssigned()) cell->Tag = OverwriteCellTag;
        if (OverwriteEndCellTag.IsAssigned()) cell->End = OverwriteEndCellTag;
        if (AddToCellTag.IsAssigned()) cell->AddToTag(StripDelimiters(AddToCellTag));

        if (OverwriteRowTag.IsAssigned()) row->Tag = OverwriteRowTag;
        if (OverwriteEndRowTag.IsAssigned()) row->End = OverwriteEndRowTag;
        if (AddToRowTag.IsAssigned()) row->AddToTag(StripDelimiters(AddToRowTag));

        if (OverwriteTableTag.IsAssigned()) table->Tag = OverwriteTableTag;
        if (OverwriteEndTableTag.IsAssigned()) table->End = OverwriteEndTableTag;
        if (AddToTableTag.IsAssigned()) table->AddToTag(StripDelimiters(AddToTableTag));

        static const char* cellFollowingTags[] = {"td", null};
        if (AddCellFollowing.IsAssigned()) AddParse(cell, AddCellFollowing, cellFollowingTags);
        if (RemoveFollowingCell.IsAssigned()) RemoveParse(cell);

        static const char* rowFollowingTags[] = {"tr", "td", null};
        if (AddRowFollowing.IsAssigned()) AddParse(row, AddRowFollowing, rowFollowingTags);
        if (RemoveFollowingRow.IsAssigned()) RemoveParse(row);

        static const char* tableFollowingTags[] = {"table", "tr", "td", null};
        if (AddTableFollowing.IsAssigned()) AddParse(table, AddTableFollowing, tableFollowingTags);

        return GenerateOutput(table);        
      }

      ceefit_init_spec ANNOTATIONFIXTURE(void) 
      {
        RegisterCeefitField(this, "OriginalHTML", OriginalHTML);
        RegisterCeefitField(this, "Row", Row);
        RegisterCeefitField(this, "Column", Column);

        RegisterCeefitField(this, "OverwriteCellBody", OverwriteCellBody);
        RegisterCeefitField(this, "AddToCellBody", AddToCellBody);
	      
        RegisterCeefitField(this, "OverwriteCellTag", OverwriteCellTag);
        RegisterCeefitField(this, "OverwriteEndCellTag", OverwriteEndCellTag);
        RegisterCeefitField(this, "AddToCellTag", AddToCellTag);
	      
        RegisterCeefitField(this, "OverwriteRowTag", OverwriteRowTag);
        RegisterCeefitField(this, "OverwriteEndRowTag", OverwriteEndRowTag);
        RegisterCeefitField(this, "AddToRowTag", AddToRowTag);

        RegisterCeefitField(this, "OverwriteTableTag", OverwriteTableTag);
        RegisterCeefitField(this, "OverwriteEndTableTag", OverwriteEndTableTag);
        RegisterCeefitField(this, "AddToTableTag", AddToTableTag);
	      
        RegisterCeefitField(this, "AddCellFollowing", AddCellFollowing);
        RegisterCeefitField(this, "RemoveFollowingCell", RemoveFollowingCell);
	      
        RegisterCeefitField(this, "AddRowFollowing", AddRowFollowing);
        RegisterCeefitField(this, "RemoveFollowingRow", RemoveFollowingRow);
	      
        RegisterCeefitField(this, "AddTableFollowing", AddTableFollowing);
      
        RegisterCeefitTest(this, "ResultingHTML", &ANNOTATIONFIXTURE::ResultingHTML);
      }

      virtual ceefit_dtor_spec ~ANNOTATIONFIXTURE(void) 
      {
      }

    private:
      ceefit_init_spec ANNOTATIONFIXTURE(const ANNOTATIONFIXTURE&);             /**< Not implemented, do not call */
      ANNOTATIONFIXTURE& ceefit_call_spec operator=(const ANNOTATIONFIXTURE&);  /**< Not implemented, do not call */
  };

  static ::CEEFIT::REGISTERFIXTURECLASS< ANNOTATIONFIXTURE > AnnotationFixtureRegistration("CEEFAT::ANNOTATIONFIXTURE", "fat.AnnotationFixture");
};
