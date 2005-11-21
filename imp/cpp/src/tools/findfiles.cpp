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

#ifdef USE_GLOB_FOR_FIND

# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <glob.h>  

#else

# ifndef IO_H_IN_SYS_SUBFOLDER
#   include <io.h>
# else
#   include <sys/io.h>
# endif

#endif

#include "tools/alloc.h"
#include "ceefit.h"

namespace CEEFIT
{
  ceefit_init_spec FINDFILEINFO::FINDFILEINFO() 
  {
  }      
      
  ceefit_dtor_spec FINDFILEINFO::~FINDFILEINFO() 
  {
  }

  ceefit_init_spec FINDFILEINFO::FINDFILEINFO(const STRING& filePath, time_t lastUpdated, bool isFolder) 
  {
    FilePath = filePath;
    LastUpdateTime = lastUpdated;
    IsFolder = isFolder;
  }

  FINDFILEINFO& ceefit_call_spec FINDFILEINFO::operator=(const FINDFILEINFO& fileInfo) 
  {
    FilePath = fileInfo.FilePath;
    LastUpdateTime = fileInfo.LastUpdateTime;
    IsFolder = fileInfo.IsFolder;

    return(*this);
  }

  ceefit_init_spec FINDFILEINFO::FINDFILEINFO(const FINDFILEINFO& fileInfo) 
  {
    FilePath = fileInfo.FilePath;
    LastUpdateTime = fileInfo.LastUpdateTime;
    IsFolder = fileInfo.IsFolder;
  }

#ifdef USE_GLOB_FOR_FIND

// You know what?  This is retarded.  Why isn't _wfindfirst / _wfindnext implemented on POSIX?  Why do I have to write this
// class?  

// And now, an open letter to the president of the POSIX foundation or whatever:

//    You are making me call a function called 'glob()'!!!  WHAT WERE YOU THINKING!?!  You know what, let's name all of POSIX
//    functions something gross!  How about 'spew()' or 'lung_cheese()'?  Take a note:  findfirst and findnext sound a lot more
//    professional, if only slightly more readable...

  class FINDITERATORIMPL : public REFCOUNTED
  {
    private:
      char** curItem;
      ::glob_t globBuffer;
      bool NoMatches;

    public:
      ceefit_init_spec FINDITERATORIMPL(const STRING& searchPattern);
      virtual ceefit_dtor_spec ~FINDITERATORIMPL(void); 
      virtual bool ceefit_call_spec HasNext(void);
      virtual FINDFILEINFO ceefit_call_spec GetNext(void);
  
    private:
      ceefit_init_spec FINDITERATORIMPL(void);
      ceefit_init_spec FINDITERATORIMPL(const FINDITERATORIMPL&);
      FINDITERATORIMPL& ceefit_call_spec operator=(const FINDITERATORIMPL&);
  };
  
  FINDITERATORIMPL::FINDITERATORIMPL(const STRING& searchPattern) 
  {
    NoMatches = false;

    DYNARRAY<char> searchChars;
    searchPattern.GetAsCharArray(searchChars);

    memset(&globBuffer, 0, sizeof(::glob_t));
    int retVal;
    if((retVal = ::glob(&searchChars[0], 0, NULL, &globBuffer)) != 0)
    {
      if(retVal == GLOB_NOMATCH)
      {
        NoMatches = true;
        return;
      }
      else
      {
        throw new EXCEPTION(STRING("Failed to find:  ") + searchPattern);
      }
    }
    curItem = globBuffer.gl_pathv;
  }

  FINDITERATORIMPL::~FINDITERATORIMPL() 
  {
    if(NoMatches)
    {
      return;
    }
    ::globfree(&globBuffer);
  }

  bool FINDITERATORIMPL::HasNext()
  {
    if(NoMatches)
    {
      return(false);
    }
    else
    {
      return(*curItem != NULL);
    }
  }

