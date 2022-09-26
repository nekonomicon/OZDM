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
// Robin, 4-22-98: Moved set_suicide_frame() here from player.cpp to allow us to 
//				   have one without a hardcoded player.mdl in tf_client.cpp

/*

===== client.cpp ========================================================

  client/server game specific stuff

*/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "saverestore.h"
#include "player.h"
#include "spectator.h"
#include "client.h"
#include "soundent.h"
#include "gamerules.h"
#include "grapple.h"
#include "game.h"

extern DLL_GLOBAL BOOL		g_ctf_redflagathome;
extern DLL_GLOBAL BOOL		g_ctf_blueflagathome;

//jammer infinity begin
int NumPlayerStarts;
int BluePlayerStarts;
int RedPlayerStarts;

//jammer ctf flag code
extern bool gRedFlagDroppedInTransit;
extern bool gBlueFlagDroppedInTransit;
extern bool gBlueFlagAtHome;
extern bool gRedFlagAtHome;
extern Vector gBlueFlagStartPoint;
extern Vector gBlueFlagStartAngle;
extern Vector gRedFlagStartPoint;
extern Vector gRedFlagStartAngle;
extern entvars_t *gBlueFlag;
extern entvars_t *gRedFlag;

extern void CtfMsgToAll(char *sztext);

extern BOOL FoundFile;
#include "weapons.h" //for the functions
extern char DatFileName[128];//world.cpp builds this string

//items data struct
itemdata_t FileItemData;

#include "io.h"
FILE *fp;

//this is for OZ ONLY, willi you trouble maker :-)
#define NUM_WEAPONS 13
//struct for weapons that are disabled
struct _weapons
{
 BOOL IsDisabled;
}Weapons[NUM_WEAPONS]; // 13 weapons to check for disabled

BOOL AllWeaponsDisabled;
void CheckDisabledWeapons(void);
//jammer end

extern DLL_GLOBAL ULONG		g_ulModelIndexPlayer;
extern DLL_GLOBAL BOOL		g_fGameOver;
extern DLL_GLOBAL int		g_iSkillLevel;
extern DLL_GLOBAL ULONG		g_ulFrameCount;

extern void CopyToBodyQue(entvars_t* pev);
extern int giPrecacheGrunt;
extern int gmsgSayText;

/*
 * used by kill command and disconnect command
 * ROBIN: Moved here from player.cpp, to allow multiple player models
 */
void set_suicide_frame(entvars_t* pev)
{       
	if (!FStrEq(STRING(pev->model), "models/player.mdl"))
		return; // allready gibbed

//	pev->frame		= $deatha11;
	pev->solid		= SOLID_NOT;
	pev->movetype	= MOVETYPE_TOSS;
	pev->deadflag	= DEAD_DEAD;
	pev->nextthink	= -1;
}


/*
===========
ClientConnect

called when a player connects to a server
============
*/
BOOL ClientConnect( edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ 128 ]  )
{	
	return g_pGameRules->ClientConnected( pEntity, pszName, pszAddress, szRejectReason );

// a client connecting during an intermission can cause problems
//	if (intermission_running)
//		ExitIntermission ();

}


/*
===========
ClientDisconnect

called when a player disconnects from a server

GLOBALS ASSUMED SET:  g_fGameOver
============
*/
void ClientDisconnect( edict_t *pEntity )
{
	if (g_fGameOver)
		return;
	
	entvars_t *pev = &pEntity->v;

	CBasePlayer *pPlayer = GetClassPtr((CBasePlayer *)pev);

	// Oz grapple
	if(pPlayer->pGrappleBolt)
	{
		if (pPlayer->IsHanging)
		{
			pPlayer->IsHanging = false;
			pPlayer->m_afPhysicsFlags &= ~PFLAG_ONTRAIN;
		}
		pev->movetype = MOVETYPE_WALK;
		pev->gravity = 1;
		pPlayer->pGrappleBolt->Killed(0,0);
		pPlayer->pGrappleBolt = NULL;
	}

	// Oz CTF - drop those flags on death!
	if (pPlayer->ctf_hasredflag)
	{
		//jammer ctf flag code
		pPlayer->ctf_hasredflag = false;
		g_ctf_redflagathome = false;
		gRedFlagDroppedInTransit = false;//just in case
		gRedFlagAtHome = true;
		
		gRedFlag->flags = FL_KILLME;
		CBaseEntity *pEntity = CBaseEntity::Create( "ctf_redflag", 
			              gRedFlagStartPoint,gRedFlagStartAngle,pPlayer->edict());


		char szText[128];
		sprintf(szText, "The Fire flag has returned to base!\n");
		CtfMsgToAll(szText);
	}
	if (pPlayer->ctf_hasblueflag)
	{
		//jammer ctf flag code
		pPlayer->ctf_hasblueflag = false;
		g_ctf_blueflagathome = false;
		gBlueFlagDroppedInTransit = false;//just in case
		gBlueFlagAtHome = true;

		gBlueFlag->flags = FL_KILLME;
		CBaseEntity *pEntity = CBaseEntity::Create( "ctf_blueflag", 
			              gBlueFlagStartPoint,gBlueFlagStartAngle,pPlayer->edict());

		char szText[128];
		sprintf(szText, "The Ice flag has returned to base!\n");
		CtfMsgToAll(szText);
	}

	// Oz runes - disconnect spawn
	if (pPlayer->ctf_hasfrag)
	{
		pPlayer->ctf_hasrune = false;
		pPlayer->ctf_hasfrag = false;

		pPlayer->SpawnRunes("ctf_frag");
	}

	if (pPlayer->ctf_hasregen)
	{
		pPlayer->ctf_hasrune = false;
		pPlayer->ctf_hasregen = false;

		pPlayer->SpawnRunes("ctf_regen");
	}

	if (pPlayer->ctf_hascrowbar)
	{
		pPlayer->ctf_hasrune = false;
		pPlayer->ctf_hascrowbar = false;

		pPlayer->SpawnRunes("ctf_crowbar");
	}

	if (pPlayer->ctf_hascycle)
	{
		pPlayer->ctf_hasrune = false;
		pPlayer->ctf_hascycle = false;

		pPlayer->SpawnRunes("ctf_cycle");
	}

	if (pPlayer->ctf_hascloak)
	{
		pPlayer->ctf_hasrune = false;
		pPlayer->ctf_hascloak = false;
		pev->rendermode = kRenderNormal;
		pev->renderfx &= ~kRenderFxGlowShell;

		pPlayer->SpawnRunes("ctf_cloak");
	}

	if (pPlayer->ctf_hashook)
	{
		pPlayer->ctf_hasrune = false;
		pPlayer->ctf_hashook = false;
	
		pPlayer->SpawnRunes("ctf_hook");
	}

	if (pPlayer->ctf_hasprotect)
	{
		pPlayer->ctf_hasrune = false;
		pPlayer->ctf_hasprotect = false;
	
		pPlayer->SpawnRunes("ctf_protect");
	}

	if (pPlayer->ctf_haslowgrav)
	{
		pPlayer->ctf_hasrune = false;
		pPlayer->ctf_haslowgrav = false;
	
		pPlayer->SpawnRunes("ctf_lowgrav");
	}

	if (pPlayer->ctf_hasvamp)
	{
		pPlayer->ctf_hasrune = false;
		pPlayer->ctf_hasvamp = false;
	
		pPlayer->SpawnRunes("ctf_vamp");
	}

	if (pPlayer->ctf_hastrap)
	{
		pPlayer->ctf_hasrune = false;
		pPlayer->ctf_hastrap = false;
	
		pPlayer->SpawnRunes("ctf_trap");
	}
	if (pPlayer->ctf_hassuperglock)//jammer new superglock
	{
		pPlayer->ctf_hasrune = false;
		pPlayer->ctf_hassuperglock = false;
	
		pPlayer->SpawnRunes("ctf_superglock");
	}

	char text[256];
	sprintf( text, "- %s has left the game\n", STRING(pEntity->v.netname) );
	MESSAGE_BEGIN( MSG_ALL, gmsgSayText, NULL );
		WRITE_BYTE( ENTINDEX(pEntity) );
		WRITE_STRING( text );
	MESSAGE_END();

	CSound *pSound;
	pSound = CSoundEnt::SoundPointerForIndex( CSoundEnt::ClientSoundIndex( pEntity ) );
	{
		// since this client isn't around to think anymore, reset their sound. 
		if ( pSound )
		{
			pSound->Reset();
		}
	}

// since the edict doesn't get deleted, fix it so it doesn't interfere.
	pEntity->v.takedamage = DAMAGE_NO;// don't attract autoaim
	pEntity->v.solid = SOLID_NOT;// nonsolid
	UTIL_SetOrigin ( &pEntity->v, pEntity->v.origin );

	g_pGameRules->ClientDisconnected( pEntity );
}


