/***
*
*	Copyright (c) 1999, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
/*

===== globals.cpp ========================================================

  DLL-wide global variable definitions.
  They're all defined here, for convenient centralization.
  Source files that need them should "extern ..." declare each
  variable, to better document what globals they care about.

*/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "soundent.h"

DLL_GLOBAL ULONG		g_ulFrameCount;
DLL_GLOBAL ULONG		g_ulModelIndexEyes;
DLL_GLOBAL ULONG		g_ulModelIndexPlayer;
DLL_GLOBAL Vector		g_vecAttackDir;
DLL_GLOBAL int			g_iSkillLevel;
DLL_GLOBAL int			gDisplayTitle;
DLL_GLOBAL BOOL			g_fGameOver;
DLL_GLOBAL const Vector	g_vecZero = Vector(0,0,0);
DLL_GLOBAL int			g_Language;
// Oz runes - global boolean
DLL_GLOBAL BOOL		g_runes_exist;

// Oz CTF - flags
DLL_GLOBAL BOOL		g_ctf_blueflagathome;
DLL_GLOBAL BOOL		g_ctf_redflagathome;

//jammer blue flag
DLL_GLOBAL bool gBlueFirstFlagSpawn;
DLL_GLOBAL bool gBlueFlagDroppedInTransit;
DLL_GLOBAL bool gBlueFlagAtHome;
DLL_GLOBAL Vector gBlueFlagStartPoint;
DLL_GLOBAL Vector gBlueFlagStartAngle;
entvars_t *gBlueFlag;

//jammer red flag
DLL_GLOBAL bool gRedFirstFlagSpawn;
DLL_GLOBAL bool gRedFlagDroppedInTransit;
DLL_GLOBAL bool gRedFlagAtHome;
DLL_GLOBAL Vector gRedFlagStartPoint;
DLL_GLOBAL Vector gRedFlagStartAngle;
entvars_t *gRedFlag;

void CtfMsgToAll(char *sztext);