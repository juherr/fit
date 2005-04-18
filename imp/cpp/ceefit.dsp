# Microsoft Developer Studio Project File - Name="ceefit" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ceefit - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ceefit.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ceefit.mak" CFG="ceefit - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ceefit - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ceefit - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ceefit - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /Ob2 /I "src" /I "lib\libunicode-0.4" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "ceefit - Win32 Debug"

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
# ADD CPP /nologo /Gr /MTd /W3 /Gm /GR /GX /Zi /Od /I "src" /I "lib\libunicode-0.4" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
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

# Name "ceefit - Win32 Release"
# Name "ceefit - Win32 Debug"
# Begin Group "CeeFIT Engine Source Files"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\src\ceefit\ActionFixture.cpp

!IF  "$(CFG)" == "ceefit - Win32 Release"

# ADD CPP /Gd
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "ceefit - Win32 Debug"

# ADD CPP /Gd /Gm- /Gi- /GR /GX /Zi /FR /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\ceefit\ceefit.cpp

!IF  "$(CFG)" == "ceefit - Win32 Release"

!ELSEIF  "$(CFG)" == "ceefit - Win32 Debug"

# ADD CPP /Zi

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\ceefit\ColumnFixture.cpp

!IF  "$(CFG)" == "ceefit - Win32 Release"

# ADD CPP /Gd
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "ceefit - Win32 Debug"

# ADD CPP /Gd /Gm- /Gi- /GR /GX /Zi /FR /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\ceefit\FileRunner.cpp

!IF  "$(CFG)" == "ceefit - Win32 Release"

# ADD CPP /Gd
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "ceefit - Win32 Debug"

# ADD CPP /Gd /Gm- /Gi- /GR /GX /Zi /FR /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\ceefit\Fixture.cpp

!IF  "$(CFG)" == "ceefit - Win32 Release"

# ADD CPP /Gd
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "ceefit - Win32 Debug"

# ADD CPP /Gd /Gm- /Gi- /GR /GX /Zi /FR /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\ceefit\Parse.cpp

!IF  "$(CFG)" == "ceefit - Win32 Release"

# ADD CPP /Gd
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "ceefit - Win32 Debug"

# ADD CPP /Gd /Gm- /Gi- /GR /GX /Zi /FR /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\ceefit\PrimitiveFixture.cpp

!IF  "$(CFG)" == "ceefit - Win32 Release"

# ADD CPP /Gd
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "ceefit - Win32 Debug"

# ADD CPP /Gd /Gm- /Gi- /GR /GX /Zi /FR /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\ceefit\RowFixture.cpp

!IF  "$(CFG)" == "ceefit - Win32 Release"

# ADD CPP /Gd
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "ceefit - Win32 Debug"

# ADD CPP /Gd /Gm- /Gi- /GR /GX /Zi /FR /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\ceefit\ScientificDouble.cpp

!IF  "$(CFG)" == "ceefit - Win32 Release"

# ADD CPP /Gd
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "ceefit - Win32 Debug"

# ADD CPP /Gd /Gm- /Gi- /GR /GX /Zi /FR /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\ceefit\Summary.cpp

!IF  "$(CFG)" == "ceefit - Win32 Release"

# ADD CPP /Gd
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "ceefit - Win32 Debug"

# ADD CPP /Gd /Gm- /Gi- /GR /GX /Zi /FR /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\ceefit\TimedActionFixture.cpp

!IF  "$(CFG)" == "ceefit - Win32 Release"

# ADD CPP /Gd
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "ceefit - Win32 Debug"

# ADD CPP /Gd /Gm- /Gi- /GR /GX /Zi /FR /YX

!ENDIF 

# End Source File
# End Group
# Begin Group "CeeFIT Engine Header Files"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\src\ceefit\ActionFixture.h
# End Source File
# Begin Source File

SOURCE=.\src\tools\alloc.h
# End Source File
# Begin Source File

SOURCE=.\src\ceefit\ColumnFixture.h
# End Source File
# Begin Source File

SOURCE=.\src\ceefit\FileRunner.h
# End Source File
# Begin Source File

SOURCE=.\src\ceefit\fitfield.h
# End Source File
# Begin Source File

SOURCE=.\src\ceefit\fittest.h
# End Source File
# Begin Source File

SOURCE=.\src\ceefit\Fixture.h
# End Source File
# Begin Source File

SOURCE=.\src\ceefit\macros.h
# End Source File
# Begin Source File

SOURCE=.\src\ceefit\mandatory.h
# End Source File
# Begin Source File

SOURCE=.\src\ceefit\Parse.h
# End Source File
# Begin Source File

SOURCE=.\src\ceefit\PrimitiveFixture.h
# End Source File
# Begin Source File

