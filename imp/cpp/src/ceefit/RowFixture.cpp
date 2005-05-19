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

#include "ceefit.h"

namespace CEEFIT
{
  void ROWFIXTURE::DoRows(PTR<PARSE>& rows)
  {
    try 
    {
      Bind(rows->Parts);
      Results.Reset();
      Query(Results);      
      
      DYNARRAY< PTR<PARSE> > rowList;
      DYNARRAY< PTR<CELLADAPTER> > resultsList;
      
      List(rowList, rows->More);
      List(resultsList, Results);
      Match(rowList, resultsList, 0);
      
      PTR<PARSE> last(rows->Last());
      BuildRows(last->More, Surplus);

      Mark(last->More, STRING("surplus"));
      Mark(Missing, "missing");
    } 
    catch(EXCEPTION* e) 
    {
      PTR<PARSE> temp(rows->Leaf());

      Exception(temp, e);
    }
  }

  void ROWFIXTURE::Match(DYNARRAY< PTR<PARSE> >& expected, DYNARRAY< PTR<CELLADAPTER> >& computed, int col)
  {
    if (col >= ColumnBindings.GetSize()) 
    {
      Check(expected, computed);
    } 
    else if (ColumnBindings[col] == NULL) 
    {
      Match(expected, computed, col+1);
    } 
    else 
    {
      HASHMAP< DYNARRAY< PTR<PARSE> > > eMap;
      HASHMAP< DYNARRAY< PTR<CELLADAPTER> > > cMap;
      DYNARRAY< PTR< PARSE_KEYNODE > > eSet;
      DYNARRAY< PTR< CELL_KEYNODE > > cSet;
      DYNARRAY< PTR< HASHMAPNODEBASE > > keys;

      eSort(eMap, expected, col);
      cSort(cMap, computed, col);
      
      eMap.UniqueKeySet(eSet);
      cMap.UniqueKeySet(cSet);
      Union(keys, eSet, cSet);
      
      int i = -1;
      while(++i < keys.GetSize()) 
      {
        PTR<HASHMAPNODEBASE> key(keys.Get(i));
        DYNARRAY< PTR<PARSE> >* eList = eMap.Get(*key);
        DYNARRAY< PTR<CELLADAPTER> >* cList = cMap.Get(*key);
        if (eList == NULL) 
        {
          Surplus.AddAll(*cList);
        } 
        else if (cList == NULL) 
        {
          Missing.AddAll(*eList);
        } 
        else if (eList->GetSize() == 1 && cList->GetSize() == 1) 
        {
          Check(*eList, *cList);
        } 
        else 
        {
          Match(*eList, *cList, col+1);
        }
      }
    }
  }

  void ROWFIXTURE::List(DYNARRAY< PTR<PARSE> >& out, PTR<PARSE>& rows)
  {
    out.Reset();

    PTR<PARSE> temp(rows);
    while (temp != NULL) {
        out.Add(temp);
        temp = temp->More;
    }
  }

  void ROWFIXTURE::List(DYNARRAY< PTR<CELLADAPTER> >& out, DYNARRAY< PTR<CELLADAPTER> >& rows)
  {
    out.Reset();

    for (int i=0; i<rows.GetSize(); i++) {
      out.Add(rows[i]);
    }
  }

  void ROWFIXTURE::eSort(HASHMAP< DYNARRAY< PTR<PARSE> > >& out, DYNARRAY< PTR<PARSE> >& list, int col)
  {
    PTR<CELLADAPTER> a(ColumnBindings[col]);

    int i = -1;
    while(++i < list.GetSize()) 
    {
      PTR<PARSE> row(list.Get(i));
      PTR<PARSE> cell(row->Parts->At(col));

      try 
      {
        PTR<CELLADAPTER> key;
        
        a->NewInstanceParse(key, cell->Text());
        Bin(out, key, row);
      } 
      catch(EXCEPTION* e) 
      {
        Exception(cell, e);
        PTR<PARSE> rest(cell->More);
        while(rest != NULL)
        {
          Ignore(rest);
          rest = rest->More;
        }
      }
    }
  }

  void ROWFIXTURE::cSort(HASHMAP< DYNARRAY< PTR<CELLADAPTER> > >& out, DYNARRAY< PTR<CELLADAPTER> >& list, int col)
  {
    PTR<CELLADAPTER> a(ColumnBindings[col]);
    int i = -1;
    while(++i < list.GetSize())
    {
      PTR<CELLADAPTER>& row = list.Get(i);
      try 
      {
        PTR<CELLADAPTER> key;
        STRING cellContents;
        
        if(a->IsField())
        {          
          // key is value in the row's bound column, parsed into some type by the ColumnBinding
          // value is the row itself coming out of the list (which comes from a call to Query)
          
          STRING keyString;

          row->ReadFromFixtureVar(keyString);           // clone the value in a so we have a copy
          a->NewInstanceParse(key, keyString);          
        }
        else // if(a->IsMethod())
        {
          // key is result of method invocation, where row is required to be of type FIXTURECONTAINER
          // value is the row coming out of the list (which comes from a call to Query)

          a->Invoke(key, row.GetPointer());
        }

        Bin(out, key, row);
      } 
      catch(EXCEPTION* e) 
      {
        // surplus anything with bad keys, including null
        Surplus.Add(row);

        delete e; // what, no logging??
      }
    }
  }

