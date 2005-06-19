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
  extern DYNARRAY<wchar_t> SprintfBuffer;

  ceefit_init_spec FILEWRITER::FILEWRITER(const STRING& filePath, bool createFile, unicode_encoding_t& aExpectedEncoding)
  {
    DYNARRAY<char> charArray;
    filePath.GetAsCharArray(charArray);
    Output = fopen(&charArray[0], createFile ? "wb" : "ab");
    if(Output == null)
    {
      STRING reason;

      reason = STRING("Failed to open file for writing:  ") + filePath;
      
      throw new IOEXCEPTION(reason);    
    }

    ExpectedEncoding = &aExpectedEncoding;
    OutFilepath = filePath;
  }

  ceefit_init_spec FILEWRITER::~FILEWRITER()
  {
    Close();
  }

  void ceefit_call_spec FILEWRITER::Write(const STRING& aString)
  {
    if(Output != null)
    {
      StringWriter.Write(aString);
    }
    else
    {
      throw new IOEXCEPTION("FILE* handle is closed, failed to write.");
    }
  }

  void ceefit_call_spec FILEWRITER::Fprint(const wchar_t* format, ...)
  {
    if(Output != null)
    {
      if(SprintfBuffer.GetSize() == 0)
      {
        SprintfBuffer.Reserve(16);
      }

      int countWritten;
      bool loopForever = true;
      while(loopForever)
      {
        va_list vaList;
        va_start(vaList, format);

        countWritten = wine_vsnwprintf(&SprintfBuffer[0], SprintfBuffer.GetSize(), format, vaList);

        va_end(vaList);

        if(countWritten >= 0 && countWritten < (SprintfBuffer.GetSize()-1))
        {
          break;
        }
        SprintfBuffer.Reserve(SprintfBuffer.GetSize());     // double the buffer's size
      }

      Write(STRING(&(SprintfBuffer[0])));

    }
    else
    {
      throw new IOEXCEPTION("FILE* handle is closed, failed to write.");
    }
  }

  void ceefit_call_spec FILEWRITER::Close()
  {
    if(Output != null)
    {
      STRING outString(StringWriter.ToString());
      DYNARRAY<unicode_char_t> convertSrcBuffer;
      DYNARRAY<char> outBuf;
      int srcLen = outString.Length();
      wchar_t* srcWchar = outString.GetBuffer();
      convertSrcBuffer.Reserve(srcLen);
      
      int i = -1;
      while(++i < srcLen)
      {
        convertSrcBuffer[i] = srcWchar[i];
      }

      outBuf.Reserve((srcLen*6) + 6);

      void* privateData;
      if(ExpectedEncoding->init)
      {
        ExpectedEncoding->init(&privateData);
      }

      unicode_char_t* src = &convertSrcBuffer[0];
      char* dest = &outBuf[0];
      size_t incharsLeft = convertSrcBuffer.GetSize();
      size_t outbytesLeft = outBuf.GetSize();
      size_t maxbytesLeft = outbytesLeft;

      memset(dest, 0, outbytesLeft);    // clear the dest buffer

      if(ExpectedEncoding->reset)
      {
        ExpectedEncoding->reset(privateData, (char**) &dest, &outbytesLeft);
      }
      
      enum unicode_write_result writeResult;
      writeResult = ExpectedEncoding->write(privateData, &src, &incharsLeft, &dest, &outbytesLeft);

      if(ExpectedEncoding->destroy)
      {
        ExpectedEncoding->destroy(&privateData);
      }

      if(writeResult != unicode_write_ok)
      {
        STRING reason;

        reason = STRING("Failed to write output file ") + OutFilepath.GetBuffer() + " using " + ExpectedEncoding->names[0] + " encoding."; 

        // must have hit a bad character, die monster die!
        throw new IOEXCEPTION(reason);
      }

      fwrite(&outBuf[0], maxbytesLeft - outbytesLeft, 1, Output);

      fclose(Output);
      Output = null;
    }
  }

  ceefit_init_spec OUTWRITER::OUTWRITER()
  {
    IsClosed = false;
  }

  ceefit_init_spec OUTWRITER::~OUTWRITER()
  {
    Close();
  }

  void ceefit_call_spec OUTWRITER::Write(const STRING& aString)
  {
    if(IsClosed != true)
    {
      fwprintf(stdout, L"%s", aString.GetBuffer());
    }
    else
    {
      throw new IOEXCEPTION("Bug:  OUTWRITER closed, should not write.");
    }
  }

  void ceefit_call_spec OUTWRITER::Fprint(const wchar_t* format, ...)
  {
    if(IsClosed != true)
    {
      va_list vaList;

      va_start(vaList, format);
      vfwprintf(stdout, format, vaList);
      va_end(vaList);
    }
    else
    {
      throw new IOEXCEPTION("Bug:  OUTWRITER closed, should not write.");
    }
  }

  void ceefit_call_spec OUTWRITER::Close()
  {
    IsClosed = true;
  }

  ceefit_init_spec STRINGWRITER::STRINGWRITER()
  {
    IsClosed = false;
  }

  ceefit_init_spec STRINGWRITER::~STRINGWRITER()
  {
    Close();
  }

  void ceefit_call_spec STRINGWRITER::Write(const STRING& aString)
  {
    Output += aString;
  }

  void ceefit_call_spec STRINGWRITER::Fprint(const wchar_t* format, ...)
  {
    if(SprintfBuffer.GetSize() == 0)
    {
      SprintfBuffer.Reserve(16);
    }

    int countWritten;
    bool loopForever = true;
    while(loopForever)
    {
      va_list vaList;
      va_start(vaList, format);

      countWritten = wine_vsnwprintf(&SprintfBuffer[0], SprintfBuffer.GetSize(), format, vaList);

      va_end(vaList);

      if(countWritten >= 0 && countWritten < (SprintfBuffer.GetSize()-1))
      {
        break;
      }
      SprintfBuffer.Reserve(SprintfBuffer.GetSize());     // double the buffer's size
    }

    Output += &(SprintfBuffer[0]);
  }

  void ceefit_call_spec STRINGWRITER::Close()
  {
    IsClosed = true;
  }

  STRING ceefit_call_spec STRINGWRITER::ToString() const
  {
    return(Output);
  }

};
