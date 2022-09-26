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
#include "soundent.h"
#include "gamerules.h"
#include "game.h"

//jammer infinity
extern BOOL FoundFile;

enum mp5_e
{
	MP5_LONGIDLE = 0,
	MP5_IDLE1,
	MP5_LAUNCH,
	MP5_RELOAD,
	MP5_DEPLOY,
	MP5_FIRE1,
	MP5_FIRE2,
	MP5_FIRE3,
};


class CMP5 : public CBasePlayerWeapon
{
public:
	void Spawn( void );
	void Precache( void );
	int iItemSlot( void ) { return 3; }
	int GetItemInfo(ItemInfo *p);
	int AddToPlayer( CBasePlayer *pPlayer );

	void PrimaryAttack( void );
	void SecondaryAttack( void );
	int SecondaryAmmoIndex( void );
	BOOL Deploy( void );
	void Reload( void );
	void WeaponIdle( void );
	float m_flNextAnimTime;
	int m_iShell;

	//jammer
	void JamMp5(int,int,int,int,BOOL);
};
LINK_ENTITY_TO_CLASS( weapon_mp5, CMP5 );
LINK_ENTITY_TO_CLASS( weapon_9mmAR, CMP5 );


//=========================================================
//=========================================================
int CMP5::SecondaryAmmoIndex( void )
{
	return m_iSecondaryAmmoType;
}

void CMP5::Spawn( )
{
	//jammer infinity
	if(FoundFile)
		pev->flags=FL_KILLME;

	// Oz variables (.value)
	if (disablemp5.value > 0)
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

	pev->classname = MAKE_STRING("weapon_9mmAR"); // hack to allow for old names
	Precache( );
	SET_MODEL(ENT(pev), "models/w_9mmAR.mdl");
	m_iId = WEAPON_MP5;

	// Oz variables (.value)
	m_iDefaultAmmo = givewithmp5.value;

	FallInit();// get ready to fall down.
}


void CMP5::Precache( void )
{
	PRECACHE_MODEL("models/v_9mmAR.mdl");
	PRECACHE_MODEL("models/w_9mmAR.mdl");
	PRECACHE_MODEL("models/p_9mmAR.mdl");

	m_iShell = PRECACHE_MODEL ("models/shell.mdl");// brass shellTE_MODEL

	PRECACHE_MODEL("models/grenade.mdl");	// grenade

	PRECACHE_MODEL("models/w_9mmARclip.mdl");
	PRECACHE_SOUND("items/9mmclip1.wav");              

	PRECACHE_SOUND("items/clipinsert1.wav");
	PRECACHE_SOUND("items/cliprelease1.wav");
//	PRECACHE_SOUND("items/guncock1.wav");

	PRECACHE_SOUND ("weapons/hks1.wav");// H to the K
	PRECACHE_SOUND ("weapons/hks2.wav");// H to the K
	PRECACHE_SOUND ("weapons/hks3.wav");// H to the K

	PRECACHE_SOUND( "weapons/glauncher.wav" );
	PRECACHE_SOUND( "weapons/glauncher2.wav" );

	PRECACHE_SOUND ("weapons/357_cock1.wav");

	PRECACHE_SOUND ("weapons/pl_gun1.wav");// H to the K
	PRECACHE_SOUND ("weapons/pl_gun2.wav");// H to the K

}

int CMP5::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "9mm";
	// Oz variables (.value)
	p->iMaxAmmo1 = max9mm.value;
	p->pszAmmo2 = "ARgrenades";
	// Oz variables (.value)
	p->iMaxAmmo2 = maxm203.value;
	// Oz variables (.value)
	p->iMaxClip = magazinemp5.value;
	p->iSlot = 2;
	p->iPosition = 0;
	p->iFlags = 0;
	p->iId = m_iId = WEAPON_MP5;
	p->iWeight = weightmp5.value;// Oz weight

	return 1;
}

int CMP5::AddToPlayer( CBasePlayer *pPlayer )
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

BOOL CMP5::Deploy( )
{
	return DefaultDeploy( "models/v_9mmAR.mdl", "models/p_9mmAR.mdl", MP5_DEPLOY, "mp5" );
}


