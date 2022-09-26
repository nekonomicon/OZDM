# Microsoft Developer Studio Generated NMAKE File, Based on mp.dsp
!IF "$(CFG)" == ""
CFG=mp - Win32 Release
!MESSAGE No configuration specified. Defaulting to mp - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "mp - Win32 Release" && "$(CFG)" != "mp - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mp.mak" CFG="mp - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mp - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "mp - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "mp - Win32 Release"

OUTDIR=.
INTDIR=.
# Begin Custom Macros
OutDir=.\ 
# End Custom Macros

ALL : "$(OUTDIR)\mp.dll" "$(OUTDIR)\mp.bsc"


CLEAN :
	-@erase "$(INTDIR)\airtank.obj"
	-@erase "$(INTDIR)\airtank.sbr"
	-@erase "$(INTDIR)\animating.obj"
	-@erase "$(INTDIR)\animating.sbr"
	-@erase "$(INTDIR)\animation.obj"
	-@erase "$(INTDIR)\animation.sbr"
	-@erase "$(INTDIR)\bmodels.obj"
	-@erase "$(INTDIR)\bmodels.sbr"
	-@erase "$(INTDIR)\buttons.obj"
	-@erase "$(INTDIR)\buttons.sbr"
	-@erase "$(INTDIR)\cbase.obj"
	-@erase "$(INTDIR)\cbase.sbr"
	-@erase "$(INTDIR)\client.obj"
	-@erase "$(INTDIR)\client.sbr"
	-@erase "$(INTDIR)\combat.obj"
	-@erase "$(INTDIR)\combat.sbr"
	-@erase "$(INTDIR)\crossbow.obj"
	-@erase "$(INTDIR)\crossbow.sbr"
	-@erase "$(INTDIR)\crowbar.obj"
	-@erase "$(INTDIR)\crowbar.sbr"
	-@erase "$(INTDIR)\doors.obj"
	-@erase "$(INTDIR)\doors.sbr"
	-@erase "$(INTDIR)\effects.obj"
	-@erase "$(INTDIR)\effects.sbr"
	-@erase "$(INTDIR)\egon.obj"
	-@erase "$(INTDIR)\egon.sbr"
	-@erase "$(INTDIR)\explode.obj"
	-@erase "$(INTDIR)\explode.sbr"
	-@erase "$(INTDIR)\func_break.obj"
	-@erase "$(INTDIR)\func_break.sbr"
	-@erase "$(INTDIR)\func_tank.obj"
	-@erase "$(INTDIR)\func_tank.sbr"
	-@erase "$(INTDIR)\game.obj"
	-@erase "$(INTDIR)\game.sbr"
	-@erase "$(INTDIR)\gamerules.obj"
	-@erase "$(INTDIR)\gamerules.sbr"
	-@erase "$(INTDIR)\gauss.obj"
	-@erase "$(INTDIR)\gauss.sbr"
	-@erase "$(INTDIR)\ggrenade.obj"
	-@erase "$(INTDIR)\ggrenade.sbr"
	-@erase "$(INTDIR)\globals.obj"
	-@erase "$(INTDIR)\globals.sbr"
	-@erase "$(INTDIR)\glock.obj"
	-@erase "$(INTDIR)\glock.sbr"
	-@erase "$(INTDIR)\grapple.obj"
	-@erase "$(INTDIR)\grapple.sbr"
	-@erase "$(INTDIR)\h_ai.obj"
	-@erase "$(INTDIR)\h_ai.sbr"
	-@erase "$(INTDIR)\h_battery.obj"
	-@erase "$(INTDIR)\h_battery.sbr"
	-@erase "$(INTDIR)\h_cycler.obj"
	-@erase "$(INTDIR)\h_cycler.sbr"
	-@erase "$(INTDIR)\h_export.obj"
	-@erase "$(INTDIR)\h_export.sbr"
	-@erase "$(INTDIR)\handgrenade.obj"
	-@erase "$(INTDIR)\handgrenade.sbr"
	-@erase "$(INTDIR)\healthkit.obj"
	-@erase "$(INTDIR)\healthkit.sbr"
	-@erase "$(INTDIR)\hornet.obj"
	-@erase "$(INTDIR)\hornet.sbr"
	-@erase "$(INTDIR)\hornetgun.obj"
	-@erase "$(INTDIR)\hornetgun.sbr"
	-@erase "$(INTDIR)\ini.obj"
	-@erase "$(INTDIR)\ini.sbr"
	-@erase "$(INTDIR)\items.obj"
	-@erase "$(INTDIR)\items.sbr"
	-@erase "$(INTDIR)\lights.obj"
	-@erase "$(INTDIR)\lights.sbr"
	-@erase "$(INTDIR)\maprules.obj"
	-@erase "$(INTDIR)\maprules.sbr"
	-@erase "$(INTDIR)\mortar.obj"
	-@erase "$(INTDIR)\mortar.sbr"
	-@erase "$(INTDIR)\mp5.obj"
	-@erase "$(INTDIR)\mp5.sbr"
	-@erase "$(INTDIR)\mpstubb.obj"
	-@erase "$(INTDIR)\mpstubb.sbr"
	-@erase "$(INTDIR)\multiplay_gamerules.obj"
	-@erase "$(INTDIR)\multiplay_gamerules.sbr"
	-@erase "$(INTDIR)\pathcorner.obj"
	-@erase "$(INTDIR)\pathcorner.sbr"
	-@erase "$(INTDIR)\plane.obj"
	-@erase "$(INTDIR)\plane.sbr"
	-@erase "$(INTDIR)\plats.obj"
	-@erase "$(INTDIR)\plats.sbr"
	-@erase "$(INTDIR)\player.obj"
	-@erase "$(INTDIR)\player.sbr"
	-@erase "$(INTDIR)\python.obj"
	-@erase "$(INTDIR)\python.sbr"
	-@erase "$(INTDIR)\rpg.obj"
	-@erase "$(INTDIR)\rpg.sbr"
	-@erase "$(INTDIR)\satchel.obj"
	-@erase "$(INTDIR)\satchel.sbr"
	-@erase "$(INTDIR)\shotgun.obj"
	-@erase "$(INTDIR)\shotgun.sbr"
	-@erase "$(INTDIR)\singleplay_gamerules.obj"
	-@erase "$(INTDIR)\singleplay_gamerules.sbr"
	-@erase "$(INTDIR)\skill.obj"
	-@erase "$(INTDIR)\skill.sbr"
	-@erase "$(INTDIR)\sound.obj"
	-@erase "$(INTDIR)\sound.sbr"
	-@erase "$(INTDIR)\soundent.obj"
	-@erase "$(INTDIR)\soundent.sbr"
	-@erase "$(INTDIR)\spectator.obj"
	-@erase "$(INTDIR)\spectator.sbr"
	-@erase "$(INTDIR)\squeakgrenade.obj"
	-@erase "$(INTDIR)\squeakgrenade.sbr"
	-@erase "$(INTDIR)\subs.obj"
	-@erase "$(INTDIR)\subs.sbr"
	-@erase "$(INTDIR)\teamplay_gamerules.obj"
	-@erase "$(INTDIR)\teamplay_gamerules.sbr"
	-@erase "$(INTDIR)\triggers.obj"
	-@erase "$(INTDIR)\triggers.sbr"
	-@erase "$(INTDIR)\tripmine.obj"
	-@erase "$(INTDIR)\tripmine.sbr"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\weapons.obj"
	-@erase "$(INTDIR)\weapons.sbr"
	-@erase "$(INTDIR)\world.obj"
	-@erase "$(INTDIR)\world.sbr"
	-@erase "$(INTDIR)\xen.obj"
	-@erase "$(INTDIR)\xen.sbr"
	-@erase "$(OUTDIR)\mp.bsc"
	-@erase "$(OUTDIR)\mp.dll"
	-@erase "$(OUTDIR)\mp.exp"
	-@erase "$(OUTDIR)\mp.lib"
	-@erase "$(OUTDIR)\mp.map"
	-@erase "$(OUTDIR)\mp.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /G5 /MT /W3 /GX /Zi /O2 /I "..\engine" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "VALVE_DLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\mp.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\mp.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\airtank.sbr" \
	"$(INTDIR)\animating.sbr" \
	"$(INTDIR)\animation.sbr" \
	"$(INTDIR)\bmodels.sbr" \
	"$(INTDIR)\buttons.sbr" \
	"$(INTDIR)\cbase.sbr" \
	"$(INTDIR)\client.sbr" \
	"$(INTDIR)\combat.sbr" \
	"$(INTDIR)\crossbow.sbr" \
	"$(INTDIR)\crowbar.sbr" \
	"$(INTDIR)\doors.sbr" \
	"$(INTDIR)\effects.sbr" \
	"$(INTDIR)\egon.sbr" \
	"$(INTDIR)\explode.sbr" \
	"$(INTDIR)\func_break.sbr" \
	"$(INTDIR)\func_tank.sbr" \
	"$(INTDIR)\game.sbr" \
	"$(INTDIR)\gamerules.sbr" \
	"$(INTDIR)\gauss.sbr" \
	"$(INTDIR)\ggrenade.sbr" \
	"$(INTDIR)\globals.sbr" \
	"$(INTDIR)\glock.sbr" \
	"$(INTDIR)\grapple.sbr" \
	"$(INTDIR)\h_ai.sbr" \
	"$(INTDIR)\h_battery.sbr" \
	"$(INTDIR)\h_cycler.sbr" \
	"$(INTDIR)\h_export.sbr" \
	"$(INTDIR)\handgrenade.sbr" \
	"$(INTDIR)\healthkit.sbr" \
	"$(INTDIR)\hornet.sbr" \
	"$(INTDIR)\hornetgun.sbr" \
	"$(INTDIR)\ini.sbr" \
	"$(INTDIR)\items.sbr" \
	"$(INTDIR)\lights.sbr" \
	"$(INTDIR)\maprules.sbr" \
	"$(INTDIR)\mortar.sbr" \
	"$(INTDIR)\mp5.sbr" \
	"$(INTDIR)\mpstubb.sbr" \
	"$(INTDIR)\multiplay_gamerules.sbr" \
	"$(INTDIR)\pathcorner.sbr" \
	"$(INTDIR)\plane.sbr" \
	"$(INTDIR)\plats.sbr" \
	"$(INTDIR)\player.sbr" \
	"$(INTDIR)\python.sbr" \
	"$(INTDIR)\rpg.sbr" \
	"$(INTDIR)\satchel.sbr" \
	"$(INTDIR)\shotgun.sbr" \
	"$(INTDIR)\singleplay_gamerules.sbr" \
	"$(INTDIR)\skill.sbr" \
	"$(INTDIR)\sound.sbr" \
	"$(INTDIR)\soundent.sbr" \
	"$(INTDIR)\spectator.sbr" \
	"$(INTDIR)\squeakgrenade.sbr" \
	"$(INTDIR)\subs.sbr" \
	"$(INTDIR)\teamplay_gamerules.sbr" \
	"$(INTDIR)\triggers.sbr" \
	"$(INTDIR)\tripmine.sbr" \
	"$(INTDIR)\util.sbr" \
	"$(INTDIR)\weapons.sbr" \
	"$(INTDIR)\world.sbr" \
	"$(INTDIR)\xen.sbr"

