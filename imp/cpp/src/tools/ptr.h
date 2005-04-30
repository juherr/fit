#ifndef __TOOLS_PTR_H__
#define __TOOLS_PTR_H__

#include "ceefit/mandatory.h"

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
   * <p>Managed object</p>
   *
   * <p>Managed pointers can only work with objects that provide managed references to themselves.  The logic for
   * exactly how a managed pointer deals with objects is defined by the managed reference class.  Extend classes that
   * extend MANAGED (like REFCOUNTED) in your objects to use them with PTR.</p>
   * @see REFCOUNTED
   * @see PTR
   */
  class MANAGED : public virtual OBJECT
  {
    public:
      inline MANAGED(void) {}
      inline MANAGED(MANAGED&) {}
      inline MANAGED& operator=(MANAGED&) { return(*this); }
      virtual inline ~MANAGED(void) {}

    public:
      virtual void AddRef(void)=0;
      virtual bool ReleaseRef(bool DoDestroy=true)=0;    
  };


  /**
   * <p>Managed pointers (also called smart pointers).</p>
   */
  template<class T> class PTR : public virtual OBJECT
  {
    public:
      /**
       * Default constructor, initializes the PTR to NULL
       */
	    inline PTR<T>(void) 
      {
		    SetPointer(NULL);
	    }	

      /**
       * Constructor, initializes the PTR with a pointer to some object
       */
	    explicit inline PTR<T>(T* aSPtr) 
      {
		    SetPointer(NULL);
		    operator=(aSPtr);
	    }

#    ifdef __GNUC__
        /**
         * <p>Constructor, initializes the PTR with a pointer to some object</p>
         *
         * <p>GCC seemed to need this less optimal version of the function in order to compile some things properly</p>
         */
        explicit inline PTR<T>(VALUE<T> aValue) 
        {
		      SetPointer(NULL);
          this->operator=(aValue);
        }

        /**
         * <p>Constructor, initializes the PTR with a pointer to some object.</p>
         *
         * <p>GCC seemed to need this less optimal version of the function in order to compile some things properly</p>
         */
        template<class U> explicit inline PTR<T>(VALUE<U> aValue) 
        {
		      SetPointer(NULL);
          this->operator=(aValue);
        }

        /**
         * Copy constructor, initializes the PTR with a pointer to some object
         */
	      explicit inline PTR<T>(PTR<T>& aSmartPtr) 
        {
		      SetPointer(NULL);
		      operator=(aSmartPtr);
	      }

        /**
         * <p>operator= assigns a new pointer value to this PTR and adds a reference to the object pointed to.</p>
         */
	      inline PTR<T>& operator=(VALUE<T> aValue) 
        {
          return(aValue.SetPtr(*this));
	      }

	      inline PTR<T>& operator=(const PTR<T>& aSmartPtr) 
        {
		      return(AssignRef(aSmartPtr.ActualPointer));
	      }

        /**
         * operator= assigns a new pointer value to this PTR and adds a reference to the object pointed to
         */
	      template<class U> inline PTR<T>& operator=(PTR<U>& aSmartPtr) 
        {    
		      return(AssignRef(aSmartPtr.GetPointer()));
	      }

#    else
        /**
         * Constructor, initializes the PTR with a pointer to some object
         */
        template<class U> explicit inline PTR<T>(VALUE<U>& aValue) 
        {
		      SetPointer(NULL);
          this->operator=(aValue);
        }
#    endif

      /**
       * Copy constructor, initializes the PTR with a pointer to some object
       */
	    template<class U> explicit inline PTR(PTR<U>& aSmartPtr) 
      {
		    SetPointer(NULL);
		    operator=(aSmartPtr);
	    }

      /**
       * Destructor, releases the reference to the pointer if any is held
       */
	    inline ~PTR<T>(void)
      {
		    if(ActualPointer) 
        {
          MANAGED* managedPointer = ActualPointer;
      
          managedPointer->ReleaseRef();
          SetPointer(NULL);
		    }
	    }

      /**
       * operator= assigns a new pointer value to this PTR and adds a reference to the object pointed to
       */
	    template<class U> inline PTR<T>& operator=(U* aPointer) 
      {
        if(aPointer != NULL)
        {
          T* castedPointer = dynamic_cast<T*>(aPointer);

	  	    return(AssignRef(castedPointer));
        }
        else
        {
          return(AssignRef(NULL));
        }
	    }

      /**
       * <p>operator= assigns a new pointer value to this PTR and adds a reference to the object pointed to.</p>
       */
	    template<class U> inline PTR<T>& operator=(VALUE<U> aValue) 
      {
        return(aValue.SetPtr(*this));
	    }

      /**
       * operator= assigns a new pointer value to this PTR and adds a reference to the object pointed to
       */
	    inline PTR& operator=(PTR& aSmartPtr) 
      {
		    return(AssignRef(aSmartPtr.ActualPointer));
	    }

      /**
       * operator! returns true iff the pointer value is NULL
       */
	    inline bool operator!(void) const 
      {
		    return(!ActualPointer);
	    }

      /**
       * operator== returns true if the rvalue in the expression is equivalent to the pointer value stored in this PTR
       */
	    inline bool operator==(const PTR<T>& aPtr) const 
      {
		    return(ActualPointer == aPtr.ActualPointer);
	    }

      /**
       * IsEqual returns true if the rvalue in the expression is equivalent to the pointer value stored in this PTR
       */
	    inline bool IsEqual(const PTR<T>& aPtr) const 
      {
		    return(ActualPointer == aPtr.ActualPointer);
	    }

      /**
       * operator!= returns true if the rvalue in the expression is not equivalent to the pointer value stored in this PTR
       */
	    inline bool operator!=(const PTR<T>& aPtr) const 
      {
		    return(ActualPointer != aPtr.ActualPointer);
	    }

      /**
       * NotEqual returns true if the rvalue in the expression is not equivalent to the pointer value stored in this PTR
       */
	    inline bool IsNotEqual(const PTR<T>& aPtr) const 
      {
		    return(ActualPointer != aPtr.ActualPointer);
	    }

      /**
       * operator== returns true if the rvalue in the expression is equivalent to the pointer value stored in this PTR
       */
	    inline bool operator==(const T* aPtr) const 
      {
		    return(ActualPointer == aPtr);
	    }

      /**
       * IsEqual returns true if the rvalue in the expression is equivalent to the pointer value stored in this PTR
       */
	    inline bool IsEqual(const T* aPtr) const 
      {
		    return(ActualPointer == aPtr);
	    }

      /**
       * operator!= returns true if the rvalue in the expression is not equivalent to the pointer value stored in this PTR
       */
	    inline bool operator!=(const T* aPtr) const 
      {
		    return(ActualPointer != aPtr);
	    }

      /**
       * NotEqual returns true if the rvalue in the expression is not equivalent to the pointer value stored in this PTR
       */
	    inline bool IsNotEqual(const T* aPtr) const 
      {
		    return(ActualPointer != aPtr);
	    }

      /**
       * operator-> returns the pointer that is stored in this PTR
       */
	    inline T* operator->(void) 
      {
        if(ActualPointer == NULL)
        {
          throw new EXCEPTION("Null pointer detected in operator->");
        }
		    return(ActualPointer); 
	    }

      /**
       * operator-> returns the pointer that is stored in this PTR
       */
	    inline const T* operator->(void) const 
      {
        if(ActualPointer == NULL)
        {
          throw new EXCEPTION("Null pointer detected in operator->");
        }
		    return(ActualPointer); 
	    }

      /**
       * <p>Get the MANAGED pointer as a VALUE</p>
       */
      inline VALUE<T> GetValue(void)
      {
        VALUE<T> aValue(ActualPointer);

        return(aValue);
      }

      /**
       * Get the value of the MANAGED pointer
       */
	    inline T* GetPointer(void) const
      {
		    return ActualPointer;
	    }
	    
    protected:
      /**
       * The pointer to the MANAGED object
       */
 	    mutable T* ActualPointer;

      /**
       * Force the managed pointer to be set to an object (or NULL) WITHOUT adding a ref to that object.
       */
	    inline void SetPointer(T* aPointer) 
      { 
		    ActualPointer = aPointer; 
	    }

      /**
       * <p>Do not call, this is not allowed</p>
       */
	    void SetPointer(T* aPointer) const;

      /**
       * Assign the reference using the T's AddRef and ReleaseRef methods.
       */
      inline PTR<T>& AssignRef(T* aPointer, bool DoDestroy=true) 
      {
	      T* currentPtrRef = this->ActualPointer;
        if(currentPtrRef == aPointer) 
        {
		      return(*this);
	      }
	      if(currentPtrRef) 
        {      
          static_cast<MANAGED*>(currentPtrRef)->ReleaseRef(DoDestroy);
          this->SetPointer(NULL);
	      }
	      if(aPointer) 
        {
          this->SetPointer(aPointer);
          static_cast<MANAGED*>(aPointer)->AddRef();
	      }

	      return(*this);
      }

  };

};

#endif // __TOOLS_PTR_H__
