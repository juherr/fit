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

      STRING ceefit_call_spec ToString(void) const;
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
          fitINT64 start;
          mutable fitINT64 elapsed;

          ceefit_init_spec RUNTIME(void);
          virtual ceefit_init_spec ~RUNTIME(void);

          virtual STRING ceefit_call_spec ToString(void) const;

        protected:
          STRING ceefit_call_spec d(fitINT64 scale) const;

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
          PTR<COUNTS> CountsRun;        /**< Counts run, from eg.ExampleTests.java */

          ceefit_init_spec SUMMARY(void);
          virtual ceefit_init_spec ~SUMMARY(void);
          SUMMARY& ceefit_call_spec operator=(SUMMARY& aSummary);
          ceefit_init_spec SUMMARY(SUMMARY& aSummary);
      };

    public:
      PTR<SUMMARY> SummaryObj;
      PTR<COUNTS> CountsObj;

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
        PTR<FIXTURE> nullFixture(null);
        this->Check(cell, a, nullFixture);
      }

    protected:
      static void ceefit_call_spec CreateFixtureByClassName(PTR<FIXTURE>& out, const STRING& className);  /**< @param out set to null if no FIXTURE factory matched className */
      static void ceefit_call_spec CreateFixtureByAlias(PTR<FIXTURE>& out, const STRING& aAlias);         /**< @param out set to null if no FIXTURE factory's alias matched aAlias */

    protected:
      // helpers for DoTables required by CeeFIT
      virtual void ceefit_call_spec DeleteFixture(EXCEPTION*& exceptionThrown);

    protected:
      FIXTURE& ceefit_call_spec operator=(const FIXTURE&);
      ceefit_init_spec FIXTURE(const FIXTURE&);

      // -------------------------------------------------------
      // CeeFIT code used to manage static member data lifetimes
      // -------------------------------------------------------
    public:
      /**
       * <p>If a FIXTURE subclass holds any static information, implement this method and release the statics there</p>
       *
       * <p>This method is called in the ::CEEFIT::Run() function after all tables have been processed</p>
       */
      virtual void ceefit_call_spec ReleaseStatics(void);

      /**
       * <p>Returns a pointer to a FIXTURE class that will be the target of FindMethod or FindField</p>
       *
       * <p>The default implementation returned a pointer to 'this'</p>
       */
      virtual VALUE<FIXTURE> ceefit_call_spec GetTargetClass(void);

      /**
       * <p>Figure out which CELLADAPTER is bound to the provided method, this is implicitly a zero arg match</p>
       */
      virtual VALUE<CELLADAPTER> ceefit_call_spec FindMethod(PTR<FIXTURE>& targetFixture, const STRING& name);

      /**
       * <p>Figure out which CELLADAPTER is bound to the provided method, number of args must match</p>
       *
       * @param errorOnMultipleMatches if true throw exception if multiple methods are found of the same name and same arg count
       */
      virtual VALUE<CELLADAPTER> ceefit_call_spec FindMethod(PTR<FIXTURE>& targetFixture, const STRING& name, int numArgs, bool errorOnMultipleMatches=false);

      /**
       * <p>Figure out which CELLADAPTER is bound to the provided name</p>
       *
       * <p>CeeFIT implementation note:  this was pulled up from COLUMNFIXTURE in order to provide generic reflection
       * facilities to all subclasses</p>
       */
      virtual VALUE<CELLADAPTER> ceefit_call_spec FindField(PTR<FIXTURE>& targetFixture, const STRING& name);

    protected:
      friend void ceefit_call_spec ::CEEFIT::LinkManualTest(::CEEFIT::FIXTURE* aFixture, FITTESTBASE* fittestManual);
      friend void ceefit_call_spec ::CEEFIT::LinkManualField(::CEEFIT::FIXTURE* aFixture, CELLADAPTER* fitfieldManual);

      DYNARRAY< PTR<CELLADAPTER> > DestroyAtFinish;   /**< Manual, dynamically allocated test and field registrations are linked here */
      SLINKLIST<CELLADAPTER> FieldList;               /**< Fields */
      SLINKLIST<CELLADAPTER> TestList;                /**< Methods */

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

  // need to define NewInstanceParse that could not have been until now
  template<class T> inline void ceefit_call_spec CEEFIT::FITFIELDBASE<T>::NewInstanceParse(CEEFIT::FIXTURE* callParseOn, CEEFIT::PTR< CEEFIT::CELLADAPTER >& out, const CEEFIT::STRING& aText)
  {
    CEEFIT::PTR<CEEFIT::CELLADAPTER> returnValue(new FITFIELD<T>());

    callParseOn->Parse(returnValue, aText);

    out = returnValue;
  }

  // Utility for java-like currentTimeMillis()
  fitINT64 ceefit_call_spec CurrentTimeMillis(void);
};

#endif // __CEEFIT_FIXTURE_H__
