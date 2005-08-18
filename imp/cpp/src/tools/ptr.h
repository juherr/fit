#ifndef __TOOLS_PTR_H__
#define __TOOLS_PTR_H__

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

  extern void ceefit_call_spec ThrowNullArrowPointer(void);
  extern void ceefit_call_spec ThrowNullStarPointer(void);

  /**
   * <p>Managed pointers (also called smart pointers).</p>
   */
  template<class T> class PTR : public virtual OBJECT
  {
    public:
      /**
       * Default constructor, initializes the PTR to null
       */
	    inline PTR<T>(void) 
      {
		    SetPointer(null);
	    }	

      /**
       * Constructor, initializes the PTR with a pointer to some object
       */
	    explicit inline PTR<T>(const int nullPointer) 
      {
        if(nullPointer != 0)
        {
          throw new EXCEPTION("Expected a null pointer for PTR<T>(int)");
        }

        SetPointer(null);
	    }

	    explicit inline PTR<T>(const unsigned int nullPointer) 
      {
        if(nullPointer != 0)
        {
          throw new EXCEPTION("Expected a null pointer for PTR<T>(unsigned int)");
        }

        SetPointer(null);
	    }

      /**
       * <p>Constructor, initializes the PTR with a pointer to some object</p>
       *
       * <p>GCC seemed to need this less optimal version of the function in order to compile some things properly</p>
       */
      explicit inline PTR<T>(VALUE<T> aValue) 
      {
        SetPointer(null);
        aValue.SetPtr(*this);
      }

      /**
       * <p>Constructor, initializes the PTR with a pointer to some object.</p>
       *
       * <p>GCC seemed to need this less optimal version of the function in order to compile some things properly</p>
       */
      template<class U> inline PTR<T>(VALUE<U>& aValue) 
      {
		    SetPointer(null);
        aValue.SetPtr(*this);
      }

      /**
       * Copy constructor, initializes the PTR with a pointer to some object
       */
//	    explicit inline PTR<T>(PTR<T>& aSmartPtr) 
//      {
//		    SetPointer(null);
//		    operator=(aSmartPtr);
//	    }

      /**
       * Copy constructor, initializes the PTR with a pointer to some object
       */
	    template<class U> inline PTR<T>(PTR<U>& aSmartPtr) 
      {
		    SetPointer(null);
		    operator=(aSmartPtr);
	    }

      /**
       * Copy constructor, initializes the PTR with a pointer to some object
       */
	    explicit inline PTR<T>(PTR<T>& aSmartPtr) 
      {
		    SetPointer(null);
		    operator=(aSmartPtr);
	    }

      /**
       * Initializes the PTR with a pointer to some object
       */
	    template<class U> explicit inline PTR<T>(U* aPointer) 
      {
		    SetPointer(null);
		    operator=(aPointer);
	    }

      /**
       * Constructor, initializes the PTR with a value, with optional addref, not for public use
       */
	    inline PTR<T>(VALUE<T>& aSPtr, bool addRef) 
      {
        if(addRef == true)
        {
  		    SetPointer(null);
		      this->operator=(aSPtr);
        }
        else
        {
          SetPointer(aSPtr.Value);
        }
	    }

      /**
       * <p>operator= assigns a new pointer value to this PTR and adds a reference to the object pointed to.</p>
       */
	    inline PTR<T>& operator=(VALUE<T> aValue) 
      {
        return(aValue.SetPtr(*this));
	    }

      /**
       * <p>operator= assigns a new pointer value to this PTR and adds a reference to the object pointed to.</p>
       */
	    template<class U> inline PTR<T>& operator=(VALUE<U>& aValue) 
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
        if(aSmartPtr != null)
        {
          T* castedPointer = dynamic_cast<T*>(aSmartPtr.GetPointer());
          AssertIsTrue(castedPointer != null);

	  	    return(AssignRef(castedPointer));
        }
        else
        {
          return(AssignRef(null));
        }
	    }

      /**
       * operator= assigns a new pointer value to this PTR and adds a reference to the object pointed to
       */
	    template<class U> inline PTR<T>& operator=(U* aPointer) 
      {
        if(aPointer != null)
        {
          T* castedPointer = dynamic_cast<T*>(aPointer);
          AssertIsTrue(castedPointer != null);

	  	    return(AssignRef(castedPointer));
        }
        else
        {
          return(AssignRef(null));
        }
	    }

      /**
       * operator= assigns a new pointer value to this PTR and adds a reference to the object pointed to
       */
