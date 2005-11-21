# Microsoft Developer Studio Project File - Name="eg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=eg - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "eg.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "eg.mak" CFG="eg - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "eg - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "eg - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "eg - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "eg___Win32_Release"
# PROP BASE Intermediate_Dir "eg___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /Gd /MT /W3 /GR /GX /O2 /Ob2 /I "src" /I "lib" /I "lib/icu/include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "eg - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /Gd /MTd /W3 /Gm /GR /GX /Zi /Od /I "src" /I "lib" /I "lib/icu/include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "eg - Win32 Release"
# Name "eg - Win32 Debug"
# Begin Group "Music Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\eg\music\Browser.cpp
# End Source File
# Begin Source File

SOURCE=.\src\eg\music\Dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\src\eg\music\Display.cpp
# End Source File
# Begin Source File

SOURCE=.\src\eg\music\Music.cpp
# End Source File
# Begin Source File

SOURCE=.\src\eg\music\MusicLibrary.cpp
# End Source File
# Begin Source File

SOURCE=.\src\eg\music\MusicPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\eg\music\Realtime.cpp
# End Source File
# Begin Source File

SOURCE=.\src\eg\music\Simulator.cpp
# End Source File
# End Group
# Begin Group "Music Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\eg\music\Browser.h
# End Source File
# Begin Source File

SOURCE=.\src\eg\music\Dialog.h
# End Source File
# Begin Source File

SOURCE=.\src\eg\music\Display.h
# End Source File
# Begin Source File

SOURCE=.\src\eg\music\Music.h
# End Source File
# Begin Source File

SOURCE=.\src\eg\music\MusicLibrary.h
# End Source File
# Begin Source File

SOURCE=.\src\eg\music\MusicPlayer.h
# End Source File
# Begin Source File

SOURCE=.\src\eg\music\Realtime.h
# End Source File
# Begin Source File

SOURCE=.\src\eg\music\Simulator.h
# End Source File
# End Group
# Begin Group "Music Data Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\eg\music\Music.txt
# End Source File
# End Group
# Begin Group "Net Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\eg\net\GeoCoordinate.cpp
# End Source File
# Begin Source File

SOURCE=.\src\eg\net\GeoSimulator.cpp
# End Source File
# End Group
# Begin Group "Net Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\eg\net\GeoCoordinate.h
# End Source File
# Begin Source File

SOURCE=.\src\eg\net\GeoSimulator.h
# End Source File
# End Group
# Begin Group "eg Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\eg\Date.h
# End Source File
# Begin Source File

SOURCE=.\src\eg\eg.h
# End Source File
# Begin Source File

SOURCE=.\src\eg\ExampleTests.h
# End Source File
# Begin Source File

SOURCE=.\src\eg\Page.h
# End Source File
# End Group
# Begin Group "eg Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\eg\Calculator.cpp
# End Source File
# Begin Source File

SOURCE=.\src\eg\Date.cpp
# End Source File
# Begin Source File

SOURCE=.\src\eg\Division.cpp
# End Source File
# Begin Source File

SOURCE=.\src\eg\ExampleTests.cpp
# End Source File
# Begin Source File

SOURCE=.\src\eg\Page.cpp
# End Source File
# Begin Source File

SOURCE=.\src\eg\Sqrt.cpp
# End Source File
# End Group
# End Target
# End Project
