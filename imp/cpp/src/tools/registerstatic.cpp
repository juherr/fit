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
  void ceefit_call_spec CELLADAPTER::Invoke(PTR<CELLADAPTER>& out, CELLADAPTER* fixtureContainer)
  {
    ::FITFIXTURECONTAINER* aContainer = dynamic_cast< ::FITFIXTURECONTAINER* >(fixtureContainer);

    AssertNotNull(aContainer);

    PTR<FIXTURE> targetFixture(aContainer->GetFixture());
    this->Invoke(out, targetFixture);
  }

  // this finds a like named CELLADAPTER in another fixture and reads from it
  void ceefit_call_spec CELLADAPTER::ReadFromFixtureVar(STRING& out, PTR<FIXTURE>& aFixture)
  {    
    if(this->IsField())
    {
      int i = -1;
      int size = aFixture->FieldList.GetSize();
      while(++i < size)
      {
        PTR<CELLADAPTER> aCell(&aFixture->FieldList.Get(i));

        if(::CEEFIT::IsEqual(aCell->GetName(), this->GetName()))
        {
          aCell->ReadFromFixtureVar(out);
          return;
        }
      }
    }
    else
    {
      int i = -1;
      int size = aFixture->TestList.GetSize();
      while(++i < size)
      {
        PTR<CELLADAPTER> aCell(&aFixture->TestList.Get(i));

        if(::CEEFIT::IsEqual(aCell->GetName(), this->GetName()))
        {
          aCell->ReadFromFixtureVar(out);
          return;
        }
      }
    }
  }
};

void ceefit_call_spec FITTESTBASE::WriteToFixtureVar(const CEEFIT::STRING&)
{
  throw new CEEFIT::EXCEPTION("Invalid operation for tests");
}

void ceefit_call_spec FITTESTBASE::ReadFromFixtureVar(CEEFIT::STRING&)
{
  throw new CEEFIT::EXCEPTION("Invalid operation for tests");
}

void ceefit_call_spec FITTESTBASE::NewInstanceParse(::CEEFIT::PTR< ::CEEFIT::CELLADAPTER >& out, const CEEFIT::STRING& aText)
{
  throw new CEEFIT::EXCEPTION("Cannot perform a new instance parse on function-type cells");
}

ceefit_init_spec FITTESTBASE::FITTESTBASE()
{
  Name = "";
}

ceefit_init_spec FITTESTBASE::~FITTESTBASE()
{
}

const CEEFIT::STRING& ceefit_call_spec FITTESTBASE::GetName() const
{
  return(Name);
}

void ceefit_call_spec FITTESTBASE::SetName(const CEEFIT::STRING& testName)
{
  Name = testName;
}

namespace CEEFIT
{
  CEEFIT::FIXTURE* ceefit_call_spec GetFixtureUnderConstruction(void)
  {
    return(CEEFIT::RUNNER::GetCurrentFixtureUnderConstruction());
  }

  void ceefit_call_spec LinkFieldToCurrentFixture(CELLADAPTER* aField)
  {
    if(RUNNER::InFixtureConstructor())
    {
      RUNNER::RegisterAutoField(aField);
    }
  }

  void ceefit_call_spec SetLastLinkedTestName(const char* aName)
  {
    PTR<CELLADAPTER> aTest(RUNNER::GetLastRegisteredTest());

    aTest->SetName(STRING(aName));
  }

  void ceefit_call_spec LinkManualTest(FIXTURE* aFixture, FITTESTBASE* fittestManual)
  {        
    aFixture->TestList.AddTail(fittestManual);
    aFixture->DestroyAtFinish.Add(fittestManual);
  }

  void ceefit_call_spec LinkManualField(FIXTURE* aFixture, CELLADAPTER* fitfieldManual)
  {
    aFixture->FieldList.AddTail(fitfieldManual);
    aFixture->DestroyAtFinish.Add(fitfieldManual);
  }

};

