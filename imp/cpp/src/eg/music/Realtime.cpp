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

declare_fit_module(MusicRealtime);

using namespace CEEFIT;

namespace EG_MUSIC
{
  fitINT64 ceefit_call_spec REALTIME::Time()
  {
    return SIMULATOR::Time;
  }

  void ceefit_call_spec REALTIME::Pause()
  {
    int seconds = 0;

    STRING cellsText(Cells->More->Text());
    if(swscanf(cellsText.GetBuffer(), L"%i", &seconds) == 0)
    {
      throw new PARSEEXCEPTION(STRING("REALTIME parse failure"));
    }
    system->Delay((double) seconds);
  }

  void ceefit_call_spec REALTIME::Await()
  {
    System("wait", Cells->More);
  }

  void ceefit_call_spec REALTIME::Fail()
  {
    System("fail", Cells->More);
  }

  void ceefit_call_spec REALTIME::Enter()
  {
    system->Delay(0.8);
    this->TIMEDACTIONFIXTURE::Enter();
  }

  void ceefit_call_spec REALTIME::Press()
  {
    system->Delay(1.2);
    this->TIMEDACTIONFIXTURE::Press();
  }

  ceefit_init_spec REALTIME::REALTIME()
  {
    system = SIMULATOR::System;

    RegisterCeefitTest(this, "time", &REALTIME::Time);
    RegisterCeefitTest(this, "pause", &REALTIME::Pause);
    RegisterCeefitTest(this, "await", &REALTIME::Await);
    RegisterCeefitTest(this, "fail", &REALTIME::Fail);
    RegisterCeefitTest(this, "enter", &REALTIME::Enter);
    RegisterCeefitTest(this, "press", &REALTIME::Press);
  }

  ceefit_init_spec REALTIME::~REALTIME()
  {
  }

  void ceefit_call_spec REALTIME::System(const STRING& prefix, PTR<PARSE>& cell)
  {
    STRING method(this->Camel(prefix + " " + cell->Text()));

    PTR<FIXTURE> systemFixture;
    PTR<CELLADAPTER> aMethod(system->FindMethod(systemFixture, method));

    try
    {
      if(aMethod != null)
      {
        PTR<CELLADAPTER> result;

        aMethod->Invoke(result, systemFixture);
      }
      else
      {
        throw new CLASSNOTFOUNDEXCEPTION(STRING("Method not found:  ") + method);
      }
    }
    catch(EXCEPTION* e)
    {
      Exception(cell, e);
    }
  }

  static ::CEEFIT::REGISTERFIXTURECLASS< REALTIME > ColumnFixtureRegistration("EG_MUSIC::REALTIME", "eg.music.Realtime");
};