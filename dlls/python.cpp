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
#if !defined( OEM_BUILD ) && !defined( HLDEMO_BUILD )

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "weapons.h"
#include "monsters.h"
#include "player.h"
#include "gamerules.h"
#include "game.h"

//jammer infinity
extern BOOL FoundFile;

enum python_e {
	PYTHON_IDLE1 = 0,
	PYTHON_FIDGET,
	PYTHON_FIRE1,
	PYTHON_RELOAD,
	PYTHON_HOLSTER,
	PYTHON_DRAW,
	PYTHON_IDLE2,
	PYTHON_IDLE3
};

class CPython : public CBasePlayerWeapon
{
public:
	void Spawn( void );
	void Precache( void );
	int iItemSlot( void ) { return 2; }
	int GetItemInfo(ItemInfo *p);
	int AddToPlayer( CBasePlayer *pPlayer );
	void PrimaryAttack( void );
	void SecondaryAttack( void );
	BOOL Deploy( void );
	void Holster( void );
	void Reload( void );
	void WeaponIdle( void );
	float m_flSoundDelay;
	
	//jammer
	void JamPython(int,int,int,int, BOOL);

	BOOL m_fInZoom;// don't save this. 
};
LINK_ENTITY_TO_CLASS( weapon_python, CPython );
LINK_ENTITY_TO_CLASS( weapon_357, CPython );

int CPython::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "357";
	// Oz variables (.value)
	p->iMaxAmmo1 = max357rounds.value;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	// Oz variables (.value)
	p->iMaxClip = magazine357.value;
	p->iFlags = 0;
	p->iSlot = 1;
	p->iPosition = 1;
	p->iId = m_iId = WEAPON_PYTHON;
	p->iWeight = weight357.value;// Oz weight

	return 1;
}

int CPython::AddToPlayer( CBasePlayer *pPlayer )
{
	if ( CBasePlayerWeapon::AddToPlayer( pPlayer ) )
	{
		MESSAGE_BEGIN( MSG_ONE, gmsgWeapPickup, NULL, pPlayer->pev );
			WRITE_BYTE( m_iId );
		MESSAGE_END();
		return TRUE;
	}
	return FALSE;
}

void CPython::Spawn( )
{
	//jammer infinity
	if(FoundFile)
		pev->flags=FL_KILLME;

	// Oz variables (.value)
	if (disable357.value > 0)
	{
		Kill( );
		return;
	}

	// Oz random spawn
	if (randomspawn.value > 0)
	{
		pev->velocity.x = RANDOM_FLOAT( -500, 500 );
		pev->velocity.y = RANDOM_FLOAT( -500, 500 );
		pev->velocity.z = RANDOM_FLOAT( 0, 500 );
	}

	pev->classname = MAKE_STRING("weapon_357"); // hack to allow for old names
	Precache( );
	m_iId = WEAPON_PYTHON;
	SET_MODEL(ENT(pev), "models/w_357.mdl");

	// Oz variables (.value)
	m_iDefaultAmmo = givewith357.value;

	FallInit();// get ready to fall down.
}


void CPython::Precache( void )
{
	PRECACHE_MODEL("models/v_357.mdl");
	PRECACHE_MODEL("models/w_357.mdl");
	PRECACHE_MODEL("models/p_357.mdl");

	PRECACHE_MODEL("models/w_357ammobox.mdl");
	PRECACHE_SOUND("items/9mmclip1.wav");              

	PRECACHE_SOUND ("weapons/357_reload1.wav");
	PRECACHE_SOUND ("weapons/357_cock1.wav");
	PRECACHE_SOUND ("weapons/357_shot1.wav");
	PRECACHE_SOUND ("weapons/357_shot2.wav");
}

BOOL CPython::Deploy( )
{
	if ( g_pGameRules->IsMultiplayer() )
	{
		// enable laser sight geometry.
		pev->body = 1;
	}
	else
	{
		pev->body = 0;
	}

	return DefaultDeploy( "models/v_357.mdl", "models/p_357.mdl", PYTHON_DRAW, "python" );
}


