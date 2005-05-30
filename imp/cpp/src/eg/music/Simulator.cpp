// Copyright (c) 2002 Cunningham & Cunningham, Inc.
// Released under the terms of the GNU General Public License version 2 or later.

package eg.music;

import fit.*;
import java.util.Date;

class SIMULATOR : public REFCOUNTED 
{
  // This discrete event simulator supports three events
  // each of which is open coded in the body of the simulator.

  protected:
    static PTR<SIMULATOR> System = new SIMULATOR();
    static INT64 Time = CurrentTimeMillis();

  public:
    static INT64 NextSearchComplete = 0;
    static INT64 NextPlayStarted = 0;
    static INT64 NextPlayComplete = 0;

  protected:
    virtual INT64 Sooner(INT64 soon, INT64 event) 
    {
      return((event > Time && event < soon) ? event : soon);
    }

    virtual INT64 NextEvent(const INT64& bound) 
    {
      INT64 result = bound;
      result = Sooner(result, NextSearchComplete);
      result = Sooner(result, NextPlayStarted);
      result = Sooner(result, NextPlayComplete);
      return result;
    }

    virtual void Perform(void) 
    {
      if(Time == NextSearchComplete)     
      {
        MusicLibrary.SearchComplete();
      }
      if(Time == NextPlayStarted)        
      {
        MusicPlayer.PlayStarted();
      }
      if(Time == NextPlayComplete)       
      {
        MusicPlayer.PlayComplete();
      }
    }

    virtual void advance(const INT64& future) 
    {
      while(time < future) 
      {
        time = NextEvent(future);
        perform();
      }
    }

    static INT64 schedule(const double& seconds)
    {
      return(Time + (INT64) (1000.0 * seconds));
    }

    virtual void Delay(const double& seconds) 
    {
      Advance(Schedule(seconds));
    }

  public:
    void WaitSearchComplete(void) 
    {
      Advance(NextSearchComplete);
    }

    void WaitPlayStarted(void) 
    {
      Advance(NextPlayStarted);
    }

    void WaitPlayComplete(void) 
    {
      Advance(NextPlayComplete);
    }

    void FailLoadJam(void) 
    {
      ACTIONFIXTURE::Actor = new Dialog("load jamed", ActionFixture.actor);
    }
}
