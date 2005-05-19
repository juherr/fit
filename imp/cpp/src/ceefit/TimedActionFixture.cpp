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

// todo
namespace CEEFIT
{
  // Traversal ////////////////////////////////

  void TIMEDACTIONFIXTURE::DoTable(PTR<PARSE>& table) 
  {
    ACTIONFIXTURE::DoTable(table);  

    PTR<PARSE> lastPart(table->Parts->Parts->Last());
    td(lastPart->More, "time");

    lastPart = table->Parts->Parts->Last();
    td(lastPart->More, "split");
  }

  void TIMEDACTIONFIXTURE::DoCells(PTR<PARSE>& cells) 
  {
    INT64 start = Time();
    ACTIONFIXTURE::DoCells(cells);
    INT64 split = Time() - start;

      
    INT64 temp = split;
    int hours = temp / 3600000;
    temp = temp % 3600000;
    int minutes = temp / 60000;
    temp = temp % 60000;
    int seconds = temp / 1000;

    char printfOut[128];
    sprintf(printfOut, "%02i:%02i:%02i", hours, minutes, seconds);

    PTR<PARSE> lastCell(cells->Last());
    td(lastCell->More, STRING(printfOut));

    lastCell = cells->Last();
    if(split < 1000) 
    {
      td(lastCell->More, STRING("&nbsp;"));
    }
    else 
    {
      sprintf(printfOut, "%g", (((double) split) / 1000.0));
      td(lastCell->More, STRING(printfOut));
    }
  }

  // Utility //////////////////////////////////

  INT64 TIMEDACTIONFIXTURE::Time() 
  {
    return CurrentTimeMillis();
  }

  void TIMEDACTIONFIXTURE::td(PTR<PARSE>& out, const STRING& body) 
  {
    PTR<PARSE> nullPtr;
    out = new PARSE(STRING("td"), this->Info(body), nullPtr, nullPtr);
  }
};
