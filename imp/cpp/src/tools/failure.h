#ifndef __TOOLS_FAILURE_H__
#define __TOOLS_FAILURE_H__

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
  /**
   * <p>A virtual base class for all FAILURE types that can be thrown</p>
   */
  class FAILURE : public virtual OBJECT
  {
    protected:
      STRING Reason;

    public:
      ceefit_init_spec FAILURE(void);
      virtual ceefit_init_spec ~FAILURE(void);

      virtual STRING& ceefit_call_spec GetReason(void);

    private:
      FAILURE& operator=(const FAILURE&);     /**< Not implemented.  Do not call. */
      FAILURE(const FAILURE&);                /**< Not implemented.  Do not call. */
  };

  /**
   * Thrown when a fit_assert_*() macro fails.
   */
  class FITASSERTIONFAILED : public FAILURE
  {
    public:
      ceefit_init_spec FITASSERTIONFAILED(const char* aFile, int aLineNumber);
      virtual ceefit_init_spec ~FITASSERTIONFAILED(void);
      
    private:
      ceefit_init_spec FITASSERTIONFAILED(void);                          /**< Not implemented.  Do not call. */
      ceefit_init_spec FITASSERTIONFAILED(const FITASSERTIONFAILED&);     /**< Not implemented.  Do not call. */
  };

  /**
   * Thrown when the fit_fail() macro is called.
   */
  class FITFAILED : public FAILURE
  {
    public:
      ceefit_init_spec FITFAILED(const char* aFile, int aLineNumber);
      virtual ceefit_init_spec ~FITFAILED(void);

    private:
      ceefit_init_spec FITFAILED(void);                           /**< Not implemented.  Do not call. */
      ceefit_init_spec FITFAILED(const FITFAILED&);               /**< Not implemented.  Do not call. */
  };
};

#endif // __TOOLS_FAILURE_H__