"$(OUTDIR)\mp.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\mp.pdb" /map:"$(INTDIR)\mp.map" /debug /machine:I386 /def:".\mp.def" /out:"$(OUTDIR)\mp.dll" /implib:"$(OUTDIR)\mp.lib" 
LINK32_OBJS= \
	"$(INTDIR)\airtank.obj" \
	"$(INTDIR)\animating.obj" \
	"$(INTDIR)\animation.obj" \
	"$(INTDIR)\bmodels.obj" \
	"$(INTDIR)\buttons.obj" \
	"$(INTDIR)\cbase.obj" \
	"$(INTDIR)\client.obj" \
	"$(INTDIR)\combat.obj" \
	"$(INTDIR)\crossbow.obj" \
	"$(INTDIR)\crowbar.obj" \
	"$(INTDIR)\doors.obj" \
	"$(INTDIR)\effects.obj" \
	"$(INTDIR)\egon.obj" \
	"$(INTDIR)\explode.obj" \
	"$(INTDIR)\func_break.obj" \
	"$(INTDIR)\func_tank.obj" \
	"$(INTDIR)\game.obj" \
	"$(INTDIR)\gamerules.obj" \
	"$(INTDIR)\gauss.obj" \
	"$(INTDIR)\ggrenade.obj" \
	"$(INTDIR)\globals.obj" \
	"$(INTDIR)\glock.obj" \
	"$(INTDIR)\grapple.obj" \
	"$(INTDIR)\h_ai.obj" \
	"$(INTDIR)\h_battery.obj" \
	"$(INTDIR)\h_cycler.obj" \
	"$(INTDIR)\h_export.obj" \
	"$(INTDIR)\handgrenade.obj" \
	"$(INTDIR)\healthkit.obj" \
	"$(INTDIR)\hornet.obj" \
	"$(INTDIR)\hornetgun.obj" \
	"$(INTDIR)\ini.obj" \
	"$(INTDIR)\items.obj" \
	"$(INTDIR)\lights.obj" \
	"$(INTDIR)\maprules.obj" \
	"$(INTDIR)\mortar.obj" \
	"$(INTDIR)\mp5.obj" \
	"$(INTDIR)\mpstubb.obj" \
	"$(INTDIR)\multiplay_gamerules.obj" \
	"$(INTDIR)\pathcorner.obj" \
	"$(INTDIR)\plane.obj" \
	"$(INTDIR)\plats.obj" \
	"$(INTDIR)\player.obj" \
	"$(INTDIR)\python.obj" \
	"$(INTDIR)\rpg.obj" \
	"$(INTDIR)\satchel.obj" \
	"$(INTDIR)\shotgun.obj" \
	"$(INTDIR)\singleplay_gamerules.obj" \
	"$(INTDIR)\skill.obj" \
	"$(INTDIR)\sound.obj" \
	"$(INTDIR)\soundent.obj" \
	"$(INTDIR)\spectator.obj" \
	"$(INTDIR)\squeakgrenade.obj" \
	"$(INTDIR)\subs.obj" \
	"$(INTDIR)\teamplay_gamerules.obj" \
	"$(INTDIR)\triggers.obj" \
	"$(INTDIR)\tripmine.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\weapons.obj" \
	"$(INTDIR)\world.obj" \
	"$(INTDIR)\xen.obj"

