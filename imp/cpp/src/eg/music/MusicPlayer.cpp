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

declare_fit_module(MusicPlayer);

using namespace CEEFIT;

namespace EG_MUSIC
{
  PTR<MUSIC> MUSICPLAYER::Playing;
  double MUSICPLAYER::Paused = 0.0;

  // Controls /////////////////////////////////

  void ceefit_call_spec MUSICPLAYER::Play(PTR<MUSIC>& m) 
  {
    if(Paused == 0) 
    {
      MUSIC::Status = "loading";
      double seconds = ((m->ToString().IsEqual(Playing != null ? Playing->ToString() : STRING("null"))) ? 0.3 : 2.5);
      SIMULATOR::NextPlayStarted = SIMULATOR::Schedule(seconds);
    } 
    else 
    {
      MUSIC::Status = "playing";
      SIMULATOR::NextPlayComplete = SIMULATOR::Schedule(Paused);
      Paused = 0;
    }
  }

  void ceefit_call_spec MUSICPLAYER::Pause() 
  {
    MUSIC::Status = "pause";
    if(Playing != null && Paused == 0) 
    {
      Paused = (SIMULATOR::NextPlayComplete - SIMULATOR::Time) / 1000.0;
      SIMULATOR::NextPlayComplete = 0;
    }
  }

  void ceefit_call_spec MUSICPLAYER::Stop() 
  {
    SIMULATOR::NextPlayStarted = 0;
    SIMULATOR::NextPlayComplete = 0;
    PlayComplete();
  }

  // Status ///////////////////////////////////

  double ceefit_call_spec MUSICPLAYER::SecondsRemaining() 
  {
    if(Paused != 0) 
    {
      return Paused;
    } 
    else if(Playing != null) 
    {
      return(SIMULATOR::NextPlayComplete - SIMULATOR::Time) / 1000.0;
    } 
    else {
      return 0;
    }
  }

  double ceefit_call_spec MUSICPLAYER::MinutesRemaining() {
    double secondsRemaining = SecondsRemaining();
    double result = secondsRemaining / .6;

    if(secondsRemaining >= 0)
    {
      return(floor(result + 0.5) / 100.0);
    }
    else 
    {
      return(ceil(result + 0.5) / 100.0);
    }
  }

  // Events ///////////////////////////////////

  void ceefit_call_spec MUSICPLAYER::PlayStarted() 
  {
    MUSIC::Status = "playing";
    Playing = MUSICLIBRARY::Looking;
    SIMULATOR::NextPlayComplete = SIMULATOR::Schedule(Playing->Seconds);
  }

  void ceefit_call_spec MUSICPLAYER::PlayComplete() 
  {
    MUSIC::Status = "ready";
    Playing = null;
  }

  ceefit_init_spec MUSICPLAYER::MUSICPLAYER() 
  {
  }

  ceefit_dtor_spec MUSICPLAYER::~MUSICPLAYER() 
  {
  }

  void ceefit_call_spec MUSICPLAYER::ReleaseStatics()
  {
    Playing = null;
    Paused = false;
  }

  static ::CEEFIT::REGISTERFIXTURECLASS< MUSICPLAYER > MusicPlayerRegistration("EG_MUSIC::MUSICPLAYER", "eg.music.MusicPlayer");
};