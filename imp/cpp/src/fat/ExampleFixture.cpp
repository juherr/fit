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

#include "ceefit.h"

declare_fit_module(FatExampleFixture);

using namespace CEEFIT;

namespace CEEFAT
{
  class FAT_EXAMPLEFIXTURE : public FIXTURE 
  {
    public:
      FAT_EXAMPLEFIXTURE(void) {}
      virtual ~FAT_EXAMPLEFIXTURE(void) {}

    private:
      FAT_EXAMPLEFIXTURE& operator=(const FAT_EXAMPLEFIXTURE&) { return(*this); }
      FAT_EXAMPLEFIXTURE(const FAT_EXAMPLEFIXTURE&) { }
  };

  static ::CEEFIT::REGISTERFIXTURECLASS< FAT_EXAMPLEFIXTURE > ExampleFixtureRegistration("FAT_EXAMPLEFIXTURE", "fat.ExampleFixture");
};