"$(OUTDIR)\mp.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "mp - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\mp.dll" "$(OUTDIR)\mp.bsc"


CLEAN :
	-@erase "$(INTDIR)\airtank.obj"
	-@erase "$(INTDIR)\airtank.sbr"
	-@erase "$(INTDIR)\animating.obj"
	-@erase "$(INTDIR)\animating.sbr"
	-@erase "$(INTDIR)\animation.obj"
	-@erase "$(INTDIR)\animation.sbr"
	-@erase "$(INTDIR)\bmodels.obj"
	-@erase "$(INTDIR)\bmodels.sbr"
	-@erase "$(INTDIR)\buttons.obj"
	-@erase "$(INTDIR)\buttons.sbr"
	-@erase "$(INTDIR)\cbase.obj"
	-@erase "$(INTDIR)\cbase.sbr"
	-@erase "$(INTDIR)\client.obj"
	-@erase "$(INTDIR)\client.sbr"
	-@erase "$(INTDIR)\combat.obj"
	-@erase "$(INTDIR)\combat.sbr"
	-@erase "$(INTDIR)\crossbow.obj"
	-@erase "$(INTDIR)\crossbow.sbr"
	-@erase "$(INTDIR)\crowbar.obj"
	-@erase "$(INTDIR)\crowbar.sbr"
	-@erase "$(INTDIR)\doors.obj"
	-@erase "$(INTDIR)\doors.sbr"
	-@erase "$(INTDIR)\effects.obj"
	-@erase "$(INTDIR)\effects.sbr"
	-@erase "$(INTDIR)\egon.obj"
	-@erase "$(INTDIR)\egon.sbr"
	-@erase "$(INTDIR)\explode.obj"
	-@erase "$(INTDIR)\explode.sbr"
	-@erase "$(INTDIR)\func_break.obj"
	-@erase "$(INTDIR)\func_break.sbr"
	-@erase "$(INTDIR)\func_tank.obj"
	-@erase "$(INTDIR)\func_tank.sbr"
	-@erase "$(INTDIR)\game.obj"
	-@erase "$(INTDIR)\game.sbr"
	-@erase "$(INTDIR)\gamerules.obj"
	-@erase "$(INTDIR)\gamerules.sbr"
	-@erase "$(INTDIR)\gauss.obj"
	-@erase "$(INTDIR)\gauss.sbr"
	-@erase "$(INTDIR)\ggrenade.obj"
	-@erase "$(INTDIR)\ggrenade.sbr"
	-@erase "$(INTDIR)\globals.obj"
	-@erase "$(INTDIR)\globals.sbr"
	-@erase "$(INTDIR)\glock.obj"
	-@erase "$(INTDIR)\glock.sbr"
	-@erase "$(INTDIR)\grapple.obj"
	-@erase "$(INTDIR)\grapple.sbr"
	-@erase "$(INTDIR)\h_ai.obj"
	-@erase "$(INTDIR)\h_ai.sbr"
	-@erase "$(INTDIR)\h_battery.obj"
	-@erase "$(INTDIR)\h_battery.sbr"
	-@erase "$(INTDIR)\h_cycler.obj"
	-@erase "$(INTDIR)\h_cycler.sbr"
	-@erase "$(INTDIR)\h_export.obj"
	-@erase "$(INTDIR)\h_export.sbr"
	-@erase "$(INTDIR)\handgrenade.obj"
	-@erase "$(INTDIR)\handgrenade.sbr"
	-@erase "$(INTDIR)\healthkit.obj"
	-@erase "$(INTDIR)\healthkit.sbr"
	-@erase "$(INTDIR)\hornet.obj"
	-@erase "$(INTDIR)\hornet.sbr"
	-@erase "$(INTDIR)\hornetgun.obj"
	-@erase "$(INTDIR)\hornetgun.sbr"
	-@erase "$(INTDIR)\ini.obj"
	-@erase "$(INTDIR)\ini.sbr"
	-@erase "$(INTDIR)\items.obj"
	-@erase "$(INTDIR)\items.sbr"
	-@erase "$(INTDIR)\lights.obj"
	-@erase "$(INTDIR)\lights.sbr"
	-@erase "$(INTDIR)\maprules.obj"
	-@erase "$(INTDIR)\maprules.sbr"
	-@erase "$(INTDIR)\mortar.obj"
	-@erase "$(INTDIR)\mortar.sbr"
	-@erase "$(INTDIR)\mp5.obj"
	-@erase "$(INTDIR)\mp5.sbr"
	-@erase "$(INTDIR)\mpstubb.obj"
	-@erase "$(INTDIR)\mpstubb.sbr"
	-@erase "$(INTDIR)\multiplay_gamerules.obj"
	-@erase "$(INTDIR)\multiplay_gamerules.sbr"
	-@erase "$(INTDIR)\pathcorner.obj"
	-@erase "$(INTDIR)\pathcorner.sbr"
	-@erase "$(INTDIR)\plane.obj"
	-@erase "$(INTDIR)\plane.sbr"
	-@erase "$(INTDIR)\plats.obj"
	-@erase "$(INTDIR)\plats.sbr"
	-@erase "$(INTDIR)\player.obj"
	-@erase "$(INTDIR)\player.sbr"
	-@erase "$(INTDIR)\python.obj"
	-@erase "$(INTDIR)\python.sbr"
	-@erase "$(INTDIR)\rpg.obj"
	-@erase "$(INTDIR)\rpg.sbr"
	-@erase "$(INTDIR)\satchel.obj"
	-@erase "$(INTDIR)\satchel.sbr"
	-@erase "$(INTDIR)\shotgun.obj"
	-@erase "$(INTDIR)\shotgun.sbr"
	-@erase "$(INTDIR)\singleplay_gamerules.obj"
	-@erase "$(INTDIR)\singleplay_gamerules.sbr"
	-@erase "$(INTDIR)\skill.obj"
	-@erase "$(INTDIR)\skill.sbr"
	-@erase "$(INTDIR)\sound.obj"
	-@erase "$(INTDIR)\sound.sbr"
	-@erase "$(INTDIR)\soundent.obj"
	-@erase "$(INTDIR)\soundent.sbr"
	-@erase "$(INTDIR)\spectator.obj"
	-@erase "$(INTDIR)\spectator.sbr"
	-@erase "$(INTDIR)\squeakgrenade.obj"
	-@erase "$(INTDIR)\squeakgrenade.sbr"
	-@erase "$(INTDIR)\subs.obj"
	-@erase "$(INTDIR)\subs.sbr"
	-@erase "$(INTDIR)\teamplay_gamerules.obj"
	-@erase "$(INTDIR)\teamplay_gamerules.sbr"
	-@erase "$(INTDIR)\triggers.obj"
	-@erase "$(INTDIR)\triggers.sbr"
	-@erase "$(INTDIR)\tripmine.obj"
	-@erase "$(INTDIR)\tripmine.sbr"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\weapons.obj"
	-@erase "$(INTDIR)\weapons.sbr"
	-@erase "$(INTDIR)\world.obj"
	-@erase "$(INTDIR)\world.sbr"
	-@erase "$(INTDIR)\xen.obj"
	-@erase "$(INTDIR)\xen.sbr"
	-@erase "$(OUTDIR)\mp.bsc"
	-@erase "$(OUTDIR)\mp.dll"
	-@erase "$(OUTDIR)\mp.exp"
	-@erase "$(OUTDIR)\mp.lib"
	-@erase "$(OUTDIR)\mp.map"
	-@erase "$(OUTDIR)\mp.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /G5 /MT /W3 /GX /Zi /Od /I "..\engine" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "VALVE_DLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\mp.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\mp.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\airtank.sbr" \
	"$(INTDIR)\animating.sbr" \
	"$(INTDIR)\animation.sbr" \
	"$(INTDIR)\bmodels.sbr" \
	"$(INTDIR)\buttons.sbr" \
	"$(INTDIR)\cbase.sbr" \
	"$(INTDIR)\client.sbr" \
	"$(INTDIR)\combat.sbr" \
	"$(INTDIR)\crossbow.sbr" \
	"$(INTDIR)\crowbar.sbr" \
	"$(INTDIR)\doors.sbr" \
	"$(INTDIR)\effects.sbr" \
	"$(INTDIR)\egon.sbr" \
	"$(INTDIR)\explode.sbr" \
	"$(INTDIR)\func_break.sbr" \
	"$(INTDIR)\func_tank.sbr" \
	"$(INTDIR)\game.sbr" \
	"$(INTDIR)\gamerules.sbr" \
	"$(INTDIR)\gauss.sbr" \
	"$(INTDIR)\ggrenade.sbr" \
	"$(INTDIR)\globals.sbr" \
	"$(INTDIR)\glock.sbr" \
	"$(INTDIR)\grapple.sbr" \
	"$(INTDIR)\h_ai.sbr" \
	"$(INTDIR)\h_battery.sbr" \
	"$(INTDIR)\h_cycler.sbr" \
	"$(INTDIR)\h_export.sbr" \
	"$(INTDIR)\handgrenade.sbr" \
	"$(INTDIR)\healthkit.sbr" \
	"$(INTDIR)\hornet.sbr" \
	"$(INTDIR)\hornetgun.sbr" \
	"$(INTDIR)\ini.sbr" \
	"$(INTDIR)\items.sbr" \
	"$(INTDIR)\lights.sbr" \
	"$(INTDIR)\maprules.sbr" \
	"$(INTDIR)\mortar.sbr" \
	"$(INTDIR)\mp5.sbr" \
	"$(INTDIR)\mpstubb.sbr" \
	"$(INTDIR)\multiplay_gamerules.sbr" \
	"$(INTDIR)\pathcorner.sbr" \
	"$(INTDIR)\plane.sbr" \
	"$(INTDIR)\plats.sbr" \
	"$(INTDIR)\player.sbr" \
	"$(INTDIR)\python.sbr" \
	"$(INTDIR)\rpg.sbr" \
	"$(INTDIR)\satchel.sbr" \
	"$(INTDIR)\shotgun.sbr" \
	"$(INTDIR)\singleplay_gamerules.sbr" \
	"$(INTDIR)\skill.sbr" \
	"$(INTDIR)\sound.sbr" \
	"$(INTDIR)\soundent.sbr" \
	"$(INTDIR)\spectator.sbr" \
	"$(INTDIR)\squeakgrenade.sbr" \
	"$(INTDIR)\subs.sbr" \
	"$(INTDIR)\teamplay_gamerules.sbr" \
	"$(INTDIR)\triggers.sbr" \
	"$(INTDIR)\tripmine.sbr" \
	"$(INTDIR)\util.sbr" \
	"$(INTDIR)\weapons.sbr" \
	"$(INTDIR)\world.sbr" \
	"$(INTDIR)\xen.sbr"

