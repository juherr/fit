#ifndef __CEEFIT_FITTEST_H__
#define __CEEFIT_FITTEST_H__

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
  class FITTESTBASE : public CELLADAPTER
  {
    protected:
      CEEFIT::STRING Name;

    public:
      ceefit_init_spec FITTESTBASE(void);
      virtual ceefit_dtor_spec ~FITTESTBASE(void);

      const CEEFIT::STRING& ceefit_call_spec GetName(void) const;
      virtual void ceefit_call_spec SetName(const CEEFIT::STRING& testName);

      fit_using_decl(CELLADAPTER::Invoke);
      virtual inline void ceefit_call_spec Invoke(::CEEFIT::PTR< ::CEEFIT::CELLADAPTER >& out, ::CEEFIT::PTR< ::CEEFIT::FIXTURE >& aFixture) const { throw new CEEFIT::EXCEPTION("Invoke not implemented for this FITTEST"); }

      virtual inline const char* ceefit_call_spec GetType(void) const { throw new CEEFIT::EXCEPTION("GetType not implemented for this FITTEST"); }

      virtual void ceefit_call_spec NewInstanceParse(::CEEFIT::FIXTURE* callParseOn, ::CEEFIT::PTR< ::CEEFIT::CELLADAPTER >& out, const CEEFIT::STRING& aText);

      virtual void ceefit_call_spec WriteToFixtureVar(const CEEFIT::STRING&);
      virtual void ceefit_call_spec ReadFromFixtureVar(CEEFIT::STRING&) const;

      /**
       * This is essentially an alias for Invoke(), but the output is not parsed into a CELLADAPTER (and rather left as a string)
       */
      virtual void ceefit_call_spec ReadFromFixtureVar(STRING& out, PTR<FIXTURE>& targetFixture) const;

      virtual inline bool ceefit_call_spec IsMethod(void) const
      {
        return(true);
      }

      virtual inline bool ceefit_call_spec IsField(void) const
      {
        return(false);
      }

      virtual inline FIXTURE* ceefit_call_spec ToFixture(void) 
      { 
        throw new EXCEPTION("Fit test not a FIXTURE");
      }

      virtual inline int ceefit_call_spec GetParameterCount(void)
      {
        return(0);
      }

      virtual inline void ceefit_call_spec GetParameterAdapter(::CEEFIT::PTR< ::CEEFIT::CELLADAPTER >& out, int index)
      {
        throw new CEEFIT::EXCEPTION("GetParameterAdapter not implemented for this FITTEST");
      }

      virtual inline int ceefit_call_spec GetHashCode(void)
      { 
        throw new CEEFIT::EXCEPTION("Cant get a hash code for methods yet");
      }
    
      virtual inline bool ceefit_call_spec IsEqual(const ::CEEFIT::CELLADAPTER& aCell) const
      { 
        throw new CEEFIT::EXCEPTION("Cant test for equality for methods yet");
      }

    private:
      /**
       * Not implemented.  Do not call.
       */
      ceefit_init_spec FITTESTBASE(const FITTESTBASE&);

      /**
       * Not implemented.  Do not call.
       */
      FITTESTBASE& ceefit_call_spec operator=(const FITTESTBASE&);
  };
};

/**
 * CeeFIT test registration used by macros
 */
