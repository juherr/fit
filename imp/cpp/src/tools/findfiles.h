#ifndef __TOOLS_FINDFILES_H__
#define __TOOLS_FINDFILES_H__

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
  class FINDFILEINFO
  {
    public:
      STRING FilePath;
      time_t LastUpdateTime;
      
      ceefit_init_spec FINDFILEINFO(void);
      virtual ceefit_init_spec ~FINDFILEINFO(void);
      ceefit_init_spec FINDFILEINFO(const STRING& filePath, time_t lastUpdated);
      FINDFILEINFO& ceefit_call_spec operator=(const FINDFILEINFO& fileInfo);
      ceefit_init_spec FINDFILEINFO(const FINDFILEINFO& fileInfo);
  };

  class FINDITERATOR : public REFCOUNTED
  {
    public:
      virtual inline ceefit_init_spec ~FINDITERATOR(void) {}
      virtual bool ceefit_call_spec HasNext(void)=0;
      virtual FINDFILEINFO ceefit_call_spec GetNext(void)=0;
  
    protected:
      inline ceefit_init_spec FINDITERATOR(void) {}
      inline ceefit_init_spec FINDITERATOR(const FINDITERATOR& aIterator) {}
      inline FINDITERATOR& ceefit_call_spec operator=(const FINDITERATOR& aIterator) { return(*this); }
  };
};

namespace CEEFIT
{
  VALUE<FINDITERATOR> ceefit_call_spec fit_FindFiles(const STRING& searchPattern);
};

#endif // __TOOLS_FINDFILES_H__
