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
#     ifdef _DEBUG
        printf("%S\n", e->GetReason().GetBuffer());
#     endif
      delete e;
      return(2);
	  }
    catch(...)
    {
#     ifdef _DEBUG
        printf("Unknown exception caught\n");
#     endif
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
    PTR<FINDITERATOR> findIterator(fit_FindFiles(aDir));
    if(!findIterator->HasNext())
    {
      DYNARRAY<char> charBuf;

      aDir.GetAsCharArray(charBuf);
#ifdef _WIN32
      if(mkdir(&charBuf[0]) == -1)
#else
      if(mkdir(&charBuf[0], S_IRWXO | S_IRWXG | S_IRWXU) == -1)
#endif
      {
        throw new IOEXCEPTION(STRING("Create folder failed"));
      }
    }
  }

  STRING ceefit_call_spec FILERUNNER::ValidateOutputPath(const STRING& rawOutPath)
  {
    wchar_t drive[fit_MAX_DRIVE];
    wchar_t dir[fit_MAX_DIR];
    wchar_t fname[fit_MAX_FNAME];
    wchar_t ext[fit_MAX_EXT];

    fit_wsplitpath(rawOutPath.GetBuffer(), drive, dir, fname, ext);

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
      printf("usage:  %S -CeeFIT <input_file> <output_file>", argv[0].GetBuffer());
      return(2);
    }

    // initialize Summary fields
    Fixture->SummaryObj->InputFile = argv[2];

    PTR<FINDITERATOR> findIterator(fit_FindFiles(argv[2]));
    if(findIterator->HasNext())
    {
      Fixture->SummaryObj->InputUpdate = findIterator->GetNext().LastUpdateTime;
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

    return(1);
  }
};
