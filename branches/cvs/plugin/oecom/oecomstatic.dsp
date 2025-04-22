# Microsoft Developer Studio Project File - Name="oecomstatic" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=oecomstatic - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "oecomstatic.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "oecomstatic.mak" CFG="oecomstatic - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "oecomstatic - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "oecomstatic - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "oecomstatic - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../common" /I "../lib" /D "NDEBUG" /D "WIN32" /D "_LIB" /D "STATIC_LIBRARY" /YX /FD /c
# ADD BASE RSC /l 0x2c0a /d "NDEBUG"
# ADD RSC /l 0x2c0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../Release/oecomstatic.lib"

!ELSEIF  "$(CFG)" == "oecomstatic - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../common" /I "../lib" /D "_DEBUG" /D "WIN32" /D "_LIB" /D "STATIC_LIBRARY" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x2c0a /d "_DEBUG"
# ADD RSC /l 0x2c0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../Debug/oecomstatic.lib"

!ENDIF 

# Begin Target

# Name "oecomstatic - Win32 Release"
# Name "oecomstatic - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\oestore\folder.cpp

!IF  "$(CFG)" == "oecomstatic - Win32 Release"

!ELSEIF  "$(CFG)" == "oecomstatic - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\main.cpp

!IF  "$(CFG)" == "oecomstatic - Win32 Release"

!ELSEIF  "$(CFG)" == "oecomstatic - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\oestore\manager.cpp

!IF  "$(CFG)" == "oecomstatic - Win32 Release"

!ELSEIF  "$(CFG)" == "oecomstatic - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\oestore\message.cpp

!IF  "$(CFG)" == "oecomstatic - Win32 Release"

!ELSEIF  "$(CFG)" == "oecomstatic - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\oeapiobj.cpp

!IF  "$(CFG)" == "oecomstatic - Win32 Release"

!ELSEIF  "$(CFG)" == "oecomstatic - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\oecom.rc
# End Source File
# Begin Source File

SOURCE=.\user_interface.cpp

!IF  "$(CFG)" == "oecomstatic - Win32 Release"

!ELSEIF  "$(CFG)" == "oecomstatic - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\oestore\folder.h
# End Source File
# Begin Source File

SOURCE=.\main.h
# End Source File
# Begin Source File

SOURCE=..\oestore\manager.h
# End Source File
# Begin Source File

SOURCE=..\oestore\message.h
# End Source File
# Begin Source File

SOURCE=..\..\common\msoeapi.h
# End Source File
# Begin Source File

SOURCE=.\oeapiobj.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\user_interface.h
# End Source File
# End Group
# End Target
# End Project
