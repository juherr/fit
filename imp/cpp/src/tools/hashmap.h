#ifndef __HASHMAP_H__
#define __HASHMAP_H__

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

  inline int GetHashCode(INT64& aValue)
  {
    int hiSdword = ((int) (aValue >> 32));
    return(((int) aValue) + hiSdword);
  }

  inline int GetHashCode(UINT64& aValue)
  {
    return(((int) aValue) + ((int) (aValue >> 32)));
  }

  inline int GetHashCode(float aValue)
  {
    return((int&) aValue);
  }

  inline int GetHashCode(double& aValue)
  {
    return(::CEEFIT::GetHashCode((INT64&) aValue));
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

      virtual bool IsEqual(HASHMAPNODEBASE& aNode)=0;
      virtual bool IsKeyEqual(HASHMAPNODEBASE& aNode)=0;
      virtual int GetHashCode(void)=0;
  };

  template<class ANYTYPE, int listsize=101> struct HASHMAP : public OBJECT
  {
    public:
      class NODE : public HASHMAPNODEBASE
      {
        private:
          ANYTYPE Value;

        public:
          virtual inline ~NODE(void) {}
    
          template<class ANYTYPE2> NODE(ANYTYPE2& aValue) 
          { 
            Value = aValue; 
          }

          inline NODE& operator=(NODE& aNode)
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
          NODE(void);
          NODE(const NODE&);
      };

    private:
      template<class KEYTYPE> class NODEIMPL : public NODE
      {
        private:
          KEYTYPE Key;

        public:
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
            NODEIMPL<KEYTYPE>* aNodeImpl = dynamic_cast< NODEIMPL<KEYTYPE>* >(&aNode);
            if(aNodeImpl != NULL)
            {
              return(::CEEFIT::IsEqual(aNodeImpl->GetKey(), GetKey()));
            }
            return(false);
          }

          inline bool IsKeyEqual(KEYTYPE& aKey)
          {
            return(::CEEFIT::IsEqual(GetKey(), aKey));
          }

          inline bool IsEqual(HASHMAPNODEBASE& aNode)
          {
            NODEIMPL<KEYTYPE>* aNodeImpl = dynamic_cast< NODEIMPL<KEYTYPE>* >(&aNode);
            if(aNodeImpl != NULL)
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

          inline NODEIMPL<KEYTYPE>(void) {}
          inline ~NODEIMPL<KEYTYPE>(void) {}

          inline NODEIMPL<KEYTYPE>& operator=(NODEIMPL<KEYTYPE>& aNode)
          {
            this->NODE::operator=(aNode);

            Key = aNode.Key;

            return(*this);
          }

          inline NODEIMPL<KEYTYPE>(NODEIMPL<KEYTYPE>& aNode) 
          { 
            operator=(aNode); 
          }

          inline NODEIMPL<KEYTYPE>(KEYTYPE& aKey, ANYTYPE& aValue) : NODE(aValue)
          { 
            SetKey(aKey);
          }

          virtual inline const type_info& GetTypeInfo(void) { return(typeid(NODEIMPL<KEYTYPE>)); }
      };

      DYNARRAY< PTR<NODE> > Array[listsize];

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
    
        DYNARRAY< PTR<NODE> >& aList = Array[aIndex];
        if(!allowDuplicates)
        {
          PTR<NODE> nodePtr(new NODEIMPL<KEYTYPE>(aKey, aValue));          

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

          PTR<NODE> nodePtr(new NODEIMPL<KEYTYPE>(aKey, aValue));

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
    
        DYNARRAY< PTR<NODE> >& aList = Array[aIndex];
        int i = aList.GetSize();
        while(i--)
        {
          NODEIMPL<KEYTYPE>* aNode = dynamic_cast<NODEIMPL<KEYTYPE>*>(aList[i].GetPointer());
          if(aNode != NULL)
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
    
        DYNARRAY< PTR<NODE> >& aList = Array[aIndex];
        int i = aList.GetSize();
        while(i--)
        {
          NODEIMPL<KEYTYPE>* aNode = dynamic_cast<NODEIMPL<KEYTYPE>*>(aList[i].GetPointer());
          if(aNode != NULL)
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
    
        DYNARRAY< PTR<NODE> >& aList = Array[aIndex];
        int i = aList.GetSize();
        while(i--)
        {
          NODEIMPL<KEYTYPE>* aNode = dynamic_cast<NODEIMPL<KEYTYPE>*>(aList[i].GetPointer());
          if(aNode != NULL)
          {
            if(aNode->IsKeyEqual(aKey))
            {
              return(&aList[i]->GetValue());
            }
          }
        }
        return(NULL);
      }

      /**
       * <p>Use the key managed in NODE to get a value in the HASHMAP</p>
       */
      inline ANYTYPE* Get(HASHMAPNODEBASE& aNode)
      {
        int aHash = aNode.GetHashCode();
        int aIndex = aHash % listsize;
    
        DYNARRAY< PTR<NODE> >& aList = Array[aIndex];
        int i = aList.GetSize();
        while(i--)
        {
          PTR<NODE> bNode(aList[i]);
          if(bNode->IsKeyEqual(aNode))
          {
            return(&bNode->GetValue());
          }
        }
        return(NULL);
      }

      template<class KEYTYPE> inline void GetAll(const KEYTYPE& aKey, DYNARRAY<ANYTYPE*>& retList)
      {
        int aHash = ::CEEFIT::GetHashCode(aKey);
        int aIndex = aHash % listsize;
    
        DYNARRAY< PTR<NODE> >& aList = Array[aIndex];
        int i = aList.GetSize();
        while(i--)
        {
          NODEIMPL<KEYTYPE>* aNode = dynamic_cast<NODEIMPL<KEYTYPE>*>(aList[i].GetPointer());
          if(aNode != NULL)
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
          DYNARRAY< PTR<NODE> >& aArray = Array[i];
          int j = -1;
          while(++j < aArray.GetSize())
          {
            PTR<NODE>& aNode = aArray[j];
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
