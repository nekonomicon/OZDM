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

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "game.h"

//jammer
extern BOOL FoundFile;

enum glock_e {
	GLOCK_IDLE1 = 0,
	GLOCK_IDLE2,
	GLOCK_IDLE3,
	GLOCK_SHOOT,
	GLOCK_SHOOT_EMPTY,
	GLOCK_RELOAD,
	GLOCK_RELOAD_NOT_EMPTY,
	GLOCK_DRAW,
	GLOCK_HOLSTER,
	GLOCK_ADD_SILENCER
};

class CGlock : public CBasePlayerWeapon
{
public:
	void Spawn( void );
	void Precache( void );
	int iItemSlot( void ) { return 2; }
	int GetItemInfo(ItemInfo *p);

	void PrimaryAttack( void );
	void SecondaryAttack( void );
	void GlockFire( float flSpread, float flCycleTime, BOOL fUseAutoAim );
	BOOL Deploy( void );
	void Reload( void );
	void WeaponIdle( void );
	int m_iShell;

	//jammer
	void JamGlock(int,int,int,int,BOOL);
};
LINK_ENTITY_TO_CLASS( weapon_glock, CGlock );
LINK_ENTITY_TO_CLASS( weapon_9mmhandgun, CGlock );


void CGlock::Spawn( )
{
	//jammer
	if(FoundFile)
	  pev->flags=FL_KILLME;
	
	// Oz variables (.value)
	if (disableglock.value > 0)
	{
		Kill( );
		return;
	}

	pev->classname = MAKE_STRING("weapon_9mmhandgun"); // hack to allow for old names
	Precache( );
	m_iId = WEAPON_GLOCK;

	//jammer
	if(silenceglock.value > 0)
		SET_MODEL(ENT(pev), "models/w_silencer.mdl");
	else
		SET_MODEL(ENT(pev), "models/w_9mmhandgun.mdl");

	// Oz variables (.value)
	m_iDefaultAmmo = givewithglock.value;

	FallInit();// get ready to fall down.
}


void CGlock::Precache( void )
{
	PRECACHE_MODEL("models/v_9mmhandgun.mdl");
	PRECACHE_MODEL("models/w_9mmhandgun.mdl");
	PRECACHE_MODEL("models/p_9mmhandgun.mdl");
	PRECACHE_MODEL("models/w_silencer.mdl");

	m_iShell = PRECACHE_MODEL ("models/shell.mdl");// brass shell

	PRECACHE_SOUND("items/9mmclip1.wav");
	PRECACHE_SOUND("items/9mmclip2.wav");

	PRECACHE_SOUND ("weapons/pl_gun1.wav");//silenced handgun
	PRECACHE_SOUND ("weapons/pl_gun2.wav");//silenced handgun
	PRECACHE_SOUND ("weapons/pl_gun3.wav");//handgun
}

int CGlock::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "9mm";
	// Oz variables (.value)
	p->iMaxAmmo1 = max9mm.value;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	// Oz variables (.value)
	p->iMaxClip = magazineglock.value;
	p->iSlot = 1;
	p->iPosition = 0;
	p->iFlags = 0;
	p->iId = m_iId = WEAPON_GLOCK;
	p->iWeight = weightglock.value;// Oz weight

	return 1;
}

BOOL CGlock::Deploy( )
{
	// Oz silencer
	if (silenceglock.value > 0)
		pev->body = 1;
	else
		pev->body = 0;
	return DefaultDeploy( "models/v_9mmhandgun.mdl", "models/p_9mmhandgun.mdl", GLOCK_DRAW, "onehanded" );
}

// Oz silencer
/*
void CGlock::SecondaryAttack( void )
{
	m_flTimeWeaponIdle = gpGlobals->time + 2.0f;
	m_flNextPrimaryAttack = m_flNextSecondaryAttack = gpGlobals->time + 2.0f;
	SetThink( AddSilencer );	pev->nextthink = gpGlobals->time + 1.0f;
	SendWeaponAnim( GLOCK_HOLSTER );
}
void CGlock::AddSilencer( void )
{
	if(pev->body == 1)
		pev->body = 0;
	else
		pev->body = 1;
	SendWeaponAnim( GLOCK_DRAW );
}
*/

