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

declare_fit_module(Summary);

namespace CEEFIT
{

  begin_fit_fixture(FIT_SUMMARY, FIXTURE, fit.Summary)

    protected:
      VALUE<PARSE> tr(PTR<PARSE>& parts, PTR<PARSE>& more)
      {
        STRING aTag("tr");
        STRING aBody("");

        return(VALUE<PARSE>(new PARSE(aTag, aBody, parts, more)));
      }

      VALUE<PARSE> td(const STRING& body, PTR<PARSE>& more)
      {
        STRING aTag("td");
        STRING aBody(Gray(body));
        PTR<PARSE> nullParts(NULL);

        return(VALUE<PARSE>(new PARSE(aTag, aBody, nullParts, more)));
      }

      void Mark(PTR<PARSE>& row)
      {
        // mark summary good/bad without counting beyond here
        PTR<FIXTURE::COUNTS> official(CountsObj);

        CountsObj = new FIXTURE::COUNTS();
        PTR<PARSE> cell(row->Parts->More);
        if (official->wrong + official->exceptions > 0)
        {
          Wrong(cell);
        }
        else
        {
          Right(cell);
        }

        // restore the old Counts
        CountsObj = official;
      }

      void Rows(PTR<PARSE>& out, DYNARRAY<FIXTURE::SUMMARYITEM>& keys, int aIndex)
      {
        if(aIndex < keys.GetSize())
        {
          PTR<PARSE> nullParse(NULL);
          PTR<PARSE> valueTd(td(keys[aIndex].Value, nullParse));
          PTR<PARSE> titleTd(td(keys[aIndex].Title, valueTd));
          PTR<PARSE> rowsParse;

          Rows(rowsParse, keys, aIndex+1);

          out = tr(titleTd, rowsParse);
          if(keys[aIndex].Title.StartsWith("Counts"))
          {
            Mark(out);
          }
        }
        else
        {
          out = (PARSE*) NULL;
        }
      }

    public:
      virtual void ceefit_call_spec DoTable(PTR<PARSE>& table)
      {
        PTR<PARSE> temp(table);

        DYNARRAY<FIXTURE::SUMMARYITEM> reportList;
        GetSummaryReport(reportList);

        Rows(temp->Parts->More, reportList, 0);
      }

  end_fit_fixture(FIT_SUMMARY);

};
