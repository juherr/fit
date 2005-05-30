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

#include "ceefit.h"

namespace CEEFIT
{

  ceefit_init_spec FIXTUREFACTORY::FIXTUREFACTORY(const char* aName, const char* aAlias) : Name(aName), Alias(aAlias)
  {
  }

  ceefit_init_spec FIXTUREFACTORY::~FIXTUREFACTORY()
  {
  }

  const char* ceefit_call_spec FIXTUREFACTORY::GetName() const
  {
    return(Name);
  }

  const char* ceefit_call_spec FIXTUREFACTORY::GetAlias() const
  {
    return(Alias);
  }

  ceefit_init_spec NONFIXTUREFACTORY::NONFIXTUREFACTORY(const char* aName, const char* aAlias) : Name(aName), Alias(aAlias)
  {
  }

  ceefit_init_spec NONFIXTUREFACTORY::~NONFIXTUREFACTORY()
  {
  }

  const char* ceefit_call_spec NONFIXTUREFACTORY::GetName() const
  {
    return(Name);
  }

  const char* ceefit_call_spec NONFIXTUREFACTORY::GetAlias() const
  {
    return(Alias);
  }

  SLINKLIST<FIXTUREFACTORY>& ceefit_call_spec RUNNER::GetFixtureFactoryList()
  {
    static SLINKLIST<FIXTUREFACTORY> FixtureFactoryList;

    return(FixtureFactoryList);
  }

  SLINKLIST<NONFIXTUREFACTORY>& ceefit_call_spec RUNNER::GetNonFixtureFactoryList()
  {
    static SLINKLIST<NONFIXTUREFACTORY> NonFixtureFactoryList;

    return(NonFixtureFactoryList);
  }

  int RUNNER::InFixtureConstructorCount = 0;

  void ceefit_call_spec RUNNER::IncInFixtureConstructor(void)
  {
    InFixtureConstructorCount++;
  }

  void ceefit_call_spec RUNNER::DecInFixtureConstructor(void)
  {
    InFixtureConstructorCount--;
  }

  bool ceefit_call_spec RUNNER::InFixtureConstructor(void)
  {
    return(InFixtureConstructorCount > 0);
  }

  void ceefit_call_spec RUNNER::ResetInFixtureConstructor(void)
  {
    InFixtureConstructorCount = 0;
  }

  ceefit_init_spec RUNNER::RUNNER()
  {
  }

  ceefit_init_spec RUNNER::~RUNNER()
  {
  }

  //Do not call.
  ceefit_init_spec RUNNER::RUNNER(const RUNNER&)
  {
    assert(false);
  }

  //Do not call.
  RUNNER& ceefit_call_spec RUNNER::operator=(const RUNNER&)
  {
    assert(false);
    return(*this);
  }

  FIXTUREFACTORY* ceefit_call_spec RUNNER::GetSetCurrentFixtureFactory(FIXTUREFACTORY* aFactory, bool clearCurrentFactory)
  {
    static FIXTUREFACTORY* CurFactory = NULL;

    if(clearCurrentFactory)
    {
      CurFactory = NULL;

      return(NULL);
    }

    if(aFactory != NULL)
    {
      CurFactory = aFactory;
    }

    if(CurFactory == NULL)
    {
      static REGISTERFIXTURECLASS<FIXTURE> DummyFactory("errorNoCurrentFixtureFound");

      CurFactory = new REGISTERFIXTURECLASS<FIXTURE>("errorNoCurrentFixtureFound");
    }

    return(CurFactory);
  }

  FIXTUREFACTORY* ceefit_call_spec RUNNER::FindFixtureFactoryByName(const char* aName)
  {
    SLINKLIST<FIXTUREFACTORY>& factoryList = GetFixtureFactoryList();

    if(aName == NULL || strlen(aName) == 0)
    {
      return(NULL);
    }

    FIXTUREFACTORY* aFactory = factoryList.GetHead();
    while(aFactory != NULL)
    {
      if(aFactory->GetName() != NULL && !strcmp(aName, aFactory->GetName()))
      {
        return(aFactory);
      }
      aFactory = aFactory->GetNext();
    }

    return(NULL);
  }

  NONFIXTUREFACTORY* ceefit_call_spec RUNNER::FindNonFixtureFactoryByName(const STRING& aName)
  {
    SLINKLIST<NONFIXTUREFACTORY>& nonFactoryList = GetNonFixtureFactoryList();

    if(aName.Length() == 0)
    {
      return(NULL);
    }

    NONFIXTUREFACTORY* aNonFactory = nonFactoryList.GetHead();
    while(aNonFactory != NULL)
    {
      if(aNonFactory->GetName() != NULL && aName.IsEqual(STRING(aNonFactory->GetName())))
      {
        return(aNonFactory);
      }
      aNonFactory = aNonFactory->GetNext();
    }

    return(NULL);
  }