// called by ClientKill and DeadThink
void respawn(entvars_t* pev, BOOL fCopyCorpse)
{
	if (gpGlobals->coop || gpGlobals->deathmatch)
	{
		// Oz observer mode
		if ( fCopyCorpse && pev->movetype != MOVETYPE_NOCLIP)
		{
			// make a copy of the dead body for appearances sake
			CopyToBodyQue(pev);
		}

		// respawn player
		GetClassPtr( (CBasePlayer *)pev)->Spawn( );
	}
	else
	{       // restart the entire server
		SERVER_COMMAND("reload\n");
	}
}

/*
============
ClientKill

Player entered the suicide command

GLOBALS ASSUMED SET:  g_ulModelIndexPlayer
============
*/
void ClientKill( edict_t *pEntity )
{
	entvars_t *pev = &pEntity->v;

	CBasePlayer *pl = (CBasePlayer*) CBasePlayer::Instance( pev );

	// Oz observer mode
	if (pl->pev->flags == FL_SPECTATOR)
		return;

	if ( pl->m_fNextSuicideTime > gpGlobals->time )
		return;  // prevent suiciding too ofter

	pl->m_fNextSuicideTime = gpGlobals->time + 1;  // don't let them suicide for 5 seconds after suiciding

	//jam : Oz runes booby trap rune
	if(pl->ctf_hastrap)
		pl->wassuicide = TRUE;

	// have the player kill themself
	pev->health = 0;
	pl->Killed( pev, GIB_NEVER );

//	pev->modelindex = g_ulModelIndexPlayer;
//	pev->frags -= 2;		// extra penalty
//	respawn( pev );
}

/*
===========
ClientPutInServer

called each time a player is spawned
============
*/
void ClientPutInServer( edict_t *pEntity )
{
	CBasePlayer *pPlayer;

	entvars_t *pev = &pEntity->v;

	pPlayer = GetClassPtr((CBasePlayer *)pev);
	pPlayer->SetCustomDecalFrames(-1); // Assume none;

	// Allocate a CBasePlayer for pev, and call spawn
	pPlayer->Spawn() ;
}

//// HOST_SAY
// String comes in as
// say blah blah blah
// or as
// blah blah blah
//
void Host_Say( edict_t *pEntity, int teamonly )
{
	CBasePlayer *client;
	int		j;
	char	*p;
	char	text[128];
	char    szTemp[256];
	const char *cpSay = "say";
	const char *cpSayTeam = "say_team";
	const char *pcmd = CMD_ARGV(0);

	// We can get a raw string now, without the "say " prepended
	if ( CMD_ARGC() == 0 )
		return;

	if ( !stricmp( pcmd, cpSay) || !stricmp( pcmd, cpSayTeam ) )
	{
		if ( CMD_ARGC() >= 2 )
		{
			p = (char *)CMD_ARGS();
		}
		else
		{
			// say with a blank message, nothing to do
			return;
		}
	}
	else  // Raw text, need to prepend argv[0]
	{
		if ( CMD_ARGC() >= 2 )
		{
			sprintf( szTemp, "%s %s", ( char * )pcmd, (char *)CMD_ARGS() );
		}
		else
		{
			// Just a one word command, use the first word...sigh
			sprintf( szTemp, "%s", ( char * )pcmd );
		}
		p = szTemp;
	}

// remove quotes if present
	if (*p == '"')
	{
		p++;
		p[strlen(p)-1] = 0;
	}

// make sure the text has content
	for ( char *pc = p; pc != NULL && *pc != 0; pc++ )
	{
		if ( isprint( *pc ) && !isspace( *pc ) )
		{
			pc = NULL;	// we've found an alphanumeric character,  so text is valid
			break;
		}
	}
	if ( pc != NULL )
		return;  // no character found, so say nothing

// turn on color set 2  (color on,  no sound)
	if ( teamonly )
		sprintf( text, "%c(TEAM) %s: ", 2, STRING( pEntity->v.netname ) );
	else
		sprintf( text, "%c%s: ", 2, STRING( pEntity->v.netname ) );

	j = sizeof(text) - 2 - strlen(text);  // -2 for /n and null terminator
	if ( (int)strlen(p) > j )
		p[j] = 0;

	strcat( text, p );
	strcat( text, "\n" );

	// loop through all players
	// Start with the first player.
	// This may return the world in single player if the client types something between levels or during spawn
	// so check it, or it will infinite loop

	client = NULL;
	while ( ((client = (CBasePlayer*)UTIL_FindEntityByClassname( client, "player" )) != NULL) && (!FNullEnt(client->edict())) ) 
	{
		if ( !client->pev )
			continue;
		
		if ( client->edict() == pEntity )
			continue;

		if ( !(client->IsNetClient()) )	// Not a client ? (should never be true)
			continue;

		if ( teamonly && g_pGameRules->PlayerRelationship(client, CBaseEntity::Instance(pEntity)) != GR_TEAMMATE )
			continue;

		MESSAGE_BEGIN( MSG_ONE, gmsgSayText, NULL, client->pev );
			WRITE_BYTE( ENTINDEX(client->edict()) );
			WRITE_STRING( text );
		MESSAGE_END();

	}

	// print to the sending client
	MESSAGE_BEGIN( MSG_ONE, gmsgSayText, NULL, &pEntity->v );
		WRITE_BYTE( ENTINDEX(pEntity) );
		WRITE_STRING( text );
	MESSAGE_END();

	// echo to server console
	g_engfuncs.pfnServerPrint( text );
}


/*
===========
ClientCommand
called each time a player uses a "cmd" command
============
*/
extern float g_flWeaponCheat;

