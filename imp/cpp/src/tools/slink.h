#ifndef __TOOLS_SLINK_H__
#define __TOOLS_SLINK_H__

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
  /**
   * <p>Single link for linked list classes.</p>
   */
  template<class T> class SLINK : public virtual OBJECT
  {
    private:
      T* Next;

    public:
      ceefit_init_spec SLINK<T>(void)
      {
        Next = null;
      }

      ceefit_init_spec SLINK<T>(SLINK<T>& aSlink)
      {
        Next = aSlink.Next;
      }

      ceefit_init_spec SLINK<T>(T* aNext)
      {
        Next = aNext;
      }

      SLINK<T>& ceefit_call_spec operator=(SLINK<T>& aSlink)
      {
        Next = aSlink.Next;
        return(*this);
      }

      inline virtual ceefit_dtor_spec ~SLINK<T>(void)
      {
      }

      inline virtual T* ceefit_call_spec GetNext(void)
      {
        return(Next);
      }

      inline virtual const T* ceefit_call_spec GetNext(void) const
      {
        return(Next);
      }

      inline virtual void ceefit_call_spec SetNext(T* aNext)
      {
        Next = aNext;
      }
  };

  template<class ANYTYPE> class SLINKLIST : public OBJECT
  {
    protected:
      ANYTYPE* Head;

    private:
      inline void ceefit_call_spec ThrowBoundsException(void) const
      {
        if(GetHead() != null)
        {
          throw new BOUNDSEXCEPTION("SLINKLIST out of bounds.");
        }
      }

    public:
	    inline ceefit_init_spec SLINKLIST(void)
	    {
		    Head = null;
	    }

      /**
       * Special constructor used in static object environments where object instantiation is not known.
       * In this case, two assumptions are made: 1) As the memory for the object is static, the Head will
       * always be null or initialized to some valid object's pointer.  2) There are no other compiled in
       * fields in this simple template class that will make SLINKLIST operate properly before the constructor
       * has been called.
       */
      inline ceefit_init_spec SLINKLIST(bool clearHead)
      {
        if(clearHead)
        {
          Head = null;
        }
      }

	    virtual inline ceefit_dtor_spec ~SLINKLIST(void)
	    {
	    }

      virtual inline ANYTYPE* ceefit_call_spec GetHead(void)
      {
        return(Head);
      }

      virtual inline const ANYTYPE* ceefit_call_spec GetHead(void) const
      {
        return(Head);
      }

      virtual inline ANYTYPE* ceefit_call_spec GetTail(void)
      {
        ANYTYPE* aNode = Head;
        while(aNode)
        {
          ANYTYPE* nextNode = (dynamic_cast< SLINK<ANYTYPE>* >(aNode))->GetNext();
          if(nextNode == null)
          {
            break;
          }
          aNode = nextNode;
        }
        return(aNode);
      }

      virtual inline const ANYTYPE* ceefit_call_spec GetTail(void) const
      {
        ANYTYPE* aNode = Head;
        while(aNode)
        {
          ANYTYPE* nextNode = (dynamic_cast< SLINK<ANYTYPE>* >(aNode))->GetNext();
          if(nextNode == null)
          {
            break;
          }
          aNode = nextNode;
        }
        return(aNode);
      }

      /**
       * <p>Set the head of the linked list.</p>
       *
       * <p>If the head of the list pointer is already set, it will be replaced with aVal.</p>
       *
       * <p>Call Destroy() prior to calling SetHead if you want the list contents to be destroyed
       * as part of setting a new head pointer</p>
       */
      virtual inline void ceefit_call_spec SetHead(ANYTYPE* aVal)
      {
        Head = aVal;
      }

      /**
       * <p>Get the size of the SLINKLIST</p>
       *
       * <p>This implementation of GetSize is inherently slow and should be avoided in performance-sensitive blocks of
       * code.  It would be nice if SLINKLIST could keep a count of items in the list, but because list links can be
       * managed outside of the list implementation that count cannot be guaranteed to be accurate.  This is why
       * this GetSize is implemented by brute force counting of values in the SLINKLIST.
       */
      virtual inline int ceefit_call_spec GetSize(void) const
	    {
		    int i;
		    const ANYTYPE* aVal = GetHead();

		    i = 0;
		    while(aVal)
		    {
			    i++;
			    aVal = (dynamic_cast< const SLINK<ANYTYPE>* >(aVal))->GetNext();
		    }

		    return(i);
	    }

      template<class ANYTYPE2> inline void ceefit_call_spec Add(ANYTYPE2& aItem)
      {
		    ANYTYPE* aVal = GetHead();
        if(aVal == null)
        {
          SetHead(dynamic_cast<ANYTYPE2*>(&aItem));
        }
        else
        {
          while((dynamic_cast< SLINK<ANYTYPE>* >(aVal))->GetNext() != null)
          {
            aVal = (dynamic_cast< SLINK<ANYTYPE>* >(aVal))->GetNext();
          }
          (dynamic_cast< SLINK<ANYTYPE>* >(aVal))->SetNext(dynamic_cast<ANYTYPE2*>(&aItem));
        }
      }

      /**
       * <p>Important note:  If Insert's aIndex equals the size of the LIST, aArray will be added to the end of the LIST.
       * In this special case the Next pointer of the last value in aArray will NOT be modified so that the behavior
       * matches that of Add.</p>
       */
      template<class ANYTYPE2> inline void ceefit_call_spec Insert(int aIndex, ANYTYPE2& aValue)
      {
        if(aIndex < 0)
        {
          ThrowBoundsException();
        }

        if(aIndex == 0)
        {
          (dynamic_cast< SLINK<ANYTYPE2>* >(&aValue))->SetNext(GetHead());
          SetHead(dynamic_cast<ANYTYPE2*>(&aValue));
        }
        else
        {
          ANYTYPE* previous = null;
          ANYTYPE* current = GetHead();

          int i = aIndex;
          while(i > 0 && (dynamic_cast< SLINK<ANYTYPE>* >(current))->GetNext() != null)
          {
            previous = current;
            current = (dynamic_cast< SLINK<ANYTYPE>* >(current))->GetNext();
            i--;
          }

          if(i != 0)
          {
            ThrowBoundsException();
          }

          (dynamic_cast< SLINK<ANYTYPE>* >(previous))->SetNext(dynamic_cast<ANYTYPE2*>(&aValue));
          if(current != null)  // This if statement maintains the semantics of an Add operation if aIndex was the end of list
          {
            (dynamic_cast< SLINK<ANYTYPE2>* >(&aValue))->SetNext(current);
          }
        }
      }

      /**
       * <p>Replace a value at the specified index</p>
       *
       * <p>A Set operation for a SLINKLIST consists of a value at a specified index in the LIST being replaced by the
       * passed value.  All existing links in the LIST that are affected by replacement will be updated correctly.</p>
       *
       * <p>Important note:  The Set implementation for SLINKLIST is NOT destructive of any overwritten values,
       * meaning, if a value exists at a particular index, it is not destroyed (with delete) prior to being replaced
       * with aValue.</p>
       *
       * <p>Important note:  If Set's aIndex equals the size of the LIST, aValue will be added to the end of the LIST.
       * In this special case the Next pointer of aValue will NOT be modified so that the behavior matches that of Add.</p>
       */
      template<class ANYTYPE2> inline void ceefit_call_spec Set(int aIndex, ANYTYPE2& aValue)
      {
        ANYTYPE* previous = null;
        ANYTYPE* current = GetHead();

        if(aIndex < 0)
        {
          ThrowBoundsException();
        }
        else if(aIndex == 0)
        {
          if(current != null)
          {
            ANYTYPE* oldNext = (dynamic_cast< SLINK<ANYTYPE>* >(current))->GetNext();
            (dynamic_cast< SLINK<ANYTYPE2>* >(&aValue))->SetNext(oldNext);
          }
          SetHead(&aValue);
          return;
        }
        else
        {
          int i = aIndex;
          while(i > 0 && (dynamic_cast< SLINK<ANYTYPE>* >(current))->GetNext() != null)
          {
            previous = current;
            current = (dynamic_cast< SLINK<ANYTYPE>* >(current))->GetNext();
            i--;
          }

          if(i != 0)
          {
            ThrowBoundsException();
          }

          (dynamic_cast< SLINK<ANYTYPE>* >(previous))->SetNext(&aValue);
          if(current != null)
          {
            (dynamic_cast< SLINK<ANYTYPE2>* >(&aValue))->SetNext((dynamic_cast< SLINK<ANYTYPE>* >(current))->GetNext());
          }
        }
      }

      virtual inline ANYTYPE& ceefit_call_spec Get(int aIndex)
      {
        ANYTYPE* current = GetHead();
        int remaining = aIndex;
        while(remaining > 0 && current != null)
        {
          current = (dynamic_cast< SLINK<ANYTYPE>* >(current))->GetNext();
          remaining--;
        }

        if(current == null)
        {
          ThrowBoundsException();
        }

        return(*current);
      }

      virtual inline const ANYTYPE& ceefit_call_spec Get(int aIndex) const
      {
        const ANYTYPE* current = GetHead();
        int remaining = aIndex;
        while(remaining > 0 && current != null)
        {
          current = (dynamic_cast< const SLINK<ANYTYPE>* >(current))->GetNext();
          remaining--;
        }

        if(current == null)
        {
          ThrowBoundsException();
        }

        return(*current);
      }

      virtual inline void ceefit_call_spec Remove(int aIndex, int aCountToRemove=1)
      {
        ANYTYPE* previous = null;
        ANYTYPE* current = GetHead();
        int i = aIndex;
        while(i > 0 && current != null)
        {
          previous = current;
          current = (dynamic_cast< SLINK<ANYTYPE>* >(current))->GetNext();
          i--;
        }

        if(current == null)
        {
          ThrowBoundsException();
        }

        while(aCountToRemove > 0)
        {
          if(current == null)
          {
            ThrowBoundsException();
          }

          ANYTYPE* newNext = (dynamic_cast< SLINK<ANYTYPE>* >(current))->GetNext();

          if(previous != null)
          {
            (dynamic_cast< SLINK<ANYTYPE>* >(previous))->SetNext(newNext);
            previous = current;
          }
          else
          {
            SetHead(newNext);
          }
          current = newNext;

          aCountToRemove--;
        }
      }

      virtual inline void ceefit_call_spec RemoveAll(void)
      {
        while(GetHead() != null)
        {
          Remove(0);
        }
      }

      virtual inline void ceefit_call_spec Reset(void)
      {
        SetHead(null);
      }

      virtual inline void ceefit_call_spec Destroy(void)
      {
        ANYTYPE* aVal = GetHead();
        SetHead(null);

        while(aVal != null)
        {
          ANYTYPE* nextVal = (dynamic_cast< SLINK<ANYTYPE>* >(aVal))->GetNext();
          delete aVal;
          aVal = nextVal;
        }
      }

	    virtual inline void ceefit_call_spec RemoveObject(ANYTYPE* aVal)
      {
        assert(aVal != null);

		    ANYTYPE* tracker;

			  if(GetHead() == aVal)
			  {
				  SetHead((dynamic_cast< SLINK<ANYTYPE>* >(aVal))->GetNext());
			  }
			  else
			  {
				  tracker = GetHead();
				  if(tracker)
				  {
					  while((dynamic_cast< SLINK<ANYTYPE>* >(tracker))->GetNext())
					  {
						  if((dynamic_cast< SLINK<ANYTYPE>* >(tracker))->GetNext() == aVal)
						  {
                ANYTYPE* nextTrackerVal = (dynamic_cast< SLINK<ANYTYPE>* >(tracker))->GetNext();

							  (dynamic_cast< SLINK<ANYTYPE>* >(tracker))->SetNext((dynamic_cast< SLINK<ANYTYPE>* >(nextTrackerVal))->GetNext());
							  break;
						  }
						  tracker = (dynamic_cast< SLINK<ANYTYPE>* >(tracker))->GetNext();
					  }
				  }
			  }
			  (dynamic_cast< SLINK<ANYTYPE>* >(aVal))->SetNext(null);
	    }

      virtual inline void ceefit_call_spec AddHead(ANYTYPE* aVal)
      {
        assert(aVal != null);

		    Insert(0, *aVal);
      }

	    virtual inline void ceefit_call_spec AddTail(ANYTYPE* aVal)
	    {
        assert(aVal != null);

		    Add(*aVal);
	    }

    private:
      SLINKLIST<ANYTYPE>& ceefit_call_spec operator=(const SLINKLIST<ANYTYPE>& aObj);    /**< Not implemented.  Do not call. */
      ceefit_init_spec SLINKLIST<ANYTYPE>(const SLINKLIST<ANYTYPE>& aObj);             /**< Not implemented.  Do not call. */
  };

};

#endif // __TOOLS_SLINK_H__
