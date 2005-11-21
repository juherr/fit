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

#define lengthof(a)       (sizeof(a) / sizeof((a)[0]))

namespace CEEFIT
{
  const wchar_t* PARSE::tags[] = {L"table", L"tr", L"td", null};

  ceefit_init_spec PARSE::PARSE(const STRING& aTag, const STRING& aBody, PTR<PARSE>& aParts, PTR<PARSE>& aMore)
  {
    Leader = "\n";
    Tag = STRING("<") + aTag + ">";
    Body = aBody;
    End = STRING("</") + aTag + ">";
    Trailer = "";
    Parts = aParts;
    More = aMore;
  }

  ceefit_dtor_spec PARSE::~PARSE()
  {
  }

  bool PARSE::IsEqual(const PARSE& aParse) const
  {
    throw new EXCEPTION("PARSE::IsEqual not yet implemented");
  }

	/* Added by Rick Mugridge, Feb 2005, adapted by Dave Woldrich for CeeFIT Apr 2005 */
	static int findMatchingEndTag(const STRING& lc, int matchFromHere, const STRING& tag, int offset) 
  {
		int fromHere = matchFromHere;
		int count = 1;
		int startEnd = 0;
		while (count > 0) 
    {
			int embeddedTag = lc.IndexOf(STRING("<") + tag, fromHere);
			int embeddedTagEnd = lc.IndexOf(STRING("</") + tag, fromHere);
			// Which one is closer?
			if (embeddedTag < 0 && embeddedTagEnd < 0)
				throw new PARSEEXCEPTION(STRING("Can't find tag: ") + tag, offset);
			if (embeddedTag < 0)
				embeddedTag = INT_MAX;
			if (embeddedTagEnd < 0)
				embeddedTagEnd = INT_MAX;
			if (embeddedTag < embeddedTagEnd) 
      {
				count++;
				startEnd = embeddedTag;
				fromHere = lc.IndexOf(STRING(">"), embeddedTag) + 1;
			}
		  else if (embeddedTagEnd < embeddedTag) 
      {
				count--;
				startEnd = embeddedTagEnd;
				fromHere = lc.IndexOf(STRING(">"), embeddedTagEnd) + 1;
			}
		}
		return startEnd;
	}

  void ceefit_call_spec PARSE::Construct(const STRING& text, const DYNARRAY<STRING>& tags, int level, int offset)
  {
    STRING lc(text.ToLowercase());
    int startTag = lc.IndexOf(STRING("<")+tags[level]);
    int endTag = lc.IndexOf(STRING(">"), startTag) + 1;
    //int startEnd = lc.IndexOf(STRING("</")+tags[level], endTag);
		int startEnd = findMatchingEndTag(lc, endTag, tags[level], offset);
    int endEnd = lc.IndexOf(STRING(">"), startEnd) + 1;
    int startMore = lc.IndexOf(STRING("<") + tags[level], endEnd);
    if (startTag<0 || endTag<0 || startEnd<0 || endEnd<0)
    {
      throw new PARSEEXCEPTION(STRING("Can't find tag: ") + GetTagsStrings()[level], offset);
    }

    Leader = text.Substring(0, startTag);
    Tag = text.Substring(startTag, endTag);
    Body = text.Substring(endTag, startEnd);
    End = text.Substring(startEnd, endEnd);
    Trailer = text.Substring(endEnd);

    if ((level+1) < tags.GetSize())
    {
      Parts = new PARSE(Body, tags, level + 1, offset + endTag);
      Body.Reset();
    }
		else 
    { 
      // Check for nested table
			int index = Body.IndexOf(STRING("<") + tags[0]);
			if (index >= 0) 
      {
				Parts = new PARSE(Body, tags, 0, offset + endTag);
				Body = L"";
			}
		}

    if (startMore>=0)
    {
      More = new PARSE(Trailer, tags, level, offset+endEnd);
      Trailer.Reset();
    }
  }

  ceefit_init_spec PARSE::PARSE(const STRING& text)
  {
    DYNARRAY<STRING> tags;
    const wchar_t** tagArray = GetTagsStrings();

    int i = 0;
    while(tagArray[i] != null)
    {
      tags.Add(tagArray[i]);
      i++;
    }

    Construct(text, tags, 0, 0);
  }

