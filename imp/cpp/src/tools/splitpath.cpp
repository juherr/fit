/*
 * Copyright 2000, 2004 Martin Fuchs
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

#include "tools/alloc.h"
#include "ceefit.h"

extern "C"
{
# include "unicode.h"
# include "convert.h"
}

// This code in module was taken from the WineHQ sources to be applied to CeeFIT
// Original copyrights and license terms apply

namespace CEEFIT
{
  void ceefit_call_spec fit_wsplitpath(const wchar_t* path, wchar_t* drv, wchar_t* dir, wchar_t* name, wchar_t* ext)
  {
    const wchar_t* end; /* end of processed string */
	  const wchar_t* p;	  /* search pointer */
	  const wchar_t* s;	  /* copy pointer */

	  /* extract drive name */
	  if (path[0] && path[1]==':') {
		  if (drv) {
			  *drv++ = *path++;
			  *drv++ = *path++;
			  *drv = L'\0';
		  }
	  } else if (drv)
		  *drv = L'\0';

	  /* search for end of string or stream separator */
	  for(end=path; *end && *end!=L':'; )
		  end++;

	  /* search for begin of file extension */
	  for(p=end; p>path && *--p!=L'\\' && *p!=L'/'; )
		  if (*p == L'.') {
			  end = p;
			  break;
		  }

	  if (ext)
		  for(s=end; (*ext=*s++); )
			  ext++;

	  /* search for end of directory name */
	  for(p=end; p>path; )
		  if (*--p=='\\' || *p=='/') {
			  p++;
			  break;
		  }

	  if (name) {
		  for(s=p; s<end; )
			  *name++ = *s++;

		  *name = L'\0';
	  }

	  if (dir) {
		  for(s=path; s<p; )
			  *dir++ = *s++;

		  *dir = L'\0';
	  }
  }

  /*********************************************************************
   *		_wmakepath (MSVCRT.@)
   *
   * Unicode version of _wmakepath.
   */
  void ceefit_call_spec fit_wmakepath(wchar_t *path, const wchar_t *drive, const wchar_t *directory, const wchar_t *filename, const wchar_t *extension)
  {
    wchar_t ch;

    if ( !path )
    {
      return;
    }

    path[0] = 0;
    if (drive && drive[0])
    {
      path[0] = drive[0];
      path[1] = ':';
      path[2] = 0;
    }
    if (directory && directory[0])
    {
      wcscat(path, directory);
      ch = path[wcslen(path) - 1];
      if (ch != '/' && ch != '\\')
      {
        static const wchar_t forwardSlashW[] = {L'/',0};
        wcscat(path, forwardSlashW);
      }
    }
    if (filename && filename[0])
    {
      wcscat(path, filename);
      if (extension && extension[0])
      {
        if ( extension[0] != '.' )
        {
          static const wchar_t dotW[] = {'.',0};
          wcscat(path, dotW);
        }
        wcscat(path, extension);
      }
    }
  }

  /* INTERNAL: Helper for _fullpath. Modified PD code from 'snippets'. */
  static void ceefit_call_spec wmsvcrt_fln_fix(wchar_t *path)
  {
    int dir_flag = 0, root_flag = 0;
    wchar_t *r, *p, *q, *s;
    wchar_t szbsdot[] = { '/', '.', 0 };

    /* Skip drive */
    if (NULL == (r = wcschr(path, ':')))
      r = path;
    else
      ++r;

    /* Ignore leading slashes */
    while ('/' == *r)
      if ('/' == r[1])
        wcscpy(r, &r[1]);
      else
      {
        root_flag = 1;
        ++r;
      }

    p = r; /* Change "//" to "/" */
    while (NULL != (p = wcschr(p, '/')))
      if ('/' ==  p[1])
        wcscpy(p, &p[1]);
      else
        ++p;

    while ('.' == *r) /* Scrunch leading ".\" */
    {
      if ('.' == r[1])
      {
        /* Ignore leading ".." */
        for (p = (r += 2); *p && (*p != '/'); ++p)
          ;
      }
      else
      {
        for (p = r + 1 ;*p && (*p != '/'); ++p)
	  ;
      }
      wcscpy(r, p + ((*p) ? 1 : 0));
    }

    while ('/' == path[wcslen(path)-1])   /* Strip last '//' */
    {
      dir_flag = 1;
      path[wcslen(path)-1] = '\0';
    }

    s = r;

    /* Look for "/." in path */

    while (NULL != (p = wcsstr(s, szbsdot)))
    {
      if ('.' == p[2])
      {
        /* Execute this section if ".." found */
        q = p - 1;
        while (q > r)           /* Backup one level           */
        {
          if (*q == '/')
            break;
          --q;
        }
        if (q > r)
        {
          wcscpy(q, p + 3);
          s = q;
        }
        else if ('.' != *q)
        {
          wcscpy(q + ((*q == '/') ? 1 : 0),
                  p + 3 + ((*(p + 3)) ? 1 : 0));
          s = q;
        }
        else  s = ++p;
      }
      else
      {
        /* Execute this section if "." found */
        q = p + 2;
        for ( ;*q && (*q != '/'); ++q)
          ;
        wcscpy(p, q);
      }
    }

    if (root_flag)  /* Embedded ".." could have bubbled up to root  */
    {
      for (p = r; *p && ('.' == *p || '/' == *p); ++p)
        ;
      if (r != p)
        wcscpy(r, p);
    }

    if (dir_flag)
    {
      wchar_t szbs[] = { '/', 0 };

      wcscpy(path, szbs);
    }
  }

  /*********************************************************************
   *		_wfullpath (MSVCRT.@)
   *
   * Unicode version of _fullpath.
   */
  wchar_t* ceefit_call_spec fit_wfullpath(wchar_t * absPath, const wchar_t* relPath, fit_size_t size)
  {
    if (!absPath)
      throw new EXCEPTION("Expected non null absPath to be passed in");

    wchar_t drive[5],dir[fit_MAX_PATH],file[fit_MAX_PATH],ext[fit_MAX_PATH];
    wchar_t res[fit_MAX_PATH];
    fit_size_t len;
    wchar_t szbs[] = { '/', 0 };

    res[0] = '\0';

    if (!relPath || !*relPath)
    {
      DYNARRAY<char> tempChar;
      tempChar.Reserve(size+1);
      getcwd(&tempChar[0], size);
      
      STRING tempUni(&tempChar[0]);
      wcscpy(absPath, tempUni.GetBuffer());
      return absPath;
    }

    if (size < 4)
    {
      errno = ERANGE;
      return NULL;
    }

    fit_wsplitpath(relPath, drive, dir, file, ext);

// drive letters only exist on WIN32
#ifdef _WIN32
    /* Get Directory and drive into 'res' */
    if (!dir[0] || (dir[0] != '/' && dir[0] != '\\'))
    {
      /* Relative or no directory given */
      _wgetdcwd(drive[0] ? unicode_toupper(drive[0]) - 'A' + 1 :  0, res, fit_MAX_PATH);
      wcscat(res,szbs);
      if (dir[0])
        wcscat(res,dir);
      if (drive[0])
        res[0] = drive[0]; /* If given a drive, preserve the letter case */
    }
#else
    if(!dir[0] || (dir[0] != '/' && dir[0] != '\\'))
    {
      /* Relative or no directory given */
      DYNARRAY<char> tempChar;
      tempChar.Reserve(size+1);
      getcwd(&tempChar[0], size);
      
      STRING tempUni(&tempChar[0]);
      wcscpy(res, tempUni.GetBuffer());
      wcscat(res, szbs);
      if(dir[0])
        wcscat(res, dir);
    }
#endif
    else
    {
      wcscpy(res,drive);
      wcscpy(res,dir);
    }

    wcscat(res,szbs);
    wcscat(res, file);
    wcscat(res, ext);

    // DW:  Replace any backslashes with forward slashes ...
    int resLen = wcslen(res);
    int i = -1;
    while(++i < resLen)
    {
      if(res[i] == L'\\')
      {
        res[i] = L'/';
      }
    }

    wmsvcrt_fln_fix(res);

    len = wcslen(res);
    if (len >= fit_MAX_PATH || len >= (fit_size_t)size)
      return NULL; /* FIXME: errno? */

    wcscpy(absPath,res);
    return absPath;
  }
};
