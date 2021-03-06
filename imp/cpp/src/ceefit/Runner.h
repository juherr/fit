#ifndef __CEEFIT_RUNNER_H__
#define __CEEFIT_RUNNER_H__

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

  void ceefit_call_spec LinkFieldToCurrentFixture(CELLADAPTER* newField);

  extern void ceefit_call_spec SetLastLinkedTestName(const char* aName);
  template<class T> void ceefit_call_spec SetLastLinkedFieldInfo(const char* aName, T& aField);

  extern CEEFIT::FIXTURE* ceefit_call_spec GetFixtureUnderConstruction(void);

  template<class T> class SETFIELDINFO
  {
    public:
      SETFIELDINFO<T>(void)
      {
        FIXTURE* aFixture = GetFixtureUnderConstruction();
        if(aFixture != null)
        {
          // T is some class that has a public static GetFieldName() method
          const char* fieldName = T::GetFieldName();

          if((&T::GetField(aFixture)) != null)
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
      ceefit_init_spec FIXTUREFACTORY(const char* aName, const char* aAlias=null);
      virtual ceefit_dtor_spec ~FIXTUREFACTORY(void);
      virtual const char* ceefit_call_spec GetName(void) const;
      virtual const char* ceefit_call_spec GetAlias(void) const;

      virtual void ceefit_call_spec CreateFixture(PTR<FIXTURE>& out)=0;

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

  class NONFIXTUREFACTORY : public SLINK<NONFIXTUREFACTORY>
  {
    protected:
      const char* Name;
      const char* Alias;

    public:
      ceefit_init_spec NONFIXTUREFACTORY(const char* aName, const char* aAlias=null);
      virtual ceefit_dtor_spec ~NONFIXTUREFACTORY(void);
      virtual const char* ceefit_call_spec GetName(void) const;
      virtual const char* ceefit_call_spec GetAlias(void) const;

      virtual OBJECT* ceefit_call_spec CreateNonFixture(void)=0;

    private:
      /**
       * Not implemented.  Do not call.
       */
      NONFIXTUREFACTORY(void);

      /**
       * Not implemented.  Do not call.
       */
      NONFIXTUREFACTORY(const NONFIXTUREFACTORY&);

      /**
       * Not implemented.  Do not call.
       */
      NONFIXTUREFACTORY& operator=(const NONFIXTUREFACTORY&);
  };

  class RUNNER : public REFCOUNTED
  {
    public:
      static SLINKLIST<FIXTUREFACTORY>& ceefit_call_spec GetFixtureFactoryList(void);
      static SLINKLIST<NONFIXTUREFACTORY>& ceefit_call_spec GetNonFixtureFactoryList(void);
      static FIXTUREFACTORY* ceefit_call_spec GetSetCurrentFixtureFactory(FIXTUREFACTORY* aFactory=null, bool clearCurrentFactory=false);

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
      static NONFIXTUREFACTORY* ceefit_call_spec FindNonFixtureFactoryByName(const STRING& aName);
      static NONFIXTUREFACTORY* ceefit_call_spec FindNonFixtureFactoryByAlias(const STRING& aAlias);

      static void ceefit_call_spec RegisterAutoField(CELLADAPTER* aVar);
      static void ceefit_call_spec RegisterAutoTest(CELLADAPTER* aTest);
      static void ceefit_call_spec RegisterFixtureFactory(FIXTUREFACTORY* aFixtureFactory);
      static void ceefit_call_spec RegisterNonFixtureFactory(NONFIXTUREFACTORY* aNonFixtureFactory);

      virtual ceefit_dtor_spec ~RUNNER(void);

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
};

#endif // __CEEFIT_RUNNER_H__
