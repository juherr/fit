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

//todo...

/*
  // Traversal ////////////////////////////////
  void ACTIONFIXTURE::doCells(PTR<PARSE>& cells) 
  {
      Cells = cells;
      try 
      {          
          Method action = getClass().getMethod(Cells.text(), empty);
          action.invoke(this, empty);
      } 
      catch (Exception e) 
      {
          exception(cells, e);
      }
  }

  // Actions //////////////////////////////////

  public void start() throws Exception {
      actor = (Fixture)(Class.forName(cells.more.text()).newInstance());
  }

  public void enter() throws Exception {
      Method method = method(1);
      Class type = method.getParameterTypes()[0];
      String text = cells.more.more.text();
      Object args[] = {TypeAdapter.on(actor, type).parse(text)};
      method.invoke(actor, args);
  }

  public void press() throws Exception {
      method(0).invoke(actor, empty);
  }

  public void check() throws Exception {
      TypeAdapter adapter = TypeAdapter.on(actor, method(0));
      check (cells.more.more, adapter);
  }

  // Utility //////////////////////////////////

  protected Method method(int args) throws NoSuchMethodException {
      return method(camel(cells.more.text()), args);
  }

  protected Method method(String test, int args) throws NoSuchMethodException {
      Method methods[] = actor.getClass().getMethods();
      Method result = null;
      for (int i=0; i<methods.length; i++) {
          Method m = methods[i];
          if (m.getName().equals(test) && m.getParameterTypes().length == args) {
              if (result==null) {
                  result = m;
              } else {
                  throw new NoSuchMethodException("too many implementations");
              }
          }
      }
      if (result==null) {
          throw new NoSuchMethodException();
      }
      return result;
  }
*/
