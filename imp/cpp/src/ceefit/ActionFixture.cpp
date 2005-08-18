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
  PTR<FIXTURE> ACTIONFIXTURE::Actor;

  void ceefit_call_spec ACTIONFIXTURE::DoCells(PTR<PARSE>& cells) 
  {
    Cells = cells;
    try 
    {
      PTR<FIXTURE> whichFixture;
      PTR<CELLADAPTER> aAction(FindMethod(whichFixture, Cells->Text()));

      if(aAction != null)
      {
        PTR<CELLADAPTER> result;

        aAction->Invoke(result, whichFixture);
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
  	FIXTURE::LoadFixture(Actor, Cells->More->Text());
  }

  void ceefit_call_spec ACTIONFIXTURE::Enter() 
  {
    PTR<FIXTURE> targetFixture;
    PTR<CELLADAPTER> aMethod(Method(targetFixture, 1));
    PTR<CELLADAPTER> aParamAdapter;

    dynamic_cast<FITTESTBASE*>(aMethod.GetPointer())->GetParameterAdapter(aParamAdapter, 0);

    STRING text(Cells->More->More->Text());
    this->Parse(aParamAdapter, text);

    PTR<CELLADAPTER> result;
    aMethod->Invoke(result, targetFixture);   // the arg is stored internal to aMethod and will be passed to the member function of Actor
  }

  void ceefit_call_spec ACTIONFIXTURE::Press() 
  {
    PTR<FIXTURE> targetFixture;
    PTR<CELLADAPTER> result;
    PTR<CELLADAPTER> methodPtr(Method(targetFixture, 0));
    
    methodPtr->Invoke(result, targetFixture);
  }

  void ceefit_call_spec ACTIONFIXTURE::ActionCheck() 
  {
    PTR<FIXTURE> targetFixture;
    PTR<CELLADAPTER> result;
    PTR<CELLADAPTER> methodPtr(Method(targetFixture, 0));

    methodPtr->Invoke(result, targetFixture);
    this->FIXTURE::Check(Cells->More->More, result);
  }

  // Utility //////////////////////////////////

  VALUE<CELLADAPTER> ceefit_call_spec ACTIONFIXTURE::Method(PTR<FIXTURE>& targetFixture, int args) 
  {
    return Method(targetFixture, Camel(Cells->More->Text()), args);
  }

  VALUE<CELLADAPTER> ceefit_call_spec ACTIONFIXTURE::Method(PTR<FIXTURE>& targetFixture, const STRING& test, int args) 
  {
    VALUE<CELLADAPTER> retVal(Actor->FindMethod(targetFixture, test, args, true));

    if(retVal == null)
    {
      throw new EXCEPTION("no such method exception");
    }
    
    return(retVal);
  }

  ceefit_init_spec ACTIONFIXTURE::ACTIONFIXTURE()
  {    
    RegisterCeefitTest(this, "start", &ACTIONFIXTURE::Start);
    RegisterCeefitTest(this, "enter", &ACTIONFIXTURE::Enter);
    RegisterCeefitTest(this, "press", &ACTIONFIXTURE::Press);
    RegisterCeefitTest(this, "check", &ACTIONFIXTURE::ActionCheck);
  }

  ceefit_init_spec ACTIONFIXTURE::~ACTIONFIXTURE()
  {
  }

  void ceefit_call_spec ACTIONFIXTURE::ReleaseStatics() 
  {
    Actor = null;

    this->FIXTURE::ReleaseStatics();
  }
  
  static ::CEEFIT::REGISTERFIXTURECLASS< ACTIONFIXTURE > ActionFixtureRegistration("CEEFIT::ACTIONFIXTURE", "fit.ActionFixture");
};
