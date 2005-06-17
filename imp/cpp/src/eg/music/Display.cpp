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

declare_fit_module(MusicDisplay);

using namespace CEEFIT;

namespace EG_MUSIC
{
  VALUE<FIXTURE> DISPLAY::GetTargetClass()
  {
    if(MusicFixture == null)
    {
      MusicFixture = new MUSIC();
    }
    return(VALUE<FIXTURE>(MusicFixture));
  }

  void DISPLAY::Query(DYNARRAY< PTR<CELLADAPTER> >& out)
  {
    MUSICLIBRARY::DisplayContents(out);
  }

  void DISPLAY::Parse(PTR<CELLADAPTER>& out, const STRING& s)
  {
    // Unlike the Java version, CeeFIT's CELLADAPTER's know how to Parse any type.

    this->ROWFIXTURE::Parse(out, s);
  }

  DISPLAY::DISPLAY(void)
  {
  }

  DISPLAY::~DISPLAY(void)
  {
  }

  static ::CEEFIT::REGISTERFIXTURECLASS< DISPLAY > DisplayFixtureRegistration("EG_MUSIC::DISPLAY", "eg.music.Display");
};
