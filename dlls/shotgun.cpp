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
#include "gamerules.h"
#include "game.h"

//jammer
extern BOOL FoundFile;

// special deathmatch shotgun spreads
#define VECTOR_CONE_DM_SHOTGUN	Vector( 0.08716, 0.04362, 0.00  )// 10 degrees by 5 degrees
#define VECTOR_CONE_DM_DOUBLESHOTGUN Vector( 0.17365, 0.04362, 0.00 ) // 20 degrees by 5 degrees

enum shotgun_e {
	SHOTGUN_IDLE = 0,
	SHOTGUN_FIRE,
	SHOTGUN_FIRE2,
	SHOTGUN_RELOAD,
	SHOTGUN_PUMP,
	SHOTGUN_START_RELOAD,
	SHOTGUN_DRAW,
	SHOTGUN_HOLSTER,
	SHOTGUN_IDLE4,
	SHOTGUN_IDLE_DEEP
};

class CShotgun : public CBasePlayerWeapon
{
public:
	int		Save( CSave &save );
	int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];

	void Spawn( void );
	void Precache( void );
	int iItemSlot( ) { return 3; }
	int GetItemInfo(ItemInfo *p);
	int AddToPlayer( CBasePlayer *pPlayer );

	void PrimaryAttack( void );
	void SecondaryAttack( void );
	BOOL Deploy( );
	void Reload( void );
	void WeaponIdle( void );
	int m_fInReload;
	float m_flNextReload;
	int m_iShell;
	float m_flPumpTime;

	//jammer
	void JamShotGun(int,int,int,int,BOOL);
};
LINK_ENTITY_TO_CLASS( weapon_shotgun, CShotgun );


TYPEDESCRIPTION	CShotgun::m_SaveData[] = 
{
	DEFINE_FIELD( CShotgun, m_flNextReload, FIELD_TIME ),
	DEFINE_FIELD( CShotgun, m_fInReload, FIELD_INTEGER ),
	DEFINE_FIELD( CShotgun, m_flNextReload, FIELD_TIME ),
	// DEFINE_FIELD( CShotgun, m_iShell, FIELD_INTEGER ),
	DEFINE_FIELD( CShotgun, m_flPumpTime, FIELD_TIME ),
};
IMPLEMENT_SAVERESTORE( CShotgun, CBasePlayerWeapon );



void CShotgun::Spawn( )
{
	//jammer
	if(FoundFile)
		pev->flags=FL_KILLME;

	// Oz variables (.value)
	if (disableshotgun.value > 0)
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

	Precache( );
	m_iId = WEAPON_SHOTGUN;
	SET_MODEL(ENT(pev), "models/w_shotgun.mdl");

	// Oz variables (.value)
	m_iDefaultAmmo = givewithshotgun.value;

	FallInit();// get ready to fall
}


void CShotgun::Precache( void )
{
	PRECACHE_MODEL("models/v_shotgun.mdl");
	PRECACHE_MODEL("models/w_shotgun.mdl");
	PRECACHE_MODEL("models/p_shotgun.mdl");

	m_iShell = PRECACHE_MODEL ("models/shotgunshell.mdl");// shotgun shell

	PRECACHE_SOUND("items/9mmclip1.wav");              

	PRECACHE_SOUND ("weapons/dbarrel1.wav");//shotgun
	PRECACHE_SOUND ("weapons/sbarrel1.wav");//shotgun

	PRECACHE_SOUND ("weapons/reload1.wav");	// shotgun reload
	PRECACHE_SOUND ("weapons/reload3.wav");	// shotgun reload

//	PRECACHE_SOUND ("weapons/sshell1.wav");	// shotgun reload - played on client
//	PRECACHE_SOUND ("weapons/sshell3.wav");	// shotgun reload - played on client
	
	PRECACHE_SOUND ("weapons/357_cock1.wav"); // gun empty sound
	PRECACHE_SOUND ("weapons/scock1.wav");	// cock gun
}

int CShotgun::AddToPlayer( CBasePlayer *pPlayer )
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


int CShotgun::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "buckshot";
	// Oz variables (.value)
	p->iMaxAmmo1 = maxshells.value;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	// Oz variables (.value)
	p->iMaxClip = magazineshotgun.value;
	p->iSlot = 2;
	p->iPosition = 1;
	p->iFlags = 0;
	p->iId = m_iId = WEAPON_SHOTGUN;
	p->iWeight = weightshotgun.value;// Oz weight

	return 1;
}