void CMP5::PrimaryAttack()
{
	// Oz cvar
	int	vc;
	// Oz variables (.value)
	vc = mp5spread.value;

	// Oz water weapons
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
		PlayEmptySound();
		m_flNextPrimaryAttack = gpGlobals->time + 0.15;
		return;
	}

	// Oz silenced mp5
	if (silencemp5.value > 0)
	{
		m_pPlayer->m_iWeaponVolume = NORMAL_GUN_VOLUME;
		m_pPlayer->m_iWeaponFlash = NORMAL_GUN_FLASH;
	}
	else
	{
		m_pPlayer->m_iWeaponVolume = QUIET_GUN_VOLUME;
		m_pPlayer->m_iWeaponFlash = DIM_GUN_FLASH;
	}

	m_iClip--;

	// Oz silenced mp5
	if (silencemp5.value < 1)
		m_pPlayer->pev->effects = (int)(m_pPlayer->pev->effects) | EF_MUZZLEFLASH;


	if (1 || m_flNextAnimTime < gpGlobals->time)
	{
		SendWeaponAnim( MP5_FIRE1 + RANDOM_LONG(0,2));
		m_flNextAnimTime = gpGlobals->time + 0.2;
	}

	// player "shoot" animation
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	// Oz silenced mp5
	if (silencemp5.value > 0)
	{
		switch( RANDOM_LONG(0,1) )
		{
		case 0: EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/pl_gun1.wav", 1, ATTN_NORM, 0, 94 + RANDOM_LONG(0,0xf)); break;
		case 1: EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/pl_gun2.wav", 1, ATTN_NORM, 0, 94 + RANDOM_LONG(0,0xf)); break;
	//	case 2: EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/hks3.wav", 1, ATTN_NORM); break;
		}
	}
	else
	{
		switch( RANDOM_LONG(0,1) )
		{
		case 0: EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/hks1.wav", 1, ATTN_NORM, 0, 94 + RANDOM_LONG(0,0xf)); break;
		case 1: EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/hks2.wav", 1, ATTN_NORM, 0, 94 + RANDOM_LONG(0,0xf)); break;
	//	case 2: EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/hks3.wav", 1, ATTN_NORM); break;
		}
	}


	UTIL_MakeVectors( m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle );

	Vector	vecShellVelocity = m_pPlayer->pev->velocity 
							 + gpGlobals->v_right * RANDOM_FLOAT(50,70) 
							 + gpGlobals->v_up * RANDOM_FLOAT(100,150) 
							 + gpGlobals->v_forward * 25;
	EjectBrass ( pev->origin + m_pPlayer->pev->view_ofs
					+ gpGlobals->v_up * -12 
					+ gpGlobals->v_forward * 20 
					+ gpGlobals->v_right * 4, vecShellVelocity, pev->angles.y, m_iShell, TE_BOUNCE_SHELL); 
	
	Vector vecSrc	 = m_pPlayer->GetGunPosition( );
	Vector vecAiming = m_pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES );
	
	if ( g_pGameRules->IsDeathmatch() )
	{
		// Oz mp5 sniper
		if (OzZoom)
		{
			m_pPlayer->FireBullets( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, 8192, BULLET_PLAYER_MP5_SNIPER, 0 );
		}
		else
		{
			// optimized multiplayer. Widened to make it easier to hit a moving player
			if (vc == 1)
				m_pPlayer->FireBullets( 1, vecSrc, vecAiming, VECTOR_CONE_1DEGREES, 8192, BULLET_PLAYER_MP5, mp5tracers.value );
			else if (vc == 2)
				m_pPlayer->FireBullets( 1, vecSrc, vecAiming, VECTOR_CONE_2DEGREES, 8192, BULLET_PLAYER_MP5, mp5tracers.value );
			else if (vc == 3)
				m_pPlayer->FireBullets( 1, vecSrc, vecAiming, VECTOR_CONE_3DEGREES, 8192, BULLET_PLAYER_MP5, mp5tracers.value );
			else if (vc == 4)
				m_pPlayer->FireBullets( 1, vecSrc, vecAiming, VECTOR_CONE_4DEGREES, 8192, BULLET_PLAYER_MP5, mp5tracers.value );
			else if (vc == 5)
				m_pPlayer->FireBullets( 1, vecSrc, vecAiming, VECTOR_CONE_5DEGREES, 8192, BULLET_PLAYER_MP5, mp5tracers.value );
			else if (vc == 6)
				m_pPlayer->FireBullets( 1, vecSrc, vecAiming, VECTOR_CONE_6DEGREES, 8192, BULLET_PLAYER_MP5, mp5tracers.value );
			else if (vc == 7)
				m_pPlayer->FireBullets( 1, vecSrc, vecAiming, VECTOR_CONE_7DEGREES, 8192, BULLET_PLAYER_MP5, mp5tracers.value );
			else if (vc == 8)
				m_pPlayer->FireBullets( 1, vecSrc, vecAiming, VECTOR_CONE_8DEGREES, 8192, BULLET_PLAYER_MP5, mp5tracers.value );
			else if (vc == 9)
				m_pPlayer->FireBullets( 1, vecSrc, vecAiming, VECTOR_CONE_9DEGREES, 8192, BULLET_PLAYER_MP5, mp5tracers.value );
			else if (vc == 10)
				m_pPlayer->FireBullets( 1, vecSrc, vecAiming, VECTOR_CONE_10DEGREES, 8192, BULLET_PLAYER_MP5, mp5tracers.value );
			else
				m_pPlayer->FireBullets( 1, vecSrc, vecAiming, VECTOR_CONE_6DEGREES, 8192, BULLET_PLAYER_MP5, mp5tracers.value );
		}
	}
	else
	{
		// single player spread
		m_pPlayer->FireBullets( 1, vecSrc, vecAiming, VECTOR_CONE_3DEGREES, 8192, BULLET_PLAYER_MP5, 2 );
	}

	if (!m_iClip && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
		// HEV suit - indicate out of ammo condition
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);

	// Oz mp5 sniper
	if (OzZoom)
	{
		// Oz runes - haste
		if (m_pPlayer->ctf_hascycle)
		{
			m_flNextPrimaryAttack = m_flNextPrimaryAttack + cyclemp5sniper.value * .5;// Oz rate of fire
			if (m_flNextPrimaryAttack < gpGlobals->time)
				m_flNextPrimaryAttack = gpGlobals->time + cyclemp5sniper.value * .5;// Oz rate of fire
		}
		else
		{
			m_flNextPrimaryAttack = m_flNextPrimaryAttack + cyclemp5sniper.value;// Oz rate of fire
			if (m_flNextPrimaryAttack < gpGlobals->time)
				m_flNextPrimaryAttack = gpGlobals->time + cyclemp5sniper.value;// Oz rate of fire
		}
	}
	else
	{
		// Oz runes - haste
		if (m_pPlayer->ctf_hascycle)
		{
			m_flNextPrimaryAttack = m_flNextPrimaryAttack + cyclemp5.value * .5;// Oz rate of fire
			if (m_flNextPrimaryAttack < gpGlobals->time)
				m_flNextPrimaryAttack = gpGlobals->time + cyclemp5.value * .5;// Oz rate of fire
		}
		else
		{
			m_flNextPrimaryAttack = m_flNextPrimaryAttack + cyclemp5.value;// Oz rate of fire
			if (m_flNextPrimaryAttack < gpGlobals->time)
				m_flNextPrimaryAttack = gpGlobals->time + cyclemp5.value;// Oz rate of fire
		}
	}

	m_flTimeWeaponIdle = gpGlobals->time + RANDOM_FLOAT ( 10, 15 );

	// Oz mp5 sniper
	if (!OzZoom)
		m_pPlayer->pev->punchangle.x = RANDOM_FLOAT( -(recoilmp5.value), recoilmp5.value );
}