void CGlock::SecondaryAttack( void )
{
	//jammer new superglock
	if(m_pPlayer->ctf_hassuperglock && m_pPlayer->ctf_hasrune)
	{
      if(SuperGlockZoom)
	  {
	    m_pPlayer->m_iFOV = 0;
		SuperGlockZoom = FALSE;
	  }
	  else
	  {
		m_pPlayer->m_iFOV = 40;
		SuperGlockZoom = TRUE;
	  }
	  	pev->nextthink = gpGlobals->time + 0.1;
		m_flNextSecondaryAttack = gpGlobals->time + .25;
	}

	// Oz glock sniper
	else if (glocksniper.value > 0)
	{
		if (OzZoom)
		{
			m_pPlayer->m_iFOV = 0; // 0 means reset to default fov
			OzZoom = 0;
		}
		else
		{
			m_pPlayer->m_iFOV = zoomamountglock.value;
			OzZoom = 1;
		}
		
		pev->nextthink = gpGlobals->time + 0.1;
		m_flNextSecondaryAttack = gpGlobals->time + 1.0;
	}
	else
		// Oz runes - haste
		if (m_pPlayer->ctf_hascycle)
			GlockFire( 0.1, cycleglock2.value * .5, FALSE );// Oz rate of fire
		else
			GlockFire( 0.1, cycleglock2.value, FALSE );// Oz rate of fire
}

void CGlock::PrimaryAttack( void )
{
	//jammer new superglock
	if(m_pPlayer->ctf_hassuperglock && m_pPlayer->ctf_hasrune)
	{
	   if(SuperGlockZoom)//zoomed so shoot slowly with big damage
	      GlockFire( 0.01,1.5, TRUE );
	   else
		  GlockFire( 0.01,.175, TRUE );//not zoomed so shoot a bit faster
	}
	
	// Oz runes - haste
	else if (m_pPlayer->ctf_hascycle)
		GlockFire( 0.01, cycleglock.value * .5, TRUE );// Oz rate of fire
	else
		GlockFire( 0.01, cycleglock.value, TRUE );// Oz rate of fire
}

void CGlock::GlockFire( float flSpread , float flCycleTime, BOOL fUseAutoAim )
{
	//jammer new superglock
	if(m_pPlayer->ctf_hassuperglock && m_pPlayer->ctf_hasrune)
		m_iClip++;
	
	if (m_iClip <= 0)
	{
		if (m_fFireOnEmpty)
		{
			PlayEmptySound();
			m_flNextPrimaryAttack = gpGlobals->time + 0.2;
		}

		return;
	}

	m_iClip--;

	m_pPlayer->pev->effects = (int)(m_pPlayer->pev->effects) | EF_MUZZLEFLASH;

	if (m_iClip != 0)
		SendWeaponAnim( GLOCK_SHOOT );
	else
		SendWeaponAnim( GLOCK_SHOOT_EMPTY );

	// player "shoot" animation
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	UTIL_MakeVectors( m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle );
		
	Vector	vecShellVelocity = m_pPlayer->pev->velocity 
							 + gpGlobals->v_right * RANDOM_FLOAT(50,70) 
							 + gpGlobals->v_up * RANDOM_FLOAT(100,150) 
							 + gpGlobals->v_forward * 25;
	EjectBrass ( pev->origin + m_pPlayer->pev->view_ofs + gpGlobals->v_up * -12 + gpGlobals->v_forward * 32 + gpGlobals->v_right * 6 , vecShellVelocity, pev->angles.y, m_iShell, TE_BOUNCE_SHELL ); 

	// Oz silencer
	if (pev->body == 1)
	{
		m_pPlayer->m_iWeaponVolume = QUIET_GUN_VOLUME;
		m_pPlayer->m_iWeaponFlash = DIM_GUN_FLASH;

		switch(RANDOM_LONG(0,1))
		{
		case 0:
			EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/pl_gun1.wav", RANDOM_FLOAT(0.9, 1.0), ATTN_NORM);
			break;
		case 1:
			EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/pl_gun2.wav", RANDOM_FLOAT(0.9, 1.0), ATTN_NORM);
			break;
		}
	}
	else
	{
		// non-silenced
		m_pPlayer->m_iWeaponVolume = NORMAL_GUN_VOLUME;
		m_pPlayer->m_iWeaponFlash = NORMAL_GUN_FLASH;
		EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/pl_gun3.wav", RANDOM_FLOAT(0.92, 1.0), ATTN_NORM, 0, 98 + RANDOM_LONG(0,3));
	}

	Vector vecSrc	 = m_pPlayer->GetGunPosition( );
	Vector vecAiming;
	
	if ( fUseAutoAim )
	{
		vecAiming = m_pPlayer->GetAutoaimVector( AUTOAIM_10DEGREES );
	}
	else
	{
		vecAiming = gpGlobals->v_forward;
	}
     //jammer new superglock
	if(SuperGlockZoom && m_pPlayer->ctf_hassuperglock && m_pPlayer->ctf_hasrune)
		m_pPlayer->FireBullets(1,vecSrc,vecAiming,Vector(flSpread,flSpread,flSpread),8192,BULLET_PLAYER_9MM_SUPERGLOCK,0);

	// Oz glock sniper
	else if (OzZoom)
		m_pPlayer->FireBullets( 1, vecSrc, vecAiming, Vector( flSpread, flSpread, flSpread ), 8192, BULLET_PLAYER_9MM_SNIPER, 0 );
	else
		m_pPlayer->FireBullets( 1, vecSrc, vecAiming, Vector( flSpread, flSpread, flSpread ), 8192, BULLET_PLAYER_9MM, 0 );

	m_flNextPrimaryAttack = m_flNextSecondaryAttack = gpGlobals->time + flCycleTime;

	if (!m_iClip && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
		// HEV suit - indicate out of ammo condition
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);

	m_flTimeWeaponIdle = gpGlobals->time + RANDOM_FLOAT ( 10, 15 );

	// Oz variables (.value)
	m_pPlayer->pev->punchangle.x -= recoilglock.value;
}