//	    inline PTR<T>& operator=(PTR<T>& aSmartPtr) 
//      {
//		    return(AssignRef(aSmartPtr.ActualPointer));
//	    }

      /**
       * <p>operator= assigns a new pointer value to this PTR and adds a reference to the object pointed to.</p>
       */
	    inline PTR<T>& operator=(const int aValue) 
      {
        if(aValue != 0)
        {
          throw new EXCEPTION("Expected a null pointer for operator=(int)");
        }
        return(AssignRef(null));
	    }

      /**
       * <p>operator= assigns a new pointer value to this PTR and adds a reference to the object pointed to.</p>
       */
	    inline PTR<T>& operator=(const unsigned int aValue) 
      {
        if(aValue != 0)
        {
          throw new EXCEPTION("Expected a null pointer for operator=(unsigned int)");
        }
        return(AssignRef(null));
	    }

      /**
       * Destructor, releases the reference to the pointer if any is held
       */
	    inline ~PTR<T>(void)
      {
		    if(ActualPointer) 
        {
#         ifdef _DEBUG
            bool pointerConsistencyCheck = (dynamic_cast<MANAGED*>(ActualPointer) != null && typeid(T) != typeid(MANAGED));
            AssertIsTrue(pointerConsistencyCheck);
#         endif

          dynamic_cast<MANAGED*>(ActualPointer)->ReleaseRef();
          SetPointer(null);
		    }
	    }

      /**
       * operator! returns true iff the pointer value is null
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
//	    inline bool IsEqual(const PTR<T>& aPtr) const 
//      {
//		    return(ActualPointer == aPtr.ActualPointer);
	    //}

      /**
       * operator!= returns true if the rvalue in the expression is not equivalent to the pointer value stored in this PTR
       */
//	    inline bool operator!=(const PTR<T>& aPtr) const 
//      {
//		    return(ActualPointer != aPtr.ActualPointer);
//	    }

      /**
       * NotEqual returns true if the rvalue in the expression is not equivalent to the pointer value stored in this PTR
       */