void CMP5::SecondaryAttack( void )
{
	// Oz mp5 sniper
	if (mp5sniper.value > 0)
	{
		if (OzZoom)
		{
			m_pPlayer->m_iFOV = 0; // 0 means reset to default fov
			OzZoom = 0;
		}
		else
		{
			m_pPlayer->m_iFOV = zoomamountmp5.value;
			OzZoom = 1;
		}
		
		pev->nextthink = gpGlobals->time + 0.1;
		m_flNextSecondaryAttack = gpGlobals->time + 1.0;
	}
	else
	{
		// don't fire underwater
		// Oz variables (.value)
		if (m_pPlayer->pev->waterlevel == 3 && waterweapons.value == 0)
		{
			PlayEmptySound( );
			m_flNextPrimaryAttack = gpGlobals->time + 0.15;
			return;
		}

		if (m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType] == 0)
		{
			PlayEmptySound( );
			return;
		}

		m_pPlayer->m_iWeaponVolume = NORMAL_GUN_VOLUME;
		m_pPlayer->m_iWeaponFlash = BRIGHT_GUN_FLASH;

		m_pPlayer->m_iExtraSoundTypes = bits_SOUND_DANGER;
		m_pPlayer->m_flStopExtraSoundTime = gpGlobals->time + 0.2;
				
		m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType]--;

		SendWeaponAnim( MP5_LAUNCH );

		// player "shoot" animation
		m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

		if ( RANDOM_LONG(0,1) )
		{
			// play this sound through BODY channel so we can hear it if player didn't stop firing MP3
			EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/glauncher.wav", 0.8, ATTN_NORM);
		}
		else
		{
			// play this sound through BODY channel so we can hear it if player didn't stop firing MP3
			EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/glauncher2.wav", 0.8, ATTN_NORM);
		}
 
		UTIL_MakeVectors( m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle );

		// we don't add in player velocity anymore.
		CGrenade::ShootContact( m_pPlayer->pev, 
								m_pPlayer->pev->origin + m_pPlayer->pev->view_ofs + gpGlobals->v_forward * 16, 
								// Oz variables (.value)
								gpGlobals->v_forward * speedm203.value );
		
		// Oz runes - haste
		if (m_pPlayer->ctf_hascycle)
			m_flNextPrimaryAttack = gpGlobals->time + cyclem203.value * .5;// Oz rate of fire
		else
			m_flNextPrimaryAttack = gpGlobals->time + cyclem203.value;// Oz rate of fire

		// Oz runes - haste
		if (m_pPlayer->ctf_hascycle)
			m_flNextSecondaryAttack = gpGlobals->time + cyclem203.value * .5;// Oz rate of fire
		else
			m_flNextSecondaryAttack = gpGlobals->time + cyclem203.value;// Oz rate of fire

		m_flTimeWeaponIdle = gpGlobals->time + 5;// idle pretty soon after shooting.

		if (!m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType])
			// HEV suit - indicate out of ammo condition
			m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);

		// Oz variables (.value)
		m_pPlayer->pev->punchangle.x -= recoilm203.value;
	}
}