// Use CMD_ARGV,  CMD_ARGV, and CMD_ARGC to get pointers the character string command.
void ClientCommand( edict_t *pEntity )
{
	const char *pcmd = CMD_ARGV(0);
	const char *pstr;

	// Is the client spawned yet?
	if ( !pEntity->pvPrivateData )
		return;

	entvars_t *pev = &pEntity->v;

	if ( FStrEq(pcmd, "say" ) )
	{
		Host_Say( pEntity, 0 );
	}
	else if ( FStrEq(pcmd, "say_team" ) )
	{
		Host_Say( pEntity, 1 );
	}
	else if ( FStrEq(pcmd, "give" ) )
	{
		if ( g_flWeaponCheat != 0.0)
		{
			int iszItem = ALLOC_STRING( CMD_ARGV(1) );	// Make a copy of the classname
			GetClassPtr((CBasePlayer *)pev)->GiveNamedItem( STRING(iszItem) );
		}
	}

	else if ( FStrEq(pcmd, "drop" ) )
	{
		// player is dropping an item. 
		GetClassPtr((CBasePlayer *)pev)->DropPlayerItem((char *)CMD_ARGV(1));
	}
	else if ( FStrEq(pcmd, "fov" ) )
	{
		if ( g_flWeaponCheat && CMD_ARGC() > 1)
		{
			GetClassPtr((CBasePlayer *)pev)->m_iFOV = atoi( CMD_ARGV(1) );
		}
		else
		{
			CLIENT_PRINTF( pEntity, print_console, UTIL_VarArgs( "\"fov\" is \"%d\"\n", (int)GetClassPtr((CBasePlayer *)pev)->m_iFOV ) );
		}
	}
	else if ( FStrEq(pcmd, "use" ) )
	{
		GetClassPtr((CBasePlayer *)pev)->SelectItem((char *)CMD_ARGV(1));
	}
	else if (((pstr = strstr(pcmd, "weapon_")) != NULL)  && (pstr == pcmd))
	{
		GetClassPtr((CBasePlayer *)pev)->SelectItem(pcmd);
	}
	else if (FStrEq(pcmd, "lastinv" ))
	{
		GetClassPtr((CBasePlayer *)pev)->SelectLastItem();
	}
	// Oz grapple
	else if (FStrEq(pcmd, "+hook" ))
	{
		if (pev->deadflag == DEAD_RESPAWNABLE || pev->deadflag == DEAD_DYING || pev->deadflag == DEAD_DEAD || pev->movetype == MOVETYPE_NOCLIP || grappledisable.value > 0)
			return;

		CBasePlayer *plr = GetClassPtr((CBasePlayer *)pev);
	
		if(plr->pGrappleBolt)
			plr->pGrappleBolt->Killed(0,0);
		plr->pGrappleBolt = CGrappleBolt::BoltCreate();
		plr->pGrappleBolt->Init(plr);
	}
	else if (FStrEq(pcmd, "-hook" ))
	{
		CBasePlayer *plr = GetClassPtr((CBasePlayer *)pev);
	
		if(plr->pGrappleBolt)
		{
			if (plr->IsHanging)
			{
				plr->IsHanging = false;
				plr->m_afPhysicsFlags &= ~PFLAG_ONTRAIN;
			}
			// Oz runes - make sure we don't reset the gravity of
			// someone with the low gravity rune
			if (plr->ctf_haslowgrav)
			{
				pev->gravity = 1;
				pev->gravity *= lowgravruneamount.value;
			}
			else
				pev->gravity = 1;

			pev->movetype = MOVETYPE_WALK;
			plr->pGrappleBolt->Killed(0,0);
			plr->pGrappleBolt = NULL;
		}
	}
	// Oz grapple end
	// Oz personal message
	else if (FStrEq(pcmd, "inouye" ))
	{
		CBasePlayer *pPlayer = GetClassPtr((CBasePlayer *)pev);
	
		char szText[201];
		hudtextparms_t     hText;

		sprintf(szText, "The Originator and Programmer of\n\n              Oz Deathmatch\n\n         has entered the server\n\n             Say hello to Willi!\n");

		memset(&hText, 0, sizeof(hText));
		hText.channel = 1;
		// These X and Y coordinates are just above
		//  the health meter.
		hText.x = 0.30;
		hText.y = 0.25;
	    
		hText.effect = 0;    // Fade in/out
		            
		hText.r1 = hText.g1 = hText.b1 = 255;
		hText.a1 = 255;
		
		hText.r2 = hText.g2 = hText.b2 = 255;
		hText.a2 = 255;
		
		hText.fadeinTime = 1;
		hText.fadeoutTime = 5;
		hText.holdTime = 5;
		hText.fxTime = 0.5;
		
		UTIL_HudMessageAll(hText, szText);
	}
	// Oz personal message end
	// Oz maddog message
	else if (FStrEq(pcmd, "dawgsozserver" ))
	{
		CBasePlayer *pPlayer = GetClassPtr((CBasePlayer *)pev);
	
		char szText[201];
		hudtextparms_t     hText;

		sprintf(szText, "    The Official Server Admin of\n\n              Oz Deathmatch\n\n         has entered the server\n\n            Say hello to Dawg!\n");

		memset(&hText, 0, sizeof(hText));
		hText.channel = 2;
		// These X and Y coordinates are just above
		//  the health meter.
		hText.x = 0.30;
		hText.y = 0.25;
	    
		hText.effect = 0;    // Fade in/out
		            
		hText.r1 = hText.g1 = hText.b1 = 255;
		hText.a1 = 255;
		
		hText.r2 = hText.g2 = hText.b2 = 255;
		hText.a2 = 255;
		
		hText.fadeinTime = 1;
		hText.fadeoutTime = 5;
		hText.holdTime = 5;
		hText.fxTime = 0.5;
		
		UTIL_HudMessageAll(hText, szText);
	}
	// Oz maddog message end
	// Oz runes - drop command
	else if (FStrEq(pcmd, "droprune" ))
	{
		CBasePlayer *pPlayer = GetClassPtr((CBasePlayer *)pev);
		if (pPlayer->ctf_hasrune)
		{
			char szText[201];
			hudtextparms_t     hText;
			if(pPlayer->ctf_hasfrag)
				sprintf(szText, "Frag Rune Discarded!\n");
			else if(pPlayer->ctf_hasregen)
				sprintf(szText, "Regeneration Rune Discarded!\n");
			else if(pPlayer->ctf_hascrowbar)
				sprintf(szText, "Crowbar Rune Discarded!\n");
			else if(pPlayer->ctf_hascycle)
				sprintf(szText, "Haste Rune Discarded!\n");
			else if(pPlayer->ctf_hascloak)
				sprintf(szText, "Cloaking Rune Discarded!\n");
			else if(pPlayer->ctf_hashook)
				sprintf(szText, "Super Hook Rune Discarded!\n");
			else if(pPlayer->ctf_hasprotect)
				sprintf(szText, "Protection Rune Discarded!\n");
			else if(pPlayer->ctf_haslowgrav)
			{
				sprintf(szText, "Low Gravity Rune Discarded!\n");
				pev->gravity = 1;
			}
			else if(pPlayer->ctf_hasvamp)
				sprintf(szText, "Vampire Rune Discarded!\n");
			else if(pPlayer->ctf_hastrap)
				sprintf(szText, "Booby Trap Rune Discarded!\n");
			//jammer superglock
			else if(pPlayer->ctf_hassuperglock)
				sprintf(szText, "Super Glock Rune Discarded!\n");
			else
				sprintf(szText, "Rune Discarded!\n");
	
			memset(&hText, 0, sizeof(hText));
			hText.channel = 4;
			// These X and Y coordinates are just above
			//  the health meter.
			hText.x = 0.04;
			hText.y = 0.80;
    
			hText.effect = 0;    // Fade in/out
						
			hText.r1 = 230;
			hText.g1 = 230; 
			hText.b1 = 230;
			hText.a1 = 255;
			
			hText.r2 = hText.g2 = hText.b2 = 255;
			hText.a2 = 255;
			
			hText.fadeinTime = 0;
			hText.fadeoutTime = 0;
			hText.holdTime = 1;
			hText.fxTime = 0.5;
			
			UTIL_HudMessage(pPlayer ,hText, szText);
		}

		if (pPlayer->ctf_hasrune && pPlayer->ctf_hasfrag)
		{
			pPlayer->ctf_hasrune = false;
			pPlayer->ctf_hasfrag = false;

			pPlayer->SpawnRunes("ctf_frag");
		}
		if (pPlayer->ctf_hasrune && pPlayer->ctf_hasregen)
		{
			pPlayer->ctf_hasrune = false;
			pPlayer->ctf_hasregen = false;

			pPlayer->SpawnRunes("ctf_regen");
		}
		if (pPlayer->ctf_hasrune && pPlayer->ctf_hascrowbar)
		{
			pPlayer->ctf_hasrune = false;
			pPlayer->ctf_hascrowbar = false;

			pPlayer->SpawnRunes("ctf_crowbar");
		}
		if (pPlayer->ctf_hasrune && pPlayer->ctf_hascycle)
		{
			pPlayer->ctf_hasrune = false;
			pPlayer->ctf_hascycle = false;

			pPlayer->SpawnRunes("ctf_cycle");
		}
		if (pPlayer->ctf_hasrune && pPlayer->ctf_hascloak)
		{
			pPlayer->ctf_hasrune = false;
			pPlayer->ctf_hascloak = false;
			pev->rendermode = kRenderNormal;
	
			if (pPlayer->ctf_hasblueflag || pPlayer->ctf_hasredflag)
				pev->renderfx |= kRenderFxGlowShell;
			else
				pev->renderfx &= ~kRenderFxGlowShell;

			pPlayer->SpawnRunes("ctf_cloak");
		}
		if (pPlayer->ctf_hasrune && pPlayer->ctf_hashook)
		{
			pPlayer->ctf_hasrune = false;
			pPlayer->ctf_hashook = false;

			pPlayer->SpawnRunes("ctf_hook");
		}
		if (pPlayer->ctf_hasrune && pPlayer->ctf_hasprotect)
		{
			pPlayer->ctf_hasrune = false;
			pPlayer->ctf_hasprotect = false;

			pPlayer->SpawnRunes("ctf_protect");
		}
		if (pPlayer->ctf_hasrune && pPlayer->ctf_haslowgrav)
		{
			pPlayer->ctf_hasrune = false;
			pPlayer->ctf_haslowgrav = false;
			pev->gravity = 1;
	
			pPlayer->SpawnRunes("ctf_lowgrav");
		}
		if (pPlayer->ctf_hasrune && pPlayer->ctf_hasvamp)
		{
			pPlayer->ctf_hasrune = false;
			pPlayer->ctf_hasvamp = false;
	
			pPlayer->SpawnRunes("ctf_vamp");
		}
		if (pPlayer->ctf_hasrune && pPlayer->ctf_hastrap)
		{
			pPlayer->ctf_hasrune = false;
			pPlayer->ctf_hastrap = false;

			pPlayer->SpawnRunes("ctf_trap");
		}
		//jammer new superglock
		if (pPlayer->ctf_hasrune && pPlayer->ctf_hassuperglock)
		{
			pPlayer->ctf_hasrune = false;
			pPlayer->ctf_hassuperglock = false;

			pPlayer->SpawnRunes("ctf_superglock");
		}

	}
	// Oz runes - drop command end
	// Oz runes - current rune
	else if (FStrEq(pcmd, "runestatus" ))
	{
		CBasePlayer *pPlayer = GetClassPtr((CBasePlayer *)pev);

		char szText[201];
		hudtextparms_t     hText;
		if(pPlayer->ctf_hasfrag)
			sprintf(szText, "Frag Rune\n");
		else if(pPlayer->ctf_hasregen)
			sprintf(szText, "Regeneration Rune\n");
		else if(pPlayer->ctf_hascrowbar)
			sprintf(szText, "Crowbar Rune\n");
		else if(pPlayer->ctf_hascycle)
			sprintf(szText, "Haste Rune\n");
		else if(pPlayer->ctf_hascloak)
			sprintf(szText, "Cloaking Rune\n");
		else if(pPlayer->ctf_hashook)
			sprintf(szText, "Super Hook Rune\n");
		else if(pPlayer->ctf_hasprotect)
			sprintf(szText, "Protection Rune\n");
		else if(pPlayer->ctf_haslowgrav)
			sprintf(szText, "Low Gravity Rune\n");
		else if(pPlayer->ctf_hasvamp)
			sprintf(szText, "Vampire Rune\n");
		else if(pPlayer->ctf_hastrap)
			sprintf(szText, "Booby Trap Rune\n");
		//jammer superglock
		else if(pPlayer->ctf_hassuperglock)
			sprintf(szText, "Super Glock Rune\n");
		else
			sprintf(szText, "You don't have a Rune\n");
	
		memset(&hText, 0, sizeof(hText));
		hText.channel = 4;
		// These X and Y coordinates are just above
		//  the health meter.
		hText.x = 0.04;
		hText.y = 0.80;
    
		hText.effect = 0;    // Fade in/out
					
		hText.r1 = 230;
		hText.g1 = 230; 
		hText.b1 = 230;
		hText.a1 = 255;
			
		hText.r2 = hText.g2 = hText.b2 = 255;
		hText.a2 = 255;
		
		hText.fadeinTime = 0;
		hText.fadeoutTime = 0;
		hText.holdTime = 1;
		hText.fxTime = 0.5;
			
		UTIL_HudMessage(pPlayer ,hText, szText);
	}
	// Oz runes - current rune end
	// Oz CTF - flag dropping command
	else if ( FStrEq(pcmd, "dropflag" ) )
	{

		//jammer new ctf drop flag
		CBasePlayer *pPlayer = GetClassPtr((CBasePlayer *)pev);

		if(pPlayer->ctf_hasredflag || pPlayer->ctf_hasblueflag)
		{
	
			Vector VecHere = pPlayer->pev->origin + gpGlobals->v_forward*50;
			TraceResult tr;
			
			UTIL_TraceLine( pPlayer->pev->origin + gpGlobals->v_forward * 16, pPlayer->pev->origin + gpGlobals->v_forward * 128, dont_ignore_monsters, NULL, &tr );

			if(tr.fAllSolid == 0 && tr.fStartSolid == 0 && tr.flFraction > .75)
			{
				char *flag = NULL;

				if(pPlayer->ctf_hasredflag)
				{	
					//jammer ctf flag code
				 pPlayer->ctf_hasredflag = false;
				 gRedFlagDroppedInTransit = true;//dropped it not at home
				 STOP_SOUND( ENT(gRedFlag), CHAN_AUTO, "ambience/siren.wav" );
				 gRedFlag->flags = FL_KILLME;
				 flag = "ctf_redflag";
				}
				else
				{
					//jammer ctf flag code
				 pPlayer->ctf_hasblueflag = false;
				 gBlueFlagDroppedInTransit = true;//dropped it not at home
				 STOP_SOUND( ENT(gBlueFlag), CHAN_AUTO, "ambience/siren.wav" );
				 gBlueFlag->flags = FL_KILLME;
				 flag = "ctf_blueflag";
				}

				CBaseEntity *pEntity = CBaseEntity::Create( flag, VecHere, pPlayer->pev->v_angle, pPlayer->edict());
				pEntity->pev->velocity = gpGlobals->v_forward*150;
				pEntity->pev->velocity.z += 150;
				pEntity->pev->angles.x = 0;
				pEntity->pev->angles.y = pPlayer->pev->angles.y;
				pEntity->pev->angles.z = 0;
				
			}
			else
			{
				hudtextparms_t     hText;
				memset(&hText, 0, sizeof(hText));
				hText.channel = 4;
				hText.x = 0.04;
				hText.y = 0.80;
				hText.effect = 0;    // Fade in/out
				hText.r1 = 230;
				hText.g1 = 230; 
				hText.b1 = 230;
				hText.a1 = 255;
							
				hText.r2 = hText.g2 = hText.b2 = 255;
				hText.a2 = 255;
						
				hText.fadeinTime = 0;
				hText.fadeoutTime = 0;
				hText.holdTime = 1;
				hText.fxTime = 0.5;
				UTIL_HudMessage(pPlayer,hText,"Too close to the wall to drop flag!");
			}
		}//end if has flags
	}
	// Oz CTF - flag dropping command
	else if ( g_pGameRules->ClientCommand( GetClassPtr((CBasePlayer *)pev), pcmd ) )
	{
		// MenuSelect returns true only if the command is properly handled,  so don't print a warning
	}
	else
	{
		// tell the user they entered an unknown command
		ClientPrint( &pEntity->v, HUD_PRINTCONSOLE, UTIL_VarArgs( "Unknown command: %s\n", pcmd ) );
	}
}


