#ifndef __FAT_TABLE_H__
#define __FAT_TABLE_H__

#include "ceefit/mandatory.h"

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

namespace CEEFAT
{

  class FAT_TABLE : public CEEFIT::PRIMITIVEFIXTURE
  {
    public:
      static CEEFIT::PTR<CEEFIT::PARSE> Table;

      inline ceefit_init_spec FAT_TABLE(void) {}
      inline virtual ceefit_init_spec ~FAT_TABLE(void) {}

      virtual void ceefit_call_spec DoRows(CEEFIT::PTR<CEEFIT::PARSE>& rows);
      static CEEFIT::VALUE<CEEFIT::PARSE> ceefit_call_spec Copy(CEEFIT::PTR<CEEFIT::PARSE>& tree);
  };

};

#endif // __FAT_TABLE_H__
