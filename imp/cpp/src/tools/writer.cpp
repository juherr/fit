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
  extern DYNARRAY<wchar_t> SprintfBuffer;

  ceefit_init_spec FILEWRITER::FILEWRITER(const STRING& filePath, bool createFile)
  {
    Output = _wfopen(filePath.GetBuffer(), createFile ? L"wb" : L"ab");
  }

  ceefit_init_spec FILEWRITER::~FILEWRITER()
  {
    Close();
  }

  void ceefit_call_spec FILEWRITER::Write(const STRING& aString)
  {
    if(Output != NULL)
    {
      StringWriter.Write(aString);
    }
    else
    {
      throw new IOEXCEPTION("FILE* handle is closed, failed to write.");
    }
  }

  static void ceefit_call_spec RestoreMsWordSpecialChar(wchar_t* uniBuffer, int aLength, wchar_t matchChar, wchar_t replaceChar)
  {
    int i = -1;

    while(++i < aLength)
    {
      if(uniBuffer[i] == matchChar)
      {
        uniBuffer[i] = replaceChar;
      }
    }
  }

  void ceefit_call_spec FILEWRITER::Fprint(const wchar_t* format, ...)
  {
    if(Output != NULL)
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

        countWritten = _vsnwprintf(&SprintfBuffer[0], SprintfBuffer.GetSize(), format, vaList);

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
    if(Output != NULL)
    {
      STRING outString(StringWriter.ToString());
      DYNARRAY<char> outBuf;
      int srcLen = outString.Length();
      outBuf.Reserve((srcLen*4) + 4);

      wchar_t* outStringBuf = outString.GetBuffer();

      // Hack:  Now restore any Java'fied smart quotes, en/em dashes and elipsis with their MsWord versions
      // Credit:  http://home.hiwaay.net/~taylorc/toolbox/sw-dev/fixquotes.html
      RestoreMsWordSpecialChar(outStringBuf, srcLen, 0x201c, 0x0093);   // smart double quote
      RestoreMsWordSpecialChar(outStringBuf, srcLen, 0x201d, 0x0094);
      RestoreMsWordSpecialChar(outStringBuf, srcLen, 0x2018, 0x0091);   // smart single quote
      RestoreMsWordSpecialChar(outStringBuf, srcLen, 0x2019, 0x0092);
      RestoreMsWordSpecialChar(outStringBuf, srcLen, 0x2013, 0x0096);   // en dash
      RestoreMsWordSpecialChar(outStringBuf, srcLen, 0x2014, 0x0097);   // em dash
      RestoreMsWordSpecialChar(outStringBuf, srcLen, 0x2026, 0x0085);   // elipsis

      outBuf[0] = '\0';
      setlocale(LC_CTYPE, "en_US.UTF-8");

      const wchar_t* src = outString.GetBuffer();
      size_t retCount = wcstombs(&outBuf[0], src, srcLen);

      if(retCount == (size_t) -1)
      {
        throw new PARSEEXCEPTION(STRING("Failed to convert Wide char to UTF-8.  Errno == ") + (errno == EINVAL ? "EINVAL" :
                                                                                               errno == EILSEQ ? "EILSEQ" :
                                                                                               "unknown reason code")
                                                                                            + "\n\n");
      }

      outBuf[retCount] = '\0';

      fwrite(&outBuf[0], retCount, 1, Output);

      fclose(Output);
      Output = NULL;
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

      countWritten = _vsnwprintf(&SprintfBuffer[0], SprintfBuffer.GetSize(), format, vaList);

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

  STRING ceefit_call_spec STRINGWRITER::ToString(void)
  {
    return(Output);
  }

};
