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

#ifndef __EG_MUSIC_SIMULATOR_H__
#define __EG_MUSIC_SIMULATOR_H__

namespace EG_MUSIC
{
  /**
   * <p>CeeFIT implementation note:  In CeeFIT this is registered as a FIXTURE because methods need to be
   * registered and bound by REALTIME.  It is not designed to be used as a FIXTURE by default.</p>
   */
  class SIMULATOR : public CEEFIT::FIXTURE
  {
    // This discrete event simulator supports three events
    // each of which is open coded in the body of the simulator.

  public:
      static CEEFIT::PTR<SIMULATOR> System;
      static CEEFIT::fitINT64 Time;

    public:
      static CEEFIT::fitINT64 NextSearchComplete;
      static CEEFIT::fitINT64 NextPlayStarted;
      static CEEFIT::fitINT64 NextPlayComplete;

    public:
      virtual CEEFIT::fitINT64 ceefit_call_spec Sooner(CEEFIT::fitINT64 soon, CEEFIT::fitINT64 event);
      virtual CEEFIT::fitINT64 ceefit_call_spec NextEvent(const CEEFIT::fitINT64& bound);
      virtual void ceefit_call_spec Perform(void);
      virtual void ceefit_call_spec Advance(const CEEFIT::fitINT64& future);
      static CEEFIT::fitINT64 ceefit_call_spec Schedule(const double& seconds);
      virtual void ceefit_call_spec Delay(const double& seconds);

    public:
      virtual void ceefit_call_spec waitSearchComplete(void);
      virtual void ceefit_call_spec waitPlayStarted(void);
      virtual void ceefit_call_spec waitPlayComplete(void);
      virtual void ceefit_call_spec failLoadJam(void);

      ceefit_init_spec SIMULATOR(void);
      virtual ceefit_dtor_spec ~SIMULATOR(void);

      void ceefit_call_spec ReleaseStatics(void);

    private:
      ceefit_init_spec SIMULATOR(const SIMULATOR&);
      SIMULATOR& ceefit_call_spec operator=(const SIMULATOR&);

  };
};

#endif // __EG_MUSIC_SIMULATOR_H__
