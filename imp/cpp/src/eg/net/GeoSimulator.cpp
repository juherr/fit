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
  ceefit_init_spec SIMULATOR::SIMULATOR()
  {
    NodeCount = 0;

    RegisterCeefitTest(this, "newCity", &SIMULATOR::NewCity);
    RegisterCeefitTest(this, "ok", &SIMULATOR::Ok);
    RegisterCeefitTest(this, "cancel", &SIMULATOR::Cancel);
    RegisterCeefitTest(this, "name", &SIMULATOR::Name);
    RegisterCeefitTest(this, "zip", &SIMULATOR::Zip);
    RegisterCeefitTest(this, "population", &SIMULATOR::Population);
    RegisterCeefitTest(this, "coord", &SIMULATOR::Coord);
    RegisterCeefitTest(this, "nodes", &SIMULATOR::Nodes);
  }

  ceefit_init_spec SIMULATOR::~SIMULATOR()
  {
  }

  void ceefit_call_spec SIMULATOR::NewCity() 
  {
  }

  void ceefit_call_spec SIMULATOR::Ok() 
  {
    NodeCount++;
  }

  void ceefit_call_spec SIMULATOR::Cancel() 
  {
  }

  void ceefit_call_spec SIMULATOR::Name(STRING n) 
  {
  }

  void ceefit_call_spec SIMULATOR::SetZip(int* z) 
  {
    ZipCodes.Reset();
    ZipCodes.Add(z[0]);
    ZipCodes.Add(z[1]);
    ZipCodes.Add(z[2]);
    ZipCodes.Add(z[3]);
    ZipCodes.Add(z[4]);
    ZipCodes.Add(z[5]);
  }

  DYNARRAY<int> ceefit_call_spec SIMULATOR::Zip() 
  {
    return ZipCodes;
  }

  void ceefit_call_spec SIMULATOR::Population(float p) 
  {
  }

  void ceefit_call_spec SIMULATOR::SetCoord(PTR<GEOCOORDINATE>& c) 
  {
    Coordinate = c;
  }

  GEOCOORDINATE ceefit_call_spec SIMULATOR::Coord() 
  {
    return *Coordinate;
  }

  int ceefit_call_spec SIMULATOR::Nodes() 
  {
    return NodeCount;
  }

  void ceefit_call_spec SIMULATOR::Parse(PTR<GEOCOORDINATE>& out, const STRING& string) 
  {
    out = GEOCOORDINATE::Parse(string);
  }
};
