#ifndef __CEEFIT_REGISTERSTATIC_H__
#define __CEEFIT_REGISTERSTATIC_H__

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

  class FIXTURE;

  /**
   * <p>This class is an attempt to simulate a lot of what the Java Fit's TypeAdapter class does.</p>
   *
   * <p>I like to think of this class as the "HTML Table Cell Type/Field/Method Adapter" because cells
   * in Fit tables can be either fields or return values to match with method calls.</p>
   */
  class CELLADAPTER : public SLINK< CELLADAPTER >
  {
    public:
      virtual void ceefit_call_spec WriteToFixtureVar(const CEEFIT::STRING& in)=0;
      virtual void ceefit_call_spec ReadFromFixtureVar(CEEFIT::STRING& out)=0;

      virtual void ceefit_call_spec SetName(const CEEFIT::STRING& aName)=0;
      virtual const CEEFIT::STRING& ceefit_call_spec GetName(void) const=0;

      /**
       * <p>This returns a unique string per each type, specified by the developer.</p>
       *
       * <p>If the cell is a function type, then the string returned is the type of the return value for the function.</p>
       */
      virtual const CEEFIT::STRING& ceefit_call_spec GetType(void) const=0;

      virtual bool ceefit_call_spec IsMethod(void) const=0;
      virtual bool ceefit_call_spec IsField(void) const=0;

      /**
       * <p>Creates a new instance of the CELLADAPTER type
       */
      virtual CELLADAPTER* ceefit_call_spec NewInstanceParse(const CEEFIT::STRING& aText)=0;

      /**
       * <p>Invoke test fixture method, only function-type cells can be invoked</p>
       *
       * @returns CELLADAPTER* Containing the return value from the fixture test method call.  This CELLADAPTER* is allocated from the
       *                       heap and must be free'd by the caller.
       */
      virtual CELLADAPTER* ceefit_call_spec Invoke(FIXTURE* aFixture)=0;
      
      inline ceefit_init_spec CELLADAPTER(void) {}
      virtual inline ceefit_init_spec ~CELLADAPTER(void) {}

    protected:
      inline ceefit_init_spec CELLADAPTER(CELLADAPTER&);     /**< not implemented, do not call. */
      inline CELLADAPTER& ceefit_call_spec operator=(const CELLADAPTER&);   /**< not implemented, do not call. */
  };

  extern void ceefit_call_spec LinkFieldToCurrentFixture(CELLADAPTER* newField);

  extern void ceefit_call_spec SetLastLinkedTestName(const char* aName);
  template<class T> void ceefit_call_spec SetLastLinkedFieldInfo(const char* aName, T& aField);

  extern CEEFIT::FIXTURE* ceefit_call_spec GetFixtureUnderConstruction(void);

  template<class T> class SETFIELDINFO
  {
    public:
      SETFIELDINFO<T>(void)
      {
        FIXTURE* aFixture = GetFixtureUnderConstruction();
        if(aFixture != NULL)
        {
          // T is some class that has a public static GetFieldName() method
          const char* fieldName = T::GetFieldName();

          if((&T::GetField(aFixture)) != NULL)
          {
            SetLastLinkedFieldInfo(fieldName, T::GetField(aFixture));
          }
        }
      }

      ~SETFIELDINFO<T>(void)
      {
      }

    private:
      SETFIELDINFO<T>& operator=(const SETFIELDINFO<T>&);  /**< not implemented, do not call */
      SETFIELDINFO<T>(const SETFIELDINFO<T>&);  /**< not implemented, do not call */
  };

  template<class T> class SETTESTNAME
  {
    public:
      SETTESTNAME<T>(void)
      {
        SetLastLinkedTestName(T::GetTestName());       // T is some class that has a public static const char* GetTestName() method
      }

      ~SETTESTNAME<T>(void)
      {
      }

    private:
      SETTESTNAME<T>& operator=(const SETTESTNAME<T>&);  /**< not implemented, do not call */
      SETTESTNAME<T>(const SETTESTNAME<T>&);  /**< not implemented, do not call */
  };

  class FIXTUREFACTORY : public SLINK<FIXTUREFACTORY>
  {
    protected:
      const char* Name;
      const char* Alias;

    public:
      ceefit_init_spec FIXTUREFACTORY(const char* aName, const char* aAlias=NULL);
      virtual ceefit_init_spec ~FIXTUREFACTORY(void);
      virtual const char* ceefit_call_spec GetName(void) const;
      virtual const char* ceefit_call_spec GetAlias(void) const;

      virtual FIXTURE* ceefit_call_spec CreateFixture(void)=0;

    private:
      /**
       * Not implemented.  Do not call.
       */
      FIXTUREFACTORY(void);

      /**
       * Not implemented.  Do not call.
       */
      FIXTUREFACTORY(const FIXTUREFACTORY&);

      /**
       * Not implemented.  Do not call.
       */
      FIXTUREFACTORY& operator=(const FIXTUREFACTORY&);
  };

  class RUNNER : public virtual OBJECT
  {
    public:
      static SLINKLIST<FIXTUREFACTORY>& ceefit_call_spec GetFixtureFactoryList(void);
      static FIXTUREFACTORY* ceefit_call_spec GetSetCurrentFixtureFactory(FIXTUREFACTORY* aFactory=NULL, bool clearCurrentFactory=false);

      static void ceefit_call_spec SetCurrentTestList(SLINKLIST<CELLADAPTER>* aList);
      static void ceefit_call_spec SetCurrentFieldList(SLINKLIST<CELLADAPTER>* aList);
      static SLINKLIST<CELLADAPTER>* ceefit_call_spec GetCurrentTestList(void);
      static SLINKLIST<CELLADAPTER>* ceefit_call_spec GetCurrentFieldList(void);

      static CELLADAPTER* ceefit_call_spec GetLastRegisteredField(void);
      static CELLADAPTER* ceefit_call_spec GetLastRegisteredTest(void);

      static void ceefit_call_spec SetCurrentFixtureUnderConstruction(FIXTURE* aFixture);
      static FIXTURE* ceefit_call_spec GetCurrentFixtureUnderConstruction(void);

      static void ceefit_call_spec IncInFixtureConstructor(void);
      static void ceefit_call_spec DecInFixtureConstructor(void);
      static bool ceefit_call_spec InFixtureConstructor(void);
      static void ceefit_call_spec ResetInFixtureConstructor(void);

      static FIXTUREFACTORY* ceefit_call_spec FindFixtureFactoryByName(const char* aName);

      static void ceefit_call_spec RegisterAutoField(CELLADAPTER* aVar);
      static void ceefit_call_spec RegisterAutoTest(CELLADAPTER* aTest);
      static void ceefit_call_spec RegisterFixtureFactory(FIXTUREFACTORY* aTest);

      virtual ceefit_init_spec ~RUNNER(void);

    protected:
      static SLINKLIST<CELLADAPTER>* CurrentTestList;   /**< During a new FIXTURE construction, this is a pointer to it's TestList */
      static SLINKLIST<CELLADAPTER>* CurrentFieldList;  /**< During a new FIXTURE construction, this is a pointer to it's FieldList */
      static int InFixtureConstructorCount;             /**< Tracks how many FIXTURE subclasses are in their constructors */
      static FIXTURE* FixtureUnderConstruction;

      /**
       * <p>Only subclasses of RUNNER may call this constructor</p>
       */
      ceefit_init_spec RUNNER(void);

      /**
       * Do not call.
       */
      ceefit_init_spec RUNNER(const RUNNER&);

      /**
       * Do not call.
       */
      RUNNER& ceefit_call_spec operator=(const RUNNER&);
  };

  template<class T> class REGISTERFIXTURECLASS : public FIXTUREFACTORY
  {
    protected:
      friend class RUNNER;

    public:
      inline ceefit_init_spec REGISTERFIXTURECLASS<T>(const char* fixtureName, const char* aliasName=NULL) : FIXTUREFACTORY(fixtureName, aliasName)
      {
        // Do not add a fixture that appears in more than once ...
        if(RUNNER::FindFixtureFactoryByName(fixtureName) == NULL)
        {
          RUNNER::RegisterFixtureFactory(this);
          RUNNER::GetSetCurrentFixtureFactory(this);
        }
      }

      inline ceefit_init_spec ~REGISTERFIXTURECLASS<T>(void)
      {
      }

      virtual FIXTURE* ceefit_call_spec CreateFixture(void)
      {
        return(new T());
      }

    private:
      /**
       * Not implemented.  Do not call.
       */
      REGISTERFIXTURECLASS<T>(void);

      /**
       * Not implemented.  Do not call.
       */
      REGISTERFIXTURECLASS<T>& operator=(const REGISTERFIXTURECLASS<T>&);

      /**
       * Not implemented.  Do not call.
       */
      REGISTERFIXTURECLASS<T>(const REGISTERFIXTURECLASS<T>&);
  };
};


