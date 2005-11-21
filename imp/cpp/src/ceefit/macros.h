#ifndef __CEEFIT_MACROS_H__
#define __CEEFIT_MACROS_H__

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

# define begin_fit_fixture(fixtureType, extendsType, alias)                                                                       \
    class fixtureType : public extendsType                                                                                        \
    {                                                                                                                             \
      private:                                                                                                                    \
        class FIXTUREBEGINBODY                                                                                                    \
        {                                                                                                                         \
          public:                                                                                                                 \
            inline ceefit_init_spec FIXTUREBEGINBODY(void)                                                                        \
            {                                                                                                                     \
              ::CEEFIT::RUNNER::IncInFixtureConstructor();                                                                        \
            }                                                                                                                     \
        };                                                                                                                        \
        FIXTUREBEGINBODY fixtureType##_BeginBody;                                                                                 \
                                                                                                                                  \
      public:                                                                                                                     \
        inline ceefit_init_spec fixtureType(void) {}                                                                              \
        inline virtual ceefit_dtor_spec ~fixtureType(void) {}                                                                     \
                                                                                                                                  \
      private:                                                                                                                    \
        fixtureType& operator=(const fixtureType&);                                                                               \
        fixtureType(const fixtureType&);                                                                                          \
                                                                                                                                  \
      public:                                                                                                                     \
        class PROVIDEFIXTUREOBJECTCASTER                                                                                          \
        {                                                                                                                         \
          public:                                                                                                                 \
            static inline fixtureType* GetFixtureSubclass(::CEEFIT::FIXTURE* aFixture)                                            \
            {                                                                                                                     \
              extendsType* extendsFixture = dynamic_cast< extendsType* >(aFixture);                                               \
              if(extendsFixture == ::CEEFIT::null)                                                                                \
              {                                                                                                                   \
                throw new ::CEEFIT::EXCEPTION(::CEEFIT::STRING("GetFixtureSubclass failed to cast FIXTURE to ") + #extendsType +  \
                                              " because the types are not compatible.");                                          \
              }                                                                                                                   \
                                                                                                                                  \
              fixtureType* subclassFixture = static_cast< fixtureType* >(extendsFixture);                                         \
              if(subclassFixture == ::CEEFIT::null)                                                                               \
              {                                                                                                                   \
                throw new ::CEEFIT::EXCEPTION(::CEEFIT::STRING("GetFixtureSubclass failed to cast ") + #extendsType + " to " +    \
                                    #fixtureType + " because the types are not compatible.");                                     \
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
                                                                                                                                  
                                                                                                                                  
# define end_fit_fixture(fixtureType)                                                                                             \
      private:                                                                                                                    \
        class FIXTUREENDBODY                                                                                                      \
        {                                                                                                                         \
          public:                                                                                                                 \
            inline FIXTUREENDBODY(void)                                                                                           \
            {                                                                                                                     \
              ::CEEFIT::RUNNER::DecInFixtureConstructor();                                                                        \
            }                                                                                                                     \
        };                                                                                                                        \
        FIXTUREENDBODY fixtureType##_EndBody;                                                                                     \
    };                                                                                                                            \
                                                                                                                                  \
    static ::CEEFIT::REGISTERFIXTURECLASS< fixtureType > fixtureType##_CeeFITFixtureRegistration(#fixtureType, fixtureType::PROVIDEFIXTUREALIAS::GetFixtureAlias());


# define begin_namespaced_fit_fixture(namespaceName, fixtureType, extendsType, alias)                                             \
    class fixtureType : public extendsType                                                                                        \
    {                                                                                                                             \
      private:                                                                                                                    \
        class FIXTUREBEGINBODY                                                                                                    \
        {                                                                                                                         \
          public:                                                                                                                 \
            inline ceefit_init_spec FIXTUREBEGINBODY(void)                                                                        \
            {                                                                                                                     \
              ::CEEFIT::RUNNER::IncInFixtureConstructor();                                                                        \
            }                                                                                                                     \
        };                                                                                                                        \
        FIXTUREBEGINBODY fixtureType##_BeginBody;                                                                                 \
                                                                                                                                  \
      public:                                                                                                                     \
        inline ceefit_init_spec fixtureType(void) {}                                                                              \
        inline virtual ceefit_dtor_spec ~fixtureType(void) {}                                                                     \
                                                                                                                                  \
      private:                                                                                                                    \
        fixtureType& operator=(const fixtureType&);                                                                               \
        fixtureType(const fixtureType&);                                                                                          \
                                                                                                                                  \
      public:                                                                                                                     \
        class PROVIDEFIXTUREOBJECTCASTER                                                                                          \
        {                                                                                                                         \
          public:                                                                                                                 \
            static inline fixtureType* GetFixtureSubclass(::CEEFIT::FIXTURE* aFixture)                                            \
            {                                                                                                                     \
              extendsType* extendsFixture = dynamic_cast< extendsType* >(aFixture);                                               \
              if(extendsFixture == ::CEEFIT::null)                                                                                \
              {                                                                                                                   \
                throw new ::CEEFIT::EXCEPTION(::CEEFIT::STRING("GetFixtureSubclass failed to cast FIXTURE to ") + #extendsType +  \
                                              " because the types are not compatible.");                                          \
              }                                                                                                                   \
                                                                                                                                  \
              fixtureType* subclassFixture = static_cast< fixtureType* >(extendsFixture);                                         \
              if(subclassFixture == ::CEEFIT::null)                                                                               \
              {                                                                                                                   \
                throw new ::CEEFIT::EXCEPTION(::CEEFIT::STRING("GetFixtureSubclass failed to cast ") + #extendsType + " to " +    \
                                              #namespaceName #fixtureType + " because the types are not compatible.");            \
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
                                                                                                                                  
                                                                                                                                  
# define end_namespaced_fit_fixture(namespaceName, fixtureType)                                                                   \
      private:                                                                                                                    \
        class FIXTUREENDBODY                                                                                                      \
        {                                                                                                                         \
          public:                                                                                                                 \
            inline FIXTUREENDBODY(void)                                                                                           \
            {                                                                                                                     \
              ::CEEFIT::RUNNER::DecInFixtureConstructor();                                                                        \
            }                                                                                                                     \
        };                                                                                                                        \
        FIXTUREENDBODY fixtureType##_EndBody;                                                                                     \
    };                                                                                                                            \
                                                                                                                                  \
    static ::CEEFIT::REGISTERFIXTURECLASS< fixtureType > fixtureType##_CeeFITFixtureRegistration(#namespaceName "::" #fixtureType, fixtureType::PROVIDEFIXTUREALIAS::GetFixtureAlias());



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
      ::CEEFIT::SETFIELDINFO<PROVIDEFIELDINFO_##varName> varName##_SetFitFieldInfo;                                               \
                                                                                                                                  \
    public:                                                                                                                       \
      class ENDFITVARMACRO_##varName { private: ENDFITVARMACRO_##varName (void); ENDFITVARMACRO_##varName (ENDFITVARMACRO_##varName &); }


# define fit_test(testName, returnType)                                                                                           \
      friend returnType ceefit_call_spec testName(void);                                                                          \
                                                                                                                                  \
    private:                                                                                                                      \
      friend class PROVIDETESTCALLER_##testName;                                                                                  \
      class PROVIDETESTCALLER_##testName                                                                                          \
      {                                                                                                                           \
        public:                                                                                                                   \
          static inline returnType CallFitTest(::CEEFIT::PTR< ::CEEFIT::FIXTURE >& fixture)                                       \
          {                                                                                                                       \
            return(PROVIDEFIXTUREOBJECTCASTER::GetFixtureSubclass(fixture.GetPointer())->testName());                             \
          }                                                                                                                       \
      };                                                                                                                          \
                                                                                                                                  \
      ::FITTEST_AUTO< returnType, PROVIDETESTCALLER_##testName > testName##_CeeFITTestRegistration;                               \
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
      inline returnType ceefit_call_spec testName()
                                                                                                                                  
                                                                                                                                  
# ifndef _DEBUG                                                                                                                   
#   define fit_assert(booleanExpr)                                                                                                \
      if(!(booleanExpr))                                                                                                          \
        throw new FITASSERTIONFAILED("\0", 0)
# else                                                                                                                            
#   define fit_assert(booleanExpr)                                                                                                \
      if(!(booleanExpr))                                                                                                          \
        throw new FITASSERTIONFAILED(__FILE__, __LINE__)
#endif                                                                                                                            
                                                                                                                                  
                                                                                                                                  
# ifndef _DEBUG                                                                                                                   
#   define fit_fail()                                                                                                             \
      throw new FITFAILED("\0", 0)
# else                                                                                                                            
#   define fit_fail()                                                                                                             \
      throw new FITFAILED(__FILE__, __LINE__)
# endif                                                                                                                           
                                                                                                                                  
# define declare_fit_module(moduleName)                                                                                           \
    int __FIT_MODULE_##moduleName                                                                                                 
                                                                                                                                  
# define force_link_fit_module(moduleName)                                                                                        \
    extern int __FIT_MODULE_##moduleName;                                                                                         \
    int __FORCE_LINK_FIT_MODULE_##moduleName = __FIT_MODULE_##moduleName                                                          

#endif // __CEEFIT_MACROS_H__