template<class T, class TESTCALLCLASS> class FITTEST_AUTO : public ::CEEFIT::FITTESTBASE
{
  public:
    inline ceefit_init_spec FITTEST_AUTO<T, TESTCALLCLASS>(void)
    {
      CEEFIT::RUNNER::RegisterAutoTest(this);
    }

    virtual inline ceefit_dtor_spec ~FITTEST_AUTO<T, TESTCALLCLASS>(void)
    {
    }

    fit_using_decl(FITTESTBASE::Invoke);
    virtual inline void ceefit_call_spec Invoke(::CEEFIT::PTR< ::CEEFIT::CELLADAPTER >& out, ::CEEFIT::PTR< ::CEEFIT::FIXTURE >& aFixture) const
    {
      FITFIELD<T>* retVal = new FITFIELD<T>();

      try
      {
        retVal->SetName(CEEFIT::STRING("<") + this->GetName() + "() return value>");
        (*retVal) = TESTCALLCLASS::CallFitTest(aFixture);
      }
      catch(CEEFIT::FAILURE* failure)
      {
        delete retVal;
        throw failure;
      }
      catch(CEEFIT::EXCEPTION* exception)
      {
        delete retVal;
        throw exception;
      }
      catch(...)
      {
        delete retVal;
        throw;    // rethrow, hopefully ...
      }

      out = retVal;
    }

    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      FITFIELD<T> typeField;

      return(typeField.GetType());
    }

  private:
    /**
     * Not implemented.  Do not call.
     */
    ceefit_init_spec FITTEST_AUTO<T, TESTCALLCLASS>(const FITTEST_AUTO<T, TESTCALLCLASS>&);

    /**
     * Not implemented.  Do not call.
     */
    FITTEST_AUTO<T, TESTCALLCLASS>& ceefit_call_spec operator=(const FITTEST_AUTO<T, TESTCALLCLASS>&);
};

namespace CEEFIT 
{
  template<class FIXTURETYPE> class FIXTURESELECTOR
  {
    private:
      FIXTURESELECTOR<FIXTURETYPE>(void) 
      {
      }

    public:
      ~FIXTURESELECTOR<FIXTURETYPE>(void) 
      {
      }

      static inline FIXTURESELECTOR<FIXTURETYPE>& GetInstance(void) 
      {
        static FIXTURESELECTOR<FIXTURETYPE> INSTANCE;

        return(INSTANCE);
      }
  };

  template<class RETURNTYPE> class METHODCALLER
  {
    private:
      ceefit_init_spec METHODCALLER<RETURNTYPE>(void) 
      {
      }

    public:
      static METHODCALLER<RETURNTYPE>& ceefit_call_spec GetInstance() 
      {
        static METHODCALLER<RETURNTYPE> INSTANCE;

        return(INSTANCE);
      }

      ceefit_dtor_spec ~METHODCALLER<RETURNTYPE>(void) 
      {
      }

      template<class FIXTURETYPE> inline VALUE<CELLADAPTER> ceefit_call_spec InvokeMethod(
                                                                FIXTURESELECTOR<FIXTURETYPE>& fixtureSelector,
                                                                const STRING& testName, 
                                                                PTR<FIXTURE>& aFixture, 
                                                                RETURNTYPE (ceefit_call_spec FIXTURETYPE::*aFuncCall)(void))
      {
        typedef ::FITFIELD<RETURNTYPE> RETFIELDTYPE;
        RETFIELDTYPE* retVal = new RETFIELDTYPE();

        try
        {
          retVal->SetName(STRING("<") + testName + "() return value>");
          FIXTURETYPE* subclassPtr = dynamic_cast<FIXTURETYPE*>(aFixture.GetPointer());

          if(subclassPtr == null)
          {
            throw new EXCEPTION("Unable to cast FIXTURE to subclass type");
          }

          if(aFuncCall != null) 
          {
            (*retVal) = (subclassPtr ->* aFuncCall)();
          }
          else
          {          
            throw new EXCEPTION(STRING("No function pointer set for test:  ") + testName);
          }        
        }
        catch(FAILURE* failure)
        {
          delete retVal;
          throw failure;
        }
        catch(EXCEPTION* exception)
        {
          delete retVal;
          throw exception;
        }
        catch(...)
        {
          delete retVal;
          throw;    // rethrow, hopefully ...
        }

        return(VALUE<CELLADAPTER>(retVal));
      }
  };

