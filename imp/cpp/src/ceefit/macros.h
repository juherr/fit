#ifndef __CEEFIT_MACROS_H__
#define __CEEFIT_MACROS_H__

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

# define begin_fit_fixture(fixtureName, extendsType, alias)                                                                       \
    class fixtureName : public extendsType                                                                                        \
    {                                                                                                                             \
      private:                                                                                                                    \
        class FIXTUREBEGINBODY                                                                                                    \
        {                                                                                                                         \
          public:                                                                                                                 \
            inline FIXTUREBEGINBODY(void)                                                                                         \
            {                                                                                                                     \
              ::CEEFIT::RUNNER::IncInFixtureConstructor();                                                                        \
            }                                                                                                                     \
        };                                                                                                                        \
        FIXTUREBEGINBODY fixtureName##_BeginBody;                                                                                 \
                                                                                                                                  \
      public:                                                                                                                     \
        inline fixtureName(void) {}                                                                                               \
        inline virtual ~fixtureName(void) {}                                                                                      \
                                                                                                                                  \
      private:                                                                                                                    \
        fixtureName& operator=(const fixtureName&);                                                                               \
        fixtureName(const fixtureName&);                                                                                          \
                                                                                                                                  \
      public:                                                                                                                     \
        class PROVIDEFIXTUREOBJECTCASTER                                                                                          \
        {                                                                                                                         \
          public:                                                                                                                 \
            static inline fixtureName* GetFixtureSubclass(::CEEFIT::FIXTURE* aFixture)                                            \
            {                                                                                                                     \
              extendsType* extendsFixture = dynamic_cast< extendsType* >(aFixture);                                               \
              if(extendsFixture == NULL)                                                                                          \
              {                                                                                                                   \
                throw new ::CEEFIT::EXCEPTION(::CEEFIT::STRING("GetFixtureSubclass failed to cast FIXTURE to ") + #extendsType +  \
                                              " because the types are not compatible.");                                          \
              }                                                                                                                   \
                                                                                                                                  \
              fixtureName* subclassFixture = static_cast<fixtureName*>(extendsFixture);                                           \
              if(subclassFixture == NULL)                                                                                         \
              {                                                                                                                   \
                throw new ::CEEFIT::EXCEPTION(::CEEFIT::STRING("GetFixtureSubclass failed to cast ") + #extendsType + " to " +    \
                                    #fixtureName + " because the types are not compatible.");                                     \
              }                                                                                                                   \
                                                                                                                                  \
              return(subclassFixture);                                                                                            \
            }                                                                                                                     \
        };                                                                                                                        \
                                                                                                                                  \
      public:                                                                                                                     \
        class PROVIDEFIXTUREALIAS                                                                                                 \
        {                                                                                                                         \
          public:                                                                                                                 \
            static inline const char* GetFixtureAlias(void)                                                                       \
            {                                                                                                                     \
              return(#alias);                                                                                                     \
            }                                                                                                                     \
        };                                                                                                                        \
                                                                                                                                  \
      public:                                                                                                                     
                                                                                                                                  
                                                                                                                                  
# define end_fit_fixture(fixtureName)                                                                                             \
      private:                                                                                                                    \
        class FIXTUREENDBODY                                                                                                      \
        {                                                                                                                         \
          public:                                                                                                                 \
            inline FIXTUREENDBODY(void)                                                                                           \
            {                                                                                                                     \
              ::CEEFIT::RUNNER::DecInFixtureConstructor();                                                                        \
            }                                                                                                                     \
        };                                                                                                                        \
        FIXTUREENDBODY fixtureName##_EndBody;                                                                                     \
    };                                                                                                                            \
    static ::CEEFIT::REGISTERFIXTURECLASS< fixtureName > fixtureName##_CeeFITFixtureRegistration(#fixtureName, fixtureName::PROVIDEFIXTUREALIAS::GetFixtureAlias());


# ifdef __GNUC__
#   define fit_type(...) __VA_ARGS__
# endif


# define fit_var(varType, varName)                                                                                                \
      varType varName;                                                                                                            \
                                                                                                                                  \
    private:                                                                                                                      \
      ::FITFIELD< varType > varName##_FitField;                                                                                   \
                                                                                                                                  \
    private:                                                                                                                      \
      class PROVIDEFIELDINFO_##varName                                                                                            \
      {                                                                                                                           \
        public:                                                                                                                   \
          static inline const char* ceefit_call_spec GetFieldName(void)                                                           \
          {                                                                                                                       \
            return #varName;                                                                                                      \
          }                                                                                                                       \
                                                                                                                                  \
          static inline varType& ceefit_call_spec GetField(::CEEFIT::FIXTURE* aFixture)                                           \
          {                                                                                                                       \
            return(PROVIDEFIXTUREOBJECTCASTER::GetFixtureSubclass(aFixture)->varName);                                            \
          }                                                                                                                       \
      };                                                                                                                          \
      friend class ::CEEFIT::SETFIELDINFO<PROVIDEFIELDINFO_##varName>;                                                            \
                                                                                                                                  \
    public:                                                                                                                       \
      ::CEEFIT::SETFIELDINFO<PROVIDEFIELDINFO_##varName> varName##_SetFitFieldName                                                
                                                                                                                                  
                                                                                                                                  
# define fit_test(testName, returnType)                                                                                           \
    friend returnType ceefit_call_spec testName(bool& __fit_test_succeeds__,                                                      \
                                                const char*& __fit_file_name__,                                                   \
                                                int& __fit_line_number__);                                                        \
                                                                                                                                  \
    private:                                                                                                                      \
      friend class PROVIDETESTCALLER_##testName;                                                                                  \
      class PROVIDETESTCALLER_##testName                                                                                          \
      {                                                                                                                           \
        public:                                                                                                                   \
          static inline returnType CallFitTest(::CEEFIT::FIXTURE* fixture,                                                        \
                                          bool& __fit_test_succeeds__,                                                            \
                                          const char*& __fit_file_name__,                                                         \
                                          int& __fit_line_number__)                                                               \
          {                                                                                                                       \
            return(PROVIDEFIXTUREOBJECTCASTER::GetFixtureSubclass(fixture)->testName(__fit_test_succeeds__,                       \
                                                                                     __fit_file_name__,                           \
                                                                                    __fit_line_number__));                        \
          }                                                                                                                       \
      };                                                                                                                          \
      friend class ::FITTEST< returnType, PROVIDETESTCALLER_##testName >;                                                         \
                                                                                                                                  \
      ::FITTEST< returnType, PROVIDETESTCALLER_##testName > testName##_CeeFITTestRegistration;                                    \
                                                                                                                                  \
      class PROVIDETESTNAME_##testName                                                                                            \
      {                                                                                                                           \
        public:                                                                                                                   \
          static inline const char* ceefit_call_spec GetTestName(void)                                                            \
          {                                                                                                                       \
            return #testName;                                                                                                     \
          }                                                                                                                       \
      };                                                                                                                          \
      friend class ::CEEFIT::SETTESTNAME<PROVIDETESTNAME_##testName>;                                                             \
                                                                                                                                  \
      ::CEEFIT::SETTESTNAME<PROVIDETESTNAME_##testName> testName##_SetCeeFITTestName;                                             \
                                                                                                                                  \
    public:                                                                                                                       \
      inline returnType ceefit_call_spec testName(bool& __fit_test_succeeds__,                                                    \
                                                  const char*& __fit_file_name__,                                                 \
                                                  int& __fit_line_number__)                                                       
                                                                                                                                  
                                                                                                                                  
# ifndef _DEBUG                                                                                                                   
#   define fit_assert(booleanExpr)                                                                                                \
      if(!(__fit_test_succeeds__ = (booleanExpr)))                                                                                \
      {                                                                                                                           \
        return L"\0";                                                                                                             \
      }                                                                                                                           \
      else                                                                                                                        \
        __fit_test_succeeds__ = __fit_test_succeeds__                                                                             
# else                                                                                                                            
#   define fit_assert(booleanExpr)                                                                                                \
      if(!(__fit_test_succeeds__ = (booleanExpr)))                                                                                \
      {                                                                                                                           \
        __fit_file_name__ = __FILE__;                                                                                             \
        __fit_line_number__ = __LINE__;                                                                                           \
        return L"\0";                                                                                                             \
      }                                                                                                                           \
      else                                                                                                                        \
        __fit_test_succeeds__ = __fit_test_succeeds__                                                                             
#endif                                                                                                                            
                                                                                                                                  
                                                                                                                                  
# ifndef _DEBUG                                                                                                                   
#   define fit_fail()                                                                                                             \
      __fit_test_succeeds__ = false;                                                                                              \
      return L"\0"                                                                                                                
# else                                                                                                                            
#   define fit_fail()                                                                                                             \
      __fit_test_succeeds__ = false;                                                                                              \
      __fit_file_name__ = __FILE__;                                                                                               \
      __fit_line_number__ = __LINE__;                                                                                             \
      return L"\0"                                                                                                                
# endif                                                                                                                           
                                                                                                                                  
# define declare_fit_module(moduleName)                                                                                           \
    int __FIT_MODULE_##moduleName                                                                                                 
                                                                                                                                  
# define force_link_fit_module(moduleName)                                                                                        \
    extern int __FIT_MODULE_##moduleName;                                                                                         \
    int __FORCE_LINK_FIT_MODULE_##moduleName = __FIT_MODULE_##moduleName                                                          

#endif // __CEEFIT_MACROS_H__
