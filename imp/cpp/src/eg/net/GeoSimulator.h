#ifndef __EG_NET_SIMULATOR_H__
#define __EG_NET_SIMULATOR_H__

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

namespace EG_NET
{
  class SIMULATOR : public CEEFIT::FIXTURE 
  {
    public:
      CEEFIT::DYNARRAY<int> ZipCodes;
      CEEFIT::PTR<GEOCOORDINATE> Coordinate;
      int NodeCount;

      ceefit_init_spec SIMULATOR(void);
      virtual ceefit_dtor_spec ~SIMULATOR(void);
      virtual void ceefit_call_spec NewCity(void);
      virtual void ceefit_call_spec Ok(void);
      virtual void ceefit_call_spec Cancel(void);
      virtual void ceefit_call_spec Name(CEEFIT::STRING n);
      virtual void ceefit_call_spec SetZip(int* z);
      virtual CEEFIT::DYNARRAY<int> ceefit_call_spec Zip(void);
      virtual void ceefit_call_spec Population(float p);
      virtual void ceefit_call_spec SetCoord(CEEFIT::PTR<GEOCOORDINATE>& c);
      virtual GEOCOORDINATE ceefit_call_spec Coord(void);
      virtual int ceefit_call_spec Nodes(void);
      virtual void ceefit_call_spec Parse(CEEFIT::PTR<GEOCOORDINATE>& out, const CEEFIT::STRING& string);

    private:
      ceefit_init_spec SIMULATOR(const SIMULATOR&);
      SIMULATOR& ceefit_call_spec operator=(const SIMULATOR&);
  };
};

// Need to make a FITFIELD for DYNARRAY<int>
template<> class FITFIELD< CEEFIT::DYNARRAY<int> > : public CEEFIT::FITFIELDBASE< CEEFIT::DYNARRAY<int> >
{
  public:
    typedef CEEFIT::FITFIELDBASE< CEEFIT::DYNARRAY<int> > FIELDBASE;

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
      this->FIELDBASE::Assign(rValue);  /* dw 05/30/06 - operator= replaced with Assign */

      return(*this);
    }

    FITFIELD< CEEFIT::DYNARRAY<int> >(CEEFIT::DYNARRAY<int>& rValue) 
    {
      this->FIELDBASE::Assign(rValue);  /* dw 05/30/06 - operator= replaced with Assign */
    }

    inline ceefit_init_spec FITFIELD< CEEFIT::DYNARRAY<int> >(void)
    {
    }

    virtual inline ceefit_dtor_spec ~FITFIELD< CEEFIT::DYNARRAY<int> >(void)
    {
    }

  private:
    //FITFIELD< CEEFIT::DYNARRAY<int> >& operator=(const FITFIELD< CEEFIT::DYNARRAY<int> >&);  /**< not implemented, do not call. */
};

#endif // __EG_NET_SIMULATOR_H__
