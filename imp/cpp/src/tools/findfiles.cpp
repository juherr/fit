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

// You know what?  This is retarded.  Why isn't _wfindfirst / _wfindnext implemented on POSIX?  Why do I have to write this
// class?  

// And now, an open letter to the president of the POSIX foundation or whatever:

//    You are making me call a function called 'glob()'!!!  WHAT WERE YOU THINKING!?!  You know what, let's name all of POSIX
//    functions something gross!  How about 'spew()' or 'lung_cheese()'?  Take a note:  findfirst and findnext sound a lot more
//    professional, if only slightly more readable...

namespace CEEFIT
{
  FINDFILEINFO::FINDFILEINFO() 
  {
  }      
      
  FINDFILEINFO::~FINDFILEINFO() 
  {
  }

  FINDFILEINFO::FINDFILEINFO(const STRING& filePath, time_t lastUpdated) 
  {
    FilePath = filePath;
    LastUpdateTime = lastUpdated;
  }

  FINDFILEINFO& FINDFILEINFO::operator=(const FINDFILEINFO& fileInfo) 
  {
    FilePath = fileInfo.FilePath;
    LastUpdateTime = fileInfo.LastUpdateTime;

    return(*this);
  }

  FINDFILEINFO::FINDFILEINFO(const FINDFILEINFO& fileInfo) 
  {
    FilePath = fileInfo.FilePath;
    LastUpdateTime = fileInfo.LastUpdateTime;
  }

#ifdef USE_GLOB_FOR_FIND
  FINDITERATOR::FINDITERATOR(const STRING& searchPattern) 
  {
    NoMatches = false;

    DYNARRAY<char> searchChars;
    searchPattern.GetAsCharArray(searchChars);

    memset(&globBuffer, 0, sizeof(::glob_t));
    int retVal;
    if((retVal = glob(&searchChars[0], 0, NULL, &globBuffer)) != 0)
    {
      if(retVal == GLOB_NOMATCH)
      {
        NoMatches = true;
      }

      throw new EXCEPTION("Failed to find:  " + searchPattern);
    }
    curItem = globBuffer.gl_pathv;
  }

  FINDITERATOR::~FINDITERATOR() 
  {
    if(NoMatches)
    {
      return;
    }
    globfree(&globBuffer);
  }

  bool FINDITERATOR::HasNext()
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

  FINDFILEINFO FINDITERATOR::GetNext()
  {
    if(NoMatches)
    {
      throw new BOUNDSEXCEPTION("No more files");
    }

    wp char* retVal = *curItem;

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

    return(FINDFILEINFO(retVal, statVal.st_mtime));
  }

#else
  FINDITERATOR::FINDITERATOR(const STRING& searchPattern) 
  {
    itemReady = false;
    memset(&findData, 0, sizeof(struct _wfinddata_t));
    findHandle = _wfindfirst(searchPattern.GetBuffer(), &findData);

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

  FINDITERATOR::~FINDITERATOR()
  {
    if(findHandle != -1)
    {
      _findclose(findHandle);
    }
  }

  bool FINDITERATOR::HasNext()
  {
    if(itemReady == true)
    {
      return(true);
    }

    if(findHandle == -1)
    {
      return(false);
    }

    if(_wfindnext(findHandle, &findData) != 0)
    {
      return(false);
    }
    itemReady = true;
    return(true);
  }

  FINDFILEINFO FINDITERATOR::GetNext()
  {
    if(itemReady == false)
    {
      if(!HasNext())
      {
        throw new BOUNDSEXCEPTION("No more files");
      }
    }
    
    itemReady = false;

    return(FINDFILEINFO(STRING(findData.name), findData.time_write));
  }

#endif

  VALUE<FINDITERATOR> ceefit_call_spec fit_FindFiles(const STRING& searchPattern)
  {
    return(VALUE<FINDITERATOR>(new FINDITERATOR(searchPattern)));    
  }
};
