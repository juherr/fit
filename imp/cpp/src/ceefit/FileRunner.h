#ifndef __CEEFIT_FILERUNNER_H__
#define __CEEFIT_FILERUNNER_H__

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
  class FILERUNNER : public RUNNER
  {
    public:
      STRING Input;
      PTR<PARSE> Tables;
      PTR<FIXTURE> Fixture;
      WRITER* Output;

      ceefit_init_spec FILERUNNER(void);
      virtual ceefit_init_spec ~FILERUNNER(void);

      virtual int ceefit_call_spec Run(const DYNARRAY<STRING>& argv);
      virtual void ceefit_call_spec Process(void);
      virtual int ceefit_call_spec Args(const DYNARRAY<STRING>& argv);
      virtual STRING ceefit_call_spec Read(const STRING& inputFilename);
      virtual int ceefit_call_spec Exit(void);        

    protected:
      virtual void ceefit_call_spec Exception(EXCEPTION* e);
      virtual STRING ceefit_call_spec ValidateOutputPath(const STRING& rawOutPath);
      virtual void ceefit_call_spec ValidateOrCreateDir(const STRING& aDir);

    private:
      ceefit_init_spec FILERUNNER(const FILERUNNER&);   /**< Not implemented, do not call */
      FILERUNNER& ceefit_call_spec operator=(const FILERUNNER&);    /**< Not implemented, do not call */
  };
};

#endif // __CEEFIT_FILERUNNER_H__
