#ifndef __TOOLS_SPLITPATH_H__
#define __TOOLS_SPLITPATH_H__

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

#ifndef fit_MAX_PATH
#define fit_MAX_DRIVE          3
#define fit_MAX_FNAME          1024
#define fit_MAX_DIR            fit_MAX_FNAME
#define fit_MAX_EXT            fit_MAX_FNAME
#define fit_MAX_PATH           1024
#endif

namespace CEEFIT
{
  void ceefit_call_spec fit_wsplitpath(const wchar_t* path, wchar_t* drv, wchar_t* dir, wchar_t* name, wchar_t* ext);

  /*********************************************************************
   *		_wmakepath (MSVCRT.@)
   *
   * Unicode version of _wmakepath.
   */
  void ceefit_call_spec fit_wmakepath(wchar_t *path, const wchar_t *drive, const wchar_t *directory, const wchar_t *filename, const wchar_t *extension);

  /*********************************************************************
   *		_wfullpath (MSVCRT.@)
   *
   * Unicode version of _fullpath.
   */
  wchar_t* ceefit_call_spec fit_wfullpath(wchar_t * absPath, const wchar_t* relPath, size_t size);
};

#endif // __TOOLS_SPLITPATH_H__
