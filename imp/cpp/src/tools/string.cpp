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

// Boost includes
/*
#define BOOST_REGEX_NO_LIB
#define BOOST_REGEX_STATIC_LINK

#ifdef _MSC_VER
# define BOOST_REGEX_HAS_MS_STACK_GUARD
#endif

# define BOOST_REGEX_NO_TEMPLATE_SWITCH_MERGE

#ifndef __GNUC__
# define BOOST_REGEX_NO_EXTERNAL_TEMPLATES
#endif

#define BOOST_REGEX_USE_C_LOCALE        // assume our locale is set by C

#define BOOST_REGEX_NON_RECURSIVE
#define BOOST_REGEX_BLOCKSIZE 4096
#define BOOST_REGEX_MAX_BLOCKS 16384
#define BOOST_REGEX_MAX_CACHE_BLOCKS 256
#define BOOST_REGEX_MATCH_EXTRA

#ifdef __GNUC__
# define BOOST_NO_STD_ALLOCATOR
# define BOOST_NO_MEMBER_TEMPLATES       // couldn't get GCC to compile without doing this ...
#endif

// define CEEFIT_EXTERNAL_BOOST before building CeeFIT if you supply your own Boost library

#ifndef CEEFIT_EXTERNAL_BOOST
# ifndef __GNUC__
#   include <cstdlib>
#   include <boost/regex/src.cpp>
#   include <libs/regex/src/instances.cpp>
#   include <libs/regex/src/winstances.cpp>
# endif
#endif

#include <boost/regex.hpp>
#include <list>
#include <string>
*/

namespace CEEFIT
{
  DYNARRAY<wchar_t> SprintfBuffer;

  void ceefit_call_spec SafeSprintf(STRING& out, const wchar_t* format, ...)
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