SOURCE=.\src\ceefit\registerstatic.h
# End Source File
# Begin Source File

SOURCE=.\src\ceefit\ScientificDouble.h
# End Source File
# End Group
# Begin Group "Tools Header Files"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\src\tools\dynarray.h
# End Source File
# Begin Source File

SOURCE=.\src\tools\equality.h
# End Source File
# Begin Source File

SOURCE=.\src\tools\exception.h
# End Source File
# Begin Source File

SOURCE=.\src\tools\failure.h
# End Source File
# Begin Source File

SOURCE=.\src\tools\misc.h
# End Source File
# Begin Source File

SOURCE=.\src\tools\object.h
# End Source File
# Begin Source File

SOURCE=.\src\tools\ptr.h
# End Source File
# Begin Source File

SOURCE=.\src\tools\refcounted.h
# End Source File
# Begin Source File

SOURCE=.\src\tools\slink.h
# End Source File
# Begin Source File

SOURCE=.\src\tools\string.h
# End Source File
# Begin Source File

SOURCE=.\src\tools\stringdata.h
# End Source File
# Begin Source File

SOURCE=.\src\tools\value.h
# End Source File
# Begin Source File

SOURCE=.\src\tools\writer.h
# End Source File
# End Group
# Begin Group "Tools Source Files"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\src\tools\alloc.cpp

!IF  "$(CFG)" == "ceefit - Win32 Release"

# ADD CPP /Gd
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "ceefit - Win32 Debug"

# ADD CPP /Gd /Gm- /Gi- /GR /GX /FR /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\tools\exception.cpp

!IF  "$(CFG)" == "ceefit - Win32 Release"

# SUBTRACT CPP /YX /Yc

!ELSEIF  "$(CFG)" == "ceefit - Win32 Debug"

# ADD CPP /Gm- /Gi- /GR /GX /FR /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\tools\failure.cpp
# End Source File
# Begin Source File

SOURCE=.\src\tools\object.cpp

!IF  "$(CFG)" == "ceefit - Win32 Release"

# SUBTRACT CPP /YX /Yc

!ELSEIF  "$(CFG)" == "ceefit - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\tools\refcounted.cpp

!IF  "$(CFG)" == "ceefit - Win32 Release"

# SUBTRACT CPP /YX /Yc

!ELSEIF  "$(CFG)" == "ceefit - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\tools\registerstatic.cpp

!IF  "$(CFG)" == "ceefit - Win32 Release"

# SUBTRACT CPP /YX /Yc

!ELSEIF  "$(CFG)" == "ceefit - Win32 Debug"

# ADD CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\tools\string.cpp

!IF  "$(CFG)" == "ceefit - Win32 Release"

# ADD CPP /Gd
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "ceefit - Win32 Debug"

# ADD CPP /Gd /Gm- /Gi- /GR /GX /FR /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\tools\writer.cpp
# End Source File
# End Group
# Begin Group "Main CeeFIT Header"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\src\ceefit.h
# End Source File
# End Group
# Begin Group "libunicode-0.4 Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\lib\libunicode-0.4\convert.c"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\init.c"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\iso8859.c"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\latin1.c"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\prop.c"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\sjis.c"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\ucs2.c"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\ucs4.c"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\utf8.c"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\utf8conv.c"
# End Source File
# End Group
# Begin Group "libunicode-0.4 Header Files"

# PROP Default_Filter ""
# Begin Group "msft Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\lib\libunicode-0.4\msft\cp932.h"
# End Source File
# End Group
# Begin Group "iso Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\lib\libunicode-0.4\iso\armscii-8.h"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\iso\georgian-academy.h"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\iso\georgian-ps.h"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\iso\iso8859-10.h"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\iso\iso8859-14.h"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\iso\iso8859-15.h"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\iso\iso8859-2.h"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\iso\iso8859-3.h"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\iso\iso8859-4.h"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\iso\iso8859-5.h"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\iso\iso8859-6.h"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\iso\iso8859-7.h"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\iso\iso8859-8.h"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\iso\iso8859-9.h"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\iso\koi8-r.h"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\iso\koi8-u.h"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\iso\tis620.2533-1.h"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\iso\windows-1252.h"
# End Source File
# End Group
# Begin Group "jis Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\lib\libunicode-0.4\jis\shiftjis.h"
# End Source File
# End Group
# Begin Source File

SOURCE=".\lib\libunicode-0.4\acconfig.h"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\chartables.h"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\config.h"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\convert.h"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\decomp.h"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\unicode.h"
# End Source File
# Begin Source File

SOURCE=".\lib\libunicode-0.4\utf8.h"
# End Source File
# End Group
# End Target
# End Project
