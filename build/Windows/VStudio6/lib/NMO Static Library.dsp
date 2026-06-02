# Microsoft Developer Studio Project File - Name="NMO Static Library" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=NMO Static Library - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NMO Static Library.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NMO Static Library.mak" CFG="NMO Static Library - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NMO Static Library - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "NMO Static Library - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NMO Static Library - Win32 Release"

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
# ADD CPP /nologo /G5 /MD /W3 /GX /O2 /I "../../../.." /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_LIBUNGIF" /Yu"nmo/NMO.hpp" /FD /c
# SUBTRACT CPP /Z<none>
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"nmo-sdk.lib"

!ELSEIF  "$(CFG)" == "NMO Static Library - Win32 Debug"

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
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I "../../../.." /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_LIBUNGIF" /Yu"nmo/NMO.hpp" /FD /GZ /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"nmo-sdkd.lib"

!ENDIF 

# Begin Target

# Name "NMO Static Library - Win32 Release"
# Name "NMO Static Library - Win32 Debug"
# Begin Group "addons"

# PROP Default_Filter ""
# Begin Group "digest"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\addons\digest\Digest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\digest\DigestMD5.cpp
# End Source File
# End Group
# Begin Group "gui"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GArrow.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GBox.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GButton.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GCheckButton.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GCheckmark.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GCheckWidget.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GCheckWidget.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GEvent.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GFrame.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GGrid.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GGrid.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GGridCell.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GLayout.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GLayout.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GLayoutPixmap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GLayoutPixmap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GLayoutVector.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GLayoutVector.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GLine.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GListBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GListBox.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GMisc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GObject.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GPanel.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GPicture.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GPicture.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GProgressBar.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GResource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GResourcePixmap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GResourcePixmap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GResourceVector.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GResourceVector.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GRoundButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GRoundButton.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GScrollBar.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GScrollBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GScrollBox.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GSelection.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GTabs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GText.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GTextEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GTextEdit.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GTexture.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GTexture.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GToggleButton.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GTrackBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GTrackBar.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\GUI.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GUpDown.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GUpDown.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GValueSelect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GValueSelect.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GWidget.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GWidget.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\gui\GWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\gui\GWindow.hpp
# End Source File
# End Group
# Begin Group "shell"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\addons\shell\ShellAlias.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\shell\ShellAlias.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\shell\ShellApplication.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\shell\ShellApplication.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\shell\ShellCommand.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\shell\ShellCommand.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\shell\ShellConst.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\addons\shell\ShellHistory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\addons\shell\ShellHistory.hpp
# End Source File
# End Group
# End Group
# Begin Group "base"

# PROP Default_Filter ""
# Begin Group "regex"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\base\regex\regex.c

!IF  "$(CFG)" == "NMO Static Library - Win32 Release"

# PROP Intermediate_Dir "Release\gnuregex"
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "NMO Static Library - Win32 Debug"

# PROP Intermediate_Dir "Debug\regex"
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\regex\regex.h

!IF  "$(CFG)" == "NMO Static Library - Win32 Release"

# PROP Intermediate_Dir "Release\gnuregex"

!ELSEIF  "$(CFG)" == "NMO Static Library - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\base\Args.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\Args.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\Array.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\Array.inl
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\Collection.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\Debug.cpp

!IF  "$(CFG)" == "NMO Static Library - Win32 Release"

!ELSEIF  "$(CFG)" == "NMO Static Library - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\Debug.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\Decimal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\Decimal.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\Defines.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\Defines.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\Exc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\Exc.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\ExcWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\Fifo.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\Fifo.inl
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\IList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\IList.inl
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\Includes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\Iterator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\Lifo.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\Lifo.inl
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\ListIterator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\ListIterator.inl
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\Memory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\Memory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\Memory.inl
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\MemoryWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\NMOObject.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\RegEx.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\RegEx.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\SharedObject.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\Stack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\Stack.inl
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\Stringz.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\Stringz.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\Stringz.inl
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\Types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\UChar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\UChar.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\UChar.inl
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\base\UStringz.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\UStringz.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\base\UStringz.inl
# End Source File
# End Group
# Begin Group "datatypes"

