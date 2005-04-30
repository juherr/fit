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

extern "C"
{
# include "unicode.h"
# include "convert.h"
};

namespace CEEFIT
{
  class BUFFEREDFILEREADER : public OBJECT
  {
    private:
      DYNARRAY<wchar_t> UniBuffer;
      int CharPos;
      int TotalChars;
      bool IsClosed;

      void ceefit_call_spec DecodeInputBuffer(const DYNARRAY<char>& Buffer, int totalsRead, const STRING& fileName, unicode_encoding_t& expectedEncoding = unicode_windows_1252_encoding)
      {
        DYNARRAY<unicode_char_t> ConvertBuffer;
        ConvertBuffer.Reserve(totalsRead);
        const char* src = &Buffer[0];
        unicode_char_t* dest = &ConvertBuffer[0];

        static bool InitedUnicode = false;
        if(InitedUnicode == false)
        {
          unicode_init();

          InitedUnicode = true;
        }

        void* privateData;
        if(expectedEncoding.init)
        {
          expectedEncoding.init(&privateData);
        }

        size_t inbytesLeft = totalsRead;
        size_t outcharsLeft = ConvertBuffer.GetSize();
        enum unicode_read_result readResult;
        readResult = expectedEncoding.read(privateData,
                     &src, &inbytesLeft,
                     &dest, &outcharsLeft);

        if(expectedEncoding.destroy)
        {
          expectedEncoding.destroy(&privateData);
        }

        if(readResult != unicode_read_ok)
        {
          STRING reason;

          reason = STRING("Failed to read input file ") + fileName.GetBuffer() + " using " + expectedEncoding.names[0] + " encoding."; 

          // must have hit a bad character, die monster die!
          throw new IOEXCEPTION(reason);
        }

        UniBuffer.Reset();
        UniBuffer = ConvertBuffer;    // convert from unsigned int to wchar_t
      }

    public:
      ceefit_init_spec BUFFEREDFILEREADER(const STRING& fileName)
      {
        FILE* FileHandle = _wfopen(fileName.GetBuffer(), L"rb");

        if(FileHandle == NULL)
        {
          throw new IOEXCEPTION("File error on open");
        }

        DYNARRAY<char> Buffer;
        int readPos = 0;
        int readSize = 16384;
        int totalsRead = 0;
        bool dataRemaining = true;
        while(dataRemaining)
        {
          Buffer.Reserve(readSize);
          size_t retVal = fread(&Buffer[readPos], 1, readSize, FileHandle);
          if(retVal != readSize && ferror(FileHandle))
          {
            fclose(FileHandle);
            FileHandle = NULL;

            throw new IOEXCEPTION("File read error");
          }

          readPos += retVal;
          totalsRead += retVal;

          if(retVal != readSize && feof(FileHandle))
          {
            fclose(FileHandle);
            FileHandle = NULL;

            break;
          }
        }
        Buffer.Reserve(4);
        Buffer[totalsRead] = '\0';
        Buffer[totalsRead+1] = '\0';
        Buffer[totalsRead+2] = '\0';
        Buffer[totalsRead+3] = '\0';

        // this populates UniBuffer with a decoded
        DecodeInputBuffer(Buffer, totalsRead+4, fileName);      

        CharPos = 0;
        TotalChars = wcslen(&UniBuffer[0]);
        IsClosed = false;
      }

      ceefit_init_spec ~BUFFEREDFILEREADER(void)
      {
        Close();
      }

      bool ceefit_call_spec IsEof(void) const
      {
        return(CharPos >= TotalChars || IsClosed);
      }

      void ceefit_call_spec Close(void)
      {
        IsClosed = true;
      }

      /**
       * @return number of chars read, -1 on end-of-file ...
       */
      int ceefit_call_spec Read(int numChars, wchar_t* charArray)
      {
        int i = 0;
        while(!IsEof() && i < numChars)
        {
          charArray[i++] = UniBuffer[CharPos++];
        }

        if(i == 0 && IsEof())
        {
          return(-1);
        }
        return(i);
      }

    private:
      ceefit_init_spec BUFFEREDFILEREADER(void);
      ceefit_init_spec BUFFEREDFILEREADER(const BUFFEREDFILEREADER&);
      BUFFEREDFILEREADER& ceefit_call_spec operator=(const BUFFEREDFILEREADER&);
  };

  ceefit_init_spec FILERUNNER::FILERUNNER()
  {
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

        stringArray.Add("wiki");
        stringArray.Add("table");
        stringArray.Add("tr");
        stringArray.Add("td");

        Tables = new PARSE(Input, stringArray);
        Fixture.DoTables(Tables->Parts);
      }
      else
      {
        DYNARRAY<STRING> stringArray;

        stringArray.Add("table");
        stringArray.Add("tr");
        stringArray.Add("td");

        Tables = new PARSE(Input, stringArray);
        Fixture.DoTables(Tables);
      }
    }
    catch(EXCEPTION* e)
    {
      Exception(e);
    }

    if(Tables != NULL)
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
    Fixture.SummaryObj->InputFile = argv[2];

    struct _wfinddata_t findData;
    memset(&findData, 0, sizeof(struct _wfinddata_t));
    long retVal = _wfindfirst(argv[2].GetBuffer(), &findData);
    if(retVal != -1)
    {
      Fixture.SummaryObj->InputUpdate = findData.time_write;

      _findclose(retVal);
    }

    Fixture.SummaryObj->OutputFile = ValidateOutputPath(argv[3]);

    // Read input and setup output writer
    Input = Read(Fixture.SummaryObj->InputFile);
    Output = new FILEWRITER(Fixture.SummaryObj->OutputFile);

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
    Fixture.Exception(Tables, e);
  }

  int ceefit_call_spec FILERUNNER::Exit()
  {
    if(Output != NULL)
    {
      Output->Close();
      delete Output;
      Output = NULL;
    }

    // todo ... figure out exit codes ...

    STRING countsString(Fixture.Counts());
    fwprintf(stderr, L"%s", countsString.GetBuffer());
    // System.exit(fixture.counts.wrong + fixture.counts.exceptions);

    return(1);
  }
};
