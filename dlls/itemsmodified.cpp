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

extern int gmsgItemPickup;

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
};

LINK_ENTITY_TO_CLASS( item_longjump, CItemLongJump );

// Oz runes - definitions *****************************************
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
	OZPrecache();
	OZRunes(255, 255, 255);		

	SetTouch(FragTouch);

	SetThink ( Move );
	pev->nextthink = gpGlobals->time + respawntimerune.value;

}


void CFrag::FragTouch( CBaseEntity *pOther )
{
	// if it's not a player, ignore
	if ( !pOther->IsPlayer() )
	{
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	// ignore touch if they are already carrying a rune

	if ( pPlayer->ctf_hasrune )
	{
		return;
	}

	pPlayer->ctf_hasfrag = true;
	pPlayer->ctf_hasrune = true;
	char szText[128];
	sprintf(szText, "Frag Rune - you get %i extra point(s) for each kill!\n", INT(fragruneamount.value));

	OZHud(pPlayer,szText);
}

void CFrag::Move( )
{
	OZMove("ctf_frag");
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
	OZPrecache( );

	OZRunes(0, 250, 0);		

	SetTouch(RegenTouch);

	SetThink ( Move );
	pev->nextthink = gpGlobals->time + respawntimerune.value;

}


void CRegen::RegenTouch( CBaseEntity *pOther )
{
	// if it's not a player, ignore
	if ( !pOther->IsPlayer() )
	{
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	// ignore touch if they are already carrying a rune

	if ( pPlayer->ctf_hasrune )
	{
		return;
	}

	pPlayer->ctf_hasregen = true;
	pPlayer->ctf_hasrune = true;

	OZHud(pPlayer, "Regeneration Rune - your health and armor will continually rise!\n");
}

void CRegen::Move( )
{
	OZMove("ctf_regen");
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
	OZPrecache( );
	OZRunes(255, 0, 0);		
	
	SetTouch(CbarTouch);

	SetThink ( Move );
	pev->nextthink = gpGlobals->time + respawntimerune.value;

}


void CCbar::CbarTouch( CBaseEntity *pOther )
{
	// if it's not a player, ignore
	if ( !pOther->IsPlayer() )
	{
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	// ignore touch if they are already carrying a rune

	if ( pPlayer->ctf_hasrune )
	{
		return;
	}

	pPlayer->ctf_hascrowbar = true;
	pPlayer->ctf_hasrune = true;

	OZHud(pPlayer, "Crowbar Rune - your crowbar has infinite damage, USE IT!\n");
}

void CCbar::Move( )
{
	OZMove("ctf_crowbar");
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
	OZPrecache( );
	
	OZRunes(0, 0, 255);		
	
	SetTouch(CycleTouch);

	SetThink ( Move );
	pev->nextthink = gpGlobals->time + respawntimerune.value;

}


void CCycle::CycleTouch( CBaseEntity *pOther )
{
	// if it's not a player, ignore
	if ( !pOther->IsPlayer() )
	{
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	// ignore touch if they are already carrying a rune

	if ( pPlayer->ctf_hasrune )
	{
		return;
	}

	pPlayer->ctf_hascycle = true;
	pPlayer->ctf_hasrune = true;
	
	OZHud(pPlayer, "Haste Rune - your weapons cycle twice as fast!\n");
}

void CCycle::Move( )
{
	OZMove("ctf_cycle");
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
	OZPrecache( );

	OZRunes(50, 250, 250);		
	
	SetTouch(CloakTouch);

	SetThink ( Move );
	pev->nextthink = gpGlobals->time + respawntimerune.value;

}


void CCloak::CloakTouch( CBaseEntity *pOther )
{
	// if it's not a player, ignore
	if ( !pOther->IsPlayer() )
	{
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	// ignore touch if they are already carrying a rune

	if ( pPlayer->ctf_hasrune )
	{
		return;
	}

	pPlayer->ctf_hascloak = true;
	pPlayer->ctf_hasrune = true;

	// turn on the invisibility effect
	pPlayer->pev->rendermode = kRenderTransTexture;

	// set the transparency
	pPlayer->pev->renderamt = cloakruneamount.value;


	OZHud(pPlayer, "Cloaking Rune - you are partially invisible!\n");
}

void CCloak::Move( )
{
	OZMove("ctf_cloak");
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
	OZPrecache( );

	OZRunes(255, 135, 50);		

	SetTouch(HookTouch);

	SetThink ( Move );
	pev->nextthink = gpGlobals->time + respawntimerune.value;

}


void CHook::HookTouch( CBaseEntity *pOther )
{
	// if it's not a player, ignore
	if ( !pOther->IsPlayer() )
	{
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	// ignore touch if they are already carrying a rune

	if ( pPlayer->ctf_hasrune )
	{
		return;
	}

	pPlayer->ctf_hashook = true;
	pPlayer->ctf_hasrune = true;

	OZHud(pPlayer, "Super Hook Rune - your grapple flys faster, pulls faster, and does damage!\n");
}

void CHook::Move( )
{
	OZMove("ctf_hook");
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
	OZPrecache( );
	OZRunes(250, 100, 250);		

	SetTouch(ProtectTouch);

	SetThink ( Move );
	pev->nextthink = gpGlobals->time + respawntimerune.value;

}


void CProtect::ProtectTouch( CBaseEntity *pOther )
{
	// if it's not a player, ignore
	if ( !pOther->IsPlayer() )
	{
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	// ignore touch if they are already carrying a rune

	if ( pPlayer->ctf_hasrune )
	{
		return;
	}

	pPlayer->ctf_hasprotect = true;
	pPlayer->ctf_hasrune = true;

	OZHud(pPlayer, "Protection Rune - you recieve half damage!\n");
}

void CProtect::Move( )
{
	OZMove("ctf_protect");
	SetThink (Move);
}

class CPierce : public CItem
{
public:
	void Spawn ( void );
	void EXPORT PierceTouch( CBaseEntity *pOther );
	void Move ( void );
};

LINK_ENTITY_TO_CLASS( ctf_pierce, CPierce );

void CPierce::Spawn( )
{
	OZPrecache( );

	OZRunes(250, 250, 50);		

	SetTouch(PierceTouch);

	SetThink ( Move );
	pev->nextthink = gpGlobals->time + respawntimerune.value;

}


void CPierce::PierceTouch( CBaseEntity *pOther )
{
	// if it's not a player, ignore
	if ( !pOther->IsPlayer() )
	{
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	// ignore touch if they are already carrying a rune

	if ( pPlayer->ctf_hasrune )
	{
		return;
	}

	pPlayer->ctf_haspierce = true;
	pPlayer->ctf_hasrune = true;

	pPlayer->pev->gravity = 1;
	pPlayer->pev->gravity *= lowgravruneamount.value;

	OZHud(pPlayer, "Low Gravity Rune - you jump higher and recieve less falling damage!\n");
}

void CPierce::Move( )
{
	OZMove("ctf_pierce");
	SetThink (Move);
}

//jam: oz //////////////////////////////////////////////////////////
class CExplode : public CItem
{
public:
	void Spawn ( void );
	void EXPORT ExplodeTouch( CBaseEntity *pOther );
	void Move ( void );
};

LINK_ENTITY_TO_CLASS( ctf_explode, CExplode );

void CExplode::Spawn( )
{
	OZPrecache();
	OZRunes(255,0,255);

	SetTouch(ExplodeTouch);

	SetThink ( Move );
	pev->nextthink = gpGlobals->time + respawntimerune.value;

}


void CExplode::ExplodeTouch( CBaseEntity *pOther )
{
	// if it's not a player, ignore
	if ( !pOther->IsPlayer() )
	{
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	// ignore touch if they are already carrying a rune

	if ( pPlayer->ctf_hasrune )
	{
		return;
	}

	pPlayer->ctf_hasexplode = true;
	pPlayer->ctf_hasrune = true;

	OZHud(pPlayer,"Explode Rune -  Someone kills u and u blow up!\n");
}

void CExplode::Move( )
{
	OZMove("ctf_explode");
	SetThink (Move);
}

//jam
class CVampire : public CItem
{
public:
	void Spawn ( void );
	void EXPORT VampireTouch( CBaseEntity *pOther );
	void Move ( void );
};

LINK_ENTITY_TO_CLASS( ctf_vampire, CVampire );

void CVampire::Spawn( )
{
	OZPrecache();
	OZRunes(150,0,50);
	
	SetTouch(VampireTouch);

	SetThink ( Move );
	pev->nextthink = gpGlobals->time + respawntimerune.value;

}

void CVampire::VampireTouch( CBaseEntity *pOther )
{
	// if it's not a player, ignore
	if ( !pOther->IsPlayer() )
	{
		return;
	}

	CBasePlayer *pPlayer = (CBasePlayer *)pOther;

	// ignore touch if they are already carrying a rune

	if ( pPlayer->ctf_hasrune )
	{
		return;
	}

	pPlayer->ctf_hasvampire = true;
	pPlayer->ctf_hasrune = true;

	OZHud(pPlayer,"Vampire Rune -  You get half health of damage you inflict!\n");
}

void CVampire::Move( )
{
	OZMove("ctf_vampire");
	SetThink (Move);
}

/*****************
OZ Functions

********************/

void CItem :: OZPrecache(void)
{
	PRECACHE_MODEL( "models/w_suit.mdl" );
	PRECACHE_SOUND( "items/gunpickup2.wav" );
}


void CItem :: OZHud(CBasePlayer *pPlayer,char szText[201])
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

	UTIL_Remove( this );
}


void CItem :: OZRunes(int r, int b ,int g)
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



void CItem :: OZMove(char szText[128])
{
	CBaseEntity *pSpot;
	CBaseEntity *pEntity = NULL;

	UTIL_Remove( this );

	pSpot = NULL;

		for ( int i = RANDOM_LONG(1,5); i > 0; i-- )
			pSpot = UTIL_FindEntityByClassname( pSpot, "info_player_deathmatch" );


		CBaseEntity *pFirstSpot = pSpot;

		do 
		{
			if ( pSpot )
			{

					if ( pSpot->pev->origin == Vector( 0, 0, 0 ) )
					{
						pSpot = UTIL_FindEntityByClassname( pSpot, "info_player_deathmatch" );
						continue;
					}
			}
		
			pSpot = UTIL_FindEntityByClassname( pSpot, "info_player_deathmatch" );
		} while ( pSpot != pFirstSpot );


		pEntity = CBaseEntity::Create( szText, pSpot->pev->origin, pev->angles, edict() );
		pEntity->pev->angles.x = 0;
		pEntity->pev->angles.z = 0;
		pEntity->pev->nextthink = gpGlobals->time + respawntimerune.value;
		pEntity->pev->velocity.x = RANDOM_FLOAT( -300, 300 );
		pEntity->pev->velocity.y = RANDOM_FLOAT( -300, 300 );
		pEntity->pev->velocity.z = RANDOM_FLOAT( 0, 300 );
		pEntity->pev->movetype = MOVETYPE_TOSS;
}