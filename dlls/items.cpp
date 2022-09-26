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

===== items.cpp ========================================================

  functions governing the selection/use of weapons for players

*/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "weapons.h"
#include "player.h"
#include "skill.h"
#include "items.h"
#include "gamerules.h"
#include "game.h"

//jammer infinity
extern BOOL FoundFile;
extern int NumPlayerStarts;
//jammer ctf
extern int BluePlayerStarts;
extern int RedPlayerStarts;

//jammer blue flag
extern bool gBlueFirstFlagSpawn;
extern Vector gBlueFlagStartPoint;
extern Vector gBlueFlagStartAngle;
extern bool gBlueFlagDroppedInTransit;
extern entvars_t *gBlueFlag;
extern bool gBlueFlagAtHome;
//jammer red flag
extern bool gRedFirstFlagSpawn;
extern Vector gRedFlagStartPoint;
extern Vector gRedFlagStartAngle;
extern bool gRedFlagDroppedInTransit;
extern entvars_t *gRedFlag;
extern bool gRedFlagAtHome;
extern void CtfMsgToAll(char *sztext);

extern int gmsgItemPickup;
#define BLUETEAM 2
#define REDTEAM 1



// Oz CTF
extern DLL_GLOBAL BOOL		g_ctf_blueflagathome;
extern DLL_GLOBAL BOOL		g_ctf_redflagathome;
extern int gmsgScoreInfo;

class CWorldItem : public CBaseEntity
{
public:
	void	KeyValue(KeyValueData *pkvd ); 
	void	Spawn( void );
	int		m_iType;
};

LINK_ENTITY_TO_CLASS(world_items, CWorldItem);