# PROP Default_Filter ""
# Begin Group "libpng"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\libpng\png.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\libpng\png.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\libpng\pngconf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\libpng\pngerror.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\libpng\pngget.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\libpng\pngmem.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\libpng\pngpread.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\libpng\pngread.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\libpng\pngrio.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\libpng\pngrtran.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\libpng\pngrutil.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\libpng\pngset.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\libpng\pngtrans.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\libpng\pngwio.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\libpng\pngwrite.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\libpng\pngwtran.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\libpng\pngwutil.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "zlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\adler32.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\compress.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\crc32.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\deflate.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\deflate.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\gzio.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\infblock.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\infblock.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\infcodes.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\infcodes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\inffast.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\inffast.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\inffixed.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\inflate.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\inftrees.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\inftrees.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\infutil.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\infutil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\trees.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\trees.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\uncompr.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\zconf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\zlib.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\zutil.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\zlib\zutil.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\DatatypePixmap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\datatypes\DatatypePixmap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\FileCfg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\datatypes\FileCfg.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\FileGif.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\datatypes\FileGif.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\FileIFF.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\datatypes\FileIFF.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\FilePal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\datatypes\FilePal.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\datatypes\FilePixmap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\FilePng.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\datatypes\FilePng.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\FileTxt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\datatypes\FileTxt.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\FileWav.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\datatypes\FileWav.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\datatypes\modules\Modungif.cpp
# End Source File
# End Group
# Begin Group "db"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\nmo\db\DBColumn.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\db\DBDatabase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\db\DBDatabase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\db\DBDatabaseODBC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\db\DBDynaset.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\db\DBDynaset.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\db\DBRecord.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\db\DBSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\db\DBSet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\db\DBSnapshot.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\db\DBSnapshot.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\db\DBTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\db\DBTable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\db\DBType.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\db\DBType.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\db\DBValue.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\db\DBValue.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\db\modules\ModODBC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\db\modules\ModODBCCP.cpp
# End Source File
# End Group
# Begin Group "interface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\nmo\interface\DefaultPal.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\GetTTUnicodeGlyphIndex.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\interface\ICachedChar.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\IColorSpace.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\interface\IColorSpace.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\interface\ICursor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\ICursorWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\IDisplay.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\interface\IDisplay.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\IDisplayChunky.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\interface\IDisplayChunky.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\IDisplayDIB.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\IDisplayDIB.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\IDisplayDirectDraw.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\IDisplayDirectDraw.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\IFont.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\interface\IFont.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\interface\IFontEngine.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\IFontEngineFixedPixmap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\IFontEngineFixedPixmap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\IFontEngineWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\interface\IKeyb.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\IKeybWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\IMouse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\interface\IMouse.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\IPalette.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\interface\IPalette.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\IPixmap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\interface\IPixmap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\IRect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\interface\IRect.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\interface\IRGBColor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\IScreen.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\interface\IScreen.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\IStdDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\interface\IStdDialog.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\interface\IStdDialogFile.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\interface\IStdDialogFileOpen.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\interface\IStdDialogFileSave.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\interface\IStdDialogMessage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\IStdDialogWin32.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\IWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\interface\IWindow.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\IWindowWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\SimpleDIB.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\SimpleDIB.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\WinApp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\interface\WinApp.hpp
# End Source File
# End Group
# Begin Group "net"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\net\InternetConnection.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\net\InternetConnection.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\net\InternetConnectionWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\net\NetID.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\net\NetID.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\net\NetIDIP.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\net\NetMessage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\net\NetMessage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\net\NetProtocol.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\net\NetStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\net\NetStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\net\NetTCP.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\net\NetUDP.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\net\URL.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\net\URL.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\net\Winsock.cpp
# End Source File
# End Group
# Begin Group "sound"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\sound\SoundBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\sound\SoundBuffer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sound\SoundBufferRaw.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sound\SoundBufferRaw.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sound\SoundDevice.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\sound\SoundDevice.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sound\SoundDeviceDirectSound.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sound\SoundDeviceDirectSound.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sound\SoundDeviceStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\sound\SoundDeviceStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\sound\SoundDeviceTracker.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sound\SoundDeviceWinMM.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sound\SoundDeviceWinMM.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\sound\SoundObject.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sound\WaveTrack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\sound\WaveTrack.hpp
# End Source File
# End Group
# Begin Group "sys"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\nmo\sys\Alarm.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sys\AlarmWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\sys\CfgStorage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sys\CfgStorageWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sys\DateTime.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\sys\DateTime.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sys\Dir.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\sys\Dir.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sys\DirScan.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\sys\DirScan.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\sys\DirSearch.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sys\DirSearchWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sys\DirWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sys\DynModule.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\sys\DynModule.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sys\DynModuleWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sys\Env.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\sys\Env.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sys\EnvPath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\sys\EnvPath.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sys\EnvPathWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sys\EnvWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sys\File.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\sys\File.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sys\FileWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\sys\Pipe.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sys\PipeWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sys\SignalEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\sys\SignalEvent.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\sys\SysInfo.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sys\SysInfoWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\sys\System.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sys\SystemWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\sys\Timer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\sys\TimerWin32.cpp
# End Source File
# End Group
# Begin Group "thread"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\thread\ThCriticalSection.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\thread\ThCriticalSection.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\thread\ThThread.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\thread\ThThreadWin32.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\Application.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\Application.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\Base.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\CGIApplication.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\CGIApplication.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\ConsoleApplication.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\ConsoleApplication.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\ConsoleApplicationWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\DaemonApplication.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\DaemonApplicationWin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\Datatypes.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\DB.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\FileStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\FileStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\GZipStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\GZipStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\Interface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\MemoryStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\MemoryStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\Net.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\NMO.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\nmopch.cpp
# ADD CPP /Yc"nmo/NMO.hpp"
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\ResourcesTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\ResourcesTable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\SharedObject.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\Sound.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\StdioApplication.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\StdioApplicationAnsi.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Stream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\Stream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\Sys.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\Thread.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\WindowApplication.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\nmo\WindowApplication.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\WindowApplicationWin32.cpp
# End Source File
# End Target
# End Project