  template<> class METHODCALLER<void>
  {
    private:
      ceefit_init_spec METHODCALLER<void>(void) 
      {
      }

    public:
      static METHODCALLER<void>& ceefit_call_spec GetInstance() 
      {
        static METHODCALLER<void> INSTANCE;

        return(INSTANCE);
      }

      ceefit_dtor_spec ~METHODCALLER<void>(void) 
      {
      }

      template<class FIXTURETYPE> inline VALUE<CELLADAPTER> ceefit_call_spec InvokeMethod(
                                                                       FIXTURESELECTOR<FIXTURETYPE>& fixtureSelector,
                                                                       const STRING& testName, 
                                                                       PTR<FIXTURE>& aFixture, 
                                                                       void (ceefit_call_spec FIXTURETYPE::*aFuncCall)(void))
      {
        ::FITFIELD<void>* retVal = new ::FITFIELD<void>();

        try
        {
          retVal->SetName(STRING("<") + testName + "() return value>");
          FIXTURETYPE* subclassPtr = dynamic_cast<FIXTURETYPE*>(aFixture.GetPointer());

          if(subclassPtr == null)
          {
            throw new EXCEPTION("Unable to cast FIXTURE to subclass type");
          }

          if(aFuncCall != null) 
          {
            (subclassPtr ->* aFuncCall)();
          }
          else
          {          
            throw new EXCEPTION(STRING("No function pointer set for test:  ") + testName);
          }        
        }
        catch(FAILURE* failure)
        {
          delete retVal;
          throw failure;
        }
        catch(EXCEPTION* exception)
        {
          delete retVal;
          throw exception;
        }
        catch(...)
        {
          delete retVal;
          throw;    // rethrow, hopefully ...
        }

        return(VALUE<CELLADAPTER>(retVal));
      }
  };

  template<class RETURNTYPE> class ARG_METHODCALLER
  {
    private:
      ceefit_init_spec ARG_METHODCALLER<RETURNTYPE>(void) 
      {
      }

    public:
      static ARG_METHODCALLER<RETURNTYPE>& ceefit_call_spec GetInstance() 
      {
        static ARG_METHODCALLER<RETURNTYPE> INSTANCE;

        return(INSTANCE);
      }

      ceefit_dtor_spec ~ARG_METHODCALLER<RETURNTYPE>(void) 
      {
      }

      template<class FIXTURETYPE, class ARGTYPE> inline VALUE<CELLADAPTER> ceefit_call_spec InvokeMethod(
                                                                                FIXTURESELECTOR<FIXTURETYPE>& fixtureSelector,
                                                                                const STRING& testName, 
                                                                                PTR<FIXTURE>& aFixture, 
                                                                                PTR< ::FITFIELD<ARGTYPE> >& aArg,
                                                                                RETURNTYPE (ceefit_call_spec FIXTURETYPE::*aFuncCall)(ARGTYPE))
      {
        ::FITFIELD<RETURNTYPE>* retVal = new ::FITFIELD<RETURNTYPE>();

        try
        {
          retVal->SetName(STRING("<") + testName + "() return value>");
          FIXTURETYPE* subclassPtr = dynamic_cast<FIXTURETYPE*>(aFixture.GetPointer());

          if(subclassPtr == null)
          {
            throw new EXCEPTION("Unable to cast FIXTURE to subclass type");
          }

          if(aFuncCall != null) 
          {
            (*retVal) = (subclassPtr ->* aFuncCall)(aArg->GetField());
          }
          else
          {          
            throw new EXCEPTION(STRING("No function pointer set for test:  ") + testName);
          }        
        }
        catch(FAILURE* failure)
        {
          delete retVal;
          throw failure;
        }
        catch(EXCEPTION* exception)
        {
          delete retVal;
          throw exception;
        }
        catch(...)
        {
          delete retVal;
          throw;    // rethrow, hopefully ...
        }

        return(VALUE<CELLADAPTER>(retVal));
      }
  };

