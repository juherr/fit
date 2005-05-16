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

      static VALUE<CELLADAPTER> ceefit_call_spec GetLastRegisteredField(void);
      static VALUE<CELLADAPTER> ceefit_call_spec GetLastRegisteredTest(void);

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

/**
 * <p>Special one-off CELLADAPTER that simply contains and exposes a FIXTURE object</p>
 *
 * <p>Currently only used by RowFixture.</p>
 */
class FITFIXTURECONTAINER : public CEEFIT::CELLADAPTER
{
  private:
    class CEEFIT::FIXTURE* Fixture;

  public:
    virtual void ceefit_call_spec WriteToFixtureVar(const CEEFIT::STRING& in)
    {
      throw new CEEFIT::EXCEPTION("Cannot WriteToFixtureVar on FITFIXTURECONTAINER CELLADAPTER");
    }

    virtual void ceefit_call_spec ReadFromFixtureVar(CEEFIT::STRING& out)
    {
      throw new CEEFIT::EXCEPTION("Cannot ReadFromFixtureVar on FITFIXTURECONTAINER CELLADAPTER");
    }

    virtual void ceefit_call_spec SetName(const CEEFIT::STRING& aName)
    {
      throw new CEEFIT::EXCEPTION("Cannot SetName on FITFIXTURECONTAINER CELLADAPTER");
    }

    virtual const CEEFIT::STRING& ceefit_call_spec GetName(void) const
    {
      throw new CEEFIT::EXCEPTION("Cannot GetName on FITFIXTURECONTAINER CELLADAPTER");
    }

    virtual const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
      throw new CEEFIT::EXCEPTION("Cannot GetType on FITFIXTURECONTAINER CELLADAPTER");
    }

    virtual bool ceefit_call_spec IsMethod(void) const
    {
      throw new CEEFIT::EXCEPTION("Cannot IsMethod on FITFIXTURECONTAINER CELLADAPTER");
    }

    virtual bool ceefit_call_spec IsField(void) const
    {
      throw new CEEFIT::EXCEPTION("Cannot IsField on FITFIXTURECONTAINER CELLADAPTER");
    }

    virtual void ceefit_call_spec NewInstanceParse(::CEEFIT::PTR<CEEFIT::CELLADAPTER>& out, const CEEFIT::STRING& aText)
    {
      throw new CEEFIT::EXCEPTION("Cannot NewInstanceParse on FITFIXTURECONTAINER CELLADAPTER");
    }

    virtual void ceefit_call_spec Invoke(::CEEFIT::PTR<CEEFIT::CELLADAPTER>& out, CEEFIT::FIXTURE* aFixture)
    {
      throw new CEEFIT::EXCEPTION("Cannot Invoke on FITFIXTURECONTAINER CELLADAPTER");
    }
    
    inline ceefit_init_spec FITFIXTURECONTAINER(CEEFIT::FIXTURE* aFixture) 
    {
      Fixture = aFixture;
    }

    virtual inline ceefit_init_spec ~FITFIXTURECONTAINER(void) {}

    virtual int ceefit_call_spec GetHashCode(void)
    {
      return(0);
    }

    virtual bool ceefit_call_spec IsEqual(const CEEFIT::CELLADAPTER* aAdapter) const
    {
      return(false);
    }

    virtual CEEFIT::FIXTURE* GetFixture(void)
    {
      return(Fixture);
    }

    virtual inline const type_info& GetTypeInfo(void) { return(typeid(FITFIXTURECONTAINER)); }

  protected:
    inline ceefit_init_spec FITFIXTURECONTAINER(void); /**< not implemented, do not call.*/
    inline ceefit_init_spec FITFIXTURECONTAINER(FITFIXTURECONTAINER&);     /**< not implemented, do not call. */
    inline FITFIXTURECONTAINER& ceefit_call_spec operator=(const FITFIXTURECONTAINER&);   /**< not implemented, do not call. */
};

#endif // __CEEFIT_REGISTERSTATIC_H__
