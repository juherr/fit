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

#ifndef __EG_MUSIC_BROWSER_H__
#define __EG_MUSIC_BROWSER_H__

namespace EG_MUSIC
{
  class BROWSER : public CEEFIT::FIXTURE 
  {
    public:
      // Library //////////////////////////////////

      virtual void ceefit_call_spec library(CEEFIT::STRING path);
      virtual int ceefit_call_spec totalSongs(void);

      // Select Detail ////////////////////////////

      virtual CEEFIT::STRING ceefit_call_spec playing(void);
      virtual void ceefit_call_spec select(int i);
      virtual CEEFIT::STRING ceefit_call_spec title(void);
      virtual CEEFIT::STRING ceefit_call_spec artist(void);
      virtual CEEFIT::STRING ceefit_call_spec album(void);
      virtual int ceefit_call_spec year(void);
      virtual MUSIC_DOUBLE ceefit_call_spec time(void);
      virtual CEEFIT::STRING ceefit_call_spec track(void);

      // Search Buttons ///////////////////////////

      virtual void ceefit_call_spec sameAlbum(void);
      virtual void ceefit_call_spec sameArtist(void);
      virtual void ceefit_call_spec sameGenre(void);
      virtual void ceefit_call_spec sameYear(void);
      virtual int ceefit_call_spec selectedSongs(void);
      virtual void ceefit_call_spec showAll(void);

      // Play Buttons /////////////////////////////

      virtual void ceefit_call_spec play(void);
      virtual void ceefit_call_spec pause(void);
      virtual CEEFIT::STRING ceefit_call_spec status(void);
      virtual double ceefit_call_spec remaining(void);
    
    public:
      ceefit_init_spec BROWSER(void);
      virtual ceefit_dtor_spec ~BROWSER(void);

    private:
      ceefit_init_spec BROWSER(BROWSER&);             /**< Not implemented, do not call. */
      BROWSER& ceefit_call_spec operator=(BROWSER&);  /**< Not implemented, do not call. */

  };
};

#endif // __EG_MUSIC_BROWSER_H__