"$(OUTDIR)\mp.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\mp.pdb" /map:"$(INTDIR)\mp.map" /debug /machine:I386 /def:".\mp.def" /out:"$(OUTDIR)\mp.dll" /implib:"$(OUTDIR)\mp.lib" 
LINK32_OBJS= \
	"$(INTDIR)\airtank.obj" \
	"$(INTDIR)\animating.obj" \
	"$(INTDIR)\animation.obj" \
	"$(INTDIR)\bmodels.obj" \
	"$(INTDIR)\buttons.obj" \
	"$(INTDIR)\cbase.obj" \
	"$(INTDIR)\client.obj" \
	"$(INTDIR)\combat.obj" \
	"$(INTDIR)\crossbow.obj" \
	"$(INTDIR)\crowbar.obj" \
	"$(INTDIR)\doors.obj" \
	"$(INTDIR)\effects.obj" \
	"$(INTDIR)\egon.obj" \
	"$(INTDIR)\explode.obj" \
	"$(INTDIR)\func_break.obj" \
	"$(INTDIR)\func_tank.obj" \
	"$(INTDIR)\game.obj" \
	"$(INTDIR)\gamerules.obj" \
	"$(INTDIR)\gauss.obj" \
	"$(INTDIR)\ggrenade.obj" \
	"$(INTDIR)\globals.obj" \
	"$(INTDIR)\glock.obj" \
	"$(INTDIR)\grapple.obj" \
	"$(INTDIR)\h_ai.obj" \
	"$(INTDIR)\h_battery.obj" \
	"$(INTDIR)\h_cycler.obj" \
	"$(INTDIR)\h_export.obj" \
	"$(INTDIR)\handgrenade.obj" \
	"$(INTDIR)\healthkit.obj" \
	"$(INTDIR)\hornet.obj" \
	"$(INTDIR)\hornetgun.obj" \
	"$(INTDIR)\ini.obj" \
	"$(INTDIR)\items.obj" \
	"$(INTDIR)\lights.obj" \
	"$(INTDIR)\maprules.obj" \
	"$(INTDIR)\mortar.obj" \
	"$(INTDIR)\mp5.obj" \
	"$(INTDIR)\mpstubb.obj" \
	"$(INTDIR)\multiplay_gamerules.obj" \
	"$(INTDIR)\pathcorner.obj" \
	"$(INTDIR)\plane.obj" \
	"$(INTDIR)\plats.obj" \
	"$(INTDIR)\player.obj" \
	"$(INTDIR)\python.obj" \
	"$(INTDIR)\rpg.obj" \
	"$(INTDIR)\satchel.obj" \
	"$(INTDIR)\shotgun.obj" \
	"$(INTDIR)\singleplay_gamerules.obj" \
	"$(INTDIR)\skill.obj" \
	"$(INTDIR)\sound.obj" \
	"$(INTDIR)\soundent.obj" \
	"$(INTDIR)\spectator.obj" \
	"$(INTDIR)\squeakgrenade.obj" \
	"$(INTDIR)\subs.obj" \
	"$(INTDIR)\teamplay_gamerules.obj" \
	"$(INTDIR)\triggers.obj" \
	"$(INTDIR)\tripmine.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\weapons.obj" \
	"$(INTDIR)\world.obj" \
	"$(INTDIR)\xen.obj"