/*
========================
ClientUserInfoChanged

called after the player changes
userinfo - gives dll a chance to modify it before
it gets sent into the rest of the engine.
========================
*/
void ClientUserInfoChanged( edict_t *pEntity, char *infobuffer )
{
	// Is the client spawned yet?
	if ( !pEntity->pvPrivateData )
		return;

	// msg everyone if someone changes their name,  and it isn't the first time (changing no name to current name)
	if ( pEntity->v.netname && STRING(pEntity->v.netname)[0] != 0 && !FStrEq( STRING(pEntity->v.netname), g_engfuncs.pfnInfoKeyValue( infobuffer, "name" )) )
	{
		char text[256];
		sprintf( text, "* %s changed name to %s\n", STRING(pEntity->v.netname), g_engfuncs.pfnInfoKeyValue( infobuffer, "name" ) );
		MESSAGE_BEGIN( MSG_ALL, gmsgSayText, NULL );
			WRITE_BYTE( ENTINDEX(pEntity) );
			WRITE_STRING( text );
		MESSAGE_END();

		UTIL_LogPrintf( "\"%s<%i>\" changed name to \"%s<%i>\"\n", STRING( pEntity->v.netname ), GETPLAYERUSERID( pEntity ), g_engfuncs.pfnInfoKeyValue( infobuffer, "name" ), GETPLAYERUSERID( pEntity ) );
	}

	g_pGameRules->ClientUserInfoChanged( GetClassPtr((CBasePlayer *)&pEntity->v), infobuffer );
}


void ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
{
	int				i;
	CBaseEntity		*pClass;

	//jammer begin
	NumPlayerStarts = 0; //set to 0 or else blamo
	//jammer ctf
	RedPlayerStarts = 0;
	BluePlayerStarts = 0;

	CBaseEntity *pOther = NULL;
	while((pOther = UTIL_FindEntityByClassname(pOther,"info_player_deathmatch")) != NULL)
	{
		NumPlayerStarts++;
	}

	if(ctf.value)
	{
	  while((pOther = UTIL_FindEntityByClassname(pOther,"ctf_redspawn")) != NULL)
		{
		  RedPlayerStarts++;
		}
	  while((pOther = UTIL_FindEntityByClassname(pOther,"ctf_bluespawn")) != NULL)
		{
		  BluePlayerStarts++;
		}
	}


	//reading in the dat file
    if(FoundFile)//found file. set in world.cpp
	{
		CheckDisabledWeapons();//what weapons can we NOT spawn

		if((access(DatFileName,4)) == 0)//found actual file, just make sure
		{
			FoundFile = FALSE;//set to false so my items can respawn, if this left true nothing will repsawn

			if((fp = fopen(DatFileName,"rb")) != NULL)
			{
				//start reading in structs
				while(fread(&FileItemData.data,sizeof(item_t),1,fp) != NULL)
				{
					//actual spawn func
					SpawnItem(FileItemData.data.posx,FileItemData.data.posy,
				       FileItemData.data.posz,FileItemData.data.angle,
					   FileItemData.data.weapon);
				}//end while

				fclose(fp);
			}//end if fopen
		}//end if access
	}//end if foundfile
	//jammer end


	// Clients have not been initialized yet
	for ( i = 0; i < edictCount; i++ )
	{
		if ( pEdictList[i].free )
			continue;
		
		// Clients aren't necessarily initialized until ClientPutInServer()
		if ( i < clientMax || !pEdictList[i].pvPrivateData )
			continue;

		pClass = CBaseEntity::Instance( &pEdictList[i] );
		// Activate this entity if it's got a class & isn't dormant
		if ( pClass && !(pClass->pev->flags & FL_DORMANT) )
		{
			pClass->Activate();
		}
		else
		{
			ALERT( at_console, "Can't instance %s\n", STRING(pEdictList[i].v.classname) );
		}
	}
}


/*
================
PlayerPreThink

Called every frame before physics are run
================
*/
void PlayerPreThink( edict_t *pEntity )
{
	entvars_t *pev = &pEntity->v;
	CBasePlayer *pPlayer = (CBasePlayer *)GET_PRIVATE(pEntity);

	if (pPlayer)
		pPlayer->PreThink( );
}

