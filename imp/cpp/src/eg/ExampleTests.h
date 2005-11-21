#ifndef __EG_EXAMPLETESTS_H__
#define __EG_EXAMPLETESTS_H__

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

namespace EG
{
  class EXAMPLETESTS : public CEEFIT::COLUMNFIXTURE 
  {
    public:
      CEEFIT::STRING File;
      bool Wiki;

    protected:
      CEEFIT::STRING Input;
      CEEFIT::PTR<CEEFIT::FILERUNNER> Runner;
      //CEEFIT::PTR<CEEFIT::PARSE> Tables;
      //CEEFIT::PTR<CEEFIT::FIXTURE> Fixture;
      CEEFIT::PTR<CEEFIT::COUNTS> RunCounts;
      CEEFIT::STRING Footnote;

    public:
      ceefit_init_spec EXAMPLETESTS(void); 
      virtual ceefit_dtor_spec ~EXAMPLETESTS(void);
      virtual void ceefit_call_spec Run(void); 
      virtual int ceefit_call_spec Right(void); 
      virtual int ceefit_call_spec Wrong(void); 
      virtual int ceefit_call_spec Ignores(void); 
      virtual int ceefit_call_spec Exceptions(void); 

    protected:
      virtual CEEFIT::STRING ceefit_call_spec Read(const CEEFIT::STRING& input);

      // Footnote /////////////////////////////////

    protected:
      CEEFIT::PTR<CEEFIT::PARSE> FileCell;

    public:
      virtual void ceefit_call_spec DoRow(CEEFIT::PTR<CEEFIT::PARSE>& row); 
      virtual void ceefit_call_spec Wrong(CEEFIT::PTR<CEEFIT::PARSE>& cell); 

    private:
      ceefit_init_spec EXAMPLETESTS(const EXAMPLETESTS&);            /**< Not implemented, do not call. */
      EXAMPLETESTS& ceefit_call_spec operator=(const EXAMPLETESTS&); /**< Not implemented, do not call. */
  };
};

#endif // __EG_EXAMPLETESTS_H__
