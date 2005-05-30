@echo off

setlocal

set DEBUG_BUILD=-Ddebug.mode=false
set RELEASE_BUILD=-Drelease.mode=false

:doSwitches
if "%1"=="-help" goto giveHelp
if "%1"=="-h" goto giveHelp
if "%1"=="-debugAnt" goto debugAnt
if "%1"=="-debug" goto debugBuild
if "%1"=="-release" goto releaseBuild
if "%1"=="-fastCompile" goto fastCompileBuild
if "%1"=="-msvc" goto useMsvc
if "%1"=="-g++" goto useGplusplus
goto checkEnv

:giveHelp
echo Ant script for CeeFIT
echo usage:  ant [-debugAnt] [{build.properties overrides}] {ant param(s)}
echo            -debugAnt     : Tells ant to print out debugging and
echo                            diagnostic information
echo .
echo          build.properties overrides:
echo            -fastCompile  : Force ant to build CeeFIT with debugging
echo                            information (overrides build.properties)
echo            -debug        : Force ant to build CeeFIT with debugging
echo                            information (overrides build.properties)
echo            -release      : Force ant to build CeeFIT with optimizations
echo                            enabled (overrides build.properties)
echo            -msvc         : Force ant to build using MSVC compiler and
echo                            linker (overrides build.properties)
echo            -g++          : Force ant to build using GCC's g++ compiler
echo                            and linker (overrides build.properties)
echo .
echo Set GCC_HOME or MSVCDir environment variables set prior to calling
echo ant.bat so that Ant knows where to find your C++ compiler.
echo .
echo   normal Ant help follows ...
echo . 
cmd /c lib\apache-ant-1.6.2\bin\ant -help
goto antDone

:debugAnt
set DEBUG_ANT_FLAG=-debug
shift
goto doSwitches

:releaseBuild
set DEBUG_BUILD=-Ddebug.mode=false
set RELEASE_BUILD=-Drelease.mode=true
shift
goto doSwitches

:debugBuild
set DEBUG_BUILD=-Ddebug.mode=true
set RELEASE_BUILD=-Drelease.mode=false
shift
goto doSwitches

:fastCompileBuild
set DEBUG_BUILD=-Ddebug.mode=false
set RELEASE_BUILD=-Drelease.mode=false
shift
goto doSwitches

:useMsvc
set USE_MSVC=-Dwhich.compiler=msvc -Dwhich.linker=msvc
set SKIP_GCC=true
shift 
goto doSwitches

:useGplusplus
set SKIP_MSVC=true
set USE_GPP=-Dwhich.compiler=g++ -Dwhich.linker=g++
shift
goto doSwitches

:checkEnv
set oldPATH=%PATH%
set oldINCLUDE=%INCLUDE%
set oldLIB=%LIB%

if "%SKIP_MSVC%"=="true" goto NoVC6
set MSVCDir_VAR=%MSVCDir%
if "%MSVCDir_VAR%"=="" goto NoVC6
set PATH=%MSVCDir%\bin;%PATH%
set INCLUDE=%MSVCDir%\include;%INCLUDE%
set LIB=%MSVCDir%\lib;%LIB%
goto runAnt

:NoVC6
if "%SKIP_GCC%"=="true" goto runAnt
set GCC_HOME_VAR=%GCC_HOME%
if "%GCC_HOME_VAR%"=="" goto runAnt
set PATH=%GCC_HOME%\bin;%PATH%
set INCLUDE=%GCC_HOME%\include;%INCLUDE%
set LIB=%GCC_HOME%\lib;%INCLUDE%
goto runAnt

:runAnt
cmd /c lib\apache-ant-1.6.2\bin\ant %DEBUG_ANT_FLAG% %DEBUG_BUILD% %RELEASE_BUILD% %USE_MSVC% %USE_GPP% %1 %2 %3 %4 %5 %6 %7 %8 %9

set PATH=%oldPATH%
set oldPATH=
set LIB=%oldLIB%
set oldLIB=
set INCLUDE=%oldINCLUDE%
set oldINCLUDE=
set GCC_HOME_VAR=
set MSVCDir_VAR=
set DEBUG_ANT_FLAG=
set DEBUG_BUILD=
set RELEASE_BUILD=
set SKIP_MSVC=
set SKIP_GCC=
set USE_MSVC=
set USE_GPP=

:antDone

endlocal