  ceefit_init_spec PARSE::PARSE(const STRING& text, const DYNARRAY<STRING>& tags)
  {
    Construct(text, tags, 0, 0);
  }

  ceefit_init_spec PARSE::PARSE(const STRING& text, const DYNARRAY<STRING>& tags, int level, int offset)
  {
    Construct(text, tags, level, offset);
  }

  // this should never be called
  ceefit_init_spec PARSE::PARSE(const PARSE&)
  {
  }

  // this should never be called
  PARSE& ceefit_call_spec PARSE::operator=(const PARSE&)
  {
    assert(false);
    return(*this);
  }

  const wchar_t** ceefit_call_spec PARSE::GetTagsStrings()
  {
    return(tags);
  }

  void ceefit_call_spec PARSE::AddToTag(const STRING& text)
  {
    int last = Tag.Length()-1;
    Tag = Tag.Substring(0, last) + text + ">";
  }

  void ceefit_call_spec PARSE::AddToBody(const STRING& text)
  {
    Body = Body + text;
  }

  int ceefit_call_spec PARSE::Size()
  {
    return More==null ? 1 : More->Size()+1;
  }

  VALUE<PARSE> ceefit_call_spec PARSE::Last()
  {
    PARSE* thisParse = this;

    return More==null ? VALUE<PARSE>(thisParse) : VALUE<PARSE>(More->Last());
  }

  VALUE<PARSE> ceefit_call_spec PARSE::Leaf()
  {
    return Parts==null ? VALUE<PARSE>(this) : VALUE<PARSE>(Parts->Leaf());
  }

  VALUE<PARSE> ceefit_call_spec PARSE::At(int i)
  {
    return (i==0 || More==null) ? VALUE<PARSE>(this) : VALUE<PARSE>(More->At(i-1));
  }

  VALUE<PARSE> ceefit_call_spec PARSE::At(int i, int j)
  {
    PTR<PARSE> parentParsePtr(At(i));

    return parentParsePtr->Parts->At(j);
  }

  VALUE<PARSE> ceefit_call_spec PARSE::At(int i, int j, int k)
  {
    PTR<PARSE> parentParsePtr(At(i,j));

    return parentParsePtr->Parts->At(k);
  }

  STRING ceefit_call_spec PARSE::Text()
  {
    return HtmlToText(Body);
  }

  STRING ceefit_call_spec PARSE::HtmlToText(const STRING& s)
  {
		STRING temp(s);

    temp = NormalizeLineBreaks(temp);
    temp = RemoveNonBreakTags(temp);
		temp = CondenseWhitespace(temp);
		temp = Unescape(temp);

    return temp;
  }

  STRING ceefit_call_spec PARSE::RemoveNonBreakTags(const STRING& s)
  {
    STRING temp(s);

    int i = 0, j;
    while((i = temp.IndexOf('<', i)) >= 0)
    {
      if((j = temp.IndexOf('>', i+1)) > 0)
      {
        if(!(temp.Substring(i, j+1).IsEqual("<br />")))
        {
          temp = temp.Substring(0, i) + temp.Substring(j+1);
        }
        else
        {
          i++;
        }
      }
      else
      {
        break;
      }
    }

    return temp;
  }

  STRING ceefit_call_spec PARSE::Unescape(const STRING& s)
  {
    STRING temp(s);

    temp = temp.SimplePatternReplaceAll("<br />", "\n");
		temp = UnescapeEntities(temp);
		temp = UnescapeSmartQuotes(temp);

    return temp;
  }

	STRING ceefit_call_spec PARSE::UnescapeSmartQuotes(const STRING& s)
  {
    STRING temp(s);

		temp = temp.Replace(L'\x201c', L'\"');
		temp = temp.Replace(L'\x201d', L'\"');
		temp = temp.Replace(L'\x2018', L'\'');
		temp = temp.Replace(L'\x2019', L'\'');

		return temp;
	}

	STRING ceefit_call_spec PARSE::UnescapeEntities(const STRING& s)
  {
    STRING temp(s);

		temp = temp.SimplePatternReplaceAll("&lt;", "<");
		temp = temp.SimplePatternReplaceAll("&gt;", ">");
		temp = temp.SimplePatternReplaceAll("&nbsp;", " ");
		temp = temp.SimplePatternReplaceAll("&quot;", "\"");
		temp = temp.SimplePatternReplaceAll("&amp;", "&");

		return temp;
	}

