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
  void ceefit_call_spec LinkFieldToCurrentFixture(CEEFIT::CELLADAPTER* newField);

  template<class OUTTYPE, class INTYPE> OUTTYPE* ceefit_call_spec safe_dynamic_cast(INTYPE* in)
  {
    if(in == null)
    {
      throw new EXCEPTION("Null pointer exception in safe_dynamic_cast");
    }

    OUTTYPE* out = dynamic_cast<OUTTYPE*>(dynamic_cast<CEEFIT::OBJECT*>(in));

    if(out == null)
    {
      throw new EXCEPTION("dynamic_cast failed to relate INTYPE to OUTTYPE");
    }

    return(out);
  }

  template<class T> class CASTTOFIXTURE           { public: static inline FIXTURE* ceefit_call_spec DoCast(T* aField) { return(safe_dynamic_cast< ::CEEFIT::FIXTURE, T >(aField)); } };
  template<> class CASTTOFIXTURE<bool>            { public: static inline FIXTURE* ceefit_call_spec DoCast(bool* aField) { return(null); } };
  template<> class CASTTOFIXTURE<char>            { public: static inline FIXTURE* ceefit_call_spec DoCast(char* aField) { return(null); } };
  template<> class CASTTOFIXTURE<unsigned char>   { public: static inline FIXTURE* ceefit_call_spec DoCast(unsigned char* aField) { return(null); } };
  template<> class CASTTOFIXTURE<signed char>     { public: static inline FIXTURE* ceefit_call_spec DoCast(signed char* aField) { return(null); } };
  template<> class CASTTOFIXTURE<wchar_t>         { public: static inline FIXTURE* ceefit_call_spec DoCast(wchar_t* aField) { return(null); } };
  template<> class CASTTOFIXTURE<signed short>    { public: static inline FIXTURE* ceefit_call_spec DoCast(signed short* aField) { return(null); } };
# ifdef __GNUC__
  // does unsigned short differ from wchar_t on GCC??
  template<> class CASTTOFIXTURE<unsigned short>  { public: static inline FIXTURE* ceefit_call_spec DoCast(unsigned short* aField) { return(null); } };
