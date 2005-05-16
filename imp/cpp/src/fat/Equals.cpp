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
#include "fat/testtypes.h"

// This macro declares this module to the system, allowing it to be linked into the EXE forcibly even though it's not being
// directly used by anything.  (see ceefit.cpp for details ...)

declare_fit_module(FatEquals);

using namespace CEEFIT;

namespace CEEFAT
{
  class FAT_EQUALS : public PRIMITIVEFIXTURE
  {
    public:
      PTR<PARSE> Heads;
      STRING Current;
      PTR<CELLADAPTER> Type;
      PTR<CELLADAPTER> X;
      PTR<CELLADAPTER> Y;
      
      ceefit_init_spec FAT_EQUALS()
      {
      }

      virtual ~FAT_EQUALS()
      {
      }

      virtual void ceefit_call_spec DoRows(PTR<PARSE>& rows)
      {
        Heads = rows->Parts;

        PRIMITIVEFIXTURE::DoRows(rows->More);
      }

      virtual VALUE<CELLADAPTER> ceefit_call_spec GetType(const STRING& name)
      {
        CELLADAPTER* type = NULL;

        if(name.IsEqual("boolean"))
        {
          type = new FITFIELD<bool>();
        }
        else if(name.IsEqual("integer"))
        {
          type = new FITFIELD<signed int>();
        }
        else if(name.IsEqual("real"))
        {
          type = new FITFIELD<float>();
        }
        else if(name.IsEqual("string"))
        {
          type = new FITFIELD<STRING>();
        }
            //name.equals("date") ?       Date.class :
            //name.equals("money") ?      Money.class :
        else if(name.IsEqual("scientific"))
        {
          type = new FITFIELD< SCIENTIFICDOUBLE >();
        }
        else if(name.IsEqual("integers"))
        {
          type = new FITFIELD< DYNARRAY<int> >();
        }
        else if(name.IsEqual("booleans"))
        {
          type = new FITFIELD< DYNARRAY<bool> >();
        }
        else if(name.IsEqual("strings"))
        {
          type = new FITFIELD< DYNARRAY<STRING> >();
        }
        if (type == NULL)
        {
          throw new EXCEPTION(STRING("Unimplemented choice ") + name);
        }
        
        return(VALUE<CELLADAPTER>(type));
      }

      virtual void ceefit_call_spec DoCell(PTR<PARSE>& cell, int col)
      {
        try {
          PTR<PARSE> aColHead(Heads->At(col));
          STRING cellText(cell->Text());
          wchar_t head = aColHead->Text().CharAt(0);

          switch(head)
          {
              case L't':
              {
                if(Type != NULL)
                {
                  Type = NULL;
                }
                Type = GetType(cellText);
                break;
              }
              case L'x':
              {
                if(X != NULL)
                {
                  X = NULL;
                }
                if(Type == NULL)
                {
                  throw new EXCEPTION("No type available to parse");
                }
                Type->NewInstanceParse(X, cellText);
                break;
              }
              case L'y':
              {
                if(Y != NULL)
                {
                  Y = NULL;
                }
                if(Type == NULL)
                {
                  throw new EXCEPTION("No type available to parse");
                }
                Type->NewInstanceParse(Y, cellText);
                break;
              }
              case L'=':
              {
                STRING aTemp;
                STRING bTemp;

                if(X == NULL || Y == NULL)
                {
                  throw new EXCEPTION("x and/or y not defined");
                }

                X->ReadFromFixtureVar(aTemp);
                Y->ReadFromFixtureVar(bTemp);

                FITFIELD<bool> equalityResults;
                equalityResults = aTemp.IsEqual(bTemp);

                Check(cell, equalityResults);
                break;
              }
              case L'?':
              {
                cell->AddToBody(Gray(STRING("x: ") + Print(X) + " y: " + Print(Y)));
                break;
              }

              default:    throw new EXCEPTION(STRING("don't do ") + head);
          }
        }
        catch(EXCEPTION* e)
        {
          Exception(cell, e);
        }
      }

      virtual void ceefit_call_spec Parse(PTR<CELLADAPTER>& aField, const STRING& s)
      {
        PRIMITIVEFIXTURE::Parse(aField, s);
      }

      /*
      Boolean parseCustomBoolean(String s) {
          if (true) throw new RuntimeException("boolean");
          return
              s.startsWith("y") ?         Boolean.TRUE :
              s.startsWith("n") ?         Boolean.FALSE :
              s.startsWith("t") ?         Boolean.TRUE :
              s.startsWith("f") ?         Boolean.FALSE :
              null ;
      }
      */

      virtual STRING ceefit_call_spec Print(PTR<CELLADAPTER>& value)
      {
        STRING aTemp;
        value->ReadFromFixtureVar(aTemp);

        return aTemp;
      }
  };

  static ::CEEFIT::REGISTERFIXTURECLASS< FAT_EQUALS > FatEqualsFixtureRegistration("FAT_EQUALS", "fat.Equals");
};