"$(OUTDIR)\mp.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("mp.dep")
!INCLUDE "mp.dep"
!ELSE 
!MESSAGE Warning: cannot find "mp.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "mp - Win32 Release" || "$(CFG)" == "mp - Win32 Debug"
SOURCE=.\airtank.cpp

"$(INTDIR)\airtank.obj"	"$(INTDIR)\airtank.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\animating.cpp

"$(INTDIR)\animating.obj"	"$(INTDIR)\animating.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\animation.cpp

"$(INTDIR)\animation.obj"	"$(INTDIR)\animation.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\bmodels.cpp

"$(INTDIR)\bmodels.obj"	"$(INTDIR)\bmodels.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\buttons.cpp

"$(INTDIR)\buttons.obj"	"$(INTDIR)\buttons.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cbase.cpp

"$(INTDIR)\cbase.obj"	"$(INTDIR)\cbase.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\client.cpp

"$(INTDIR)\client.obj"	"$(INTDIR)\client.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\combat.cpp

"$(INTDIR)\combat.obj"	"$(INTDIR)\combat.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\crossbow.cpp

"$(INTDIR)\crossbow.obj"	"$(INTDIR)\crossbow.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\crowbar.cpp

"$(INTDIR)\crowbar.obj"	"$(INTDIR)\crowbar.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\doors.cpp

