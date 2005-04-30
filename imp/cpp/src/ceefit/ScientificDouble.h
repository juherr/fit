#ifndef __PRIVATE_SCIENTIFICDOUBLE_H__
#define __PRIVATE_SCIENTIFICDOUBLE_H__

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

namespace CEEFIT
{
  class SCIENTIFICDOUBLE
  {
    protected:
      double Value;
      double Precision;
    
      static double ceefit_call_spec ParseDouble(const STRING& s);
      static bool ceefit_call_spec IsNaN(double aDouble);

    public:
      ceefit_init_spec SCIENTIFICDOUBLE(double value);
      ceefit_init_spec SCIENTIFICDOUBLE(void);
      ceefit_init_spec SCIENTIFICDOUBLE(const SCIENTIFICDOUBLE&);
      virtual ceefit_init_spec ~SCIENTIFICDOUBLE(void);
      SCIENTIFICDOUBLE& ceefit_call_spec operator=(const SCIENTIFICDOUBLE& obj);
      SCIENTIFICDOUBLE& ceefit_call_spec operator=(const STRING& obj);
      SCIENTIFICDOUBLE& ceefit_call_spec operator=(const double& obj);

      static SCIENTIFICDOUBLE ceefit_call_spec ValueOf(const STRING& s);
      static double ceefit_call_spec CalcPrecision(const STRING& s);
      static STRING ceefit_call_spec Tweak(const STRING& s);

      virtual bool ceefit_call_spec IsEqual(const SCIENTIFICDOUBLE& obj) const;
      virtual int ceefit_call_spec CompareTo(const SCIENTIFICDOUBLE& obj) const; 
      virtual STRING ceefit_call_spec ToString(void) const;
      virtual double ceefit_call_spec DoubleValue(void) const; 
      virtual float ceefit_call_spec FloatValue(void) const;
      virtual long ceefit_call_spec LongValue(void) const;
      virtual int ceefit_call_spec IntValue(void) const;
  };
};

template<> class FITFIELD<CEEFIT::SCIENTIFICDOUBLE> : public FITFIELDBASE<CEEFIT::SCIENTIFICDOUBLE>
{
  public:
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const CEEFIT::SCIENTIFICDOUBLE& in)
    {
      out = in.ToString();
    }

    inline bool ceefit_call_spec Parse(CEEFIT::SCIENTIFICDOUBLE& out, const CEEFIT::STRING& in)
    {
      try
      {
        out = CEEFIT::SCIENTIFICDOUBLE::ValueOf(in);
        return(true);
      }
      catch(CEEFIT::PARSEEXCEPTION* pe) 
      {
        delete pe;
        return(false);
      }
    }

    virtual inline const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
      static CEEFIT::STRING FieldType("CEEFIT::SCIENTIFICDOUBLE");

      return(FieldType);
    }

    template<class U> inline FITFIELD<CEEFIT::SCIENTIFICDOUBLE>& ceefit_call_spec operator=(U& rValue)
    {
      FITFIELDBASE<CEEFIT::SCIENTIFICDOUBLE>::operator=(rValue);

      return(*this);
    }

    FITFIELD<CEEFIT::SCIENTIFICDOUBLE>& ceefit_call_spec operator=(const CEEFIT::SCIENTIFICDOUBLE& rValue)
    {
      FITFIELDBASE<CEEFIT::SCIENTIFICDOUBLE>::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<CEEFIT::SCIENTIFICDOUBLE>(void)
    {
    }

    virtual inline ceefit_init_spec ~FITFIELD<CEEFIT::SCIENTIFICDOUBLE>(void)
    {
    }

  private:
    ceefit_init_spec FITFIELD<CEEFIT::SCIENTIFICDOUBLE>(const FITFIELD<CEEFIT::SCIENTIFICDOUBLE>&);  /**< not implemented, do not call. */
};

#endif // __SCIENTIFICDOUBLE_H__
