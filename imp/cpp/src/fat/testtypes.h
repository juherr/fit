#ifndef __FAT_TESTTYPES_H__
#define __FAT_TESTTYPES_H__

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
template<> class FITFIELD< CEEFIT::DYNARRAY<int> > : public CEEFIT::FITFIELDBASE< CEEFIT::DYNARRAY<int> >
{
  public:
    typedef CEEFIT::FITFIELDBASE< CEEFIT::DYNARRAY<int> > FIELDBASE;

    fit_using_decl(FIELDBASE::ToString);
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const CEEFIT::DYNARRAY<int>& in) const
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

      CEEFIT::Tokenize(tokens, in, CEEFIT::STRING(","), false);

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

    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      return("DYNARRAY<int>");
    }

    template<class U> inline FITFIELD< CEEFIT::DYNARRAY<int> >& operator=(U& rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    FITFIELD< CEEFIT::DYNARRAY<int> >& operator=(const CEEFIT::DYNARRAY<int>& rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD< CEEFIT::DYNARRAY<int> >(void)
    {
    }

    virtual inline ceefit_dtor_spec ~FITFIELD< CEEFIT::DYNARRAY<int> >(void)
    {
    }

  private:
    FITFIELD< CEEFIT::DYNARRAY<int> >(const FITFIELD< CEEFIT::DYNARRAY<int> >&);  /**< not implemented, do not call. */
};

template<> class FITFIELD< CEEFIT::DYNARRAY<bool> > : public CEEFIT::FITFIELDBASE< CEEFIT::DYNARRAY<bool> >
{
  public:
    typedef CEEFIT::FITFIELDBASE< CEEFIT::DYNARRAY<bool> > FIELDBASE;

    fit_using_decl(FIELDBASE::ToString);
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const CEEFIT::DYNARRAY<bool>& in) const 
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

      CEEFIT::Tokenize(tokens, in, CEEFIT::STRING(","), false);

      out.Reset();

      int i = -1;
      while(++i < tokens.GetSize())
      {
        CEEFIT::STRING temp(tokens[i].Trim());

        out.Add(temp.IsEqual("true") ? true : false);
      }
      return(true);
    }

    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      return("DYNARRAY<bool>");
    }

    template<class U> inline FITFIELD< CEEFIT::DYNARRAY<bool> >& operator=(U& rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    FITFIELD< CEEFIT::DYNARRAY<bool> >& operator=(const CEEFIT::DYNARRAY<bool>& rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD< CEEFIT::DYNARRAY<bool> >(void)
    {
    }

    virtual inline ceefit_dtor_spec ~FITFIELD< CEEFIT::DYNARRAY<bool> >(void)
    {
    }

  private:
    FITFIELD< CEEFIT::DYNARRAY<bool> >(const FITFIELD< CEEFIT::DYNARRAY<bool> >&);  /**< not implemented, do not call. */
    //FITFIELD< CEEFIT::DYNARRAY<bool> >& operator=(const FITFIELD< CEEFIT::DYNARRAY<bool> >&);  /**< not implemented, do not call. */
};

template<> class FITFIELD< CEEFIT::DYNARRAY<CEEFIT::STRING> > : public CEEFIT::FITFIELDBASE< CEEFIT::DYNARRAY<CEEFIT::STRING> >
{
  public:
    typedef CEEFIT::FITFIELDBASE< CEEFIT::DYNARRAY<CEEFIT::STRING> > FIELDBASE;

    fit_using_decl(FIELDBASE::ToString);
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const CEEFIT::DYNARRAY<CEEFIT::STRING>& in) const
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

      CEEFIT::Tokenize(tokens, in, CEEFIT::STRING(","), false);

      out.Reset();

      int i = -1;
      while(++i < tokens.GetSize())
      {
        out.Add(tokens[i].Trim());
      }
      return(true);
    }

    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      return("DYNARRAY<STRING>");
    }

    template<class U> inline FITFIELD< CEEFIT::DYNARRAY<CEEFIT::STRING> >& operator=(U& rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    FITFIELD< CEEFIT::DYNARRAY<CEEFIT::STRING> >& operator=(const CEEFIT::DYNARRAY<CEEFIT::STRING>& rValue)
    {
      this->FIELDBASE::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD< CEEFIT::DYNARRAY<CEEFIT::STRING> >(void)
    {
    }

    virtual inline ceefit_dtor_spec ~FITFIELD< CEEFIT::DYNARRAY<CEEFIT::STRING> >(void)
    {
    }

  private:
    FITFIELD< CEEFIT::DYNARRAY<CEEFIT::STRING> >(const FITFIELD< CEEFIT::DYNARRAY<CEEFIT::STRING> >&);  /**< not implemented, do not call. */
    //FITFIELD< CEEFIT::DYNARRAY<CEEFIT::STRING> >& operator=(const FITFIELD< CEEFIT::DYNARRAY<CEEFIT::STRING> >&);  /**< not implemented, do not call. */
};


#endif // __FAT_TESTTYPES_H__
