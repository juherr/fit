#ifndef __CEEFIT_ACTIONFIXTURE_H__
#define __CEEFIT_ACTIONFIXTURE_H__

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
  class ACTIONFIXTURE : public FIXTURE
  {
    protected:
      PTR<PARSE> Cells;
  
    public:
      static PTR<FIXTURE> Actor;      /**< Uhhh ... static?!?  Don't blame me, I copied Java!!  :)  */

    public:
      ceefit_init_spec ACTIONFIXTURE(void);
      virtual ceefit_init_spec ~ACTIONFIXTURE(void);

      // Traversal ////////////////////////////////

      virtual void ceefit_call_spec DoCells(PTR<PARSE>& cells);

      // Actions //////////////////////////////////

      virtual void ceefit_call_spec Start(void);
      virtual void ceefit_call_spec Enter(void);
      virtual void ceefit_call_spec Press(void);
      virtual void ceefit_call_spec ActionCheck(void);  // due to overload resolution difficulties that C++ has that java does not,
                                                        // this method has been renamed from Check() to ActionCheck().
                                                        // See the constructor in ActionFixture.cpp.  This method is registered
                                                        // to match with the name "check" to conform with Java Fit.

    protected:
      // Utility //////////////////////////////////

      virtual VALUE<CELLADAPTER> ceefit_call_spec Method(PTR<FIXTURE>& targetFixture, int args);
      virtual VALUE<CELLADAPTER> ceefit_call_spec Method(PTR<FIXTURE>& targetFixture, const STRING& test, int args);

      // -------------------------------------------------------
      // CeeFIT code used to manage static member data lifetimes
      // -------------------------------------------------------
    public:
      virtual void ceefit_call_spec ReleaseStatics(void); 

    protected:
      inline ACTIONFIXTURE& ceefit_call_spec operator=(const ACTIONFIXTURE&);
      inline ceefit_init_spec ACTIONFIXTURE(const ACTIONFIXTURE&);
  };
};

#endif // __CEEFIT_ACTIONFIXTURE_H__
