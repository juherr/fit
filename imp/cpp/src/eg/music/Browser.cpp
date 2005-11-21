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

declare_fit_module(BrowserFixture);

using namespace CEEFIT;

namespace EG_MUSIC
{
  void ceefit_call_spec BROWSER::library(STRING path) 
  {
    MUSICLIBRARY::Load(path);
  }

  int ceefit_call_spec BROWSER::totalSongs() 
  {
    return MUSICLIBRARY::Library.GetSize();
  }

  // Select Detail ////////////////////////////

  STRING ceefit_call_spec BROWSER::playing() 
  {
    return MUSICPLAYER::Playing->Title;
  }

  void ceefit_call_spec BROWSER::select(int i) 
  {
    MUSICLIBRARY::Select(MUSICLIBRARY::Library[i-1]);
  }

  STRING ceefit_call_spec BROWSER::title() 
  {
    return MUSICLIBRARY::Looking->Title;
  }

  STRING ceefit_call_spec BROWSER::artist() 
  {
    return MUSICLIBRARY::Looking->Artist;
  }

  STRING ceefit_call_spec BROWSER::album() 
  {
    return MUSICLIBRARY::Looking->Album;
  }

  int ceefit_call_spec BROWSER::year() 
  {
    return MUSICLIBRARY::Looking->Year;
  }

  double ceefit_call_spec BROWSER::time() 
  {
    return MUSICLIBRARY::Looking->Time();
  }

  STRING ceefit_call_spec BROWSER::track() 
  {
    return MUSICLIBRARY::Looking->Track();
  }

  // Search Buttons ///////////////////////////

  void ceefit_call_spec BROWSER::sameAlbum() 
  {
    MUSICLIBRARY::FindAlbum(MUSICLIBRARY::Looking->Album);
  }

  void ceefit_call_spec BROWSER::sameArtist() 
  {
    MUSICLIBRARY::FindArtist(MUSICLIBRARY::Looking->Artist);
  }

  void ceefit_call_spec BROWSER::sameGenre() 
  {
    MUSICLIBRARY::FindGenre(MUSICLIBRARY::Looking->Genre);
  }

  void ceefit_call_spec BROWSER::sameYear() 
  {
    MUSICLIBRARY::FindYear(MUSICLIBRARY::Looking->Year);
  }

  int ceefit_call_spec BROWSER::selectedSongs() 
  {
    return MUSICLIBRARY::DisplayCount();
  }

  void ceefit_call_spec BROWSER::showAll() 
  {
    MUSICLIBRARY::FindAll();
  }

  // Play Buttons /////////////////////////////

  void ceefit_call_spec BROWSER::play() 
  {
    MUSICPLAYER::Play(MUSICLIBRARY::Looking);
  }

  void ceefit_call_spec BROWSER::pause() 
  {
    MUSICPLAYER::Pause();
  }

  STRING ceefit_call_spec BROWSER::status() 
  {
    return STRING(MUSIC::Status);
  }

  double ceefit_call_spec BROWSER::remaining() 
  {
    return MUSICPLAYER::MinutesRemaining();
  }

  ceefit_init_spec BROWSER::BROWSER()
  {
    RegisterCeefitTest(this, "library", &BROWSER::library);
    RegisterCeefitTest(this, "totalSongs", &BROWSER::totalSongs);
    RegisterCeefitTest(this, "playing", &BROWSER::playing);
    RegisterCeefitTest(this, "select", &BROWSER::select);
    RegisterCeefitTest(this, "title", &BROWSER::title);
    RegisterCeefitTest(this, "artist", &BROWSER::artist);
    RegisterCeefitTest(this, "album", &BROWSER::album);
    RegisterCeefitTest(this, "year", &BROWSER::year);
    RegisterCeefitTest(this, "time", &BROWSER::time);
    RegisterCeefitTest(this, "track", &BROWSER::track);
    RegisterCeefitTest(this, "sameAlbum", &BROWSER::sameAlbum);
    RegisterCeefitTest(this, "sameArtist", &BROWSER::sameArtist);
    RegisterCeefitTest(this, "sameGenre", &BROWSER::sameGenre);
    RegisterCeefitTest(this, "sameYear", &BROWSER::sameYear);
    RegisterCeefitTest(this, "selectedSongs", &BROWSER::selectedSongs);
    RegisterCeefitTest(this, "showAll", &BROWSER::showAll);
    RegisterCeefitTest(this, "play", &BROWSER::play);
    RegisterCeefitTest(this, "pause", &BROWSER::pause);
    RegisterCeefitTest(this, "status", &BROWSER::status);
    RegisterCeefitTest(this, "remaining", &BROWSER::remaining);
  }

  ceefit_dtor_spec BROWSER::~BROWSER()
  {
  }

  static ::CEEFIT::REGISTERFIXTURECLASS< BROWSER > BrowserFixtureRegistration("EG_MUSIC::BROWSER", "eg.music.Browser");
};
