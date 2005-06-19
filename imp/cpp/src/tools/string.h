#ifndef __TOOLS_STRING_H__
#define __TOOLS_STRING_H__

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
  class STRINGDATA;
  template<class T> class DYNARRAY;

  template<class ANYTYPE> class ASSIGNHELP
  {
  };

  /**
   * <p>Provide C++ equivalents of all Java String methods used in Java Fit to make porting easy.</p>
   */
  class STRING : public virtual OBJECT
  {
    public:
      void ceefit_call_spec Set(const STRING& aString);
      void ceefit_call_spec SetChar(const char* aChar);
      void ceefit_call_spec SetWChar(const wchar_t* aChar);
      void ceefit_call_spec Append(const STRING& aString);

      void ceefit_call_spec Reset(void);  /**< Reset internal buffer to empty string */

    private:
      STRINGDATA& Data;     /**< STRING needs a DYNARRAY before we have that class declared ... this is a way to get around that problem */

    public:
      ceefit_init_spec STRING(void);
      ceefit_init_spec STRING(const STRING& aVal);
      ceefit_init_spec STRING(const char* aVal);
      ceefit_init_spec STRING(const wchar_t* aVal);
      ceefit_init_spec STRING(const char aVal);
      ceefit_init_spec STRING(const wchar_t aVal);

//      template<class ANYTYPE> inline ceefit_init_spec STRING(ANYTYPE aVal) : Data(InitStringData())
//      {
//        ASSIGNHELP<ANYTYPE>::Assign(*this, aVal);
//      }

      ceefit_init_spec ~STRING(void);

      inline STRING& ceefit_call_spec operator=(const STRING& aVal)
      {
        this->Set(aVal);

        return(*this);
      }

      template<class ANYTYPE> inline STRING& ceefit_call_spec operator=(const ANYTYPE aVal)
      {
        ASSIGNHELP<const ANYTYPE>::Assign(*this, aVal);

        return(*this);
      }

      template<class ANYTYPE> inline STRING& ceefit_call_spec operator+=(const ANYTYPE aValue)
      {
        STRING temp(aValue);

        this->Append(temp);

        return(*this);
      }

      STRING& ceefit_call_spec operator+=(const int aValue);     /**< stringify integer and append */

      template<class ANYTYPE> inline STRING ceefit_call_spec operator+(const ANYTYPE aValue) const
      {
        STRING temp1(*this);
        STRING temp2(aValue);

        temp1.Append(temp2);

        return(temp1);
      }

      STRING ceefit_call_spec operator+(const int aValue) const;     /**< stringify integer and concat */

      int ceefit_call_spec Length(void) const;

      /**
       * <p>Indicate whether this STRING has been modified.</p>
       *
       * <p>This is not a complete port of the Java version of String.  Often a String's reference is checked to see if
       * an object was created and the reference was assigned.  Here we do something similar by tracking whether the
       * String object was ever assigned or modified.  The assignment to a STRING is a similar indicator of 'presence' to 
       * that of an immutable Java String's reference being set.</p>
       */
      bool ceefit_call_spec IsAssigned(void) const;

      wchar_t ceefit_call_spec CharAt(int index) const;
      int ceefit_call_spec IndexOf(wchar_t aChar) const;
      int ceefit_call_spec IndexOf(char aChar) const;
      int ceefit_call_spec IndexOf(const STRING& aString) const;
      int ceefit_call_spec IndexOf(const wchar_t* aString) const;
      int ceefit_call_spec IndexOf(const char* aString) const;
      int ceefit_call_spec IndexOf(wchar_t aChar, int fromIndex) const;
      int ceefit_call_spec IndexOf(char aChar, int fromIndex) const;
      int ceefit_call_spec IndexOf(const STRING& aString, int fromIndex) const;
      int ceefit_call_spec IndexOf(const wchar_t* aString, int fromIndex) const;
      int ceefit_call_spec IndexOf(const char* aString, int fromIndex) const;
      STRING ceefit_call_spec Substring(int startChar, int endChar) const;
      STRING ceefit_call_spec Substring(int startChar) const;
      STRING ceefit_call_spec ToLowercase(void) const;
      STRING ceefit_call_spec ToUppercase(void) const;
      STRING ceefit_call_spec Replace(wchar_t matchChar, wchar_t replaceChar) const;

      STRING ceefit_call_spec Trim(void) const;

// these calls used Boost to provide regular expressions, Boost wouldn't link on gcc so I had to take it out.      
//      STRING ceefit_call_spec ReplaceAll(const STRING& matchStr, const STRING& replaceStr);
//      void ceefit_call_spec Split(DYNARRAY<STRING>& out, const STRING& matchStr) const;
//      bool ceefit_call_spec CapturingGroups(DYNARRAY<STRING>& out, const STRING& regexStr) const;

      // these methods provide regex-free replacements for the functionality that ReplaceAll, Split, and CapturingGroups provided
      STRING ceefit_call_spec SimplePatternReplaceAll(const STRING& patternStr, const STRING& replaceStr) const;
      STRING ceefit_call_spec ArrayRegexPatternReplaceAll(const DYNARRAY<STRING>& patternStr, const STRING& replaceStr, bool dotAll=false) const;
      void ceefit_call_spec ArrayRegexPatternSplit(DYNARRAY<STRING>& out, const DYNARRAY<STRING>& patternStrArray, bool dotAll=false) const;

      bool ceefit_call_spec EndsWith(const STRING& aString) const;
      bool ceefit_call_spec EndsWith(const wchar_t* aString) const;
      bool ceefit_call_spec EndsWith(const char* aString) const;
      bool ceefit_call_spec StartsWith(const STRING& aString) const;
      bool ceefit_call_spec StartsWith(const wchar_t* aString) const;
      bool ceefit_call_spec StartsWith(const char* aString) const;

      const wchar_t* ceefit_call_spec GetBuffer(void) const;  /**< Get const buffer ... calling this is evil, do you reeeally want to be evil? */
      wchar_t* ceefit_call_spec GetBuffer(void);  /**< Get buffer ... calling this is REALLY evil, do you reeeally want to be really really evil? */

      bool ceefit_call_spec IsEqual(const STRING& aString) const;
      bool ceefit_call_spec IsEqual(const char* aString) const;
      bool ceefit_call_spec IsEqual(const wchar_t* aString) const;

      int ceefit_call_spec GetHashCode(void) const;
  
      /**
       * <p>Not a very exact conversion from unicode to char/ascii.</p>
       *
       * <p>Just needed a quick n' dirty way of getting the unicode chars converted back over to ascii.  If
       * any of the unicode characters are greater-than 255, they are output as underscores.</p>
       */
      void ceefit_call_spec GetAsCharArray(DYNARRAY<char>& out);
  };

  extern void ceefit_call_spec Tokenize(DYNARRAY< STRING >& argList, const STRING& inString, const STRING& delimitList, bool delimitersAreTokens=false);
  extern void ceefit_call_spec TokenizeRespectQuotes(DYNARRAY< STRING >& argList, const STRING& inString, const STRING& delimitList, bool doDequotify=true, bool delimitersAreTokens=false);

  template<> class ASSIGNHELP<const STRING>
  {
    public:
      static inline void Assign(STRING& dest, const STRING& src)
      {
        dest.Set(src);
      }

    private:
      ASSIGNHELP<const STRING>(void);       /**< Not implemented.  Do not call. */
      ~ASSIGNHELP<const STRING>(void);       /**< Not implemented.  Do not call. */
      ASSIGNHELP<const STRING>& operator=(const ASSIGNHELP<const STRING>&);       /**< Not implemented.  Do not call. */
      ASSIGNHELP<const STRING>(const ASSIGNHELP<const STRING>&);       /**< Not implemented.  Do not call. */
  };

  template<> class ASSIGNHELP<const wchar_t*>
  {
    public:
      static inline void Assign(STRING& dest, const wchar_t*& src)
      {
        dest.SetWChar(src);
      }

    private:
      ASSIGNHELP<const wchar_t*>(void);       /**< Not implemented.  Do not call. */
      ~ASSIGNHELP<const wchar_t*>(void);       /**< Not implemented.  Do not call. */
      ASSIGNHELP<const wchar_t*>& operator=(const ASSIGNHELP<const wchar_t*>&);       /**< Not implemented.  Do not call. */
      ASSIGNHELP<const wchar_t*>(const ASSIGNHELP<const wchar_t*>&);       /**< Not implemented.  Do not call. */
  };

  template<> class ASSIGNHELP<wchar_t* const>
  {
    public:
      static inline void Assign(STRING& dest, wchar_t* const& src)
      {
        dest.SetWChar(src);
      }

    private:
      ASSIGNHELP<wchar_t* const>(void);       /**< Not implemented.  Do not call. */
      ~ASSIGNHELP<wchar_t* const>(void);       /**< Not implemented.  Do not call. */
      ASSIGNHELP<wchar_t* const>& operator=(const ASSIGNHELP<wchar_t* const>&);       /**< Not implemented.  Do not call. */
      ASSIGNHELP<wchar_t* const>(const ASSIGNHELP<wchar_t* const>&);       /**< Not implemented.  Do not call. */
  };

  template<> class ASSIGNHELP<const wchar_t* const>
  {
    public:
      static inline void Assign(STRING& dest, const wchar_t* const& src)
      {
        dest.SetWChar(src);
      }

    private:
      ASSIGNHELP<const wchar_t* const>(void);       /**< Not implemented.  Do not call. */
      ~ASSIGNHELP<const wchar_t* const>(void);       /**< Not implemented.  Do not call. */
      ASSIGNHELP<const wchar_t* const>& operator=(const ASSIGNHELP<const wchar_t* const>&);       /**< Not implemented.  Do not call. */
      ASSIGNHELP<const wchar_t* const>(const ASSIGNHELP<const wchar_t* const>&);       /**< Not implemented.  Do not call. */
  };

  template<> class ASSIGNHELP<const char*>
  {
    public:
      static inline void Assign(STRING& dest, const char*& src)
      {
        dest.SetChar(src);
      }

    private:
      ASSIGNHELP<const char*>(void);       /**< Not implemented.  Do not call. */
      ~ASSIGNHELP<const char*>(void);       /**< Not implemented.  Do not call. */
      ASSIGNHELP<const char*>& operator=(const ASSIGNHELP<const char*>&);       /**< Not implemented.  Do not call. */
      ASSIGNHELP<const char*>(const ASSIGNHELP<const char*>&);       /**< Not implemented.  Do not call. */
  };

  template<> class ASSIGNHELP<char* const>
  {
    public:
      static inline void Assign(STRING& dest, char* const& src)
      {
        dest.SetChar(src);
      }

    private:
      ASSIGNHELP<char* const>(void);       /**< Not implemented.  Do not call. */
      ~ASSIGNHELP<char* const>(void);       /**< Not implemented.  Do not call. */
      ASSIGNHELP<char* const>& operator=(const ASSIGNHELP<char* const>&);       /**< Not implemented.  Do not call. */
      ASSIGNHELP<char* const>(const ASSIGNHELP<char* const>&);       /**< Not implemented.  Do not call. */
  };

  template<> class ASSIGNHELP<const char* const>
  {
    public:
      static inline void Assign(STRING& dest, const char* const& src)
      {
        dest.SetChar(src);
      }

    private:
      ASSIGNHELP<const char* const>(void);       /**< Not implemented.  Do not call. */
      ~ASSIGNHELP<const char* const>(void);       /**< Not implemented.  Do not call. */
      ASSIGNHELP<const char* const>& operator=(const ASSIGNHELP<const char* const>&);       /**< Not implemented.  Do not call. */
      ASSIGNHELP<const char* const>(const ASSIGNHELP<const char* const>&);       /**< Not implemented.  Do not call. */
  };

  /**
   * A helper function written to replace a _wcsicmp that might be missing on the host platform
   */
  extern int ceefit_call_spec fit_wcsicmp(const wchar_t* aStr, const wchar_t* bStr);
};

#endif // __TOOLS_STRING_H__
