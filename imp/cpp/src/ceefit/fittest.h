#ifndef __CEEFIT_FITTEST_H__
#define __CEEFIT_FITTEST_H__

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

class FITTESTBASE : public CEEFIT::CELLADAPTER
{
  protected:
    CEEFIT::STRING Name;

  public:
    ceefit_init_spec FITTESTBASE(void);
    virtual ceefit_init_spec ~FITTESTBASE();

    const CEEFIT::STRING& ceefit_call_spec GetName(void) const;
    void ceefit_call_spec SetName(const CEEFIT::STRING& testName);

    virtual CEEFIT::CELLADAPTER* ceefit_call_spec Invoke(CEEFIT::FIXTURE* aFixture)=0;

    virtual const CEEFIT::STRING& ceefit_call_spec GetType(void) const=0;

    virtual CEEFIT::CELLADAPTER* ceefit_call_spec NewInstanceParse(const CEEFIT::STRING& aText);

    void ceefit_call_spec WriteToFixtureVar(const CEEFIT::STRING&);
    void ceefit_call_spec ReadFromFixtureVar(CEEFIT::STRING&);

    inline bool ceefit_call_spec IsMethod(void) const
    {
      return(true);
    }

    inline bool ceefit_call_spec IsField(void) const
    {
      return(false);
    }

  private:
    /**
     * Not implemented.  Do not call.
     */
    FITTESTBASE(const FITTESTBASE&);

    /**
     * Not implemented.  Do not call.
     */
    FITTESTBASE& operator=(const FITTESTBASE&);
};

template<class T, class TESTCALLCLASS> class FITTEST : public FITTESTBASE
{
  public:
    inline ceefit_init_spec FITTEST<T, TESTCALLCLASS>(void)
    {
    }

    virtual inline ceefit_init_spec ~FITTEST<T, TESTCALLCLASS>(void)
    {
    }

    virtual inline CEEFIT::CELLADAPTER* ceefit_call_spec Invoke(CEEFIT::FIXTURE* aFixture)
    {
      FITFIELD<T>* retVal = new FITFIELD<T>();
      bool testSuccess = true;
      const char* fileName = NULL;
      int lineNum = 0;

      try
      {
        retVal->SetName(CEEFIT::STRING("<") + this->GetName() + "() return value>");
        (*retVal) = TESTCALLCLASS::CallFitTest(aFixture, testSuccess, fileName, lineNum);
      }
      catch(...)
      {
        delete retVal;
        throw;    // rethrow, hopefully ...
      }

      return(retVal);
    }

    virtual inline const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
      static FITFIELD<T> typeField;

      return(typeField.GetType());
    }

  private:
    /**
     * Not implemented.  Do not call.
     */
    FITTEST<T, TESTCALLCLASS>(const FITTEST<T, TESTCALLCLASS>&);

    /**
     * Not implemented.  Do not call.
     */
    FITTEST<T, TESTCALLCLASS>& operator=(const FITTEST<T, TESTCALLCLASS>&);
};


#endif // __CEEFIT_FITTEST_H__
