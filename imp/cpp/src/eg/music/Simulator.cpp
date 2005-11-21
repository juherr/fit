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

declare_fit_module(MusicSimulator);

using namespace CEEFIT;

namespace EG_MUSIC
{
  PTR<SIMULATOR> SIMULATOR::System(new SIMULATOR());
  fitINT64 SIMULATOR::Time = CurrentTimeMillis();

  fitINT64 SIMULATOR::NextSearchComplete = 0;
  fitINT64 SIMULATOR::NextPlayStarted = 0;
  fitINT64 SIMULATOR::NextPlayComplete = 0;

  fitINT64 SIMULATOR::Sooner(fitINT64 soon, fitINT64 event)
  {
    return((event > Time && event < soon) ? event : soon);
  }

  fitINT64 SIMULATOR::NextEvent(const fitINT64& bound)
  {
    fitINT64 result = bound;
    result = Sooner(result, NextSearchComplete);
    result = Sooner(result, NextPlayStarted);
    result = Sooner(result, NextPlayComplete);
    return result;
  }

  void SIMULATOR::Perform()
  {
    if(Time == NextSearchComplete)
    {
      MUSICLIBRARY::SearchComplete();
    }
    if(Time == NextPlayStarted)
    {
      MUSICPLAYER::PlayStarted();
    }
    if(Time == NextPlayComplete)
    {
      MUSICPLAYER::PlayComplete();
    }
  }

  void SIMULATOR::Advance(const fitINT64& future)
  {
    while(Time < future)
    {
      Time = NextEvent(future);
      Perform();
    }
  }

  fitINT64 SIMULATOR::Schedule(const double& seconds)
  {
    return(Time + (fitINT64) (1000.0 * seconds));
  }

  void SIMULATOR::Delay(const double& seconds)
  {
    Advance(Schedule(seconds));
  }

  void SIMULATOR::waitSearchComplete()
  {
    Advance(NextSearchComplete);
  }

  void SIMULATOR::waitPlayStarted()
  {
    Advance(NextPlayStarted);
  }

  void SIMULATOR::waitPlayComplete()
  {
    Advance(NextPlayComplete);
  }

  void SIMULATOR::failLoadJam()
  {
    ACTIONFIXTURE::Actor = new DIALOG(STRING("load jamed"), ACTIONFIXTURE::Actor);
  }

  ceefit_init_spec SIMULATOR::SIMULATOR()
  {
    RegisterCeefitTest(this, "waitSearchComplete", &SIMULATOR::waitSearchComplete);
    RegisterCeefitTest(this, "waitPlayStarted", &SIMULATOR::waitPlayStarted);
    RegisterCeefitTest(this, "waitPlayComplete", &SIMULATOR::waitPlayComplete);
    RegisterCeefitTest(this, "failLoadJam", &SIMULATOR::failLoadJam);
  }

  ceefit_dtor_spec SIMULATOR::~SIMULATOR()
  {
  }

  void SIMULATOR::ReleaseStatics()
  {
    SIMULATOR::System = null;
  }

  static ::CEEFIT::REGISTERFIXTURECLASS< SIMULATOR > ColumnFixtureRegistration("EG_MUSIC::SIMULATOR", "eg.music.Simulator");
};