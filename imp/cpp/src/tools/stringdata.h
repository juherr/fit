#ifndef __TOOLS_STRINGDATA_H__
#define __TOOLS_STRINGDATA_H__

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

namespace CEEFIT
{

  /**
   * <p>Just a container for a DYNARRAY&lt;wchar_t&gt;</p>
   */
  class STRINGDATA : public OBJECT
  {
    public:
      DYNARRAY<wchar_t> Array;
      bool AssignedFlag;

    public:
      STRINGDATA(void)
      {        
        AssignedFlag = false;
      }

      inline ceefit_dtor_spec ~STRINGDATA(void)
      {
      }

      inline STRINGDATA(const STRINGDATA& aData)
      {        
        Array = aData.Array;
        AssignedFlag = aData.AssignedFlag;
      }

      inline STRINGDATA& operator=(const STRINGDATA& aData)
      {
        Array = aData.Array;
        AssignedFlag = aData.AssignedFlag;

        return(*this);
      }
  };

};

#endif // __TOOLS_STRINGDATA_H__