"$(INTDIR)\doors.obj"	"$(INTDIR)\doors.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\effects.cpp

"$(INTDIR)\effects.obj"	"$(INTDIR)\effects.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\egon.cpp

"$(INTDIR)\egon.obj"	"$(INTDIR)\egon.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\explode.cpp

"$(INTDIR)\explode.obj"	"$(INTDIR)\explode.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\func_break.cpp

"$(INTDIR)\func_break.obj"	"$(INTDIR)\func_break.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\func_tank.cpp

"$(INTDIR)\func_tank.obj"	"$(INTDIR)\func_tank.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\game.cpp

"$(INTDIR)\game.obj"	"$(INTDIR)\game.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gamerules.cpp

"$(INTDIR)\gamerules.obj"	"$(INTDIR)\gamerules.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gauss.cpp

"$(INTDIR)\gauss.obj"	"$(INTDIR)\gauss.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ggrenade.cpp

"$(INTDIR)\ggrenade.obj"	"$(INTDIR)\ggrenade.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\globals.cpp

"$(INTDIR)\globals.obj"	"$(INTDIR)\globals.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\glock.cpp

"$(INTDIR)\glock.obj"	"$(INTDIR)\glock.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\grapple.cpp

"$(INTDIR)\grapple.obj"	"$(INTDIR)\grapple.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\h_ai.cpp