BOOL CShotgun::Deploy( )
{
	return DefaultDeploy( "models/v_shotgun.mdl", "models/p_shotgun.mdl", SHOTGUN_DRAW, "shotgun" );
}


void CShotgun::PrimaryAttack()
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
		Reload( );
		if (m_iClip == 0)
			PlayEmptySound( );
		return;
	}

	m_pPlayer->m_iWeaponVolume = LOUD_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = NORMAL_GUN_FLASH;

	m_iClip--;
	m_pPlayer->pev->effects = (int)(m_pPlayer->pev->effects) | EF_MUZZLEFLASH;

	SendWeaponAnim( SHOTGUN_FIRE );

	// player "shoot" animation
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	UTIL_MakeVectors( m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle );

	Vector	vecShellVelocity = m_pPlayer->pev->velocity 
							 + gpGlobals->v_right * RANDOM_FLOAT(50,70) 
							 + gpGlobals->v_up * RANDOM_FLOAT(100,150) 
							 + gpGlobals->v_forward * 25;

	EjectBrass ( m_pPlayer->pev->origin + m_pPlayer->pev->view_ofs + gpGlobals->v_up * -12 + gpGlobals->v_forward * 20 + gpGlobals->v_right * 4 , vecShellVelocity, pev->angles.y, m_iShell, TE_BOUNCE_SHOTSHELL); 

	EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/sbarrel1.wav", RANDOM_FLOAT(0.95, 1.0), ATTN_NORM, 0, 93 + RANDOM_LONG(0,0x1f));
	

	Vector vecSrc	 = m_pPlayer->GetGunPosition( );
	Vector vecAiming = m_pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES );

	if ( g_pGameRules->IsDeathmatch() )
	{
		// altered deathmatch spread
		// Oz variables (.value)
		m_pPlayer->FireBullets( pelletsshotgun.value, vecSrc, vecAiming, VECTOR_CONE_DM_SHOTGUN, 2048, BULLET_PLAYER_BUCKSHOT, 0 );
	}
	else
	{
		// regular old, untouched spread. 
		// Oz variables (.value)
		m_pPlayer->FireBullets( (pelletsshotgun.value + 2), vecSrc, vecAiming, VECTOR_CONE_10DEGREES, 2048, BULLET_PLAYER_BUCKSHOT, 0 );
	}

	if (!m_iClip && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
		// HEV suit - indicate out of ammo condition
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);

	if (m_iClip != 0)
		m_flPumpTime = gpGlobals->time + 0.5;

	// Oz runes - haste
	if (m_pPlayer->ctf_hascycle)
	{
		m_flNextPrimaryAttack = gpGlobals->time + cycleshotgun.value * .5;// Oz rate of fire
		m_flNextSecondaryAttack = gpGlobals->time + cycleshotgun.value * .5;// Oz rate of fire
	}
	else
	{
		m_flNextPrimaryAttack = gpGlobals->time + cycleshotgun.value;// Oz rate of fire
		m_flNextSecondaryAttack = gpGlobals->time + cycleshotgun.value;// Oz rate of fire
	}

	if (m_iClip != 0)
		m_flTimeWeaponIdle = gpGlobals->time + 5.0;
	else
		m_flTimeWeaponIdle = 0.75;
	m_fInReload = 0;

	// Oz variables (.value)
	m_pPlayer->pev->punchangle.x -= recoilshotgun.value;
}