void CMP5::Reload( void )
{
	// Oz mp5 sniper
	if ( OzZoom )
	{
		SecondaryAttack();
	}

	// Oz variables (.value)
	DefaultReload( magazinemp5.value, MP5_RELOAD, reloadmp5.value );
}



void CMP5::WeaponIdle( void )
{
	ResetEmptySound( );

	m_pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES );

	if (m_flTimeWeaponIdle > gpGlobals->time)
		return;

	int iAnim;
	switch ( RANDOM_LONG( 0, 1 ) )
	{
	case 0:	
		iAnim = MP5_LONGIDLE;	
		break;
	
	default:
	case 1:
		iAnim = MP5_IDLE1;
		break;
	}

	SendWeaponAnim( iAnim );

	m_flTimeWeaponIdle = gpGlobals->time + RANDOM_FLOAT ( 10, 15 );// how long till we do this again.
}



class CMP5AmmoClip : public CBasePlayerAmmo
{
	void Spawn( void )
	{ 
		Precache( );
		SET_MODEL(ENT(pev), "models/w_9mmARclip.mdl");
		CBasePlayerAmmo::Spawn( );
	}
	void Precache( void )
	{
		PRECACHE_MODEL ("models/w_9mmARclip.mdl");
		PRECACHE_SOUND("items/9mmclip1.wav");
	}
	BOOL AddAmmo( CBaseEntity *pOther ) 
	{ 
		// Oz variables (.value)
		int bResult = (pOther->GiveAmmo( givemp5clip.value, "9mm", max9mm.value) != -1);
		if (bResult)
		{
			EMIT_SOUND(ENT(pev), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);
		}
		return bResult;
	}

//jammer infinity
public:
	void JamMP5AmmoClip(int,int,int,int,BOOL);
};
LINK_ENTITY_TO_CLASS( ammo_mp5clip, CMP5AmmoClip );
LINK_ENTITY_TO_CLASS( ammo_9mmAR, CMP5AmmoClip );