template<class T> class FITFIELDBASE : public CEEFIT::CELLADAPTER
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
      CEEFIT::LinkFieldToCurrentFixture(this);                    // the new FIXTURE will be located and this will be added to it
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
        DestroyField = false;
      }
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

    virtual CEEFIT::CELLADAPTER* ceefit_call_spec Invoke(CEEFIT::FIXTURE* aFixture)
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

    virtual inline CEEFIT::CELLADAPTER* ceefit_call_spec NewInstanceParse(const CEEFIT::STRING& aText);

  private:
    template<class U> class RVAL
    {
      public:
        static inline T ceefit_call_spec Resolve(U& rValue)
        {
          return((T) rValue);
        }
    };

    template<class U> class RVAL< FITFIELDBASE<U> >
    {
      public:
        static inline U& ceefit_call_spec Resolve(FITFIELDBASE<U>& rValue)
        {
          return(*(rValue.Field));
        }
    };

  public:
    template<class U> inline T& ceefit_call_spec operator=(U& rValue) { return(GetField() = RVAL<U>::Resolve(rValue)); }

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
      this->Parse(GetField(), in);
    }

    inline void ceefit_call_spec ReadFromFixtureVar(CEEFIT::STRING& out)
    {
      this->ToString(out, GetField());
    }

    virtual void ceefit_call_spec ToString(CEEFIT::STRING& out, const T& in)=0;
    virtual bool ceefit_call_spec Parse(T& out, const CEEFIT::STRING& in)=0;

  protected:
    ceefit_init_spec FITFIELDBASE<T>(FITFIELDBASE<T>&);  /**< not implemented, do not call */
};

#endif // __CEEFIT_REGISTERSTATIC_H__
