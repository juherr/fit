#ifndef __TOOLS_VALUE_H__
#define __TOOLS_VALUE_H__

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

namespace CEEFIT
{
  template<class ANYTYPE> class PTR;

  /**
   * <p>Values that can be copied into managed pointers</p>
   */
  template<class T> class VALUE 
  {
    protected:
      T* Value;

    public:
      explicit inline VALUE<T>(T* aValue)
      {      
        Value = aValue;
      }

      template<class U> explicit inline VALUE<T>(VALUE<U>& aValueObj)
      {
        Value = aValueObj.Value;
      }
/*
      template<class U> explicit inline VALUE<T>(U* aValue)
      {      
        Value = aValue;
      }
*/
      template<class U> explicit inline VALUE<T>(PTR<U>& aPtr)   // set a VALUE from a PTR
      {     
        VALUE<T> aValue(aPtr.GetValue());
     
        (*this) = aValue;
      }

      template<class U> inline VALUE<T>& operator=(VALUE<U>& aValue)
      {
        Value = aValue.Value;

        return(*this);
      }

      inline ~VALUE(void) 
      {
      }

      template<class U> inline bool operator==(const VALUE<U>& aValue) const
      {
        return(Value == aValue.Value);
      }

      template<class U> inline bool operator!=(const VALUE<U>& aValue) const
      {
        return(Value != aValue.Value);
      } 

      inline bool operator==(const T* aValue) const
      {
        return(Value == aValue);
      }

      template<class U> inline bool operator==(const U* aValue) const
      {
        return(Value == aValue);
      }

      inline bool operator!=(const T* aValue) const
      {
        return(Value != aValue);
      }

      template<class U> inline bool operator!=(const U* aValue) const
      {
        return(Value != aValue);
      }

      /**
       * <p>Set a PTR with the pointer value stored in this REF</p>
       *
       * @param aPtrTarget A PTR<T>  Due to forward declaration restrictions on template types, it is impossible for us to
       * specify that aPtrTarget should be a PTR<T> ... Please do not call this method unless aPtrTarget is a PTR<T>
       * object.
       */
      template<class U> inline U& SetPtr(U& aPtrTarget) 
      {
        return(aPtrTarget = Value);
      }

      template<class U> inline void AtomicSetPtr(U& aPtrTarget) 
      {
        aPtrTarget.AtomicSet(Value);
      }

    private:
      VALUE(void);
  };

};

#endif // __TOOLS_VALUE_H__