/*
================
PlayerPostThink

Called every frame after physics are run
================
*/
void PlayerPostThink( edict_t *pEntity )
{
	entvars_t *pev = &pEntity->v;
	CBasePlayer *pPlayer = (CBasePlayer *)GET_PRIVATE(pEntity);
	CBaseEntity *plr = (CBaseEntity *)GET_PRIVATE(pEntity);

	// Oz messaging
	if (pPlayer->m_iOz < 26)
		pPlayer->m_iOz++;

//	if (pPlayer->m_iGrapple < 26)
//		pPlayer->m_iGrapple++;

	if (pPlayer->m_iCommands < 26)
		pPlayer->m_iCommands++;

	if (pPlayer->m_iStart < 26)
		pPlayer->m_iStart++;

	if (pPlayer->m_iStart == 15)
	{
		pPlayer->MOTD( pPlayer->edict() );
	}

	if (pPlayer->m_iStart == 25)
	{
		char szText[201];
		hudtextparms_t     hText;

		sprintf(szText, "Fire or hit your +use key to begin\n");
			
		memset(&hText, 0, sizeof(hText));
		hText.channel = 1;
		// These X and Y coordinates are just above
		//  the health meter.
		hText.x = 0.01;
		hText.y = 0.85;
	    
		hText.effect = 0;    // Fade in/out
		            
		hText.r1 = hText.g1 = hText.b1 = 255;
		hText.a1 = 255;
		
		hText.r2 = hText.g2 = hText.b2 = 255;
		hText.a2 = 255;
		
		hText.fadeinTime = 1;
		hText.fadeoutTime = 5;
		hText.holdTime = 120;
		hText.fxTime = 0.5;
		
		//UTIL_HudMessageAll(hText, szText);
		UTIL_HudMessage(pPlayer ,hText, szText);
		pPlayer->m_iStart = 26;
	}

	if (pPlayer->m_iCommands == 25)
	{
		char szText[201];
		hudtextparms_t     hText;

		//(fragrunenumber.value > 0 || regenrunenumber.value > 0 || crowbarrunenumber.value > 0 || hasterunenumber.value > 0 || cloakrunenumber.value > 0 || hookrunenumber.value > 0 || protectrunenumber.value > 0 || lowgravrunenumber.value > 0 || vamprunenumber.value > 0 || traprunenumber.value > 0 || superglockrunenumber.value > 0)
		//grappledisable.value < 1
		//ctf.value

		// ctf, grapple, and runes
		if ((ctf.value) && (grappledisable.value < 1) && (fragrunenumber.value > 0 || regenrunenumber.value > 0 || crowbarrunenumber.value > 0 || hasterunenumber.value > 0 || cloakrunenumber.value > 0 || hookrunenumber.value > 0 || protectrunenumber.value > 0 || lowgravrunenumber.value > 0 || vamprunenumber.value > 0 || traprunenumber.value > 0 || superglockrunenumber.value > 0))
			sprintf(szText, "+hook - Fires the offhand grappling hook\ndroprune - Drops your current rune\nrunestatus - Shows your current rune\ndropflag - Drops the flag you are carrying\n");
		// ctf and runes
		else if ((ctf.value) && (grappledisable.value) && (fragrunenumber.value > 0 || regenrunenumber.value > 0 || crowbarrunenumber.value > 0 || hasterunenumber.value > 0 || cloakrunenumber.value > 0 || hookrunenumber.value > 0 || protectrunenumber.value > 0 || lowgravrunenumber.value > 0 || vamprunenumber.value > 0 || traprunenumber.value > 0 || superglockrunenumber.value > 0))
			sprintf(szText, "droprune - Drops your current rune\nrunestatus - Shows your current rune\ndropflag - Drops the flag you are carrying\n");
		// ctf and grapple
		else if ((ctf.value) && (grappledisable.value < 1))
			sprintf(szText, "+hook - Fires the offhand grappling hook\ndropflag - Drops the flag you are carrying\n");
		// grapple and runes
		else if ((ctf.value == 0) && (grappledisable.value < 1) && (fragrunenumber.value > 0 || regenrunenumber.value > 0 || crowbarrunenumber.value > 0 || hasterunenumber.value > 0 || cloakrunenumber.value > 0 || hookrunenumber.value > 0 || protectrunenumber.value > 0 || lowgravrunenumber.value > 0 || vamprunenumber.value > 0 || traprunenumber.value > 0 || superglockrunenumber.value > 0))
			sprintf(szText, "+hook - Fires the offhand grappling hook\ndroprune - Drops your current rune\nrunestatus - Shows your current rune\n");
		// grapple
		else if ((!ctf.value) && (grappledisable.value < 1))
			sprintf(szText, "+hook - Fires the offhand grappling hook\n");
		// ctf
		else if ((ctf.value) && (grappledisable.value))
			sprintf(szText, "dropflag - Drops the flag you are carrying\n");
		// runes
		else if ((!ctf.value) && (grappledisable.value) && (fragrunenumber.value > 0 || regenrunenumber.value > 0 || crowbarrunenumber.value > 0 || hasterunenumber.value > 0 || cloakrunenumber.value > 0 || hookrunenumber.value > 0 || protectrunenumber.value > 0 || lowgravrunenumber.value > 0 || vamprunenumber.value > 0 || traprunenumber.value > 0 || superglockrunenumber.value > 0))
			sprintf(szText, "droprune - Drops your current rune\nrunestatus - Shows your current rune\n");
		else
			sprintf(szText, "\n");

		memset(&hText, 0, sizeof(hText));
		hText.channel = 4;
		// These X and Y coordinates are just above
		//  the health meter.
		hText.x = 0.01;
		hText.y = 0.60;
	    
		hText.effect = 0;    // Fade in/out
		            
		hText.r1 = hText.g1 = hText.b1 = 255;
		hText.a1 = 255;
		
		hText.r2 = hText.g2 = hText.b2 = 255;
		hText.a2 = 255;
		
		hText.fadeinTime = 1;
		hText.fadeoutTime = 5;
		hText.holdTime = 60;
		hText.fxTime = 0.5;
		
		//UTIL_HudMessageAll(hText, szText);
		UTIL_HudMessage(pPlayer ,hText, szText);
		pPlayer->m_iCommands = 26;
	}
/*
	if (pPlayer->m_iGrapple == 25 && grappledisable.value < 1)
	{
		char szText[201];
		hudtextparms_t     hText;

		sprintf(szText, "+hook - Fires the offhand grappling hook\n");
		
		memset(&hText, 0, sizeof(hText));
		hText.channel = 3;
		// These X and Y coordinates are just above
		//  the health meter.
		hText.x = 0.01;
		hText.y = 0.73;
	    
		hText.effect = 0;    // Fade in/out
		            
		hText.r1 = hText.g1 = hText.b1 = 255;
		hText.a1 = 255;
		
		hText.r2 = hText.g2 = hText.b2 = 255;
		hText.a2 = 255;
		
		hText.fadeinTime = 1;
		hText.fadeoutTime = 5;
		hText.holdTime = 60;
		hText.fxTime = 0.5;
		
		//UTIL_HudMessageAll(hText, szText);
		UTIL_HudMessage(pPlayer ,hText, szText);
		pPlayer->m_iGrapple = 26;
	}
*/
	if (pPlayer->m_iOz == 25)
	{
		char szText[201];
		hudtextparms_t     hText;
	
		if (plr->ctf_team == 1 && ctf.value > 0)
			sprintf(szText, "Oz CTF (Fire & Ice)\nv1.7\nYou are on the Fire team (red)\n");
		else if (plr->ctf_team == 2 && ctf.value > 0)
			sprintf(szText, "Oz CTF (Fire & Ice)\nv1.7\nYou are on the Ice team (blue)\n");
		else if (CVAR_GET_FLOAT( "mp_teamplay"))
			sprintf(szText, "Oz Teamplay\nv1.7\n");
		else
			sprintf(szText, "Oz Deathmatch\nv1.7\n");
			
		memset(&hText, 0, sizeof(hText));
		hText.channel = 2;
		// These X and Y coordinates are just above
		//  the health meter.
		hText.x = 0.01;
		hText.y = 0.10;
	    
		hText.effect = 0;    // Fade in/out
		            
		hText.r1 = hText.g1 = hText.b1 = 255;
		hText.a1 = 255;
		
		hText.r2 = hText.g2 = hText.b2 = 255;
		hText.a2 = 255;
		
		hText.fadeinTime = 1;
		hText.fadeoutTime = 5;
		hText.holdTime = 7;
		hText.fxTime = 0.5;
		
		//UTIL_HudMessageAll(hText, szText);
		UTIL_HudMessage(pPlayer ,hText, szText);

		pPlayer->m_iOz = 26;
	}
	// Oz messaging end

	if (pPlayer)
		pPlayer->PostThink( );
}



void ParmsNewLevel( void )
{
}


void ParmsChangeLevel( void )
{
	// retrieve the pointer to the save data
	SAVERESTOREDATA *pSaveData = (SAVERESTOREDATA *)gpGlobals->pSaveData;

	if ( pSaveData )
		pSaveData->connectionCount = BuildChangeList( pSaveData->levelList, MAX_LEVEL_CONNECTIONS );
}


//
// GLOBALS ASSUMED SET:  g_ulFrameCount
//
void StartFrame( void )
{
	if ( g_pGameRules )
		g_pGameRules->Think();

	if ( g_fGameOver )
		return;

	gpGlobals->teamplay = CVAR_GET_FLOAT("teamplay");
	g_iSkillLevel = CVAR_GET_FLOAT("skill");
	g_ulFrameCount++;
}


