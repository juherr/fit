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

#ifndef __EG_MUSIC_MUSICLIBRARY_H__
#define __EG_MUSIC_MUSICLIBRARY_H__

namespace EG_MUSIC
{
  /**
   * <p>CeeFIT implementation note:  this class is implemented as a FIXTURE so that it's statics can be cleaned up 
   * automatically at the end of the CEEFIT::Run() call.</p>
   */
  class MUSICLIBRARY : public CEEFIT::FIXTURE 
  {
    public:
      static CEEFIT::PTR<MUSIC> Looking;
      static CEEFIT::DYNARRAY< CEEFIT::PTR<MUSIC> > Library;

      static void ceefit_call_spec Load(const CEEFIT::STRING& name); 
      static void ceefit_call_spec Select(CEEFIT::PTR<MUSIC>& m); 
      static void ceefit_call_spec Search(double seconds);
      static void ceefit_call_spec SearchComplete(void);
      static void ceefit_call_spec FindAll(void);
      static void ceefit_call_spec FindArtist(const CEEFIT::STRING& a); 
      static void ceefit_call_spec FindAlbum(const CEEFIT::STRING& a);
      static void ceefit_call_spec FindGenre(const CEEFIT::STRING& a);
      static void ceefit_call_spec FindYear(int a);
      static int ceefit_call_spec DisplayCount(void);
      static void ceefit_call_spec DisplayContents(CEEFIT::DYNARRAY< CEEFIT::PTR<CEEFIT::CELLADAPTER> >& out);
    
    private:
      friend class CEEFIT::REGISTERFIXTURECLASS<MUSICLIBRARY>;    // Only REGISTERFIXTURECLASS may instantiate me for the purposes
                                                                  // of calling ReleaseStatics
      ceefit_init_spec MUSICLIBRARY(void);

    public:
      ceefit_dtor_spec ~MUSICLIBRARY(void);

      void ceefit_call_spec ReleaseStatics(void);

    private:
      ceefit_init_spec MUSICLIBRARY(const MUSICLIBRARY&);
      MUSICLIBRARY& ceefit_call_spec operator=(const MUSICLIBRARY&);
  };
};

#endif // __EG_MUSIC_MUSICLIBRARY_H__
