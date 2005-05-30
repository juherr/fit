// Copyright (c) 2002 Cunningham & Cunningham, Inc.
// Released under the terms of the GNU General Public License version 2 or later.

package eg.music;

import fit.*;
import java.util.Date;

namespace EG_MUSIC
{
  class REALTIME : public TIMEDACTIONFIXTURE 
  {
    protected:
      Simulator system = Simulator.system;

    private:
      void ceefit_call_spec System(const STRING& prefix, PTR<PARSE>& cell) 
      {
          STRING method(this->Camel( prefix + " " + cell->Text()));
          
          Class[] empty = {};
          try {
              system.getClass().getMethod(method,empty).invoke(system,empty);
          } catch (Exception e) {
              exception (cell, e);
          }
      }

    public:
      virtual INT64 ceefit_call_spec Time() 
      {
        return new Date(Simulator.time);
      }

      public void ceefit_call_spec pause () 
      {
          double seconds = Double.parseDouble(cells.more.text());
          system.delay(seconds);
      }

      public void ceefit_call_spec await () 
      {
          system("wait", cells.more);
      }

      public void ceefit_call_spec fail () 
      {
          system("fail", cells.more);
      }

      public void ceefit_call_spec enter() 
      {
          system.delay(0.8);
          super.enter();
      }

      public void ceefit_call_spec press() 
      {
          system.delay(1.2);
          super.press();
      }
  };
};