  void ROWFIXTURE::Bin(HASHMAP< DYNARRAY< PTR<PARSE> > >& map, PTR<CELLADAPTER>& key, PTR<PARSE>& row)
  {
    DYNARRAY< PTR<PARSE> >* listInMap = map.Get(key);
    if(listInMap != NULL) 
    {
      listInMap->Add(row);
    } 
    else 
    {
      DYNARRAY< PTR<PARSE> > list;
      list.Add(row);
      map.Put(key, list);
    }

  }

  void ROWFIXTURE::Bin(HASHMAP< DYNARRAY< PTR<CELLADAPTER> > >& map, PTR<CELLADAPTER>& key, PTR<CELLADAPTER>& row)
  {
    DYNARRAY< PTR<CELLADAPTER> >* listInMap = map.Get(key);
    if(listInMap != NULL) 
    {
      listInMap->Add(row);
    } 
    else 
    {
      DYNARRAY< PTR<CELLADAPTER> > list;
      list.Add(row);
      map.Put(key, list);
    }

  }

  void ROWFIXTURE::Union(DYNARRAY< PTR< HASHMAPNODEBASE > >& out, 
                         DYNARRAY< PTR< PARSE_KEYNODE > >& a, 
                         DYNARRAY< PTR< CELL_KEYNODE > >& b)
  {
    HASHMAP<int> result;
    int i = -1;
    while(++i < a.GetSize())
    {
      result.Put(a[i], i);
    }
    i = -1;
    while(++i < b.GetSize())
    {
      result.Put(b[i], i);
    }

    out.Reset();
    result.UniqueKeySet(out);
  }

  void ROWFIXTURE::Check(DYNARRAY< PTR<PARSE> >& eList, DYNARRAY< PTR<CELLADAPTER> >& cList)
  {
    if (eList.GetSize() == 0) 
    {
      Surplus.AddAll(cList);
      return;
    }
    if(cList.GetSize() == 0) 
    {
      Missing.AddAll(eList);
      return;
    }
    
    PTR<PARSE> row(eList.Get(0));
    eList.Remove(0);

    PTR<PARSE> cell(row->Parts);
    
    PTR<CELLADAPTER> obj(cList.Get(0));
    cList.Remove(0);

    for (int i=0; i < ColumnBindings.GetSize() && cell != NULL; i++) 
    {
      FIXTURE* target = this;
      PTR<CELLADAPTER> a(ColumnBindings[i]);
      if (a != NULL) 
      {
        ::FITFIXTURECONTAINER* fixtureContainer = dynamic_cast< ::FITFIXTURECONTAINER* >(obj.GetPointer());
        AssertNotNull(fixtureContainer);

        target = fixtureContainer->GetFixture();
      }
      
      this->COLUMNFIXTURE::Check(cell, a, target);
      cell = cell->More;
    }
    
    Check(eList, cList); // recurse until one or both lists are exhausted
  }

  void ROWFIXTURE::Mark(PTR<PARSE>& rows, const STRING& message)
  {
  }

  void ROWFIXTURE::Mark(DYNARRAY< PTR<PARSE> >& rows, const STRING& message)
  {
  }

  void ROWFIXTURE::BuildRows(PTR<PARSE>& out, DYNARRAY< PTR<CELLADAPTER> >& rows)
  {
    PTR<PARSE> emptyParse;    // null

    PTR<PARSE> root(new PARSE(STRING(), STRING(), emptyParse, emptyParse));
    PTR<PARSE> next(root);
    for (int i=0; i < rows.GetSize(); i++) 
    {
      PTR<PARSE> builtCells;      
      BuildCells(builtCells, rows[i]);

      next = next->More = new PARSE(STRING("tr"), STRING(), builtCells, emptyParse);
    }

    out = root->More;
  }

  void ROWFIXTURE::BuildCells(PTR<PARSE>& out, PTR<CELLADAPTER>& row)
  {
    PTR<PARSE> emptyParse;    // null

    if (row == NULL) 
    {
      PTR<PARSE> nil(new PARSE(STRING("td"), STRING("null"), emptyParse, emptyParse));
      nil->AddToTag(" colspan=" + ColumnBindings.GetSize());

      out = nil;
      return;
    }
    PTR<PARSE> root(new PARSE(STRING(), STRING(), emptyParse, emptyParse));
    PTR<PARSE> next(root);
    for (int i=0; i < ColumnBindings.GetSize(); i++) 
    {
      next = next->More = new PARSE(STRING("td"), STRING("&nbsp;"), emptyParse, emptyParse);
      PTR<CELLADAPTER> a(ColumnBindings[i]);
      if(a == NULL) 
      {
        Ignore(next);
      } 
      else 
      {
        try 
        {
          STRING rowVal;
          row->ReadFromFixtureVar(rowVal);
          
          PTR<CELLADAPTER> temp;         
          a->NewInstanceParse(temp, rowVal);

          STRING columnVal;
          temp->ReadFromFixtureVar(columnVal);

          Info(next, columnVal);
        } 
        catch(EXCEPTION* e) 
        {
          Exception(next, e);
        }
      }
    }
    out = root->More;
  }
};
