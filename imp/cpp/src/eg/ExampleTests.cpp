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
#include "eg/eg.h"

declare_fit_module(ExampleTestsFixture);

using namespace CEEFIT;

namespace EG
{
  /* The CeeFIT version of EXAMPLETESTS::Run is taken more from CEEFAT::ReferenceFixture than from Java's eg.ExampleTests */
  /* (but it should do the same thing...) */
  void EXAMPLETESTS::Run() 
  {
    wchar_t inputFileFullPath[fit_MAX_PATH];
    wchar_t drive[fit_MAX_DRIVE];
    wchar_t dir[fit_MAX_DIR];
    wchar_t otherDir[fit_MAX_DIR];
    wchar_t fname[fit_MAX_FNAME];
    wchar_t ext[fit_MAX_EXT];

    fit_wfullpath(inputFileFullPath, SummaryObj->InputFile.GetBuffer(), fit_MAX_PATH);
    fit_wsplitpath(inputFileFullPath, drive, dir, null, null);

		STRING inputFileName(STRING(drive) + dir + File);

    fit_wsplitpath(SummaryObj->OutputFile.GetBuffer(), drive, dir, null, null);
    fit_wsplitpath(File.GetBuffer(), null, otherDir, fname, ext);

    STRING outDirName(drive);
    DYNARRAY<STRING> argList;
    TokenizeRespectQuotes(argList, STRING(dir) + otherDir, STRING("\\/"));

    int i = -1;
    while(++i < argList.GetSize())
    {        
      outDirName += STRING("/") + argList[i];
    }

  	Runner = new FILERUNNER();

    // synthesize an output file path by merging the original output drive/dir with the dir/fname of Location
		STRING outputFileName(outDirName + "/" + fname + ext);
		try 
    {
      DYNARRAY<STRING> args;
      args.Add("ExampleTests");
      args.Add("-CeeFIT");
      args.Add(inputFileName);
      args.Add(outputFileName);

			Runner->Args(args);
			Runner->Process();
			Runner->Exit();
		}
		catch (EXCEPTION* e) 
    {
      RunCounts->Tally(*Runner->Fixture->CountsObj);
      SummaryObj->CountsRun = RunCounts;    

      throw e;
		}

    RunCounts->Tally(*Runner->Fixture->CountsObj);
    SummaryObj->CountsRun = RunCounts;    
  }

  int EXAMPLETESTS::Right() 
  {
    Run();
    return Runner->Fixture->CountsObj->right;
  }

  int EXAMPLETESTS::Wrong() 
  {
    return Runner->Fixture->CountsObj->wrong;
  }

  int EXAMPLETESTS::Ignores() 
  {
    return Runner->Fixture->CountsObj->ignores;
  }

  int EXAMPLETESTS::Exceptions() 
  {
    return Runner->Fixture->CountsObj->exceptions;
  }

  STRING EXAMPLETESTS::Read(const STRING& input) 
  {
    STRING retVal;
    BUFFEREDFILEREADER in(input);

    wchar_t readBuf[65536];
    
    int numRead;
    
    while(true) 
    {
      numRead = in.Read(65535, &readBuf[0]);
      if(numRead < 0) 
      {
        break;
      }
      readBuf[numRead] = L'\0';
      retVal += readBuf;
    }

    in.Close();
    return retVal;
  }


  // Footnote /////////////////////////////////

  void EXAMPLETESTS::DoRow(PTR<PARSE>& row) 
  {
    FileCell = row->Leaf();
    this->COLUMNFIXTURE::DoRow(row);
  }

  void EXAMPLETESTS::Wrong(PTR<PARSE>& cell) 
  {
    this->COLUMNFIXTURE::Wrong(cell);
    if(!Footnote.IsAssigned()) 
    {
      Footnote = Runner->Tables->Footnote();
      FileCell->AddToBody(Footnote);
    }
  }

  EXAMPLETESTS::EXAMPLETESTS() 
  {
    RunCounts = new COUNTS();

    RegisterCeefitField(this, "file", File);
    RegisterCeefitField(this, "wiki", Wiki);

    // This class is one of those weird cases where we've got two overloads for a function
    // and we need to take a function pointer to one of them.  The following local variable
    // disambiguates what we mean for the corresponding RegisterCeefitTest function below.
    // Were we to pass "&EXAMPLETESTS::Wrong" all by itself, we'd get a crazy 
    // "Unable to pick overload from overload" error because C++ doesn't have enough info
    // to figure out what we want.
    int (ceefit_call_spec EXAMPLETESTS::*wrongFtnPtr)(void) = &EXAMPLETESTS::Wrong;

    RegisterCeefitTest(this, "right", &EXAMPLETESTS::Right);
    RegisterCeefitTest(this, "wrong", wrongFtnPtr);
    RegisterCeefitTest(this, "ignores", &EXAMPLETESTS::Ignores);
    RegisterCeefitTest(this, "exceptions", &EXAMPLETESTS::Exceptions);
  }

  EXAMPLETESTS::~EXAMPLETESTS()
  {
  }

  
  static ::CEEFIT::REGISTERFIXTURECLASS< EXAMPLETESTS > ExampleTestsRegistration("EG::EXAMPLETESTS", "eg.ExampleTests");
};