void CPython::Holster( )
{
	m_fInReload = FALSE;// cancel any reload in progress.

	if ( m_fInZoom )
	{
		SecondaryAttack();
	}

	m_pPlayer->m_flNextAttack = gpGlobals->time + 1.0;
	m_flTimeWeaponIdle = gpGlobals->time + 10 + RANDOM_FLOAT ( 0, 5 );
	SendWeaponAnim( PYTHON_HOLSTER );
}

void CPython::SecondaryAttack( void )
{
	if ( !g_pGameRules->IsMultiplayer() )
	{
		return;
	}

	if ( m_fInZoom )
	{
		m_fInZoom = FALSE;
		m_pPlayer->m_iFOV = 0;  // 0 means reset to default fov
	}
	else
	{
		m_fInZoom = TRUE;
		m_pPlayer->m_iFOV = 40;
	}

	m_flNextSecondaryAttack = gpGlobals->time + 0.5;
}

void CPython::PrimaryAttack()
{
	// don't fire underwater
	// Oz variables (.value)
	if (m_pPlayer->pev->waterlevel == 3 && waterweapons.value == 0)
	{
		PlayEmptySound( );
		m_flNextPrimaryAttack = gpGlobals->time + 0.15;
		return;
	}

	if (m_iClip <= 0)
	{
		if (!m_fFireOnEmpty)
			Reload( );
		else
		{
	//		if (m_iClip == 0)
			//PlayEmptySound( );

			EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/357_cock1.wav", 0.8, ATTN_NORM);
			m_flNextPrimaryAttack = gpGlobals->time + 0.15;
		}

		return;
	}
/*
	if (m_iClip <= 0)
	{
		Reload( );
		if (m_iClip == 0)
			PlayEmptySound( );
		return;
	}
*/
	m_pPlayer->m_iWeaponVolume = LOUD_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = BRIGHT_GUN_FLASH;

	m_iClip--;

/*
	if (m_iClip || m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] == 0)
*/
		SendWeaponAnim( PYTHON_FIRE1 );

		// player "shoot" animation
		m_pPlayer->SetAnimation( PLAYER_ATTACK1 );
/*
	else
		Reload( );
*/
	m_pPlayer->pev->effects = (int)(m_pPlayer->pev->effects) | EF_MUZZLEFLASH;

	switch(RANDOM_LONG(0,1))
	{
	case 0:
		EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/357_shot1.wav", RANDOM_FLOAT(0.8, 0.9), ATTN_NORM);
		break;
	case 1:
		EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/357_shot2.wav", RANDOM_FLOAT(0.8, 0.9), ATTN_NORM);
		break;
	}

	UTIL_MakeVectors( m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle );

	Vector vecSrc	 = m_pPlayer->GetGunPosition( );
	Vector vecAiming = m_pPlayer->GetAutoaimVector( AUTOAIM_10DEGREES );

	// Oz 357 sniper
	if (m_fInZoom)
		m_pPlayer->FireBullets( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, 8192, BULLET_PLAYER_357_SNIPER, 0 );
	else
		m_pPlayer->FireBullets( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, 8192, BULLET_PLAYER_357, 0 );

	if (!m_iClip && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
		// HEV suit - indicate out of ammo condition
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);

	// Oz runes - haste
	if (m_pPlayer->ctf_hascycle)
		m_flNextPrimaryAttack = gpGlobals->time + cycle357.value * .5;// Oz rate of fire
	else
		m_flNextPrimaryAttack = gpGlobals->time + cycle357.value;// Oz rate of fire

	m_flTimeWeaponIdle = gpGlobals->time + RANDOM_FLOAT ( 10, 15 );

	// Oz variables (.value)
	m_pPlayer->pev->punchangle.x -= recoil357.value;
}


void CPython::Reload( void )
{
	if ( m_fInZoom )
	{
		m_fInZoom = FALSE;
		m_pPlayer->m_iFOV = 0;  // 0 means reset to default fov
	}

	// Oz variables (.value)
	if (DefaultReload( magazine357.value, PYTHON_RELOAD, reload357.value ))
	{
		m_flSoundDelay = gpGlobals->time + 1.5;
	}
}


