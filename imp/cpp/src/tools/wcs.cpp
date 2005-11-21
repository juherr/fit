/*
 * msvcrt.dll wide-char functions
 *
 * Copyright 1999 Alexandre Julliard
 * Copyright 2000 Jon Griffiths
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <limits.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "tools/alloc.h"
#include "ceefit.h"

extern "C"
{
# include "unicode.h"
# include "convert.h"
}

// This module was taken from the WineHQ sources to be applied to CeeFIT
// Original copyrights and license terms apply

namespace CEEFIT
{
  /*********************************************************************
   *		_wcsnset (MSVCRT.@)
   */
  wchar_t* _wcsnset( wchar_t* str, wchar_t c, fit_size_t n )
  {
    wchar_t* ret = str;
    while ((n-- > 0) && *str) *str++ = c;
    return ret;
  }

  /*********************************************************************
   *		_wcsrev (MSVCRT.@)
   */
  wchar_t* _wcsrev( wchar_t* str )
  {
    wchar_t* ret = str;
    wchar_t* end = str + wcslen(str) - 1;
    while (end > str)
    {
      wchar_t t = *end;
      *end--  = *str;
      *str++  = t;
    }
    return ret;
  }

  /*********************************************************************
   *		_wcsset (MSVCRT.@)
   */
  wchar_t* _wcsset( wchar_t* str, wchar_t c )
  {
    wchar_t* ret = str;
    while (*str) *str++ = c;
    return ret;
  }

  /*********************************************************************
   *		wcstod (MSVCRT.@)
   */
  double MSVCRT_wcstod(const wchar_t* lpszStr, wchar_t** end)
  {
    const wchar_t* str = lpszStr;
    int negative = 0;
    double ret = 0, divisor = 10.0;

    /* FIXME:
     * - Should set errno on failure
     * - Should fail on overflow
     * - Need to check which input formats are allowed
     */
    while (unicode_isspace(*str))
      str++;

    if (*str == '-')
    {
      negative = 1;
      str++;
    }

    while (unicode_isdigit(*str))
    {
      ret = ret * 10.0 + (*str - '0');
      str++;
    }
    if (*str == '.')
      str++;
    while (unicode_isdigit(*str))
    {
      ret = ret + (*str - '0') / divisor;
      divisor *= 10;
      str++;
    }

    if (*str == 'E' || *str == 'e' || *str == 'D' || *str == 'd')
    {
      int negativeExponent = 0;
      int exponent = 0;
      if (*(++str) == '-')
      {
        negativeExponent = 1;
        str++;
      }
      while (unicode_isdigit(*str))
      {
        exponent = exponent * 10 + (*str - '0');
        str++;
      }
      if (exponent != 0)
      {
        if (negativeExponent)
          ret = ret / pow(10.0, exponent);
        else
          ret = ret * pow(10.0, exponent);
      }
    }

    if (negative)
      ret = -ret;

    if (end)
      *end = (wchar_t*)str;

    return ret;
  }


  typedef struct pf_output_t
  {
      int used;
      int len;
      bool unicode;
      union {
          wchar_t* W;
          char*  A;
      } buf;
  } pf_output;

  typedef struct pf_flags_t
  {
      char Sign, LeftAlign, Alternate, PadZero;
      char FieldLength, Precision;
      char IntegerLength, IntegerDouble;
      char WideString;
      char Format;
  } pf_flags;

  /*
   * writes a string of characters to the output
   * returns -1 if the string doesn't fit in the output buffer
   * return the length of the string if all characters were written
   */
  static inline int pf_output_stringW( pf_output *out, const wchar_t* str, int len )
  {
      int space = out->len - out->used;

      if( len < 0 )
          len = wcslen( str );
      if( out->unicode )
      {
          wchar_t* p = out->buf.W + out->used;

          if( space >= len )
          {
              memcpy( p, str, len*sizeof(wchar_t) );
              out->used += len;
              return len;
          }
          if( space > 0 )
              memcpy( p, str, space*sizeof(wchar_t) );
          out->used += len;
      }
      else
      {
        // don't want to deal with this
        throw new EXCEPTION("Unsupported output_stringW call");
      }
      return -1;
  }

  static inline int pf_output_stringA( pf_output *out, const char* str, int len )
  {
      int space = out->len - out->used;

      if( len < 0 )
          len = strlen( str );
      if( !out->unicode )
      {
          char* p = out->buf.A + out->used;

          if( space >= len )
          {
              memcpy( p, str, len );
              out->used += len;
              return len;
          }
          if( space > 0 )
              memcpy( p, str, space );
          out->used += len;
      }
      else
      {
        STRING wideVer(str);
        int n = wideVer.Length();
        wchar_t* p = out->buf.W + out->used;

        if( space >= n )
        {
          wcscpy(p, wideVer.GetBuffer());
          out->used += n;
          return(len);
        }

        if( space > 0 )
        {
          wcsncpy(p, wideVer.GetBuffer(), space);
        }
        out->used += n;
      }
      return -1;
  }

  static inline int pf_fill( pf_output *out, int len, pf_flags *flags, char left )
  {
      int i, r = 0;

      if( ( !left &&  flags->LeftAlign ) || 
          (  left && !flags->LeftAlign ) ||
          (  left &&  flags->PadZero   ) )
      {
          for( i=0; (i<(flags->FieldLength-len)) && (r>=0); i++ )
          {
              if( flags->PadZero )
                  r = pf_output_stringA( out, "0", 1 );
              else
                  r = pf_output_stringA( out, " ", 1 );
          }
      }

      return r;
  }

  static inline int pf_output_format_W( pf_output *out, const wchar_t* str,
                                        int len, pf_flags *flags )
  {
      int r = 0;

      if( len < 0 )
          len = wcslen( str );

      if (flags->Precision && flags->Precision < len)
        len = flags->Precision;

      r = pf_fill( out, len, flags, 1 );

      if( r>=0 )
          r = pf_output_stringW( out, str, len );

      if( r>=0 )
          r = pf_fill( out, len, flags, 0 );

      return r;
  }

  static inline int pf_output_format_A( pf_output *out, const char* str,
                                        int len, pf_flags *flags )
  {
      int r = 0;

      if( len < 0 )
          len = strlen( str );

      if (flags->Precision && flags->Precision < len)
        len = flags->Precision;

      r = pf_fill( out, len, flags, 1 );

      if( r>=0 )
          r = pf_output_stringA( out, str, len );

      if( r>=0 )
          r = pf_fill( out, len, flags, 0 );

      return r;
  }

  static inline bool pf_is_double_format( char fmt )
  {
      char float_fmts[] = "aefg";
      if (!fmt)
          return false;
      fmt = tolower( fmt );
      return strchr( float_fmts, fmt ) ? true : false;
  }

  static inline bool pf_is_valid_format( char fmt )
  {
      char float_fmts[] = "acdefginoux";
      if (!fmt)
          return false;
      fmt = tolower( fmt );
      return strchr( float_fmts, fmt ) ? true : false;
  }

  static void pf_rebuild_format_string( char *p, pf_flags *flags )
  {
      *p++ = '%';
      if( flags->Sign )
          *p++ = flags->Sign;
      if( flags->LeftAlign )
          *p++ = flags->LeftAlign;
      if( flags->Alternate )
          *p++ = flags->Alternate;
      if( flags->PadZero )
          *p++ = flags->PadZero;
      if( flags->FieldLength )
      {
          sprintf(p, "%d", flags->FieldLength);
          p += strlen(p);
      }
      if( flags->Precision )
      {
          sprintf(p, ".%d", flags->Precision);
          p += strlen(p);
      }
      *p++ = flags->Format;
      *p++ = 0;
  }

  /*********************************************************************
   *  pf_vsnprintf  (INTERNAL)
   *
   *  implements both A and W vsnprintf functions
   */
  static int pf_vsnprintf( pf_output *out, const wchar_t* format, va_list valist )
  {
      int r;
      const wchar_t* q;
      const wchar_t* p = format;
      pf_flags flags;

      while (*p)
      {
          q = wcschr( p, '%' );

          /* there's no % characters left, output the rest of the string */
          if( !q )
          {
              r = pf_output_stringW(out, p, -1);
              if( r<0 )
                  return r;
              p += r;
              continue;
          }

          /* there's characters before the %, output them */
          if( q != p )
          {
              r = pf_output_stringW(out, p, q - p);
              if( r<0 )
                  return r;
              p = q;
          }

          /* we must be at a % now, skip over it */
          assert( *p == '%' );
          p++;

          /* output a single % character */
          if( *p == '%' )
          {
              r = pf_output_stringW(out, p++, 1);
              if( r<0 )
                  return r;
              continue;
          }

          /* parse the flags */
          memset( &flags, 0, sizeof flags );
          while (*p)
          {
              if( *p == '+' || *p == ' ' )
                  flags.Sign = '+';
              else if( *p == '-' )
                  flags.LeftAlign = *p;
              else if( *p == '0' )
                  flags.PadZero = *p;
              else if( *p == '#' )
                  flags.Alternate = *p;
              else
                  break;
              p++;
          }

          /* deal with the field width specifier */
          flags.FieldLength = 0;
          if( *p == '*' ) 
          {
              flags.FieldLength = va_arg( valist, int );
              p++;
          }
          else while( isdigit(*p) )
          {
              flags.FieldLength *= 10;
              flags.FieldLength += *p++ - '0';
          }

          /* deal with precision */
          if( *p == '.' )
          {
              p++;
              if( *p == '*' )
              {
                  flags.Precision = va_arg( valist, int );
                  p++;
              }
              else while( isdigit(*p) )
              {
                  flags.Precision *= 10;
                  flags.Precision += *p++ - '0';
              }
          }

          /* deal with integer width modifier */
          while( *p )
          {
              if( *p == 'h' || *p == 'l' || *p == 'L' )
              {
                  if( flags.IntegerLength == *p )  /* FIXME: this is wrong */
                      flags.IntegerDouble++;
                  else
                      flags.IntegerLength = *p;
                  p++;
              }
              else if( *p == 'w' )
                  flags.WideString = *p++;
              else if( *p == 'F' )
                  p++; /* ignore */
              else
                  break;
          }

          flags.Format = *p;
          r = 0;

          /* output a unicode string */
          if( ( flags.Format == 's' && (flags.WideString || flags.IntegerLength == 'l' )) ||
              ( !out->unicode && flags.Format == 'S' ) ||
              ( out->unicode && flags.Format == 's' ) )
          {
              const wchar_t* str = va_arg( valist, const wchar_t* );

              if( str )
                  r = pf_output_format_W( out, str, -1, &flags );
              else
                  r = pf_output_format_A( out, "(null)", -1, &flags );
          }

          /* output a ASCII string */
          else if( ( flags.Format == 's' && flags.IntegerLength == 'h' ) ||
              ( out->unicode && flags.Format == 'S' ) ||
              ( !out->unicode && flags.Format == 's' ) )
          {
              const char* str = va_arg( valist, const char*);

              if( str )
                  r = pf_output_format_A( out, str, -1, &flags );
              else
                  r = pf_output_format_A( out, "(null)", -1, &flags );
          }

          /* output a single wide character */
          else if( ( flags.Format == 'c' && flags.IntegerLength == 'w' ) ||
              ( out->unicode && flags.Format == 'c' ) ||
              ( !out->unicode && flags.Format == 'C' ) )
          {
              wchar_t ch = va_arg( valist, int );

              r = pf_output_format_W( out, &ch, 1, &flags );
          }

          /* output a single ascii character */
          else if( ( flags.Format == 'c' && flags.IntegerLength == 'h' ) ||
              ( out->unicode && flags.Format == 'C' ) ||
              ( !out->unicode && flags.Format == 'c' ) )
          {
              char ch = va_arg( valist, int );

              r = pf_output_format_A( out, &ch, 1, &flags );
          }

          /* output a pointer */
          else if( flags.Format == 'p' )
          {
              char pointer[10];

              flags.PadZero = 0;
              if( flags.Alternate )
                  sprintf(pointer, "0X%08lX", va_arg(valist, long));
              else
                  sprintf(pointer, "%08lX", va_arg(valist, long));
              r = pf_output_format_A( out, pointer, -1, &flags );
          }

          /* deal with %n */
          else if( flags.Format == 'n' )
          {
              int *x = va_arg(valist, int *);
              *x = out->used;
          }

          /* deal with integers and floats using libc's printf */
          else if( pf_is_valid_format( flags.Format ) )
          {
              char fmt[20], number[40], *x = number;

              if( flags.FieldLength >= sizeof number )
              {
                  x = new char[flags.FieldLength+1];
                  memset(x, 0, flags.FieldLength+1);
              }

              pf_rebuild_format_string( fmt, &flags );

              if( pf_is_double_format( flags.Format ) )
                  sprintf( number, fmt, va_arg(valist, double) );
              else
                  sprintf( number, fmt, va_arg(valist, int) );

              r = pf_output_stringA( out, number, -1 );
              if( x != number )
              {
                  delete [] x;
              }
          }
          else
              continue;

          if( r<0 )
              return r;
          p++;
      }

      /* check we reached the end, and null terminate the string */
      assert( *p == 0 );
      r = pf_output_stringW( out, p, 1 );
      if( r<0 )
          return r;

      return out->used - 1;
  }

  /*********************************************************************
   *		_vsnprintf (MSVCRT.@)
   */
  int MSVCRT_vsnprintf( char *str, unsigned int len,
                  const char *format, va_list valist )
  {
      pf_output out;
      int r;

      out.unicode = false;
      out.buf.A = str;
      out.used = 0;
      out.len = len;

      if( format )
      {
          STRING formatW(format);

          r = pf_vsnprintf( &out, formatW.GetBuffer(), valist );
      }
      else
      {
        r = pf_vsnprintf( &out, NULL, valist );
      }

      return r;
  }

  /*********************************************************************
   *		_vsnwsprintf (MSVCRT.@)
   */
  int ceefit_call_spec wine_vsnwprintf( wchar_t *str, unsigned int len,
                                const wchar_t* format, va_list valist )
  {
      pf_output out;

      out.unicode = true;
      out.buf.W = str;
      out.used = 0;
      out.len = len;

      return pf_vsnprintf( &out, format, valist );
  }

  /*********************************************************************
   *		sprintf (MSVCRT.@)
   */
  int ceefit_call_spec MSVCRT_sprintf( char *str, const char *format, ... )
  {
      va_list ap;
      int r;

      va_start( ap, format );
      r = MSVCRT_vsnprintf( str, INT_MAX, format, ap );
      va_end( ap );
      return r;
  }

  /*********************************************************************
   *		swprintf (MSVCRT.@)
   */
  int ceefit_call_spec MSVCRT_swprintf( wchar_t *str, const wchar_t *format, ... )
  {
      va_list ap;
      int r;

      va_start( ap, format );
      r = wine_vsnwprintf( str, INT_MAX, format, ap );
      va_end( ap );
      return r;
  }

  /*********************************************************************
   *		vswprintf (MSVCRT.@)
   */
  int MSVCRT_vswprintf( wchar_t* str, const wchar_t* format, va_list args )
  {
      return wine_vsnwprintf( str, INT_MAX, format, args );
  }

  /*********************************************************************
   *		wcscoll (MSVCRT.@)
   */
  int MSVCRT_wcscoll( const wchar_t* str1, const wchar_t* str2 )
  {
    /* FIXME: handle collates */
    return wcscmp( str1, str2 );
  }

  /*********************************************************************
   *		wcspbrk (MSVCRT.@)
   */
  wchar_t* MSVCRT_wcspbrk( const wchar_t* str, const wchar_t* accept )
  {
    const wchar_t* p;
    while (*str)
    {
      for (p = accept; *p; p++) if (*p == *str) return (wchar_t*)str;
        str++;
    }
    return NULL;
  }

};
