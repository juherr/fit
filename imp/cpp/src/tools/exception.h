#ifndef __TOOLS_EXCEPTION_H__
#define __TOOLS_EXCEPTION_H__

#include "ceefit/mandatory.h"

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

namespace CEEFIT
{
  /**
   * <p>A virtual base class for all EXCEPTION types that can be thrown</p>
   */
  class EXCEPTION : public virtual OBJECT
  {
    protected:
      STRING Reason;

    public:
      ceefit_init_spec EXCEPTION(void);
      virtual ceefit_init_spec ~EXCEPTION(void);

      virtual STRING& ceefit_call_spec GetReason(void);
      virtual void ceefit_call_spec SetReason(const STRING& aReason);
      virtual void ceefit_call_spec SetReason(const wchar_t* aReason);
      virtual const STRING& ceefit_call_spec GetExceptionTypeName(void);

      explicit ceefit_init_spec EXCEPTION(const STRING& aReason);
      explicit ceefit_init_spec EXCEPTION(const wchar_t* aReason);

    private:
      EXCEPTION& operator=(const EXCEPTION&);     /**< Not implemented.  Do not call. */
      EXCEPTION(const EXCEPTION&);                /**< Not implemented.  Do not call. */
  };

  /**
   * <p>Thrown when a parse fails</p>
   */
  class PARSEEXCEPTION : public EXCEPTION
  {
    private:
      int Offset;

    public:
      ceefit_init_spec PARSEEXCEPTION(void);
      ceefit_init_spec PARSEEXCEPTION(const STRING& aString);
      ceefit_init_spec PARSEEXCEPTION(const wchar_t* aString);
      virtual ceefit_init_spec ~PARSEEXCEPTION(void);
      virtual void ceefit_call_spec SetOffset(int aOffset);
      virtual int ceefit_call_spec GetOffset(void) const;
      ceefit_init_spec PARSEEXCEPTION(const STRING& aString, int aOffset);
      ceefit_init_spec PARSEEXCEPTION(const wchar_t* aString, int aOffset);
      virtual const STRING& ceefit_call_spec GetExceptionTypeName(void);

    private:
      PARSEEXCEPTION& operator=(const PARSEEXCEPTION&);     /**< Not implemented.  Do not call. */
      PARSEEXCEPTION(const PARSEEXCEPTION&);                /**< Not implemented.  Do not call. */
  };

  /**
   * <p>Thrown when something goes out of bounds</p>
   */
  class BOUNDSEXCEPTION : public EXCEPTION
  {
    private:
      int Index;

    public:
      ceefit_init_spec BOUNDSEXCEPTION(void);
      ceefit_init_spec BOUNDSEXCEPTION(const STRING& aString);
      ceefit_init_spec BOUNDSEXCEPTION(const wchar_t* aString);
      virtual ceefit_init_spec ~BOUNDSEXCEPTION(void);
      virtual void ceefit_call_spec SetIndex(int aIndex);
      virtual int ceefit_call_spec GetIndex(void) const;
      ceefit_init_spec BOUNDSEXCEPTION(const STRING& aString, int aIndex);
      ceefit_init_spec BOUNDSEXCEPTION(const wchar_t* aString, int aIndex);
      virtual const STRING& ceefit_call_spec GetExceptionTypeName(void);

    private:
      BOUNDSEXCEPTION& operator=(const BOUNDSEXCEPTION&);     /**< Not implemented.  Do not call. */
      BOUNDSEXCEPTION(const BOUNDSEXCEPTION&);                /**< Not implemented.  Do not call. */
  };

  /**
   * <p>Thrown when I/O operation fails</p>
   */
  class IOEXCEPTION : public EXCEPTION
  {
    public:
      ceefit_init_spec IOEXCEPTION(void);
      ceefit_init_spec IOEXCEPTION(const STRING& aString);
      ceefit_init_spec IOEXCEPTION(const wchar_t* aString);
      virtual ceefit_init_spec ~IOEXCEPTION(void);
      virtual const STRING& ceefit_call_spec GetExceptionTypeName(void);

    private:
      IOEXCEPTION& operator=(const IOEXCEPTION&);     /**< Not implemented.  Do not call. */
      IOEXCEPTION(const IOEXCEPTION&);                /**< Not implemented.  Do not call. */
  };

  /**
   * <p>Thrown when a FIXTURE class is not found by its name</p>
   */
  class CLASSNOTFOUNDEXCEPTION : public EXCEPTION
  {
    public:
      ceefit_init_spec CLASSNOTFOUNDEXCEPTION(void);
      ceefit_init_spec CLASSNOTFOUNDEXCEPTION(const STRING& aString);
      ceefit_init_spec CLASSNOTFOUNDEXCEPTION(const wchar_t* aString);
      virtual ceefit_init_spec ~CLASSNOTFOUNDEXCEPTION(void);
      virtual const STRING& ceefit_call_spec GetExceptionTypeName(void);

    private:
      CLASSNOTFOUNDEXCEPTION& operator=(const CLASSNOTFOUNDEXCEPTION&);     /**< Not implemented.  Do not call. */
      CLASSNOTFOUNDEXCEPTION(const CLASSNOTFOUNDEXCEPTION&);                /**< Not implemented.  Do not call. */
  };

  /**
   * <p>Thrown when a FIXTURE class if an operation would divide by zero</p>
   */
  class DIVIDEBYZEROEXCEPTION : public EXCEPTION
  {
    public:
      ceefit_init_spec DIVIDEBYZEROEXCEPTION(void);
      ceefit_init_spec DIVIDEBYZEROEXCEPTION(const STRING& aString);
      ceefit_init_spec DIVIDEBYZEROEXCEPTION(const wchar_t* aString);
      virtual ceefit_init_spec ~DIVIDEBYZEROEXCEPTION(void);
      virtual const STRING& ceefit_call_spec GetExceptionTypeName(void);

    private:
      DIVIDEBYZEROEXCEPTION& operator=(const DIVIDEBYZEROEXCEPTION&);     /**< Not implemented.  Do not call. */
      DIVIDEBYZEROEXCEPTION(const DIVIDEBYZEROEXCEPTION&);                /**< Not implemented.  Do not call. */
  };

};

#endif // __TOOLS_EXCEPTION_H__