"$(INTDIR)\h_ai.obj"	"$(INTDIR)\h_ai.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\h_battery.cpp

"$(INTDIR)\h_battery.obj"	"$(INTDIR)\h_battery.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\h_cycler.cpp

"$(INTDIR)\h_cycler.obj"	"$(INTDIR)\h_cycler.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\h_export.cpp

"$(INTDIR)\h_export.obj"	"$(INTDIR)\h_export.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\handgrenade.cpp

"$(INTDIR)\handgrenade.obj"	"$(INTDIR)\handgrenade.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\healthkit.cpp

"$(INTDIR)\healthkit.obj"	"$(INTDIR)\healthkit.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\hornet.cpp

"$(INTDIR)\hornet.obj"	"$(INTDIR)\hornet.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\hornetgun.cpp

"$(INTDIR)\hornetgun.obj"	"$(INTDIR)\hornetgun.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ini.cpp

"$(INTDIR)\ini.obj"	"$(INTDIR)\ini.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\items.cpp

"$(INTDIR)\items.obj"	"$(INTDIR)\items.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\lights.cpp

"$(INTDIR)\lights.obj"	"$(INTDIR)\lights.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\maprules.cpp

"$(INTDIR)\maprules.obj"	"$(INTDIR)\maprules.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mortar.cpp

