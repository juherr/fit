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

declare_fit_module(MusicLibrary);

using namespace CEEFIT;

namespace EG_MUSIC
{
  PTR<MUSIC> MUSICLIBRARY::Looking;
  DYNARRAY< PTR<MUSIC> > MUSICLIBRARY::Library;

  void ceefit_call_spec MUSICLIBRARY::Load(const STRING& name) 
  {
    BUFFEREDFILEREADER reader(name);
    Library.Reset();

    reader.ReadLine(); // skip column headings
    while(!reader.IsEof()) 
    {
      Library.Add(MUSIC::Parse(reader.ReadLine()));
    }
    reader.Close();
  }

  void ceefit_call_spec MUSICLIBRARY::Select(PTR<MUSIC>& m) 
  {
    Looking = m;
  }

  void ceefit_call_spec MUSICLIBRARY::Search(double seconds)
  {
    MUSIC::Status = "searching";
    SIMULATOR::NextSearchComplete = SIMULATOR::Schedule(seconds);
  }

  void ceefit_call_spec MUSICLIBRARY::SearchComplete() 
  {
    MUSIC::Status = (MUSICPLAYER::Playing == null) ? "ready" : "playing";
  }

  void ceefit_call_spec MUSICLIBRARY::FindAll() 
  {
    Search(3.2);
    for (int i=0; i < Library.GetSize(); i++) 
    {
      Library[i]->Selected = true;
    }
  }

  void ceefit_call_spec MUSICLIBRARY::FindArtist(const STRING& a) 
  {
    Search(2.3);
    for(int i=0; i < Library.GetSize(); i++) 
    {
      Library[i]->Selected = Library[i]->Artist.IsEqual(a);
    }
  }

  void ceefit_call_spec MUSICLIBRARY::FindAlbum(const STRING& a) 
  {
    Search(1.1);
    for(int i=0; i < Library.GetSize(); i++) 
    {
      Library[i]->Selected = Library[i]->Album.IsEqual(a);
    }
  }

  void ceefit_call_spec MUSICLIBRARY::FindGenre(const STRING& a) 
  {
    Search(0.2);
    for (int i=0; i < Library.GetSize(); i++) 
    {
      Library[i]->Selected = Library[i]->Genre.IsEqual(a);
    }
  }

  void ceefit_call_spec MUSICLIBRARY::FindYear(int a) 
  {
    Search(0.8);
    for (int i=0; i < Library.GetSize(); i++) 
    {
      Library[i]->Selected = Library[i]->Year == a;
    }
  }

  int ceefit_call_spec MUSICLIBRARY::DisplayCount(void) 
  {
    int count = 0;
    for (int i=0; i < Library.GetSize(); i++) 
    {
      count += (Library[i]->Selected ? 1 : 0);
    }
    return count;
  }

  void ceefit_call_spec MUSICLIBRARY::DisplayContents(DYNARRAY< PTR<CELLADAPTER> >& out) 
  {
    out.Reset();        
    for (int i=0, j=0; i < Library.GetSize(); i++) 
    {
      if(Library[i]->Selected) 
      {
        PTR<CELLADAPTER> cellAdapter(new FITFIELD<MUSIC>());

        cellAdapter->WriteToFixtureVar(Library[i]->Dump());

        out.Add(cellAdapter);
      }
    }
  }

  ceefit_init_spec MUSICLIBRARY::MUSICLIBRARY(void)
  {
  }

  ceefit_dtor_spec MUSICLIBRARY::~MUSICLIBRARY(void)
  {
  }

  void ceefit_call_spec MUSICLIBRARY::ReleaseStatics(void)
  {
    Looking = null;
    Library.Reset();
  }

  static ::CEEFIT::REGISTERFIXTURECLASS< MUSICLIBRARY > MusicLibraryRegistration("EG_MUSIC::MUSICLIBRARY", "eg.music.MusicLibrary");
};