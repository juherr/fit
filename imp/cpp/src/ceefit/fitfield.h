#ifndef __CEEFIT_FITFIELD_H__
#define __CEEFIT_FITFIELD_H__

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
  void ceefit_call_spec LinkFieldToCurrentFixture(CELLADAPTER* newField);

  template<class T> class FITFIELDBASE : public CELLADAPTER
  {
    protected:
      mutable T* Field;
      mutable bool DestroyField;      /**< Sometimes we refer to an external Field, sometimes we refer to an internal one (which must be destroyed) */
      CEEFIT::STRING Name;

    public:
      inline ceefit_init_spec FITFIELDBASE<T>(void)
      {
        Field = NULL;
        DestroyField = false;

        ::CEEFIT::LinkFieldToCurrentFixture(this);          // the new FIXTURE will be located and this will be added to it
      }

      virtual inline ~FITFIELDBASE<T>(void)
      {
        if(DestroyField)
        {
          delete Field;
        }
      }

      inline void ceefit_call_spec SetName(const CEEFIT::STRING& aName)
      {
        Name = aName;
      }

      inline const CEEFIT::STRING& ceefit_call_spec GetName(void) const
      {
        return(Name);
      }

      virtual const CEEFIT::STRING& ceefit_call_spec GetType(void) const=0;

      virtual inline bool ceefit_call_spec IsMethod(void) const
      {
        return(false);
      }

      virtual inline bool ceefit_call_spec IsField(void) const
      {
        return(true);
      }

      inline void ceefit_call_spec SetFieldPointer(T* aField)
      {
        if(Field != NULL && DestroyField == true) 
        {
          delete Field;
          Field = NULL;
        }
        DestroyField = false;
        Field = aField;
      }

      inline T& ceefit_call_spec GetField(void)
      {
        if(Field == NULL)
        {
          Field = new T;
          DestroyField = true;
        }

        return(*Field);
      }

      inline const T& ceefit_call_spec GetField(void) const
      {
        if(Field == NULL)
        {
          Field = new T;
          DestroyField = true;
        }

        return(*Field);
      }

      virtual void ceefit_call_spec Invoke(::CEEFIT::PTR< ::CEEFIT::CELLADAPTER >& out, CEEFIT::PTR<CEEFIT::FIXTURE>& aFixture)
      {
        throw new CEEFIT::EXCEPTION("You may not call Invoke on a Field-type cell");
      }

      inline ceefit_call_spec operator T&(void)
      {
        return(GetField());
      }

      inline ceefit_call_spec operator const T&(void) const
      {
        return(GetField());
      }

      virtual inline void ceefit_call_spec NewInstanceParse(::CEEFIT::PTR<CEEFIT::CELLADAPTER>& out, const CEEFIT::STRING& aText);

      virtual int ceefit_call_spec GetHashCode(void) 
      { 
        return(::CEEFIT::GetHashCode(this->GetField())); 
      }

      virtual bool ceefit_call_spec IsEqual(const ::CEEFIT::CELLADAPTER* aCell) const
      { 
        const ::CEEFIT::FITFIELDBASE<T>* aField;
        if(aCell == NULL || (aField = dynamic_cast< const ::CEEFIT::FITFIELDBASE<T>* >(aCell)) == NULL) 
        {
          return(false); 
        }
        else 
        {
          const T& thisField = this->GetField();
          const T& aCellField = aField->GetField();
          return(::CEEFIT::IsEqual(thisField, aCellField));
        }
      }

    private:
      template<class U> class RVAL
      {
        public:
          static inline T ceefit_call_spec Resolve(U& rValue)
          {
            return((T) rValue);
          }
      };

      template<class U> class RVAL< ::CEEFIT::FITFIELDBASE<U> >
      {
        public:
          static inline U& ceefit_call_spec Resolve(FITFIELDBASE<U>& rValue)
          {
            return(*(rValue.Field));
          }
      };

    public:
      template<class U> inline FITFIELDBASE<T>& operator=(U& rValue) { GetField() = RVAL<U>::Resolve(rValue); return(*this); }

      template<class U> inline T ceefit_call_spec operator+(U& rValue) { return(GetField() + RVAL<U>::Resolve(rValue)); }
      template<class U> inline T ceefit_call_spec operator-(U& rValue) { return(GetField() - RVAL<U>::Resolve(rValue)); }
      template<class U> inline T ceefit_call_spec operator*(U& rValue) { return(GetField() * RVAL<U>::Resolve(rValue)); }
      template<class U> inline T ceefit_call_spec operator/(U& rValue) { return(GetField() / RVAL<U>::Resolve(rValue)); }
      template<class U> inline T ceefit_call_spec operator%(U& rValue) { return(GetField() % RVAL<U>::Resolve(rValue)); }

      template<class U> inline T& ceefit_call_spec operator+=(U& rValue) { return(GetField() += RVAL<U>::Resolve(rValue)); }
      template<class U> inline T& ceefit_call_spec operator-=(U& rValue) { return(GetField() -= RVAL<U>::Resolve(rValue)); }
      template<class U> inline T& ceefit_call_spec operator*=(U& rValue) { return(GetField() *= RVAL<U>::Resolve(rValue)); }
      template<class U> inline T& ceefit_call_spec operator/=(U& rValue) { return(GetField() /= RVAL<U>::Resolve(rValue)); }
      template<class U> inline T& ceefit_call_spec operator%=(U& rValue) { return(GetField() %= RVAL<U>::Resolve(rValue)); }

      template<class U> inline bool ceefit_call_spec operator==(U& rValue) { return(GetField() == (T) rValue); }
      template<class U> inline bool ceefit_call_spec operator!=(U& rValue) { return(GetField() != (T) rValue); }

      inline T& ceefit_call_spec operator*(int) { return(*GetField()); }
      inline const T& ceefit_call_spec operator*(int) const { return(*GetField()); }

      inline void ceefit_call_spec WriteToFixtureVar(const CEEFIT::STRING& in)
      {
        if(!this->Parse(this->GetField(), in)) 
        {
          throw new PARSEEXCEPTION(STRING("Failed to parse a field, expected type") + 
              ((this->GetName().Length() > 0) ? (STRING(" for field named \"") + this->GetName() + "\"") : STRING("")) + 
              " was:  " + typeid(T).name() + ".  String that failed to parse to expected type was:  " + in);
        }
      }

      inline void ceefit_call_spec ReadFromFixtureVar(CEEFIT::STRING& out)
      {
        this->ToString(out, this->GetField());
      }

      virtual void ceefit_call_spec ToString(CEEFIT::STRING& out, const T& in)=0;
      virtual bool ceefit_call_spec Parse(T& out, const CEEFIT::STRING& in)=0;

    protected:
      ceefit_init_spec FITFIELDBASE<T>(FITFIELDBASE<T>&);  /**< not implemented, do not call */
  };

};

