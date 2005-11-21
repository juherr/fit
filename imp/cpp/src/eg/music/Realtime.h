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

#ifndef __EG_MUSIC_REALTIME_H__
#define __EG_MUSIC_REALTIME_H__

namespace EG_MUSIC
{
  class REALTIME : public CEEFIT::TIMEDACTIONFIXTURE
  {
    protected:
      CEEFIT::PTR<SIMULATOR> system;

    private:
      void ceefit_call_spec System(const CEEFIT::STRING& prefix, CEEFIT::PTR<CEEFIT::PARSE>& cell);

    public:
      virtual CEEFIT::fitINT64 ceefit_call_spec Time(void);
      virtual void ceefit_call_spec Pause(void);
      virtual void ceefit_call_spec Await(void);
      virtual void ceefit_call_spec Fail(void);
      virtual void ceefit_call_spec Enter(void);
      virtual void ceefit_call_spec Press(void);

    public:
      ceefit_init_spec REALTIME(void);
      virtual ceefit_dtor_spec ~REALTIME(void);

    private:
      ceefit_init_spec REALTIME(const REALTIME&);
      REALTIME& ceefit_call_spec operator=(const REALTIME&);
  };
};

#endif // __EG_MUSIC_REALTIME_H__