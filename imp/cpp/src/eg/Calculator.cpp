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

extern "C"
{
  // defined in ceefit.lib
  typedef unsigned int unicode_char_t;
  extern int unicode_isdigit(unicode_char_t c);
}

declare_fit_module(CalculatorFixture);

using namespace CEEFIT;

namespace EG
{
  begin_namespaced_fit_fixture(EG, CALCULATOR, COLUMNFIXTURE, eg.Calculator)
    public:
      class HP35 
      {
        public:
          double R[4];
          double S;

          HP35(void) 
          {      
            R[0] = R[1] = R[2] = R[3] = 0.0;
            S = 0.0;
          }

          virtual ~HP35(void) 
          {
          }

        private:
          static double ToRadians(double degreesAngle) 
          {
            double PI	= 3.14159265359;

            return(degreesAngle * (PI / 180.0));
          }
                
        public:
          virtual void Key(const STRING& aKey) 
          {
            if (Numeric(aKey))              
            {
              float temp;
              if(swscanf(aKey.GetBuffer(), L"%g", &temp) == 0) { throw new PARSEEXCEPTION("Failed to parse double"); }
              Push((double) temp);
            }
            else if (aKey.IsEqual("enter"))   {Push();}
            else if (aKey.IsEqual("+"))       {Push(Pop()+Pop());}
            else if (aKey.IsEqual("-"))       {double t=Pop(); Push(Pop()-t);}
            else if (aKey.IsEqual("*"))       {Push(Pop()*Pop());}
            else if (aKey.IsEqual("/"))       {double t=Pop(); Push(Pop()/t);}
            else if (aKey.IsEqual("x^y"))     {Push(exp(log(Pop())*Pop()));}
            else if (aKey.IsEqual("clx"))     {R[0]=0.0;}
            else if (aKey.IsEqual("clr"))     {R[0]=R[1]=R[2]=R[3]=0.0;}
            else if (aKey.IsEqual("chs"))     {R[0]=-R[0];}
            else if (aKey.IsEqual("x<>y"))    {double t=R[0]; R[0]=R[1]; R[1]=t;}
            else if (aKey.IsEqual("r!"))      {R[3]=Pop();}
            else if (aKey.IsEqual("sto"))     {S=R[0];}
            else if (aKey.IsEqual("rcl"))     {Push(S);}
            else if (aKey.IsEqual("sqrt"))    {Push(sqrt(Pop()));}
            else if (aKey.IsEqual("ln"))      {Push(log(Pop()));}
            else if (aKey.IsEqual("sin"))     {Push(sin(ToRadians(Pop())));}
            else if (aKey.IsEqual("cos"))     {Push(cos(ToRadians(Pop())));}
            else if (aKey.IsEqual("tan"))     {Push(tan(ToRadians(Pop())));}
            else {throw new EXCEPTION(STRING("can't do key: ") + aKey);
            }
          }

          virtual bool Numeric(const STRING& key) 
          {
            return(key.Length() >= 1 &&
                    (unicode_isdigit(key.CharAt(0)) ||
                        (key.Length() >= 2 &&
                            (key.CharAt(0) == '-' &&
                                unicode_isdigit(key.CharAt(1))))));
          }

          virtual void Push() 
          {
            for (int i=3; i>0; i--) 
            {
              R[i] = R[i-1];
            }
          }

          virtual void Push(double value) 
          {
            Push();
            R[0] = value;
          }

          virtual double Pop() 
          {
            double result = R[0];
            for (int i=0; i<3; i++) 
            {
              R[i] = R[i+1];
            }
            return result;
          }
        
        private:
          HP35(const HP35&);
          HP35& operator=(const HP35&);
      };

      static HP35 Hp;

    public:
      fit_var(float, volts);
      fit_var(STRING, key);

      fit_test(points, bool)
      {
        return false;
      }

      fit_test(flash, bool)
      {
        return false;
      }

      fit_test(watts, float)
      {
        return 0.5f;
      }

      fit_test(x, SCIENTIFICDOUBLE) 
      {
        return SCIENTIFICDOUBLE(Hp.R[0]);
      }

      fit_test(y, SCIENTIFICDOUBLE) 
      {
        return SCIENTIFICDOUBLE(Hp.R[1]);
      }

      fit_test(z, SCIENTIFICDOUBLE) 
      {
        return SCIENTIFICDOUBLE(Hp.R[2]);
      }

      fit_test(t, SCIENTIFICDOUBLE) 
      {
        return SCIENTIFICDOUBLE(Hp.R[3]);
      }

    public:
      // 
      // Override the Reset and Execute functions in the COLUMNFIXTURE baseclass
      // 

      virtual void ceefit_call_spec Reset() 
      {
        key = "";
      }

      virtual void ceefit_call_spec Execute()
      {
        if(key.Length() != 0) 
        {
          Hp.Key(this->key);
        }
      }

  end_namespaced_fit_fixture(EG, CALCULATOR)

  // instantiate the static
  EG::CALCULATOR::HP35 EG::CALCULATOR::Hp;
};