  FINDFILEINFO FINDITERATORIMPL::GetNext()
  {
    if(NoMatches)
    {
      throw new BOUNDSEXCEPTION("No more files");
    }

    const char* retVal = *curItem;

    if(retVal == NULL)
    {
      throw new BOUNDSEXCEPTION("No more files");
    }

    curItem++;

    struct stat statVal;
    if(stat(retVal, &statVal) != 0)
    {
      throw new EXCEPTION("Could not stat to get the last modified date");
    }

    return(FINDFILEINFO(retVal, statVal.st_mtime, statVal.st_mode & S_IFMT == S_IFDIR));
  }

#else

  class FINDITERATORIMPL : public FINDITERATOR
  {
    private:
      long findHandle;
      bool itemReady;
#     if defined(__BORLANDC__)
        struct _wffblk findData;
#     else
        struct _wfinddata_t findData;
#     endif

    public:
      ceefit_init_spec FINDITERATORIMPL(const STRING& searchPattern);
      virtual ceefit_dtor_spec ~FINDITERATORIMPL(void);
      virtual bool ceefit_call_spec HasNext(void);
      virtual FINDFILEINFO ceefit_call_spec GetNext(void);
  
    private:
      ceefit_init_spec FINDITERATORIMPL(void);
      ceefit_init_spec FINDITERATORIMPL(const FINDITERATORIMPL&);
      FINDITERATORIMPL& ceefit_call_spec operator=(const FINDITERATORIMPL&);
  };

  ceefit_init_spec FINDITERATORIMPL::FINDITERATORIMPL(const STRING& searchPattern) 
  {
    itemReady = false;
    STRING temp(searchPattern);

#   if defined(__BORLANDC__)
      memset(&findData, 0, sizeof(struct _wffblk));      
      findHandle = _wfindfirst(temp.GetBuffer(), &findData, FA_RDONLY | FA_HIDDEN | FA_ARCH | FA_SYSTEM | FA_DIREC);
#   else
      memset(&findData, 0, sizeof(struct _wfinddata_t));
      findHandle = _wfindfirst(temp.GetBuffer(), &findData);
#   endif  

    if(findHandle == -1)
    {
      if(errno == ENOENT)
      {
        itemReady = false;
      }
      else if(errno == EINVAL)
      {
        throw new IOEXCEPTION(STRING("Bad pathspec, search failed:  ") + searchPattern);
      }
      else
      {
        throw new IOEXCEPTION(STRING("IOEXCEPTION while searching for:  ") + searchPattern);
      }
    }
    else
    {
      itemReady = true;
    }
  }

  ceefit_dtor_spec FINDITERATORIMPL::~FINDITERATORIMPL()
  {
    if(findHandle != -1)
    {
      _findclose(findHandle);
    }
  }

  bool ceefit_call_spec FINDITERATORIMPL::HasNext()
  {
    if(itemReady == true)
    {
      return(true);
    }

    if(findHandle == -1)
    {
      return(false);
    }

# if defined(__BORLANDC__)
    if(_wfindnext(&findData) != 0)
# else
    if(_wfindnext(findHandle, &findData) != 0)
# endif
    {
      return(false);
    }
    itemReady = true;
    return(true);
  }

  FINDFILEINFO ceefit_call_spec FINDITERATORIMPL::GetNext()
  {
    if(itemReady == false)
    {
      if(!HasNext())
      {
        throw new BOUNDSEXCEPTION("No more files");
      }
    }
    
    itemReady = false;
#   if defined(__BORLANDC__)
      return(FINDFILEINFO(STRING(findData.ff_name), findData.ff_ftime, (findData.ff_attrib & FA_DIREC) == FA_DIREC));
#   else
      return(FINDFILEINFO(STRING(findData.name), findData.time_write, (findData.attrib & _A_SUBDIR) == _A_SUBDIR));
#   endif
  }

#endif

  VALUE<FINDITERATOR> ceefit_call_spec fit_FindFiles(const STRING& searchPattern)
  {
    return(VALUE<FINDITERATOR>(new FINDITERATORIMPL(searchPattern)));    
  }
};