  NONFIXTUREFACTORY* ceefit_call_spec RUNNER::FindNonFixtureFactoryByAlias(const STRING& aAlias)
  {
    SLINKLIST<NONFIXTUREFACTORY>& nonFactoryList = GetNonFixtureFactoryList();

    if(aAlias.Length() == 0)
    {
      return(NULL);
    }

    NONFIXTUREFACTORY* aNonFactory = nonFactoryList.GetHead();
    while(aNonFactory != NULL)
    {
      if(aNonFactory->GetAlias() != NULL && aAlias.IsEqual(STRING(aNonFactory->GetAlias())))
      {
        return(aNonFactory);
      }
      aNonFactory = aNonFactory->GetNext();
    }

    return(NULL);
  }

  void ceefit_call_spec RUNNER::RegisterFixtureFactory(FIXTUREFACTORY* aFixture)
  {
    GetFixtureFactoryList().AddTail(aFixture);
  }

  void ceefit_call_spec RUNNER::RegisterNonFixtureFactory(NONFIXTUREFACTORY* aNonFixture)
  {
    GetNonFixtureFactoryList().AddTail(aNonFixture);
  }

  SLINKLIST<CELLADAPTER>* RUNNER::CurrentTestList = NULL;
  SLINKLIST<CELLADAPTER>* RUNNER::CurrentFieldList = NULL;

  void ceefit_call_spec RUNNER::SetCurrentTestList(SLINKLIST<CELLADAPTER>* aList)
  {
    CurrentTestList = aList;
  }

  SLINKLIST<CELLADAPTER>* ceefit_call_spec RUNNER::GetCurrentTestList(void)
  {
    return(CurrentTestList);
  }

  void ceefit_call_spec RUNNER::SetCurrentFieldList(SLINKLIST<CELLADAPTER>* aList)
  {
    CurrentFieldList = aList;
  }

  SLINKLIST<CELLADAPTER>* ceefit_call_spec RUNNER::GetCurrentFieldList(void)
  {
    return(CurrentFieldList);
  }

  FIXTURE* RUNNER::FixtureUnderConstruction = NULL;

  void ceefit_call_spec RUNNER::SetCurrentFixtureUnderConstruction(FIXTURE* aFixture)
  {
    FixtureUnderConstruction = aFixture;
  }

  FIXTURE* ceefit_call_spec RUNNER::GetCurrentFixtureUnderConstruction()
  {
    return(FixtureUnderConstruction);
  }

  void ceefit_call_spec RUNNER::RegisterAutoTest(CELLADAPTER* aTest)
  {
    SLINKLIST<CELLADAPTER>* aList = RUNNER::GetCurrentTestList();

    if(aList == NULL)
    {
      throw new EXCEPTION("No Current Test List");
    }

    // Todo:  this is insufficient due to base class field names potentially taking precendence
    // over sub class ones.
    aList->AddTail(aTest);
    aTest->AddRef();  // Since the auto test is a member of the containing class, we need to add a ref to guarantee it will never accidentially be dereferenced and deleted
  }

  void ceefit_call_spec RUNNER::RegisterAutoField(CELLADAPTER* aField)
  {
    SLINKLIST<CELLADAPTER>* aList = RUNNER::GetCurrentFieldList();

    if(aList == NULL)
    {
      throw new EXCEPTION("No Current Field List");
    }
    aList->AddTail(aField);
    aField->AddRef();  // Since the auto field is a member of the containing class, we need to add a ref to guarantee it will never accidentially be dereferenced and deleted
  }

  VALUE<CELLADAPTER> ceefit_call_spec RUNNER::GetLastRegisteredField()
  {
    SLINKLIST<CELLADAPTER>* aList = RUNNER::GetCurrentFieldList();

    if(aList == NULL)
    {
      throw new EXCEPTION("No Current Field List");
    }
    return(VALUE<CELLADAPTER>(aList->GetTail()));
  }

  VALUE<CELLADAPTER> ceefit_call_spec RUNNER::GetLastRegisteredTest()
  {
    SLINKLIST<CELLADAPTER>* aList = RUNNER::GetCurrentTestList();

    if(aList == NULL)
    {
      throw new EXCEPTION("No Current Test List");
    }
    return(VALUE<CELLADAPTER>(aList->GetTail()));
  }
};