#ifndef __EG_DLLTEST_FOODFIXTURE_H__
#define __EG_DLLTEST_FOODFIXTURE_H__

extern "C" 
{
  __declspec(dllimport) void ceefit_call_spec DoRegisterFoodFixtureDll(CEEFIT::SLINKLIST<CEEFIT::FIXTUREFACTORY>& fixtureFactoryListFromExe, ::CEEFITALLOCFUNC allocFuncFromExe, ::CEEFITFREEFUNC freeFuncFromExe);
};

#endif // __EG_DLLTEST_FOODFIXTURE_H__