void CPython::WeaponIdle( void )
{
	ResetEmptySound( );

	m_pPlayer->GetAutoaimVector( AUTOAIM_10DEGREES );

	// ALERT( at_console, "%.2f\n", gpGlobals->time - m_flSoundDelay );
	if (m_flSoundDelay != 0 && m_flSoundDelay <= gpGlobals->time)
	{
		EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/357_reload1.wav", RANDOM_FLOAT(0.8, 0.9), ATTN_NORM);
		m_flSoundDelay = 0;

		/*
		for (int i = 0; i < 6; i++)
		{
			EjectBrass ( m_pPlayer->pev->origin, 
						Vector( RANDOM_FLOAT( -10.0, 10.0 ), RANDOM_FLOAT( -10.0, 10.0 ), (float)0.0 ), 
						m_pPlayer->pev->angles.y, TE_BOUNCE_SHELL); 
		}
		*/
	}

	if (m_flTimeWeaponIdle > gpGlobals->time)
		return;

	int iAnim;
	float flRand = RANDOM_FLOAT(0, 1);
	if (flRand <= 0.5)
	{
		iAnim = PYTHON_IDLE1;
		m_flTimeWeaponIdle = gpGlobals->time + (70.0/30.0);
	}
	else if (flRand <= 0.7)
	{
		iAnim = PYTHON_IDLE2;
		m_flTimeWeaponIdle = gpGlobals->time + (60.0/30.0);
	}
	else if (flRand <= 0.9)
	{
		iAnim = PYTHON_IDLE3;
		m_flTimeWeaponIdle = gpGlobals->time + (88.0/30.0);
	}
	else
	{
		iAnim = PYTHON_FIDGET;
		m_flTimeWeaponIdle = gpGlobals->time + (170.0/30.0);
	}
	SendWeaponAnim( iAnim );
}



class CPythonAmmo : public CBasePlayerAmmo
{
	void Spawn( void )
	{ 
		Precache( );
		SET_MODEL(ENT(pev), "models/w_357ammobox.mdl");
		CBasePlayerAmmo::Spawn( );
	}
	void Precache( void )
	{
		PRECACHE_MODEL ("models/w_357ammobox.mdl");
		PRECACHE_SOUND("items/9mmclip1.wav");
	}
	BOOL AddAmmo( CBaseEntity *pOther ) 
	{ 
		// Oz variables (.value)
		if (pOther->GiveAmmo( give357rounds.value, "357", max357rounds.value ) != -1)
		{
			EMIT_SOUND(ENT(pev), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);
			return TRUE;
		}
		return FALSE;
	}
//jammer
public:
	void JamPythonAmmo(int,int,int,int,BOOL);
};
LINK_ENTITY_TO_CLASS( ammo_357, CPythonAmmo );

//jammer infinity
void CPython::JamPython(int x, int y ,int z, int a, BOOL random)
{
  Vector pos(x,y,z);
  Vector angle(0,a,0);    
  CBaseEntity *pPython = CBaseEntity::Create("weapon_357",pos,angle,ENT(pev));//edict());
  if(random)
	  pPython->pev->flags = FL_RANDOM;
}

void Python(int x, int y, int z, int a, BOOL random)
{
  CPython *pPython = GetClassPtr((CPython *)NULL);
  pPython->JamPython(x,y,z,a,random);
}
//jammer
void CPythonAmmo::JamPythonAmmo(int x, int y ,int z, int a, BOOL random)
{
  Vector pos(x,y,z);
  Vector angle(0,a,0);    
  CBaseEntity *pPythonAmmo = CBaseEntity::Create("ammo_357",pos,angle,ENT(pev));
  if(random)
	  pPythonAmmo->pev->flags = FL_RANDOM;
}

void PythonAmmo(int x, int y, int z, int a, BOOL random)
{
  CPythonAmmo *pPythonAmmo = GetClassPtr((CPythonAmmo *)NULL);
  pPythonAmmo->JamPythonAmmo(x,y,z,a,random);
}

#endif