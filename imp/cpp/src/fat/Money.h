#ifndef __FAT_MONEY_H__
#define __FAT_MONEY_H__

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

#include "ceefit/mandatory.h"

namespace CEEFAT
{
  class MONEY : public ::CEEFIT::OBJECT 
  {
    protected:
      long cents;

    public:
      static MONEY ceefit_call_spec ValueOf(const STRING& s);
      ceefit_init_spec MONEY(void);
      ceefit_init_spec MONEY(const STRING& s);
      virtual ceefit_init_spec ~MONEY(void);
      MONEY& ceefit_call_spec operator=(const MONEY& obj);
      ceefit_init_spec MONEY(const MONEY& obj);
      virtual bool ceefit_call_spec IsEqual(const MONEY& value) const;
      virtual int ceefit_call_spec GetHashCode(void);
      virtual STRING ceefit_call_spec ToString(void) const;
  };

};

template<> class FITFIELD<CEEFAT::MONEY> : public ::CEEFIT::FITFIELDBASE<CEEFAT::MONEY>
{
  public:
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const CEEFAT::MONEY& in)
    {
      out = in.ToString();
    }

    inline bool ceefit_call_spec Parse(CEEFAT::MONEY& out, const CEEFIT::STRING& in)
    {
      try
      {
        out = CEEFAT::MONEY::ValueOf(in);
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
      static CEEFIT::STRING FieldType("CEEFAT::MONEY");

      return(FieldType);
    }

    template<class U> inline FITFIELD<CEEFAT::MONEY>& operator=(U& rValue)
    {
      this->FITFIELDBASE<CEEFAT::MONEY>::operator=(rValue);

      return(*this);
    }

    FITFIELD<CEEFAT::MONEY>& operator=(const CEEFAT::MONEY& rValue)
    {
      this->FITFIELDBASE<CEEFAT::MONEY>::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD<CEEFAT::MONEY>(void)
    {
    }

    virtual inline ceefit_init_spec ~FITFIELD<CEEFAT::MONEY>(void)
    {
    }

    virtual inline const type_info& GetTypeInfo(void) { return(typeid(FITFIELD<CEEFAT::MONEY>)); }

  private:
    ceefit_init_spec FITFIELD<CEEFAT::MONEY>(const FITFIELD<CEEFAT::MONEY>&);  /**< not implemented, do not call. */
};

#endif // __FAT_MONEY_H__