using namespace CEEFIT;

/* notice that FITFIELD is in the global namespace ... this is to be friendly to users so that they can make their own
   FITFIELD specializations for custom types outside of the CEEFIT namespace ... */

template<class T> class FITFIELD : public CEEFIT::FITFIELDBASE<T>
{
  private:
    inline void ceefit_call_spec ThrowSpecializationNeededException(void) const
    {
      throw new CEEFIT::EXCEPTION(CEEFIT::STRING("Specialization needed for FITFIELD for type:  ") + typeid(T).name());
    }

  public:
    /**
     * <p>Parse an T field or return value and convert (that was read from an HTML file) from
     */
    // if you get a linkage error related to the line below, it just means you need to define a specialization of FITFIELD for your
    // unique member, see examples below for details
    virtual inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const T& in)
    {
      ThrowSpecializationNeededException();
    }

    // if you get a linkage error related to the line below, it just means you need to define a specialization of FITFIELD for your
    // unique member, see examples below for details
    virtual inline bool ceefit_call_spec Parse(T& out, const CEEFIT::STRING& in)
    {
      ThrowSpecializationNeededException();

      return(false);
    }

    virtual inline const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
      ThrowSpecializationNeededException();

      static CEEFIT::STRING dummy;
      return(dummy);
    }

    inline ceefit_init_spec FITFIELD<T>(void)
    {
      ThrowSpecializationNeededException();
    }

    virtual inline ceefit_init_spec ~FITFIELD<T>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<T>(const FITFIELD<T>&);  /**< not implemented, do not call. */
    FITFIELD<T>& operator=(const FITFIELD<T>&);  /**< not implemented, do not call. */
};

