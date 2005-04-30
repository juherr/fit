#ifndef __FAT_TESTTYPES_H__
#define __FAT_TESTTYPES_H__

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

// Need to make a FITFIELD for DYNARRAY<int>
template<> class FITFIELD< CEEFIT::DYNARRAY<int> > : public FITFIELDBASE< CEEFIT::DYNARRAY<int> >
{
  public:
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const CEEFIT::DYNARRAY<int>& in)
    {
      CEEFIT::STRING temp;
      int i = -1;
      bool isFirst = true;
      while(++i < in.GetSize())
      {
        if(!isFirst)
        {
          out += ", ";
        }
        CEEFIT::SafeSprintf(temp, L"%i", in[i]);
        out += temp;

        isFirst = false;
      }
    }

    inline bool ceefit_call_spec Parse(CEEFIT::DYNARRAY<int>& out, const CEEFIT::STRING& in)
    {
      CEEFIT::DYNARRAY<CEEFIT::STRING> tokens;

      CEEFIT::TokenizeRespectQuotes(tokens, in, CEEFIT::STRING(","), false);

      out.Reset();

      int i = -1;
      while(++i < tokens.GetSize())
      {
        int temp = 0;
        bool retVal = (swscanf(tokens[i].Trim().GetBuffer(), L"%i", &temp)==1);
        out.Add(temp);
        if(retVal == false)
        {
          return(false);
        }
      }
      return(true);
    }

    virtual inline const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
      static CEEFIT::STRING FieldType("DYNARRAY<int>");

      return(FieldType);
    }

    template<class U> inline CEEFIT::DYNARRAY<int>& ceefit_call_spec operator=(U& rValue)
    {
      return(FITFIELDBASE< CEEFIT::DYNARRAY<int> >::operator=(rValue));
    }

    inline ceefit_init_spec FITFIELD< CEEFIT::DYNARRAY<int> >(void)
    {
    }

    virtual inline ceefit_init_spec ~FITFIELD< CEEFIT::DYNARRAY<int> >(void)
    {
    }

  private:
    FITFIELD< CEEFIT::DYNARRAY<int> >(const FITFIELD< CEEFIT::DYNARRAY<int> >&);  /**< not implemented, do not call. */
    //FITFIELD< CEEFIT::DYNARRAY<int> >& operator=(const FITFIELD< CEEFIT::DYNARRAY<int> >&);  /**< not implemented, do not call. */
};

template<> class FITFIELD< CEEFIT::DYNARRAY<bool> > : public FITFIELDBASE< CEEFIT::DYNARRAY<bool> >
{
  public:
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const CEEFIT::DYNARRAY<bool>& in)
    {
      CEEFIT::STRING temp;
      int i = -1;
      bool isFirst = true;
      while(++i < in.GetSize())
      {
        if(!isFirst)
        {
          out += ", ";
        }
        CEEFIT::SafeSprintf(temp, L"%s", in[i] ? "true" : "false");
        out += temp;

        isFirst = false;
      }
    }

    inline bool ceefit_call_spec Parse(CEEFIT::DYNARRAY<bool>& out, const CEEFIT::STRING& in)
    {
      CEEFIT::DYNARRAY<CEEFIT::STRING> tokens;

      CEEFIT::TokenizeRespectQuotes(tokens, in, CEEFIT::STRING(","), false);

      out.Reset();

      int i = -1;
      while(++i < tokens.GetSize())
      {
        CEEFIT::STRING temp(tokens[i].Trim());

        out.Add(temp.IsEqual("true") ? true : false);
      }
      return(true);
    }

    virtual inline const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
      static CEEFIT::STRING FieldType("DYNARRAY<bool>");

      return(FieldType);
    }

    template<class U> inline CEEFIT::DYNARRAY<bool>& ceefit_call_spec operator=(U& rValue)
    {
      return(FITFIELDBASE< CEEFIT::DYNARRAY<bool> >::operator=(rValue));
    }

    inline ceefit_init_spec FITFIELD< CEEFIT::DYNARRAY<bool> >(void)
    {
    }

    virtual inline ceefit_init_spec ~FITFIELD< CEEFIT::DYNARRAY<bool> >(void)
    {
    }

  private:
    FITFIELD< CEEFIT::DYNARRAY<bool> >(const FITFIELD< CEEFIT::DYNARRAY<bool> >&);  /**< not implemented, do not call. */
    //FITFIELD< CEEFIT::DYNARRAY<bool> >& operator=(const FITFIELD< CEEFIT::DYNARRAY<bool> >&);  /**< not implemented, do not call. */
};

template<> class FITFIELD< CEEFIT::DYNARRAY<CEEFIT::STRING> > : public FITFIELDBASE< CEEFIT::DYNARRAY<CEEFIT::STRING> >
{
  public:
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const CEEFIT::DYNARRAY<CEEFIT::STRING>& in)
    {
      CEEFIT::STRING temp;
      int i = -1;
      bool isFirst = true;
      while(++i < in.GetSize())
      {
        if(!isFirst)
        {
          out += ", ";
        }
        out += in[i];

        isFirst = false;
      }
    }

    inline bool ceefit_call_spec Parse(CEEFIT::DYNARRAY<CEEFIT::STRING>& out, const CEEFIT::STRING& in)
    {
      CEEFIT::DYNARRAY<CEEFIT::STRING> tokens;

      CEEFIT::TokenizeRespectQuotes(tokens, in, CEEFIT::STRING(","), false);

      out.Reset();

      int i = -1;
      while(++i < tokens.GetSize())
      {
        out.Add(tokens[i].Trim());
      }
      return(true);
    }

    virtual inline const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
      static CEEFIT::STRING FieldType("DYNARRAY<STRING>");

      return(FieldType);
    }

    template<class U> inline CEEFIT::DYNARRAY<CEEFIT::STRING>& ceefit_call_spec operator=(U& rValue)
    {
      return(FITFIELDBASE< CEEFIT::DYNARRAY<CEEFIT::STRING> >::operator=(rValue));
    }

    inline ceefit_init_spec FITFIELD< CEEFIT::DYNARRAY<CEEFIT::STRING> >(void)
    {
    }

    virtual inline ceefit_init_spec ~FITFIELD< CEEFIT::DYNARRAY<CEEFIT::STRING> >(void)
    {
    }

  private:
    FITFIELD< CEEFIT::DYNARRAY<CEEFIT::STRING> >(const FITFIELD< CEEFIT::DYNARRAY<CEEFIT::STRING> >&);  /**< not implemented, do not call. */
    //FITFIELD< CEEFIT::DYNARRAY<CEEFIT::STRING> >& operator=(const FITFIELD< CEEFIT::DYNARRAY<CEEFIT::STRING> >&);  /**< not implemented, do not call. */
};


#endif // __FAT_TESTTYPES_H__
