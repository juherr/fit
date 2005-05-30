#ifndef __TOOLS_WRITER_H__
#define __TOOLS_WRITER_H__

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

extern "C"
{
  struct unicode_encoding_t;

  extern struct unicode_encoding_t unicode_windows_1252_encoding;
};

namespace CEEFIT
{
  class WRITER
  {
    public:
      inline ceefit_init_spec WRITER(void) {}
      virtual inline ceefit_init_spec ~WRITER(void) {}

      virtual void ceefit_call_spec Write(const STRING& aString)=0;
      virtual void ceefit_call_spec Fprint(const wchar_t* format, ...)=0;
      virtual void ceefit_call_spec Close(void)=0;

    private:
      WRITER& ceefit_call_spec operator=(const WRITER&);
      ceefit_init_spec WRITER(const WRITER&);
  };


  /**
   * <p>STRING WRITER to STRING</p>
   */
  class STRINGWRITER : public WRITER
  {
    private:
      bool IsClosed;
      STRING Output;

    public:
      ceefit_init_spec STRINGWRITER(void);
      virtual ceefit_init_spec ~STRINGWRITER(void);

      virtual void ceefit_call_spec Write(const STRING& aString);
      virtual void ceefit_call_spec Fprint(const wchar_t* format, ...);
      virtual void ceefit_call_spec Close(void);     /**< NOP:  actually doesn't try to close stdout handle */

      virtual STRING ceefit_call_spec ToString(void);

    private:
      STRINGWRITER& ceefit_call_spec operator=(const STRINGWRITER&);
      ceefit_init_spec STRINGWRITER(const STRINGWRITER&);
  };

  /**
   * <p>STRING WRITER to FILE*</p>
   */
  class FILEWRITER : public WRITER
  {
    private:
      unicode_encoding_t* ExpectedEncoding;
      STRINGWRITER StringWriter;
      FILE* Output;
      STRING OutFilepath;

    public:
      /**
       * @param filePath file path to open
       * @param createFile true = create the file (or truncate it if it already exists), false = append to end of existing file (or create if it does not exist)
       */
      ceefit_init_spec FILEWRITER(const STRING& filePath, bool createFile=true, unicode_encoding_t& aExpectedEncoding = unicode_windows_1252_encoding);
      virtual ceefit_init_spec ~FILEWRITER(void);

      virtual void ceefit_call_spec Write(const STRING& aString);
      virtual void ceefit_call_spec Fprint(const wchar_t* format, ...);
      virtual void ceefit_call_spec Close(void);

    private:
      FILEWRITER& ceefit_call_spec operator=(const FILEWRITER&);
      ceefit_init_spec FILEWRITER(const FILEWRITER&);
  };

  /**
   * <p>STRING WRITER to stdout</p>
   */
  class OUTWRITER : public WRITER
  {
    private:
      bool IsClosed;

    public:
      ceefit_init_spec OUTWRITER(void);
      virtual ceefit_init_spec ~OUTWRITER(void);

      virtual void ceefit_call_spec Write(const STRING& aString);
      virtual void ceefit_call_spec Fprint(const wchar_t* format, ...);
      virtual void ceefit_call_spec Close(void);     /**< NOP:  actually doesn't try to close stdout handle */

    private:
      OUTWRITER& ceefit_call_spec operator=(const OUTWRITER&);
      ceefit_init_spec OUTWRITER(const OUTWRITER&);
  };
};

#endif // __CEEFIT_WRITER_H__
