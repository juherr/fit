#ifndef __CEEFIT_REGISTERSTATIC_H__
#define __CEEFIT_REGISTERSTATIC_H__

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
  template<class T> class REGISTERFIXTURECLASS : public FIXTUREFACTORY
  {
    protected:
      friend class RUNNER;

    public:
      inline ceefit_init_spec REGISTERFIXTURECLASS<T>(const char* fixtureName, const char* aliasName=NULL) : FIXTUREFACTORY(fixtureName, aliasName)
      {
        // Do not add a fixture that appears in more than once ...
        if(RUNNER::FindFixtureFactoryByName(fixtureName) == NULL)
        {
          RUNNER::RegisterFixtureFactory(this);
          RUNNER::GetSetCurrentFixtureFactory(this);
        }
      }

      inline ceefit_init_spec ~REGISTERFIXTURECLASS<T>(void)
      {
      }

      virtual void ceefit_call_spec CreateFixture(PTR<FIXTURE>& out)
      {
        out = new T();
      }

    private:
      /**
       * Not implemented.  Do not call.
       */
      REGISTERFIXTURECLASS<T>(void);

      /**
       * Not implemented.  Do not call.
       */
      REGISTERFIXTURECLASS<T>& operator=(const REGISTERFIXTURECLASS<T>&);

      /**
       * Not implemented.  Do not call.
       */
      REGISTERFIXTURECLASS<T>(const REGISTERFIXTURECLASS<T>&);
  };

  template<class T> class REGISTERNONFIXTURECLASS : public NONFIXTUREFACTORY
  {
    protected:
      friend class RUNNER;

    public:
      inline ceefit_init_spec REGISTERNONFIXTURECLASS<T>(const char* nonFixtureName, const char* aliasName=NULL) : NONFIXTUREFACTORY(nonFixtureName, aliasName)
      {
        RUNNER::RegisterNonFixtureFactory(this);
      }

      inline ceefit_init_spec ~REGISTERNONFIXTURECLASS<T>(void)
      {
      }

      virtual OBJECT* ceefit_call_spec CreateNonFixture(void)
      {
        return(new T());
      }

    private:
      /**
       * Not implemented.  Do not call.
       */
      REGISTERNONFIXTURECLASS<T>(void);

      /**
       * Not implemented.  Do not call.
       */
      REGISTERNONFIXTURECLASS<T>& operator=(const REGISTERNONFIXTURECLASS<T>&);

      /**
       * Not implemented.  Do not call.
       */
      REGISTERNONFIXTURECLASS<T>(const REGISTERNONFIXTURECLASS<T>&);
  };

};

namespace CEEFIT 
{
  class FIXTURE;
};

/**
 * <p>Special one-off CELLADAPTER that simply contains and exposes a FIXTURE object</p>
 *
 * <p>Currently only used by RowFixture.</p>
 */
class FITFIXTURECONTAINER : public CEEFIT::CELLADAPTER
{
  private:
    CEEFIT::PTR< CEEFIT::FIXTURE > Fixture;

  public:
    virtual void ceefit_call_spec WriteToFixtureVar(const CEEFIT::STRING& in)
    {
      throw new CEEFIT::EXCEPTION("Cannot WriteToFixtureVar on FITFIXTURECONTAINER CELLADAPTER");
    }

    virtual void ceefit_call_spec ReadFromFixtureVar(CEEFIT::STRING& out)
    {
      throw new CEEFIT::EXCEPTION("Cannot ReadFromFixtureVar on FITFIXTURECONTAINER CELLADAPTER");
    }

    virtual void ceefit_call_spec SetName(const CEEFIT::STRING& aName)
    {
      throw new CEEFIT::EXCEPTION("Cannot SetName on FITFIXTURECONTAINER CELLADAPTER");
    }

    virtual const CEEFIT::STRING& ceefit_call_spec GetName(void) const
    {
      throw new CEEFIT::EXCEPTION("Cannot GetName on FITFIXTURECONTAINER CELLADAPTER");
    }

    virtual const CEEFIT::STRING& ceefit_call_spec GetType(void) const
    {
      throw new CEEFIT::EXCEPTION("Cannot GetType on FITFIXTURECONTAINER CELLADAPTER");
    }

    virtual bool ceefit_call_spec IsMethod(void) const
    {
      throw new CEEFIT::EXCEPTION("Cannot IsMethod on FITFIXTURECONTAINER CELLADAPTER");
    }

    virtual bool ceefit_call_spec IsField(void) const
    {
      throw new CEEFIT::EXCEPTION("Cannot IsField on FITFIXTURECONTAINER CELLADAPTER");
    }

    virtual void ceefit_call_spec NewInstanceParse(::CEEFIT::PTR<CEEFIT::CELLADAPTER>& out, const CEEFIT::STRING& aText)
    {
      throw new CEEFIT::EXCEPTION("Cannot NewInstanceParse on FITFIXTURECONTAINER CELLADAPTER");
    }

    virtual void ceefit_call_spec Invoke(::CEEFIT::PTR<CEEFIT::CELLADAPTER>& out, ::CEEFIT::PTR<CEEFIT::FIXTURE>& aFixture)
    {
      throw new CEEFIT::EXCEPTION("Cannot Invoke on FITFIXTURECONTAINER CELLADAPTER");
    }
    
    inline ceefit_init_spec FITFIXTURECONTAINER(::CEEFIT::PTR<CEEFIT::FIXTURE>& aFixture) 
    {
      Fixture = aFixture;
    }

    virtual inline ceefit_init_spec ~FITFIXTURECONTAINER(void) {}

    virtual int ceefit_call_spec GetHashCode(void)
    {
      return(0);
    }

    virtual bool ceefit_call_spec IsEqual(const CEEFIT::CELLADAPTER* aAdapter) const
    {
      return(false);
    }

    virtual ::CEEFIT::VALUE<CEEFIT::FIXTURE> GetFixture(void)
    {
      return(::CEEFIT::VALUE< ::CEEFIT::FIXTURE >(Fixture));
    }

  protected:
    inline ceefit_init_spec FITFIXTURECONTAINER(void); /**< not implemented, do not call.*/
    inline ceefit_init_spec FITFIXTURECONTAINER(FITFIXTURECONTAINER&);     /**< not implemented, do not call. */
    inline FITFIXTURECONTAINER& ceefit_call_spec operator=(const FITFIXTURECONTAINER&);   /**< not implemented, do not call. */
};

#endif // __CEEFIT_REGISTERSTATIC_H__