  template<> class ARG_METHODCALLER<void>
  {
    private:
      ceefit_init_spec ARG_METHODCALLER<void>(void) 
      {
      }

    public:
      static ARG_METHODCALLER<void>& ceefit_call_spec GetInstance() 
      {
        static ARG_METHODCALLER<void> INSTANCE;

        return(INSTANCE);
      }

      ceefit_dtor_spec ~ARG_METHODCALLER<void>(void) 
      {
      }

      template<class FIXTURETYPE, class ARGTYPE> inline VALUE<CELLADAPTER> ceefit_call_spec InvokeMethod(
                                                                                FIXTURESELECTOR<FIXTURETYPE>& fixtureSelector,
                                                                                const STRING& testName, 
                                                                                PTR<FIXTURE>& aFixture, 
                                                                                PTR< ::FITFIELD<ARGTYPE> >& aArg,
                                                                                void (ceefit_call_spec FIXTURETYPE::*aFuncCall)(ARGTYPE))
      {
        ::FITFIELD<void>* retVal = new ::FITFIELD<void>();

        try
        {
          retVal->SetName(STRING("<") + testName + "() return value>");
          FIXTURETYPE* subclassPtr = dynamic_cast<FIXTURETYPE*>(aFixture.GetPointer());

          if(subclassPtr == null)
          {
            throw new EXCEPTION("Unable to cast FIXTURE to subclass type");
          }

          if(aFuncCall != null) 
          {
            (subclassPtr ->* aFuncCall)(aArg->GetField());
          }
          else
          {          
            throw new EXCEPTION(STRING("No function pointer set for test:  ") + testName);
          }        
        }
        catch(FAILURE* failure)
        {
          delete retVal;
          throw failure;
        }
        catch(EXCEPTION* exception)
        {
          delete retVal;
          throw exception;
        }
        catch(...)
        {
          delete retVal;
          throw;    // rethrow, hopefully ...
        }

        return(VALUE<CELLADAPTER>(retVal));
      }
  };
};

template<class FIXTURETYPE, class RETURNTYPE> class FITTEST_MANUAL : public ::CEEFIT::FITTESTBASE
{
  private:
    RETURNTYPE (FIXTURETYPE::*FuncCall)(void);

  public:
    inline ceefit_init_spec FITTEST_MANUAL<FIXTURETYPE, RETURNTYPE>(RETURNTYPE (ceefit_call_spec FIXTURETYPE::*aFuncCall)(void))
    {
      FuncCall = (RETURNTYPE (FIXTURETYPE::*)(void)) aFuncCall;
    }

    virtual inline ceefit_dtor_spec ~FITTEST_MANUAL<FIXTURETYPE, RETURNTYPE>(void)
    {
    }

    fit_using_decl(FITTESTBASE::Invoke);
    virtual inline void ceefit_call_spec Invoke(::CEEFIT::PTR< ::CEEFIT::CELLADAPTER >& out, ::CEEFIT::PTR< ::CEEFIT::FIXTURE >& aFixture) const
    {
      CEEFIT::FIXTURESELECTOR<FIXTURETYPE>& fixtureSelector = CEEFIT::FIXTURESELECTOR<FIXTURETYPE>::GetInstance();
      CEEFIT::METHODCALLER<RETURNTYPE>& methodCaller = CEEFIT::METHODCALLER<RETURNTYPE>::GetInstance();

      out = methodCaller.InvokeMethod(fixtureSelector, this->GetName(), aFixture, (RETURNTYPE (ceefit_call_spec FIXTURETYPE::*)(void)) FuncCall);
    }

    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      FITFIELD<RETURNTYPE> typeField;

      return(typeField.GetType());
    }

  private:
    ceefit_init_spec FITTEST_MANUAL<FIXTURETYPE, RETURNTYPE>(void);
    ceefit_init_spec FITTEST_MANUAL<FIXTURETYPE, RETURNTYPE>(const FITTEST_MANUAL<FIXTURETYPE, RETURNTYPE>&);
};