//	    inline bool IsNotEqual(const PTR<T>& aPtr) const 
//      {
//		    return(ActualPointer != aPtr.ActualPointer);
//	    }

    private:
      template<class U> bool ObjectIsEqual(U* aPtr)
      {
        if(ActualPointer == null)
        {
          return(aPtr == null);
        }
        else
        {
          if(aPtr == null)
          {
            return(false);
          }

          return(::CEEFIT::IsEqual(*ActualPointer, *aPtr));
        }
      }

      template<class U> bool ObjectIsEqual(const U* aPtr) const
      {
        if(ActualPointer == null)
        {
          return(aPtr == null);
        }
        else
        {
          if(aPtr == null)
          {
            return(false);
          }

          const T& thisRef = *ActualPointer;
          const U& otherRef = *aPtr;
          return(::CEEFIT::IsEqual(thisRef, otherRef));
        }
      }

    public:
      /**
       * operator== returns true if the rvalue in the expression is equivalent to the pointer value stored in this PTR
       */
	    inline bool operator==(const T* aPtr) const 
      {
		    return(ActualPointer == aPtr);
	    }

      /**
       * IsEqual returns true if the rvalue in the expression is equivalent to the pointer value stored in this PTR, in addition,
       * the actual objects are compared
       */
	    inline bool IsEqual(T* aPtr)
      {
		    return(ActualPointer == aPtr || ObjectIsEqual(aPtr));
	    }

	    template<class U> inline bool IsEqual(PTR<U>& aPtr)
      {
		    return(ActualPointer == aPtr.ActualPointer || ObjectIsEqual(aPtr.GetPointer()));
	    }

	    template<class U> inline bool IsEqual(const PTR<U>& aPtr) const
      {
		    return(ActualPointer == aPtr.ActualPointer || ObjectIsEqual(aPtr.GetPointer()));
	    }

      /**
       * operator!= returns true if the rvalue in the expression is not equivalent to the pointer value stored in this PTR
       */
	    inline bool operator!=(const T* aPtr) const 
      {
		    return(ActualPointer != aPtr);
	    }

      /**
       * NotEqual returns true if the rvalue in the expression is not equivalent to the pointer value stored in this PTR, in addition,
       * the actual objects are compared
       */
	    inline bool IsNotEqual(T* aPtr)
      {
		    return(ActualPointer != aPtr && !ObjectIsEqual(aPtr));
	    }

	    template<class U> inline bool IsNotEqual(PTR<U>& aPtr)
      {
		    return(ActualPointer != aPtr.ActualPointer && !ObjectIsEqual(aPtr.GetPointer()));
	    }

	    template<class U> inline bool IsNotEqual(const PTR<U>& aPtr) const
      {
		    return(ActualPointer != aPtr.ActualPointer && !ObjectIsEqual(aPtr.GetPointer()));
	    }

      inline int GetHashCode(void)
      {
        if(ActualPointer == null)
        {
          return(0);
        }
        else 
        {
          return(ActualPointer->GetHashCode());
        }
      }

      /**
       * operator-> returns the pointer that is stored in this PTR
       */
	    inline T* operator->(void) 
      {
        if(ActualPointer == null)
        {
          ThrowNullArrowPointer();
        }
		    return(ActualPointer); 
	    }

      /**
       * operator-> returns the pointer that is stored in this PTR
       */
	    inline const T* operator->(void) const 
      {
        if(ActualPointer == null)
        {
          ThrowNullArrowPointer();
        }
		    return(ActualPointer); 
	    }

      /**
       * <p>operator* returns a reference to the object that is stored in this PTR.</p>
       *
       * <p>It is an error to attempt to perform the operator* on a null PTR.</p>
       */
	    inline T& operator*(void) 
      {
        if(ActualPointer == null)
        {
          ThrowNullStarPointer();
        }

		    return(*ActualPointer); 
	    }

      /**
       * <p>operator* returns a reference to the object that is stored in this PTR.</p>
       *
       * <p>It is an error to attempt to perform the operator* on a null PTR.</p>
       */
	    inline const T& operator*(void) const
      {
        if(ActualPointer == null)
        {
          ThrowNullStarPointer();
        }

		    return(*ActualPointer); 
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
       * Force the managed pointer to be set to an object (or null) WITHOUT adding a ref to that object.
       */
	    inline void SetPointer(T* aPointer) 
      {
#       ifdef _DEBUG
          if(aPointer != null) 
          {
            AssertIsTrue(dynamic_cast<MANAGED*>(aPointer) != null);
            AssertIsTrue(typeid(T) != typeid(MANAGED));
          }
#       endif        

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
          this->SetPointer(null);
	      }
	      if(aPointer) 
        {
          this->SetPointer(aPointer);
          MANAGED* aManaged = dynamic_cast<MANAGED*>(aPointer);
          
          AssertNotNull(aManaged);

          aManaged->AddRef();
	      }
        if(currentPtrRef)
        {
          dynamic_cast<MANAGED*>(currentPtrRef)->ReleaseRef(DoDestroy);
        }

	      return(*this);
      }
  };

  template<class T> inline VALUE<T>::VALUE(PTR<T>& aPtr)
  {
    Value = aPtr.GetPointer();
  }

# ifndef _MSC_VER
    template<class T> template<class U> inline VALUE<T>::VALUE(PTR<U>& aPtr)
    {
      Value = dynamic_cast<T*>(aPtr.GetPointer());
    }
# endif
};

#endif // __TOOLS_PTR_H__
