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

declare_fit_module(FatTable);

using namespace CEEFIT;

namespace CEEFAT
{

  void ceefit_call_spec FAT_TABLE::DoRows(PTR<PARSE>& rows)
  {
    PTR<PARSE> nullParse(NULL);
    STRING emptyString;

    PTR<PARSE> rowCopy(Copy(rows));
    FAT_TABLE::Table = new PARSE(STRING("table"), emptyString, rowCopy, nullParse);

    // evaluate the rest of the table like a runner
    PTR<FIXTURE> temp(new FIXTURE());
    temp->DoTables(FAT_TABLE::Table);
  }

  VALUE<PARSE> ceefit_call_spec FAT_TABLE::Copy(PTR<PARSE>& tree)
  {
    if(tree == NULL)
    {
      return(VALUE<PARSE>(NULL));
    }
    else
    {
      PTR<PARSE> treeParts(Copy(tree->Parts));
      PTR<PARSE> treeMore(Copy(tree->More));

      return(VALUE<PARSE>(new PARSE(tree->Tag, tree->Body, treeParts, treeMore)));
    }
  }

  static REGISTERFIXTURECLASS< FAT_TABLE > FAT_TABLE_CeeFITFixtureRegistration("FAT_TABLE", "fat.Table");

  PTR<PARSE> FAT_TABLE::Table(NULL);

};