    out = &(SprintfBuffer[0]);
  }

  void ceefit_call_spec SafeFprintf(FILE* out, const wchar_t* format, ...)
  {
    int countWritten = 0;

    va_list vaList;
    va_start(vaList, format);

    countWritten = vfwprintf(out, format, vaList);

    va_end(vaList);

    if(countWritten < 0)
    {
      throw new IOEXCEPTION("Write error during SafeFprintf");
    }
  }

  bool ceefit_call_spec STRING::IsAssigned(void) const
  {
    return(Data.AssignedFlag);
  }

  ceefit_init_spec STRING::STRING() : Data(*(new STRINGDATA()))
  {
    Data.Array.Add(L'\0');
    Data.AssignedFlag = false;
  }

  ceefit_init_spec STRING::STRING(const STRING& aVal) : Data(*(new STRINGDATA()))
  {
    ASSIGNHELP<const STRING>::Assign(*this, aVal);
    Data.AssignedFlag = true;
  }

  ceefit_init_spec STRING::STRING(const wchar_t* aVal) : Data(*(new STRINGDATA()))
  {
    ASSIGNHELP<const wchar_t*>::Assign(*this, aVal);
    Data.AssignedFlag = true;
  }

  ceefit_init_spec STRING::STRING(const char* aVal) : Data(*(new STRINGDATA()))
  {
    ASSIGNHELP<const char*>::Assign(*this, aVal);
    Data.AssignedFlag = true;
  }

  ceefit_init_spec STRING::STRING(const wchar_t aVal) : Data(*(new STRINGDATA()))
  {
    wchar_t tempArray[2];

    tempArray[0] = aVal;
    tempArray[1] = L'\0';

    const wchar_t* tempPtr = &tempArray[0];

    ASSIGNHELP<const wchar_t*>::Assign(*this, tempPtr);
    Data.AssignedFlag = true;
  }

  ceefit_init_spec STRING::STRING(const char aVal) : Data(*(new STRINGDATA()))
  {
    char tempArray[2];

    tempArray[0] = aVal;
    tempArray[1] = '\0';

    const char* tempPtr = &tempArray[0];

    ASSIGNHELP<const char*>::Assign(*this, tempPtr);
    Data.AssignedFlag = true;
  }

  ceefit_init_spec STRING::~STRING()
  {
    delete &Data;
  }

  void ceefit_call_spec STRING::Reset()
  {
    Data.Array.Reset();
    Data.Array.Add(L'\0');
    Data.AssignedFlag = false;
  }

  STRING ceefit_call_spec STRING::operator+(const int aValue) const
  {
    wchar_t intBuffer[32];

    _itow(aValue, intBuffer, 10);

    return((*this) + intBuffer);
  }

  STRING& ceefit_call_spec STRING::operator+=(const int aValue)
  {
    wchar_t intBuffer[32];

    _itow(aValue, intBuffer, 10);

    Data.AssignedFlag = true;
    return((*this) += intBuffer);
  }

  void ceefit_call_spec STRING::Set(const STRING& src)
  {
    Data.Array = src.Data.Array;
    Data.AssignedFlag = true;
  }

  void ceefit_call_spec STRING::SetChar(const char* str)
  {
    Data.Array.Reset();

    if(str == NULL)
    {
      Data.Array.Reserve(1);
      Data.Array[0] = L'\0';
    }

    int len = strlen(str);
    Data.Array.Reserve(len + 1);

    swprintf(&Data.Array[0], L"%S", str);
    Data.AssignedFlag = true;
  }

  void ceefit_call_spec STRING::SetWChar(const wchar_t* str)
  {
    Data.Array.Reset();

    if(str == NULL)
    {
      Data.Array.Reserve(1);
      Data.Array[0] = L'\0';
    }

    int len = wcslen(str);
    Data.Array.Reserve(len + 1);

    int i = 0;
    int j = 0;
    while(str[j])
    {
      Data.Array[i++] = str[j++];
    }
    Data.Array[i] = L'\0';
    Data.AssignedFlag = true;
  }

  void ceefit_call_spec STRING::Append(const STRING& str)
  {
    int len1 = wcslen(&Data.Array[0]);
    int len2 = wcslen(&str.Data.Array[0]);

    int i = len1;
    int j = 0;

    if(len2 > 0)
    {
      Data.Array.Reserve(len2);

      while(str.Data.Array[j])
      {
        Data.Array[i++] = str.Data.Array[j++];
      }

      Data.Array[i] = L'\0';
    }
    Data.AssignedFlag = true;
  }

  int ceefit_call_spec STRING::Length() const
  {
    return(wcslen(&Data.Array[0]));
  }

  wchar_t ceefit_call_spec STRING::CharAt(int index) const
  {
    return(Data.Array[index]);
  }

  int ceefit_call_spec STRING::IndexOf(wchar_t aChar) const
  {
    int i = 0;
    wchar_t bChar;

    while((bChar = Data.Array[i]))
    {
      if(aChar == bChar)
      {
        return(i);
      }
      i++;
    }
    return(-1);
  }

  int ceefit_call_spec STRING::IndexOf(char aChar) const
  {
    return(IndexOf((wchar_t) aChar));
  }

  int ceefit_call_spec STRING::IndexOf(const STRING& aString) const
  {
    if(aString.Length() == 0)
    {
      return(-1);
    }

    int i = 0;
    wchar_t bChar;

    while((bChar = Data.Array[i]))
    {
      int iPrime = i;
      int j = 0;
      wchar_t bCharPrime;
      wchar_t aChar = aString.CharAt(j);

      while((bCharPrime = Data.Array[iPrime]))
      {
        if(bCharPrime != aChar)
        {
          break;
        }

        j++;
        aChar = aString.CharAt(j);
        if(aChar == L'\0')
        {
          return(i);
        }

        iPrime++;
      }

      i++;
    }
    return(-1);
  }

  int ceefit_call_spec STRING::IndexOf(wchar_t aChar, int fromIndex) const
  {
    int i = fromIndex;
    wchar_t bChar;

    while((bChar = Data.Array[i]))
    {
      if(aChar == bChar)
      {
        return(i);
      }
      i++;
    }
    return(-1);
  }

  int ceefit_call_spec STRING::IndexOf(char aChar, int fromIndex) const
  {
    return(IndexOf((wchar_t) aChar, fromIndex));
  }

  int ceefit_call_spec STRING::IndexOf(const STRING& aString, int fromIndex) const
  {
    if(aString.Length() == 0)
    {
      return(-1);
    }

    int i = fromIndex;
    wchar_t bChar;

    while((bChar = Data.Array[i]))
    {
      int iPrime = i;
      int j = 0;
      wchar_t bCharPrime;
      wchar_t aChar = aString.CharAt(j);
      while((bCharPrime = Data.Array[iPrime]))
      {
        if(bCharPrime != aChar)
        {
          break;
        }

        j++;
        aChar = aString.CharAt(j);
        if(aChar == L'\0')
        {
          return(i);
        }

        iPrime++;
      }

      i++;
    }
    return(-1);
  }

  const wchar_t* ceefit_call_spec STRING::GetBuffer() const
  {
    return(&(Data.Array[0]));
  }

  wchar_t* ceefit_call_spec STRING::GetBuffer()
  {
    return(&(Data.Array[0]));
  }

  STRING ceefit_call_spec STRING::ToLowercase() const
  {
    STRING retVal;
    retVal.Data.Array.Reserve(this->Length());

    int length = this->Length();
    int i = 0;
    while(i < length)
    {
      wchar_t& aDest = retVal.Data.Array[i];
      aDest = towlower(this->CharAt(i++));
    }
    retVal.Data.Array[i] = L'\0';

    return(retVal);
  }

  STRING ceefit_call_spec STRING::ToUppercase() const
  {
    STRING retVal;
    retVal.Data.Array.Reserve(this->Length());

    int length = this->Length();
    int i = 0;
    while(i < length)
    {
      wchar_t& aDest = retVal.Data.Array[i];
      aDest = towupper(this->CharAt(i++));
    }
    retVal.Data.Array[i] = L'\0';

    return(retVal);
  }

  STRING ceefit_call_spec STRING::Replace(wchar_t matchChar, wchar_t replaceChar) const
  {
    STRING retVal;
    retVal.Data.Array.Reserve(this->Length());

    int i = 0;
    wchar_t bChar;

    while((bChar = Data.Array[i]))
    {
      if(matchChar == bChar)
      {
        retVal.Data.Array[i++] = replaceChar;
      }
      else
      {
        retVal.Data.Array[i++] = bChar;
      }
    }
    retVal.Data.Array[i] = L'\0';

    return(retVal);
  }

  STRING ceefit_call_spec STRING::Trim() const
  {
    int firstChar = 0;
    int length = this->Length();
    int lastChar = length;

    while(firstChar < length && iswspace(Data.Array[firstChar]))
    {
      firstChar++;
    }

    while(lastChar >= firstChar)
    {
      lastChar--;
      if(lastChar < firstChar)
      {
        break;
      }
      if(!iswspace(Data.Array[lastChar]))
      {
        break;
      }
      // already decremented lastChar
    }

    return(this->Substring(firstChar, lastChar+1));
  }

