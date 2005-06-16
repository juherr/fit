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

#include "tools/alloc.h"
#include "ceefit.h"
#include "eg/eg.h"

using namespace CEEFIT;

namespace EG_NET
{
  ceefit_init_spec GEOCOORDINATE::GEOCOORDINATE()
  {
    Lat = 0.0f;
    Lon = 0.0f;
  }

  ceefit_init_spec GEOCOORDINATE::GEOCOORDINATE(const GEOCOORDINATE& aGeoCoord)
  {
    Lat = aGeoCoord.Lat;
    Lon = aGeoCoord.Lon;
  }

  ceefit_init_spec GEOCOORDINATE::GEOCOORDINATE(float aLatitude, float aLongitude) 
  {
    Lat = aLatitude;
    Lon = aLongitude;
  }

  GEOCOORDINATE& ceefit_call_spec GEOCOORDINATE::operator=(const GEOCOORDINATE& aGeoCoord)
  {
    Lat = aGeoCoord.Lat;
    Lon = aGeoCoord.Lon;

    return(*this);
  }

  ceefit_init_spec GEOCOORDINATE::~GEOCOORDINATE() 
  {
  }

  VALUE<GEOCOORDINATE> ceefit_call_spec GEOCOORDINATE::Parse(const STRING& string) 
  {
    DYNARRAY<STRING> tokens;
    Tokenize(tokens, string, STRING("nNsSeEwW \'\","), true);
    float n[6];
    n[0] = 0.0f; n[1] = 0.0f; n[2] = 0.0f; n[3] = 0.0f; n[4] = 0.0f; n[5] = 0.0f;

    bool north=true, east=true;
    int j = 0;
    for(int i=0; i<6 && j < tokens.GetSize(); j++) 
    {
      STRING token(tokens[j].ToLowercase());
      char ch = token.CharAt(0);
      if(iswdigit(ch) || ch == '-') 
      {
        if(swscanf(token.GetBuffer(), L"%f", &n[i]) == 0) 
        {
          throw new PARSEEXCEPTION(STRING("Failed to parse float:  ") + token);
        }
        i++;
      }
      if(ch == L's')                              
      {
        north = false;
      }
      if(ch == L'w')                              
      {
        east = false;
      }
      if (i>0 && STRING("nsew").IndexOf(ch) >= 0)           
      {
        i = 3;
      }
    }

    float Lat = n[0] + n[1]/60.0f + n[2]/3600.0f;
    float Lon = n[3] + n[4]/60.0f + n[5]/3600.0f;
    return(VALUE<GEOCOORDINATE>(new GEOCOORDINATE(north ? Lat : -Lat, east ? Lon : -Lon)));
  }

  const double GEOCOORDINATE::Precision = 0.00001;

  bool ceefit_call_spec GEOCOORDINATE::IsEqual(const GEOCOORDINATE& arg) const
  {
    return(((fitINT64) (Lat/Precision)) == ((fitINT64) (arg.Lat/Precision)) &&
           ((fitINT64) (Lon/Precision)) == ((fitINT64) (arg.Lon/Precision)));
  }

  int ceefit_call_spec GEOCOORDINATE::GetHashCode() const
  {
    return ((int) (Lat/Precision)) + ((int) (Lon/Precision));
  }

  STRING ceefit_call_spec GEOCOORDINATE::FloatToGeoCoordString(float aFloat)
  {
    int dec, sign;

    STRING out(_fcvt(fabs(aFloat), 4, &dec, &sign));  // want up to 4 decimal places
    // _fcvt pads the stringified float with 0's past the decimal point.  To match Java, strip trailing 0's
    while(out.Length() > 0 && out.EndsWith("0") && !out.EndsWith(".0")) 
    {
      out = out.Substring(0, out.Length()-1);
    }

    return(out);
  }

  STRING ceefit_call_spec GEOCOORDINATE::ToString() const
  {
    return(FloatToGeoCoordString(fabs(Lat)) + (Lat>=0 ? "N " : "S ") + FloatToGeoCoordString(fabs(Lon)) + (Lon>=0 ? "E " : "W "));
  }
};