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
  void ceefit_call_spec BUFFEREDFILEREADER::DecodeInputBuffer(const DYNARRAY<char>& Buffer, int totalsRead, const STRING& fileName)
  {
    this->DecodeInputBuffer(Buffer, totalsRead, fileName, unicode_windows_1252_encoding);
  }

  bool InitedUnicode = false;

  void ceefit_call_spec BUFFEREDFILEREADER::DecodeInputBuffer(const DYNARRAY<char>& Buffer, int totalsRead, const STRING& fileName, unicode_encoding_t& expectedEncoding)
  {
    DYNARRAY<unicode_char_t> ConvertBuffer;
    ConvertBuffer.Reserve(totalsRead);
    const char* src = &Buffer[0];
    unicode_char_t* dest = &ConvertBuffer[0];

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

    fit_size_t inbytesLeft = totalsRead;
    fit_size_t outcharsLeft = ConvertBuffer.GetSize();
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

  ceefit_init_spec BUFFEREDFILEREADER::BUFFEREDFILEREADER(const STRING& fileName)
  {
    FILE* FileHandle = _wfopen(fileName.GetBuffer(), L"rb");

    if(FileHandle == null)
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
        FileHandle = null;

        throw new IOEXCEPTION("File read error");
      }

      readPos += retVal;
      totalsRead += retVal;

      if(retVal != readSize && feof(FileHandle))
      {
        fclose(FileHandle);
        FileHandle = null;

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

  ceefit_init_spec BUFFEREDFILEREADER::~BUFFEREDFILEREADER(void)
  {
    Close();
  }

  bool ceefit_call_spec BUFFEREDFILEREADER::IsEof(void) const
  {
    return(CharPos >= TotalChars || IsClosed);
  }

  void ceefit_call_spec BUFFEREDFILEREADER::Close(void)
  {
    IsClosed = true;
  }

  int ceefit_call_spec BUFFEREDFILEREADER::Read(int numChars, wchar_t* charArray)
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

  STRING ceefit_call_spec BUFFEREDFILEREADER::ReadLine(void)
  {
    STRING out;
    wchar_t aChar;

    int numRead;

    while((numRead = Read(1, &aChar)) > 0)
    {
      if(aChar == '\n')
      {
        break;
      }
      out += aChar;
    }

    if(out.EndsWith(STRING("\r")))     // in case it was cr-lf, clip off the cr too
    {
      out = out.Substring(0, out.Length()-1);
    }

    return(out);
  }

};