/* Couldn't get Boost to link on gcc ...

  // based on the new_allocator class in the gcc std c++ sources
  template<typename _Tp> class ceefit_allocator : public std::allocator<_Tp>
  {
    public:
      typedef size_t     size_type;
      typedef ptrdiff_t  difference_type;
      typedef _Tp*       pointer;
      typedef const _Tp* const_pointer;
      typedef _Tp&       reference;
      typedef const _Tp& const_reference;
      typedef _Tp        value_type;

      template<typename _Tp1> struct rebind
      {
        typedef ceefit_allocator<_Tp1> other;
      };

      ceefit_allocator<_Tp>(void) throw() { }

      template<typename _Tp1> ceefit_allocator<_Tp>(const _Tp1&) throw() { }

      explicit ceefit_allocator<_Tp>(const ceefit_allocator<_Tp>& ca) throw() { }

      ~ceefit_allocator<_Tp>(void) throw() { }

      pointer address(reference __x) const
      {
        return &__x;
      }

      const_pointer address(const_reference __x) const
      {
        return &__x;
      }

      pointer allocate(size_type __n, const void* = 0)
      {
        return static_cast<_Tp*>(operator new(__n * sizeof(_Tp)));
      }

      void deallocate(void* __p, size_type)
      {
        assert(__p != NULL);

        operator delete(__p);
      }

      size_type max_size() const throw()
      {
        return size_t(-1) / sizeof(_Tp);
      }

      void  construct(pointer __p, const _Tp& __val)
      {
        ::new(__p) _Tp(__val);
      }

      void destroy(pointer __p)
      {
        __p->~_Tp();
      }
  };
};

typedef boost::detail::allocator_adapter< wchar_t, CEEFIT::ceefit_allocator< wchar_t > > REGEXALLOCATOR;
typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, REGEXALLOCATOR> CPPSTRING;
typedef boost::regex_traits<wchar_t> REGEXTRAITS;
typedef boost::reg_expression<wchar_t, REGEXTRAITS, REGEXALLOCATOR> REGEX;
typedef boost::match_results<CPPSTRING::iterator, REGEXALLOCATOR> REGEXRESULTS;

namespace CEEFIT
{
  template<typename _Tp> inline bool operator==(const ceefit_allocator<_Tp>&, const ceefit_allocator<_Tp>&)
  {
    return true;
  }

  template<typename _Tp> inline bool operator!=(const ceefit_allocator<_Tp>&, const ceefit_allocator<_Tp>&)
  {
    return false;
  }

# pragma optimize( "", off )

  STRING ceefit_call_spec STRING::ReplaceAll(const STRING& matchStr, const STRING& replaceStr)
  {
    CPPSTRING src;
    src = this->GetBuffer();

    REGEX regEx(matchStr.GetBuffer(), ::boost::regex_constants::normal);
    
    CPPSTRING retStr;
    ::boost::re_detail::string_out_iterator< CPPSTRING > i(retStr);    
    ::boost::regex_replace(i, src.begin(), src.end(), regEx, replaceStr.GetBuffer());
    
    STRING outStr(&retStr[0]);
    return(outStr);
  }

  void ceefit_call_spec STRING::Split(DYNARRAY<STRING>& out, const STRING& matchStr) const
  {
    ::std::basic_string<wchar_t> src;
    src = this->GetBuffer();

    const REGEX regEx(matchStr.GetBuffer(), ::boost::regex_constants::normal);

    ::std::list< ::std::basic_string<wchar_t> > l;

    ::boost::regex_split(::std::back_inserter(l), src, regEx, ::boost::match_default, UINT_MAX);

    ::std::list< ::std::basic_string<wchar_t> >::iterator aIterator;
    for(aIterator = l.begin(); aIterator != l.end(); aIterator++)
    {
      out.Add(STRING(&((*aIterator)[0])));
    }
  }

  bool ceefit_call_spec STRING::CapturingGroups(DYNARRAY<STRING>& out, const STRING& regexStr) const
  {
    CPPSTRING src;
    src = this->GetBuffer();

    REGEXRESULTS what;
    REGEX regEx(regexStr.GetBuffer(), ::boost::regex_constants::normal);

    if(::boost::regex_match(src.begin(), src.end(), what, regEx))
    {
      int i = -1;
      while(++i < what.size())
      {
        out.Add(&what[i].str()[0]);
      }

      return(true);
    }
    return(false);
  }

# pragma optimize( "", on )
 couldn't get Boost to link on gcc */

  STRING ceefit_call_spec STRING::Substring(int startChar, int endChar) const
  {
    if(!(startChar >= 0 && endChar >= 0 && startChar <= endChar))
    {
      throw new BOUNDSEXCEPTION("Index out of bounds.", startChar);
    }
    if(!(endChar <= this->Length()))
    {
      throw new BOUNDSEXCEPTION("endChar extends past end of string.", endChar);
    }

    STRING retVal;
    int i = 0;

    if(startChar != endChar)
    {
      retVal.Data.Array.Reserve(endChar - startChar);

      while(startChar < endChar)
      {
        retVal.Data.Array[i++] = this->Data.Array[startChar++];
      }
      retVal.Data.Array[i] = L'\0';
    }

    return(retVal);
  }

  STRING ceefit_call_spec STRING::Substring(int startChar) const
  {
    int endChar = this->Length();

    if(!(startChar >= 0))
    {
      throw new BOUNDSEXCEPTION("Index out of bounds.", startChar);
    }
    if(!(startChar <= endChar))
    {
      throw new BOUNDSEXCEPTION("Start char > end char", startChar);
    }

    STRING retVal;
    int i = 0;

    if(startChar != endChar)
    {
      retVal.Data.Array.Reserve(endChar - startChar);

      while(startChar < endChar)
      {
        retVal.Data.Array[i++] = this->Data.Array[startChar++];
      }
      retVal.Data.Array[i] = L'\0';
    }

    return(retVal);
  }

  bool ceefit_call_spec STRING::IsEqual(const STRING& aString) const
  {
    return(!wcscmp(this->GetBuffer(), aString.GetBuffer()));
  }

  bool ceefit_call_spec STRING::IsEqual(const char* aString) const
  {
    STRING temp(aString);

    return(this->IsEqual(temp));
  }

  bool ceefit_call_spec STRING::IsEqual(const wchar_t* aString) const
  {
    STRING temp(aString);

    return(this->IsEqual(temp));
  }

  bool ceefit_call_spec STRING::EndsWith(const STRING& aString) const
  {
    int thisIndex = Length();
    int otherIndex = aString.Length();

    const wchar_t* thisBuffer = GetBuffer();
    const wchar_t* otherBuffer = aString.GetBuffer();

    if(otherIndex == 0)
    {
      return(true);
    }

    while(thisIndex >= 0 && otherIndex >= 0)
    {
      if(thisBuffer[thisIndex] != otherBuffer[otherIndex])
      {
        break;
      }
      thisIndex--;
      otherIndex--;
    }

    return(otherIndex < 0);
  }

  bool ceefit_call_spec STRING::StartsWith(const STRING& aString) const
  {
    const wchar_t* thisBuffer = GetBuffer();
    const wchar_t* otherBuffer = aString.GetBuffer();

    while(*thisBuffer && *otherBuffer)
    {
      if(*thisBuffer != *otherBuffer)
      {
        break;
      }
      thisBuffer++;
      otherBuffer++;
    }

    return(!(*otherBuffer));
  }

  /**
   * <p>This method tokenizes this STRING based on a Delimiter list while considering double quoted
   * parts of the STRTYPE as atomic.</p>
   *
   * Delimiters found inside the double quoted strings are ignored.</p>
   */
  void ceefit_call_spec TokenizeRespectQuotes(DYNARRAY< STRING >& argList, const STRING& inString, const STRING& delimitList, bool doDequotify)
  {
	  int i;
	  STRING tempCopy(inString);
    wchar_t* last = tempCopy.GetBuffer();
	  wchar_t* cur = tempCopy.GetBuffer();
	  bool quoteMode = false;

	  if(!(*cur))
    {
      return;
    }

	  while(*cur)
    {
		  if(*cur == L'\"')
      {
			  if(quoteMode && (*(cur+1) == L'\"'))
        {
				  // double quote ... s'ok let it pass..
				  cur++;
				  if(!doDequotify)
          {
					  cur++;
				  }
			  }
			  else
        {
				  // quote mode
				  quoteMode = !quoteMode;
			  }
			  // drag everything one char back from Cur
			  if(doDequotify)
        {
				  wchar_t* temp = cur;
				  while(*temp)
          {
					  *temp = *(temp+1);
					  temp++;
				  }
				  continue;
			  }
		  }
		  else if(!quoteMode)
      {
			  i = 0;
        const wchar_t* delimitChar = delimitList.GetBuffer();
			  while(delimitChar[i])
        {
				  if(*cur == delimitChar[i])
          {
					  *cur = L'\0';
					  if(last != cur)
            {
						  argList.Add(last);
					  }
					  last = cur+1;
					  break;
				  }
				  i++;
			  }
		  }
		  cur++;
	  }
	  if(last != cur)
    {
		  argList.Add(last);
	  }
  }

  STRING ceefit_call_spec STRING::SimplePatternReplaceAll(const STRING& patternStr, const STRING& replaceStr) const
  {
    STRING temp(*this);

    while(true)
    {
      STRING work;

      wchar_t* tempBuf = temp.GetBuffer();
      wchar_t* aOccurrence = wcsstr(tempBuf, patternStr.GetBuffer());
      if(aOccurrence == NULL)
      {
        break;
      }

      work.Append(temp.Substring(0, (int) (aOccurrence - tempBuf)));
      work.Append(replaceStr);
      work.Append(temp.Substring(((int) (aOccurrence - tempBuf)) + patternStr.Length()));
      
      temp = work;
    }

    return(temp);
  }

  template<class ANYTYPE> static void AssertIsTrue(const ANYTYPE& expression)
  {
    if(!expression)
    {
      throw new EXCEPTION("Internal CeeFIT Assertion failed");
    }
  }

  static bool RegexPatternMatch(bool atBufferStart, wchar_t* curBuf, const STRING& patternBuf, const STRING* lookAheadPattern, wchar_t*& matchEnd, bool dotAll=false);

  static bool RegexMatchZeroOrMoreCharClass(wchar_t* curBuf, const STRING& patternBuf, wchar_t*& matchEnd)
  {
    matchEnd = curBuf;    // we always match on zero or more

    while(*curBuf != L'\0')
    {
      const wchar_t* aPattern = patternBuf.GetBuffer();
      aPattern++;  // skip the leading '['

      while(*aPattern != L']')
      {
        if(*curBuf == L'\0') 
        { 
          return(true);             // don't fall off the end of the string by accident
        }
        AssertIsTrue(*aPattern);    // don't fall off the end of the string by accident

        if(*aPattern == L'\\')
        {
          aPattern++;               // something's been escaped, skip to it ...
          AssertIsTrue(*aPattern);  // don't fall off the end of the string by accident
        }

        if(*curBuf == *aPattern)
        {
          curBuf++;
          matchEnd = curBuf;
          break;
        }

        aPattern++;
      }
      if(*aPattern == L']')
      {
        return(true);
      }
    }
    return(true);
  }

  static bool RegexMatchOneOrMoreCharClass(wchar_t* curBuf, const STRING& patternBuf, wchar_t*& matchEnd)
  {
    while(*curBuf != L'\0')
    {
      const wchar_t* aPattern = patternBuf.GetBuffer();
      aPattern++;  // skip the leading '['

      while(*aPattern != L']')
      {
        if(*curBuf == L'\0') 
        { 
          return(matchEnd != NULL); // don't fall off the end of the string by accident
        }
        AssertIsTrue(*aPattern);    // don't fall off the end of the string by accident

        if(*aPattern == L'\\')
        {
          aPattern++;               // something's been escaped, skip to it ...
          AssertIsTrue(*aPattern);  // don't fall off the end of the string by accident
        }

        if(*curBuf == *aPattern)
        {
          curBuf++;
          matchEnd = curBuf;
          break;
        }

        aPattern++;
      }
      if(*aPattern == L']')
      {
        return(matchEnd != NULL);
      }
    }
    return(matchEnd != NULL);
  }

  static bool RegexMatchOneCharClass(wchar_t* curBuf, const STRING& patternBuf, wchar_t*& matchEnd)
  {
    const wchar_t* aPattern = patternBuf.GetBuffer();
    aPattern++;  // skip the leading '['

    while(*aPattern != L']')
    {
      if(*curBuf == L'\0') 
      { 
        return(matchEnd != NULL); // don't fall off the end of the string by accident
      }
      AssertIsTrue(*aPattern);    // don't fall off the end of the string by accident

      if(*aPattern == L'\\')
      {
        aPattern++;               // something's been escaped, skip to it ...
        AssertIsTrue(*aPattern);  // don't fall off the end of the string by accident
      }

      if(*curBuf == *aPattern)
      {
        curBuf++;
        matchEnd = curBuf;
        break;
      }

      aPattern++;
    }
    return(matchEnd != NULL);
  }

  static bool RegexMatchZeroOrMore(wchar_t* curBuf, const STRING& patternBuf, wchar_t*& matchEnd)
  {
    matchEnd = curBuf;    // we always match on zero or more

    while(*curBuf != L'\0')
    {
      const wchar_t* aPattern = patternBuf.GetBuffer();
      while(*aPattern != L'*')
      {
        if(*curBuf == L'\0') 
        { 
          return(true);             // don't fall off the end of the string by accident
        }
        AssertIsTrue(*aPattern);    // don't fall off the end of the string by accident

        if(*aPattern == L'\\')
        {
          aPattern++;               // something's been escaped, skip to it ...
          AssertIsTrue(*aPattern);  // don't fall off the end of the string by accident
        }

        if(*curBuf != *aPattern)
        {
          return(true);
        }
        curBuf++;
        aPattern++;
      }
      matchEnd = curBuf;
    }
    return(true);
  }

  static bool RegexMatchZeroOrMoreAnyNoNewline(wchar_t* curBuf, const STRING& patternBuf, const STRING* lookaheadPattern, wchar_t*& matchEnd, bool dotAll)
  {
    matchEnd = curBuf;    // we always match on zero or more

    AssertIsTrue(patternBuf.IsEqual(".*"));   // this is a no frills matcher, must be .*

    while(*curBuf != L'\0')
    {
      const wchar_t* aPattern = patternBuf.GetBuffer();
      while(*aPattern != L'*')
      {
        if(*curBuf == L'\0') 
        { 
          return(true);             // don't fall off the end of the string by accident
        }
        AssertIsTrue(*aPattern);    // don't fall off the end of the string by accident

        // lookahead and see if we match the lookahead
        if(lookaheadPattern != NULL) 
        {
          wchar_t* lookaheadMatchEnd = NULL;
          
          if(RegexPatternMatch(false, curBuf, *lookaheadPattern, NULL, lookaheadMatchEnd, dotAll))
          {
            return(true);
          }         
        }

        // dot star only goes till end of line
        if(dotAll == false)
        {
          if(*curBuf == L'\r' || *curBuf == L'\n')
          {
            return(true);
          }
        }

        curBuf++;
        aPattern++;
      }
      matchEnd = curBuf;
    }
    return(true);
  }

  static bool RegexMatchOneOrMore(wchar_t* curBuf, const STRING& patternBuf, wchar_t*& matchEnd)
  {
    while(*curBuf != L'\0')
    {
      const wchar_t* aPattern = patternBuf.GetBuffer();
      while(*aPattern != L'+')
      {
        if(*curBuf == L'\0') 
        { 
          return(matchEnd != NULL); // don't fall off the end of the string by accident
        }

        AssertIsTrue(*curBuf);      // don't fall off the end of the string by accident
        AssertIsTrue(*aPattern);    // don't fall off the end of the string by accident

        if(*aPattern == L'\\')
        {
          aPattern++;               // something's been escaped, skip to it ...
          AssertIsTrue(*aPattern);  // don't fall off the end of the string by accident
        }

        if(*curBuf != *aPattern)
        {
          return(matchEnd != NULL);
        }
        curBuf++;
        aPattern++;
      }
      matchEnd = curBuf;
    }
    return(matchEnd != NULL);
  }

  static bool RegexMatchOneOrNone(wchar_t* curBuf, const STRING& patternBuf, wchar_t*& matchEnd)
  {
    while(*curBuf != L'\0')
    {
      const wchar_t* aPattern = patternBuf.GetBuffer();
      while(*aPattern != L'?')
      {
        if(*curBuf == L'\0') 
        { 
          return(matchEnd != NULL); // don't fall off the end of the string by accident
        }

        AssertIsTrue(*aPattern);    // don't fall off the end of the string by accident

        if(*aPattern == L'\\')
        {
          aPattern++;               // something's been escaped, skip to it ...
          AssertIsTrue(*aPattern);  // don't fall off the end of the string by accident
        }

        if(*curBuf != *aPattern)
        {
          return(matchEnd != NULL);
        }
        curBuf++;
        aPattern++;
      }
      if(matchEnd != NULL)
      {
        // we matched more than one, matchEnd != NULL, we were only supposed to match one
        matchEnd = NULL;
        return(false);
      }
      matchEnd = curBuf;
    }
    return(matchEnd != NULL);
  }

  static bool RegexMatchOne(wchar_t* curBuf, const STRING& patternBuf, wchar_t*& matchEnd)
  {
    const wchar_t* aPattern = patternBuf.GetBuffer();
    while(*aPattern != L'\0')
    {
      if(*curBuf == L'\0') 
      { 
        return(false);            // don't fall off the end of the string by accident
      }
      AssertIsTrue(*aPattern);    // don't fall off the end of the string by accident
      if(*aPattern == L'\\')
      {
        aPattern++;               // something's been escaped, skip to it ...
        AssertIsTrue(*aPattern);  // don't fall off the end of the string by accident
      }

      if(*curBuf != *aPattern)
      {
        return(false);
      }
      curBuf++;
      aPattern++;
    }
    matchEnd = curBuf;
    return(true);
  }

  static bool RegexMatchOneAnyNoNewline(wchar_t* curBuf, const STRING& patternBuf, const STRING* lookAheadPattern, wchar_t*& matchEnd, bool dotAll)
  {
    const wchar_t* aPattern = patternBuf.GetBuffer();
    while(*aPattern != L'\0')
    {
      if(*curBuf == L'\0') 
      { 
        return(false);            // don't fall off the end of the string by accident
      }
      AssertIsTrue(*aPattern);    // don't fall off the end of the string by accident
      if(*aPattern == L'\\')
      {
        aPattern++;               // something's been escaped, skip to it ...
        AssertIsTrue(*aPattern);  // don't fall off the end of the string by accident
      }

      if(*aPattern != L'.')
      {
        if(*curBuf != *aPattern)
        {
          return(false);
        }
      }
      else 
      {
        if(dotAll == false) 
        {
          if(*curBuf == L'\n' || *curBuf == L'\r') 
          {
            return(false);
          }
        }
      }
      curBuf++;
      aPattern++;
    }
    matchEnd = curBuf;
    return(true);
  }

  static bool RegexMatchEndOfLine(wchar_t* curBuf, const STRING& patternBuf, wchar_t*& matchEnd)
  {
    if(*curBuf == L'\0')
    {
      matchEnd = curBuf;
      return(true);
    }
    return(false);
  }

  static bool RegexMatchStartOfLine(bool isAtStartOfLine, wchar_t* curBuf, wchar_t*& matchEnd)
  {
    if(isAtStartOfLine)
    {
      matchEnd = curBuf;
      return(true);
    }
    return(false);
  }

  static bool RegexPatternMatch(bool atBufferStart, wchar_t* curBuf, const STRING& patternBuf, const STRING* lookAheadPattern, wchar_t*& matchEnd, bool dotAll)
  {
    static STRING brace("[");
    static STRING star("*");
    static STRING questionMark("?");
    static STRING plus("+");
    static STRING dollarSign("$");
    static STRING caret("^");
    static STRING slashEscape("\\");

    if(patternBuf.IsEqual(caret)) 
    {
      return(RegexMatchStartOfLine(atBufferStart, curBuf, matchEnd));
    }
    else if(patternBuf.IsEqual(dollarSign))
    {
      return(RegexMatchEndOfLine(curBuf, patternBuf, matchEnd));
    }
    else if(patternBuf.StartsWith(".")) 
    {
      if(patternBuf.IsEqual(".")) 
      {
        return(RegexMatchOneAnyNoNewline(curBuf, patternBuf, lookAheadPattern, matchEnd, dotAll));
      }
      else if(patternBuf.IsEqual(".*")) 
      {
        return(RegexMatchZeroOrMoreAnyNoNewline(curBuf, patternBuf, lookAheadPattern, matchEnd, dotAll));
      }
      else 
      {
        // should never get here
        return(false);
      }
    }
    else if(patternBuf.StartsWith(brace))
    {
      if(patternBuf.EndsWith(star))
      {
        return(RegexMatchZeroOrMoreCharClass(curBuf, patternBuf, matchEnd));
      }
      else if(patternBuf.EndsWith(plus))
      {
        return(RegexMatchOneOrMoreCharClass(curBuf, patternBuf, matchEnd));
      }
      else 
      {
        return(RegexMatchOneCharClass(curBuf, patternBuf, matchEnd));
      }
    }
    else
    {
      if(patternBuf.EndsWith(star))
      {
        return(RegexMatchZeroOrMore(curBuf, patternBuf, matchEnd));
      }
      else if(patternBuf.EndsWith(plus))
      {
        return(RegexMatchOneOrMore(curBuf, patternBuf, matchEnd));
      }
      else if(patternBuf.EndsWith(questionMark))
      {
        return(RegexMatchOneOrNone(curBuf, patternBuf, matchEnd));
      }
      else 
      {
        return(RegexMatchOne(curBuf, patternBuf, matchEnd));
      }
    }
  }

  STRING ceefit_call_spec STRING::ArrayRegexPatternReplaceAll(const DYNARRAY<STRING>& patternStrArray, const STRING& replaceStr, bool dotAll) const
  {    
    STRING temp(*this);

    bool atStartOfString = true;
    int startIndex = 0;

  restart:
    while(true)
    {
      wchar_t* curStartBuf = temp.GetBuffer() + startIndex;
      wchar_t* curBuf = curStartBuf;
      wchar_t* firstMatch = NULL;
      while(true)
      {
        int i = -1;
        while(true)
        {
          while(++i < patternStrArray.GetSize())
          {
            wchar_t* matchEnd = NULL;
            if(RegexPatternMatch(atStartOfString && curBuf == curStartBuf, 
                                 curBuf, 
                                 patternStrArray.Get(i), 
                                 (i+1) < patternStrArray.GetSize() ? &patternStrArray.Get(i+1) : NULL,
                                 matchEnd, 
                                 dotAll))
            {
              if(firstMatch == NULL)
              {
                firstMatch = curBuf;
                if(firstMatch > curStartBuf)
                {
                  curStartBuf = firstMatch;
                }
              }
              curBuf = matchEnd;
            }
            else
            {
              firstMatch = NULL;
              break;
            }
          }

          if(i == patternStrArray.GetSize() && firstMatch != NULL)
          {
            STRING work;

            work.Append(temp.Substring(0, (int) (firstMatch - temp.GetBuffer())));
            work.Append(replaceStr);
            startIndex = work.Length();
            work.Append(temp.Substring((int) (curBuf - temp.GetBuffer())));

            temp = work;
            atStartOfString = false;
            goto restart;
          }
          else
          {
            if(*curStartBuf == L'\0')
            {
              return(temp);
            }

            curStartBuf++;
            atStartOfString = false;
            curBuf = curStartBuf;
            break;
          }
        }

        if(*curStartBuf == L'\0')
        {
          return(temp);
        }
      }
    }
  }

  void ceefit_call_spec STRING::ArrayRegexPatternSplit(DYNARRAY<STRING>& out, const DYNARRAY<STRING>& patternStrArray, bool dotAll) const
  {
    STRING temp(*this);
    bool atStartOfString = true;

  restart:
    while(true)
    {
      wchar_t* curStartBuf = temp.GetBuffer();
      wchar_t* curBuf = curStartBuf;
      wchar_t* firstMatch = NULL;
      while(true)
      {
        int i = -1;
        while(true)
        {
          while(++i < patternStrArray.GetSize())
          {
            wchar_t* matchEnd = NULL;
            if(RegexPatternMatch(atStartOfString && curBuf == curStartBuf, 
                                 curBuf, 
                                 patternStrArray.Get(i), 
                                 (i+1) < patternStrArray.GetSize() ? &patternStrArray.Get(i+1) : NULL,
                                 matchEnd,
                                 dotAll))
            {
              if(firstMatch == NULL)
              {
                firstMatch = curBuf;
                if(firstMatch > curStartBuf)
                {
                  curStartBuf = firstMatch;
                  atStartOfString = false;
                }
              }
              curBuf = matchEnd;
            }
            else
            {
              firstMatch = NULL;
              break;
            }
          }

          if(i == patternStrArray.GetSize() && firstMatch != NULL)
          {
            STRING work;

            out.Add(temp.Substring(0, (int) (firstMatch - temp.GetBuffer())));
            temp = temp.Substring((int) (curBuf - temp.GetBuffer()));
            atStartOfString = false;
            goto restart;
          }
          else
          {
            if(*curStartBuf == L'\0')
            {
              if(temp.Length() > 0)
              {
                out.Add(temp);
              }
              return;
            }

            curStartBuf++;
            atStartOfString = false;
            curBuf = curStartBuf;
            break;
          }
        }

        if(*curStartBuf == L'\0')
        {
          if(temp.Length() > 0)
          {
            out.Add(temp);
          }
          return;
        }
      }
    }
  }
};