#endif
  template<> class CASTTOFIXTURE<signed long>     { public: static inline FIXTURE* ceefit_call_spec DoCast(signed long* aField) { return(null); } };
  template<> class CASTTOFIXTURE<unsigned long>   { public: static inline FIXTURE* ceefit_call_spec DoCast(unsigned long* aField) { return(null); } };
  template<> class CASTTOFIXTURE<signed int>      { public: static inline FIXTURE* ceefit_call_spec DoCast(signed int* aField) { return(null); } };
  template<> class CASTTOFIXTURE<unsigned int>    { public: static inline FIXTURE* ceefit_call_spec DoCast(unsigned int* aField) { return(null); } };
  template<> class CASTTOFIXTURE<float>           { public: static inline FIXTURE* ceefit_call_spec DoCast(float* aField) { return(null); } };
  template<> class CASTTOFIXTURE<double>          { public: static inline FIXTURE* ceefit_call_spec DoCast(double* aField) { return(null); } };
  template<> class CASTTOFIXTURE<fitINT64>        { public: static inline FIXTURE* ceefit_call_spec DoCast(fitINT64* aField) { return(null); } };
  template<> class CASTTOFIXTURE<UfitINT64>       { public: static inline FIXTURE* ceefit_call_spec DoCast(UfitINT64* aField) { return(null); } };


  template<class T> class FITFIELDBASE : public CEEFIT::CELLADAPTER
  {
    protected:
      mutable T* Field;
      mutable bool DestroyField;        /**< Sometimes we refer to an external Field, sometimes we refer to an internal one (which must be destroyed) */
      mutable PTR<FIXTURE> FixturePtr;  /**< Make sure we keep a reference, just in case */
      STRING Name;

    public:
      inline ceefit_init_spec FITFIELDBASE<T>(void)
      {
        Field = null;
        DestroyField = false;

        LinkFieldToCurrentFixture(this);          // the new CEEFIT::FIXTURE will be located and this will be added to it
      }

      virtual inline ceefit_dtor_spec ~FITFIELDBASE<T>(void)
      {
        if(FixturePtr != null)
        {
          FixturePtr = null;
        }
        else if(DestroyField)
        {
          delete Field;
        }
      }

      inline void ceefit_call_spec SetName(const STRING& aName)
      {
        Name = aName;
      }

      inline const STRING& ceefit_call_spec GetName(void) const
      {
        return(Name);
      }

      virtual const char* ceefit_call_spec GetType(void) const=0;

      virtual inline bool ceefit_call_spec IsMethod(void) const
      {
        return(false);
      }

      virtual inline bool ceefit_call_spec IsField(void) const
      {
        return(true);
      }

      virtual FIXTURE* ceefit_call_spec ToFixture(void)
      {
        FIXTURE* out = CASTTOFIXTURE<T>::DoCast(Field);

        if(out == null)
        {
          throw new EXCEPTION("Field is not a FIXTURE type");
        }

        FixturePtr = out;

        return(out);
      }

      inline void ceefit_call_spec SetFieldPointer(T* aField)
      {
        if(Field != null && DestroyField == true)
        {
          if(FixturePtr != null)
          {
            FixturePtr = null;    // the reference counting should take over here ...
          }
          else
          {
            delete Field;
          }
          Field = null;
        }
        DestroyField = false;
        Field = aField;
      }

      inline T& ceefit_call_spec GetField(void)
      {
        if(Field == null)
        {
          Field = new T;
          DestroyField = true;
        }

        return(*Field);
      }

      inline const T& ceefit_call_spec GetField(void) const
      {
        if(Field == null)
        {
          Field = new T;
          DestroyField = true;
        }

        return(*Field);
      }

      fit_using_decl(CELLADAPTER::Invoke);
      virtual void ceefit_call_spec Invoke(CEEFIT::PTR< CEEFIT::CELLADAPTER >& out, CEEFIT::PTR<CEEFIT::FIXTURE>& aFixture) const
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

      virtual inline void ceefit_call_spec NewInstanceParse(CEEFIT::FIXTURE* callParseOn, CEEFIT::PTR<CEEFIT::CELLADAPTER>& out, const STRING& aText);

      virtual int ceefit_call_spec GetHashCode(void)
      {
        return(CEEFIT::GetHashCode(this->GetField()));
      }

      virtual bool ceefit_call_spec IsEqual(const CEEFIT::CELLADAPTER& aCell) const;

    private:
      template<class U> class RVAL
      {
        public:
          static inline T ceefit_call_spec Resolve(U& rValue)
          {
            return(T(rValue));
          }
      };

      template<class U> class RVAL < FITFIELDBASE<U> >
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

      fit_using_decl(CELLADAPTER::WriteToFixtureVar);
      inline void ceefit_call_spec WriteToFixtureVar(const STRING& in)
      {
        if(!this->Parse(this->GetField(), in))
        {
          throw new PARSEEXCEPTION(STRING("Failed to parse a field:  expected type") +
              ((this->GetName().Length() > 0) ? (STRING(" for field named \"") + this->GetName() + "\"") : STRING("")) +
              " was:  " + typeid(T).name() + ".  String that failed to parse to expected type was:  " + in);
        }
      }

      fit_using_decl(CELLADAPTER::ReadFromFixtureVar);
      inline void ceefit_call_spec ReadFromFixtureVar(STRING& out) const
      {
        this->ToString(out, this->GetField());
      }

      fit_using_decl(CELLADAPTER::ToString);
      virtual void ceefit_call_spec ToString(STRING& out, const T& in) const=0;
      virtual bool ceefit_call_spec Parse(T& out, const STRING& in)=0;

    protected:
      ceefit_init_spec FITFIELDBASE<T>(FITFIELDBASE<T>&);  /**< not implemented, do not call */
  };
};

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
    virtual inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const T& in) const
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

    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      ThrowSpecializationNeededException();

      return("");
    }

    inline ceefit_init_spec FITFIELD<T>(void)
    {
      ThrowSpecializationNeededException();
    }

    virtual inline ceefit_dtor_spec ~FITFIELD<T>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<T>(const FITFIELD<T>&);  /**< not implemented, do not call. */
    FITFIELD<T>& operator=(const FITFIELD<T>&);  /**< not implemented, do not call. */
};

