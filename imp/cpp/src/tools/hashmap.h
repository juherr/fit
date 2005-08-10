#ifndef __HASHMAP_H__
#define __HASHMAP_H__

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
  inline int GetHashCode(bool aValue)
  {
    return((int) aValue);
  }

  inline int GetHashCode(signed char aValue)
  {
    return((int) aValue);
  }

  inline int GetHashCode(unsigned char aValue)
  {
    return((int) (char&) aValue);
  }

  inline int GetHashCode(char aValue)
  {
    return((int) aValue);
  }

  inline int GetHashCode(signed short aValue)
  {
    return((int) (short&) aValue);
  }

  inline int GetHashCode(unsigned short aValue)
  {
    return((int) (short&) aValue);
  }

#ifdef __GNUC__
  inline int GetHashCode(wchar_t aValue)
  {
    return((int) aValue);
  }
#endif

  inline int GetHashCode(unsigned int aValue)
  {
    return((int&) aValue);
  }

  inline int GetHashCode(int aValue)
  {
    return((int) aValue);
  }

  inline int GetHashCode(unsigned long aValue)
  {
    return((int&) aValue);
  }

  inline int GetHashCode(long aValue)
  {
    return((int) aValue);
  }

  inline int GetHashCode(fitINT64& aValue)
  {
    int hiSdword = ((int) (aValue >> 32));
    return(((int) aValue) + hiSdword);
  }

  inline int GetHashCode(UfitINT64& aValue)
  {
    return(((int) aValue) + ((int) (aValue >> 32)));
  }

  inline int GetHashCode(float aValue)
  {
    return((int&) aValue);
  }

  inline int GetHashCode(double& aValue)
  {
    return(::CEEFIT::GetHashCode((fitINT64&) aValue));
  }

  inline int GetHashCode(const char* aValue)
  {
    int accum = 0;
    int i = 0;
    while(aValue[i] != '\0')
    {
      accum += aValue[i];
      i++;
    }

    return(accum);
  }

  inline int GetHashCode(char* aValue)
  {
    return(::CEEFIT::GetHashCode((const char*) aValue));
  }

  inline int GetHashCode(const wchar_t* aValue)
  {
    int accum = 0;
    int i = 0;
    while(aValue[i] != L'\0')
    {
      accum += aValue[i];
      i++;
    }

    return(accum);
  }

  inline int GetHashCode(wchar_t* aValue)
  {
    return(::CEEFIT::GetHashCode((const wchar_t*) aValue));
  }

  template<class ANYTYPE> int GetHashCode(ANYTYPE* aValue)
  {
    return(aValue->GetHashCode());
  }

  template<class ANYTYPE> int GetHashCode(ANYTYPE& aValue)
  {
    return(aValue.GetHashCode());
  }

  template<class ANYTYPE> int DYNARRAY<ANYTYPE>::GetHashCode(void) const
  {
    int hashCode = 0;
    int i = -1;
    while(++i < GetSize())
    {
      hashCode += ::CEEFIT::GetHashCode(Get(i));
    }
    return(hashCode);
  }

  class HASHMAPNODEBASE : public REFCOUNTED
  {
    public:
      inline HASHMAPNODEBASE(void) {}
      virtual inline ~HASHMAPNODEBASE(void) {}
      inline HASHMAPNODEBASE(const HASHMAPNODEBASE&) {}
      HASHMAPNODEBASE& operator=(const HASHMAPNODEBASE&) { return(*this); }

      virtual const std::type_info& GetKeyType(void) const=0;
      virtual bool IsEqual(HASHMAPNODEBASE& aNode)=0;
      virtual bool IsKeyEqual(HASHMAPNODEBASE& aNode)=0;
      virtual bool IsKeyEqual(void* aVoidPtr)=0;
      virtual int GetHashCode(void)=0;
  };

  template<class ANYTYPE> class NODE : public HASHMAPNODEBASE
  {
    private:
      ANYTYPE Value;

    public:
      virtual inline ~NODE<ANYTYPE>(void) {}

      virtual const std::type_info& GetKeyType(void) const=0;

      template<class ANYTYPE2> NODE<ANYTYPE>(ANYTYPE2& aValue)
      {
        Value = aValue;
      }

      inline NODE<ANYTYPE>& operator=(NODE<ANYTYPE>& aNode)
      {
        Value = aNode.Value;

        return(*this);
      }

      inline ANYTYPE& GetValue(void)
      {
        return(Value);
      }

      inline const ANYTYPE& GetValue(void) const
      {
        return(Value);
      }

      inline void SetValue(ANYTYPE& aValue)
      {
        Value = aValue;
      }

      inline void SetValue(const ANYTYPE& aValue)
      {
        Value = aValue;
      }

      virtual bool IsEqual(HASHMAPNODEBASE& aNode)=0;
      virtual bool IsKeyEqual(HASHMAPNODEBASE& aNode)=0;
      virtual bool IsKeyEqual(void* aVoidPtr)=0;

      virtual int GetHashCode(void)=0;

      inline bool IsEqual(PTR<NODE>& aNode)
      {
        return(this->IsEqual(*aNode.GetPointer()));
      }

      inline bool IsKeyEqual(PTR<NODE>& aNode)
      {
        return(this->IsEqual(*aNode.GetPointer()));
      }

    private:
      NODE<ANYTYPE>(void);
      NODE<ANYTYPE>(const NODE<ANYTYPE>&);
  };

  template<class ANYTYPE, class KEYTYPE> class NODEIMPL : public NODE<ANYTYPE>
  {
    private:
      KEYTYPE Key;

    public:
      const std::type_info& GetKeyType(void) const 
      {
        return(typeid(KEYTYPE));
      }

      inline KEYTYPE& GetKey(void)
      {
        return(Key);
      }

      inline const KEYTYPE& GetKey(void) const
      {
        return(Key);
      }

      inline void SetKey(KEYTYPE& aKey)
      {
        Key = aKey;
      }

      inline void SetKey(const KEYTYPE& aKey)
      {
        Key = aKey;
      }

      inline bool IsKeyEqual(HASHMAPNODEBASE& aNode)
      {
        const std::type_info& aNodeKeyType = aNode.GetKeyType();
        const std::type_info& thisNodeKeyType = this->GetKeyType();

        if(aNodeKeyType == thisNodeKeyType)
        {
          return(aNode.IsKeyEqual((void*) &this->GetKey()));
        }
        return(false);
      }

      inline bool IsKeyEqual(void* aVoidPtr)
      {
        KEYTYPE* otherKeyPtr = (KEYTYPE*) aVoidPtr;

        return(::CEEFIT::IsEqual(*otherKeyPtr, this->GetKey()));
      }

      inline bool IsKeyEqual(KEYTYPE& aKey)
      {
        return(::CEEFIT::IsEqual(GetKey(), aKey));
      }

      inline bool IsEqual(HASHMAPNODEBASE& aNode)
      {
        NODEIMPL<ANYTYPE, KEYTYPE>* aNodeImpl = dynamic_cast< NODEIMPL<ANYTYPE, KEYTYPE>* >(&aNode);
        if(aNodeImpl != null)
        {
          return(::CEEFIT::IsEqual(aNodeImpl->GetKey(), this->GetKey()) && ::CEEFIT::IsEqual(aNodeImpl->GetValue(), this->GetValue()));
        }
        return(false);
      }

      inline bool IsEqual(KEYTYPE& aKey, ANYTYPE& aValue)
      {
        return(::CEEFIT::IsEqual(this->GetKey(), aKey) && ::CEEFIT::IsEqual(this->GetValue(), aValue));
      }

      inline int GetHashCode(void)
      {
        return(::CEEFIT::GetHashCode(Key));
      }

      inline NODEIMPL<ANYTYPE, KEYTYPE>(void) {}
      inline ~NODEIMPL<ANYTYPE, KEYTYPE>(void) {}

      inline NODEIMPL<ANYTYPE, KEYTYPE>& operator=(NODEIMPL<ANYTYPE, KEYTYPE>& aNode)
      {
        this->NODE<ANYTYPE>::operator=(aNode);

        Key = aNode.Key;

        return(*this);
      }

      inline NODEIMPL<ANYTYPE, KEYTYPE>(NODEIMPL<ANYTYPE, KEYTYPE>& aNode)
      {
        operator=(aNode);
      }

      inline NODEIMPL<ANYTYPE, KEYTYPE>(KEYTYPE& aKey, ANYTYPE& aValue) : NODE<ANYTYPE>(aValue)
      {
        SetKey(aKey);
      }
  };

  template<class ANYTYPE, int listsize=101> struct HASHMAP : public OBJECT
  {
    public:

      DYNARRAY< PTR< NODE<ANYTYPE> > > Array[listsize];

      HASHMAP<ANYTYPE, listsize>& operator=(const HASHMAP<ANYTYPE, listsize>& aHash);
      HASHMAP<ANYTYPE, listsize>(const HASHMAP<ANYTYPE, listsize>& aHash);

    public:
      inline HASHMAP<ANYTYPE, listsize>(void) {}
      inline ~HASHMAP<ANYTYPE, listsize>(void) {}

      inline void Reset(void)
      {
        int i = -1;
        while(++i < listsize)
        {
          Array[i].Reset();
        }
      }

      template<class KEYTYPE> inline bool Put(KEYTYPE& aKey, ANYTYPE& aValue, bool allowDuplicates = false)
      {
        int aHash = ::CEEFIT::GetHashCode(aKey);
        int aIndex = aHash % listsize;

        DYNARRAY< PTR< NODE<ANYTYPE> > >& aList = Array[aIndex];
        if(!allowDuplicates)
        {
          PTR< NODE<ANYTYPE> > nodePtr(new NODEIMPL<ANYTYPE, KEYTYPE>(aKey, aValue));

          int i = aList.GetSize();
          while(i--)
          {
            if(aList[i]->IsKeyEqual(*nodePtr))
            {
              aList[i]->SetValue(aValue);
              return(false);
            }
          }

          aList.Add(nodePtr);
        }
        else
        {
          int i = aList.GetSize();

          PTR< NODE<ANYTYPE> > nodePtr(new NODEIMPL<ANYTYPE, KEYTYPE>(aKey, aValue));

          while(i--)
          {
            if(aList[i]->IsEqual(nodePtr))
            {
              return(false);
            }
          }
          aList.Add(nodePtr);
        }
        return(true);
      }

      template<class KEYTYPE> inline void RemoveKey(KEYTYPE& aKey)
      {
        int aHash = ::CEEFIT::GetHashCode(aKey);
        int aIndex = aHash % listsize;

        DYNARRAY< PTR< NODE<ANYTYPE> > >& aList = Array[aIndex];
        int i = aList.GetSize();
        while(i--)
        {
          NODEIMPL<ANYTYPE, KEYTYPE>* aNode = dynamic_cast<NODEIMPL<ANYTYPE, KEYTYPE>*>(aList[i].GetPointer());
          if(aNode != null)
          {
            if(aNode->IsKeyEqual(aKey))
            {
              aList.Remove(i);
            }
          }
        }
      }

      template<class KEYTYPE> inline void Remove(KEYTYPE& aKey, ANYTYPE& aValue)
      {
        int aHash = ::CEEFIT::GetHashCode(aKey);
        int aIndex = aHash % listsize;

        DYNARRAY< PTR< NODE<ANYTYPE> > >& aList = Array[aIndex];
        int i = aList.GetSize();
        while(i--)
        {
          NODEIMPL<ANYTYPE, KEYTYPE>* aNode = dynamic_cast<NODEIMPL<ANYTYPE, KEYTYPE>*>(aList[i].GetPointer());
          if(aNode != null)
          {
            if(aNode->IsEqual(aKey, aValue))
            {
              aList.Remove(i);
              break;
            }
          }
        }
      }

      template<class KEYTYPE> inline ANYTYPE* Get(KEYTYPE& aKey)
      {
        int aHash = ::CEEFIT::GetHashCode(aKey);
        int aIndex = aHash % listsize;

        DYNARRAY< PTR< NODE<ANYTYPE> > >& aList = Array[aIndex];
        int i = aList.GetSize();
        while(i--)
        {
          NODEIMPL<ANYTYPE, KEYTYPE>* aNode = dynamic_cast<NODEIMPL<ANYTYPE, KEYTYPE>*>(aList[i].GetPointer());
          if(aNode != null)
          {
            if(aNode->IsKeyEqual(aKey))
            {
              return(&aList[i]->GetValue());
            }
          }
        }
        return(null);
      }

      /**
       * <p>Use the key managed in NODE to get a value in the HASHMAP</p>
       */
      inline ANYTYPE* Get(HASHMAPNODEBASE& aNode)
      {
        int aHash = aNode.GetHashCode();
        int aIndex = aHash % listsize;

        DYNARRAY< PTR< NODE<ANYTYPE> > >& aList = Array[aIndex];
        int i = aList.GetSize();
        while(i--)
        {
          PTR< NODE<ANYTYPE> > bNode(aList[i]);
          if(bNode->IsKeyEqual(aNode))
          {
            return(&bNode->GetValue());
          }
        }
        return(null);
      }

      template<class KEYTYPE> inline void GetAll(const KEYTYPE& aKey, DYNARRAY<ANYTYPE*>& retList)
      {
        int aHash = ::CEEFIT::GetHashCode(aKey);
        int aIndex = aHash % listsize;

        DYNARRAY< PTR< NODE<ANYTYPE> > >& aList = Array[aIndex];
        int i = aList.GetSize();
        while(i--)
        {
          NODEIMPL<ANYTYPE, KEYTYPE>* aNode = dynamic_cast<NODEIMPL<ANYTYPE, KEYTYPE>*>(aList[i].GetPointer());
          if(aNode != null)
          {
            if(aNode->IsKeyEqual(aKey))
            {
              retList.Add(&aNode->GetValue());
            }
          }
        }
      }

      template<class NODETYPE> void UniqueKeySet(DYNARRAY< PTR<NODETYPE> >& out)
      {
        int i = -1;
        while(++i < listsize)
        {
          DYNARRAY< PTR< NODE<ANYTYPE> > >& aArray = Array[i];
          int j = -1;
          while(++j < aArray.GetSize())
          {
            PTR< NODE<ANYTYPE> >& aNode = aArray[j];
            if(out.GetSize() == 0)
            {
              PTR<NODETYPE> outNode(aNode.GetPointer());

              out.Add(outNode);
            }
            else
            {
              int k = -1;
              while(++k < out.GetSize())
              {
                if(out[k]->IsKeyEqual(*aNode))
                {
                  break;
                }
              }
              if(k == out.GetSize())
              {
                PTR<NODETYPE> outNode(aNode.GetPointer());

                out.Add(outNode);
              }
            }
          }
        }
      }

  };
};

#endif // __HASHMAP_H__
