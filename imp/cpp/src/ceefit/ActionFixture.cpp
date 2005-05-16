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

#include "tools/alloc.h"
#include "ceefit.h"

declare_fit_module(FitActionFixture);

// Traversal ////////////////////////////////

namespace CEEFIT
{
  FIXTURE* ACTIONFIXTURE::Actor = NULL;

  // copied from COLUMNFIXTURE
  VALUE<CELLADAPTER> ceefit_call_spec ACTIONFIXTURE::BindMethod(const STRING& name)
  {
    PTR<CELLADAPTER> aTest(TestList.GetHead());
    while(aTest != NULL)
    {
      if(aTest->IsMethod() && aTest->GetName().IsEqual(name))
      {
        return(VALUE<CELLADAPTER>(aTest));
      }
      aTest = aTest->GetNext();
    }
    return(VALUE<CELLADAPTER>(NULL));
  }

  void ceefit_call_spec ACTIONFIXTURE::DoCells(PTR<PARSE>& cells) 
  {
    Cells = cells;
    try 
    {
      PTR<CELLADAPTER> aAction(BindMethod(Cells->Text()));

      if(aAction != NULL)
      {
        PTR<CELLADAPTER> result;

        aAction->Invoke(result, this);
      }
      else 
      {
        throw new EXCEPTION(STRING("Action method not found:  ") + Cells->Text());
      }
    } 
    catch(EXCEPTION* e) 
    {
      Exception(cells, e);
    }
  }

  // Actions //////////////////////////////////

  void ceefit_call_spec ACTIONFIXTURE::Start() 
  {
    delete Actor;
    Actor = NULL;

  	Actor = FIXTURE::LoadFixture(Cells->More->Text());
  }

  void ceefit_call_spec ACTIONFIXTURE::Enter() 
  {
    PTR<CELLADAPTER> aMethod(Method(1));
    PTR<CELLADAPTER> aParamAdapter;

    dynamic_cast<FITTESTBASE*>(aMethod.GetPointer())->GetParameterAdapter(aParamAdapter, 0);

    STRING text(Cells->More->More->Text());
    aParamAdapter->WriteToFixtureVar(text);

    PTR<CELLADAPTER> result;
    aMethod->Invoke(result, Actor);   // the arg is stored internal to aMethod and will be passed to the member function of Actor
  }

  void ceefit_call_spec ACTIONFIXTURE::Press() 
  {
    PTR<CELLADAPTER> result;
    Method(0)->Invoke(result, Actor);
  }

  void ceefit_call_spec ACTIONFIXTURE::Check() 
  {
    PTR<CELLADAPTER> result;

    Method(0)->Invoke(result, Actor);
    FIXTURE::Check(Cells->More->More, result);
  }

  // Utility //////////////////////////////////

  CELLADAPTER* ceefit_call_spec ACTIONFIXTURE::Method(int args) 
  {
    return Method(Camel(Cells->More->Text()), args);
  }

  CELLADAPTER* ceefit_call_spec ACTIONFIXTURE::Method(const STRING& test, int args) 
  {
    SLINKLIST<CELLADAPTER>& testList = Actor->TestList;
    FITTESTBASE* result = NULL;
    PTR<CELLADAPTER> aAdapter(testList.GetHead());

    while(aAdapter != NULL)
    {
      FITTESTBASE* testPtr = dynamic_cast<FITTESTBASE*>(aAdapter.GetPointer());
      if(aAdapter->GetName().IsEqual(test) && testPtr->GetParameterCount() == args)
      {
        if(result == NULL)
        {
          result = testPtr;
        }
        else 
        {
          throw new EXCEPTION("too many implementations");
        }
      }
      aAdapter = aAdapter->GetNext();
    }

    if(result == NULL)
    {
      throw new EXCEPTION("no such method exception");
    }

    return result;
  }

  ceefit_init_spec ACTIONFIXTURE::ACTIONFIXTURE()
  {    
    Actor = NULL;

    RegisterCeefitTest(this, "start", &ACTIONFIXTURE::Start);
    RegisterCeefitTest(this, "enter", &ACTIONFIXTURE::Enter);
    RegisterCeefitTest(this, "press", &ACTIONFIXTURE::Press);
    RegisterCeefitTest(this, "check", &ACTIONFIXTURE::Check);
  }

  ceefit_init_spec ACTIONFIXTURE::~ACTIONFIXTURE()
  {
    delete Actor;
  }
  
  static ::CEEFIT::REGISTERFIXTURECLASS< ACTIONFIXTURE > ActionFixtureRegistration("ACTIONFIXTURE", "fit.ActionFixture");
};
