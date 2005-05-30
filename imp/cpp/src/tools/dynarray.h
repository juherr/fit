#ifndef __TOOLS_DYNARRAY_H__
#define __TOOLS_DYNARRAY_H__

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

# define min_dynarray_size 16

  /**
   * <p>A dynamically sized ARRAY/LIST class that is able to grow and shrink.</p>
   */ 
  template<class ANYTYPE> class DYNARRAY : public virtual OBJECT
  {
    protected:
      int Limit;        /**< DYNARRAY potential element limit */
      int Count;        /**< DYNARRAY element count */
      ANYTYPE* List;    /**< DYNARRAY ANYTYPE list (ANYTYPE == generic type) */

    public:
      virtual inline ~DYNARRAY<ANYTYPE>(void)
      {
        Reset();
      }

      inline ceefit_init_spec DYNARRAY<ANYTYPE>(void) 
      {
        Limit = 0;
        Count = 0;
        List = NULL;
      }

      template<class ANYTYPE2> explicit inline ceefit_init_spec DYNARRAY<ANYTYPE>(DYNARRAY<ANYTYPE2>& otherArray)
      {
        Limit = 0;
        Count = 0;
        List = NULL;

        AddAll(otherArray);
      }

      template<class ANYTYPE2> explicit inline ceefit_init_spec DYNARRAY<ANYTYPE>(const DYNARRAY<ANYTYPE2>& otherArray)
      {
        Limit = 0;
        Count = 0;
        List = NULL;

        AddAll(otherArray);
      }

# ifdef __GNUC__
      explicit inline ceefit_init_spec DYNARRAY<ANYTYPE>(const DYNARRAY<ANYTYPE>& otherArray)
      {
        Limit = 0;
        Count = 0;
        List = NULL;

        AddAll(otherArray);
      }
# endif

      template<class ANYTYPE2> inline DYNARRAY<ANYTYPE>& ceefit_call_spec operator=(DYNARRAY<ANYTYPE2>& otherArray)
      {
        Reset();

        AddAll(otherArray);      

        return(*this);
      }

      template<class ANYTYPE2> inline DYNARRAY<ANYTYPE>& ceefit_call_spec operator=(const DYNARRAY<ANYTYPE2>& otherArray)
      {
        Reset();

        AddAll(otherArray);      

        return(*this);
      }

//#ifdef __GNUC__
      inline DYNARRAY<ANYTYPE>& ceefit_call_spec operator=(const DYNARRAY<ANYTYPE>& otherArray)
      {
        Reset();

        AddAll(otherArray);      

        return(*this);
      }
//#endif

    public:
      static inline DYNARRAY<ANYTYPE>& ceefit_call_spec CreateArray(int size)
      {
        DYNARRAY<ANYTYPE>* dynArray = new DYNARRAY<ANYTYPE>();

        dynArray->Reserve(size);

        return(*dynArray);
      }

      virtual inline ANYTYPE& ceefit_call_spec Get(int aIndex)
      {
        return(operator[](aIndex));
      }

      virtual inline const ANYTYPE& ceefit_call_spec Get(int aIndex) const
      {
        return(operator[](aIndex));
      }

      virtual inline int ceefit_call_spec GetSize(void) const 
      {
        return(Count);
      }

      virtual inline void ceefit_call_spec Add(ANYTYPE& aItem)
      {
        if(Limit == Count) 
        {        
          ExtendLimit(GetExtension(1));
        }
        List[Count++] = aItem;
      }

//#ifdef __GNUC__
//      template<class ANYTYPE2> inline void ceefit_call_spec Add(ANYTYPE2& aItem)
//      {
//        if(Limit == Count) 
//        {        
//          ExtendLimit(GetExtension(1));
//        }
//        List[Count++] = aItem;
//      }
//#endif

      template<class ANYTYPE2> inline void ceefit_call_spec Add(const ANYTYPE2& aItem)
      {
        if(Limit == Count) 
        {        
          ExtendLimit(GetExtension(1));
        }

        ANYTYPE& aDest = List[Count++];
        aDest = aItem;
      }

      template<class ANYTYPE2> inline void ceefit_call_spec AddAll(const DYNARRAY<ANYTYPE2>& aCollection)
      {
        int aSize = aCollection.GetSize();

        if(aSize == 0)
        {
          return;
        }

        if(Limit <= (Count + aSize)) 
        {
          ExtendLimit(GetExtension(Count + aSize - Limit));
        }

        int i = -1;
        while(++i < aCollection.GetSize())
        {
          List[Count++] = aCollection[i];
        }
      }

      template<class ANYTYPE2> inline void ceefit_call_spec AddAll(DYNARRAY<ANYTYPE2>& aCollection)
      {
        int aSize = aCollection.GetSize();

        if(aSize == 0)
        {
          return;
        }

        if(Limit <= (Count + aSize)) 
        {
          ExtendLimit(GetExtension(Count + aSize - Limit));
        }

        int i = -1;
        while(++i < aCollection.GetSize())
        {
          List[Count++] = aCollection[i];
        }
      }

      virtual inline void ceefit_call_spec Set(int aIndex, ANYTYPE& aValue)
      {
        // Validate parameters
        if(aIndex < 0 || aIndex > Count)
        {
          throw new BOUNDSEXCEPTION(L"Index out of bounds", aIndex);
        }

        if(aIndex < Count) 
        {
          // setting to the interior of the LIST
          List[aIndex] = aValue;
        }
        else 
        {
          // setting to the end of the LIST
          Add(aValue);
        }
      }

      template<class ANYTYPE2> inline void ceefit_call_spec Set(int aIndex, ANYTYPE2& aValue)
      {
        // Validate parameters
        if(aIndex < 0 || aIndex > Count)
        {
          throw new BOUNDSEXCEPTION(L"Index out of bounds", aIndex);
        }

        if(aIndex < Count) 
        {
          ANYTYPE& aDest = List[aIndex];
        
          // setting to the interior of the LIST
          aDest = aValue;
        }
        else 
        {
          // setting to the end of the LIST
          Add(aValue);
        }
      }

      template<class ANYTYPE2> inline void ceefit_call_spec Set(int aIndex, const ANYTYPE2& aValue)
      {
        // Validate parameters
        if(aIndex < 0 || aIndex > Count)
        {
          throw new BOUNDSEXCEPTION(L"Index out of bounds", aIndex);
        }

        if(aIndex < Count) 
        {
          ANYTYPE& aDest = List[aIndex];

          // setting to the interior of the LIST
          aDest = aValue;
        }
        else 
        {
          // setting to the end of the LIST
          Add(aValue);
        }
      }

      virtual inline void ceefit_call_spec Insert(int aIndex, ANYTYPE& aValue)
      {
        // Validate parameters
        if(aIndex < 0 || aIndex > Count)
        {
          throw new BOUNDSEXCEPTION(L"Index out of bounds", aIndex);
        }
      
        if(aIndex == Count)
        {
          Add(aValue);
        }
        else 
        {
          if(Count == Limit)
          {
            ExtendLimit(GetExtension(1));
          }

          int i = Count;
          while(i > aIndex)
          {
            List[i] = List[i-1];
            i--;
          }

          List[aIndex] = aValue;
          Count++;
        }
      }

      template<class ANYTYPE2> inline void ceefit_call_spec Insert(int aIndex, ANYTYPE2& aValue)
      {
        // Validate parameters
        if(aIndex < 0 || aIndex > Count)
        {
          throw new BOUNDSEXCEPTION(L"Index out of bounds", aIndex);
        }
      
        if(aIndex == Count)
        {
          Add(aValue);
        }
        else 
        {
          if(Count == Limit)
          {
            ExtendLimit(GetExtension(1));
          }

          int i = Count;
          while(i > aIndex)
          {
            List[i] = List[i-1];
            i--;
          }

          List[aIndex] = aValue;
          Count++;
        }
      }

      template<class ANYTYPE2> inline void ceefit_call_spec Insert(int aIndex, const ANYTYPE2& aValue)
      {
        // Validate parameters
        if(aIndex < 0 || aIndex > Count)
        {
          throw new BOUNDSEXCEPTION(L"Index out of bounds", aIndex);
        }
      
        if(aIndex == Count)
        {
          Add(aValue);
        }
        else 
        {
          if(Count == Limit)
          {
            ExtendLimit(GetExtension(1));
          }

          int i = Count;
          while(i > aIndex)
          {
            List[i] = List[i-1];
            i--;
          }

          List[aIndex] = aValue;
          Count++;
        }
      }

      /**
       * <p>Removes values from the DYNARRAY</p>
       *
       * <p>Warning:  The way this method is implemented makes it compatible with built-in types and C++
       * objects, but is has a drawback in that items are not necessarily removed, just merely made 
       * inaccessable to the caller.  This way of working can lead to difficult to track errors in that
       * objects that you expect to be destroyed as a result of a call to Remove are not necessarily
       * deleted.  Use Compact to delete objects that need deleting.</p>
       */
      virtual inline void ceefit_call_spec Remove(int aIndex, int aCountToRemove=1)
      {
        // Validate parameters
        if(aIndex < 0 || aIndex >= Count)
        {
          throw new BOUNDSEXCEPTION(L"Index out of bounds", aIndex);
        }

        if(aCountToRemove <= 0 || (aIndex + aCountToRemove) > Count)
        {
          throw new BOUNDSEXCEPTION(L"Count to remove went out of bounds", aCountToRemove);
        }

        // Move values from the end of the LIST over the removed items
        int endValueIndex = aIndex + aCountToRemove;
        int startValueIndex = aIndex;

        while(endValueIndex < Count) 
        {
          List[startValueIndex] = List[endValueIndex];

          endValueIndex++;
          startValueIndex++;
        }

        Count -= aCountToRemove;
      }

      virtual inline void ceefit_call_spec Reset(void)
      {
        Count = 0;      
        Limit = 0;
        delete [] List;
        List = NULL;
      }

      virtual inline void ceefit_call_spec Compact(void) 
      {    
        if(Count > 0) 
        {  
          if(Limit > 0 && Count < Limit) 
          {
            ANYTYPE* oldList = List;
            Limit = Count;
            List = new ANYTYPE[Limit];
      
            if(oldList != NULL) 
            {
              int i = -1;
              while(++i < Count) 
              {
                List[i] = oldList[i];
              }        

              delete [] oldList;
            }              
          }
        }
        else 
        {
          Reset();
        }
      }

      virtual inline ANYTYPE& ceefit_call_spec operator[](int index) 
      {
        if(!((index >= 0) && (index < Count)))
        {
          throw new BOUNDSEXCEPTION(L"Index out of bounds", index);
        }

        return(List[index]); 
      }

      virtual inline const ANYTYPE& ceefit_call_spec operator[](int index) const
      {
        if(!((index >= 0) && (index < Count)))
        {
          throw new BOUNDSEXCEPTION(L"Index out of bounds", index);
        }

        return(List[index]); 
      }

      /**
       * <p>Extends the count of ANYTYPE elements that can be used.</p>
       *
       * <p>If the free number of elements does not already exist in the DYNARRAY, the Limit is automatically extended to cover any extra 
       * number of elements that need to be reserved.</p>
       */
      virtual inline void ceefit_call_spec Reserve(int howMany) 
      {
        int difference = Limit - (Count + howMany);

        if(difference < 0)
        {
          ExtendLimit(GetExtension(-difference));
        }
          
        Count += howMany;
      }

      /**
       * <p>IsEqual compares contents of arrays</p>
       */
      template<class ANYTYPE2> inline bool IsEqual(const DYNARRAY<ANYTYPE2>& other)
      {
        if(GetSize() != other.GetSize())
        {
          return(false);
        }
        int i = -1;
        while(++i < GetSize())
        {
          if(!::CEEFIT::IsEqual(Get(i), other.Get(i)))
          {
            return(false);
          }
        }
        return(true);
      }

      /**
       * <p>IsEqual compares contents of arrays</p>
       */
      inline bool IsEqual(const DYNARRAY<ANYTYPE>& other) const
      {
        if(GetSize() != other.GetSize())
        {
          return(false);
        }
        int i = -1;
        while(++i < GetSize())
        {
          if(!::CEEFIT::IsEqual(Get(i), other.Get(i)))
          {
            return(false);
          }
        }
        return(true);
      }

      /**
       * <p>Generate a hash code from the contents of the array</p>
       */
      int GetHashCode(void) const;

    protected:
      /**
       * Extend the limit of the DYNARRAY.  
       *
       * @param howMany the number of values to extend the limit by, it is illegal for this value to be <= 0
       */
      virtual inline void ceefit_call_spec ExtendLimit(int howMany) 
      {
        if(!(howMany > 0))
        {
          throw new BOUNDSEXCEPTION(L"howMany <= 0", howMany);
        }

        ANYTYPE* oldList = List;
        Limit += howMany;
        List = new ANYTYPE[Limit];
      
        if(oldList != NULL) 
        {
          int i = -1;
          while(++i < Count) 
          {
            List[i] = oldList[i];
          }        

          delete [] oldList;
        }      
      }

      /**
       * <p>The extension algorithm for DYNARRAY.</p>
       *
       * <p>So that we aren't always copying the internal array with a fast growing list (which is costly cpu-wise) 
       * we will always double in size after the first extension.</p>
       *
       * <p>Depending on the number of DYNARRAY's there are in the system and how many of them grow to a substantial
       * size, this algorithm may lead to an unneccessary waste of memory.  Use Compact on your DYNARRAY's if memory 
       * consumption is an issue in your program due to this method's implementation.</p>
       */
      virtual inline int ceefit_call_spec GetExtension(int howMany) 
      {
        int retVal = Limit;
      
        do 
        {
          retVal = retVal * 2;
          if(retVal == 0) 
          {
            retVal = min_dynarray_size;
          }
        }
        while((retVal - Count) < howMany);

        return(retVal);
      }
  };
};

#endif // __TOOLS_DYNARRAY_H__