void CWorldItem::KeyValue(KeyValueData *pkvd)
{
	if (FStrEq(pkvd->szKeyName, "type"))
	{
		m_iType = atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else
		CBaseEntity::KeyValue( pkvd );
}

void CWorldItem::Spawn( void )
{
	CBaseEntity *pEntity = NULL;

	switch (m_iType) 
	{
	case 44: // ITEM_BATTERY:
		pEntity = CBaseEntity::Create( "item_battery", pev->origin, pev->angles );
		break;
	case 42: // ITEM_ANTIDOTE:
		pEntity = CBaseEntity::Create( "item_antidote", pev->origin, pev->angles );
		break;
	case 43: // ITEM_SECURITY:
		pEntity = CBaseEntity::Create( "item_security", pev->origin, pev->angles );
		break;
	case 45: // ITEM_SUIT:
		pEntity = CBaseEntity::Create( "item_suit", pev->origin, pev->angles );
		break;
	}

	if (!pEntity)
	{
		ALERT( at_console, "unable to create world_item %d\n", m_iType );
	}
	else
	{
		pEntity->pev->target = pev->target;
		pEntity->pev->targetname = pev->targetname;
		pEntity->pev->spawnflags = pev->spawnflags;
	}

	REMOVE_ENTITY(edict());
}


void CItem::Spawn( void )
{
	//jammer infinity
	if(FoundFile)
		pev->flags=FL_KILLME;
	
	pev->movetype = MOVETYPE_TOSS;
	pev->solid = SOLID_TRIGGER;

	// Oz random spawn
	if (randomspawn.value > 0)
	{
		pev->velocity.x = RANDOM_FLOAT( -500, 500 );
		pev->velocity.y = RANDOM_FLOAT( -500, 500 );
		pev->velocity.z = RANDOM_FLOAT( 0, 500 );
	}

	UTIL_SetOrigin( pev, pev->origin );
	UTIL_SetSize(pev, Vector(-16, -16, 0), Vector(16, 16, 16));
	SetTouch(ItemTouch);

	if (DROP_TO_FLOOR(ENT(pev)) == 0)
	{
		ALERT(at_error, "Item %s fell out of level at %f,%f,%f", STRING( pev->classname ), pev->origin.x, pev->origin.y, pev->origin.z);
		UTIL_Remove( this );
		return;
	}
}

extern int gEvilImpulse101;

void CItem::ItemTouch( CBaseEntity *pOther )
{
	// if it's not a player, ignore
	if ( !pOther->IsPlayer() )
	{
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	// ok, a player is touching this item, but can he have it?
	if ( !g_pGameRules->CanHaveItem( pPlayer, this ) )
	{
		// no? Ignore the touch.
		return;
	}

	if (MyTouch( pPlayer ))
	{
		SUB_UseTargets( pOther, USE_TOGGLE, 0 );
		SetTouch( NULL );
		
		// player grabbed the item. 
		g_pGameRules->PlayerGotItem( pPlayer, this );
		if ( g_pGameRules->ItemShouldRespawn( this ) == GR_ITEM_RESPAWN_YES )
		{
			Respawn(); 
		}
		else
		{
			UTIL_Remove( this );
		}
	}
	else if (gEvilImpulse101)
	{
		UTIL_Remove( this );
	}
}

CBaseEntity* CItem::Respawn( void )
{
	SetTouch( NULL );
	pev->effects |= EF_NODRAW;

	// Oz random spawn
	if (randomspawn.value > 0)
	{
		pev->velocity.x = RANDOM_FLOAT( -500, 500 );
		pev->velocity.y = RANDOM_FLOAT( -500, 500 );
		pev->velocity.z = RANDOM_FLOAT( 0, 500 );
	}

	UTIL_SetOrigin( pev, g_pGameRules->VecItemRespawnSpot( this ) );// blip to whereever you should respawn.

	SetThink ( Materialize );
	pev->nextthink = g_pGameRules->FlItemRespawnTime( this ); 
	return this;
}

void CItem::Materialize( void )
{
	if ( pev->effects & EF_NODRAW )
	{
		// changing from invisible state to visible.
		EMIT_SOUND_DYN( ENT(pev), CHAN_WEAPON, "items/suitchargeok1.wav", 1, ATTN_NORM, 0, 150 );
		pev->effects &= ~EF_NODRAW;
		pev->effects |= EF_MUZZLEFLASH;
	}

	SetTouch( ItemTouch );
}

#define SF_SUIT_SHORTLOGON		0x0001

class CItemSuit : public CItem
{
	void Spawn( void )
	{ 
		Precache( );
		SET_MODEL(ENT(pev), "models/w_suit.mdl");
		CItem::Spawn( );
	}
	void Precache( void )
	{
		PRECACHE_MODEL ("models/w_suit.mdl");
	}
	BOOL MyTouch( CBasePlayer *pPlayer )
	{
		if ( pPlayer->pev->weapons & (1<<WEAPON_SUIT) )
			return FALSE;

		if ( pev->spawnflags & SF_SUIT_SHORTLOGON )
			EMIT_SOUND_SUIT(pPlayer->edict(), "!HEV_A0");		// short version of suit logon,
		else
			EMIT_SOUND_SUIT(pPlayer->edict(), "!HEV_AAx");	// long version of suit logon

		pPlayer->pev->weapons |= (1<<WEAPON_SUIT);
		return TRUE;
	}
};

LINK_ENTITY_TO_CLASS(item_suit, CItemSuit);



class CItemBattery : public CItem
{
	void Spawn( void )
	{ 
		Precache( );
		SET_MODEL(ENT(pev), "models/w_battery.mdl");
		CItem::Spawn( );
	}
	void Precache( void )
	{
		PRECACHE_MODEL ("models/w_battery.mdl");
		PRECACHE_SOUND( "items/gunpickup2.wav" );
	}
	BOOL MyTouch( CBasePlayer *pPlayer )
	{
		int maxbattery;
		// Oz variables (.value)
		maxbattery = maxcharge.value;

		if ((pPlayer->pev->armorvalue < maxbattery) &&
			(pPlayer->pev->weapons & (1<<WEAPON_SUIT)))
		{
			int pct;
			char szcharge[64];

			// Oz variables (.value)
			pPlayer->pev->armorvalue += battery.value;
			pPlayer->pev->armorvalue = min(pPlayer->pev->armorvalue, maxbattery);

			EMIT_SOUND( pPlayer->edict(), CHAN_ITEM, "items/gunpickup2.wav", 1, ATTN_NORM );

			MESSAGE_BEGIN( MSG_ONE, gmsgItemPickup, NULL, pPlayer->pev );
				WRITE_STRING( STRING(pev->classname) );
			MESSAGE_END();

			
			// Suit reports new power level
			// For some reason this wasn't working in release build -- round it.
			pct = (int)( (float)(pPlayer->pev->armorvalue * 100.0) * (1.0/maxbattery) + 0.5);
			pct = (pct / 5);
			if (pct > 0)
				pct--;
		
			sprintf( szcharge,"!HEV_%1dP", pct );
			
			//EMIT_SOUND_SUIT(ENT(pev), szcharge);
			pPlayer->SetSuitUpdate(szcharge, FALSE, SUIT_NEXT_IN_30SEC);
			return TRUE;		
		}
		return FALSE;
	}

//jammer infinity
public:
	void JamItemBattery(int,int,int,int,BOOL);
};

LINK_ENTITY_TO_CLASS(item_battery, CItemBattery);


class CItemAntidote : public CItem
{
	void Spawn( void )
	{ 
		Precache( );
		SET_MODEL(ENT(pev), "models/w_antidote.mdl");
		CItem::Spawn( );
	}
	void Precache( void )
	{
		PRECACHE_MODEL ("models/w_antidote.mdl");
	}
	BOOL MyTouch( CBasePlayer *pPlayer )
	{
		pPlayer->SetSuitUpdate("!HEV_DET4", FALSE, SUIT_NEXT_IN_1MIN);
		
		pPlayer->m_rgItems[ITEM_ANTIDOTE] += 1;
		return TRUE;
	}
};

LINK_ENTITY_TO_CLASS(item_antidote, CItemAntidote);


class CItemSecurity : public CItem
{
	void Spawn( void )
	{ 
		Precache( );
		SET_MODEL(ENT(pev), "models/w_security.mdl");
		CItem::Spawn( );
	}
	void Precache( void )
	{
		PRECACHE_MODEL ("models/w_security.mdl");
	}
	BOOL MyTouch( CBasePlayer *pPlayer )
	{
		pPlayer->m_rgItems[ITEM_SECURITY] += 1;
		return TRUE;
	}
};

LINK_ENTITY_TO_CLASS(item_security, CItemSecurity);

class CItemLongJump : public CItem
{
	void Spawn( void )
	{ 
		Precache( );
		SET_MODEL(ENT(pev), "models/w_longjump.mdl");
		CItem::Spawn( );
	}
	void Precache( void )
	{
		PRECACHE_MODEL ("models/w_longjump.mdl");
	}
	BOOL MyTouch( CBasePlayer *pPlayer )
	{
		if ( pPlayer->m_fLongJump )
		{
			return FALSE;
		}

		if ( ( pPlayer->pev->weapons & (1<<WEAPON_SUIT) ) )
		{
			pPlayer->m_fLongJump = TRUE;// player now has longjump module

			MESSAGE_BEGIN( MSG_ONE, gmsgItemPickup, NULL, pPlayer->pev );
				WRITE_STRING( STRING(pev->classname) );
			MESSAGE_END();

			EMIT_SOUND_SUIT( pPlayer->edict(), "!HEV_A1" );	// Play the longjump sound UNDONE: Kelly? correct sound?
			return TRUE;		
		}
		return FALSE;
	}

//jammer infinity
public:
	void JamItemLongJump(int,int,int,int,BOOL);
};

LINK_ENTITY_TO_CLASS( item_longjump, CItemLongJump );


//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Oz runes - definitions
class CFrag : public CItem
{
public:
	void Spawn ( void );
	void EXPORT FragTouch( CBaseEntity *pOther );
	void Move ( void );
};

LINK_ENTITY_TO_CLASS( ctf_frag, CFrag );

void CFrag::Spawn( )
{
	RunePrecache();
	RuneColor(255, 255, 255);		

	SetTouch(FragTouch);

	SetThink ( Move );
	pev->nextthink = gpGlobals->time + respawntimerune.value;
}

void CFrag::FragTouch( CBaseEntity *pOther )
{
	if ( !pOther->IsPlayer() )
	{
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	if ( pPlayer->ctf_hasrune )
	{
		return;
	}

	pPlayer->ctf_hasfrag = true;
	pPlayer->ctf_hasrune = true;

	EMIT_SOUND( pPlayer->edict(), CHAN_ITEM, "items/gunpickup2.wav", 1, ATTN_NORM );	

	char szText[201];

	sprintf(szText, "Frag Rune - you get %i extra point(s) for each kill!\n", (int)( fragruneamount.value ) );
		
	RuneHud(pPlayer,szText);

	UTIL_Remove( this );
}

void CFrag::Move( )
{
	RuneMove("ctf_frag");
	SetThink (Move);
}

class CRegen : public CItem
{
public:
	void Spawn ( void );
	void EXPORT RegenTouch( CBaseEntity *pOther );
	void Move ( void );
};

LINK_ENTITY_TO_CLASS( ctf_regen, CRegen );

void CRegen::Spawn( )
{
	RunePrecache();
	RuneColor(0, 250, 0);		

	SetTouch(RegenTouch);

	SetThink ( Move );
	pev->nextthink = gpGlobals->time + respawntimerune.value;

}

void CRegen::RegenTouch( CBaseEntity *pOther )
{
	if ( !pOther->IsPlayer() )
	{
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	if ( pPlayer->ctf_hasrune )
	{
		return;
	}

	pPlayer->ctf_hasregen = true;
	pPlayer->ctf_hasrune = true;

	EMIT_SOUND( pPlayer->edict(), CHAN_ITEM, "items/gunpickup2.wav", 1, ATTN_NORM );	

	char szText[201];

	sprintf(szText, "Regeneration Rune - your health and armor will continually rise!\n");
		
	RuneHud(pPlayer,szText);

	UTIL_Remove( this );
}

void CRegen::Move( )
{
	RuneMove("ctf_regen");
	SetThink (Move);
}

class CCbar : public CItem
{
public:
	void Spawn ( void );
	void EXPORT CbarTouch( CBaseEntity *pOther );
	void Move ( void );
};

LINK_ENTITY_TO_CLASS( ctf_crowbar, CCbar );

void CCbar::Spawn( )
{
	RunePrecache();
	RuneColor(250, 0, 0);		

	SetTouch(CbarTouch);

	SetThink ( Move );
	pev->nextthink = gpGlobals->time + respawntimerune.value;
}

void CCbar::CbarTouch( CBaseEntity *pOther )
{
	if ( !pOther->IsPlayer() )
	{
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	if ( pPlayer->ctf_hasrune )
	{
		return;
	}

	pPlayer->ctf_hascrowbar = true;
	pPlayer->ctf_hasrune = true;

	EMIT_SOUND( pPlayer->edict(), CHAN_ITEM, "items/gunpickup2.wav", 1, ATTN_NORM );	

	char szText[201];

	sprintf(szText, "Crowbar Rune - your crowbar has infinite damage, USE IT!\n");
		
	RuneHud(pPlayer,szText);

	UTIL_Remove( this );
}

void CCbar::Move( )
{
	RuneMove("ctf_crowbar");
	SetThink (Move);
}

class CCycle : public CItem
{
public:
	void Spawn ( void );
	void EXPORT CycleTouch( CBaseEntity *pOther );
	void Move ( void );
};

LINK_ENTITY_TO_CLASS( ctf_cycle, CCycle );

void CCycle::Spawn( )
{
	RunePrecache();
	RuneColor(0, 0, 250);		

	SetTouch(CycleTouch);

	SetThink ( Move );
	pev->nextthink = gpGlobals->time + respawntimerune.value;
}

void CCycle::CycleTouch( CBaseEntity *pOther )
{
	if ( !pOther->IsPlayer() )
	{
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	if ( pPlayer->ctf_hasrune )
	{
		return;
	}

	pPlayer->ctf_hascycle = true;
	pPlayer->ctf_hasrune = true;

	EMIT_SOUND( pPlayer->edict(), CHAN_ITEM, "items/gunpickup2.wav", 1, ATTN_NORM );	

	char szText[201];

	sprintf(szText, "Haste Rune - your weapons cycle twice as fast!\n");
		
	RuneHud(pPlayer,szText);

	UTIL_Remove( this );
}

void CCycle::Move( )
{
	RuneMove("ctf_cycle");
	SetThink (Move);
}

class CCloak : public CItem
{
public:
	void Spawn ( void );
	void EXPORT CloakTouch( CBaseEntity *pOther );
	void Move ( void );
};

LINK_ENTITY_TO_CLASS( ctf_cloak, CCloak );

void CCloak::Spawn( )
{
	RunePrecache();
	RuneColor(0, 250, 250);		

	SetTouch(CloakTouch);

	SetThink ( Move );
	pev->nextthink = gpGlobals->time + respawntimerune.value;
}

void CCloak::CloakTouch( CBaseEntity *pOther )
{
	if ( !pOther->IsPlayer() )
	{
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	if ( pPlayer->ctf_hasrune )
	{
		return;
	}

	pPlayer->ctf_hascloak = true;
	pPlayer->ctf_hasrune = true;

	pPlayer->pev->rendermode = kRenderTransTexture;

	pPlayer->pev->renderamt = 10;

	pPlayer->pev->renderfx |= kRenderFxGlowShell;
	pPlayer->pev->rendercolor = Vector(100,100,100);
	pPlayer->pev->renderamt = 10;

	EMIT_SOUND( pPlayer->edict(), CHAN_ITEM, "items/gunpickup2.wav", 1, ATTN_NORM );	

	char szText[201];

	sprintf(szText, "Cloaking Rune - you are partially invisible!\n");
		
	RuneHud(pPlayer,szText);

	UTIL_Remove( this );
}

void CCloak::Move( )
{
	RuneMove("ctf_cloak");
	SetThink (Move);
}

class CHook : public CItem
{
public:
	void Spawn ( void );
	void EXPORT HookTouch( CBaseEntity *pOther );
	void Move ( void );
};

LINK_ENTITY_TO_CLASS( ctf_hook, CHook );

void CHook::Spawn( )
{
	RunePrecache();
	RuneColor(250, 175, 100);		

	SetTouch(HookTouch);

	SetThink ( Move );
	pev->nextthink = gpGlobals->time + respawntimerune.value;
}

void CHook::HookTouch( CBaseEntity *pOther )
{
	if ( !pOther->IsPlayer() )
	{
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	if ( pPlayer->ctf_hasrune )
	{
		return;
	}

	pPlayer->ctf_hashook = true;
	pPlayer->ctf_hasrune = true;

	EMIT_SOUND( pPlayer->edict(), CHAN_ITEM, "items/gunpickup2.wav", 1, ATTN_NORM );	

	char szText[201];

	sprintf(szText, "Super Hook Rune - your grapple flys faster, pulls faster, and does damage!\n");
		
	RuneHud(pPlayer,szText);

	UTIL_Remove( this );
}

void CHook::Move( )
{
	RuneMove("ctf_hook");
	SetThink (Move);
}

class CProtect : public CItem
{
public:
	void Spawn ( void );
	void EXPORT ProtectTouch( CBaseEntity *pOther );
	void Move ( void );
};

LINK_ENTITY_TO_CLASS( ctf_protect, CProtect );

void CProtect::Spawn( )
{
	RunePrecache();
	RuneColor(150, 250, 150);		

	SetTouch(ProtectTouch);

	SetThink ( Move );
	pev->nextthink = gpGlobals->time + respawntimerune.value;
}

void CProtect::ProtectTouch( CBaseEntity *pOther )
{
	if ( !pOther->IsPlayer() )
	{
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	if ( pPlayer->ctf_hasrune )
	{
		return;
	}

	pPlayer->ctf_hasprotect = true;
	pPlayer->ctf_hasrune = true;

	EMIT_SOUND( pPlayer->edict(), CHAN_ITEM, "items/gunpickup2.wav", 1, ATTN_NORM );	

	char szText[201];

	sprintf(szText, "Protection Rune - you recieve half damage!\n");
		
	RuneHud(pPlayer,szText);

	UTIL_Remove( this );
}

void CProtect::Move( )
{
	RuneMove("ctf_protect");
	SetThink (Move);
}

class CLowgrav : public CItem
{
public:
	void Spawn ( void );
	void EXPORT LowgravTouch( CBaseEntity *pOther );
	void Move ( void );
};

LINK_ENTITY_TO_CLASS( ctf_lowgrav, CLowgrav );

void CLowgrav::Spawn( )
{
	RunePrecache();
	RuneColor(250, 250, 0);		

	SetTouch(LowgravTouch);

	SetThink ( Move );
	pev->nextthink = gpGlobals->time + respawntimerune.value;
}

void CLowgrav::LowgravTouch( CBaseEntity *pOther )
{
	if ( !pOther->IsPlayer() )
	{
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	if ( pPlayer->ctf_hasrune )
	{
		return;
	}

	pPlayer->ctf_haslowgrav = true;
	pPlayer->ctf_hasrune = true;

	pPlayer->pev->gravity = 1;
	pPlayer->pev->gravity *= lowgravruneamount.value;

	EMIT_SOUND( pPlayer->edict(), CHAN_ITEM, "items/gunpickup2.wav", 1, ATTN_NORM );	

	char szText[201];

	sprintf(szText, "Low Gravity Rune - you jump higher and recieve less falling damage!\n");
		
	RuneHud(pPlayer,szText);

	UTIL_Remove( this );
}

void CLowgrav::Move( )
{
	RuneMove("ctf_lowgrav");
	SetThink (Move);
}


class CVamp : public CItem
{
public:
	void Spawn ( void );
	void EXPORT VampTouch( CBaseEntity *pOther );
	void Move ( void );
};

LINK_ENTITY_TO_CLASS( ctf_vamp, CVamp );

void CVamp::Spawn( )
{
	RunePrecache();
	RuneColor(250, 0, 250);		

	SetTouch(VampTouch);

	SetThink ( Move );
	pev->nextthink = gpGlobals->time + respawntimerune.value;
}

void CVamp::VampTouch( CBaseEntity *pOther )
{
	if ( !pOther->IsPlayer() )
	{
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	if ( pPlayer->ctf_hasrune )
	{
		return;
	}

	pPlayer->ctf_hasvamp = true;
	pPlayer->ctf_hasrune = true;

	EMIT_SOUND( pPlayer->edict(), CHAN_ITEM, "items/gunpickup2.wav", 1, ATTN_NORM );	

	char szText[201];

	sprintf(szText, "Vampire Rune - you gain 1 health for every 2 damage you deal!\n" );
		
	RuneHud(pPlayer,szText);

	UTIL_Remove( this );
}

void CVamp::Move( )
{
	RuneMove("ctf_vamp");
	SetThink (Move);
}

class CTrap : public CItem
{
public:
	void Spawn ( void );
	void EXPORT TrapTouch( CBaseEntity *pOther );
	void Move ( void );
};

LINK_ENTITY_TO_CLASS( ctf_trap, CTrap );

void CTrap::Spawn( )
{
	RunePrecache();
	RuneColor(250, 150, 150);		

	SetTouch(TrapTouch);

	SetThink ( Move );
	pev->nextthink = gpGlobals->time + respawntimerune.value;
}

void CTrap::TrapTouch( CBaseEntity *pOther )
{
	if ( !pOther->IsPlayer() )
	{
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	if ( pPlayer->ctf_hasrune )
	{
		return;
	}

	pPlayer->ctf_hastrap = true;
	pPlayer->ctf_hasrune = true;

	EMIT_SOUND( pPlayer->edict(), CHAN_ITEM, "items/gunpickup2.wav", 1, ATTN_NORM );	

	char szText[201];

	sprintf(szText, "Booby Trap Rune - you will explode if you are killed, taking others with you!\n" );
		
	RuneHud(pPlayer,szText);

	UTIL_Remove( this );
}

void CTrap::Move( )
{
	RuneMove("ctf_trap");
	SetThink (Move);
}


//jammer new glock rune
class CSuperGlock : public CItem
{
public:
	void Spawn ( void );
	void EXPORT SuperGlockTouch( CBaseEntity *pOther );
	void Move ( void );
};

LINK_ENTITY_TO_CLASS(ctf_superglock, CSuperGlock);

void CSuperGlock::Spawn( )
{
	RunePrecache();
	RuneColor(50, 25, 75);

	SetTouch(SuperGlockTouch);

	SetThink ( Move );
	pev->nextthink = gpGlobals->time + respawntimerune.value;
}

void CSuperGlock::SuperGlockTouch( CBaseEntity *pOther )
{
	if ( !pOther->IsPlayer() )
	{
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	if ( pPlayer->ctf_hasrune )
	{
		return;
	}

	pPlayer->ctf_hassuperglock = true;
	pPlayer->ctf_hasrune = true;
	pPlayer->GiveNamedItem( "weapon_9mmhandgun" );
	pPlayer->GiveNamedItem( "ammo_9mmclip" );

	EMIT_SOUND( pPlayer->edict(), CHAN_ITEM, "items/gunpickup2.wav", 1, ATTN_NORM );	

	char szText[201];

	sprintf(szText, "Super Glock Rune - Unlimited ammo and a deadly sniper mode!" );
		
	RuneHud(pPlayer,szText);

	UTIL_Remove( this );
}

void CSuperGlock::Move( )
{
	RuneMove("ctf_superglock");
	SetThink (Move);
}

//jammer new glock rune

// Oz Rune helper functions by Jam
void CItem :: RunePrecache(void)
{
	PRECACHE_MODEL( "models/w_suit.mdl" );
	PRECACHE_SOUND( "items/gunpickup2.wav" );
}

void CItem :: RuneHud(CBasePlayer *pPlayer,char szText[201])
{
	EMIT_SOUND( pPlayer->edict(), CHAN_ITEM, "items/gunpickup2.wav", 1, ATTN_NORM );	

	// tell them on screen they have it
	hudtextparms_t     hText;
		
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
	hText.fadeoutTime = 3;
	hText.holdTime = 5;
	hText.fxTime = 0.5;
	
	UTIL_HudMessage(pPlayer ,hText, szText);
}

void CItem :: RuneColor(int r, int b ,int g)
{	
	SET_MODEL( ENT(pev), "models/w_suit.mdl" );

	pev->renderfx |= kRenderFxGlowShell;
	pev->rendercolor = Vector(r,b,g);
	pev->renderamt = 30; //With the KRFGS this is distance from the model.	

	CBaseEntity *pSpot;
	CBaseEntity *pEntity = NULL;

	pev->movetype = MOVETYPE_TOSS;
	pev->solid = SOLID_TRIGGER;

	for ( int i = RANDOM_LONG(1,5); i > 0; i-- )
		pSpot = UTIL_FindEntityByClassname( NULL, "info_player_deathmatch" );

	UTIL_SetOrigin( pev, pev->origin );
	UTIL_SetSize(pev, Vector(-16, -16, 0), Vector(16, 16, 16));
}

void CItem :: RuneMove(char szText[128])
{
	CBaseEntity *pSpot;
	CBaseEntity *pEntity = NULL;
	//jammer ctf, z
	int z;

	UTIL_Remove( this );

	pSpot = NULL;

	//jammer ctf
	//this is rune spawning stuff
	//if ctf is enabled use ctf and dm spawn points
	//if no ctf then just use dm spawn points
	if(ctf.value && RedPlayerStarts != 0 && BluePlayerStarts != 0)//ctf so get ctf spawns because may not be dm spawns
	{
	 if(NumPlayerStarts == 0)//no dm points in level, just ctf team points
	    z = RANDOM_LONG(1,2);
	 else
		z = RANDOM_LONG(1,3);//we found some dm even in ctf so use them

	 if(z == 1)//1 so red
	 {
		int x = RANDOM_LONG(1,RedPlayerStarts);
	   for(int i = 0; i < x; i++)
		  pSpot = UTIL_FindEntityByClassname(pSpot, "ctf_redspawn");
	 }
	 else if(z == 2)//not 1 so blue
	 {
		int x = RANDOM_LONG(1,BluePlayerStarts);
	   for(int i = 0; i < x; i++)
		  pSpot = UTIL_FindEntityByClassname(pSpot, "ctf_bluespawn");
	 }
	 else if(z == 3)//dm spawn spots
	 {
		int x = RANDOM_LONG(1,NumPlayerStarts);
	   for(int i = 0; i < x; i++)
		  pSpot = UTIL_FindEntityByClassname(pSpot, "info_player_deathmatch");
	 }
	}
	else//ctf mode in not invoked so use dm spawns only
	{
	   int x = RANDOM_LONG(1,NumPlayerStarts);
	  for(int i = 0; i < x; i++)
		  pSpot = UTIL_FindEntityByClassname( pSpot, "info_player_deathmatch" );
	}
	//end jammer ctf

	pEntity = CBaseEntity::Create( szText, pSpot->pev->origin, pev->angles, edict() );
	pEntity->pev->angles.x = 0;
	pEntity->pev->angles.z = 0;
	pEntity->pev->nextthink = gpGlobals->time + respawntimerune.value;
	pEntity->pev->velocity.x = RANDOM_FLOAT( -300, 300 );
	pEntity->pev->velocity.y = RANDOM_FLOAT( -300, 300 );
	pEntity->pev->velocity.z = RANDOM_FLOAT( 0, 300 );
	pEntity->pev->movetype = MOVETYPE_TOSS;
}

//jammer infinity, NO RANDOMING OF THESE ITEMS OR HEALTHKIT
void CItemBattery::JamItemBattery(int x, int y ,int z, int a, BOOL random)
{
  Vector pos(x,y,z);
  Vector angle(0,a,0);    
  CBaseEntity *pItemBattery = CBaseEntity::Create("item_battery",pos,angle,ENT(pev));
}

void ItemBattery(int x, int y, int z, int a, BOOL random)
{
  CItemBattery *pItemBattery = GetClassPtr((CItemBattery *)NULL);
  pItemBattery->JamItemBattery(x,y,z,a,random);
}

//jammer infinity
void CItemLongJump::JamItemLongJump(int x, int y ,int z, int a, BOOL random)
{
  Vector pos(x,y,z);
  Vector angle(0,a,0);    
  CBaseEntity *pItemLongJump = CBaseEntity::Create("item_longjump",pos,angle,ENT(pev));
}

void ItemLongJump(int x, int y, int z, int a, BOOL random)
{
  CItemLongJump *pItemLongJump = GetClassPtr((CItemLongJump *)NULL);
  pItemLongJump->JamItemLongJump(x,y,z,a,random);
}


//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------

// Oz CTF - flags while at their bases
/*
class CRedFlagHome : public CItem
{
public:
	void Spawn ( void );
	void EXPORT RedTouch( CBaseEntity *pOther );
	void RedThink( void );
	CBaseEntity*	RRespawn( void );

	//jammer ctf stuff for infinity
	void JamRedFlagHome(int x, int y ,int z, int a, BOOL random);
};

LINK_ENTITY_TO_CLASS( ctf_redflag, CRedFlagHome );

void CRedFlagHome::Spawn( )
{
	if (!ctf.value)
	{
		UTIL_Remove( this );
		return;
	}
	PRECACHE_MODEL( "models/redflag.mdl" );
	PRECACHE_SOUND( "ambience/siren.wav" );
	PRECACHE_SOUND( "ambience/particle_suck1.wav" );
	PRECACHE_SOUND( "holo/tr_holo_fantastic.wav" );
	PRECACHE_SOUND( "holo/tr_holo_nicejob.wav" );
	SET_MODEL(ENT(pev), "models/redflag.mdl");

	pev->movetype = MOVETYPE_TOSS;
	pev->solid = SOLID_TRIGGER;

	pev->renderfx |= kRenderFxGlowShell;
	pev->rendercolor = Vector(250,0,0);
	pev->renderamt = 30; //With the KRFGS this is distance from the model.	

	UTIL_SetOrigin( pev, pev->origin );
	UTIL_SetSize(pev, Vector(-16, -16, 0), Vector(16, 16, 16));

	SetTouch(RedTouch);
	SetThink(NULL);

	if (DROP_TO_FLOOR(ENT(pev)) == 0)
	{
		ALERT(at_error, "Item %s fell out of level at %f,%f,%f", STRING( pev->classname ), pev->origin.x, pev->origin.y, pev->origin.z);
		UTIL_Remove( this );
		return;
	}
}

void CRedFlagHome::RedTouch( CBaseEntity *pOther )
{
	CBasePlayer *pPlayer = (CBasePlayer *)pOther;
	entvars_t *pevPlayer = pPlayer->pev;

	if ( !pOther->IsPlayer() )
		return;

	if ( pOther->ctf_team == 1)
	{
		if ( pPlayer->ctf_hasblueflag)
		{
			// the player has captured the other team's flag!
			pPlayer->ctf_hasblueflag = false;
			g_ctf_blueflagathome = false;

			if (!pPlayer->ctf_hascloak)
				pPlayer->pev->renderfx &= ~kRenderFxGlowShell;
			else
			{
				pPlayer->pev->rendercolor = Vector(100,100,100);
				pPlayer->pev->renderamt = 10;
			}
		
			// Oz CTF - Scoring
			pevPlayer->frags += ctfflagscore.value;// 5 frags for capping it!
			pOther->captures++;
			
			MESSAGE_BEGIN( MSG_ALL, gmsgScoreInfo );
				WRITE_BYTE( ENTINDEX(pPlayer->edict()) );
				WRITE_SHORT( pPlayer->pev->frags );
				WRITE_SHORT( pPlayer->m_iDeaths );
			MESSAGE_END();

			switch ( RANDOM_LONG( 0, 1 ) )
			{
			case 0:
				EMIT_SOUND( ENT(pev), CHAN_VOICE, "holo/tr_holo_fantastic.wav", 1, ATTN_NORM);
				EMIT_SOUND( ENT(pev), CHAN_ITEM, "ambience/particle_suck1.wav", 1, ATTN_NORM );	
				break;
			case 1:
				EMIT_SOUND( ENT(pev), CHAN_VOICE, "holo/tr_holo_nicejob.wav", 1, ATTN_NORM);
				EMIT_SOUND( ENT(pev), CHAN_ITEM, "ambience/particle_suck1.wav", 1, ATTN_NORM );	
				break;
			}
			
			char szText[201];
			hudtextparms_t     hText;
			sprintf(szText, "%s has captured the Ice flag!\n", STRING(pPlayer->pev->netname));
			memset(&hText, 0, sizeof(hText));
			hText.channel = 1;
			// These X and Y coordinates are just above
			//  the health meter.
			hText.x = 0.01;
			hText.y = 0.05;
			hText.effect = 0;    // Fade in/out
			hText.r1 = hText.g1 = hText.b1 = 255;
			hText.a1 = 255;
			hText.r2 = hText.g2 = hText.b2 = 255;
			hText.a2 = 255;
			hText.fadeinTime = 0;
			hText.fadeoutTime = 0;
			hText.holdTime = 3;
			hText.fxTime = 0.5;
				
			UTIL_HudMessageAll(hText, szText);
			return;
		}
		return;
	}

	pPlayer->ctf_hasredflag = true;
	g_ctf_redflagathome = true;
	pPlayer->pev->renderfx |= kRenderFxGlowShell;
	pPlayer->pev->rendercolor = Vector(250,0,0);
	pPlayer->pev->renderamt = 30;

	// Oz CTF - Scoring
	pevPlayer->frags += ctfstealscore.value;// one frag for taking the flag from the BASE

	MESSAGE_BEGIN( MSG_ALL, gmsgScoreInfo );
		WRITE_BYTE( ENTINDEX(pPlayer->edict()) );
		WRITE_SHORT( pPlayer->pev->frags );
		WRITE_SHORT( pPlayer->m_iDeaths );
	MESSAGE_END();

	
	EMIT_SOUND( ENT(pev), CHAN_AUTO, "ambience/siren.wav", 1, ATTN_NORM );	

	char szText[201];
	hudtextparms_t     hText;

	sprintf(szText, "%s has taken the Fire flag!\n", STRING(pPlayer->pev->netname));
			
	memset(&hText, 0, sizeof(hText));
	hText.channel = 1;
	// These X and Y coordinates are just above
	//  the health meter.
	hText.x = 0.01;
	hText.y = 0.05;
	    
	hText.effect = 0;    // Fade in/out
		            
	hText.r1 = hText.g1 = hText.b1 = 255;
	hText.a1 = 255;
		
	hText.r2 = hText.g2 = hText.b2 = 255;
	hText.a2 = 255;
		
	hText.fadeinTime = 0;
	hText.fadeoutTime = 0;
	hText.holdTime = 3;
	hText.fxTime = 0.5;
		
	UTIL_HudMessageAll(hText, szText);

	RRespawn();
}

void CRedFlagHome::RedThink( )
{
	if (g_ctf_redflagathome == false)
	{
		g_ctf_redflagathome = true;

		STOP_SOUND( ENT(pev), CHAN_AUTO, "ambience/siren.wav" );
		EMIT_SOUND( ENT(pev), CHAN_VOICE, "ambience/particle_suck1.wav", 1, ATTN_NORM );	
	
		pev->effects &= ~EF_NODRAW;
		pev->effects |= EF_MUZZLEFLASH;
		SetTouch( RedTouch );
		SetThink( NULL );
	}
	pev->nextthink = gpGlobals->time + 1; 
}

CBaseEntity* CRedFlagHome::RRespawn( void )
{
	SetTouch( NULL );
	pev->effects |= EF_NODRAW;

	UTIL_SetOrigin( pev, g_pGameRules->VecItemRespawnSpot( this ) );// blip to whereever you should respawn.

	SetThink ( RedThink );
	pev->nextthink = gpGlobals->time + 1; 
	return this;
}

*/
/**********************************
red flag stuff
***********************************/
//jammer ctf flag code
class CRedFlag : public CItem
{
public:
	void Spawn ( void );
	void EXPORT RedTouch( CBaseEntity *pOther );
	void RedThink( void );
	void RedSoundThink(void);

	//jammer ctf stuff for infinity
	void JamRedFlag(int x, int y ,int z, int a, BOOL random);

};

LINK_ENTITY_TO_CLASS(ctf_redflag, CRedFlag);

/*************************************************/
void CRedFlag::Spawn( )
{
	if (!ctf.value)
	{
		UTIL_Remove( this );
		return;
	}
	PRECACHE_MODEL( "models/redflag.mdl" );
	PRECACHE_SOUND( "ambience/siren.wav" );
	PRECACHE_SOUND( "ambience/particle_suck1.wav" );
	PRECACHE_SOUND( "holo/tr_holo_fantastic.wav" );
	PRECACHE_SOUND( "holo/tr_holo_nicejob.wav" );
	//PRECACHE_SOUND( "items/gunpickup2.wav" );
	PRECACHE_SOUND("items/ammopickup2.wav");

	SET_MODEL(ENT(pev), "models/redflag.mdl");

	pev->movetype = MOVETYPE_TOSS;
	pev->solid = SOLID_TRIGGER;

	pev->renderfx |= kRenderFxGlowShell;
	pev->rendercolor = Vector(250,0,0);
	pev->renderamt = 30; //With the KRFGS this is distance from the model.	

	UTIL_SetOrigin( pev, pev->origin );
	UTIL_SetSize(pev, Vector(-16, -16, 0), Vector(16, 16, 16));

	SetTouch(RedTouch);
	
	
	//player has dropped the flag and this spawn is on the floor
	//and not at home, so think
	if(gRedFlagDroppedInTransit)
	{
	  pev->nextthink = gpGlobals->time + 30;
	  SetThink(RedThink);
	}
	else
	   SetThink(NULL);

	if (DROP_TO_FLOOR(ENT(pev)) == 0)
	{
		ALERT(at_error, "Item %s fell out of level at %f,%f,%f", STRING( pev->classname ), pev->origin.x, pev->origin.y, pev->origin.z);
		UTIL_Remove( this );
		return;
	}

	//these are the points where the flags starts out when level starts
	//only get these the very first time we start a level, home spawning points
	if(gRedFirstFlagSpawn)
	{
	  gRedFlagStartPoint = pev->origin;
	  gRedFlagStartAngle = pev->angles;
	  gRedFirstFlagSpawn = false;
	}

	//this is global refernce to flag so i can kill it when i need to
	//like when the player dies with flag kill the one he is holding
	//create a new one where he is standing, well was standing
	gRedFlag = pev;
}

/*************************************************/
void CRedFlag::RedTouch( CBaseEntity *pOther )
{
	CBasePlayer *pPlayer = (CBasePlayer *)pOther;
	entvars_t *pevPlayer = pPlayer->pev;
	if ( !pOther->IsPlayer() )
		return;

	//blue team grabbed flag so follow player
   if(pOther->ctf_team == BLUETEAM)
   {
	 pPlayer->ctf_hasredflag = true;

	 pev->movetype = MOVETYPE_FOLLOW;
	 pev->solid = SOLID_NOT;
	 pev->aiment = pOther->edict();
	 //pev->effects = EF_NODRAW;
	 pev->owner = pOther->edict();
	 pev->sequence = 2;
	 
	 //set to false
	 gRedFlagDroppedInTransit = false;
	 SetTouch( NULL );

	 // Oz CTF - Scoring
	 pevPlayer->frags += ctfstealscore.value;

	 MESSAGE_BEGIN( MSG_ALL, gmsgScoreInfo );
		WRITE_BYTE( ENTINDEX(pPlayer->edict()) );
		WRITE_SHORT( pPlayer->pev->frags );
		WRITE_SHORT( pPlayer->m_iDeaths );
	 MESSAGE_END();
	
	 char szText[128];
	 sprintf(szText, "%s has taken the Fire flag!\n", STRING(pPlayer->pev->netname));
	 CtfMsgToAll(szText);
	 
	 if(gRedFlagAtHome)//if at home play siren
	 {
	   EMIT_SOUND( ENT(pev), CHAN_AUTO, "ambience/siren.wav", 1, ATTN_NORM );	
	   pev->nextthink = gpGlobals->time + 15;
	   SetThink(RedSoundThink);
	   gRedFlagAtHome = false;
	 }
	 else
	 {
	     EMIT_SOUND(pPlayer->edict(),CHAN_ITEM,"items/ammopickup2.wav", 1, ATTN_NORM );	
		 SetThink(NULL);
	 }

   }
   
   //flag was dropped and then touched by a red guy so return to home
   else if(pOther->ctf_team == REDTEAM && gRedFlagDroppedInTransit)
   {
	 gRedFlag->flags = FL_KILLME;
	 //set to false because flag is now home and not in transit
	 gRedFlagDroppedInTransit = false;
	 gRedFlagAtHome = true;

	 CBaseEntity *pEntity = CBaseEntity::Create( "ctf_redflag", 
			              gRedFlagStartPoint,gRedFlagStartAngle, edict() );
	 // Oz CTF - Scoring
	pevPlayer->frags += ctfreturnscore.value;//one frag for returning the flag

	MESSAGE_BEGIN( MSG_ALL, gmsgScoreInfo );
		WRITE_BYTE( ENTINDEX(pPlayer->edict()) );
		WRITE_SHORT( pPlayer->pev->frags );
		WRITE_SHORT( pPlayer->m_iDeaths );
	MESSAGE_END();

	char szText[128];
	sprintf(szText, "%s has returned the Fire flag!\n", STRING(pPlayer->pev->netname));
	CtfMsgToAll(szText);
   }
   else if(pOther->ctf_team == REDTEAM && pPlayer->ctf_hasblueflag)
   {
	 //red player has blue flag so kill blue flag and send it home
	   //and score for red player
	 pPlayer->ctf_hasblueflag = false;
		
	 //kill flag on back and get ready to spawn a new one at home
	 STOP_SOUND(ENT(gBlueFlag), CHAN_AUTO, "ambience/siren.wav" );
	 gBlueFlag->flags = FL_KILLME;

	// Oz CTF - Scoring
	pevPlayer->frags += ctfflagscore.value;//5 frags for capping it!
	pOther->captures++;

	MESSAGE_BEGIN( MSG_ALL, gmsgScoreInfo );
		WRITE_BYTE( ENTINDEX(pPlayer->edict()) );
		WRITE_SHORT( pPlayer->pev->frags );
		WRITE_SHORT( pPlayer->m_iDeaths );
	MESSAGE_END();
	

	switch ( RANDOM_LONG( 0, 1 ) )
	{
	case 0:
		EMIT_SOUND( ENT(pev), CHAN_VOICE, "holo/tr_holo_fantastic.wav", 1, ATTN_NORM);
		EMIT_SOUND( ENT(pev), CHAN_ITEM, "ambience/particle_suck1.wav", 1, ATTN_NORM );	
		break;
	case 1:
		EMIT_SOUND( ENT(pev), CHAN_VOICE, "holo/tr_holo_nicejob.wav", 1, ATTN_NORM);
		EMIT_SOUND( ENT(pev), CHAN_ITEM, "ambience/particle_suck1.wav", 1, ATTN_NORM );	
		break;
	}

	gBlueFlagAtHome = true;

	char szText[128];
	sprintf(szText, "%s has captured the Ice flag!\n", STRING(pPlayer->pev->netname));
	CtfMsgToAll(szText);

	//spawn new flag at home
	CBaseEntity *pEntity = CBaseEntity::Create( "ctf_blueflag", 
			              gBlueFlagStartPoint,gBlueFlagStartAngle, edict() );
   }
}

/*************************************************/
void CRedFlag::RedSoundThink(void)
{
  STOP_SOUND( ENT(pev), CHAN_AUTO, "ambience/siren.wav" );
  SetThink(NULL);
}

/*************************************************/
void CRedFlag::RedThink(void)
{
   //this is ONLY true when a player dies with flag
	//when a player dies with flag the one on his back is killed and a new one
	//is spawned. think is 30 seconds. if it is touched before 30 seconds
	//then the gredflagdroppedintransit will be set to false, see above
  if(gRedFlagDroppedInTransit)
   {
	 gRedFlag->flags = FL_KILLME;//call this before creating a new flag!!!!
	 gRedFlagDroppedInTransit = false;
	 gRedFlagAtHome = true;
	 CBaseEntity *pEntity = CBaseEntity::Create( "ctf_redflag", 
			              gRedFlagStartPoint,gRedFlagStartAngle, edict() );
   }
}

/***********************
blue flag stuff
**************************************/

class CBlueFlag : public CItem
{
public:
	void Spawn ( void );
	void EXPORT BlueTouch( CBaseEntity *pOther );
	void BlueThink( void );
	void BlueSoundThink(void);

	//jammer ctf stuff for infinity
	void JamBlueFlag(int x, int y ,int z, int a, BOOL random);

};

LINK_ENTITY_TO_CLASS(ctf_blueflag, CBlueFlag);

/*************************************************/
void CBlueFlag::Spawn( )
{
	if (!ctf.value)
	{
		UTIL_Remove( this );
		return;
	}
	PRECACHE_MODEL("models/blueflag.mdl");
	PRECACHE_SOUND("ambience/siren.wav");
	PRECACHE_SOUND("ambience/particle_suck1.wav");
	PRECACHE_SOUND("holo/tr_holo_fantastic.wav");
	PRECACHE_SOUND("holo/tr_holo_nicejob.wav");
	//PRECACHE_SOUND("items/gunpickup2.wav");
	PRECACHE_SOUND("items/ammopickup2.wav");

	SET_MODEL(ENT(pev), "models/blueflag.mdl");

	pev->movetype = MOVETYPE_TOSS;
	pev->solid = SOLID_TRIGGER;

	pev->renderfx |= kRenderFxGlowShell;
	pev->rendercolor = Vector(0,0,250);
	pev->renderamt = 30; //With the KRFGS this is distance from the model.	

	UTIL_SetOrigin( pev, pev->origin );
	UTIL_SetSize(pev, Vector(-16, -16, 0), Vector(16, 16, 16));

	SetTouch(BlueTouch);
	
	
	//player has dropped the flag and this spawn is on the floor
	//and not at home, so think
	if(gBlueFlagDroppedInTransit)
	{
	  pev->nextthink = gpGlobals->time + 30;
	  SetThink(BlueThink);
	}
	else
	   SetThink(NULL);

	if (DROP_TO_FLOOR(ENT(pev)) == 0)
	{
		ALERT(at_error, "Item %s fell out of level at %f,%f,%f", STRING( pev->classname ), pev->origin.x, pev->origin.y, pev->origin.z);
		UTIL_Remove( this );
		return;
	}

	//these are the points where the flags starts out when level starts
	//only get these the very first time we start a level, home spawning points
	if(gBlueFirstFlagSpawn)
	{
	  gBlueFlagStartPoint = pev->origin;
	  gBlueFlagStartAngle = pev->angles;
	  gBlueFirstFlagSpawn = false;
	}

	//this is global refernce to flag so i can kill it when i need to
	//like when the player dies with flag kill the one he is holding
	//create a new one where he is standing, well was standing
	gBlueFlag = pev;
}
/*************************************************/
void CBlueFlag::BlueTouch( CBaseEntity *pOther )
{
	CBasePlayer *pPlayer = (CBasePlayer *)pOther;
	entvars_t *pevPlayer = pPlayer->pev;
	if ( !pOther->IsPlayer() )
		return;

	//red team grabbed flag so follow player
   if(pOther->ctf_team == REDTEAM)
   {
	 pPlayer->ctf_hasblueflag = true;

	 pev->movetype = MOVETYPE_FOLLOW;
	 pev->solid = SOLID_NOT;
	 pev->aiment = pOther->edict();
	 //pev->effects = EF_NODRAW;
	 pev->owner = pOther->edict();
	 pev->sequence = 2;
	 
	 //set to false
	 gBlueFlagDroppedInTransit = false;
	 SetThink(NULL);
	 SetTouch( NULL );

	 // Oz CTF - Scoring
	 pevPlayer->frags += ctfstealscore.value;

		
	 MESSAGE_BEGIN( MSG_ALL, gmsgScoreInfo );
		WRITE_BYTE( ENTINDEX(pPlayer->edict()) );
		WRITE_SHORT( pPlayer->pev->frags );
		WRITE_SHORT( pPlayer->m_iDeaths );
	 MESSAGE_END();
	
	 char szText[128];
	 sprintf(szText, "%s has taken the Ice flag!\n", STRING(pPlayer->pev->netname));
	 CtfMsgToAll(szText);
	
	 if(gBlueFlagAtHome)//if at home play siren
	 {
	   EMIT_SOUND( ENT(pev), CHAN_AUTO, "ambience/siren.wav", 1, ATTN_NORM );
	   pev->nextthink = gpGlobals->time + 15;
	   SetThink(BlueSoundThink);
	   gBlueFlagAtHome = false;
	 }
	 else//not at home
	 {
	     EMIT_SOUND(pPlayer->edict(),CHAN_ITEM,"items/ammopickup2.wav", 1, ATTN_NORM );	
		 SetThink(NULL);
	 }

   }
   
   //flag was dropped and then touched by a blue guy so return to home
   else if(pOther->ctf_team == BLUETEAM && gBlueFlagDroppedInTransit)
   {
	 gBlueFlag->flags = FL_KILLME;
	 //set to false because flag is now home and not in transit
	 gBlueFlagDroppedInTransit = false;
	 gBlueFlagAtHome = true;

	 CBaseEntity *pEntity = CBaseEntity::Create( "ctf_blueflag", 
			              gBlueFlagStartPoint,gBlueFlagStartAngle, edict() );
	 // Oz CTF - Scoring
	pevPlayer->frags += ctfreturnscore.value;//one frag for returning the flag

	MESSAGE_BEGIN( MSG_ALL, gmsgScoreInfo );
		WRITE_BYTE( ENTINDEX(pPlayer->edict()) );
		WRITE_SHORT( pPlayer->pev->frags );
		WRITE_SHORT( pPlayer->m_iDeaths );
	MESSAGE_END();

	char szText[128];
	sprintf(szText, "%s has returned the Ice flag!\n", STRING(pPlayer->pev->netname));
	CtfMsgToAll(szText);
   }
   else if(pOther->ctf_team == BLUETEAM && pPlayer->ctf_hasredflag)
   {
	  //blue player has red flag so kill red flag and send it home
	   //and score for blue player
	   pPlayer->ctf_hasredflag = false;
	   
	   //kill flag on the back, gonna spawn a new one
	   STOP_SOUND(ENT(gRedFlag), CHAN_AUTO, "ambience/siren.wav" );
	   gRedFlag->flags = FL_KILLME;

		// Oz CTF - Scoring
		pevPlayer->frags += ctfflagscore.value;//5 frags for capping it!
		pOther->captures++;

		MESSAGE_BEGIN( MSG_ALL, gmsgScoreInfo );
			WRITE_BYTE( ENTINDEX(pPlayer->edict()) );
			WRITE_SHORT( pPlayer->pev->frags );
			WRITE_SHORT( pPlayer->m_iDeaths );
		MESSAGE_END();

		
		switch ( RANDOM_LONG( 0, 1 ) )
		{
		 case 0:
			EMIT_SOUND( ENT(pev), CHAN_VOICE, "holo/tr_holo_fantastic.wav", 1, ATTN_NORM);
			EMIT_SOUND( ENT(pev), CHAN_ITEM, "ambience/particle_suck1.wav", 1, ATTN_NORM );	
			break;
	 	 case 1:
			EMIT_SOUND( ENT(pev), CHAN_VOICE, "holo/tr_holo_nicejob.wav", 1, ATTN_NORM);
			EMIT_SOUND( ENT(pev), CHAN_ITEM, "ambience/particle_suck1.wav", 1, ATTN_NORM );	
			break;
		}

		gRedFlagAtHome = true;
		char szText[128];
		sprintf(szText, "%s has captured the Fire flag!\n", STRING(pPlayer->pev->netname));
		CtfMsgToAll(szText);
	 
		CBaseEntity *pEntity = CBaseEntity::Create( "ctf_redflag", 
			              gRedFlagStartPoint,gRedFlagStartAngle, edict() );
   }
}

/*************************************************/
void CBlueFlag::BlueSoundThink(void)
{
  STOP_SOUND(ENT(pev), CHAN_AUTO, "ambience/siren.wav" );
  SetThink(NULL);
}

/*************************************************/
void CBlueFlag::BlueThink(void)
{
   //this is ONLY true when a player dies with flag
	//when a player dies with flag the one on his back is killed and a new one
	//is spawned. think is 30 seconds. if it is touched before 30 seconds
	//then the gblueflagdroppedintransit will be set to false, see above
  if(gBlueFlagDroppedInTransit)
   {
	 gBlueFlag->flags = FL_KILLME;//call this before creating a new flag!!!!
	 gBlueFlagDroppedInTransit = false;
	 gBlueFlagAtHome = true;

	 CBaseEntity *pEntity = CBaseEntity::Create( "ctf_blueflag", 
			              gBlueFlagStartPoint,gBlueFlagStartAngle, edict() );
   }
}


/*
class CBlueFlagHome : public CItem
{
public:
	void Spawn ( void );
	void EXPORT BlueTouch( CBaseEntity *pOther );
	void BlueThink( void );
	CBaseEntity*	BRespawn( void );

	//jammer ctf stuff for infinity
	void JamBlueFlagHome(int x, int y ,int z, int a, BOOL random);

};

LINK_ENTITY_TO_CLASS( ctf_blueflag, CBlueFlagHome );

void CBlueFlagHome::Spawn( )
{
	if (!ctf.value)
	{
		UTIL_Remove( this );
		return;
	}
	PRECACHE_MODEL( "models/blueflag.mdl" );
	PRECACHE_SOUND( "ambience/siren.wav" );
	PRECACHE_SOUND( "ambience/particle_suck1.wav" );
	PRECACHE_SOUND( "holo/tr_holo_fantastic.wav" );
	PRECACHE_SOUND( "holo/tr_holo_nicejob.wav" );
	SET_MODEL(ENT(pev), "models/blueflag.mdl");

	pev->movetype = MOVETYPE_TOSS;
	pev->solid = SOLID_TRIGGER;

	pev->renderfx |= kRenderFxGlowShell;
	pev->rendercolor = Vector(0,0,250);
	pev->renderamt = 30; //With the KRFGS this is distance from the model.	

	UTIL_SetOrigin( pev, pev->origin );
	UTIL_SetSize(pev, Vector(-16, -16, 0), Vector(16, 16, 16));

	SetTouch(BlueTouch);
	SetThink(NULL);

	if (DROP_TO_FLOOR(ENT(pev)) == 0)
	{
		ALERT(at_error, "Item %s fell out of level at %f,%f,%f", STRING( pev->classname ), pev->origin.x, pev->origin.y, pev->origin.z);
		UTIL_Remove( this );
		return;
	}
}

void CBlueFlagHome::BlueTouch( CBaseEntity *pOther )
{
	CBasePlayer *pPlayer = (CBasePlayer *)pOther;
	entvars_t *pevPlayer = pPlayer->pev;
	if ( !pOther->IsPlayer() )
		return;


	if ( pOther->ctf_team == 2)
	{
		if ( pPlayer->ctf_hasredflag)
		{
			// the player has captured the other teams flag!
			pPlayer->ctf_hasredflag = false;
			g_ctf_redflagathome = false;

			if (!pPlayer->ctf_hascloak)
				pPlayer->pev->renderfx &= ~kRenderFxGlowShell;
			else
			{
				pPlayer->pev->rendercolor = Vector(100,100,100);
				pPlayer->pev->renderamt = 10;
			}

			// Oz CTF - Scoring
			pevPlayer->frags += ctfflagscore.value;//5 frags for capping it!
			pOther->captures++;

			MESSAGE_BEGIN( MSG_ALL, gmsgScoreInfo );
				WRITE_BYTE( ENTINDEX(pPlayer->edict()) );
				WRITE_SHORT( pPlayer->pev->frags );
				WRITE_SHORT( pPlayer->m_iDeaths );
			MESSAGE_END();

			switch ( RANDOM_LONG( 0, 1 ) )
			{
				EMIT_SOUND( ENT(pev), CHAN_VOICE, "holo/tr_holo_fantastic.wav", 1, ATTN_NORM);
				EMIT_SOUND( ENT(pev), CHAN_ITEM, "ambience/particle_suck1.wav", 1, ATTN_NORM );	
				break;
			case 1:
				EMIT_SOUND( ENT(pev), CHAN_VOICE, "holo/tr_holo_nicejob.wav", 1, ATTN_NORM);
				EMIT_SOUND( ENT(pev), CHAN_ITEM, "ambience/particle_suck1.wav", 1, ATTN_NORM );	
			}

			char szText[201];
			hudtextparms_t     hText;
			sprintf(szText, "%s has captured the Fire flag!\n", STRING(pPlayer->pev->netname));
			memset(&hText, 0, sizeof(hText));
			hText.channel = 1;
			// These X and Y coordinates are just above
			//  the health meter.
			hText.x = 0.01;
			hText.y = 0.05;
			hText.effect = 0;    // Fade in/out
			hText.r1 = hText.g1 = hText.b1 = 255;
			hText.a1 = 255;
			hText.r2 = hText.g2 = hText.b2 = 255;
			hText.a2 = 255;
			hText.fadeinTime = 0;
			hText.fadeoutTime = 0;
			hText.holdTime = 3;
			hText.fxTime = 0.5;
				
			UTIL_HudMessageAll(hText, szText);
			return;
		}
		return;
	}

	pPlayer->ctf_hasblueflag = true;
	g_ctf_blueflagathome = true;
	pPlayer->pev->renderfx |= kRenderFxGlowShell;
	pPlayer->pev->rendercolor = Vector(0,0,250);
	pPlayer->pev->renderamt = 30;

	// Oz CTF - Scoring
	pevPlayer->frags += ctfstealscore.value;// one frag for taking the flag from the BASE

	MESSAGE_BEGIN( MSG_ALL, gmsgScoreInfo );
		WRITE_BYTE( ENTINDEX(pPlayer->edict()) );
		WRITE_SHORT( pPlayer->pev->frags );
		WRITE_SHORT( pPlayer->m_iDeaths );
	MESSAGE_END();

	EMIT_SOUND( ENT(pev), CHAN_AUTO, "ambience/siren.wav", 1, ATTN_NORM );	

	char szText[201];
	hudtextparms_t     hText;

	sprintf(szText, "%s has taken the Ice flag!\n", STRING(pPlayer->pev->netname));
			
	memset(&hText, 0, sizeof(hText));
	hText.channel = 1;
	// These X and Y coordinates are just above
	//  the health meter.
	hText.x = 0.01;
	hText.y = 0.05;
	    
	hText.effect = 0;    // Fade in/out
		            
	hText.r1 = hText.g1 = hText.b1 = 255;
	hText.a1 = 255;
		
	hText.r2 = hText.g2 = hText.b2 = 255;
	hText.a2 = 255;
		
	hText.fadeinTime = 0;
	hText.fadeoutTime = 0;
	hText.holdTime = 3;
	hText.fxTime = 0.5;
		
	UTIL_HudMessageAll(hText, szText);
	
	BRespawn();
}


void CBlueFlagHome::BlueThink( )
{
	if (g_ctf_blueflagathome == false)
	{
		g_ctf_blueflagathome = true;
	
		STOP_SOUND( ENT(pev), CHAN_AUTO, "ambience/siren.wav" );
		EMIT_SOUND( ENT(pev), CHAN_VOICE, "ambience/particle_suck1.wav", 1, ATTN_NORM );	

		pev->effects &= ~EF_NODRAW;
		pev->effects |= EF_MUZZLEFLASH;

		
		SetTouch( BlueTouch );
		SetThink( NULL );
	}


	pev->nextthink = gpGlobals->time + 1; 
}

CBaseEntity* CBlueFlagHome::BRespawn( void )
{
	SetTouch( NULL );
	pev->effects |= EF_NODRAW;

	UTIL_SetOrigin( pev, g_pGameRules->VecItemRespawnSpot( this ) );// blip to whereever you should respawn.

	SetThink ( BlueThink );
	pev->nextthink = gpGlobals->time + 1; 
	return this;
}

//----------------------------------------------------------------------
// Oz CTF - flags while in transit

class CRedFlagInTransit : public CItem
{
public:
	void Spawn ( void );
	void EXPORT RedTouch( CBaseEntity *pOther );
	void RedThink( void );
};

LINK_ENTITY_TO_CLASS( ctf_redflag_transit, CRedFlagInTransit );

void CRedFlagInTransit::Spawn( )
{
	PRECACHE_MODEL( "models/redflag.mdl" );
	PRECACHE_SOUND( "items/gunpickup2.wav" );
	SET_MODEL(ENT(pev), "models/redflag.mdl");

	pev->movetype = MOVETYPE_TOSS;
	pev->solid = SOLID_TRIGGER;

	pev->renderfx |= kRenderFxGlowShell;
	pev->rendercolor = Vector(250,0,0);
	pev->renderamt = 30; //With the KRFGS this is distance from the model.	

	UTIL_SetOrigin( pev, pev->origin );
	UTIL_SetSize(pev, Vector(-16, -16, 0), Vector(16, 16, 16));

	SetTouch(RedTouch);
	SetThink(RedThink);
	pev->nextthink = gpGlobals->time + 30;

	if (DROP_TO_FLOOR(ENT(pev)) == 0)
	{
		ALERT(at_error, "Item %s fell out of level at %f,%f,%f", STRING( pev->classname ), pev->origin.x, pev->origin.y, pev->origin.z);
		UTIL_Remove( this );
		return;
	}
}

void CRedFlagInTransit::RedTouch( CBaseEntity *pOther )
{
	CBasePlayer *pPlayer = (CBasePlayer *)pOther;
	entvars_t *pevPlayer = pPlayer->pev;

	if ( !pOther->IsPlayer() )
		return;

	if ( pOther->ctf_team == 1)// on red team
	{
		// the player has his team's flag
		g_ctf_redflagathome = false;// send it home

		// Oz CTF - Scoring
		pevPlayer->frags += ctfreturnscore.value;// one frag for returning it

		MESSAGE_BEGIN( MSG_ALL, gmsgScoreInfo );
			WRITE_BYTE( ENTINDEX(pPlayer->edict()) );
			WRITE_SHORT( pPlayer->pev->frags );
			WRITE_SHORT( pPlayer->m_iDeaths );
		MESSAGE_END();

		char szText[201];
		hudtextparms_t     hText;
		sprintf(szText, "%s has returned the Fire flag!\n", STRING(pPlayer->pev->netname));
		memset(&hText, 0, sizeof(hText));
		hText.channel = 1;
		hText.x = 0.01;
		hText.y = 0.05;
		hText.effect = 0;    // Fade in/out
		hText.r1 = hText.g1 = hText.b1 = 255;
		hText.a1 = 255;
		hText.r2 = hText.g2 = hText.b2 = 255;
		hText.a2 = 255;
		hText.fadeinTime = 0;
		hText.fadeoutTime = 0;
		hText.holdTime = 3;
		hText.fxTime = 0.5;
				
		UTIL_HudMessageAll(hText, szText);
	}
	else
	{
		pPlayer->ctf_hasredflag = true;
		g_ctf_redflagathome = true;
		pPlayer->pev->renderfx |= kRenderFxGlowShell;
		pPlayer->pev->rendercolor = Vector(250,0,0);
		pPlayer->pev->renderamt = 30;

		EMIT_SOUND( pPlayer->edict(), CHAN_ITEM, "items/gunpickup2.wav", 1, ATTN_NORM );	

		char szText[201];
		hudtextparms_t     hText;

		sprintf(szText, "%s has taken the Fire flag!\n", STRING(pPlayer->pev->netname));
				
		memset(&hText, 0, sizeof(hText));
		hText.channel = 1;
		// These X and Y coordinates are just above
		//  the health meter.
		hText.x = 0.01;
		hText.y = 0.05;
			
		hText.effect = 0;    // Fade in/out
						
		hText.r1 = hText.g1 = hText.b1 = 255;
		hText.a1 = 255;
			
		hText.r2 = hText.g2 = hText.b2 = 255;
		hText.a2 = 255;
			
		hText.fadeinTime = 0;
		hText.fadeoutTime = 0;
		hText.holdTime = 3;
		hText.fxTime = 0.5;
			
		UTIL_HudMessageAll(hText, szText);
	}

	UTIL_Remove( this );
}

void CRedFlagInTransit::RedThink( )
{
	pev->nextthink = gpGlobals->time + 30; 
	SetThink(RedThink);
	g_ctf_redflagathome = false;
	UTIL_Remove( this );

	char szText[201];
	hudtextparms_t     hText;

	sprintf(szText, "The Fire flag has returned to base.\n");
				
	memset(&hText, 0, sizeof(hText));
	hText.channel = 1;
	hText.x = 0.01;
	hText.y = 0.05;
	hText.effect = 0;    // Fade in/out
	hText.r1 = hText.g1 = hText.b1 = 255;
	hText.a1 = 255;
	hText.r2 = hText.g2 = hText.b2 = 255;
	hText.a2 = 255;
	hText.fadeinTime = 0;
	hText.fadeoutTime = 0;
	hText.holdTime = 3;
	hText.fxTime = 0.5;
			
	UTIL_HudMessageAll(hText, szText);
}

class CBlueFlagInTransit : public CItem
{
public:
	void Spawn ( void );
	void EXPORT BlueTouch( CBaseEntity *pOther );
	void BlueThink( void );
};

LINK_ENTITY_TO_CLASS( ctf_blueflag_transit, CBlueFlagInTransit );

void CBlueFlagInTransit::Spawn( )
{
	PRECACHE_MODEL( "models/blueflag.mdl" );
	PRECACHE_SOUND( "items/gunpickup2.wav" );
	SET_MODEL(ENT(pev), "models/blueflag.mdl");

	pev->movetype = MOVETYPE_TOSS;
	pev->solid = SOLID_TRIGGER;

	pev->renderfx |= kRenderFxGlowShell;
	pev->rendercolor = Vector(0,0,250);
	pev->renderamt = 30; //With the KRFGS this is distance from the model.	

	UTIL_SetOrigin( pev, pev->origin );
	UTIL_SetSize(pev, Vector(-16, -16, 0), Vector(16, 16, 16));

	SetTouch(BlueTouch);
	SetThink(BlueThink);
	pev->nextthink = gpGlobals->time + 30;

	if (DROP_TO_FLOOR(ENT(pev)) == 0)
	{
		ALERT(at_error, "Item %s fell out of level at %f,%f,%f", STRING( pev->classname ), pev->origin.x, pev->origin.y, pev->origin.z);
		UTIL_Remove( this );
		return;
	}
}

void CBlueFlagInTransit::BlueTouch( CBaseEntity *pOther )
{
	CBasePlayer *pPlayer = (CBasePlayer *)pOther;
	entvars_t *pevPlayer = pPlayer->pev;

	if ( !pOther->IsPlayer() )
		return;

	if ( pOther->ctf_team == 2)// on blue team
	{
		// the player has his team's flag
		g_ctf_blueflagathome = false;// send it home

		// Oz CTF - Scoring
		pevPlayer->frags += ctfreturnscore.value;//one frag for returning the flag

		MESSAGE_BEGIN( MSG_ALL, gmsgScoreInfo );
			WRITE_BYTE( ENTINDEX(pPlayer->edict()) );
			WRITE_SHORT( pPlayer->pev->frags );
			WRITE_SHORT( pPlayer->m_iDeaths );
		MESSAGE_END();

		char szText[201];
		hudtextparms_t     hText;
		sprintf(szText, "%s has returned the Ice flag!\n", STRING(pPlayer->pev->netname));
		memset(&hText, 0, sizeof(hText));
		hText.channel = 1;
		hText.x = 0.01;
		hText.y = 0.05;
		hText.effect = 0;    // Fade in/out
		hText.r1 = hText.g1 = hText.b1 = 255;
		hText.a1 = 255;
		hText.r2 = hText.g2 = hText.b2 = 255;
		hText.a2 = 255;
		hText.fadeinTime = 0;
		hText.fadeoutTime = 0;
		hText.holdTime = 3;
		hText.fxTime = 0.5;
				
		UTIL_HudMessageAll(hText, szText);
	}
	else
	{
		pPlayer->ctf_hasblueflag = true;
		g_ctf_blueflagathome = true;
		pPlayer->pev->renderfx |= kRenderFxGlowShell;
		pPlayer->pev->rendercolor = Vector(0,0,250);
		pPlayer->pev->renderamt = 30;
		EMIT_SOUND( pPlayer->edict(), CHAN_ITEM, "items/gunpickup2.wav", 1, ATTN_NORM );	

		char szText[201];
		hudtextparms_t     hText;

		sprintf(szText, "%s has taken the Ice flag!\n", STRING(pPlayer->pev->netname));
				
		memset(&hText, 0, sizeof(hText));
		hText.channel = 1;
		// These X and Y coordinates are just above
		//  the health meter.
		hText.x = 0.01;
		hText.y = 0.05;
			
		hText.effect = 0;    // Fade in/out
						
		hText.r1 = hText.g1 = hText.b1 = 255;
		hText.a1 = 255;
			
		hText.r2 = hText.g2 = hText.b2 = 255;
		hText.a2 = 255;
			
		hText.fadeinTime = 0;
		hText.fadeoutTime = 0;
		hText.holdTime = 3;
		hText.fxTime = 0.5;
			
		UTIL_HudMessageAll(hText, szText);

	}

	UTIL_Remove( this );
}

void CBlueFlagInTransit::BlueThink( )
{
	pev->nextthink = gpGlobals->time + 30;
	SetThink(BlueThink);
	g_ctf_blueflagathome = false;
	UTIL_Remove( this );

	char szText[201];
	hudtextparms_t     hText;

	sprintf(szText, "The Ice flag has returned to base.\n");
				
	memset(&hText, 0, sizeof(hText));
	hText.channel = 1;
	hText.x = 0.01;
	hText.y = 0.05;
	hText.effect = 0;    // Fade in/out
	hText.r1 = hText.g1 = hText.b1 = 255;
	hText.a1 = 255;
	hText.r2 = hText.g2 = hText.b2 = 255;
	hText.a2 = 255;
	hText.fadeinTime = 0;
	hText.fadeoutTime = 0;
	hText.holdTime = 3;
	hText.fxTime = 0.5;
			
	UTIL_HudMessageAll(hText, szText);
}

*/

//jammer ctf stuff for infinity
void CRedFlag::JamRedFlag(int x, int y ,int z, int a, BOOL random)
{
  Vector pos(x,y,z);
  Vector angle(0,a,0);    
  CBaseEntity *pRedFlag = CBaseEntity::Create("ctf_redflag",pos,angle,ENT(pev));
}

void SpawnRedFlag(int x, int y ,int z, int a, BOOL random)
{
  CRedFlag *pRedFlag = GetClassPtr((CRedFlag *)NULL);
  pRedFlag->JamRedFlag(x,y,z,a,random);
}


//jammer ctf stuff for infinity
void CBlueFlag::JamBlueFlag(int x, int y ,int z, int a, BOOL random)
{
  Vector pos(x,y,z);
  Vector angle(0,a,0);    
  CBaseEntity *pBlueFlag = CBaseEntity::Create("ctf_blueflag",pos,angle,ENT(pev));

}

void SpawnBlueFlag(int x, int y ,int z, int a, BOOL random)
{
  CBlueFlag *pBlueFlag = GetClassPtr((CBlueFlag *)NULL);
  pBlueFlag->JamBlueFlag(x,y,z,a,random);
}

/*******************************************
ctf msg stuff, mostly for flag info
******************************************/
void CtfMsgToAll(char *sztext)
{
	hudtextparms_t     hText;
	memset(&hText, 0, sizeof(hText));
	hText.channel = 1;
	hText.x = 0.01;
	hText.y = 0.05;
	hText.effect = 0;    // Fade in/out
	hText.r1 = hText.g1 = hText.b1 = 255;
	hText.a1 = 255;
	hText.r2 = hText.g2 = hText.b2 = 255;
	hText.a2 = 255;
	hText.fadeinTime = 0;
	hText.fadeoutTime = 0;
	hText.holdTime = 3;
	hText.fxTime = 0.5;
	UTIL_HudMessageAll(hText, sztext);
}