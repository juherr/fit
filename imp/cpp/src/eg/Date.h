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

#ifndef __EG_DATE_H__
#define __EG_DATE_H__

namespace EG
{
  class DATE : public virtual CEEFIT::OBJECT
  {
    private:
      CEEFIT::fitINT64 Value;

    public:
      ceefit_init_spec DATE(void);
      virtual ceefit_init_spec ~DATE(void);
      DATE& ceefit_call_spec operator=(const DATE& aDate);
      ceefit_init_spec DATE(const DATE& aDate);

      virtual CEEFIT::fitINT64 GetTime(void) const;
      virtual CEEFIT::STRING ceefit_call_spec ToString(void) const;
      virtual void ceefit_call_spec Parse(const CEEFIT::STRING& aString);
      virtual bool ceefit_call_spec IsEqual(const DATE& aDate) const;
      virtual int ceefit_call_spec GetHashCode(void) const;
  };
};

template<> class FITFIELD< EG::DATE > : public CEEFIT::FITFIELDBASE< EG::DATE >
{
  public:
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const EG::DATE& in)
    {
      out = in.ToString();
    }

    inline bool ceefit_call_spec Parse(EG::DATE& out, const CEEFIT::STRING& in)
    {
      try
      {
        out.Parse(in);
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
      return("EG::DATE");
    }

    template<class U> inline FITFIELD< EG::DATE >& operator=(U& rValue)
    {
      this->FITFIELDBASE< EG::DATE >::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD< EG::DATE >(void)
    {
    }

    virtual inline ceefit_init_spec ~FITFIELD< EG::DATE >(void)
    {
    }

  private:
    FITFIELD< EG::DATE >(const FITFIELD< EG::DATE >&);              /**< not implemented, do not call. */
    FITFIELD< EG::DATE >& operator=(const FITFIELD< EG::DATE >&);   /**< not implemented, do not call. */
};

#endif // __EG_DATE_H__
