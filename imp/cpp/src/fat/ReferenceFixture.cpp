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

declare_fit_module(ReferenceFixture);

using namespace CEEFIT;

namespace CEEFAT
{

  begin_fit_fixture(FAT_REFERENCEFIXTURE, COLUMNFIXTURE, fat.ReferenceFixture);

    public:
      fit_var(STRING, Description);
      fit_var(STRING, Location);

    public:
	    fit_test(Result, STRING)
      {
        wchar_t inputFileFullPath[_MAX_PATH];
        wchar_t drive[_MAX_DRIVE];
        wchar_t dir[_MAX_DIR];
        wchar_t otherDir[_MAX_DIR];
        wchar_t fname[_MAX_FNAME];
        wchar_t ext[_MAX_EXT];

        _wfullpath(inputFileFullPath, SummaryObj->InputFile.GetBuffer(), _MAX_PATH);
        _wsplitpath(inputFileFullPath, drive, dir, NULL, NULL);

		    STRING inputFileName(STRING(drive) + dir + Location);

        _wsplitpath(SummaryObj->OutputFile.GetBuffer(), drive, dir, NULL, NULL);
        _wsplitpath(Location.GetBuffer(), NULL, otherDir, fname, ext);

        STRING outDirName(drive);
        DYNARRAY<STRING> argList;
        TokenizeRespectQuotes(argList, STRING(dir) + otherDir, STRING("\\/"));

        int i = -1;
        while(++i < argList.GetSize())
        {        
          outDirName += STRING("/") + argList[i];
        }

        // synthesize an output file path by merging the original output drive/dir with the dir/fname of Location
		    STRING outputFileName(outDirName + "/" + fname + ext);
		    try 
        {
			    FILERUNNER runner;
          DYNARRAY<STRING> args;
          args.Add("ReferenceFixture");
          args.Add("-CeeFIT");
          args.Add(inputFileName);
          args.Add(outputFileName);

			    runner.Args(args);
			    runner.Process();
			    runner.Exit();
			    
			    PTR<FIXTURE::COUNTS> counts(runner.Fixture.CountsObj);
			    if ((counts->exceptions == 0) && (counts->wrong == 0)) 
          {
				    return STRING("pass");
			    }
			    else 
          {
				    return STRING("fail: ") + counts->right + " right, " + counts->wrong + " wrong, " + counts->exceptions + " exceptions";
			    }

          //counts = (FIXTUREDATA::COUNTS*) NULL;
          //runner.Fixture.FixtureData.Counts = (FIXTUREDATA::COUNTS*) NULL;
          //delete runner;
		    }
		    catch (EXCEPTION* e) 
        {
          STRING reason(e->GetReason());

          delete e;

          return(STRING("EXCEPTION:  ") + reason);
		    }
      }

  end_fit_fixture(FAT_REFERENCEFIXTURE);    

};