// need to define NewInstanceParse that could not have been until now
template<class T> inline void ceefit_call_spec ::CEEFIT::FITFIELDBASE<T>::NewInstanceParse(::CEEFIT::PTR< ::CEEFIT::CELLADAPTER >& out, const CEEFIT::STRING& aText)
{
  CEEFIT::CELLADAPTER* returnValue = new FITFIELD<T>();

  returnValue->WriteToFixtureVar(aText);

  out = returnValue;
}

template<> class FITFIELD<void> : public CEEFIT::CELLADAPTER
{
  protected:
    CEEFIT::STRING Name;

  public:
    virtual inline const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
      static CEEFIT::STRING FieldType("void");

      return(FieldType);
    }

    template<class U> inline FITFIELD<void>& operator=(U& rValue) { throw new CEEFIT::EXCEPTION("Cannot assign to void"); }
    inline ceefit_init_spec FITFIELD<void>(void) {}
    virtual inline ceefit_init_spec ~FITFIELD<void>(void) {}

    virtual void ceefit_call_spec WriteToFixtureVar(const CEEFIT::STRING& in) { throw new CEEFIT::EXCEPTION("Cannot assign to void"); }
    virtual void ceefit_call_spec ReadFromFixtureVar(CEEFIT::STRING& out) { throw new CEEFIT::EXCEPTION("Cannot read from void"); }

    virtual void ceefit_call_spec SetName(const CEEFIT::STRING& aName) { Name = aName; }
    virtual const CEEFIT::STRING& ceefit_call_spec GetName(void) const { return(Name); }
    virtual bool ceefit_call_spec IsMethod(void) const { return(false); }
    virtual bool ceefit_call_spec IsField(void) const { return(true); }
    virtual void ceefit_call_spec NewInstanceParse(::CEEFIT::PTR< ::CEEFIT::CELLADAPTER >& out, const CEEFIT::STRING& aText) { out = new FITFIELD<void>(); }
    virtual void ceefit_call_spec Invoke(::CEEFIT::PTR< ::CEEFIT::CELLADAPTER >& out, ::CEEFIT::PTR< ::CEEFIT::FIXTURE >& aFixture) { throw new CEEFIT::EXCEPTION("Not a method"); }

    virtual int ceefit_call_spec GetHashCode() { throw new CEEFIT::EXCEPTION("Cannot get hashcode to void"); }
    virtual bool ceefit_call_spec IsEqual(const ::CEEFIT::CELLADAPTER* aCell) const { throw new CEEFIT::EXCEPTION("Cannot test hashcode for equality"); }

  private:
    ceefit_init_spec FITFIELD<void>(const FITFIELD<void>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD<bool> : public FITFIELDBASE<bool>
{
  public:
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const bool& in)
    {
      CEEFIT::SafeSprintf(out, L"%s", in ? L"true" : L"false");
    }

    inline bool ceefit_call_spec Parse(bool& out, const CEEFIT::STRING& in)
    {
      const wchar_t* aString = in.GetBuffer();
      if(!_wcsicmp(aString, L"true") || !_wcsicmp(aString, L"yes") || !_wcsicmp(aString, L"1"))
      {
        out = true;
        return(true);
      }
      else if(!_wcsicmp(aString, L"false") || !_wcsicmp(aString, L"no") || !_wcsicmp(aString, L"0"))
      {
        out = false;
        return(true);
      }
      return(false);
    }

    virtual inline const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
      static CEEFIT::STRING FieldType("bool");

      return(FieldType);
    }

    template<class U> inline FITFIELD<bool>& operator=(U& rValue)
    {
      this->FITFIELDBASE<bool>::operator=(rValue);

      return(*this);
    }

    FITFIELD<bool>& operator=(bool rValue)
    {
      this->FITFIELDBASE<bool>::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<bool>(void)
    {
    }

    virtual inline ceefit_init_spec ~FITFIELD<bool>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<bool>(const FITFIELD<bool>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD<unsigned char> : public FITFIELDBASE<unsigned char>
{
  public:
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const unsigned char& in)
    {
      CEEFIT::SafeSprintf(out, L"%u", (unsigned int) in);
    }

    inline bool ceefit_call_spec Parse(unsigned char& out, const CEEFIT::STRING& in)
    {
      unsigned int temp = 0;
      bool retVal = (swscanf(in.GetBuffer(), L"%u", &temp)==1);
      out = (unsigned char) temp;
      return(retVal);
    }

    virtual inline const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
      static CEEFIT::STRING FieldType("unsigned char");

      return(FieldType);
    }

    template<class U> inline FITFIELD<unsigned char>& operator=(U& rValue)
    {
      this->FITFIELDBASE<unsigned char>::operator=(rValue);

      return(*this);
    }

    FITFIELD<unsigned char>& operator=(unsigned char rValue)
    {
      this->FITFIELDBASE<unsigned char>::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<unsigned char>(void)
    {
    }

    virtual inline ceefit_init_spec ~FITFIELD<unsigned char>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<unsigned char>(const FITFIELD<unsigned char>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD<signed char> : public ::CEEFIT::FITFIELDBASE<signed char>
{
  public:
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const signed char& in)
    {
      CEEFIT::SafeSprintf(out, L"%C", (signed int) in);
    }

    inline bool ceefit_call_spec Parse(signed char& out, const CEEFIT::STRING& in)
    {
      int temp = 0;
      bool retVal = (swscanf(in.GetBuffer(), L"%C", &temp)==1);
      out = (signed char) temp;
      return(retVal);
    }

    virtual inline const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
      static CEEFIT::STRING FieldType("signed char");

      return(FieldType);
    }

    template<class U> inline FITFIELD<signed char>& operator=(U& rValue)
    {
      this->FITFIELDBASE<signed char>::operator=(rValue);

      return(*this);
    }

    FITFIELD<signed char>& operator=(signed char rValue)
    {
      this->FITFIELDBASE<signed char>::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<signed char>(void)
    {
    }

    virtual inline ceefit_init_spec ~FITFIELD<signed char>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<signed char>(const FITFIELD<signed char>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD<char> : public ::CEEFIT::FITFIELDBASE<char>
{
  public:
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const char& in)
    {
      char temp = in;
      CEEFIT::SafeSprintf(out, L"%C", temp);
    }

    inline bool ceefit_call_spec Parse(char& out, const CEEFIT::STRING& in)
    {
      int temp = 0;
      bool retVal = (swscanf(in.GetBuffer(), L"%C", &temp)==1);
      out = (char) temp;
      return(retVal);
    }

    virtual inline const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
      static CEEFIT::STRING FieldType("char");

      return(FieldType);
    }

    template<class U> inline FITFIELD<char>& operator=(U& rValue)
    {
      this->FITFIELDBASE<char>::operator=(rValue);

      return(*this);
    }

    FITFIELD<char>& operator=(char rValue)
    {
      this->FITFIELDBASE<char>::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<char>(void)
    {
    }

    virtual inline ceefit_init_spec ~FITFIELD<char>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<char>(const FITFIELD<char>&);  /**< not implemented, do not call. */
};

# ifdef __GNUC__
  // does unsigned short differ from wchar_t on GCC??
  template<> class FITFIELD<unsigned short> : public ::CEEFIT::FITFIELDBASE<unsigned short>
  {
    public:
      inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const unsigned short& in)
      {
        unsigned int temp = in;
        CEEFIT::SafeSprintf(out, L"%u", temp);
      }

      inline bool ceefit_call_spec Parse(unsigned short& out, const CEEFIT::STRING& in)
      {
        unsigned int temp = 0;
        bool retVal = (swscanf(in.GetBuffer(), L"%u", &temp)==1);
        out = (unsigned short) temp;
        return(retVal);
      }

      virtual inline const CEEFIT::STRING& ceefit_call_spec GetType(void) const
      {
        static CEEFIT::STRING FieldType("unsigned short");

        return(FieldType);
      }

      template<class U> inline FITFIELD<unsigned short>& operator=(U& rValue)
      {
        this->FITFIELDBASE<unsigned short>::operator=(rValue);

        return(*this);
      }

      FITFIELD<unsigned short>& operator=(unsigned short rValue)
      {
        this->FITFIELDBASE<unsigned short>::operator=(rValue);

        return(*this);
      }

      inline ceefit_init_spec FITFIELD<unsigned short>(void)
      {
      }

      virtual inline ceefit_init_spec ~FITFIELD<unsigned short>(void)
      {
      }

    private:
      ceefit_init_spec FITFIELD<unsigned short>(const FITFIELD<unsigned short>&);  /**< not implemented, do not call. */
  };
# endif

template<> class FITFIELD<signed short> : public ::CEEFIT::FITFIELDBASE<signed short>
{
  public:
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const signed short& in)
    {
      signed int temp = in;

      CEEFIT::SafeSprintf(out, L"%i", temp);
    }

    inline bool ceefit_call_spec Parse(signed short& out, const CEEFIT::STRING& in)
    {
      signed int temp = 0;
      bool retVal = (swscanf(in.GetBuffer(), L"%i", &temp)==1);
      out = (signed short) temp;
      return(retVal);
    }

    virtual inline const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
      static CEEFIT::STRING FieldType("signed short");

      return(FieldType);
    }

    template<class U> inline FITFIELD<signed short>& operator=(U& rValue)
    {
      this->FITFIELDBASE<signed short>::operator=(rValue);

      return(*this);
    }

    FITFIELD<signed short>& operator=(signed short rValue)
    {
      this->FITFIELDBASE<signed short>::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<signed short>(void)
    {
    }

    virtual inline ceefit_init_spec ~FITFIELD<signed short>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<signed short>(const FITFIELD<signed short>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD<wchar_t> : public ::CEEFIT::FITFIELDBASE<wchar_t>
{
  public:
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const wchar_t& in)
    {
      wchar_t temp = in;

      CEEFIT::SafeSprintf(out, L"%c", temp);
    }

    inline bool ceefit_call_spec Parse(wchar_t& out, const CEEFIT::STRING& in)
    {
      return(swscanf(in.GetBuffer(), L"%c", &out)==1);
    }

    virtual inline const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
      static CEEFIT::STRING FieldType("wchar_t");

      return(FieldType);
    }

    template<class U> inline FITFIELD<wchar_t>& operator=(U& rValue)
    {
      this->FITFIELDBASE<wchar_t>::operator=(rValue);

      return(*this);
    }

    FITFIELD<wchar_t>& operator=(wchar_t rValue)
    {
      this->FITFIELDBASE<wchar_t>::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<wchar_t>(void)
    {
    }

    virtual inline ceefit_init_spec ~FITFIELD<wchar_t>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<wchar_t>(const FITFIELD<wchar_t>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD<unsigned int> : public ::CEEFIT::FITFIELDBASE<unsigned int>
{
  public:
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const unsigned int& in)
    {
      unsigned int temp = in;

      CEEFIT::SafeSprintf(out, L"%u", temp);
    }

    inline bool ceefit_call_spec Parse(unsigned int& out, const CEEFIT::STRING& in)
    {
      return(swscanf(in.GetBuffer(), L"%u", &out)==1);
    }

    virtual inline const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
      static CEEFIT::STRING FieldType("unsigned int");

      return(FieldType);
    }

    template<class U> inline FITFIELD<unsigned int>& operator=(U& rValue)
    {
      this->FITFIELDBASE<unsigned int>::operator=(rValue);

      return(*this);
    }

    FITFIELD<unsigned int>& operator=(unsigned int rValue)
    {
      this->FITFIELDBASE<unsigned int>::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<unsigned int>(void)
    {
    }

    virtual inline ceefit_init_spec ~FITFIELD<unsigned int>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<unsigned int>(const FITFIELD<unsigned int>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD<signed int> : public ::CEEFIT::FITFIELDBASE<signed int>
{
  public:
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const signed int& in)
    {
      signed int temp = in;

      CEEFIT::SafeSprintf(out, L"%i", temp);
    }

    inline bool ceefit_call_spec Parse(signed int& out, const CEEFIT::STRING& in)
    {
      return(swscanf(in.GetBuffer(), L"%i", &out)==1);
    }

    virtual inline const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
      static CEEFIT::STRING FieldType("signed int");

      return(FieldType);
    }

    template<class U> inline FITFIELD<signed int>& operator=(U& rValue)
    {
      this->FITFIELDBASE<signed int>::operator=(rValue);

      return(*this);
    }

    FITFIELD<signed int>& operator=(signed int rValue)
    {
      this->FITFIELDBASE<signed int>::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<signed int>(void)
    {
    }

    virtual inline ceefit_init_spec ~FITFIELD<signed int>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<signed int>(const FITFIELD<signed int>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD<unsigned long> : public ::CEEFIT::FITFIELDBASE<unsigned long>
{
  public:
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const unsigned long& in)
    {
      unsigned long temp = in;

      CEEFIT::SafeSprintf(out, L"%lu", temp);
    }

    inline bool ceefit_call_spec Parse(unsigned long& out, const CEEFIT::STRING& in)
    {
      return(swscanf(in.GetBuffer(), L"%lu", &out)==1);
    }

    virtual inline const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
      static CEEFIT::STRING FieldType("unsigned long");

      return(FieldType);
    }

    template<class U> inline FITFIELD<unsigned long>& operator=(U& rValue)
    {
      this->FITFIELDBASE<unsigned long>::operator=(rValue);

      return(*this);
    }

    FITFIELD<unsigned long>& operator=(unsigned long rValue)
    {
      this->FITFIELDBASE<unsigned long>::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<unsigned long>(void)
    {
    }

    virtual inline ceefit_init_spec ~FITFIELD<unsigned long>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<unsigned long>(const FITFIELD<unsigned long>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD<signed long> : public ::CEEFIT::FITFIELDBASE<signed long>
{
  public:
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const signed long& in)
    {
      signed long temp = in;

      CEEFIT::SafeSprintf(out, L"%li", temp);
    }

    inline bool ceefit_call_spec Parse(signed long& out, const CEEFIT::STRING& in)
    {
      return(swscanf(in.GetBuffer(), L"%li", &out)==1);
    }

    virtual inline const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
      static CEEFIT::STRING FieldType("signed long");

      return(FieldType);
    }

    template<class U> inline FITFIELD<signed long>& operator=(U& rValue)
    {
      this->FITFIELDBASE<signed long>::operator=(rValue);

      return(*this);
    }

    FITFIELD<signed long>& operator=(signed long rValue)
    {
      this->FITFIELDBASE<signed long>::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<signed long>(void)
    {
    }

    virtual inline ceefit_init_spec ~FITFIELD<signed long>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<signed long>(const FITFIELD<signed long>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD< ::CEEFIT::UINT64 > : public ::CEEFIT::FITFIELDBASE< ::CEEFIT::UINT64 >
{
  public:
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const ::CEEFIT::UINT64& in)
    {
      ::CEEFIT::UINT64 temp = in;

#     ifdef _MSC_VER
        CEEFIT::SafeSprintf(out, L"%I64u", temp);
#     else
        CEEFIT::SafeSprintf(out, L"%llu", temp);
#     endif
    }

    inline bool ceefit_call_spec Parse(::CEEFIT::UINT64& out, const CEEFIT::STRING& in)
    {
#     ifdef _MSC_VER
        return(swscanf(in.GetBuffer(), L"%I64u", &out)==1);
#     else
        return(swscanf(in.GetBuffer(), L"%llu", &out)==1);
#     endif
    }

    virtual inline const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
      static CEEFIT::STRING FieldType("UINT64");

      return(FieldType);
    }

    template<class U> inline FITFIELD< ::CEEFIT::UINT64 >& operator=(U& rValue)
    {
      this->FITFIELDBASE< ::CEEFIT::UINT64 >::operator=(rValue);

      return(*this);
    }

    FITFIELD< ::CEEFIT::UINT64 >& operator=(::CEEFIT::UINT64& rValue)
    {
      this->FITFIELDBASE< ::CEEFIT::UINT64 >::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD< ::CEEFIT::UINT64 >(void)
    {
    }

    virtual inline ceefit_init_spec ~FITFIELD< ::CEEFIT::UINT64 >(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD< ::CEEFIT::UINT64 >(const FITFIELD< ::CEEFIT::UINT64 >&);  /**< not implemented, do not call. */
};

template<> class FITFIELD< ::CEEFIT::INT64 > : public ::CEEFIT::FITFIELDBASE< ::CEEFIT::INT64 >
{
  public:
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const ::CEEFIT::INT64& in)
    {
      ::CEEFIT::INT64 temp = in;

#     ifdef _MSC_VER
        CEEFIT::SafeSprintf(out, L"%I64i", temp);
#     else
        CEEFIT::SafeSprintf(out, L"%lld", temp);
#     endif
    }

    inline bool ceefit_call_spec Parse(::CEEFIT::INT64& out, const CEEFIT::STRING& in)
    {
#     ifdef _MSC_VER
        return(swscanf(in.GetBuffer(), L"%I64i", &out)==1);
#     else
        return(swscanf(in.GetBuffer(), L"%lld", &out)==1);
#     endif
    }

    virtual inline const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
        static CEEFIT::STRING FieldType("INT64");

      return(FieldType);
    }

    template<class U> inline FITFIELD< ::CEEFIT::INT64 >& operator=(U& rValue)
    {
      this->FITFIELDBASE< ::CEEFIT::INT64 >::operator=(rValue);

      return(*this);
    }

    FITFIELD< ::CEEFIT::INT64 >& operator=(::CEEFIT::INT64& rValue)
    {
      this->FITFIELDBASE< ::CEEFIT::INT64 >::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD< ::CEEFIT::INT64 >(void)
    {
    }

    virtual inline ceefit_init_spec ~FITFIELD< ::CEEFIT::INT64 >(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD< ::CEEFIT::INT64 >(const FITFIELD< ::CEEFIT::INT64 >&);  /**< not implemented, do not call. */
};

template<> class FITFIELD<float> : public ::CEEFIT::FITFIELDBASE<float>
{
  public:
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const float& in)
    {
      float temp = in;

      CEEFIT::SafeSprintf(out, L"%f", temp);
    }

    inline bool ceefit_call_spec Parse(float& out, const CEEFIT::STRING& in)
    {
      return(swscanf(in.GetBuffer(), L"%f", &out)==1);
    }

    virtual inline const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
      static CEEFIT::STRING FieldType("float");

      return(FieldType);
    }

    template<class U> inline FITFIELD<float>& operator=(U& rValue)
    {
      this->FITFIELDBASE<float>::operator=(rValue);

      return(*this);
    }

    FITFIELD<float>& operator=(float rValue)
    {
      this->FITFIELDBASE<float>::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<float>(void)
    {
    }

    virtual inline ceefit_init_spec ~FITFIELD<float>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<float>(const FITFIELD<float>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD<double> : public ::CEEFIT::FITFIELDBASE<double>
{
  public:
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const double& in)
    {
      double temp = in;

      CEEFIT::SafeSprintf(out, L"%g", temp);
    }

    inline bool ceefit_call_spec Parse(double& out, const CEEFIT::STRING& in)
    {
      return(swscanf(in.GetBuffer(), L"%g", &out)==1);
    }

    virtual inline const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
      static CEEFIT::STRING FieldType("double");

      return(FieldType);
    }

    template<class U> inline FITFIELD<double>& operator=(U& rValue)
    {
      this->FITFIELDBASE<double>::operator=(rValue);

      return(*this);
    }

    FITFIELD<double>& operator=(double rValue)
    {
      this->FITFIELDBASE<double>::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<double>(void)
    {
    }

    virtual inline ceefit_init_spec ~FITFIELD<double>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<double>(const FITFIELD<double>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD< CEEFIT::STRING > : public ::CEEFIT::FITFIELDBASE< CEEFIT::STRING >
{
  public:
    inline void ceefit_call_spec ToString( CEEFIT::STRING& out, const CEEFIT::STRING& in )
    {
      out = in;
    }

    inline bool ceefit_call_spec Parse( CEEFIT::STRING& out, const CEEFIT::STRING& in )
    {
      out = in;
      return(true);
    }

    virtual inline const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
      static CEEFIT::STRING FieldType("STRING");

      return(FieldType);
    }

    template<class U> inline FITFIELD<CEEFIT::STRING>& operator=(U& rValue)
    {
      this->FITFIELDBASE<CEEFIT::STRING>::operator=(rValue);

      return(*this);
    }

    FITFIELD<CEEFIT::STRING>& operator=(const CEEFIT::STRING& rValue)
    {
      this->FITFIELDBASE<CEEFIT::STRING>::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<CEEFIT::STRING>(void)
    {
    }

    virtual inline ceefit_init_spec ~FITFIELD<CEEFIT::STRING>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<CEEFIT::STRING>(const FITFIELD<CEEFIT::STRING>&);  /**< not implemented, do not call. */
};

namespace CEEFIT
{
  template<class T> inline void ceefit_call_spec SetLastLinkedFieldInfo(const char* aName, T& aField)
  {
    PTR<CELLADAPTER> baseAdapter(RUNNER::GetLastRegisteredField());
    ::FITFIELD<T>* fitField = dynamic_cast< ::FITFIELD<T>* >(baseAdapter.GetPointer());

    AssertNotNull(fitField);

    fitField->SetName(STRING(aName));
    fitField->SetFieldPointer(&aField);
  }

  void ceefit_call_spec LinkManualField(::CEEFIT::FIXTURE* aFixture, CELLADAPTER* fitfieldManual);
};

template<class FIXTURETYPE, class FIELDTYPE> void ceefit_call_spec RegisterCeefitField(FIXTURETYPE* fixture, const char* fieldName, FIELDTYPE& actualField)
{
  FITFIELD<FIELDTYPE>* aFitField = new FITFIELD<FIELDTYPE>();

  aFitField->SetName(::CEEFIT::STRING(fieldName));
  aFitField->SetFieldPointer(&actualField);

  ::CEEFIT::LinkManualField(fixture, aFitField);
}


#endif // __CEEFIT_FITFIELD_H__