void CShotgun::SecondaryAttack( void )
{
	// don't fire underwater
	// Oz variables (.value)
	if (m_pPlayer->pev->waterlevel == 3 && waterweapons.value == 0)
	{
		PlayEmptySound( );
		m_flNextPrimaryAttack = gpGlobals->time + 0.15;
		return;
	}

	if (m_iClip <= 1)
	{
		Reload( );
		PlayEmptySound( );
		return;
	}

	m_pPlayer->m_iWeaponVolume = LOUD_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = NORMAL_GUN_FLASH;

	m_iClip -= 2;

	m_pPlayer->pev->effects = (int)(m_pPlayer->pev->effects) | EF_MUZZLEFLASH;

	SendWeaponAnim( SHOTGUN_FIRE2 );

	// player "shoot" animation
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	UTIL_MakeVectors( m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle );
		
	Vector vecShellVelocity = m_pPlayer->pev->velocity 
							+ gpGlobals->v_right * RANDOM_FLOAT(50,70) 
							+ gpGlobals->v_up * RANDOM_FLOAT(100,150) 
							+ gpGlobals->v_forward * 25;
	EjectBrass ( m_pPlayer->pev->origin + m_pPlayer->pev->view_ofs + gpGlobals->v_up * -12 + gpGlobals->v_forward * 20 + gpGlobals->v_right * 4 , vecShellVelocity, pev->angles.y, m_iShell, TE_BOUNCE_SHOTSHELL); 
	vecShellVelocity		= m_pPlayer->pev->velocity 
							+ gpGlobals->v_right * RANDOM_FLOAT(50,70) 
							+ gpGlobals->v_up * RANDOM_FLOAT(100,150) 
							+ gpGlobals->v_forward * 25;
	EjectBrass ( m_pPlayer->pev->origin + m_pPlayer->pev->view_ofs + gpGlobals->v_up * -12 + gpGlobals->v_forward * 20 + gpGlobals->v_right * 4 , vecShellVelocity, pev->angles.y, m_iShell, TE_BOUNCE_SHOTSHELL); 

	EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/dbarrel1.wav", RANDOM_FLOAT(0.98, 1.0), ATTN_NORM, 0, 85 + RANDOM_LONG(0,0x1f));
	
	Vector vecSrc	 = m_pPlayer->GetGunPosition( );
	Vector vecAiming = m_pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES );
	
	if ( g_pGameRules->IsDeathmatch() )
	{
		// tuned for deathmatch
		// Oz variables (.value)
		m_pPlayer->FireBullets( pelletsdoubleshotgun.value, vecSrc, vecAiming, VECTOR_CONE_DM_DOUBLESHOTGUN, 2048, BULLET_PLAYER_BUCKSHOT, 0 );
	}
	else
	{
		// untouched default single player
		// Oz variables (.value)
		m_pPlayer->FireBullets( (pelletsdoubleshotgun.value + 4), vecSrc, vecAiming, VECTOR_CONE_10DEGREES, 2048, BULLET_PLAYER_BUCKSHOT, 0 );
	}


	if (!m_iClip && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
		// HEV suit - indicate out of ammo condition
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);

	if (m_iClip != 0)
		m_flPumpTime = gpGlobals->time + 0.95;

	// Oz runes - haste
	if (m_pPlayer->ctf_hascycle)
	{
		m_flNextPrimaryAttack = gpGlobals->time + cycleshotgun2.value * .5;// Oz rate of fire
		m_flNextSecondaryAttack = gpGlobals->time + cycleshotgun2.value * .5;// Oz rate of fire
	}
	else
	{
		m_flNextPrimaryAttack = gpGlobals->time + cycleshotgun2.value;// Oz rate of fire
		m_flNextSecondaryAttack = gpGlobals->time + cycleshotgun2.value;// Oz rate of fire
	}

	if (m_iClip != 0)
		m_flTimeWeaponIdle = gpGlobals->time + 6.0;
	else
		m_flTimeWeaponIdle = 1.5;

	m_fInReload = 0;

	// Oz variables (.value)
	m_pPlayer->pev->punchangle.x -= recoilshotgun2.value;
}


void CShotgun::Reload( void )
{
	int m_iMaxClip;
	int m_iDiff;
	// Oz variables (.value)
	m_iMaxClip = magazineshotgun.value;
	if (m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0 || m_iClip == m_iMaxClip)
		return;

	if (m_flNextReload > gpGlobals->time)
		return;

	// don't reload until recoil is done
	if (m_flNextPrimaryAttack > gpGlobals->time)
		return;

	// check to see if we're ready to reload
	if (m_fInReload == 0)
	{
		SendWeaponAnim( SHOTGUN_START_RELOAD );
		m_fInReload = 1;
		m_pPlayer->m_flNextAttack = gpGlobals->time + 0.6;
		m_flTimeWeaponIdle = gpGlobals->time + 0.6;
		m_flNextPrimaryAttack = gpGlobals->time + 1.0;
		m_flNextSecondaryAttack = gpGlobals->time + 1.0;
		return;
	}
	else if (m_fInReload == 1)
	{
		if (m_flTimeWeaponIdle > gpGlobals->time)
			return;
		// was waiting for gun to move to side
		m_fInReload = 2;

		if (RANDOM_LONG(0,1))
			EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/reload1.wav", 1, ATTN_NORM, 0, 85 + RANDOM_LONG(0,0x1f));
		else
			EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/reload3.wav", 1, ATTN_NORM, 0, 85 + RANDOM_LONG(0,0x1f));

		SendWeaponAnim( SHOTGUN_RELOAD );

		m_flNextReload = gpGlobals->time + 0.5;
		m_flTimeWeaponIdle = gpGlobals->time + 0.5;
	}
	else
	{
		// Add them to the clip
		m_iClip += reloadshotgun.value;
		if (m_iClip > m_iMaxClip)
		{
			m_iDiff = (m_iClip - m_iMaxClip);
			m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] += m_iDiff;
			m_iClip = m_iMaxClip;
		}
		// Oz variables (.value)
		m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] -= reloadshotgun.value;
		m_fInReload = 1;
	}
}


