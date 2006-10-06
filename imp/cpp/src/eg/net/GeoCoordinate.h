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

#ifndef __EG_NET_GEOCOORDINATE_H__
#define __EG_NET_GEOCOORDINATE_H__

namespace EG_NET
{
  class GEOCOORDINATE : public CEEFIT::REFCOUNTED
  {
    public:
      float Lat;
      float Lon;

      ceefit_init_spec GEOCOORDINATE(void);
      ceefit_init_spec GEOCOORDINATE(const GEOCOORDINATE& aGeoCoord);              
      ceefit_init_spec GEOCOORDINATE(float aLatitude, float aLongitude);
      GEOCOORDINATE& ceefit_call_spec operator=(const GEOCOORDINATE& aGeoCoord);
      virtual ceefit_dtor_spec ~GEOCOORDINATE(void);

      static CEEFIT::VALUE<GEOCOORDINATE> ceefit_call_spec Parse(const CEEFIT::STRING& string);

    private:
      static const double Precision;

      static CEEFIT::STRING ceefit_call_spec FloatToGeoCoordString(float aFloat);

    public:
      virtual bool ceefit_call_spec IsEqual(const GEOCOORDINATE& arg) const;
      virtual int ceefit_call_spec GetHashCode(void) const;
      virtual CEEFIT::STRING ceefit_call_spec ToString(void) const;

    private:
  };
};

template<> class FITFIELD< EG_NET::GEOCOORDINATE > : public CEEFIT::FITFIELDBASE< EG_NET::GEOCOORDINATE >
{
  public:
    typedef CEEFIT::FITFIELDBASE< EG_NET::GEOCOORDINATE > FIELDBASE;

    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const EG_NET::GEOCOORDINATE& in) const
    {
      out = in.ToString();
    }

    inline bool ceefit_call_spec Parse(EG_NET::GEOCOORDINATE& out, const CEEFIT::STRING& in)
    {
      try
      {
        CEEFIT::PTR<EG_NET::GEOCOORDINATE> theCoordinates(EG_NET::GEOCOORDINATE::Parse(in));
        out = *theCoordinates;
        return(true);
      }
      catch(CEEFIT::PARSEEXCEPTION* pe)
      {
        delete pe;
        return(false);
      }
    }

    virtual inline const char* ceefit_call_spec GetType(void) const
    {
      return("EG_NET::GEOCOORDINATE");
    }

    template<class U> inline FITFIELD< EG_NET::GEOCOORDINATE >& operator=(U& rValue)
    {
      this->FIELDBASE::Assign(rValue);  /* dw 05/30/06 - operator= replaced with Assign */

      return(*this);
    }

    inline ceefit_init_spec FITFIELD< EG_NET::GEOCOORDINATE >(void)
    {
    }

    virtual inline ceefit_dtor_spec ~FITFIELD< EG_NET::GEOCOORDINATE >(void)
    {
    }

  private:
    FITFIELD< EG_NET::GEOCOORDINATE >(const FITFIELD< EG_NET::GEOCOORDINATE >&);              /**< not implemented, do not call. */
    //FITFIELD< EG_NET::GEOCOORDINATE >& operator=(const FITFIELD< EG_NET::GEOCOORDINATE >&);   /**< not implemented, do not call. */
};

#endif // __EG_NET_GEOCOORDINATE_H__
