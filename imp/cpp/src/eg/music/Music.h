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

#ifndef __EG_MUSIC_MUSIC_H__
#define __EG_MUSIC_MUSIC_H__

namespace EG_MUSIC
{
  class MUSIC : public virtual CEEFIT::FIXTURE
  {
    public:
      static const char* Status;

      CEEFIT::STRING Title;
      CEEFIT::STRING Artist;
      CEEFIT::STRING Album;
      CEEFIT::STRING Genre;
      long Size;
      int Seconds;
      int TrackNumber;
      int TrackCount;
      int Year;
      EG::DATE Date;
      bool Selected;

      ceefit_init_spec MUSIC(void);
      virtual ceefit_dtor_spec ~MUSIC(void);
      MUSIC& ceefit_call_spec operator=(const MUSIC& aMusic);

      // Accessors ////////////////////////////////

      virtual CEEFIT::STRING ceefit_call_spec Track(void);
      virtual double ceefit_call_spec Time(void);
      virtual int ceefit_call_spec GetHashCode(void) const;
      virtual bool ceefit_call_spec IsEqual(const MUSIC& aMusic) const;
      virtual CEEFIT::STRING ceefit_call_spec ToString(void) const;
      virtual CEEFIT::STRING ceefit_call_spec Dump(void) const;   /**< Special for CeeFIT, dumps the contents of MUSIC in the format found in the data file */

      // Factory //////////////////////////////////

      static CEEFIT::VALUE<MUSIC> ceefit_call_spec Parse(const CEEFIT::STRING& string);

    private:
      ceefit_init_spec MUSIC(const MUSIC&);                  /**< Not implemented, do not call. */
  };
};

template<> class FITFIELD< EG_MUSIC::MUSIC > : public CEEFIT::FITFIELDBASE< EG_MUSIC::MUSIC >
{
  public:
    inline void ceefit_call_spec ToString(CEEFIT::STRING& out, const EG_MUSIC::MUSIC& in) const
    {
      out = in.Dump();
    }

    inline bool ceefit_call_spec Parse(EG_MUSIC::MUSIC& out, const CEEFIT::STRING& in)
    {
      try
      {
        CEEFIT::PTR<EG_MUSIC::MUSIC> aMusic(EG_MUSIC::MUSIC::Parse(in));
        out = *aMusic;
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
      return("EG_MUSIC::MUSIC");
    }

    template<class U> inline FITFIELD< EG_MUSIC::MUSIC >& operator=(U& rValue)
    {
      this->FITFIELDBASE< EG_MUSIC::MUSIC >::operator=(rValue);

      return(*this);
    }

    inline ceefit_init_spec FITFIELD< EG_MUSIC::MUSIC >(void)
    {
    }

    virtual inline ceefit_dtor_spec ~FITFIELD< EG_MUSIC::MUSIC >(void)
    {
    }

  private:
    FITFIELD< EG_MUSIC::MUSIC >(const FITFIELD< EG_MUSIC::MUSIC >&);              /**< not implemented, do not call. */
    FITFIELD< EG_MUSIC::MUSIC >& operator=(const FITFIELD< EG_MUSIC::MUSIC >&);   /**< not implemented, do not call. */
};

#endif // __EG_MUSIC_MUSIC_H__
