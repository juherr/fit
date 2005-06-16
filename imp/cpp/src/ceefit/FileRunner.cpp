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
  ceefit_init_spec FILERUNNER::FILERUNNER()
  {
    Fixture = new FIXTURE();
    Output = null;
  }

  ceefit_init_spec FILERUNNER::~FILERUNNER()
  {
  }

  int ceefit_call_spec FILERUNNER::Run(const DYNARRAY<STRING>& argv)
  {
    try
    {
      int retVal;

      retVal = Args(argv);
      if(retVal != 1)
      {
        return(retVal);
      }

      Process();
      Exit();

      return(1);
	  }
	  catch(EXCEPTION* e)
    {
      fwprintf(stderr, L"%s\n", e->GetReason().GetBuffer());
      delete e;
      return(2);
	  }
    catch(...)
    {
      fwprintf(stderr, L"Unknown exception caught\n");
      return(2);
    }
  }

  void ceefit_call_spec FILERUNNER::Process()
  {
    try
    {
      if(Input.IndexOf("<wiki>") >= 0)
      {
        DYNARRAY<STRING> stringArray;

        if(stringArray.GetSize() == 0)      // done only once, the first time stringArray is used
        {
          stringArray.Add("wiki");
          stringArray.Add("table");
          stringArray.Add("tr");
          stringArray.Add("td");
        }

        Tables = new PARSE(Input, stringArray);
        Fixture->DoTables(Tables->Parts);
      }
      else
      {
        DYNARRAY<STRING> stringArray;

        if(stringArray.GetSize() == 0)      // done only once, the first time stringArray is used
        {
          stringArray.Add("table");
          stringArray.Add("tr");
          stringArray.Add("td");
        }

        Tables = new PARSE(Input, stringArray);
        Fixture->DoTables(Tables);
      }
    }
    catch(EXCEPTION* e)
    {
      Exception(e);
    }

    if(Tables != null)
    {
      Tables->Print(Output);
    }
  }

  void ceefit_call_spec FILERUNNER::ValidateOrCreateDir(const STRING& aDir)
  {
    struct _wfinddata_t findData;
    memset(&findData, 0, sizeof(struct _wfinddata_t));

    long retVal = _wfindfirst(aDir.GetBuffer(), &findData);
    if(retVal == -1)
    {
      if(errno == ENOENT)
      {
        if(_wmkdir(aDir.GetBuffer()) == -1)
        {
          throw new IOEXCEPTION(STRING("Create folder failed"));
        }
      }
      else if(errno == EINVAL)
      {
        throw new IOEXCEPTION(STRING("Bad pathspec, search failed:  ") + aDir);
      }
    }
    else
    {
      _findclose(retVal);
    }
  }

  STRING ceefit_call_spec FILERUNNER::ValidateOutputPath(const STRING& rawOutPath)
  {
    wchar_t drive[_MAX_DRIVE];
    wchar_t dir[_MAX_DIR];
    wchar_t fname[_MAX_FNAME];
    wchar_t ext[_MAX_EXT];

    _wsplitpath(rawOutPath.GetBuffer(), drive, dir, fname, ext);

    STRING outPathName(drive);
    DYNARRAY<STRING> pathParts;
    TokenizeRespectQuotes(pathParts, STRING(dir), STRING("\\/"));

    int i = -1;
    while(++i < pathParts.GetSize())
    {
      if(outPathName.Length() > 0)
      {
        outPathName += STRING("/") + pathParts[i];
      }
      else
      {
        outPathName += pathParts[i];
      }

      ValidateOrCreateDir(outPathName);
    }

    // synthesize an output file path by merging the verified output drive/dir with the fname/ext of file
    return(outPathName + "/" + fname + ext);
  }

  int ceefit_call_spec FILERUNNER::Args(const DYNARRAY<STRING>& argv)
  {
    if(argv.GetSize() != 4)
    {
      fwprintf(stderr, L"usage:  %s -CeeFIT <input_file> <output_file>", argv[0].GetBuffer());
      return(2);
    }

    // initialize Summary fields
    Fixture->SummaryObj->InputFile = argv[2];

    struct _wfinddata_t findData;
    memset(&findData, 0, sizeof(struct _wfinddata_t));
    long retVal = _wfindfirst(argv[2].GetBuffer(), &findData);
    if(retVal != -1)
    {
      Fixture->SummaryObj->InputUpdate = findData.time_write;

      _findclose(retVal);
    }

    Fixture->SummaryObj->OutputFile = ValidateOutputPath(argv[3]);

    // Read input and setup output writer
    Input = Read(Fixture->SummaryObj->InputFile);
    Output = new FILEWRITER(Fixture->SummaryObj->OutputFile);

    return(1);
  }

  STRING ceefit_call_spec FILERUNNER::Read(const STRING& inputFilename)
  {
    BUFFEREDFILEREADER reader(inputFilename);
    STRING retVal;

    int i = -1;
    while(!reader.IsEof())
    {
      wchar_t readBuf[16385];
      int countRead = reader.Read(16384, &readBuf[0]);

      if(countRead == -1)
      {
        break;
      }
      if(countRead > 0)
      {
        readBuf[countRead] = L'\0';
        retVal += readBuf;
      }
    }

    reader.Close();
    return(retVal);
  }

  void ceefit_call_spec FILERUNNER::Exception(EXCEPTION* e)
  {
    PTR<PARSE> nullParse;

    Tables = new PARSE(STRING("body"), STRING("Unable to parse input. Input ignored."), nullParse, nullParse);
    Fixture->Exception(Tables, e);
  }

  int ceefit_call_spec FILERUNNER::Exit()
  {
    if(Output != null)
    {
      Output->Close();
      delete Output;
      Output = null;
    }

    // todo ... figure out exit codes ...

    STRING countsString(Fixture->Counts());
    fwprintf(stderr, L"%s", countsString.GetBuffer());
    // System.exit(fixture.counts.wrong + fixture.counts.exceptions);

    return(1);
  }
};