namespace CEEFIT
{
  template<class T> bool ceefit_call_spec FITFIELDBASE<T>::IsEqual(const CELLADAPTER& aCell) const
  {
    const CELLEQUITABLE<T>* cellEquitable = dynamic_cast< const CELLEQUITABLE<T>* >(&aCell);
    const CELLEQUITABLE<T>* thisEquitable = dynamic_cast< const CELLEQUITABLE<T>* >(this);

    if(cellEquitable == null || thisEquitable == null)
    {
      CEEFIT::STRING thisString;
      this->ReadFromFixtureVar(thisString);
      CEEFIT::STRING cellString;
      aCell.ReadFromFixtureVar(cellString);

      return(thisString.IsEqual(cellString));
    }
    else
    {
      return(thisEquitable->CellIsEqual(*cellEquitable));
    }
  }
};

template<> class FITFIELD<void> : public CEEFIT::CELLADAPTER
{
  protected:
    CEEFIT::STRING Name;

  public:
    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      return("void");
    }

    template<class U> inline FITFIELD<void>& operator=(U& rValue) { throw new CEEFIT::EXCEPTION("Cannot assign to void"); }
    inline ceefit_init_spec FITFIELD<void>(void) {}
    virtual inline ceefit_dtor_spec ~FITFIELD<void>(void) {}

    fit_using_decl(CELLADAPTER::WriteToFixtureVar);
    fit_using_decl(CELLADAPTER::ReadFromFixtureVar);

    virtual void ceefit_call_spec WriteToFixtureVar(const CEEFIT::STRING& aIn) { throw new CEEFIT::EXCEPTION("Cannot assign to void"); }
    virtual void ceefit_call_spec ReadFromFixtureVar(CEEFIT::STRING& aOut) const { throw new CEEFIT::EXCEPTION("Cannot read from void"); }

    virtual void ceefit_call_spec SetName(const CEEFIT::STRING& aName) { Name = aName; }
    virtual const CEEFIT::STRING& ceefit_call_spec GetName(void) const { return(Name); }
    virtual bool ceefit_call_spec IsMethod(void) const { return(false); }
    virtual bool ceefit_call_spec IsField(void) const { return(true); }
    virtual CEEFIT::FIXTURE* ceefit_call_spec ToFixture(void) { throw new CEEFIT::EXCEPTION("Not a fixture"); }
    virtual void ceefit_call_spec NewInstanceParse(CEEFIT::FIXTURE* callParseOn, CEEFIT::PTR< CEEFIT::CELLADAPTER >& out, const CEEFIT::STRING& aText) { out = new FITFIELD<void>(); }

    fit_using_decl(CELLADAPTER::Invoke);
    virtual void ceefit_call_spec Invoke(CEEFIT::PTR< CEEFIT::CELLADAPTER >& out, CEEFIT::PTR< CEEFIT::FIXTURE >& aFixture) const { throw new CEEFIT::EXCEPTION("Not a method"); }

    virtual int ceefit_call_spec GetHashCode() { throw new CEEFIT::EXCEPTION("Cannot get hashcode to void"); }
    virtual bool ceefit_call_spec IsEqual(const CEEFIT::CELLADAPTER& aCell) const { throw new CEEFIT::EXCEPTION("Cannot test hashcode for equality"); }

  private:
    ceefit_init_spec FITFIELD<void>(const FITFIELD<void>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD<bool> : public CEEFIT::FITFIELDBASE<bool>
{
  public:
    typedef CEEFIT::FITFIELDBASE<bool> FIELDBASE;

    fit_using_decl(FIELDBASE::ToString);
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const bool& in) const
    {
      CEEFIT::SafeSprintf(out, L"%s", in ? L"true" : L"false");
    }

    inline bool ceefit_call_spec Parse(bool& out, const CEEFIT::STRING& in)
    {
      const wchar_t* aString = in.GetBuffer();
      if(!CEEFIT::fit_wcsicmp(aString, L"true") || !CEEFIT::fit_wcsicmp(aString, L"yes") || !CEEFIT::fit_wcsicmp(aString, L"1"))
      {
        out = true;
        return(true);
      }
      else if(!CEEFIT::fit_wcsicmp(aString, L"false") || !CEEFIT::fit_wcsicmp(aString, L"no") || !CEEFIT::fit_wcsicmp(aString, L"0"))
      {
        out = false;
        return(true);
      }
      return(false);
    }

    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      return("bool");
    }

    template<class U> inline FITFIELD<bool>& operator=(U& rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    FITFIELD<bool>& operator=(bool rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<bool>(void)
    {
    }

    virtual inline ceefit_dtor_spec ~FITFIELD<bool>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<bool>(const FITFIELD<bool>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD<unsigned char> : public CEEFIT::FITFIELDBASE<unsigned char>
{
  public:
    typedef CEEFIT::FITFIELDBASE<unsigned char> FIELDBASE;

    fit_using_decl(FIELDBASE::ToString);
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const unsigned char& in) const
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

    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      return("unsigned char");
    }

    template<class U> inline FITFIELD<unsigned char>& operator=(U& rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    FITFIELD<unsigned char>& operator=(unsigned char rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<unsigned char>(void)
    {
    }

    virtual inline ceefit_dtor_spec ~FITFIELD<unsigned char>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<unsigned char>(const FITFIELD<unsigned char>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD<signed char> : public CEEFIT::FITFIELDBASE<signed char>
{
  public:
    typedef CEEFIT::FITFIELDBASE<signed char> FIELDBASE;

    fit_using_decl(FIELDBASE::ToString);
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const signed char& in) const
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

    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      return("signed char");
    }

    template<class U> inline FITFIELD<signed char>& operator=(U& rValue)
    {
      this->FITFIELD::operator=(rValue);

      return(*this);
    }

    FITFIELD<signed char>& operator=(signed char rValue)
    {
      this->FITFIELD::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<signed char>(void)
    {
    }

    virtual inline ceefit_dtor_spec ~FITFIELD<signed char>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<signed char>(const FITFIELD<signed char>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD<char> : public CEEFIT::FITFIELDBASE<char>
{
  public:
    typedef CEEFIT::FITFIELDBASE<char> FIELDBASE;

    fit_using_decl(FIELDBASE::ToString);
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const char& in) const
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

    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      return("char");
    }

    template<class U> inline FITFIELD<char>& operator=(U& rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    FITFIELD<char>& operator=(char rValue)
    {
      this->FITFIELD::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<char>(void)
    {
    }

    virtual inline ceefit_dtor_spec ~FITFIELD<char>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<char>(const FITFIELD<char>&);  /**< not implemented, do not call. */
};

# ifdef __GNUC__
  // does unsigned short differ from wchar_t on GCC??
  template<> class FITFIELD<unsigned short> : public CEEFIT::FITFIELDBASE<unsigned short>
  {
    public:
      typedef CEEFIT::FITFIELDBASE<unsigned short> FIELDBASE;

      fit_using_decl(FIELDBASE::ToString);
      inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const unsigned short& in) const
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

      virtual inline const char* ceefit_call_spec GetType(void) const
      {
        return("unsigned short");
      }

      template<class U> inline FITFIELD<unsigned short>& operator=(U& rValue)
      {
        this->FIELDBASE::operator=(rValue);

        return(*this);
      }

      FITFIELD<unsigned short>& operator=(unsigned short rValue)
      {
        this->FIELDBASE::operator=(rValue);

        return(*this);
      }

      inline ceefit_init_spec FITFIELD<unsigned short>(void)
      {
      }

      virtual inline ceefit_dtor_spec ~FITFIELD<unsigned short>(void)
      {
      }

    private:
      ceefit_init_spec FITFIELD<unsigned short>(const FITFIELD<unsigned short>&);  /**< not implemented, do not call. */
  };
# endif

template<> class FITFIELD<signed short> : public CEEFIT::FITFIELDBASE<signed short>
{
  public:
    typedef CEEFIT::FITFIELDBASE<signed short> FIELDBASE;

    fit_using_decl(FIELDBASE::ToString);
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const signed short& in) const
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

    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      return("signed short");
    }

    template<class U> inline FITFIELD<signed short>& operator=(U& rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    FITFIELD<signed short>& operator=(signed short rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<signed short>(void)
    {
    }

    virtual inline ceefit_dtor_spec ~FITFIELD<signed short>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<signed short>(const FITFIELD<signed short>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD<wchar_t> : public CEEFIT::FITFIELDBASE<wchar_t>
{
  public:
    typedef CEEFIT::FITFIELDBASE<wchar_t> FIELDBASE;

    fit_using_decl(FIELDBASE::ToString);
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const wchar_t& in) const
    {
      wchar_t temp = in;

      CEEFIT::SafeSprintf(out, L"%c", temp);
    }

    inline bool ceefit_call_spec Parse(wchar_t& out, const CEEFIT::STRING& in)
    {
      return(swscanf(in.GetBuffer(), L"%c", &out)==1);
    }

    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      return("wchar_t");
    }

    template<class U> inline FITFIELD<wchar_t>& operator=(U& rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    FITFIELD<wchar_t>& operator=(wchar_t rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<wchar_t>(void)
    {
    }

    virtual inline ceefit_dtor_spec ~FITFIELD<wchar_t>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<wchar_t>(const FITFIELD<wchar_t>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD<unsigned int> : public CEEFIT::FITFIELDBASE<unsigned int>
{
  public:
    typedef CEEFIT::FITFIELDBASE<unsigned int> FIELDBASE;

    fit_using_decl(FIELDBASE::ToString);
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const unsigned int& in) const
    {
      unsigned int temp = in;

      CEEFIT::SafeSprintf(out, L"%u", temp);
    }

    inline bool ceefit_call_spec Parse(unsigned int& out, const CEEFIT::STRING& in)
    {
      return(swscanf(in.GetBuffer(), L"%u", &out)==1);
    }

    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      return("unsigned int");
    }

    template<class U> inline FITFIELD<unsigned int>& operator=(U& rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    FITFIELD<unsigned int>& operator=(unsigned int rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<unsigned int>(void)
    {
    }

    virtual inline ceefit_dtor_spec ~FITFIELD<unsigned int>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<unsigned int>(const FITFIELD<unsigned int>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD<signed int> : public CEEFIT::FITFIELDBASE<signed int>
{
  public:
    typedef CEEFIT::FITFIELDBASE<signed int> FIELDBASE;

    fit_using_decl(FIELDBASE::ToString);
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const signed int& in) const
    {
      signed int temp = in;

      CEEFIT::SafeSprintf(out, L"%i", temp);
    }

    inline bool ceefit_call_spec Parse(signed int& out, const CEEFIT::STRING& in)
    {
      return(swscanf(in.GetBuffer(), L"%i", &out)==1);
    }

    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      return("signed int");
    }

    template<class U> inline FITFIELD<signed int>& operator=(U& rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    FITFIELD<signed int>& operator=(signed int rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<signed int>(void)
    {
    }

    virtual inline ceefit_dtor_spec ~FITFIELD<signed int>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<signed int>(const FITFIELD<signed int>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD<unsigned long> : public CEEFIT::FITFIELDBASE<unsigned long>
{
  public:
    typedef CEEFIT::FITFIELDBASE<unsigned long> FIELDBASE;

    fit_using_decl(FIELDBASE::ToString);
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const unsigned long& in) const
    {
      unsigned long temp = in;

      CEEFIT::SafeSprintf(out, L"%lu", temp);
    }

    inline bool ceefit_call_spec Parse(unsigned long& out, const CEEFIT::STRING& in)
    {
      return(swscanf(in.GetBuffer(), L"%lu", &out)==1);
    }

    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      return("unsigned long");
    }

    template<class U> inline FITFIELD<unsigned long>& operator=(U& rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    FITFIELD<unsigned long>& operator=(unsigned long rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<unsigned long>(void)
    {
    }

    virtual inline ceefit_dtor_spec ~FITFIELD<unsigned long>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<unsigned long>(const FITFIELD<unsigned long>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD<signed long> : public CEEFIT::FITFIELDBASE<signed long>
{
  public:
    typedef CEEFIT::FITFIELDBASE<signed long> FIELDBASE;

    fit_using_decl(FIELDBASE::ToString);
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const signed long& in) const
    {
      signed long temp = in;

      CEEFIT::SafeSprintf(out, L"%li", temp);
    }

    inline bool ceefit_call_spec Parse(signed long& out, const CEEFIT::STRING& in)
    {
      return(swscanf(in.GetBuffer(), L"%li", &out)==1);
    }

    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      return("signed long");
    }

    template<class U> inline FITFIELD<signed long>& operator=(U& rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    FITFIELD<signed long>& operator=(signed long rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<signed long>(void)
    {
    }

    virtual inline ceefit_dtor_spec ~FITFIELD<signed long>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<signed long>(const FITFIELD<signed long>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD< CEEFIT::UfitINT64 > : public CEEFIT::FITFIELDBASE< CEEFIT::UfitINT64 >
{
  public:
    typedef CEEFIT::FITFIELDBASE< CEEFIT::UfitINT64 > FIELDBASE;

    fit_using_decl(FIELDBASE::ToString);
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const CEEFIT::UfitINT64& in) const
    {
      CEEFIT::UfitINT64 temp = in;

      CEEFIT::SafeSprintf(out, L"%llu", temp);
    }

    inline bool ceefit_call_spec Parse(CEEFIT::UfitINT64& out, const CEEFIT::STRING& in)
    {
#     if (defined(_MSC_VER) || defined(__BORLANDC__))
        return(swscanf(in.GetBuffer(), L"%I64u", &out)==1);
#     else
        return(swscanf(in.GetBuffer(), L"%llu", &out)==1);
#     endif
    }

    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      return("CEEFIT::UfitINT64");
    }

    template<class U> inline FITFIELD< CEEFIT::UfitINT64 >& operator=(U& rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    FITFIELD< CEEFIT::UfitINT64 >& operator=(CEEFIT::UfitINT64& rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD< CEEFIT::UfitINT64 >(void)
    {
    }

    virtual inline ceefit_dtor_spec ~FITFIELD< CEEFIT::UfitINT64 >(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD< CEEFIT::UfitINT64 >(const FITFIELD< CEEFIT::UfitINT64 >&);  /**< not implemented, do not call. */
};

template<> class FITFIELD< CEEFIT::fitINT64 > : public CEEFIT::FITFIELDBASE< CEEFIT::fitINT64 >
{
  public:
    typedef CEEFIT::FITFIELDBASE< CEEFIT::fitINT64 > FIELDBASE;

    fit_using_decl(FIELDBASE::ToString);
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const CEEFIT::fitINT64& in) const
    {
      CEEFIT::fitINT64 temp = in;

      CEEFIT::SafeSprintf(out, L"%lld", temp);
    }

    inline bool ceefit_call_spec Parse(CEEFIT::fitINT64& out, const CEEFIT::STRING& in)
    {
#     if (defined(_MSC_VER) || defined(__BORLANDC__))
        return(swscanf(in.GetBuffer(), L"%I64d", &out)==1);
#     else
        return(swscanf(in.GetBuffer(), L"%lld", &out)==1);
#     endif
    }

    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      return("CEEFIT::fitINT64");
    }

    template<class U> inline FITFIELD< CEEFIT::fitINT64 >& operator=(U& rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    FITFIELD< CEEFIT::fitINT64 >& operator=(CEEFIT::fitINT64& rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD< CEEFIT::fitINT64 >(void)
    {
    }

    virtual inline ceefit_dtor_spec ~FITFIELD< CEEFIT::fitINT64 >(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD< CEEFIT::fitINT64 >(const FITFIELD< CEEFIT::fitINT64 >&);  /**< not implemented, do not call. */
};

template<> class FITFIELD<float> : public CEEFIT::FITFIELDBASE<float>, public CEEFIT::CELLEQUITABLE<float>
{
  public:
    typedef CEEFIT::FITFIELDBASE<float> FIELDBASE;

    fit_using_decl(FIELDBASE::ToString);
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const float& in) const
    {
      float temp = in;

      CEEFIT::SafeSprintf(out, L"%f", temp);
    }

    inline bool ceefit_call_spec Parse(float& out, const CEEFIT::STRING& in)
    {
      errno = 0;
      wchar_t* endChar;
      out = (float) wcstod(in.GetBuffer(), &endChar);

      return(errno != ERANGE);
    }

    inline bool ceefit_call_spec CellIsEqual(const CEEFIT::CELLEQUITABLE<float>& otherCell) const
    {
      const FITFIELD<float>* otherField = dynamic_cast< const FITFIELD<float>* >(&otherCell);

      CEEFIT::AssertNotNull(otherField);

      return(this->GetField() == otherField->GetField());
    }

    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      return("float");
    }

    template<class U> inline FITFIELD<float>& operator=(U& rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    FITFIELD<float>& operator=(float rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<float>(void)
    {
    }

    virtual inline ceefit_dtor_spec ~FITFIELD<float>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<float>(const FITFIELD<float>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD<double> : public CEEFIT::FITFIELDBASE<double>, public CEEFIT::CELLEQUITABLE<double>
{
  public:
    typedef CEEFIT::FITFIELDBASE<double> FIELDBASE;

    fit_using_decl(FIELDBASE::ToString);
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const double& in) const
    {
      double temp = in;

      CEEFIT::SafeSprintf(out, L"%g", temp);
    }

    inline bool ceefit_call_spec Parse(double& out, const CEEFIT::STRING& in)
    {
      errno = 0;
      wchar_t* endChar;
      out = wcstod(in.GetBuffer(), &endChar);
      return(errno != ERANGE);
    }

    inline bool ceefit_call_spec CellIsEqual(const CEEFIT::CELLEQUITABLE<double>& otherCell) const
    {
      const FITFIELD<double>* otherField = dynamic_cast< const FITFIELD<double>* >(&otherCell);

      CEEFIT::AssertNotNull(otherField);

      return(this->GetField() == otherField->GetField());
    }

    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      return("double");
    }

    template<class U> inline FITFIELD<double>& operator=(U& rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    FITFIELD<double>& operator=(double rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<double>(void)
    {
    }

    virtual inline ceefit_dtor_spec ~FITFIELD<double>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<double>(const FITFIELD<double>&);  /**< not implemented, do not call. */
};

template<> class FITFIELD< CEEFIT::STRING > : public CEEFIT::FITFIELDBASE< CEEFIT::STRING >
{
  public:
    typedef CEEFIT::FITFIELDBASE< CEEFIT::STRING > FIELDBASE;

    fit_using_decl(FIELDBASE::ToString);
    inline void ceefit_call_spec ToString( CEEFIT::STRING& out, const CEEFIT::STRING& in ) const
    {
      out = in;
    }

    inline bool ceefit_call_spec Parse( CEEFIT::STRING& out, const CEEFIT::STRING& in )
    {
      out = in;
      return(true);
    }

    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      return("CEEFIT::STRING");
    }

    template<class U> inline FITFIELD<CEEFIT::STRING>& operator=(U& rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    FITFIELD<CEEFIT::STRING>& operator=(const CEEFIT::STRING& rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<CEEFIT::STRING>(void)
    {
    }

    virtual inline ceefit_dtor_spec ~FITFIELD<CEEFIT::STRING>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<CEEFIT::STRING>(const FITFIELD<CEEFIT::STRING>&);  /**< not implemented, do not call. */
};

namespace CEEFIT
{
  template<class T> inline void ceefit_call_spec SetLastLinkedFieldInfo(const char* aName, T& aField)
  {
    CEEFIT::PTR<CEEFIT::CELLADAPTER> baseAdapter(RUNNER::GetLastRegisteredField());
    ::FITFIELD<T>* fitField = dynamic_cast< ::FITFIELD<T>* >(baseAdapter.GetPointer());

    AssertNotNull(fitField);

    fitField->SetName(STRING(aName));
    fitField->SetFieldPointer(&aField);
  }

  void ceefit_call_spec LinkManualField(CEEFIT::FIXTURE* aFixture, CEEFIT::CELLADAPTER* fitfieldManual);
};

template<class FIXTURETYPE, class FIELDTYPE> void ceefit_call_spec RegisterCeefitField(FIXTURETYPE* fixture, const char* fieldName, FIELDTYPE& actualField)
{
  FITFIELD<FIELDTYPE>* aFitField = new FITFIELD<FIELDTYPE>();

  aFitField->SetName(::CEEFIT::STRING(fieldName));
  aFitField->SetFieldPointer(&actualField);

  ::CEEFIT::LinkManualField(fixture, aFitField);
}


#endif // __CEEFIT_FITFIELD_H__
