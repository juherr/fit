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

declare_fit_module(MusicClass);

using namespace CEEFIT;

namespace EG_MUSIC
{
  const char* MUSIC::Status = "ready";

  ceefit_init_spec MUSIC::MUSIC()
  {
    Size = 0L;
    Seconds = 0;
    TrackNumber = 0;
    TrackCount = 0;
    Year = 0;
    Selected = false;

    RegisterCeefitField(this, "title", Title);
    RegisterCeefitField(this, "artist", Artist);
    RegisterCeefitField(this, "album", Album);
    RegisterCeefitField(this, "genre", Genre);
    RegisterCeefitField(this, "size", Size);
    RegisterCeefitField(this, "seconds", Seconds);
    RegisterCeefitField(this, "trackNumber", TrackNumber);
    RegisterCeefitField(this, "trackCount", TrackCount);
    RegisterCeefitField(this, "year", Year);
    RegisterCeefitField(this, "date", Date);
    RegisterCeefitField(this, "selected", Selected);
    
    RegisterCeefitTest(this, "track", &MUSIC::Track);
    RegisterCeefitTest(this, "time", &MUSIC::Time);
  }

  ceefit_dtor_spec MUSIC::~MUSIC()
  {
  }

  MUSIC& ceefit_call_spec MUSIC::operator=(const MUSIC& aMusic)
  {
    Title = aMusic.Title;
    Artist = aMusic.Artist;
    Album = aMusic.Album;
    Genre = aMusic.Genre;
    Size = aMusic.Size;
    Seconds = aMusic.Seconds;
    TrackNumber = aMusic.TrackNumber;
    TrackCount = aMusic.TrackCount;
    Year = aMusic.Year;
    Date = aMusic.Date;
    Selected = aMusic.Selected;

    return(*this);
  }

  STRING ceefit_call_spec MUSIC::Track()
  {
    return(STRING() + TrackNumber + " of " + TrackCount);
  }

  double ceefit_call_spec MUSIC::Time()
  {
    double result = (Seconds / 0.6);
    if(Seconds >= 0)
    {
      return(floor(result + 0.5) / 100.0);
    }
    else
    {
      return(ceil(result + 0.5) / 100.0);
    }
  }

  int ceefit_call_spec MUSIC::GetHashCode(void) const
  {
    return(Title.GetHashCode());
  }

  bool ceefit_call_spec MUSIC::IsEqual(const MUSIC& aMusic) const
  {
    return(Title.IsEqual(aMusic.Title) &&
           Artist.IsEqual(aMusic.Artist) &&
           Album.IsEqual(aMusic.Album) &&
           Genre.IsEqual(aMusic.Genre) &&
           Size == aMusic.Size &&
           Seconds == aMusic.Seconds &&
           TrackNumber == aMusic.TrackNumber &&
           TrackCount == aMusic.TrackCount &&
           Year == aMusic.Year &&
           Date.IsEqual(aMusic.Date) &&
           Selected == aMusic.Selected);
  }

  STRING ceefit_call_spec MUSIC::ToString() const
  {
    if(Title.Length() == 0)
    {
      return Title;
    }
    else
    {
      return this->OBJECT::ToString();
    }
  }


  // Factory //////////////////////////////////

  VALUE<MUSIC> ceefit_call_spec MUSIC::Parse(const STRING& string)
  {
    MUSIC* m = new MUSIC();

    DYNARRAY<STRING> parsedTokens;
    Tokenize(parsedTokens, string, STRING("\t"));
    int i = 0;
    m->Title =       parsedTokens[i++];
    m->Artist =      parsedTokens[i++];
    m->Album =       parsedTokens[i++];
    m->Genre =       parsedTokens[i++];
    if(!swscanf(parsedTokens[i++].GetBuffer(), L"%li", &m->Size) ||
       !swscanf(parsedTokens[i++].GetBuffer(), L"%i", &m->Seconds) ||
       !swscanf(parsedTokens[i++].GetBuffer(), L"%i", &m->TrackNumber) ||
       !swscanf(parsedTokens[i++].GetBuffer(), L"%i", &m->TrackCount) ||
       !swscanf(parsedTokens[i++].GetBuffer(), L"%i", &m->Year))
    {
      throw new PARSEEXCEPTION("Parse exception in MUSIC::Parse");
    }
    m->Date.Parse(parsedTokens[i++]);

    return VALUE<MUSIC>(m);
  }

  STRING ceefit_call_spec MUSIC::Dump() const
  {
    STRING out;
    STRING numberParts;
    STRING dateString(Date.ToString());

    SafeSprintf(numberParts, L"%li\t%i\t%i\t%i\t%i", Size, Seconds, TrackNumber, TrackCount, Year);
    out += Title + "\t" + Artist + "\t" + Album + "\t" + Genre + "\t" + numberParts + "\t" + dateString;

    return(out);
  }

  static ::CEEFIT::REGISTERFIXTURECLASS< MUSIC > MusicRegistration("EG_MUSIC::MUSIC", "eg.music.Music");
}
