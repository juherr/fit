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

#ifndef __CEEFIT_BUFFEREDFILEREADER_H__
#define __CEEFIT_BUFFEREDFILEREADER_H__

namespace CEEFIT
{
  class BUFFEREDFILEREADER : public OBJECT
  {
    private:
      DYNARRAY<wchar_t> UniBuffer;
      int CharPos;
      int TotalChars;
      bool IsClosed;

      /**
       * <p>Defaults to CP1252 decoding</p>
       */ 
      void ceefit_call_spec DecodeInputBuffer(const DYNARRAY<char>& Buffer, int totalsRead, const STRING& fileName);

      void ceefit_call_spec DecodeInputBuffer(const DYNARRAY<char>& Buffer, int totalsRead, const STRING& fileName, unicode_encoding_t& expectedEncoding);

    public:
      ceefit_init_spec BUFFEREDFILEREADER(const STRING& fileName);

      ceefit_init_spec ~BUFFEREDFILEREADER(void);

      bool ceefit_call_spec IsEof(void) const;

      void ceefit_call_spec Close(void);

      /**
       * @return number of chars read, -1 on end-of-file ...
       */
      int ceefit_call_spec Read(int numChars, wchar_t* charArray);

      STRING ceefit_call_spec ReadLine(void);

    private:
      ceefit_init_spec BUFFEREDFILEREADER(void);
      ceefit_init_spec BUFFEREDFILEREADER(const BUFFEREDFILEREADER&);
      BUFFEREDFILEREADER& ceefit_call_spec operator=(const BUFFEREDFILEREADER&);
  };
};

#endif // __CEEFIT_BUFFEREDFILEREADER_H__