void ClientPrecache( void )
{
	// setup precaches always needed
	PRECACHE_SOUND("player/sprayer.wav");			// spray paint sound for PreAlpha
	
	// PRECACHE_SOUND("player/pl_jumpland2.wav");		// UNDONE: play 2x step sound
	
	PRECACHE_SOUND("player/pl_fallpain2.wav");		
	PRECACHE_SOUND("player/pl_fallpain3.wav");		
	
	PRECACHE_SOUND("player/pl_step1.wav");		// walk on concrete
	PRECACHE_SOUND("player/pl_step2.wav");
	PRECACHE_SOUND("player/pl_step3.wav");
	PRECACHE_SOUND("player/pl_step4.wav");

	PRECACHE_SOUND("common/npc_step1.wav");		// NPC walk on concrete
	PRECACHE_SOUND("common/npc_step2.wav");
	PRECACHE_SOUND("common/npc_step3.wav");
	PRECACHE_SOUND("common/npc_step4.wav");

	PRECACHE_SOUND("player/pl_metal1.wav");		// walk on metal
	PRECACHE_SOUND("player/pl_metal2.wav");
	PRECACHE_SOUND("player/pl_metal3.wav");
	PRECACHE_SOUND("player/pl_metal4.wav");

	PRECACHE_SOUND("player/pl_dirt1.wav");		// walk on dirt
	PRECACHE_SOUND("player/pl_dirt2.wav");
	PRECACHE_SOUND("player/pl_dirt3.wav");
	PRECACHE_SOUND("player/pl_dirt4.wav");

	PRECACHE_SOUND("player/pl_duct1.wav");		// walk in duct
	PRECACHE_SOUND("player/pl_duct2.wav");
	PRECACHE_SOUND("player/pl_duct3.wav");
	PRECACHE_SOUND("player/pl_duct4.wav");

	PRECACHE_SOUND("player/pl_grate1.wav");		// walk on grate
	PRECACHE_SOUND("player/pl_grate2.wav");
	PRECACHE_SOUND("player/pl_grate3.wav");
	PRECACHE_SOUND("player/pl_grate4.wav");

	PRECACHE_SOUND("player/pl_slosh1.wav");		// walk in shallow water
	PRECACHE_SOUND("player/pl_slosh2.wav");
	PRECACHE_SOUND("player/pl_slosh3.wav");
	PRECACHE_SOUND("player/pl_slosh4.wav");

	PRECACHE_SOUND("player/pl_tile1.wav");		// walk on tile
	PRECACHE_SOUND("player/pl_tile2.wav");
	PRECACHE_SOUND("player/pl_tile3.wav");
	PRECACHE_SOUND("player/pl_tile4.wav");
	PRECACHE_SOUND("player/pl_tile5.wav");

	PRECACHE_SOUND("player/pl_swim1.wav");		// breathe bubbles
	PRECACHE_SOUND("player/pl_swim2.wav");
	PRECACHE_SOUND("player/pl_swim3.wav");
	PRECACHE_SOUND("player/pl_swim4.wav");

	PRECACHE_SOUND("player/pl_ladder1.wav");	// climb ladder rung
	PRECACHE_SOUND("player/pl_ladder2.wav");
	PRECACHE_SOUND("player/pl_ladder3.wav");
	PRECACHE_SOUND("player/pl_ladder4.wav");

	PRECACHE_SOUND("player/pl_wade1.wav");		// wade in water
	PRECACHE_SOUND("player/pl_wade2.wav");
	PRECACHE_SOUND("player/pl_wade3.wav");
	PRECACHE_SOUND("player/pl_wade4.wav");

	PRECACHE_SOUND("debris/wood1.wav");			// hit wood texture
	PRECACHE_SOUND("debris/wood2.wav");
	PRECACHE_SOUND("debris/wood3.wav");

	PRECACHE_SOUND("plats/train_use1.wav");		// use a train

	PRECACHE_SOUND("buttons/spark5.wav");		// hit computer texture
	PRECACHE_SOUND("buttons/spark6.wav");
	PRECACHE_SOUND("debris/glass1.wav");
	PRECACHE_SOUND("debris/glass2.wav");
	PRECACHE_SOUND("debris/glass3.wav");

	PRECACHE_SOUND( SOUND_FLASHLIGHT_ON );
	PRECACHE_SOUND( SOUND_FLASHLIGHT_OFF );

// player gib sounds
	PRECACHE_SOUND("common/bodysplat.wav");		               

// player pain sounds
	PRECACHE_SOUND("player/pl_pain2.wav");
	PRECACHE_SOUND("player/pl_pain4.wav");
	PRECACHE_SOUND("player/pl_pain5.wav");
	PRECACHE_SOUND("player/pl_pain6.wav");
	PRECACHE_SOUND("player/pl_pain7.wav");

	PRECACHE_MODEL("models/player.mdl");

	// hud sounds

	PRECACHE_SOUND("common/wpn_hudoff.wav");
	PRECACHE_SOUND("common/wpn_hudon.wav");
	PRECACHE_SOUND("common/wpn_moveselect.wav");
	PRECACHE_SOUND("common/wpn_select.wav");
	PRECACHE_SOUND("common/wpn_denyselect.wav");


	// geiger sounds

	PRECACHE_SOUND("player/geiger6.wav");
	PRECACHE_SOUND("player/geiger5.wav");
	PRECACHE_SOUND("player/geiger4.wav");
	PRECACHE_SOUND("player/geiger3.wav");
	PRECACHE_SOUND("player/geiger2.wav");
	PRECACHE_SOUND("player/geiger1.wav");

	if (giPrecacheGrunt)
		UTIL_PrecacheOther("monster_human_grunt");
}

/*
===============
const char *GetGameDescription()

Returns the descriptive name of this .dll.  E.g., Half-Life, or Team Fortress 2
===============
*/
const char *GetGameDescription()
{
	if ( g_pGameRules ) // this function may be called before the world has spawned, and the game rules initialized
		return g_pGameRules->GetGameDescription();
	else
		return "Oz Deathmatch";
}

/*
================
PlayerCustomization

A new player customization has been registered on the server
UNDONE:  This only sets the # of frames of the spray can logo
animation right now.
================
*/
void PlayerCustomization( edict_t *pEntity, customization_t *pCust )
{
	entvars_t *pev = &pEntity->v;
	CBasePlayer *pPlayer = (CBasePlayer *)GET_PRIVATE(pEntity);

	if (!pPlayer)
	{
		ALERT(at_console, "PlayerCustomization:  Couldn't get player!\n");
		return;
	}

	if (!pCust)
	{
		ALERT(at_console, "PlayerCustomization:  NULL customization!\n");
		return;
	}

	switch (pCust->resource.type)
	{
	case t_decal:
		pPlayer->SetCustomDecalFrames(pCust->nUserData2); // Second int is max # of frames.
		break;
	case t_sound:
	case t_skin:
	case t_model:
		// Ignore for now.
		break;
	default:
		ALERT(at_console, "PlayerCustomization:  Unknown customization type!\n");
		break;
	}
}

/*
================
SpectatorConnect

A spectator has joined the game
================
*/
void SpectatorConnect( edict_t *pEntity )
{
	entvars_t *pev = &pEntity->v;
	CBaseSpectator *pPlayer = (CBaseSpectator *)GET_PRIVATE(pEntity);

	if (pPlayer)
		pPlayer->SpectatorConnect( );
}

