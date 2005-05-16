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
  static INT64 CurrentTimeMillis(void)
  {
    static double clocksPerMsec = ((double) CLOCKS_PER_SEC) / 1000.0;

    double temp = clock();
    temp = temp / clocksPerMsec;

    return((INT64) clock());
  }

  SLINKLIST<FIXTUREFACTORY>& ceefit_call_spec RUNNER::GetFixtureFactoryList()
  {
    static SLINKLIST<FIXTUREFACTORY> FixtureFactoryList;

    return(FixtureFactoryList);
  }

  ceefit_init_spec COUNTS::COUNTS()
  {
    right = 0;
    wrong = 0;
    ignores = 0;
    exceptions = 0;
  }

  ceefit_init_spec COUNTS::~COUNTS()
  {
  }

  STRING ceefit_call_spec COUNTS::ToString()
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

  STRING ceefit_call_spec FIXTURE::RUNTIME::ToString()
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

  STRING ceefit_call_spec FIXTURE::RUNTIME::d(INT64 scale)
  {
    INT64 report = elapsed / scale;
    elapsed -= report * scale;

    STRING out;
#   ifdef _MSC_VER
      SafeSprintf(out, L"%I64i", report);
#   else
      SafeSprintf(out, L"%lld", report);
#   endif

    return out;
  }

  ceefit_init_spec FIXTURE::RUNTIME::RUNTIME()
  {
    start = CurrentTimeMillis();
    elapsed = 0;
  }

  ceefit_init_spec FIXTURE::RUNTIME::~RUNTIME()
  {
  }

  ceefit_init_spec FIXTURE::SUMMARY::SUMMARY()
  {
    memset(&RunDate, 0, sizeof(struct _timeb));
  }

  ceefit_init_spec FIXTURE::SUMMARY::~SUMMARY()
  {
  }

  FIXTURE::SUMMARY& ceefit_call_spec FIXTURE::SUMMARY::operator=(FIXTURE::SUMMARY& aSummary)
  {
    InputFile = aSummary.InputFile;
    memcpy(&InputUpdate, &aSummary.InputUpdate, sizeof(struct _utimbuf));
    OutputFile = aSummary.OutputFile;
    memcpy(&RunDate, &aSummary.RunDate, sizeof(struct _timeb));
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

    aItem.Title = "Run elapsed time";
    aItem.Value = SummaryObj->RunElapsedTime->ToString();
    reportList.Add(aItem);
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

  ceefit_init_spec FIXTURE::~FIXTURE(void)
  {
    SummaryObj = (SUMMARY*) NULL;
    CountsObj = (COUNTS*) NULL;
  }

  void ceefit_call_spec FIXTURE::DeleteFixture(EXCEPTION*& exceptionThrown)
  {
    try
    {
      delete this;
    }
    catch(EXCEPTION* e)
    {
      if(exceptionThrown == NULL)
      {
        exceptionThrown = e;
      }
      else
      {
        // first exception takes precedence
        delete e;
      }
    }
  }

  // Traversal //////////////////////////
 
  /* Altered by Rick Mugridge to dispatch on the first Fixture, adapted for CeeFIT by Dave Woldrich 05/01/05 */
  void ceefit_call_spec FIXTURE::DoTables(PTR<PARSE>& tables) 
  {
    _ftime(&SummaryObj->RunDate);
    SummaryObj->RunElapsedTime = new FIXTURE::RUNTIME();
    if(tables != NULL) 
    {
      PTR<PARSE> fixtureName(FixtureName(tables));
      if(fixtureName != NULL) 
      {
        try 
        {
          FIXTURE* fixture = GetLinkedFixtureWithArgs(tables);
          
          fixture->InterpretTables(tables);
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
    PTR<PARSE> temp(tables);
    //listener.tableFinished(temp);     // cruftacular!!!

    temp = temp->More;      /// uhhh ... what is this leading ->More for?
    while (temp != NULL) 
    {
      PTR<PARSE> fixtureName(FixtureName(temp));
      if (fixtureName != NULL) 
      {
        EXCEPTION* exceptionThrown = NULL;
        try 
        {
          FIXTURE* fixture = GetLinkedFixtureWithArgs(temp);

          // Step 2:  Call DoTable
          if(exceptionThrown == NULL)
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

      //listener.tableFinished(temp);     // more yummy cruft from java version!
      temp = temp->More;
    }
  }

    /* Added from FitNesse, adapted for CeeFIT by Dave Woldrich 05/01/05 */
	FIXTURE* ceefit_call_spec FIXTURE::GetLinkedFixtureWithArgs(PTR<PARSE>& tables) 
  {
		PTR<PARSE> header(tables->At(0, 0, 0));

    FIXTURE* fixture = LoadFixture(header->Text());

    // Step 1:  accumulate fixture objects
		fixture->CountsObj = CountsObj;
		fixture->SummaryObj = SummaryObj;

    // Step 1a:  uhhh... what ees thees?
    fixture->GetArgsForTable(tables);

		return fixture;
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
    for (; parameters != NULL; parameters = parameters->More)
    {
      Args.Add(parameters->Text());
    }
	}

	/* Added by Rick, from FitNesse */
  DYNARRAY<STRING>& ceefit_call_spec FIXTURE::GetArgs() 
  {
    return Args;
  }

  /* old DoTables
  void ceefit_call_spec FIXTURE::DoTables(PTR<PARSE>& tables)
  {
    _ftime(&SummaryObj->RunDate);
    SummaryObj->RunElapsedTime = new FIXTURE::RUNTIME();

    PTR<PARSE> temp(tables);

    while (temp != NULL)
    {
      PTR<PARSE> heading(temp->At(0,0,0));
      if (heading != NULL)
      {
        try
        {
          EXCEPTION* exceptionThrown = NULL;
          STRING headingText(heading->Text());
          FIXTURE* fixture = LoadFixture(headingText);

          // Step 1:  accumulate fixture Counts
          fixture->CountsObj = CountsObj;

          // Step 2:  Call DoTable
          if(exceptionThrown == NULL)
          {
            try
            {
              fixture->SummaryObj = SummaryObj;  // copy the fixture summary to the new fixture
              fixture->DoTable(temp);
            }
            catch(EXCEPTION* e)
            {
              exceptionThrown = e;
            }
          }

          // Step 3:  delete the fixture
          DeleteFixture(fixture, exceptionThrown);

          if(exceptionThrown)
          {
            // rethrow any exception that was thrown ...
            throw exceptionThrown;
          }
        }
        catch(EXCEPTION* e)
        {
          Exception(heading, e);
        }
      }

      temp = temp->More;
    }
  }
*/

  FIXTURE* ceefit_call_spec FIXTURE::CreateFixtureByClassName(const STRING& className)
  {
    SLINKLIST<FIXTUREFACTORY>& aList = RUNNER::GetFixtureFactoryList();

    FIXTUREFACTORY* aFactory = aList.GetHead();
    while(aFactory != NULL)
    {
      const char* aName = aFactory->GetName();
      if(aName != NULL && className.IsEqual(aName))
      {
        return(aFactory->CreateFixture());
      }
      aFactory = aFactory->GetNext();
    }

    return(NULL);
  }

  FIXTURE* ceefit_call_spec FIXTURE::CreateFixtureByAlias(const STRING& aAlias)
  {
    SLINKLIST<FIXTUREFACTORY>& aList = RUNNER::GetFixtureFactoryList();

    FIXTUREFACTORY* aFactory = aList.GetHead();
    while(aFactory != NULL)
    {
      const char* bAlias = aFactory->GetAlias();
      if(bAlias != NULL && aAlias.IsEqual(bAlias))
      {
        return(aFactory->CreateFixture());
      }
      aFactory = aFactory->GetNext();
    }

    return(NULL);
  }

	FIXTURE* ceefit_call_spec FIXTURE::LoadFixture(const STRING& fixtureName)
  {
    FIXTURE* aFixture = FIXTURE::CreateFixtureByClassName(fixtureName);

    if(aFixture == NULL)
    {
      // ok, now see if the fixture name matches any known aliases
      aFixture = FIXTURE::CreateFixtureByAlias(fixtureName);

      if(aFixture == NULL)
      {
        throw new CLASSNOTFOUNDEXCEPTION(STRING("Fixture class ") + fixtureName + " not found.");
      }
    }

    return(aFixture);
	}

  void ceefit_call_spec FIXTURE::DoTable(PTR<PARSE>& table)
  {
    DoRows(table->Parts->More);
  }

  void ceefit_call_spec FIXTURE::DoRows(PTR<PARSE>& rows)
  {
    PTR<PARSE> temp(rows);

    while (temp != NULL)
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

    for (int i=0; temp != NULL; i++)
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

    message = STRING("An unhandled exception occurred:  ") + (e != NULL ? e->GetReason() : "<unknown reason>");
    wprintf(L"%s\n", message.GetBuffer());

    Error(cell, message);

    delete e;
  }

  // special form of exception for CeeFIT system failures
  void ceefit_call_spec FIXTURE::Failure(PTR<PARSE>& cell, FAILURE* f)
  {
    STRING message;

    message = STRING("A failure occurred:  ") + (f != NULL ? f->GetReason() : "<unknown reason>");
    wprintf(L"%s\n", message.GetBuffer());

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
    return (STRING(" <font size=-1 color=#c08080><i>") + string + "</i></font>");
  }

  STRING ceefit_call_spec FIXTURE::Gray(const STRING& string)
  {
    return (STRING(" <font color=#808080>") + string + "</font>");
  }

  STRING ceefit_call_spec FIXTURE::Escape(const STRING& string)
  {
    STRING temp(string);

    temp = temp.SimplePatternReplaceAll("&", "&amp;");
    temp = temp.SimplePatternReplaceAll("<", "&lt;");
    temp = temp.SimplePatternReplaceAll("  ", " &nbsp;");
		temp = temp.SimplePatternReplaceAll("\r\n", "<br />");
		temp = temp.SimplePatternReplaceAll("\n\r", "<br />");
		temp = temp.SimplePatternReplaceAll("\r", "<br />");
		temp = temp.SimplePatternReplaceAll("\n", "<br />");

    return temp;
  }

  STRING ceefit_call_spec FIXTURE::Camel(const STRING& name)
  {
    DYNARRAY<STRING> argList;

    TokenizeRespectQuotes(argList, name, STRING(" "), false);

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
  void ceefit_call_spec FIXTURE::Check(PTR<PARSE>& cell, PTR<CELLADAPTER>& aAdapter, FIXTURE* target)
  {
      if(target == NULL)
      {
        target = this;      // set the default fixture target to this...
      }

      STRING text(cell->Text());

      if(aAdapter == NULL)
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

            aAdapter->Invoke(result, target);
            result->ReadFromFixtureVar(aTemp);
          }
          else // if(aAdpater->IsField())
          {
            if(target == this)
            {
              aAdapter->ReadFromFixtureVar(aTemp);
            }
            else
            {
              aAdapter->ReadFromFixtureVar(aTemp, target);
            }
          }

          cell->AddToBody(Gray(aTemp));
        }
        catch(FAILURE* failure)
        {
          cell->AddToBody(Gray(failure->GetReason()));

          delete failure;
        }
        catch(EXCEPTION* e)
        {
          cell->AddToBody(Gray("error"));

          delete e;
        }
        catch(...)
        {
          cell->AddToBody(Gray("error"));
        }
      }
      else if (text.IsEqual("error"))
      {
        try
        {
          STRING aTemp;
          PTR<CELLADAPTER> aResult;

          aAdapter->Invoke(aResult, this);

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
            aAdapter->ReadFromFixtureVar(result);             // result == the value to be compared
            aAdapter->NewInstanceParse(checkValueCell, text);

            checkValueCell->ReadFromFixtureVar(checkValue);   // checkValue == filtered/stringified (check value) cell text
          }
          else // if(aAdapter->IsMethod())
          {
            aAdapter->Invoke(callResults, this);
            callResults->NewInstanceParse(checkValueCell, text);

            // load results and checkValue with the parsed/checked STRING's in the cells
            callResults->ReadFromFixtureVar(result);
            checkValueCell->ReadFromFixtureVar(checkValue);
          }

          // checkValue must exactly equal result for the test to pass
          if (checkValue.IsEqual(result))
          {
            Right(cell);
          }
          else
          {
            Wrong(cell, result);
          }
        }
        catch (EXCEPTION* e)
        {
          Exception(cell, e);
        }
        catch(...)
        {
          EXCEPTION* systemException = new EXCEPTION("System generated exception or unknown user exception type");

          Exception(cell, systemException);
        }
      }
  }

};
