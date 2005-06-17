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

#ifndef __EG_MUSIC_MUSICPLAYER_H__
#define __EG_MUSIC_MUSICPLAYER_H__

namespace EG_MUSIC
{
  class MUSICPLAYER : public CEEFIT::FIXTURE
  {
    public:
      static CEEFIT::PTR<MUSIC> Playing;
      static double Paused;

      // Controls /////////////////////////////////

      static void ceefit_call_spec Play(CEEFIT::PTR<MUSIC>& m); 
      static void ceefit_call_spec Pause(void); 
      static void ceefit_call_spec Stop(void); 

      // Status ///////////////////////////////////

      static double ceefit_call_spec SecondsRemaining(void);
      static double ceefit_call_spec MinutesRemaining(void);

      // Events ///////////////////////////////////

      static void ceefit_call_spec PlayStarted(void);
      static void ceefit_call_spec PlayComplete(void);

    private:
      friend class CEEFIT::REGISTERFIXTURECLASS<MUSICPLAYER>;     // Only REGISTERFIXTURECLASS may instantiate me for the purposes
                                                                  // of calling ReleaseStatics
      ceefit_init_spec MUSICPLAYER(void);                               

    public:
      ceefit_init_spec ~MUSICPLAYER(void);                              

      void ceefit_call_spec ReleaseStatics(void);

    private:
      ceefit_init_spec MUSICPLAYER(const MUSICPLAYER&);                 /**< Not implemented, do not call */
      MUSICPLAYER& ceefit_call_spec operator=(const MUSICPLAYER&);      /**< Not implemented, do not call */
  };
};

#endif // __EG_MUSIC_MUSICPLAYER_H__