void CShotgun::WeaponIdle( void )
{
	int m_iMaxClip;
	// Oz variables (.value)
	m_iMaxClip = magazineshotgun.value;

	ResetEmptySound( );

	m_pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES );

	if (m_flPumpTime && m_flPumpTime < gpGlobals->time)
	{
		// play pumping sound
		EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/scock1.wav", 1, ATTN_NORM, 0, 95 + RANDOM_LONG(0,0x1f));
		m_flPumpTime = 0;
	}

	if (m_flTimeWeaponIdle < gpGlobals->time)
	{
		if (m_iClip == 0 && m_fInReload == 0 && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType])
		{
			Reload( );
		}
		else if (m_fInReload != 0)
		{
			if (m_iClip != m_iMaxClip && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType])
			{
				Reload( );
			}
			else
			{
				// reload debounce has timed out
				SendWeaponAnim( SHOTGUN_PUMP );
				
				// play cocking sound
				EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/scock1.wav", 1, ATTN_NORM, 0, 95 + RANDOM_LONG(0,0x1f));
				m_fInReload = 0;
				m_flTimeWeaponIdle = gpGlobals->time + 1.5;
			}
		}
		else
		{
			int iAnim;
			float flRand = RANDOM_FLOAT(0, 1);
			if (flRand <= 0.8)
			{
				iAnim = SHOTGUN_IDLE_DEEP;
				m_flTimeWeaponIdle = gpGlobals->time + (60.0/12.0);// * RANDOM_LONG(2, 5);
			}
			else if (flRand <= 0.95)
			{
				iAnim = SHOTGUN_IDLE;
				m_flTimeWeaponIdle = gpGlobals->time + (20.0/9.0);
			}
			else
			{
				iAnim = SHOTGUN_IDLE4;
				m_flTimeWeaponIdle = gpGlobals->time + (20.0/9.0);
			}
			SendWeaponAnim( iAnim );
		}
	}
}



class CShotgunAmmo : public CBasePlayerAmmo
{
	void Spawn( void )
	{ 
		Precache( );
		SET_MODEL(ENT(pev), "models/w_shotbox.mdl");
		CBasePlayerAmmo::Spawn( );
	}
	void Precache( void )
	{
		PRECACHE_MODEL ("models/w_shotbox.mdl");
		PRECACHE_SOUND("items/9mmclip1.wav");
	}
	BOOL AddAmmo( CBaseEntity *pOther ) 
	{ 
		// Oz variables (.value)
		if (pOther->GiveAmmo( giveshells.value, "buckshot", maxshells.value ) != -1)
		{
			EMIT_SOUND(ENT(pev), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);
			return TRUE;
		}
		return FALSE;
	}
public: //jammer
	void JamShotGunAmmo(int,int,int,int,BOOL);
};
LINK_ENTITY_TO_CLASS( ammo_buckshot, CShotgunAmmo );

//jammer
void CShotgun::JamShotGun(int x, int y ,int z, int a,BOOL random)
{
  Vector pos(x,y,z);
  Vector angle(0,a,0);    
  CBaseEntity *pShotgun = CBaseEntity::Create("weapon_shotgun",pos,angle,ENT(pev));
  if(random)
	  pShotgun->pev->flags = FL_RANDOM;
}

void ShotGun(int x, int y, int z, int a,BOOL random)
{
  CShotgun *pShotgun = GetClassPtr((CShotgun *)NULL);
  pShotgun->JamShotGun(x,y,z,a,random);
}
//jammer
void CShotgunAmmo::JamShotGunAmmo(int x, int y ,int z, int a, BOOL random)
{
  Vector pos(x,y,z);
  Vector angle(0,a,0);    
  CBaseEntity *pShotgunAmmo = CBaseEntity::Create("ammo_buckshot",pos,angle,ENT(pev));
  if(random)
	  pShotgunAmmo->pev->flags = FL_RANDOM;
}

void ShotGunAmmo(int x, int y, int z, int a, BOOL random)
{
  CShotgunAmmo *pShotgunAmmo = GetClassPtr((CShotgunAmmo *)NULL);
  pShotgunAmmo->JamShotGunAmmo(x,y,z,a,random);
}


