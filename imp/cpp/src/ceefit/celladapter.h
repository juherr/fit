#ifndef __CEEFIT_CELLADAPTER_H__
#define __CEEFIT_CELLADAPTER_H__

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
  class FIXTURE;

  /**
   * <p>This class is an attempt to simulate a lot of what the Java Fit's TypeAdapter class does.</p>
   *
   * <p>I like to think of this class as the "HTML Table Cell Type/Field/Method Adapter" because cells
   * in Fit tables can be either fields or return values to match with method calls.</p>
   */
  class CELLADAPTER : public REFCOUNTED, public SLINK<CELLADAPTER>
  {
    private:
      friend class RUNNER;

    public:
      virtual void ceefit_call_spec WriteToFixtureVar(const CEEFIT::STRING& in)=0;
      virtual void ceefit_call_spec ReadFromFixtureVar(CEEFIT::STRING& out)=0;

      /**
       * <p>Special variant of ReadFromFixtureVar that allows you to execute on a different FIXTURE.</p>
       *
       * <p>Only used in special cases like RowFixture</p>
       */
      virtual void ceefit_call_spec ReadFromFixtureVar(CEEFIT::STRING& out, FIXTURE* aFixture);

      virtual void ceefit_call_spec SetName(const CEEFIT::STRING& aName)=0;
      virtual const CEEFIT::STRING& ceefit_call_spec GetName(void) const=0;

      /**
       * <p>This returns a unique string per each type, specified by the developer.</p>
       *
       * <p>If the cell is a function type, then the string returned is the type of the return value for the function.</p>
       */
      virtual const CEEFIT::STRING& ceefit_call_spec GetType(void) const=0;

      virtual bool ceefit_call_spec IsMethod(void) const=0;
      virtual bool ceefit_call_spec IsField(void) const=0;

      /**
       * <p>Creates a new instance of the CELLADAPTER type
       */
      virtual void ceefit_call_spec NewInstanceParse(PTR<CELLADAPTER>& out, const CEEFIT::STRING& aText)=0;

      /**
       * <p>Invoke test fixture method, only function-type cells can be invoked</p>
       *
       * @param out PTR<CELLADAPTER> Containing the return value from the fixture test method call.
       */
      virtual void ceefit_call_spec Invoke(PTR<CELLADAPTER>& out, FIXTURE* aFixture)=0;

      /**
       * <p>Invoke test fixture method, where the FIXTURE is supplied by a CELLADAPTER</p>
       *
       * @param out PTR<CELLADAPTER> Containing the return value from the fixture test method call.
       * @param aFixture only valid type of CELLADAPTER that can be accepted here is a FITFIXTURECONTAINER
       */
      virtual void ceefit_call_spec Invoke(PTR<CELLADAPTER>& out, CELLADAPTER* fixtureContainer);

      inline ceefit_init_spec CELLADAPTER(void) {}
      virtual inline ceefit_init_spec ~CELLADAPTER(void) {}

      virtual int ceefit_call_spec GetHashCode(void)=0;
      virtual bool ceefit_call_spec IsEqual(const CELLADAPTER* aAdapter) const=0;

      virtual inline const type_info& GetTypeInfo(void) { return(typeid(CELLADAPTER)); }

    protected:
      inline ceefit_init_spec CELLADAPTER(CELLADAPTER&);     /**< not implemented, do not call. */
      inline CELLADAPTER& ceefit_call_spec operator=(const CELLADAPTER&);   /**< not implemented, do not call. */
  };

};

#endif // __CEEFIT_CELLADAPTER_H__