void CGlock::Reload( void )
{
	int iResult;

	// Oz glock sniper
	if ( OzZoom )
	{
		SecondaryAttack();
	}

	if (m_iClip == 0)
		// Oz variables (.value)
		iResult = DefaultReload( magazineglock.value, GLOCK_RELOAD, reloadglock.value );
	else
		// Oz variables (.value)
		iResult = DefaultReload( magazineglock.value + 1, GLOCK_RELOAD_NOT_EMPTY, reloadglock.value );

	if (iResult)
	{
		m_flTimeWeaponIdle = gpGlobals->time + RANDOM_FLOAT ( 10, 15 );
	}
}



void CGlock::WeaponIdle( void )
{
	ResetEmptySound( );

	m_pPlayer->GetAutoaimVector( AUTOAIM_10DEGREES );

	if (m_flTimeWeaponIdle > gpGlobals->time)
		return;

	// only idle if the slid isn't back
	if (m_iClip != 0)
	{
		int iAnim;
		float flRand = RANDOM_FLOAT(0, 1);
		if (flRand <= 0.3 + 0 * 0.75)
		{
			iAnim = GLOCK_IDLE3;
			m_flTimeWeaponIdle = gpGlobals->time + 49.0 / 16;
		}
		else if (flRand <= 0.6 + 0 * 0.875)
		{
			iAnim = GLOCK_IDLE1;
			m_flTimeWeaponIdle = gpGlobals->time + 60.0 / 16.0;
		}
		else
		{
			iAnim = GLOCK_IDLE2;
			m_flTimeWeaponIdle = gpGlobals->time + 40.0 / 16.0;
		}
		SendWeaponAnim( iAnim );
	}
}








class CGlockAmmo : public CBasePlayerAmmo
{
	void Spawn( void )
	{ 
		Precache( );
		SET_MODEL(ENT(pev), "models/w_9mmclip.mdl");
		CBasePlayerAmmo::Spawn( );
	}
	void Precache( void )
	{
		PRECACHE_MODEL ("models/w_9mmclip.mdl");
		PRECACHE_SOUND("items/9mmclip1.wav");
	}
	BOOL AddAmmo( CBaseEntity *pOther ) 
	{ 
		// Oz variables (.value)
		if (pOther->GiveAmmo( giveglockclip.value, "9mm", max9mm.value ) != -1)
		{
			EMIT_SOUND(ENT(pev), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);
			return TRUE;
		}
		return FALSE;
	}

	//jammer
public:
	void JamGlockAmmo(int,int,int,int,BOOL);
};
LINK_ENTITY_TO_CLASS( ammo_glockclip, CGlockAmmo );
LINK_ENTITY_TO_CLASS( ammo_9mmclip, CGlockAmmo );

//jammer
void CGlock::JamGlock(int x, int y ,int z, int a, BOOL random)
{
  Vector pos(x,y,z);
  Vector angle(0,a,0);    
  CBaseEntity *pGlock = CBaseEntity::Create("weapon_9mmhandgun",pos,angle,ENT(pev));//edict());
  if(random)
	  pGlock->pev->flags = FL_RANDOM;
}

void Glock(int x, int y, int z, int a, BOOL random)
{
  CGlock *pGlock = GetClassPtr((CGlock *)NULL);
  pGlock->JamGlock(x,y,z,a,random);
}
//jammer
void CGlockAmmo::JamGlockAmmo(int x, int y ,int z, int a, BOOL random)
{
  Vector pos(x,y,z);
  Vector angle(0,a,0);    
  CBaseEntity *pGlockAmmo = CBaseEntity::Create("ammo_9mmclip",pos,angle,ENT(pev));//edict());
  if(random)
	  pGlockAmmo->pev->flags = FL_RANDOM;
}

void GlockAmmo(int x, int y, int z, int a, BOOL random)
{
  CGlockAmmo *pGlockAmmo = GetClassPtr((CGlockAmmo *)NULL);
  pGlockAmmo->JamGlockAmmo(x,y,z,a,random);
}













