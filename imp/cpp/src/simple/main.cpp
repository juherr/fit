#include "ceefit.h"

// ====== Init Release and MainLoop

void InitOpenGL()         { /* this inits something flashy */ }
void InitDirectSound()    { /* this inits something noisy */ }
void SynchronizeClocks()  { /* this inits something networky */ }
void ReleaseClocks()      { /* world time stops */ }
void ReleaseDirectSound() { /* volume == 0 */ }
void ReleaseOpenGL()      { /* fade to black */ }

int MainLoop()           
{ 
  printf("CeeFIT didn't run, forget command line params?\n");
  return(0); 
}


// ====== CeeFIT support routines

/**
 * <p>CeeFIT needs to know what heap alloc function your program uses.</p>
 *
 * <p>If yours isn't malloc, make a CEEFITALLOCFUNC that calls your allocator
 * and return that function pointer here.<p>
 */
CEEFITALLOCFUNC ceefit_call_spec GetCeeFitAllocFunc()
{
  return(malloc);
}

/**
 * <p>CeeFIT needs to know what heap free function your program uses.</p>
 *
 * <p>If yours isn't free, make a CEEFITFREEFUNC that calls your allocator
 * and return that function pointer here.<p>
 */
CEEFITFREEFUNC ceefit_call_spec GetCeeFitFreeFunc()
{
  return(free);
}

// Latch onto the ExampleMultiply variable from multiply.cpp
// If you can command your linker to force a linkage to multiply.o, then 
// this statement is not-required.
force_link_fit_module(ExampleMultiply);


// ====== Program main entry point

/**
 * <p>The main function for your existing program ...</p>
 */
int __cdecl main(int argc, char** argv)
{
  int exitCode = 0;

  InitOpenGL();
  InitDirectSound();
  SynchronizeClocks();

  int retVal;
  if((retVal = CEEFIT::Run(argc, argv)) > 0) 
  {
    printf("\nCeeFIT:  %s\n", retVal == 1 ? "pass" : "fail");
  }
  else
  {
    exitCode = MainLoop(); /* run the program normally if CeeFIT did nothing */
  }

  ReleaseClocks();
  ReleaseDirectSound();
  ReleaseOpenGL();

  return(exitCode);
}