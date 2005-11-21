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
  fitINT64 ceefit_call_spec CurrentTimeMillis(void)
  {
    static double clocksPerMsec = ((double) CLOCKS_PER_SEC) / 1000.0;

    double temp = clock();
    temp = temp / clocksPerMsec;

    return((fitINT64) clock());
  }

  ceefit_init_spec COUNTS::COUNTS()
  {
    right = 0;
    wrong = 0;
    ignores = 0;
    exceptions = 0;
  }

  ceefit_dtor_spec COUNTS::~COUNTS()
  {
  }

  STRING ceefit_call_spec COUNTS::ToString() const
  {
    STRING out;
    SafeSprintf(out, L"%i right, %i wrong, %i ignored, %i exceptions\n", right, wrong, ignores, exceptions);

    return(out);
  }

  void ceefit_call_spec COUNTS::Tally(const COUNTS& source)
  {
    right += source.right;
    wrong += source.wrong;
    ignores += source.ignores;
    exceptions += source.exceptions;
  }

  STRING ceefit_call_spec FIXTURE::RUNTIME::ToString() const
  {
    elapsed = (CurrentTimeMillis()-start);

    if (elapsed > 600000)
    {
      STRING out(d(3600000)+":");
      out += d(600000);
      out += d(60000)+":";
      out += d(10000);
      out += d(1000);

      return(out);
    }
    else
    {
      STRING out(d(60000)+":");
      out += d(10000);
      out += d(1000)+".";
      out += d(100);
      out += d(10);

      return(out);
    }
  }

  STRING ceefit_call_spec FIXTURE::RUNTIME::d(fitINT64 scale) const
  {
    fitINT64 report = elapsed / scale;
    elapsed -= report * scale;

    STRING out;
    SafeSprintf(out, L"%lld", report);

    return out;
  }

  ceefit_init_spec FIXTURE::RUNTIME::RUNTIME()
  {
    start = CurrentTimeMillis();
    elapsed = 0;
  }

  ceefit_dtor_spec FIXTURE::RUNTIME::~RUNTIME()
  {
  }

  ceefit_init_spec FIXTURE::SUMMARY::SUMMARY()
  {
    memset(&RunDate, 0, sizeof(struct timeb));
  }

  ceefit_dtor_spec FIXTURE::SUMMARY::~SUMMARY()
  {
  }

  FIXTURE::SUMMARY& ceefit_call_spec FIXTURE::SUMMARY::operator=(FIXTURE::SUMMARY& aSummary)
  {
    InputFile = aSummary.InputFile;
    InputUpdate = aSummary.InputUpdate;
    OutputFile = aSummary.OutputFile;
    memcpy(&RunDate, &aSummary.RunDate, sizeof(struct timeb));
    RunElapsedTime = aSummary.RunElapsedTime;

    return(*this);
  }

  ceefit_init_spec FIXTURE::SUMMARY::SUMMARY(FIXTURE::SUMMARY& aSummary)
  {
    (*this) = aSummary;
  }

  void ceefit_call_spec FIXTURE::GetSummaryReport(DYNARRAY<SUMMARYITEM>& reportList)
  {
    SUMMARYITEM aItem;

    aItem.Title = "Counts";
    aItem.Value = CountsObj->ToString();
    reportList.Add(aItem);

    aItem.Title = "Input file";
    aItem.Value = SummaryObj->InputFile;
    reportList.Add(aItem);

    wchar_t timeChar[128];
    timeChar[0] = L'\0';
    wcsftime(timeChar, 128, L"%a %b %d %H:%M:%S %X %Y", gmtime(&SummaryObj->InputUpdate));

    aItem.Title = "Input last updated";
    aItem.Value = timeChar;
    reportList.Add(aItem);

    aItem.Title = "Output file";
    aItem.Value = SummaryObj->OutputFile;
    reportList.Add(aItem);

    timeChar[0] = L'\0';
    wcsftime(timeChar, 128, L"%a %b %d %H:%M:%S %X %Y", gmtime(&SummaryObj->RunDate.time));

    aItem.Title = "Run date";
    aItem.Value = timeChar;
    reportList.Add(aItem);

    if(SummaryObj->RunElapsedTime != null)
    {
      aItem.Title = "Run elapsed time";
      aItem.Value = SummaryObj->RunElapsedTime->ToString();
      reportList.Add(aItem);
    }

    if(SummaryObj->CountsRun != null)
    {
      aItem.Title = "counts run";
      aItem.Value = SummaryObj->CountsRun->ToString();
      reportList.Add(aItem);
    }
  }

  ceefit_init_spec FIXTURE::FIXTURE(void)
  {
    SummaryObj = new SUMMARY();
    CountsObj = new COUNTS();

    RUNNER::SetCurrentTestList(&TestList);
    RUNNER::SetCurrentFieldList(&FieldList);
    RUNNER::SetCurrentFixtureUnderConstruction(this);
  }

  // this should never be called
  ceefit_init_spec FIXTURE::FIXTURE(const FIXTURE&)
  {
    assert(false);
  }

  // this should never be called
  FIXTURE& ceefit_call_spec FIXTURE::operator=(const FIXTURE&)
  {
    assert(false);
    return(*this);
  }

  ceefit_dtor_spec FIXTURE::~FIXTURE(void)
  {
    SummaryObj = (SUMMARY*) null;
    CountsObj = (COUNTS*) null;
  }

  void ceefit_call_spec FIXTURE::DeleteFixture(EXCEPTION*& exceptionThrown)
  {
  }

  // Traversal //////////////////////////

  /* Altered by Rick Mugridge to dispatch on the first Fixture, adapted for CeeFIT by Dave Woldrich 05/01/05 */
  void ceefit_call_spec FIXTURE::DoTables(PTR<PARSE>& tables)
  {
    PTR<FIXTURE> fixture;

    ftime(&SummaryObj->RunDate);
    SummaryObj->RunElapsedTime = new FIXTURE::RUNTIME();
    if(tables != null)
    {
      PTR<PARSE> fixtureName(FixtureName(tables));
      if(fixtureName != null)
      {
        try
        {
          GetLinkedFixtureWithArgs(fixture, tables);

          fixture->InterpretTables(tables);

          EXCEPTION* exceptionThrown = null;
          fixture->DeleteFixture(exceptionThrown);
          if(exceptionThrown)
          {
            // rethrow any exception that was thrown ...
            throw exceptionThrown;
          }
        }
        catch (EXCEPTION* e)
        {
          Exception(fixtureName, e);
          InterpretFollowingTables(tables);
        }
      }
    }
  }

  /* Added by Rick Mugridge to allow a dispatch into DoFixture, adapted for CeeFIT by Dave Woldrich 05/01/05 */
  void ceefit_call_spec FIXTURE::InterpretTables(PTR<PARSE>& tables)
  {
  	try
    { // Don't create the first fixture again, because creation may do something important.  (Uhhh ... riiight.  Wait, huh??)
  		GetArgsForTable(tables); // get them again for the new fixture object
  		DoTable(tables);
  	}
    catch(EXCEPTION* ex)
    {
      PTR<PARSE> temp(FixtureName(tables));

  		Exception(temp, ex);
  		return;
  	}

  	InterpretFollowingTables(tables);
  }

  /* Added by Rick Mugridge, adapted for CeeFIT by Dave Woldrich 05/01/05 */
  void ceefit_call_spec FIXTURE::InterpretFollowingTables(PTR<PARSE>& tables)
  {
    PTR<FIXTURE> fixture;
    PTR<PARSE> temp(tables);

    temp = temp->More;
    while(temp != null)
    {
      PTR<PARSE> fixtureName(FixtureName(temp));
      if (fixtureName != null)
      {
        EXCEPTION* exceptionThrown = null;
        try
        {
          // Step 1:  Make sure our PTR is null, just to ensure any old FIXTURE is released
          //          and get next FIXTURE
          fixture = null;
          GetLinkedFixtureWithArgs(fixture, temp);

          // Step 2:  Call DoTable
          if(exceptionThrown == null)
          {
            try
            {
              fixture->DoTable(temp);
            }
            catch(EXCEPTION* e)
            {
              exceptionThrown = e;
            }
          }

          // Step 3:  delete the fixture
          fixture->DeleteFixture(exceptionThrown);

          if(exceptionThrown)
          {
            // rethrow any exception that was thrown ...
            throw exceptionThrown;
          }
        }
        catch (EXCEPTION* e)
        {
          Exception(fixtureName, e);
        }
      }

      temp = temp->More;
    }
  }

    /* Added from FitNesse, adapted for CeeFIT by Dave Woldrich 05/01/05 */
	void ceefit_call_spec FIXTURE::GetLinkedFixtureWithArgs(PTR<FIXTURE>& out, PTR<PARSE>& tables)
  {
		PTR<PARSE> header(tables->At(0, 0, 0));

    LoadFixture(out, header->Text());

    // Step 1:  accumulate fixture objects
		out->CountsObj = CountsObj;
		out->SummaryObj = SummaryObj;

    // Step 1a:  uhhh... what ees thees?
    out->GetArgsForTable(tables);
	}

	/* Added by Rick Mugridge?  Adapted for CeeFIT by Dave Woldrich 05/01/05 */
  VALUE<PARSE> ceefit_call_spec FIXTURE::FixtureName(PTR<PARSE>& tables)
  {
		return(tables->At(0, 0, 0));
	}

	/* Added by Rick Mugridge, from FitNesse, adapted for CeeFIT by Dave Woldrich 05/01/05 */
	void ceefit_call_spec FIXTURE::GetArgsForTable(PTR<PARSE>& table)
  {
    Args.Reset();

    PTR<PARSE> parameters(table->Parts->Parts->More);
    for (; parameters != null; parameters = parameters->More)
    {
      Args.Add(parameters->Text());
    }
	}

	/* Added by Rick, from FitNesse */
  DYNARRAY<STRING>& ceefit_call_spec FIXTURE::GetArgs()
  {
    return Args;
  }

  void ceefit_call_spec FIXTURE::CreateFixtureByClassName(PTR<FIXTURE>& out, const STRING& className)
  {
    SLINKLIST<FIXTUREFACTORY>& aList = RUNNER::GetFixtureFactoryList();

    FIXTUREFACTORY* aFactory = aList.GetHead();
    while(aFactory != null)
    {
      const char* aName = aFactory->GetName();
      if(aName != null && className.IsEqual(aName))
      {
        aFactory->CreateFixture(out);
        return;
      }
      aFactory = aFactory->GetNext();
    }

    out = null;
  }

  void ceefit_call_spec FIXTURE::CreateFixtureByAlias(PTR<FIXTURE>& out, const STRING& aAlias)
  {
    SLINKLIST<FIXTUREFACTORY>& aList = RUNNER::GetFixtureFactoryList();

    FIXTUREFACTORY* aFactory = aList.GetHead();
    while(aFactory != null)
    {
      const char* bAlias = aFactory->GetAlias();
      if(bAlias != null && aAlias.IsEqual(bAlias))
      {
        aFactory->CreateFixture(out);
        return;
      }
      aFactory = aFactory->GetNext();
    }

    out = null;
  }

	void ceefit_call_spec FIXTURE::LoadFixture(PTR<FIXTURE>& out, const STRING& fixtureName)
  {
    FIXTURE::CreateFixtureByClassName(out, fixtureName);

    if(out == null)
    {
      // ok, now see if the fixture name matches any known aliases
      FIXTURE::CreateFixtureByAlias(out, fixtureName);

      if(out == null)
      {
        NONFIXTUREFACTORY* nonFixtureFactoryByName = RUNNER::FindNonFixtureFactoryByName(fixtureName);
        NONFIXTUREFACTORY* nonFixtureFactoryByAlias = RUNNER::FindNonFixtureFactoryByAlias(fixtureName);

        if(nonFixtureFactoryByName || nonFixtureFactoryByAlias)
        {
          throw new CLASSNOTFOUNDEXCEPTION(STRING("\"") + fixtureName + "\" was found, but it's not a fixture.");
        }
        else
        {
          throw new CLASSNOTFOUNDEXCEPTION(STRING("The fixture \"") + fixtureName + "\" was not found.");
        }
      }
    }
	}

  void ceefit_call_spec FIXTURE::DoTable(PTR<PARSE>& table)
  {
    DoRows(table->Parts->More);
  }

  void ceefit_call_spec FIXTURE::DoRows(PTR<PARSE>& rows)
  {
    PTR<PARSE> temp(rows);

    while (temp != null)
    {
      PTR<PARSE> more(temp->More);
      DoRow(temp);
      temp = more;
    }
  }

  void ceefit_call_spec FIXTURE::DoRow(PTR<PARSE>& row)
  {
    DoCells(row->Parts);
  }

  void ceefit_call_spec FIXTURE::DoCells(PTR<PARSE>& cells)
  {
    PTR<PARSE> temp(cells);

    for (int i=0; temp != null; i++)
    {
      try
      {
        DoCell(temp, i);
      }
      catch (EXCEPTION* e)
      {
        Exception(temp, e);
      }
      temp = temp->More;
    }
  }

  void ceefit_call_spec FIXTURE::DoCell(PTR<PARSE>& cell, int columnNumber)
  {
    Ignore(cell);
  }


  // Annotation ///////////////////////////////

  const char* FIXTURE::green = "#cfffcf";
  const char* FIXTURE::red = "#ffcfcf" ;
  const char* FIXTURE::gray = "#efefef";
  const char* FIXTURE::yellow = "#ffffcf";

  void ceefit_call_spec FIXTURE::Right(PTR<PARSE>& cell)
  {
    cell->AddToTag(STRING(" bgcolor=\"") + green + "\"");
    CountsObj->right++;
  }

  void ceefit_call_spec FIXTURE::Wrong(PTR<PARSE>& cell)
  {
    cell->AddToTag(STRING(" bgcolor=\"") + red + "\"");
		cell->Body = Escape(cell->Text());
    CountsObj->wrong++;
  }

  void ceefit_call_spec FIXTURE::Wrong(PTR<PARSE>& cell, const STRING& actual)
  {
    Wrong(cell);
    cell->AddToBody(Label("expected") + "<hr>" + this->Escape(actual) + Label("actual"));
  }

  void ceefit_call_spec FIXTURE::Info(PTR<PARSE>& cell, const STRING& message)
  {
		cell->AddToBody(Info(message));
	}

  STRING ceefit_call_spec FIXTURE::Info(const STRING& message)
  {
		return(STRING(" <font color=\"#808080\">") + Escape(message) + "</font>");
	}

  void ceefit_call_spec FIXTURE::Ignore(PTR<PARSE>& cell)
  {
    cell->AddToTag(STRING(" bgcolor=\"") + gray + "\"");
    CountsObj->ignores++;
  }

	void ceefit_call_spec FIXTURE::Error(PTR<PARSE>& cell, const STRING& message)
  {
		cell->Body = Escape(cell->Text());
		cell->AddToBody(STRING("<hr><pre>") + Escape(message) + "</pre>");
    cell->AddToTag(STRING(" bgcolor=\"") + FIXTURE::yellow + "\"");

    CountsObj->exceptions++;
	}

  void ceefit_call_spec FIXTURE::Exception(PTR<PARSE>& cell, EXCEPTION* e)
  {
    STRING message;

    message = STRING("An unhandled exception occurred:  ") + (e != null ? e->GetReason() : STRING("<unknown reason>"));
#   ifdef _DEBUG
      printf("%S\n", message.GetBuffer());
#   endif

    Error(cell, message);

    delete e;
  }

  // special form of exception for CeeFIT system failures
  void ceefit_call_spec FIXTURE::Failure(PTR<PARSE>& cell, FAILURE* f)
  {
    STRING message;

    message = STRING("A failure occurred:  ") + (f != null ? f->GetReason() : STRING("<unknown reason>"));
#   ifdef _DEBUG
      printf("%S\n", message.GetBuffer());
#   endif

    Error(cell, message);

    delete f;
  }

  // Utility //////////////////////////////////

  STRING ceefit_call_spec FIXTURE::Counts()
  {
    return CountsObj->ToString();
  }

  STRING ceefit_call_spec FIXTURE::Label(const STRING& string)
  {
    return (STRING(" <font size=-1 color=\"#c08080\"><i>") + string + "</i></font>");
  }

  STRING ceefit_call_spec FIXTURE::Escape(const STRING& string)
  {
    STRING temp(string);

    temp = temp.SimplePatternReplaceAll("&", "&amp;");
    temp = temp.SimplePatternReplaceAll("<", "&lt;");
    temp = temp.SimplePatternReplaceAll("  ", " &nbsp;");
		temp = temp.SimplePatternReplaceAll("\r\n", "<br />");
		temp = temp.SimplePatternReplaceAll("\r", "<br />");
		temp = temp.SimplePatternReplaceAll("\n", "<br />");

    return temp;
  }

  STRING ceefit_call_spec FIXTURE::Camel(const STRING& name)
  {
    DYNARRAY<STRING> argList;

    Tokenize(argList, name, STRING(" "), false);

    int i = 0;
    STRING b;
    b += argList[i++];
    while (i < argList.GetSize())
    {
      const STRING& token = argList[i++];
      b += token.Substring(0, 1).ToUppercase();      // replace spaces with camelCase
      b += token.Substring(1);
    }
    return b;
  }

  void ceefit_call_spec FIXTURE::Parse(PTR<CELLADAPTER>& aField, const STRING& s)
  {
    aField->WriteToFixtureVar(s);
  }

  // This method differs from the Java version in that we must go towards STRING for value comparisons versus using
  // TypeAdapter.equals() which works on Object's.  Here the CELLADAPTER is used to parse and filter values as well
  // as convey return data from method invocations.
  void ceefit_call_spec FIXTURE::Check(PTR<PARSE>& cell, PTR<CELLADAPTER>& aAdapter, PTR<FIXTURE>& target)
  {
      PTR<FIXTURE> targetPtr(target);
      if(targetPtr == null)
      {
        targetPtr = this;      // set the default fixture target to this...
      }

      STRING text(cell->Text());

      if(aAdapter == null)
      {
        Ignore(cell);
      }
      else if(text.IsEqual(""))
      {
        try
        {
          STRING aTemp;

          if(aAdapter->IsMethod())
          {
            PTR<CELLADAPTER> result;

            aAdapter->Invoke(result, targetPtr);
            result->ReadFromFixtureVar(aTemp);
          }
          else // if(aAdpater->IsField())
          {
            if(targetPtr == this)
            {
              aAdapter->ReadFromFixtureVar(aTemp);
            }
            else
            {
              aAdapter->ReadFromFixtureVar(aTemp, targetPtr);
            }
          }

          Info(cell, aTemp);
        }
        catch(FAILURE* failure)
        {
          Info(cell, "error");

          delete failure;
        }
        catch(EXCEPTION* e)
        {
          Info(cell, "error");

          delete e;
        }
        catch(...)
        {
          Info(cell, "error");
        }
      }
      else if (text.IsEqual("error"))
      {
        try
        {
          STRING aTemp;
          PTR<CELLADAPTER> aResult;

          aAdapter->Invoke(aResult, targetPtr);

          aResult->ReadFromFixtureVar(aTemp);
          Wrong(cell, aTemp);
        }
        catch(FITFAILED* fitFailed)
        {
          Right(cell);

          delete fitFailed;
        }
        catch(FITASSERTIONFAILED* fitAssertionFailed)
        {
          Right(cell);

          delete fitAssertionFailed;
        }
        catch(EXCEPTION* e)
        {
          Right(cell);

          delete e;
        }
        catch(...)
        {
          Right(cell);
        }
      }
      else
      {
        PTR<CELLADAPTER> callResults;
        PTR<CELLADAPTER> checkValueCell;

        try
        {
          STRING result;
          STRING checkValue;

          if(aAdapter->IsField())
          {
            // The "callResults" is the field we are comparing
            if(targetPtr == this) 
            {
              callResults = aAdapter;
            }
            else
            {
              PTR<FIXTURE> notNeeded;
              callResults = targetPtr->FindField(notNeeded, aAdapter->GetName());

              if(callResults == null) 
              {
                throw new EXCEPTION(STRING("Field not found in FIXTURE:  ") + aAdapter->GetName());
              }
            }

            aAdapter->NewInstanceParse(targetPtr.GetPointer(), checkValueCell, text);
          }
          else // if(aAdapter->IsMethod())
          {
            aAdapter->Invoke(callResults, targetPtr);
            callResults->NewInstanceParse(targetPtr.GetPointer(), checkValueCell, text);
          }

          // checkValueCell must exactly equal result for the test to pass
          if (checkValueCell->IsEqual(*callResults))   // note:  this calls through to CELLCOMPARABLE implementations if available
          {
            Right(cell);
          }
          else
          {
            callResults->ReadFromFixtureVar(result);
            Wrong(cell, result);
          }
        }
        catch (EXCEPTION* e)
        {
          Exception(cell, e);
        }
        catch(...)
        {
          EXCEPTION* systemException = new EXCEPTION("System generated exception or unknown user exception type (exceptions thrown from a FIXTURE class should extend CEEFIT::EXCEPTION.)");

          Exception(cell, systemException);
        }
      }
  }

  void ceefit_call_spec FIXTURE::ReleaseStatics()
  {
  }

  VALUE<FIXTURE> FIXTURE::GetTargetClass()
  {
    return(VALUE<FIXTURE>(this));
  }

  VALUE<CELLADAPTER> ceefit_call_spec FIXTURE::FindMethod(PTR<FIXTURE>& targetFixture, const STRING& name)
  {
    targetFixture = this->GetTargetClass();

    CELLADAPTER* retVal = null;
    PTR<CELLADAPTER> aMethod(targetFixture->TestList.GetHead());
    while(aMethod != null)
    {
      FITTESTBASE* methodPtr = dynamic_cast<FITTESTBASE*>(aMethod.GetPointer());
      if(aMethod->IsMethod() && aMethod->GetName().IsEqual(name) && methodPtr != null && methodPtr->GetParameterCount() == 0)
      {
        retVal = aMethod.GetPointer();
      }
      aMethod = dynamic_cast<SLINK<CELLADAPTER>*>(aMethod.GetPointer())->GetNext();
    }
    
    return(VALUE<CELLADAPTER>(retVal));
  }

  VALUE<CELLADAPTER> ceefit_call_spec FIXTURE::FindMethod(PTR<FIXTURE>& targetFixture, const STRING& methodName, int numArgs, bool errorOnMultipleMatches)
  {
    targetFixture = this->GetTargetClass();

    FITTESTBASE* result = null;
    PTR<CELLADAPTER> aAdapter(targetFixture->TestList.GetHead());

    while(aAdapter != null)
    {
      FITTESTBASE* testPtr = dynamic_cast<FITTESTBASE*>(aAdapter.GetPointer());
      if(aAdapter->IsMethod() && aAdapter->GetName().IsEqual(methodName) && testPtr != null && testPtr->GetParameterCount() == numArgs)
      {
        if(result == null)
        {
          result = testPtr;
        }
        else 
        {
          if(errorOnMultipleMatches)
          {
            throw new EXCEPTION("too many implementations");
          }
          else 
          {
            result = testPtr;
          }
        }
      }
      aAdapter = dynamic_cast<SLINK<CELLADAPTER>*>(aAdapter.GetPointer())->GetNext();
    }

    return(VALUE<CELLADAPTER>(result));
  }

  VALUE<CELLADAPTER> ceefit_call_spec FIXTURE::FindField(PTR<FIXTURE>& targetFixture, const STRING& name)
  {
    targetFixture = this->GetTargetClass();

    CELLADAPTER* retVal = null;
    PTR<CELLADAPTER> aField(targetFixture->FieldList.GetHead());
    while(aField != null)
    {
      if(aField->IsField() && aField->GetName().IsEqual(name))
      {
        retVal = aField.GetPointer();
        // continue on ... if a subclass overrides a subclass field, it will come later ... we want the subclass' field
      }
      aField = dynamic_cast<SLINK<CELLADAPTER>*>(aField.GetPointer())->GetNext();
    }
    
    return(VALUE<CELLADAPTER>(retVal));
  }

  static ::CEEFIT::REGISTERFIXTURECLASS< FIXTURE > FixtureRegistration("CEEFIT::FIXTURE", "fit.Fixture");
};
