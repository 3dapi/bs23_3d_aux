# Microsoft Developer Studio Project File - Name="Mck" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Mck - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Mck.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Mck.mak" CFG="Mck - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Mck - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Mck - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Mck - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /Z7 /O2 /MD /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Z7 /O2 /MD /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c /I "$(DXSDK_DIR)include"
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 dsound.lib dinput8.lib d3dx9.lib d3d9.lib d3dxof.lib dxguid.lib winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /libpath:$(DXSDK_DIR)Lib\x86
# ADD LINK32 dsound.lib dinput8.lib d3dx9.lib d3d9.lib d3dxof.lib dxguid.lib winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /libpath:$(DXSDK_DIR)Lib\x86

!ELSEIF  "$(CFG)" == "Mck - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /MDd /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /GX /Z7 /Od /MDd /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c /I "$(DXSDK_DIR)include"
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 dsound.lib dinput8.lib d3dx9.lib d3d9.lib d3dxof.lib dxguid.lib winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:$(DXSDK_DIR)Lib\x86 /pdbtype:sept
# ADD LINK32 dsound.lib dinput8.lib d3dx9.lib d3d9.lib d3dxof.lib dxguid.lib winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:$(DXSDK_DIR)Lib\x86 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Mck - Win32 Release"
# Name "Mck - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\d3dapp.cpp
# End Source File
# Begin Source File

SOURCE=.\d3denum.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dutil.cpp
# End Source File
# Begin Source File

SOURCE=.\dxutil.cpp
# End Source File
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\McCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\McInput.cpp
# End Source File
# Begin Source File

SOURCE=.\Mck.cpp
# End Source File
# Begin Source File

SOURCE=.\Mck.rc
# End Source File
# Begin Source File

SOURCE=.\McScene.cpp
# End Source File
# Begin Source File

SOURCE=.\McXFile.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\_StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\d3dapp.h
# End Source File
# Begin Source File

SOURCE=.\d3denum.h
# End Source File
# Begin Source File

SOURCE=.\d3dsettings.h
# End Source File
# Begin Source File

SOURCE=.\d3dutil.h
# End Source File
# Begin Source File

SOURCE=.\dxutil.h
# End Source File
# Begin Source File

SOURCE=.\Main.h
# End Source File
# Begin Source File

SOURCE=.\McCamera.h
# End Source File
# Begin Source File

SOURCE=.\McInput.h
# End Source File
# Begin Source File

SOURCE=.\McScene.h
# End Source File
# Begin Source File

SOURCE=.\McUtil.h
# End Source File
# Begin Source File

SOURCE=.\McXFile.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
