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

declare_fit_module(MusicDialog);

using namespace CEEFIT;

namespace EG_MUSIC
{
  ceefit_init_spec DIALOG::DIALOG(const STRING& aMessage, PTR<FIXTURE>& aCaller) 
  {
    Message = aMessage;
    Caller = aCaller;

    this->Initialize();
  }

  ceefit_init_spec DIALOG::DIALOG() 
  {
    this->Initialize();
  }

  ceefit_init_spec DIALOG::~DIALOG() 
  {
  }

  STRING ceefit_call_spec DIALOG::message() 
  {
    return Message;
  }

  void ceefit_call_spec DIALOG::ok() 
  {
    if(Message.IsEqual("load jamed"))   
    {
      MUSICPLAYER::Stop();
    }
    ACTIONFIXTURE::Actor = Caller;
  }

  void ceefit_call_spec DIALOG::Initialize()
  {
    RegisterCeefitTest(this, "ok", &DIALOG::ok);
    RegisterCeefitTest(this, "message", &DIALOG::message);        
  }

  static ::CEEFIT::REGISTERFIXTURECLASS< DIALOG > DialogFixtureRegistration("EG_MUSIC::DIALOG", "eg.music.Dialog");
};