template<class FIXTURETYPE, class RETURNTYPE, class ARGTYPE> class FITTEST_ARG_MANUAL : public ::CEEFIT::FITTESTBASE
{
  private:
    RETURNTYPE (FIXTURETYPE::*FuncCall)(ARGTYPE);
    mutable ::CEEFIT::PTR< ::FITFIELD< ARGTYPE > > ParamField;

  public:
    inline ceefit_init_spec FITTEST_ARG_MANUAL<FIXTURETYPE, RETURNTYPE, ARGTYPE>(RETURNTYPE (ceefit_call_spec FIXTURETYPE::*aFuncCall)(ARGTYPE))
    {
      ParamField = new FITFIELD<ARGTYPE>();
      FuncCall = (RETURNTYPE (FIXTURETYPE::*)(ARGTYPE)) aFuncCall;
    }

    virtual inline ceefit_dtor_spec ~FITTEST_ARG_MANUAL<FIXTURETYPE, RETURNTYPE, ARGTYPE>(void)
    {
    }

    virtual inline int ceefit_call_spec GetParameterCount(void)
    {
      return(1);
    }

    virtual inline void ceefit_call_spec GetParameterAdapter(::CEEFIT::PTR< ::CEEFIT::CELLADAPTER >& out, int paramIndex)
    {
      if(paramIndex != 0)
      {
        throw new CEEFIT::BOUNDSEXCEPTION("Only one parameter available");
      }

      out = ParamField.GetPointer();
    }

    virtual inline void ceefit_call_spec Invoke(::CEEFIT::PTR< ::CEEFIT::CELLADAPTER >& out, ::CEEFIT::PTR< ::CEEFIT::FIXTURE >& aFixture) const
    {
      CEEFIT::FIXTURESELECTOR<FIXTURETYPE>& fixtureSelector = CEEFIT::FIXTURESELECTOR<FIXTURETYPE>::GetInstance();
      CEEFIT::ARG_METHODCALLER<RETURNTYPE>& methodCaller = CEEFIT::ARG_METHODCALLER<RETURNTYPE>::GetInstance();

      out = methodCaller.InvokeMethod(fixtureSelector, this->GetName(), aFixture, ParamField, (RETURNTYPE (ceefit_call_spec FIXTURETYPE::*)(ARGTYPE)) FuncCall);
    }

    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      FITFIELD<RETURNTYPE> typeField;

      return(typeField.GetType());
    }

  private:
    ceefit_init_spec FITTEST_ARG_MANUAL<FIXTURETYPE, RETURNTYPE, ARGTYPE>(void);
    ceefit_init_spec FITTEST_ARG_MANUAL<FIXTURETYPE, RETURNTYPE, ARGTYPE>(const FITTEST_ARG_MANUAL<FIXTURETYPE, RETURNTYPE, ARGTYPE>&);
};

namespace CEEFIT
{
  void ceefit_call_spec LinkManualTest(::CEEFIT::FIXTURE* aFixture, FITTESTBASE* fittestManual);
};

/**
 * <p>Manually register a CeeFIT test with the FIXTURE.</p>
 */
template<class FIXTURETYPE, class RETURNTYPE> void ceefit_call_spec RegisterCeefitTest(FIXTURETYPE* fixture, const char* testName, RETURNTYPE (ceefit_call_spec FIXTURETYPE::*testFunc)(void))
{
  FITTEST_MANUAL<FIXTURETYPE, RETURNTYPE>* manualTestRegister = new FITTEST_MANUAL<FIXTURETYPE, RETURNTYPE>(testFunc);

  manualTestRegister->SetName(::CEEFIT::STRING(testName));

  ::CEEFIT::LinkManualTest(fixture, manualTestRegister);
}

/**
 * <p>Manually register a CeeFIT test with the FIXTURE that takes one argument.</p>
 */
template<class FIXTURETYPE, class RETURNTYPE, class ARGTYPE> void ceefit_call_spec RegisterCeefitTest(FIXTURETYPE* fixture, const char* testName, RETURNTYPE (ceefit_call_spec FIXTURETYPE::*testFunc)(ARGTYPE))
{
  FITTEST_ARG_MANUAL<FIXTURETYPE, RETURNTYPE, ARGTYPE>* manualTestRegister = new FITTEST_ARG_MANUAL<FIXTURETYPE, RETURNTYPE, ARGTYPE>(testFunc);

  manualTestRegister->SetName(::CEEFIT::STRING(testName));

  ::CEEFIT::LinkManualTest(fixture, manualTestRegister);
}

#endif // __CEEFIT_FITTEST_H__
