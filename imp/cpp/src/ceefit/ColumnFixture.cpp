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

namespace CEEFIT
{
  COLUMNFIXTURE::COLUMNFIXTURE()
  {
    HasExecuted = false;
  }

  COLUMNFIXTURE::~COLUMNFIXTURE()
  {
  }

  // don't call
  COLUMNFIXTURE::COLUMNFIXTURE(COLUMNFIXTURE&)
  {
    throw new EXCEPTION("Unsuppoted constructor");
  }

  // don't call
  COLUMNFIXTURE& COLUMNFIXTURE::operator=(COLUMNFIXTURE& aFixture)
  {
    HasExecuted = aFixture.HasExecuted;
    ColumnBindings = aFixture.ColumnBindings;

    return(*this);
  }

  // Traversal ////////////////////////////////

  void COLUMNFIXTURE::DoRows(PTR<PARSE>& rows)
  {
    Bind(rows->Parts);

    FIXTURE::DoRows(rows->More);
  }

  void COLUMNFIXTURE::DoRow(PTR<PARSE>& row)
  {
    HasExecuted = false;
    try
    {
      Reset();
      FIXTURE::DoRow(row);
      if(!HasExecuted)
      {
        Execute();
      }
    }
    catch(EXCEPTION* e)
    {
      PTR<PARSE> aLeaf(row->Leaf());

      Exception(aLeaf, e);
    }
  }

  void COLUMNFIXTURE::DoCell(PTR<PARSE>& cell, int column)
  {
    PTR<CELLADAPTER> a(ColumnBindings[column]);
    try
    {
      STRING text(cell->Text());
      if(text.IsEqual(""))
      {
        Check(cell, a);
      }
      else if(a == NULL)
      {
        Ignore(cell);
      }
      else if(a->IsField())
      {
        a->WriteToFixtureVar(text);
      }
      else if(a->IsMethod())
      {
        Check(cell, a);
      }
    }
    catch(EXCEPTION* e)
    {
      Exception(cell, e);
    }
  }

  void COLUMNFIXTURE::Check(PTR<PARSE>& cell, PTR<CELLADAPTER>& a, FIXTURE* target)
  {
    if(target == NULL)
    {
      target = this;
    }

    if (!HasExecuted)
    {
      try
      {
        Execute();
      }
      catch(EXCEPTION* e)
      {
        Exception(cell, e);
      }
      HasExecuted = true;
    }
    FIXTURE::Check(cell, a, target);
  }

  void COLUMNFIXTURE::Reset()
  {
      // about to process first cell of row
  }

  void COLUMNFIXTURE::Execute()
  {
      // about to process first method call of row
  }

  // Utility //////////////////////////////////
  void COLUMNFIXTURE::DeleteFixture(EXCEPTION*& exceptionThrown)
  {
    ColumnBindings.Reset();

    this->FIXTURE::DeleteFixture(exceptionThrown);
  }
  
  void COLUMNFIXTURE::Bind(PTR<PARSE>& heads)
  {
    ColumnBindings.Reset();
    ColumnBindings.Reserve(heads->Size());

    PTR<PARSE> headsPtr(heads);
    for (int i=0; headsPtr != NULL; i++, headsPtr = headsPtr->More)
    {
      static STRING suffix("()");

      STRING name(headsPtr->Text());
      try
      {
        if(name.IsEqual(""))
        {
          ColumnBindings[i] = (CELLADAPTER*) NULL;
        }
        else if(name.EndsWith(suffix))
        {
          ColumnBindings[i] = BindMethod(name.Substring(0, name.Length() - suffix.Length()));
        }
        else
        {
          ColumnBindings[i] = BindField(name);
        }
      }
      catch (EXCEPTION* e)
      {
        Exception(heads, e);
      }
    }

  }

  VALUE<CELLADAPTER> ceefit_call_spec COLUMNFIXTURE::BindMethod(const STRING& name)
  {
    PTR<CELLADAPTER> aTest(TestList.GetHead());
    while(aTest != NULL)
    {
      if(aTest->IsMethod() && aTest->GetName().IsEqual(name))
      {
        return(VALUE<FITTESTBASE>(aTest));
      }
      aTest = aTest->GetNext();
    }
    return(VALUE<FITTESTBASE>(NULL));
  }

  VALUE<CELLADAPTER> ceefit_call_spec COLUMNFIXTURE::BindField(const STRING& name)
  {
    PTR<CELLADAPTER> aField(FieldList.GetHead());
    while(aField != NULL)
    {
      if(aField->IsField() && aField->GetName().IsEqual(name))
      {
        return(VALUE<CELLADAPTER>(aField));
      }
      aField = dynamic_cast<SLINK<CELLADAPTER>*>(aField.GetPointer())->GetNext();
    }
    return(VALUE<CELLADAPTER>(NULL));
  }
};
