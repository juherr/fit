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
#include "eg/eg.h"

using namespace CEEFIT;

namespace EG
{
  STRING PAGE::Url;
  STRING PAGE::Text;

  void ceefit_call_spec PAGE::Location(const STRING& url) 
  {
    PAGE::Url = url;
    PAGE::Text = Get(PAGE::Url);
  }

  STRING ceefit_call_spec PAGE::Title(void) 
  {
    DYNARRAY<CEEFIT::STRING> tags;
    tags.Add("title");
    PTR<PARSE> aParse(new PARSE(PAGE::Text, tags));
  
    return(aParse->Text());
  }

  void ceefit_call_spec PAGE::Link(const STRING& label) 
  {
    DYNARRAY<STRING> tags;
    tags.Add("a");
    PTR<PARSE> links(new PARSE(PAGE::Text, tags));

    while(!links->Text().StartsWith(label)) 
    {
      links = links->More;
    }
    DYNARRAY<STRING> tokens;
    Tokenize(tokens, links->Tag, STRING("<> =\""));
    int i = 0;
    while(i < tokens.GetSize() && !tokens[i].ToLowercase().IsEqual("href")) 
    {
      i++;
    }
    Url = Url + tokens[i+1];
    Text = Get(Url);
  }

  // rows /////////////////////////////////////

  void ceefit_call_spec PAGE::Query(DYNARRAY< PTR<CELLADAPTER> >& out) 
  {
    throw new EXCEPTION("Unimplemented function");
    /*
    String tags[] = {"wiki", "table", "tr", "td"};
    Parse rows = new Parse(text, tags).at(0, 0, 2);
    Row result[] = new Row[rows.size()];
    for (int i=0; i<result.length; i++) 
    {
      result[i] = new Row(rows);
      rows = rows.more;
    }
    return result;*/
  }

  /*
  class Row 
  {
    Parse cells;

    Row(Parse row) 
    {
      this.cells = row.parts;
    }

    STRING Numerator() 
    {
      return Cells->At(0)->Text();
    }

    STRING Denominator() 
    {
      return Cells->At(1)->Text();
    }

    public String quotient() {
      return cells.at(2).text();
    }
  }
  */

  // utilities ////////////////////////////////

  STRING ceefit_call_spec PAGE::Get(const STRING& url) 
  {
    throw new EXCEPTION("Unimplemented function");
    /*
    Page.url = url;
    InputStream stream =  (InputStream) url.getContent();
    BufferedReader reader = new BufferedReader(new InputStreamReader(stream));
    StringBuffer buffer = new StringBuffer(10000);
    for (String line; (line = reader.readLine()) != null; ) {
      buffer.append(line);
      buffer.append('\n');
    }
    return buffer.toString();
    */
  }
};