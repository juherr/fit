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
#include "fat/Table.h"

declare_fit_module(FatColor);

extern "C"
{
  // defined in ceefit.lib
  typedef unsigned int unicode_char_t;
  extern int __cdecl unicode_isspace (unicode_char_t c);
};

using namespace CEEFIT;

namespace CEEFAT
{
  begin_namespaced_fit_fixture(CEEFAT, COLOR, PRIMITIVEFIXTURE, fat.Color)

    public:
      PTR<PARSE> ActualRow;

    private:
      virtual STRING Color(PTR<PARSE>& cell)
      {
        STRING b(Extract(cell->Tag, "bgcolor=\"", "white"));
        STRING f(Extract(cell->Body, "<font color=", "black"));

        return (f.IsEqual("black") ? b : f+"/"+b);
      }

      virtual STRING Extract(const STRING& text, const STRING& pattern, const STRING& defaultColor)
      {
        int index = text.IndexOf(pattern);
        if (index < 0)
        {
          return defaultColor;
        }

        index += pattern.Length();
        while(text.CharAt(index) == '\'' || text.CharAt(index) == '\"' || unicode_isspace(text.CharAt(index)))
        {
          index++;
        }
        return(Decode(text.Substring(index, index+7)));
      }

      virtual STRING Decode(const STRING& code)
      {
          return
              code.IsEqual(FIXTURE::red) ?      "red" :
              code.IsEqual(FIXTURE::green) ?    "green" :
              code.IsEqual(FIXTURE::yellow) ?   "yellow" :
              code.IsEqual(FIXTURE::gray) ?     "gray" :
              code.IsEqual("#808080") ?        "gray" :
              code;
      }

    public:
      virtual void ceefit_call_spec DoRows(PTR<PARSE>& rows)
      {
        ActualRow = TABLE::Table->Parts;
        if (rows->Size() != ActualRow->Size())
        {
          throw new EXCEPTION("wrong size table");
        }
        PRIMITIVEFIXTURE::DoRows(rows);
      }

      virtual void ceefit_call_spec DoRow(PTR<PARSE>& row)
      {
        PRIMITIVEFIXTURE::DoRow(row);

        ActualRow = ActualRow->More;
      }

      virtual void ceefit_call_spec DoCell(PTR<PARSE>& cell, int columnNumber)
      {
        PTR<PARSE> temp(ActualRow->Parts->At(columnNumber));

        Check(cell, Color(temp));
      }

  end_namespaced_fit_fixture(CEEFAT, COLOR)

};
