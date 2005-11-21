#ifndef __CEEFIT_PARSE_H__
#define __CEEFIT_PARSE_H__

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

namespace CEEFIT
{
  class PARSE : public REFCOUNTED
  {
    private:
      /**
       * Constructor helper
       */
      void ceefit_call_spec Construct(const STRING& text, const DYNARRAY<STRING>& tags, int level, int offset);

    public:
      STRING Leader;
      STRING Tag;
      STRING Body;
      STRING End;
      STRING Trailer;

      PTR<PARSE> More;
      PTR<PARSE> Parts;

      ceefit_init_spec PARSE(const STRING& aTag, const STRING& aBody, PTR<PARSE>& aParts, PTR<PARSE>& aMore);
      virtual ceefit_dtor_spec ~PARSE(void);
      ceefit_init_spec PARSE(const STRING& text, const DYNARRAY<STRING>& tags, int level, int offset);

      virtual void ceefit_call_spec AddToTag(const STRING& text);
      virtual void ceefit_call_spec AddToBody(const STRING& text);

      virtual int ceefit_call_spec Size(void);
      virtual VALUE<PARSE> ceefit_call_spec Last(void);
      virtual VALUE<PARSE> ceefit_call_spec Leaf(void);
      virtual VALUE<PARSE> ceefit_call_spec At(int i);
      virtual VALUE<PARSE> ceefit_call_spec At(int i, int j);
      virtual VALUE<PARSE> ceefit_call_spec At(int i, int j, int k);
      virtual STRING ceefit_call_spec Text();

      virtual void ceefit_call_spec Print(WRITER* out);

      static STRING ceefit_call_spec HtmlToText(const STRING& s);
      static STRING ceefit_call_spec Unescape(const STRING& s);
      static STRING ceefit_call_spec CondenseWhitespace(const STRING& s);

      static int FootnoteFiles;
      virtual STRING ceefit_call_spec Footnote(void);

      virtual bool ceefit_call_spec IsEqual(const PARSE& aParse) const;

    private:
      static STRING ceefit_call_spec RemoveNonBreakTags(const STRING& s);
	    static STRING ceefit_call_spec UnescapeSmartQuotes(const STRING& s);
	    static STRING ceefit_call_spec UnescapeEntities(const STRING& s);
	    static STRING ceefit_call_spec NormalizeLineBreaks(const STRING& s);

      static const wchar_t* tags[];

      /**
       * <p>Turns the tags wchar_t* array into a DYNARRAY&lt;STRING&gt;</p>
       */
      static const wchar_t** ceefit_call_spec GetTagsStrings(void);

    public:
      ceefit_init_spec PARSE(const STRING& text);
      ceefit_init_spec PARSE(const STRING& text, const DYNARRAY<STRING>& tags);

    protected:
      ceefit_init_spec PARSE(const PARSE&);
      PARSE& ceefit_call_spec operator=(const PARSE&);
  };

};

#endif // __CEEFIT_PARSE_H__