  static const char* RegexSpaceCharacters = "[ \t\n\x0B\f\r]";

	STRING ceefit_call_spec PARSE::NormalizeLineBreaks(const STRING& s)
  {
    STRING temp(s);

    DYNARRAY<STRING> brPatternArray;
    brPatternArray.Add("<");
    brPatternArray.Add(STRING(RegexSpaceCharacters) + "*");
    brPatternArray.Add("br");
    brPatternArray.Add(STRING(RegexSpaceCharacters) + "*");
    brPatternArray.Add("/?");
    brPatternArray.Add(STRING(RegexSpaceCharacters) + "*");
    brPatternArray.Add(">");
    temp = temp.ArrayRegexPatternReplaceAll(brPatternArray, "<br />");   // was				s = s.replaceAll("<\\s*br\\s*/?\\s*>", "<br />");

    DYNARRAY<STRING> pPatternArray;
    pPatternArray.Add("<");
    pPatternArray.Add(STRING(RegexSpaceCharacters) + "*");
    pPatternArray.Add("/");
    pPatternArray.Add(STRING(RegexSpaceCharacters) + "*");
    pPatternArray.Add("p");
    pPatternArray.Add(STRING(RegexSpaceCharacters) + "*");
    pPatternArray.Add(">");
    pPatternArray.Add(STRING(RegexSpaceCharacters) + "*");
    pPatternArray.Add("<");
    pPatternArray.Add(STRING(RegexSpaceCharacters) + "*");
    pPatternArray.Add("p");
    pPatternArray.Add(STRING(RegexSpaceCharacters) + "+??");
    pPatternArray.Add(".*?");
    pPatternArray.Add(">");
		temp = temp.ArrayRegexPatternReplaceAll(pPatternArray, "<br />", true);  // was    		s = s.replaceAll("<\\s*/\\s*p\\s*>\\s*<\\s*p( .*?)?>", "<br />");

		return temp;
	}

  STRING ceefit_call_spec PARSE::CondenseWhitespace(const STRING& s)
  {
    static const wchar_t NON_BREAKING_SPACE = L'\x00a0';

    STRING temp(s);

    DYNARRAY<STRING> spacePatternArray;
    spacePatternArray.Add(STRING(RegexSpaceCharacters) + "+");

    temp = temp.ArrayRegexPatternReplaceAll(spacePatternArray, " ");  // temp = temp.ReplaceAll("[ \t\n\x0B\f\r]+", " ");    
	  temp = temp.Replace(NON_BREAKING_SPACE, L' ');
    temp = temp.SimplePatternReplaceAll("&nbsp;", " ");
	  temp = temp.Trim();

    return temp;
  }

  void ceefit_call_spec PARSE::Print(WRITER* out)
  {
    out->Fprint(L"%s", Leader.GetBuffer());
    out->Fprint(L"%s", Tag.GetBuffer());
    if(Parts != null)
    {
      Parts->Print(out);
    }
    else
    {
      out->Fprint(L"%s", Body.GetBuffer());
    }
    out->Fprint(L"%s", End.GetBuffer());

    if(More != null)
    {
      More->Print(out);
    }
    else
    {
      out->Fprint(L"%s", Trailer.GetBuffer());
    }
  }

  int PARSE::FootnoteFiles = 0;

  STRING ceefit_call_spec PARSE::Footnote()
  {
    if(FootnoteFiles >= 25)
    {
      return "[-]";
    }
    else
    {
      try
      {
        int thisFootnote = ++FootnoteFiles;
        STRING html(STRING("footnotes/") + thisFootnote + ".html");

        STRING fullPath(STRING("Reports/") + html);

        WRITER* output = new FILEWRITER(fullPath);
        if(output == null)
        {
          throw new IOEXCEPTION("Create Footnote file operation failed");
        }

        Print(output);
        output->Close();

        return (STRING("<a href=/fit/Release/Reports/") + html + "> [" + thisFootnote + "]</a>");
      }
      catch (IOEXCEPTION* e)
      {
        delete e;

        return "[!]";
      }
    }
  }

};