class CMP5Chainammo : public CBasePlayerAmmo
{
	void Spawn( void )
	{ 
		Precache( );
		SET_MODEL(ENT(pev), "models/w_chainammo.mdl");
		CBasePlayerAmmo::Spawn( );
	}
	void Precache( void )
	{
		PRECACHE_MODEL ("models/w_chainammo.mdl");
		PRECACHE_SOUND("items/9mmclip1.wav");
	}
	BOOL AddAmmo( CBaseEntity *pOther ) 
	{ 
		// Oz variables (.value)
		int bResult = (pOther->GiveAmmo( givechainbox.value, "9mm", max9mm.value) != -1);
		if (bResult)
		{
			EMIT_SOUND(ENT(pev), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);
		}
		return bResult;
	}
};
LINK_ENTITY_TO_CLASS( ammo_9mmbox, CMP5Chainammo );


class CMP5AmmoGrenade : public CBasePlayerAmmo
{
	void Spawn( void )
	{ 
		// Oz variables (.value)
		if (disablem203.value < 1)
		{
			Precache( );
			SET_MODEL(ENT(pev), "models/w_ARgrenade.mdl");
			CBasePlayerAmmo::Spawn( );
		}
	}
	void Precache( void )
	{
		PRECACHE_MODEL ("models/w_ARgrenade.mdl");
		PRECACHE_SOUND("items/9mmclip1.wav");
	}
	BOOL AddAmmo( CBaseEntity *pOther ) 
	{ 
		// Oz variables (.value)
		int bResult = (pOther->GiveAmmo( givem203.value, "ARgrenades", maxm203.value ) != -1);

		if (bResult)
		{
			EMIT_SOUND(ENT(pev), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);
		}
		return bResult;
	}

//jammer infinity
public:
	void JamMP5AmmoGrenade(int,int,int,int,BOOL);
};
LINK_ENTITY_TO_CLASS( ammo_mp5grenades, CMP5AmmoGrenade );
LINK_ENTITY_TO_CLASS( ammo_ARgrenades, CMP5AmmoGrenade );

//jammer infinity
void CMP5::JamMp5(int x, int y ,int z, int a, BOOL random)
{
  Vector pos(x,y,z);
  Vector angle(0,a,0);    
  CBaseEntity *pMp5 = CBaseEntity::Create("weapon_9mmAR",pos,angle,ENT(pev));//edict());
  if(random)
	  pMp5->pev->flags = FL_RANDOM;
}

void Mp5(int x, int y, int z, int a,BOOL random)
{
  CMP5 *pMp5 = GetClassPtr((CMP5 *)NULL);
  pMp5->JamMp5(x,y,z,a,random);
}

//jammer
void CMP5AmmoClip::JamMP5AmmoClip(int x, int y ,int z, int a, BOOL random)
{
  Vector pos(x,y,z);
  Vector angle(0,a,0);    
  CBaseEntity *pMp5AmmoClip = CBaseEntity::Create("ammo_mp5clip",pos,angle,ENT(pev));//edict());
  if(random)
	  pMp5AmmoClip->pev->flags = FL_RANDOM;
}

void MP5Ammoclip(int x, int y, int z, int a, BOOL random)
{
  CMP5AmmoClip *pMp5AmmoClip = GetClassPtr((CMP5AmmoClip *)NULL);
  pMp5AmmoClip->JamMP5AmmoClip(x,y,z,a,random);
}
//jammer
void CMP5AmmoGrenade::JamMP5AmmoGrenade(int x, int y ,int z, int a, BOOL random)
{
  Vector pos(x,y,z);
  Vector angle(0,a,0);    
  CBaseEntity *pMp5AmmoGrenade = CBaseEntity::Create("ammo_mp5grenades",pos,angle,ENT(pev));
  if(random)
	  pMp5AmmoGrenade->pev->flags = FL_RANDOM;
}

void MP5AmmoGrenade(int x, int y, int z, int a, BOOL random)
{
  CMP5AmmoGrenade *pMp5AmmoGrenade = GetClassPtr((CMP5AmmoGrenade *)NULL);
  pMp5AmmoGrenade->JamMP5AmmoGrenade(x,y,z,a,random);
}

