/*
================
SpectatorConnect

A spectator has left the game
================
*/
void SpectatorDisconnect( edict_t *pEntity )
{
	entvars_t *pev = &pEntity->v;
	CBaseSpectator *pPlayer = (CBaseSpectator *)GET_PRIVATE(pEntity);

	if (pPlayer)
		pPlayer->SpectatorDisconnect( );
}

/*
================
SpectatorConnect

A spectator has sent a usercmd
================
*/
void SpectatorThink( edict_t *pEntity )
{
	entvars_t *pev = &pEntity->v;
	CBaseSpectator *pPlayer = (CBaseSpectator *)GET_PRIVATE(pEntity);

	if (pPlayer)
		pPlayer->SpectatorThink( );
}

//*****************************************************************
//jammer infinity spawn the item read from the file
//item spawn
void SpawnItem(int x, int y, int z,int angle,int type)
{
  int weapon , random;

 if(type == W_RANDOM)
   {
	 //for OZ only here, because oz diables weapons
	 if(AllWeaponsDisabled)//all weapons are disabled dooh!
		 return;

	 weapon = RANDOM_LONG(0,(NUM_WEAPONS - 1));//- 1 because there is not a Weapons[13] only 12, 0 - 12
	 
	 while(Weapons[weapon].IsDisabled)//weapon is diabled so grab another one
	 {
	   weapon = RANDOM_LONG(0,(NUM_WEAPONS - 1));//if a lot of weapons are disable this could
	 }											  //take a while to get one
	 
	 random = TRUE; //is random
	 //ok if we get a 0 from the random above we need the first weapon
	 //so add the lowest weapon number which is w_first to get
	 //200 which is the glock, etc......
	 weapon += W_FIRST;
   }
 else if(type == A_RANDOM)
   {
	 //oz does not disable ammo so no need to check anything out here
	 weapon = RANDOM_LONG(A_FIRST,A_LAST);
	 random = TRUE; //is random
   }
 else
 {
	 weapon = type;//no randoms so spawn regular weapon
	 random = FALSE;
 }

   //weapons
   if(weapon == W_RPG)
	Rpg(x,y,z,angle,random);
   else if(weapon == W_SHOTGUN)
	ShotGun(x,y,z,angle,random);
   else if(weapon == W_GLOCK)
	Glock(x,y,z,angle,random);
   else if(weapon == W_PYTHON)
	Python(x,y,z,angle,random);
   else if(weapon == W_MP5)
	Mp5(x,y,z,angle,random);
   else if(weapon == W_CROSSBOW)
	Crossbow(x,y,z,angle,random);
   else if(weapon == W_GAUSS)
	Gauss(x,y,z,angle,random);
   else if(weapon == W_HORNET)
	Hornet(x,y,z,angle,random);
  else if(weapon == W_EGON)
	Egon(x,y,z,angle,random);
  else if(weapon == W_SNARK)
	Squeak(x,y,z,angle,random);
  else if(weapon == W_TRIPMINE)
	Tripmine(x,y,z,angle,random);
  else if(weapon == W_HANDGRENADE)
	HandGrenade(x,y,z,angle,random);
  else if(weapon == W_SATCHEL)
	Satchel(x,y,z,angle,random);
  //ammo
  else if(weapon == A_RPG)
 	RpgAmmo(x,y,z,angle,random);
  else if(weapon == A_CROSSBOW)
	CrossbowAmmo(x,y,z,angle,random);
  else if(weapon == A_GAUSS)
	GaussAmmo(x,y,z,angle,random);
  else if(weapon == A_GLOCK)
	GlockAmmo(x,y,z,angle,random);
  else if(weapon == A_AR)
	MP5Ammoclip(x,y,z,angle,random);
  else if(weapon == A_ARGRENADE)
	MP5AmmoGrenade(x,y,z,angle,random);
  else if(weapon == A_PYTHON)
	PythonAmmo(x,y,z,angle,random);
  else if(weapon == A_SHOTGUN)
	ShotGunAmmo(x,y,z,angle,random);

	//items, random is not used in these, but still there
  else if(weapon == I_BATTERY)
	ItemBattery(x,y,z,angle,random);
  else if(weapon == I_LONGJUMP)
	ItemLongJump(x,y,z,angle,random);
  else if(weapon == I_MEDIKIT)
	HealthKit(x,y,z,angle,random);
	
	//new jammer dm player starts
	else if(weapon == I_PLAYERSTART)
	{
		SpawnPlayerStart(x,y,z,angle,random);
		NumPlayerStarts++;
	}

	//jammer ctf stuff
	if(ctf.value > 0)
	{
		if(weapon == CTF_REDFLAG)
			SpawnRedFlag(x,y,z,angle,random);
		else if(weapon == CTF_BLUEFLAG)
  			SpawnBlueFlag(x,y,z,angle,random);
		else if(weapon == CTF_BLUESPAWN)
		{
			BlueSpawnPoint(x,y,z,angle,random);
			BluePlayerStarts++;
		}
		else if(weapon == CTF_REDSPAWN)
		{
			RedSpawnPoint(x,y,z,angle,random);
			RedPlayerStarts++;
		}
	}
}


//call this one time when server changes levels
//from serveractivate, if one of these values is changed in the
//middle of the level things could get weird. example: if the server
//admin were to type in disableglock 1, but who would do that
void CheckDisabledWeapons(void)
{
 
//just here for reference below
/*#define W_GLOCK		200
#define W_PYTHON	201
#define W_SHOTGUN	202
#define W_MP5		203
#define W_CROSSBOW	204
#define W_RPG		205
#define W_GAUSS		206
#define W_EGON		207
#define W_HORNET	208
#define W_HANDGRENADE	209
#define W_TRIPMINE	210
#define W_SATCHEL	211
#define W_SNARK		212
*/

//these must in the same order as above for use with the spawnitem
int x = 0;

if(disableglock.value > 0)
{
   //this would work also
	//Weapons[W_GLOCK = W_FIRST].ISDisabled
   Weapons[0].IsDisabled = TRUE;
   x++;
}
if(disable357.value > 0)
{
   Weapons[1].IsDisabled = TRUE;
   x++;
}
if(disableshotgun.value > 0)
{
   Weapons[2].IsDisabled = TRUE;
   x++;
}
if(disablemp5.value > 0)
{
   Weapons[3].IsDisabled = TRUE;
   x++;
}
if(disablexbow.value > 0)
{
   Weapons[4].IsDisabled = TRUE;
   x++;
}
if(disablerpg.value > 0)
{
   Weapons[5].IsDisabled = TRUE;
   x++;
}
if(disablegauss.value > 0)
{
   Weapons[6].IsDisabled = TRUE;
   x++;
}
if(disableegon.value > 0)
{
   Weapons[7].IsDisabled = TRUE;
   x++;
}
if(disablehivehand.value > 0)
{
   Weapons[8].IsDisabled = TRUE;
   x++;
}
if(disablehgrenade.value > 0)
{
   Weapons[9].IsDisabled = TRUE;
   x++;
}
if(disabletripmine.value > 0)
{
   Weapons[10].IsDisabled = TRUE;
   x++;
}
if(disablesatchel.value > 0)
{
   Weapons[11].IsDisabled = TRUE;
   x++;
}
if(disablesnark.value > 0)
{
   Weapons[12].IsDisabled = TRUE;
   x++;
}


 if(x >= NUM_WEAPONS)
	 AllWeaponsDisabled = TRUE;
 else
	AllWeaponsDisabled = FALSE;
}
//jammer end