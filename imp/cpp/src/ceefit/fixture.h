#ifndef __CEEFIT_FIXTURE_H__
#define __CEEFIT_FIXTURE_H__

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
  class PARSE;
  class EXCEPTION;
  class FAILURE;

  class COUNTS : public REFCOUNTED
  {
    public:
      int right;
      int wrong;
      int ignores;
      int exceptions;

      ceefit_init_spec COUNTS(void);
      ceefit_init_spec ~COUNTS(void);

      STRING ceefit_call_spec ToString(void);
      void ceefit_call_spec Tally(const COUNTS& source);

    private:
      ceefit_init_spec COUNTS(const COUNTS&);
      COUNTS& ceefit_call_spec operator=(const COUNTS&);
  };

  class FIXTURE : public REFCOUNTED
  {
    public:
      class RUNTIME : public REFCOUNTED
      {
        public:
          INT64 start;
          INT64 elapsed;

          ceefit_init_spec RUNTIME(void);
          virtual ceefit_init_spec ~RUNTIME(void);

          virtual STRING ceefit_call_spec ToString(void);

        protected:
          STRING ceefit_call_spec d(INT64 scale);

        private:
          ceefit_init_spec RUNTIME(const RUNTIME&);    /**< Not implemented. do not call */
          RUNTIME& ceefit_call_spec operator=(const RUNTIME&);    /**< Not implemented. do not call */
      };

      class SUMMARY : public REFCOUNTED
      {
        public:
          STRING InputFile;             /**< The input filename */
          time_t InputUpdate;           /**< The time (in msecs) that the input file was last modified */
          STRING OutputFile;            /**< The output filename */
          struct _timeb RunDate;        /**< The time (in msecs) that the Fixture was run */
          PTR<RUNTIME> RunElapsedTime;  /**< Tracks elapsed times */

          ceefit_init_spec SUMMARY(void);
          virtual ceefit_init_spec ~SUMMARY(void);
          SUMMARY& ceefit_call_spec operator=(SUMMARY& aSummary);
          ceefit_init_spec SUMMARY(SUMMARY& aSummary);
      };

    public:
      PTR<SUMMARY> SummaryObj;
      PTR<COUNTS> CountsObj;

    protected:
      friend void ceefit_call_spec ::CEEFIT::LinkManualTest(::CEEFIT::FIXTURE* aFixture, FITTESTBASE* fittestManual);
      friend void ceefit_call_spec ::CEEFIT::LinkManualField(::CEEFIT::FIXTURE* aFixture, CELLADAPTER* fitfieldManual);

    public:
      SLINKLIST<CELLADAPTER> FieldList;
      SLINKLIST<CELLADAPTER> TestList;

    private:
      DYNARRAY< PTR<CELLADAPTER> > DestroyAtFinish;   /**< Manual, dynamically allocated test and field registrations are linked here */

    public:
      ceefit_init_spec FIXTURE(void);
      virtual ceefit_init_spec ~FIXTURE(void);

    public:
      // Traversal ////////////////////////////////////

      virtual void ceefit_call_spec DoTables(PTR<PARSE>& tables);
  	  static void ceefit_call_spec LoadFixture(PTR<FIXTURE>& out, const STRING& fixtureName);
      virtual void ceefit_call_spec DoTable(PTR<PARSE>& table);
      virtual void ceefit_call_spec DoRows(PTR<PARSE>& rows);
      virtual void ceefit_call_spec DoRow(PTR<PARSE>& row);
      virtual void ceefit_call_spec DoCells(PTR<PARSE>& cells);
      virtual void ceefit_call_spec DoCell(PTR<PARSE>& cell, int columnNumber);

    protected:      
      // Witness the Creeping of Scope for Fit 1.1 /////

      DYNARRAY<STRING> Args;

      /* Added by Rick Mugridge to allow a dispatch into DoFixture, Modified for CeeFIT by Dave Woldrich 05/01/05 */
      void ceefit_call_spec InterpretTables(PTR<PARSE>& tables);

      /* Added from FitNesse, Modified for CeeFIT by Dave Woldrich 05/01/05 */
    	void ceefit_call_spec GetLinkedFixtureWithArgs(PTR<FIXTURE>& out, PTR<PARSE>& tables);

	    /* Added by Rick Mugridge, from FitNesse */
	    void ceefit_call_spec GetArgsForTable(PTR<PARSE>& table);

    private:
      /* Added by Rick Mugridge to allow a dispatch into DoFixture, Modified for CeeFIT by Dave Woldrich 05/01/05 */
      void ceefit_call_spec InterpretFollowingTables(PTR<PARSE>& tables);

    public:
	    /* Added by Rick, from FitNesse, Modified for CeeFIT by Dave Woldrich 05/01/05 */
      DYNARRAY<STRING>& ceefit_call_spec GetArgs(void);

      /* Added by Rick Mugridge?  Adapted for CeeFIT by Dave Woldrich 05/01/05 */
      VALUE<PARSE> ceefit_call_spec FixtureName(PTR<PARSE>& tables);

      // Annotation ////////////////////////////////////
      
      static const char* green;
      static const char* red;
      static const char* gray;
      static const char* yellow;

      virtual void ceefit_call_spec Right(PTR<PARSE>& cell);
      virtual void ceefit_call_spec Wrong(PTR<PARSE>& cell);
      virtual void ceefit_call_spec Wrong(PTR<PARSE>& cell, const STRING& actual);
      virtual void ceefit_call_spec Info(PTR<PARSE>& cell, const STRING& message);
	    virtual STRING ceefit_call_spec Info(const STRING& message);
      virtual void ceefit_call_spec Ignore(PTR<PARSE>& cell);
    	virtual void ceefit_call_spec Error(PTR<PARSE>& cell, const STRING& message);
      virtual void ceefit_call_spec Exception(PTR<PARSE>& cell, EXCEPTION* exception);
      virtual void ceefit_call_spec Failure(PTR<PARSE>& cell, FAILURE* exception);

      // Utility //////////////////////////////////

      virtual STRING ceefit_call_spec Counts(void);
      virtual STRING ceefit_call_spec Label(const STRING& string);
      static STRING ceefit_call_spec Escape(const STRING& string);
      static STRING ceefit_call_spec Camel(const STRING& name);
      virtual void ceefit_call_spec Parse(PTR<CELLADAPTER>& aField, const STRING& s);
      virtual void ceefit_call_spec Check(PTR<PARSE>& cell, PTR<CELLADAPTER>& a, PTR<FIXTURE>& whichFixture);
      virtual inline void ceefit_call_spec Check(PTR<PARSE>& cell, PTR<CELLADAPTER>& a) 
      {
        PTR<FIXTURE> nullFixture(NULL);
        this->Check(cell, a, nullFixture);
      }

    protected:
      static void ceefit_call_spec CreateFixtureByClassName(PTR<FIXTURE>& out, const STRING& className);  /**< @param out set to NULL if no FIXTURE factory matched className */
      static void ceefit_call_spec CreateFixtureByAlias(PTR<FIXTURE>& out, const STRING& aAlias);         /**< @param out set to NULL if no FIXTURE factory's alias matched aAlias */

    protected:
      // helpers for DoTables required by CeeFIT
      virtual void ceefit_call_spec DeleteFixture(EXCEPTION*& exceptionThrown);

    protected:
      FIXTURE& ceefit_call_spec operator=(const FIXTURE&);
      ceefit_init_spec FIXTURE(const FIXTURE&);

    // ----------------------------------------------------------------------------------------------------
    // additional CeeFIT code used to take the place of the Summary HashMap that exists in the Java version
    // ----------------------------------------------------------------------------------------------------
    public:
      /**
       * Simple key/value pair for reporting summary information about the FIXTURE
       */
      class SUMMARYITEM
      {
        public:
          STRING Title;
          STRING Value;

          inline ceefit_init_spec SUMMARYITEM(void) {}
          inline ceefit_init_spec ~SUMMARYITEM(void) {}
          inline SUMMARYITEM& ceefit_call_spec operator=(const SUMMARYITEM& aItem)
          {
            Title = aItem.Title;
            Value = aItem.Value;

            return(*this);
          }

        private:
          ceefit_init_spec SUMMARYITEM(const SUMMARYITEM&);
      };

      /**
       * Fills an array of SUMMARYITEM key/value pairs containing summary information about this FIXTURE
       *
       * @see SUMMARYITEM
       */
      void ceefit_call_spec GetSummaryReport(DYNARRAY<SUMMARYITEM>& reportList);
  };

  INT64 CurrentTimeMillis(void);
};

#endif // __CEEFIT_FIXTURE_H__