"$(INTDIR)\mortar.obj"	"$(INTDIR)\mortar.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mp5.cpp

"$(INTDIR)\mp5.obj"	"$(INTDIR)\mp5.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mpstubb.cpp

"$(INTDIR)\mpstubb.obj"	"$(INTDIR)\mpstubb.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\multiplay_gamerules.cpp

"$(INTDIR)\multiplay_gamerules.obj"	"$(INTDIR)\multiplay_gamerules.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pathcorner.cpp

"$(INTDIR)\pathcorner.obj"	"$(INTDIR)\pathcorner.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\plane.cpp

"$(INTDIR)\plane.obj"	"$(INTDIR)\plane.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\plats.cpp

"$(INTDIR)\plats.obj"	"$(INTDIR)\plats.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\player.cpp

"$(INTDIR)\player.obj"	"$(INTDIR)\player.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\python.cpp

"$(INTDIR)\python.obj"	"$(INTDIR)\python.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\rpg.cpp

"$(INTDIR)\rpg.obj"	"$(INTDIR)\rpg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\satchel.cpp

"$(INTDIR)\satchel.obj"	"$(INTDIR)\satchel.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\shotgun.cpp

"$(INTDIR)\shotgun.obj"	"$(INTDIR)\shotgun.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\singleplay_gamerules.cpp

"$(INTDIR)\singleplay_gamerules.obj"	"$(INTDIR)\singleplay_gamerules.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\skill.cpp

"$(INTDIR)\skill.obj"	"$(INTDIR)\skill.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\sound.cpp

"$(INTDIR)\sound.obj"	"$(INTDIR)\sound.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\soundent.cpp

"$(INTDIR)\soundent.obj"	"$(INTDIR)\soundent.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\spectator.cpp

"$(INTDIR)\spectator.obj"	"$(INTDIR)\spectator.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\squeakgrenade.cpp

"$(INTDIR)\squeakgrenade.obj"	"$(INTDIR)\squeakgrenade.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\subs.cpp

"$(INTDIR)\subs.obj"	"$(INTDIR)\subs.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\teamplay_gamerules.cpp

"$(INTDIR)\teamplay_gamerules.obj"	"$(INTDIR)\teamplay_gamerules.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\triggers.cpp

"$(INTDIR)\triggers.obj"	"$(INTDIR)\triggers.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tripmine.cpp

"$(INTDIR)\tripmine.obj"	"$(INTDIR)\tripmine.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\util.cpp

"$(INTDIR)\util.obj"	"$(INTDIR)\util.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\weapons.cpp

"$(INTDIR)\weapons.obj"	"$(INTDIR)\weapons.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\world.cpp

"$(INTDIR)\world.obj"	"$(INTDIR)\world.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\xen.cpp

"$(INTDIR)\xen.obj"	"$(INTDIR)\xen.sbr" : $(SOURCE) "$(INTDIR)"



!ENDIF 

