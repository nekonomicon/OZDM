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
#include "eiface.h"
#include "util.h"
#include "game.h"


cvar_t	displaysoundlist = {"displaysoundlist","0"};
cvar_t  mapcyclefile = {"mapcyclefile","mapcycle.txt"};
cvar_t  servercfgfile = {"servercfgfile","server.cfg"};
cvar_t  lservercfgfile = {"lservercfgfile","listenserver.cfg"};

// multiplayer server rules
cvar_t	teamplay	= {"mp_teamplay","0", FCVAR_SERVER };
cvar_t	fraglimit	= {"mp_fraglimit","0", FCVAR_SERVER };
cvar_t	timelimit	= {"mp_timelimit","0", FCVAR_SERVER };
cvar_t	friendlyfire= {"mp_friendlyfire","0", FCVAR_SERVER };
cvar_t	falldamage	= {"mp_falldamage","0", FCVAR_SERVER };
cvar_t	weaponstay	= {"mp_weaponstay","0", FCVAR_SERVER };
cvar_t	forcerespawn= {"mp_forcerespawn","1", FCVAR_SERVER };
cvar_t	footsteps	= {"mp_footsteps","1", FCVAR_SERVER };
cvar_t	flashlight	= {"mp_flashlight","0", FCVAR_SERVER };
cvar_t	aimcrosshair= {"mp_autocrosshair","1", FCVAR_SERVER };
cvar_t	decalfrequency = {"decalfrequency","20", FCVAR_SERVER };
cvar_t	teamlist = {"mp_teamlist","hgrunt;scientist", FCVAR_SERVER };
cvar_t	teamoverride = {"mp_teamoverride","1" };
cvar_t	defaultteam = {"mp_defaultteam","0" };
cvar_t	allowmonsters={"mp_allowmonsters","0", FCVAR_SERVER };

//CVARS FOR SKILL LEVEL SETTINGS
// Agrunt
cvar_t	sk_agrunt_health1 = {"sk_agrunt_health1","0"};
cvar_t	sk_agrunt_health2 = {"sk_agrunt_health2","0"};
cvar_t	sk_agrunt_health3 = {"sk_agrunt_health3","0"};

cvar_t	sk_agrunt_dmg_punch1 = {"sk_agrunt_dmg_punch1","0"};
cvar_t	sk_agrunt_dmg_punch2 = {"sk_agrunt_dmg_punch2","0"};
cvar_t	sk_agrunt_dmg_punch3 = {"sk_agrunt_dmg_punch3","0"};

// Apache
cvar_t	sk_apache_health1	= {"sk_apache_health1","0"};
cvar_t	sk_apache_health2	= {"sk_apache_health2","0"};
cvar_t	sk_apache_health3	= {"sk_apache_health3","0"};

// Barney
cvar_t	sk_barney_health1	= {"sk_barney_health1","0"};
cvar_t	sk_barney_health2	= {"sk_barney_health2","0"};
cvar_t	sk_barney_health3	= {"sk_barney_health3","0"};

// Bullsquid
cvar_t	sk_bullsquid_health1 = {"sk_bullsquid_health1","0"};
cvar_t	sk_bullsquid_health2 = {"sk_bullsquid_health2","0"};
cvar_t	sk_bullsquid_health3 = {"sk_bullsquid_health3","0"};

cvar_t	sk_bullsquid_dmg_bite1 = {"sk_bullsquid_dmg_bite1","0"};
cvar_t	sk_bullsquid_dmg_bite2 = {"sk_bullsquid_dmg_bite2","0"};
cvar_t	sk_bullsquid_dmg_bite3 = {"sk_bullsquid_dmg_bite3","0"};

cvar_t	sk_bullsquid_dmg_whip1 = {"sk_bullsquid_dmg_whip1","0"};
cvar_t	sk_bullsquid_dmg_whip2 = {"sk_bullsquid_dmg_whip2","0"};
cvar_t	sk_bullsquid_dmg_whip3 = {"sk_bullsquid_dmg_whip3","0"};

cvar_t	sk_bullsquid_dmg_spit1 = {"sk_bullsquid_dmg_spit1","0"};
cvar_t	sk_bullsquid_dmg_spit2 = {"sk_bullsquid_dmg_spit2","0"};
cvar_t	sk_bullsquid_dmg_spit3 = {"sk_bullsquid_dmg_spit3","0"};


// Big Momma
cvar_t	sk_bigmomma_health_factor1 = {"sk_bigmomma_health_factor1","1.0"};
cvar_t	sk_bigmomma_health_factor2 = {"sk_bigmomma_health_factor2","1.0"};
cvar_t	sk_bigmomma_health_factor3 = {"sk_bigmomma_health_factor3","1.0"};

cvar_t	sk_bigmomma_dmg_slash1 = {"sk_bigmomma_dmg_slash1","50"};
cvar_t	sk_bigmomma_dmg_slash2 = {"sk_bigmomma_dmg_slash2","50"};
cvar_t	sk_bigmomma_dmg_slash3 = {"sk_bigmomma_dmg_slash3","50"};

cvar_t	sk_bigmomma_dmg_blast1 = {"sk_bigmomma_dmg_blast1","100"};
cvar_t	sk_bigmomma_dmg_blast2 = {"sk_bigmomma_dmg_blast2","100"};
cvar_t	sk_bigmomma_dmg_blast3 = {"sk_bigmomma_dmg_blast3","100"};

cvar_t	sk_bigmomma_radius_blast1 = {"sk_bigmomma_radius_blast1","250"};
cvar_t	sk_bigmomma_radius_blast2 = {"sk_bigmomma_radius_blast2","250"};
cvar_t	sk_bigmomma_radius_blast3 = {"sk_bigmomma_radius_blast3","250"};

// Gargantua
cvar_t	sk_gargantua_health1 = {"sk_gargantua_health1","0"};
cvar_t	sk_gargantua_health2 = {"sk_gargantua_health2","0"};
cvar_t	sk_gargantua_health3 = {"sk_gargantua_health3","0"};

cvar_t	sk_gargantua_dmg_slash1	= {"sk_gargantua_dmg_slash1","0"};
cvar_t	sk_gargantua_dmg_slash2	= {"sk_gargantua_dmg_slash2","0"};
cvar_t	sk_gargantua_dmg_slash3	= {"sk_gargantua_dmg_slash3","0"};

cvar_t	sk_gargantua_dmg_fire1 = {"sk_gargantua_dmg_fire1","0"};
cvar_t	sk_gargantua_dmg_fire2 = {"sk_gargantua_dmg_fire2","0"};
cvar_t	sk_gargantua_dmg_fire3 = {"sk_gargantua_dmg_fire3","0"};

cvar_t	sk_gargantua_dmg_stomp1	= {"sk_gargantua_dmg_stomp1","0"};
cvar_t	sk_gargantua_dmg_stomp2	= {"sk_gargantua_dmg_stomp2","0"};
cvar_t	sk_gargantua_dmg_stomp3	= {"sk_gargantua_dmg_stomp3","0"};


// Hassassin
cvar_t	sk_hassassin_health1 = {"sk_hassassin_health1","0"};
cvar_t	sk_hassassin_health2 = {"sk_hassassin_health2","0"};
cvar_t	sk_hassassin_health3 = {"sk_hassassin_health3","0"};


// Headcrab
cvar_t	sk_headcrab_health1 = {"sk_headcrab_health1","0"};
cvar_t	sk_headcrab_health2 = {"sk_headcrab_health2","0"};
cvar_t	sk_headcrab_health3 = {"sk_headcrab_health3","0"};

cvar_t	sk_headcrab_dmg_bite1 = {"sk_headcrab_dmg_bite1","0"};
cvar_t	sk_headcrab_dmg_bite2 = {"sk_headcrab_dmg_bite2","0"};
cvar_t	sk_headcrab_dmg_bite3 = {"sk_headcrab_dmg_bite3","0"};


// Hgrunt 
cvar_t	sk_hgrunt_health1 = {"sk_hgrunt_health1","0"};
cvar_t	sk_hgrunt_health2 = {"sk_hgrunt_health2","0"};
cvar_t	sk_hgrunt_health3 = {"sk_hgrunt_health3","0"};

cvar_t	sk_hgrunt_kick1 = {"sk_hgrunt_kick1","0"};
cvar_t	sk_hgrunt_kick2 = {"sk_hgrunt_kick2","0"};
cvar_t	sk_hgrunt_kick3 = {"sk_hgrunt_kick3","0"};

cvar_t	sk_hgrunt_pellets1 = {"sk_hgrunt_pellets1","0"};
cvar_t	sk_hgrunt_pellets2 = {"sk_hgrunt_pellets2","0"};
cvar_t	sk_hgrunt_pellets3 = {"sk_hgrunt_pellets3","0"};

cvar_t	sk_hgrunt_gspeed1 = {"sk_hgrunt_gspeed1","0"};
cvar_t	sk_hgrunt_gspeed2 = {"sk_hgrunt_gspeed2","0"};
cvar_t	sk_hgrunt_gspeed3 = {"sk_hgrunt_gspeed3","0"};

// Houndeye
cvar_t	sk_houndeye_health1 = {"sk_houndeye_health1","0"};
cvar_t	sk_houndeye_health2 = {"sk_houndeye_health2","0"};
cvar_t	sk_houndeye_health3 = {"sk_houndeye_health3","0"};

cvar_t	sk_houndeye_dmg_blast1 = {"sk_houndeye_dmg_blast1","0"};
cvar_t	sk_houndeye_dmg_blast2 = {"sk_houndeye_dmg_blast2","0"};
cvar_t	sk_houndeye_dmg_blast3 = {"sk_houndeye_dmg_blast3","0"};


// ISlave
cvar_t	sk_islave_health1 = {"sk_islave_health1","0"};
cvar_t	sk_islave_health2 = {"sk_islave_health2","0"};
cvar_t	sk_islave_health3 = {"sk_islave_health3","0"};

cvar_t	sk_islave_dmg_claw1 = {"sk_islave_dmg_claw1","0"};
cvar_t	sk_islave_dmg_claw2 = {"sk_islave_dmg_claw2","0"};
cvar_t	sk_islave_dmg_claw3 = {"sk_islave_dmg_claw3","0"};

cvar_t	sk_islave_dmg_clawrake1	= {"sk_islave_dmg_clawrake1","0"};
cvar_t	sk_islave_dmg_clawrake2	= {"sk_islave_dmg_clawrake2","0"};
cvar_t	sk_islave_dmg_clawrake3	= {"sk_islave_dmg_clawrake3","0"};
	
cvar_t	sk_islave_dmg_zap1 = {"sk_islave_dmg_zap1","0"};
cvar_t	sk_islave_dmg_zap2 = {"sk_islave_dmg_zap2","0"};
cvar_t	sk_islave_dmg_zap3 = {"sk_islave_dmg_zap3","0"};


// Icthyosaur
cvar_t	sk_ichthyosaur_health1	= {"sk_ichthyosaur_health1","0"};
cvar_t	sk_ichthyosaur_health2	= {"sk_ichthyosaur_health2","0"};
cvar_t	sk_ichthyosaur_health3	= {"sk_ichthyosaur_health3","0"};

cvar_t	sk_ichthyosaur_shake1	= {"sk_ichthyosaur_shake1","0"};
cvar_t	sk_ichthyosaur_shake2	= {"sk_ichthyosaur_shake2","0"};
cvar_t	sk_ichthyosaur_shake3	= {"sk_ichthyosaur_shake3","0"};


// Leech
cvar_t	sk_leech_health1 = {"sk_leech_health1","0"};
cvar_t	sk_leech_health2 = {"sk_leech_health2","0"};
cvar_t	sk_leech_health3 = {"sk_leech_health3","0"};

cvar_t	sk_leech_dmg_bite1 = {"sk_leech_dmg_bite1","0"};
cvar_t	sk_leech_dmg_bite2 = {"sk_leech_dmg_bite2","0"};
cvar_t	sk_leech_dmg_bite3 = {"sk_leech_dmg_bite3","0"};

// Controller
cvar_t	sk_controller_health1 = {"sk_controller_health1","0"};
cvar_t	sk_controller_health2 = {"sk_controller_health2","0"};
cvar_t	sk_controller_health3 = {"sk_controller_health3","0"};

cvar_t	sk_controller_dmgzap1 = {"sk_controller_dmgzap1","0"};
cvar_t	sk_controller_dmgzap2 = {"sk_controller_dmgzap2","0"};
cvar_t	sk_controller_dmgzap3 = {"sk_controller_dmgzap3","0"};

cvar_t	sk_controller_speedball1 = {"sk_controller_speedball1","0"};
cvar_t	sk_controller_speedball2 = {"sk_controller_speedball2","0"};
cvar_t	sk_controller_speedball3 = {"sk_controller_speedball3","0"};

cvar_t	sk_controller_dmgball1 = {"sk_controller_dmgball1","0"};
cvar_t	sk_controller_dmgball2 = {"sk_controller_dmgball2","0"};
cvar_t	sk_controller_dmgball3 = {"sk_controller_dmgball3","0"};

// Nihilanth
cvar_t	sk_nihilanth_health1 = {"sk_nihilanth_health1","0"};
cvar_t	sk_nihilanth_health2 = {"sk_nihilanth_health2","0"};
cvar_t	sk_nihilanth_health3 = {"sk_nihilanth_health3","0"};

cvar_t	sk_nihilanth_zap1 = {"sk_nihilanth_zap1","0"};
cvar_t	sk_nihilanth_zap2 = {"sk_nihilanth_zap2","0"};
cvar_t	sk_nihilanth_zap3 = {"sk_nihilanth_zap3","0"};

// Scientist
cvar_t	sk_scientist_health1 = {"sk_scientist_health1","0"};
cvar_t	sk_scientist_health2 = {"sk_scientist_health2","0"};
cvar_t	sk_scientist_health3 = {"sk_scientist_health3","0"};


// Snark
cvar_t	sk_snark_health1 = {"sk_snark_health1","0"};
cvar_t	sk_snark_health2 = {"sk_snark_health2","0"};
cvar_t	sk_snark_health3 = {"sk_snark_health3","0"};

cvar_t	sk_snark_dmg_bite1 = {"sk_snark_dmg_bite1","0"};
cvar_t	sk_snark_dmg_bite2 = {"sk_snark_dmg_bite2","0"};
cvar_t	sk_snark_dmg_bite3 = {"sk_snark_dmg_bite3","0"};

cvar_t	sk_snark_dmg_pop1 = {"sk_snark_dmg_pop1","0"};
cvar_t	sk_snark_dmg_pop2 = {"sk_snark_dmg_pop2","0"};
cvar_t	sk_snark_dmg_pop3 = {"sk_snark_dmg_pop3","0"};



// Zombie
cvar_t	sk_zombie_health1 = {"sk_zombie_health1","0"};
cvar_t	sk_zombie_health2 = {"sk_zombie_health2","0"};
cvar_t	sk_zombie_health3 = {"sk_zombie_health3","0"};

cvar_t	sk_zombie_dmg_one_slash1 = {"sk_zombie_dmg_one_slash1","0"};
cvar_t	sk_zombie_dmg_one_slash2 = {"sk_zombie_dmg_one_slash2","0"};
cvar_t	sk_zombie_dmg_one_slash3 = {"sk_zombie_dmg_one_slash3","0"};

cvar_t	sk_zombie_dmg_both_slash1 = {"sk_zombie_dmg_both_slash1","0"};
cvar_t	sk_zombie_dmg_both_slash2 = {"sk_zombie_dmg_both_slash2","0"};
cvar_t	sk_zombie_dmg_both_slash3 = {"sk_zombie_dmg_both_slash3","0"};


//Turret
cvar_t	sk_turret_health1 = {"sk_turret_health1","0"};
cvar_t	sk_turret_health2 = {"sk_turret_health2","0"};
cvar_t	sk_turret_health3 = {"sk_turret_health3","0"};


// MiniTurret
cvar_t	sk_miniturret_health1 = {"sk_miniturret_health1","0"};
cvar_t	sk_miniturret_health2 = {"sk_miniturret_health2","0"};
cvar_t	sk_miniturret_health3 = {"sk_miniturret_health3","0"};


// Sentry Turret
cvar_t	sk_sentry_health1 = {"sk_sentry_health1","0"};
cvar_t	sk_sentry_health2 = {"sk_sentry_health2","0"};
cvar_t	sk_sentry_health3 = {"sk_sentry_health3","0"};


// PLAYER WEAPONS

// Crowbar whack
cvar_t	sk_plr_crowbar1 = {"sk_plr_crowbar1","0"};
cvar_t	sk_plr_crowbar2 = {"sk_plr_crowbar2","0"};
cvar_t	sk_plr_crowbar3 = {"sk_plr_crowbar3","0"};

// Glock Round
cvar_t	sk_plr_9mm_bullet1 = {"sk_plr_9mm_bullet1","0"};
cvar_t	sk_plr_9mm_bullet2 = {"sk_plr_9mm_bullet2","0"};
cvar_t	sk_plr_9mm_bullet3 = {"sk_plr_9mm_bullet3","0"};

// 357 Round
cvar_t	sk_plr_357_bullet1 = {"sk_plr_357_bullet1","0"};
cvar_t	sk_plr_357_bullet2 = {"sk_plr_357_bullet2","0"};
cvar_t	sk_plr_357_bullet3 = {"sk_plr_357_bullet3","0"};

// MP5 Round
cvar_t	sk_plr_9mmAR_bullet1 = {"sk_plr_9mmAR_bullet1","0"};
cvar_t	sk_plr_9mmAR_bullet2 = {"sk_plr_9mmAR_bullet2","0"};
cvar_t	sk_plr_9mmAR_bullet3 = {"sk_plr_9mmAR_bullet3","0"};


// M203 grenade
cvar_t	sk_plr_9mmAR_grenade1 = {"sk_plr_9mmAR_grenade1","0"};
cvar_t	sk_plr_9mmAR_grenade2 = {"sk_plr_9mmAR_grenade2","0"};
cvar_t	sk_plr_9mmAR_grenade3 = {"sk_plr_9mmAR_grenade3","0"};


// Shotgun buckshot
cvar_t	sk_plr_buckshot1 = {"sk_plr_buckshot1","0"};
cvar_t	sk_plr_buckshot2 = {"sk_plr_buckshot2","0"};
cvar_t	sk_plr_buckshot3 = {"sk_plr_buckshot3","0"};


// Crossbow
cvar_t	sk_plr_xbow_bolt_client1 = {"sk_plr_xbow_bolt_client1","0"};
cvar_t	sk_plr_xbow_bolt_client2 = {"sk_plr_xbow_bolt_client2","0"};
cvar_t	sk_plr_xbow_bolt_client3 = {"sk_plr_xbow_bolt_client3","0"};

cvar_t	sk_plr_xbow_bolt_monster1 = {"sk_plr_xbow_bolt_monster1","0"};
cvar_t	sk_plr_xbow_bolt_monster2 = {"sk_plr_xbow_bolt_monster2","0"};
cvar_t	sk_plr_xbow_bolt_monster3 = {"sk_plr_xbow_bolt_monster3","0"};


// RPG
cvar_t	sk_plr_rpg1 = {"sk_plr_rpg1","0"};
cvar_t	sk_plr_rpg2 = {"sk_plr_rpg2","0"};
cvar_t	sk_plr_rpg3 = {"sk_plr_rpg3","0"};


// Zero Point Generator
cvar_t	sk_plr_gauss1 = {"sk_plr_gauss1","0"};
cvar_t	sk_plr_gauss2 = {"sk_plr_gauss2","0"};
cvar_t	sk_plr_gauss3 = {"sk_plr_gauss3","0"};


// Tau Cannon
cvar_t	sk_plr_egon_narrow1 = {"sk_plr_egon_narrow1","0"};
cvar_t	sk_plr_egon_narrow2 = {"sk_plr_egon_narrow2","0"};
cvar_t	sk_plr_egon_narrow3 = {"sk_plr_egon_narrow3","0"};

cvar_t	sk_plr_egon_wide1 = {"sk_plr_egon_wide1","0"};
cvar_t	sk_plr_egon_wide2 = {"sk_plr_egon_wide2","0"};
cvar_t	sk_plr_egon_wide3 = {"sk_plr_egon_wide3","0"};


// Hand Grendade
cvar_t	sk_plr_hand_grenade1 = {"sk_plr_hand_grenade1","0"};
cvar_t	sk_plr_hand_grenade2 = {"sk_plr_hand_grenade2","0"};
cvar_t	sk_plr_hand_grenade3 = {"sk_plr_hand_grenade3","0"};


// Satchel Charge
cvar_t	sk_plr_satchel1	= {"sk_plr_satchel1","0"};
cvar_t	sk_plr_satchel2	= {"sk_plr_satchel2","0"};
cvar_t	sk_plr_satchel3	= {"sk_plr_satchel3","0"};


// Tripmine
cvar_t	sk_plr_tripmine1 = {"sk_plr_tripmine1","0"};
cvar_t	sk_plr_tripmine2 = {"sk_plr_tripmine2","0"};
cvar_t	sk_plr_tripmine3 = {"sk_plr_tripmine3","0"};


// WORLD WEAPONS
cvar_t	sk_12mm_bullet1 = {"sk_12mm_bullet1","0"};
cvar_t	sk_12mm_bullet2 = {"sk_12mm_bullet2","0"};
cvar_t	sk_12mm_bullet3 = {"sk_12mm_bullet3","0"};

cvar_t	sk_9mmAR_bullet1 = {"sk_9mmAR_bullet1","0"};
cvar_t	sk_9mmAR_bullet2 = {"sk_9mmAR_bullet2","0"};
cvar_t	sk_9mmAR_bullet3 = {"sk_9mmAR_bullet3","0"};

cvar_t	sk_9mm_bullet1 = {"sk_9mm_bullet1","0"};
cvar_t	sk_9mm_bullet2 = {"sk_9mm_bullet2","0"};
cvar_t	sk_9mm_bullet3 = {"sk_9mm_bullet3","0"};


// HORNET
cvar_t	sk_hornet_dmg1 = {"sk_hornet_dmg1","0"};
cvar_t	sk_hornet_dmg2 = {"sk_hornet_dmg2","0"};
cvar_t	sk_hornet_dmg3 = {"sk_hornet_dmg3","0"};

// HEALTH/CHARGE
cvar_t	sk_suitcharger1	= { "sk_suitcharger1","0" };
cvar_t	sk_suitcharger2	= { "sk_suitcharger2","0" };		
cvar_t	sk_suitcharger3	= { "sk_suitcharger3","0" };		

cvar_t	sk_battery1	= { "sk_battery1","0" };			
cvar_t	sk_battery2	= { "sk_battery2","0" };			
cvar_t	sk_battery3	= { "sk_battery3","0" };			

cvar_t	sk_healthcharger1	= { "sk_healthcharger1","0" };		
cvar_t	sk_healthcharger2	= { "sk_healthcharger2","0" };		
cvar_t	sk_healthcharger3	= { "sk_healthcharger3","0" };		

cvar_t	sk_healthkit1	= { "sk_healthkit1","0" };		
cvar_t	sk_healthkit2	= { "sk_healthkit2","0" };		
cvar_t	sk_healthkit3	= { "sk_healthkit3","0" };		

cvar_t	sk_scientist_heal1	= { "sk_scientist_heal1","0" };	
cvar_t	sk_scientist_heal2	= { "sk_scientist_heal2","0" };	
cvar_t	sk_scientist_heal3	= { "sk_scientist_heal3","0" };	


// monster damage adjusters
cvar_t	sk_monster_head1	= { "sk_monster_head1","2" };
cvar_t	sk_monster_head2	= { "sk_monster_head2","2" };
cvar_t	sk_monster_head3	= { "sk_monster_head3","2" };

cvar_t	sk_monster_chest1	= { "sk_monster_chest1","1" };
cvar_t	sk_monster_chest2	= { "sk_monster_chest2","1" };
cvar_t	sk_monster_chest3	= { "sk_monster_chest3","1" };

cvar_t	sk_monster_stomach1	= { "sk_monster_stomach1","1" };
cvar_t	sk_monster_stomach2	= { "sk_monster_stomach2","1" };
cvar_t	sk_monster_stomach3	= { "sk_monster_stomach3","1" };

cvar_t	sk_monster_arm1	= { "sk_monster_arm1","1" };
cvar_t	sk_monster_arm2	= { "sk_monster_arm2","1" };
cvar_t	sk_monster_arm3	= { "sk_monster_arm3","1" };

cvar_t	sk_monster_leg1	= { "sk_monster_leg1","1" };
cvar_t	sk_monster_leg2	= { "sk_monster_leg2","1" };
cvar_t	sk_monster_leg3	= { "sk_monster_leg3","1" };

// player damage adjusters
cvar_t	sk_player_head1	= { "sk_player_head1","2" };
cvar_t	sk_player_head2	= { "sk_player_head2","2" };
cvar_t	sk_player_head3	= { "sk_player_head3","2" };

cvar_t	sk_player_chest1	= { "sk_player_chest1","1" };
cvar_t	sk_player_chest2	= { "sk_player_chest2","1" };
cvar_t	sk_player_chest3	= { "sk_player_chest3","1" };

cvar_t	sk_player_stomach1	= { "sk_player_stomach1","1" };
cvar_t	sk_player_stomach2	= { "sk_player_stomach2","1" };
cvar_t	sk_player_stomach3	= { "sk_player_stomach3","1" };

cvar_t	sk_player_arm1	= { "sk_player_arm1","1" };
cvar_t	sk_player_arm2	= { "sk_player_arm2","1" };
cvar_t	sk_player_arm3	= { "sk_player_arm3","1" };

cvar_t	sk_player_leg1	= { "sk_player_leg1","1" };
cvar_t	sk_player_leg2	= { "sk_player_leg2","1" };
cvar_t	sk_player_leg3	= { "sk_player_leg3","1" };

// END Cvars for Skill Level settings

// Oz Cvars Begin
cvar_t	suitchargercapacity	= {"suitcharger_capacity","30" };
cvar_t	healthchargercapacity = {"healthcharger_capacity","50" };
cvar_t	healthchargerrecharge	 = {"healthcharger_rechargetime","60" };
cvar_t	suitchargerrecharge	 = {"suitcharger_rechargetime","30" };
cvar_t	healthchargeramount	 = {"healthcharger_amount","1" };
cvar_t	suitchargeramount	 = {"suitcharger_amount","1" };
cvar_t	healthkit = {"healthkit","15" };
cvar_t	battery	= {"battery","15" };
cvar_t	starthealth	= {"start_health","100" };
cvar_t	startcharge	= {"start_charge","0" };
cvar_t	maxhealth	= {"max_health","100" };
cvar_t	maxcharge	= {"max_charge","100" };
cvar_t	jumpheight	= {"jump_height","1" };
cvar_t	fallingdamage	= {"damage_falling","0" };
cvar_t	startlongjump	= {"start_longjump","0" };

cvar_t	headdamage	= {"head_damage","2" };
cvar_t	chestdamage	= {"chest_damage","1" };
cvar_t	stomachdamage	= {"stomach_damage","1" };
cvar_t	armdamage	= {"arm_damage","1" };
cvar_t	legdamage	= {"leg_damage","1" };

cvar_t	bloodamount	= {"blood_amount","2" };

cvar_t	grappleairspeed		= {"grapple_airspeed","1500" };
cvar_t	grapplewaterspeed	= {"grapple_waterspeed","1000" };
cvar_t	grappledragspeed	= {"grapple_dragspeed","700" };
cvar_t	grappledisable		= {"disable_grapple","0" };
cvar_t	grappledamage		= {"damage_grapple","5" };
cvar_t	grappleglow			= {"grapple_glow","1" };

cvar_t	disableglock	= {"disable_glock","0" };
cvar_t	disable357		= {"disable_357","0" };
cvar_t	disablemp5		= {"disable_mp5","0" };
cvar_t	disablem203		= {"disable_m203","0" };
cvar_t	disableshotgun	= {"disable_shotgun","0" };
cvar_t	disablexbow		= {"disable_xbow","0" };
cvar_t	disablerpg		= {"disable_rpg","0" };
cvar_t	disablegauss	= {"disable_gauss","0" };
cvar_t	disableegon		= {"disable_egon","0" };
cvar_t	disablehivehand	= {"disable_hivehand","0" };
cvar_t	disablehgrenade	= {"disable_hgrenade","0" };
cvar_t	disablesatchel	= {"disable_satchel","0" };
cvar_t	disabletripmine	= {"disable_tripmine","0" };
cvar_t	disablesnark	= {"disable_snark","0" };

cvar_t	startglock	= {"start_glock","1" };
cvar_t	start357	= {"start_357","0" };
cvar_t	startmp5	= {"start_mp5","0" };
cvar_t	startshotgun	= {"start_shotgun","0" };
cvar_t	startxbow	= {"start_xbow","0" };
cvar_t	startrpg	= {"start_rpg","0" };
cvar_t	startgauss	= {"start_gauss","0" };
cvar_t	startegon	= {"start_egon","0" };
cvar_t	starthivehand	= {"start_hivehand","0" };
cvar_t	start9mm	= {"start_9mm","50" };
cvar_t	startshells	= {"start_shells","0" };
cvar_t	start357rounds	= {"start_357rounds","0" };
cvar_t	startm203	= {"start_m203","0" };
cvar_t	starturanium	= {"start_uranium","0" };
cvar_t	startrockets	= {"start_rockets","0" };
cvar_t	startxbowbolts	= {"start_xbowbolts","0" };
cvar_t	starthgrenades	= {"start_hgrenade","0" };
cvar_t	startsatchels	= {"start_satchel","0" };
cvar_t	starttripmines	= {"start_tripmine","0" };
cvar_t	startsnarks	= {"start_snark","0" };

cvar_t	maxuranium		= {"max_uranium","100" };
cvar_t	max9mm			= {"max_9mm","250" };
cvar_t	maxshells		= {"max_shells","125" };
cvar_t	max357rounds	= {"max_357rounds","36" };
cvar_t	maxm203			= {"max_m203","10" };
cvar_t	maxrockets		= {"max_rockets","5" };
cvar_t	maxsatchels		= {"max_satchels","5" };
cvar_t	maxtripmines	= {"max_tripmines","5" };
cvar_t	maxhgrenades	= {"max_hgrenades","10" };
cvar_t	maxsnarks		= {"max_snarks","10" };
cvar_t	maxhornets		= {"max_hornets","8" };
cvar_t	maxxbowbolts	= {"max_xbowbolts","50" };

cvar_t	giveuranium		= {"item_uranium","20" };
cvar_t	givechainbox	= {"item_chainbox","200" };
cvar_t	giveshells		= {"item_shells","12" };
cvar_t	give357rounds	= {"item_357rounds","6" };
cvar_t	givem203		= {"item_m203","2" };
cvar_t	giveglockclip	= {"item_glockclip","17" };
cvar_t	givemp5clip		= {"item_mp5clip","50" };
cvar_t	giverpgclip		= {"item_rpgclip","2" };
cvar_t	givexbowbolts	= {"item_xbowclip","5" };

cvar_t	givewithmp5			= {"ammo_mp5","50" };
cvar_t	givewithrpg			= {"ammo_rpg","2" };
cvar_t	givewithglock		= {"ammo_glock","17" };
cvar_t	givewithshotgun		= {"ammo_shotgun","12" };
cvar_t	givewithgauss		= {"ammo_gauss","20" };
cvar_t	givewithegon		= {"ammo_egon","20" };
cvar_t	givewithhgrenade	= {"ammo_hgrenade","5" };
cvar_t	givewithsatchel		= {"ammo_satchel","1" };
cvar_t	givewithtripmine	= {"ammo_tripmine","1" };
cvar_t	givewithxbow		= {"ammo_xbow","5" };
cvar_t	givewithm203		= {"ammo_m203","0" };
cvar_t	givewithsnark		= {"ammo_snark","5" };
cvar_t	givewithhivehand	= {"ammo_hivehand","8" };
cvar_t	givewith357			= {"ammo_357","6" };

cvar_t	magazineglock	= {"clip_glock","17" };
cvar_t	magazine357	= {"clip_357","6" };
cvar_t	magazinemp5	= {"clip_mp5","50" };
cvar_t	magazineshotgun	= {"clip_shotgun","8" };
cvar_t	magazinexbow	= {"clip_xbow","5" };

cvar_t	damagecrowbar		= {"damage_crowbar","25" };
cvar_t	damageglock			= {"damage_glock","12" };
cvar_t	damageglocksniper	= {"damage_glock_sniper","12" };
cvar_t	damage357			= {"damage_357","25" };
cvar_t	damage357sniper		= {"damage_357_sniper","25" };
cvar_t	damagemp5			= {"damage_mp5","12" };
cvar_t	damagemp5sniper		= {"damage_mp5_sniper","12" };
cvar_t	damageshotgun		= {"damage_shotgun_pellet","20" };
cvar_t	damagegauss			= {"damage_gauss","20" };
cvar_t	damagegaussmax		= {"damage_gaussmax","200" };
cvar_t	damageegonwide		= {"damage_egonwide","20" };
cvar_t	damageegonnarrow	= {"damage_egonnarrow","10" };
cvar_t	damagerpg			= {"damage_rpg","120" };
cvar_t	damagem203			= {"damage_m203","100" };
cvar_t	damagexbow			= {"damage_xbow","20" };
cvar_t	damagexbowexplode	= {"damage_xbowexplode","40" };
cvar_t	damagexbowsniper	= {"damage_xbow_sniper","120" };
cvar_t	damagehgrenade		= {"damage_hgrenade","100" };
cvar_t	damagehornet		= {"damage_hornet","10" };
cvar_t	damagesatchel		= {"damage_satchel","120" };
cvar_t	damagetripmine		= {"damage_tripmine","150" };
cvar_t	damagesnarkbite		= {"damage_snarkbite","10" };
cvar_t	damagesnarkpop		= {"damage_snarkpop","30" };

cvar_t	speedhgrenade	= {"speed_hgrenade","4" };
cvar_t	speedm203		= {"speed_m203","800" };
cvar_t	speedrpgwater	= {"speed_rpg_water","300" };
cvar_t	speedrpgair		= {"speed_rpg_air","2000" };
cvar_t	speedsatchel	= {"speed_satchel","274" };
cvar_t	speedxbowair	= {"speed_xbow_air","2000" };
cvar_t	speedxbowwater	= {"speed_xbow_water","1000" };
cvar_t	speedhornet		= {"speed_hornet_red","600" };
cvar_t	speedhornet2	= {"speed_hornet_orange","800" };

cvar_t	mp5tracers	= {"mp5_tracers","2" };
cvar_t	mp5spread	= {"mp5_spread","6" };
cvar_t	mp5sniper	= {"mp5_sniper","0" };
cvar_t	silencemp5	= {"mp5_silence","0" };
cvar_t	glocksniper	= {"glock_sniper","0" };
cvar_t	silenceglock			= {"glock_silence","0" };
cvar_t	pelletsshotgun			= {"shotgun_pellets","4" };
cvar_t	pelletsdoubleshotgun	= {"shotgun2_pellets","8" };
cvar_t	snarkhealth		= {"snark_health","10" };
cvar_t	snarktime		= {"snark_time","15" };
cvar_t	itemrespawntime	= {"respawntime_item","30" };
cvar_t	ammorespawntime	= {"respawntime_ammo","20" };
cvar_t	weaponrespawntime	= {"respawntime_weapon","20" };
cvar_t	gaussmaxchargetime	= {"gauss_maxchargetime","10" };
cvar_t	nightops	= {"night_ops","0" };
cvar_t	lightlevel	= {"light_level","3" };
cvar_t	autoswitch	= {"auto_switch","1" };
cvar_t	grenadetime	= {"grenade_time","3" };
cvar_t	flaretime	= {"flare_time","45" };
cvar_t	waterweapons	= {"water_weapons","0" };
cvar_t	randomspawn		= {"random_spawn","0" };

cvar_t	colorredrpg		= {"color_red_rpg","224" };
cvar_t	colorgreenrpg	= {"color_green_rpg","255" };
cvar_t	colorbluerpg	= {"color_blue_rpg","224" };

cvar_t	colorredgrapple		= {"color_red_grapple","50" };
cvar_t	colorgreengrapple	= {"color_green_grapple","100" };
cvar_t	colorbluegrapple	= {"color_blue_grapple","200" };

cvar_t	colorredhornet1		= {"color_red_hornet1","179" };
cvar_t	colorgreenhornet1	= {"color_green_hornet1","39" };
cvar_t	colorbluehornet1	= {"color_blue_hornet1","14" };

cvar_t	colorredhornet2		= {"color_red_hornet2","255" };
cvar_t	colorgreenhornet2	= {"color_green_hornet2","128" };
cvar_t	colorbluehornet2	= {"color_blue_hornet2","0" };

cvar_t	colorredgaussprimary	= {"color_red_gaussprimary","255" };
cvar_t	colorgreengaussprimary	= {"color_green_gaussprimary","128" };
cvar_t	colorbluegaussprimary	= {"color_blue_gaussprimary","0" };

cvar_t	colorredgausssecondary  	= {"color_red_gausssecondary","255" };
cvar_t	colorgreengausssecondary	= {"color_green_gausssecondary","255" };
cvar_t	colorbluegausssecondary		= {"color_blue_gausssecondary","255" };

cvar_t	colorredtripmine		= {"color_red_tripmine","0" };
cvar_t	colorgreentripmine	= {"color_green_tripmine","214" };
cvar_t	colorbluetripmine	= {"color_blue_tripmine","198" };

cvar_t	recoilglock		 = {"recoil_glock","2" };
cvar_t	recoilmp5		 = {"recoil_mp5","2" };
cvar_t	recoilm203		 = {"recoil_m203","10" };
cvar_t	recoilxbow		 = {"recoil_xbow","2" };
cvar_t	recoilrpg		 = {"recoil_rpg","5" };
cvar_t	recoil357		 = {"recoil_357","10" };
cvar_t	recoilshotgun	 = {"recoil_shotgun","5" };
cvar_t	recoilshotgun2	 = {"recoil_shotgun2","10" };
cvar_t	recoilhivehand	 = {"recoil_hivehand","2" };
cvar_t	recoilgauss		 = {"recoil_gauss","2" };

cvar_t	cyclecrowbar	 = {"cycle_crowbar_first",".5" };
cvar_t	cyclecrowbar2	 = {"cycle_crowbar_after",".25" };
cvar_t	cycleglock		 = {"cycle_glock",".3" };
cvar_t	cycleglock2		 = {"cycle_glock2",".2" };
cvar_t	cyclemp5		 = {"cycle_mp5",".1" };
cvar_t	cyclemp5sniper	 = {"cycle_mp5_sniper",".3" };
cvar_t	cycle357		 = {"cycle_357",".75" };
cvar_t	cycleshotgun	 = {"cycle_shotgun",".75" };
cvar_t	cycleshotgun2	 = {"cycle_shotgun2","1.5" };
cvar_t	cyclegauss		 = {"cycle_gauss",".2" };
cvar_t	cyclem203		 = {"cycle_m203","1" };
cvar_t	cyclexbow		 = {"cycle_xbow",".75" };
cvar_t	cyclexbow2		 = {"cycle_xbow_sniper",".75" };
cvar_t	cyclehivehand	 = {"cycle_hivehand",".25" };
cvar_t	cyclehivehand2	 = {"cycle_hivehand2",".1" };

cvar_t	pointsperkill		 = {"points_per_kill","1" };
cvar_t	pointspersuicide	 = {"points_per_suicide","1" };
cvar_t	pointsperworldkill	 = {"points_per_worldkill","1" };
cvar_t	pointsperteamkill	 = {"points_per_teamkill","1" };
cvar_t	pointsperdeath		 = {"points_per_death","1" };

cvar_t	playerair	 = {"player_air","12" };

cvar_t	weightcrowbar	 = {"weight_crowbar","0" };
cvar_t	weightglock	 = {"weight_glock","10" };
cvar_t	weight357	 = {"weight_357","15" };
cvar_t	weightmp5	 = {"weight_mp5","15" };
cvar_t	weightshotgun	 = {"weight_shotgun","0" };
cvar_t	weightxbow	 = {"weight_xbow","10" };
cvar_t	weightrpg	 = {"weight_rpg","20" };
cvar_t	weightgauss	 = {"weight_gauss","20" };
cvar_t	weightegon	 = {"weight_egon","20" };
cvar_t	weighthivehand	 = {"weight_hivehand","10" };
cvar_t	weighthgrenade	 = {"weight_hgrenade","5" };
cvar_t	weightsnark	 = {"weight_snark","5" };
cvar_t	weightsatchel	 = {"weight_satchel","-10" };
cvar_t	weighttripmine	 = {"weight_tripmine","-10" };

cvar_t	randomgrapplecolor	= {"random_grapple_color","1" };

cvar_t	respawntimerune		= {"respawntime_rune","30" };
cvar_t	fragruneamount		= {"frag_rune_value","1" };
cvar_t	regenrunehealth		= {"regen_rune_health","1" };
cvar_t	regenrunearmor		= {"regen_rune_armor","1" };
cvar_t	superhookrunedamage	= {"superhook_rune_damage","25" };
cvar_t	lowgravruneamount	= {"lowgravity_rune_value",".2" };

cvar_t	reloadglock		 = {"reload_glock","1.5" };
cvar_t	reload357		 = {"reload_357","2" };
cvar_t	reloadmp5		 = {"reload_mp5","1.5" };
cvar_t	reloadshotgun	 = {"shotgun_shells_per_reload","1" };
cvar_t	reloadxbow		 = {"reload_xbow","4.5" };
cvar_t	reloadrpg		 = {"reload_rpg","2" };

cvar_t	playerid		 = {"player_id","1" };

cvar_t	fragrunenumber		= {"number_of_frag_runes","1" };
cvar_t	regenrunenumber		= {"number_of_regen_runes","1" };
cvar_t	crowbarrunenumber	= {"number_of_crowbar_runes","1" };
cvar_t	hasterunenumber		= {"number_of_haste_runes","1" };
cvar_t	cloakrunenumber		= {"number_of_cloak_runes","1" };
cvar_t	hookrunenumber		= {"number_of_superhook_runes","1" };
cvar_t	protectrunenumber	= {"number_of_protection_runes","1" };
cvar_t	lowgravrunenumber	= {"number_of_lowgravity_runes","1" };
cvar_t	vamprunenumber	= {"number_of_vampire_runes","1" };
cvar_t	traprunenumber	= {"number_of_boobytrap_runes","1" };
//jammer superglock
cvar_t superglockrunenumber = {"number_of_superglock_runes","1"};

cvar_t	zoomamountglock		= {"zoom_fov_glock","40" };
cvar_t	zoomamountmp5		= {"zoom_fov_mp5","30" };
cvar_t	zoomamountxbow		= {"zoom_fov_xbow","20" };

cvar_t	maxhealthwithvamprune		= {"max_health_with_vampire_rune","150" };
cvar_t	boobytrapdamage				= {"damage_boobytrap_rune","150" };

cvar_t	infinity		= {"infinity_dm_map_configs","0" };
cvar_t	infinityctf		= {"infinity_ctf_map_configs","1" };

cvar_t	homing			= {"rpg_homing","0" };

cvar_t	ctf			= {"mp_ctf","0", FCVAR_SERVER };
cvar_t	ctfteamlist	= {"ctf_teamlist","hgrunt;scientist", FCVAR_SERVER };
cvar_t	ctfflagscore= {"points_per_flag_capture","10", FCVAR_SERVER };
cvar_t	ctfstealscore= {"points_per_flag_steal","1", FCVAR_SERVER };
cvar_t	ctfreturnscore= {"points_per_flag_return","1", FCVAR_SERVER };


cvar_t	damagehomingrpg	= {"damage_rpg_homing","50" };

// Oz Cvars End

// Register your console variables here
// This gets called one time when the game is initialied

void GameDLLInit( void )
{
	// Register cvars here:

	CVAR_REGISTER (&displaysoundlist);
	CVAR_REGISTER (&mapcyclefile);
	CVAR_REGISTER (&servercfgfile);
	CVAR_REGISTER (&lservercfgfile);

	CVAR_REGISTER (&teamplay);
	CVAR_REGISTER (&fraglimit);
	CVAR_REGISTER (&timelimit);

	CVAR_REGISTER (&friendlyfire);
	CVAR_REGISTER (&falldamage);
	CVAR_REGISTER (&weaponstay);
	CVAR_REGISTER (&forcerespawn);
	CVAR_REGISTER (&footsteps);
	CVAR_REGISTER (&flashlight);
	CVAR_REGISTER (&aimcrosshair);
	CVAR_REGISTER (&decalfrequency);
	CVAR_REGISTER (&teamlist);
	CVAR_REGISTER (&teamoverride);
	CVAR_REGISTER (&defaultteam);
	CVAR_REGISTER (&allowmonsters);

// REGISTER CVARS FOR SKILL LEVEL STUFF
	// Agrunt
	CVAR_REGISTER ( &sk_agrunt_health1 );// {"sk_agrunt_health1","0"};
	CVAR_REGISTER ( &sk_agrunt_health2 );// {"sk_agrunt_health2","0"};
	CVAR_REGISTER ( &sk_agrunt_health3 );// {"sk_agrunt_health3","0"};

	CVAR_REGISTER ( &sk_agrunt_dmg_punch1 );// {"sk_agrunt_dmg_punch1","0"};
	CVAR_REGISTER ( &sk_agrunt_dmg_punch2 );// {"sk_agrunt_dmg_punch2","0"};
	CVAR_REGISTER ( &sk_agrunt_dmg_punch3 );// {"sk_agrunt_dmg_punch3","0"};

	// Apache
	CVAR_REGISTER ( &sk_apache_health1 );// {"sk_apache_health1","0"};
	CVAR_REGISTER ( &sk_apache_health2 );// {"sk_apache_health2","0"};
	CVAR_REGISTER ( &sk_apache_health3 );// {"sk_apache_health3","0"};

	// Barney
	CVAR_REGISTER ( &sk_barney_health1 );// {"sk_barney_health1","0"};
	CVAR_REGISTER ( &sk_barney_health2 );// {"sk_barney_health2","0"};
	CVAR_REGISTER ( &sk_barney_health3 );// {"sk_barney_health3","0"};

	// Bullsquid
	CVAR_REGISTER ( &sk_bullsquid_health1 );// {"sk_bullsquid_health1","0"};
	CVAR_REGISTER ( &sk_bullsquid_health2 );// {"sk_bullsquid_health2","0"};
	CVAR_REGISTER ( &sk_bullsquid_health3 );// {"sk_bullsquid_health3","0"};

	CVAR_REGISTER ( &sk_bullsquid_dmg_bite1 );// {"sk_bullsquid_dmg_bite1","0"};
	CVAR_REGISTER ( &sk_bullsquid_dmg_bite2 );// {"sk_bullsquid_dmg_bite2","0"};
	CVAR_REGISTER ( &sk_bullsquid_dmg_bite3 );// {"sk_bullsquid_dmg_bite3","0"};

	CVAR_REGISTER ( &sk_bullsquid_dmg_whip1 );// {"sk_bullsquid_dmg_whip1","0"};
	CVAR_REGISTER ( &sk_bullsquid_dmg_whip2 );// {"sk_bullsquid_dmg_whip2","0"};
	CVAR_REGISTER ( &sk_bullsquid_dmg_whip3 );// {"sk_bullsquid_dmg_whip3","0"};

	CVAR_REGISTER ( &sk_bullsquid_dmg_spit1 );// {"sk_bullsquid_dmg_spit1","0"};
	CVAR_REGISTER ( &sk_bullsquid_dmg_spit2 );// {"sk_bullsquid_dmg_spit2","0"};
	CVAR_REGISTER ( &sk_bullsquid_dmg_spit3 );// {"sk_bullsquid_dmg_spit3","0"};


	CVAR_REGISTER ( &sk_bigmomma_health_factor1 );// {"sk_bigmomma_health_factor1","1.0"};
	CVAR_REGISTER ( &sk_bigmomma_health_factor2 );// {"sk_bigmomma_health_factor2","1.0"};
	CVAR_REGISTER ( &sk_bigmomma_health_factor3 );// {"sk_bigmomma_health_factor3","1.0"};

	CVAR_REGISTER ( &sk_bigmomma_dmg_slash1 );// {"sk_bigmomma_dmg_slash1","50"};
	CVAR_REGISTER ( &sk_bigmomma_dmg_slash2 );// {"sk_bigmomma_dmg_slash2","50"};
	CVAR_REGISTER ( &sk_bigmomma_dmg_slash3 );// {"sk_bigmomma_dmg_slash3","50"};

	CVAR_REGISTER ( &sk_bigmomma_dmg_blast1 );// {"sk_bigmomma_dmg_blast1","100"};
	CVAR_REGISTER ( &sk_bigmomma_dmg_blast2 );// {"sk_bigmomma_dmg_blast2","100"};
	CVAR_REGISTER ( &sk_bigmomma_dmg_blast3 );// {"sk_bigmomma_dmg_blast3","100"};

	CVAR_REGISTER ( &sk_bigmomma_radius_blast1 );// {"sk_bigmomma_radius_blast1","250"};
	CVAR_REGISTER ( &sk_bigmomma_radius_blast2 );// {"sk_bigmomma_radius_blast2","250"};
	CVAR_REGISTER ( &sk_bigmomma_radius_blast3 );// {"sk_bigmomma_radius_blast3","250"};

	// Gargantua
	CVAR_REGISTER ( &sk_gargantua_health1 );// {"sk_gargantua_health1","0"};
	CVAR_REGISTER ( &sk_gargantua_health2 );// {"sk_gargantua_health2","0"};
	CVAR_REGISTER ( &sk_gargantua_health3 );// {"sk_gargantua_health3","0"};

	CVAR_REGISTER ( &sk_gargantua_dmg_slash1 );// {"sk_gargantua_dmg_slash1","0"};
	CVAR_REGISTER ( &sk_gargantua_dmg_slash2 );// {"sk_gargantua_dmg_slash2","0"};
	CVAR_REGISTER ( &sk_gargantua_dmg_slash3 );// {"sk_gargantua_dmg_slash3","0"};

	CVAR_REGISTER ( &sk_gargantua_dmg_fire1 );// {"sk_gargantua_dmg_fire1","0"};
	CVAR_REGISTER ( &sk_gargantua_dmg_fire2 );// {"sk_gargantua_dmg_fire2","0"};
	CVAR_REGISTER ( &sk_gargantua_dmg_fire3 );// {"sk_gargantua_dmg_fire3","0"};

	CVAR_REGISTER ( &sk_gargantua_dmg_stomp1 );// {"sk_gargantua_dmg_stomp1","0"};
	CVAR_REGISTER ( &sk_gargantua_dmg_stomp2 );// {"sk_gargantua_dmg_stomp2","0"};
	CVAR_REGISTER ( &sk_gargantua_dmg_stomp3	);// {"sk_gargantua_dmg_stomp3","0"};


	// Hassassin
	CVAR_REGISTER ( &sk_hassassin_health1 );// {"sk_hassassin_health1","0"};
	CVAR_REGISTER ( &sk_hassassin_health2 );// {"sk_hassassin_health2","0"};
	CVAR_REGISTER ( &sk_hassassin_health3 );// {"sk_hassassin_health3","0"};


	// Headcrab
	CVAR_REGISTER ( &sk_headcrab_health1 );// {"sk_headcrab_health1","0"};
	CVAR_REGISTER ( &sk_headcrab_health2 );// {"sk_headcrab_health2","0"};
	CVAR_REGISTER ( &sk_headcrab_health3 );// {"sk_headcrab_health3","0"};

	CVAR_REGISTER ( &sk_headcrab_dmg_bite1 );// {"sk_headcrab_dmg_bite1","0"};
	CVAR_REGISTER ( &sk_headcrab_dmg_bite2 );// {"sk_headcrab_dmg_bite2","0"};
	CVAR_REGISTER ( &sk_headcrab_dmg_bite3 );// {"sk_headcrab_dmg_bite3","0"};


	// Hgrunt 
	CVAR_REGISTER ( &sk_hgrunt_health1 );// {"sk_hgrunt_health1","0"};
	CVAR_REGISTER ( &sk_hgrunt_health2 );// {"sk_hgrunt_health2","0"};
	CVAR_REGISTER ( &sk_hgrunt_health3 );// {"sk_hgrunt_health3","0"};

	CVAR_REGISTER ( &sk_hgrunt_kick1 );// {"sk_hgrunt_kick1","0"};
	CVAR_REGISTER ( &sk_hgrunt_kick2 );// {"sk_hgrunt_kick2","0"};
	CVAR_REGISTER ( &sk_hgrunt_kick3 );// {"sk_hgrunt_kick3","0"};

	CVAR_REGISTER ( &sk_hgrunt_pellets1 );
	CVAR_REGISTER ( &sk_hgrunt_pellets2 );
	CVAR_REGISTER ( &sk_hgrunt_pellets3 );

	CVAR_REGISTER ( &sk_hgrunt_gspeed1 );
	CVAR_REGISTER ( &sk_hgrunt_gspeed2 );
	CVAR_REGISTER ( &sk_hgrunt_gspeed3 );

	// Houndeye
	CVAR_REGISTER ( &sk_houndeye_health1 );// {"sk_houndeye_health1","0"};
	CVAR_REGISTER ( &sk_houndeye_health2 );// {"sk_houndeye_health2","0"};
	CVAR_REGISTER ( &sk_houndeye_health3 );// {"sk_houndeye_health3","0"};

	CVAR_REGISTER ( &sk_houndeye_dmg_blast1 );// {"sk_houndeye_dmg_blast1","0"};
	CVAR_REGISTER ( &sk_houndeye_dmg_blast2 );// {"sk_houndeye_dmg_blast2","0"};
	CVAR_REGISTER ( &sk_houndeye_dmg_blast3 );// {"sk_houndeye_dmg_blast3","0"};


	// ISlave
	CVAR_REGISTER ( &sk_islave_health1 );// {"sk_islave_health1","0"};
	CVAR_REGISTER ( &sk_islave_health2 );// {"sk_islave_health2","0"};
	CVAR_REGISTER ( &sk_islave_health3 );// {"sk_islave_health3","0"};

	CVAR_REGISTER ( &sk_islave_dmg_claw1 );// {"sk_islave_dmg_claw1","0"};
	CVAR_REGISTER ( &sk_islave_dmg_claw2 );// {"sk_islave_dmg_claw2","0"};
	CVAR_REGISTER ( &sk_islave_dmg_claw3 );// {"sk_islave_dmg_claw3","0"};

	CVAR_REGISTER ( &sk_islave_dmg_clawrake1	);// {"sk_islave_dmg_clawrake1","0"};
	CVAR_REGISTER ( &sk_islave_dmg_clawrake2	);// {"sk_islave_dmg_clawrake2","0"};
	CVAR_REGISTER ( &sk_islave_dmg_clawrake3	);// {"sk_islave_dmg_clawrake3","0"};
		
	CVAR_REGISTER ( &sk_islave_dmg_zap1 );// {"sk_islave_dmg_zap1","0"};
	CVAR_REGISTER ( &sk_islave_dmg_zap2 );// {"sk_islave_dmg_zap2","0"};
	CVAR_REGISTER ( &sk_islave_dmg_zap3 );// {"sk_islave_dmg_zap3","0"};


	// Icthyosaur
	CVAR_REGISTER ( &sk_ichthyosaur_health1	);// {"sk_ichthyosaur_health1","0"};
	CVAR_REGISTER ( &sk_ichthyosaur_health2	);// {"sk_ichthyosaur_health2","0"};
	CVAR_REGISTER ( &sk_ichthyosaur_health3	);// {"sk_ichthyosaur_health3","0"};

	CVAR_REGISTER ( &sk_ichthyosaur_shake1	);// {"sk_ichthyosaur_health3","0"};
	CVAR_REGISTER ( &sk_ichthyosaur_shake2	);// {"sk_ichthyosaur_health3","0"};
	CVAR_REGISTER ( &sk_ichthyosaur_shake3	);// {"sk_ichthyosaur_health3","0"};



	// Leech
	CVAR_REGISTER ( &sk_leech_health1 );// {"sk_leech_health1","0"};
	CVAR_REGISTER ( &sk_leech_health2 );// {"sk_leech_health2","0"};
	CVAR_REGISTER ( &sk_leech_health3 );// {"sk_leech_health3","0"};

	CVAR_REGISTER ( &sk_leech_dmg_bite1 );// {"sk_leech_dmg_bite1","0"};
	CVAR_REGISTER ( &sk_leech_dmg_bite2 );// {"sk_leech_dmg_bite2","0"};
	CVAR_REGISTER ( &sk_leech_dmg_bite3 );// {"sk_leech_dmg_bite3","0"};


	// Controller
	CVAR_REGISTER ( &sk_controller_health1 );
	CVAR_REGISTER ( &sk_controller_health2 );
	CVAR_REGISTER ( &sk_controller_health3 );

	CVAR_REGISTER ( &sk_controller_dmgzap1 );
	CVAR_REGISTER ( &sk_controller_dmgzap2 );
	CVAR_REGISTER ( &sk_controller_dmgzap3 );

	CVAR_REGISTER ( &sk_controller_speedball1 );
	CVAR_REGISTER ( &sk_controller_speedball2 );
	CVAR_REGISTER ( &sk_controller_speedball3 );

	CVAR_REGISTER ( &sk_controller_dmgball1 );
	CVAR_REGISTER ( &sk_controller_dmgball2 );
	CVAR_REGISTER ( &sk_controller_dmgball3 );

	// Nihilanth
	CVAR_REGISTER ( &sk_nihilanth_health1 );// {"sk_nihilanth_health1","0"};
	CVAR_REGISTER ( &sk_nihilanth_health2 );// {"sk_nihilanth_health2","0"};
	CVAR_REGISTER ( &sk_nihilanth_health3 );// {"sk_nihilanth_health3","0"};

	CVAR_REGISTER ( &sk_nihilanth_zap1 );
	CVAR_REGISTER ( &sk_nihilanth_zap2 );
	CVAR_REGISTER ( &sk_nihilanth_zap3 );

	// Scientist
	CVAR_REGISTER ( &sk_scientist_health1 );// {"sk_scientist_health1","0"};
	CVAR_REGISTER ( &sk_scientist_health2 );// {"sk_scientist_health2","0"};
	CVAR_REGISTER ( &sk_scientist_health3 );// {"sk_scientist_health3","0"};


	// Snark
	CVAR_REGISTER ( &sk_snark_health1 );// {"sk_snark_health1","0"};
	CVAR_REGISTER ( &sk_snark_health2 );// {"sk_snark_health2","0"};
	CVAR_REGISTER ( &sk_snark_health3 );// {"sk_snark_health3","0"};

	CVAR_REGISTER ( &sk_snark_dmg_bite1 );// {"sk_snark_dmg_bite1","0"};
	CVAR_REGISTER ( &sk_snark_dmg_bite2 );// {"sk_snark_dmg_bite2","0"};
	CVAR_REGISTER ( &sk_snark_dmg_bite3 );// {"sk_snark_dmg_bite3","0"};

	CVAR_REGISTER ( &sk_snark_dmg_pop1 );// {"sk_snark_dmg_pop1","0"};
	CVAR_REGISTER ( &sk_snark_dmg_pop2 );// {"sk_snark_dmg_pop2","0"};
	CVAR_REGISTER ( &sk_snark_dmg_pop3 );// {"sk_snark_dmg_pop3","0"};



	// Zombie
	CVAR_REGISTER ( &sk_zombie_health1 );// {"sk_zombie_health1","0"};
	CVAR_REGISTER ( &sk_zombie_health2 );// {"sk_zombie_health3","0"};
	CVAR_REGISTER ( &sk_zombie_health3 );// {"sk_zombie_health3","0"};

	CVAR_REGISTER ( &sk_zombie_dmg_one_slash1 );// {"sk_zombie_dmg_one_slash1","0"};
	CVAR_REGISTER ( &sk_zombie_dmg_one_slash2 );// {"sk_zombie_dmg_one_slash2","0"};
	CVAR_REGISTER ( &sk_zombie_dmg_one_slash3 );// {"sk_zombie_dmg_one_slash3","0"};

	CVAR_REGISTER ( &sk_zombie_dmg_both_slash1 );// {"sk_zombie_dmg_both_slash1","0"};
	CVAR_REGISTER ( &sk_zombie_dmg_both_slash2 );// {"sk_zombie_dmg_both_slash2","0"};
	CVAR_REGISTER ( &sk_zombie_dmg_both_slash3 );// {"sk_zombie_dmg_both_slash3","0"};


	//Turret
	CVAR_REGISTER ( &sk_turret_health1 );// {"sk_turret_health1","0"};
	CVAR_REGISTER ( &sk_turret_health2 );// {"sk_turret_health2","0"};
	CVAR_REGISTER ( &sk_turret_health3 );// {"sk_turret_health3","0"};


	// MiniTurret
	CVAR_REGISTER ( &sk_miniturret_health1 );// {"sk_miniturret_health1","0"};
	CVAR_REGISTER ( &sk_miniturret_health2 );// {"sk_miniturret_health2","0"};
	CVAR_REGISTER ( &sk_miniturret_health3 );// {"sk_miniturret_health3","0"};


	// Sentry Turret
	CVAR_REGISTER ( &sk_sentry_health1 );// {"sk_sentry_health1","0"};
	CVAR_REGISTER ( &sk_sentry_health2 );// {"sk_sentry_health2","0"};
	CVAR_REGISTER ( &sk_sentry_health3 );// {"sk_sentry_health3","0"};


	// PLAYER WEAPONS

	// Crowbar whack
	CVAR_REGISTER ( &sk_plr_crowbar1 );// {"sk_plr_crowbar1","0"};
	CVAR_REGISTER ( &sk_plr_crowbar2 );// {"sk_plr_crowbar2","0"};
	CVAR_REGISTER ( &sk_plr_crowbar3 );// {"sk_plr_crowbar3","0"};

	// Glock Round
	CVAR_REGISTER ( &sk_plr_9mm_bullet1 );// {"sk_plr_9mm_bullet1","0"};
	CVAR_REGISTER ( &sk_plr_9mm_bullet2 );// {"sk_plr_9mm_bullet2","0"};
	CVAR_REGISTER ( &sk_plr_9mm_bullet3 );// {"sk_plr_9mm_bullet3","0"};

	// 357 Round
	CVAR_REGISTER ( &sk_plr_357_bullet1 );// {"sk_plr_357_bullet1","0"};
	CVAR_REGISTER ( &sk_plr_357_bullet2 );// {"sk_plr_357_bullet2","0"};
	CVAR_REGISTER ( &sk_plr_357_bullet3 );// {"sk_plr_357_bullet3","0"};

	// MP5 Round
	CVAR_REGISTER ( &sk_plr_9mmAR_bullet1 );// {"sk_plr_9mmAR_bullet1","0"};
	CVAR_REGISTER ( &sk_plr_9mmAR_bullet2 );// {"sk_plr_9mmAR_bullet2","0"};
	CVAR_REGISTER ( &sk_plr_9mmAR_bullet3 );// {"sk_plr_9mmAR_bullet3","0"};


	// M203 grenade
	CVAR_REGISTER ( &sk_plr_9mmAR_grenade1 );// {"sk_plr_9mmAR_grenade1","0"};
	CVAR_REGISTER ( &sk_plr_9mmAR_grenade2 );// {"sk_plr_9mmAR_grenade2","0"};
	CVAR_REGISTER ( &sk_plr_9mmAR_grenade3 );// {"sk_plr_9mmAR_grenade3","0"};


	// Shotgun buckshot
	CVAR_REGISTER ( &sk_plr_buckshot1 );// {"sk_plr_buckshot1","0"};
	CVAR_REGISTER ( &sk_plr_buckshot2 );// {"sk_plr_buckshot2","0"};
	CVAR_REGISTER ( &sk_plr_buckshot3 );// {"sk_plr_buckshot3","0"};


	// Crossbow
	CVAR_REGISTER ( &sk_plr_xbow_bolt_monster1 );// {"sk_plr_xbow_bolt1","0"};
	CVAR_REGISTER ( &sk_plr_xbow_bolt_monster2 );// {"sk_plr_xbow_bolt2","0"};
	CVAR_REGISTER ( &sk_plr_xbow_bolt_monster3 );// {"sk_plr_xbow_bolt3","0"};

	CVAR_REGISTER ( &sk_plr_xbow_bolt_client1 );// {"sk_plr_xbow_bolt1","0"};
	CVAR_REGISTER ( &sk_plr_xbow_bolt_client2 );// {"sk_plr_xbow_bolt2","0"};
	CVAR_REGISTER ( &sk_plr_xbow_bolt_client3 );// {"sk_plr_xbow_bolt3","0"};


	// RPG
	CVAR_REGISTER ( &sk_plr_rpg1 );// {"sk_plr_rpg1","0"};
	CVAR_REGISTER ( &sk_plr_rpg2 );// {"sk_plr_rpg2","0"};
	CVAR_REGISTER ( &sk_plr_rpg3 );// {"sk_plr_rpg3","0"};


	// Gauss Gun
	CVAR_REGISTER ( &sk_plr_gauss1 );// {"sk_plr_gauss1","0"};
	CVAR_REGISTER ( &sk_plr_gauss2 );// {"sk_plr_gauss2","0"};
	CVAR_REGISTER ( &sk_plr_gauss3 );// {"sk_plr_gauss3","0"};


	// Egon Gun
	CVAR_REGISTER ( &sk_plr_egon_narrow1 );// {"sk_plr_egon_narrow1","0"};
	CVAR_REGISTER ( &sk_plr_egon_narrow2 );// {"sk_plr_egon_narrow2","0"};
	CVAR_REGISTER ( &sk_plr_egon_narrow3 );// {"sk_plr_egon_narrow3","0"};

	CVAR_REGISTER ( &sk_plr_egon_wide1 );// {"sk_plr_egon_wide1","0"};
	CVAR_REGISTER ( &sk_plr_egon_wide2 );// {"sk_plr_egon_wide2","0"};
	CVAR_REGISTER ( &sk_plr_egon_wide3 );// {"sk_plr_egon_wide3","0"};


	// Hand Grendade
	CVAR_REGISTER ( &sk_plr_hand_grenade1 );// {"sk_plr_hand_grenade1","0"};
	CVAR_REGISTER ( &sk_plr_hand_grenade2 );// {"sk_plr_hand_grenade2","0"};
	CVAR_REGISTER ( &sk_plr_hand_grenade3 );// {"sk_plr_hand_grenade3","0"};


	// Satchel Charge
	CVAR_REGISTER ( &sk_plr_satchel1 );// {"sk_plr_satchel1","0"};
	CVAR_REGISTER ( &sk_plr_satchel2 );// {"sk_plr_satchel2","0"};
	CVAR_REGISTER ( &sk_plr_satchel3 );// {"sk_plr_satchel3","0"};


	// Tripmine
	CVAR_REGISTER ( &sk_plr_tripmine1 );// {"sk_plr_tripmine1","0"};
	CVAR_REGISTER ( &sk_plr_tripmine2 );// {"sk_plr_tripmine2","0"};
	CVAR_REGISTER ( &sk_plr_tripmine3 );// {"sk_plr_tripmine3","0"};


	// WORLD WEAPONS
	CVAR_REGISTER ( &sk_12mm_bullet1 );// {"sk_12mm_bullet1","0"};
	CVAR_REGISTER ( &sk_12mm_bullet2 );// {"sk_12mm_bullet2","0"};
	CVAR_REGISTER ( &sk_12mm_bullet3 );// {"sk_12mm_bullet3","0"};

	CVAR_REGISTER ( &sk_9mmAR_bullet1 );// {"sk_9mm_bullet1","0"};
	CVAR_REGISTER ( &sk_9mmAR_bullet2 );// {"sk_9mm_bullet1","0"};
	CVAR_REGISTER ( &sk_9mmAR_bullet3 );// {"sk_9mm_bullet1","0"};

	CVAR_REGISTER ( &sk_9mm_bullet1 );// {"sk_9mm_bullet1","0"};
	CVAR_REGISTER ( &sk_9mm_bullet2 );// {"sk_9mm_bullet2","0"};
	CVAR_REGISTER ( &sk_9mm_bullet3 );// {"sk_9mm_bullet3","0"};


	// HORNET
	CVAR_REGISTER ( &sk_hornet_dmg1 );// {"sk_hornet_dmg1","0"};
	CVAR_REGISTER ( &sk_hornet_dmg2 );// {"sk_hornet_dmg2","0"};
	CVAR_REGISTER ( &sk_hornet_dmg3 );// {"sk_hornet_dmg3","0"};

	// HEALTH/SUIT CHARGE DISTRIBUTION
	CVAR_REGISTER ( &sk_suitcharger1 );
	CVAR_REGISTER ( &sk_suitcharger2 );
	CVAR_REGISTER ( &sk_suitcharger3 );

	CVAR_REGISTER ( &sk_battery1 );
	CVAR_REGISTER ( &sk_battery2 );
	CVAR_REGISTER ( &sk_battery3 );

	CVAR_REGISTER ( &sk_healthcharger1 );
	CVAR_REGISTER ( &sk_healthcharger2 );
	CVAR_REGISTER ( &sk_healthcharger3 );

	CVAR_REGISTER ( &sk_healthkit1 );
	CVAR_REGISTER ( &sk_healthkit2 );
	CVAR_REGISTER ( &sk_healthkit3 );

	CVAR_REGISTER ( &sk_scientist_heal1 );
	CVAR_REGISTER ( &sk_scientist_heal2 );
	CVAR_REGISTER ( &sk_scientist_heal3 );

// monster damage adjusters
	CVAR_REGISTER ( &sk_monster_head1 );
	CVAR_REGISTER ( &sk_monster_head2 );
	CVAR_REGISTER ( &sk_monster_head3 );

	CVAR_REGISTER ( &sk_monster_chest1 );
	CVAR_REGISTER ( &sk_monster_chest2 );
	CVAR_REGISTER ( &sk_monster_chest3 );

	CVAR_REGISTER ( &sk_monster_stomach1 );
	CVAR_REGISTER ( &sk_monster_stomach2 );
	CVAR_REGISTER ( &sk_monster_stomach3 );

	CVAR_REGISTER ( &sk_monster_arm1 );
	CVAR_REGISTER ( &sk_monster_arm2 );
	CVAR_REGISTER ( &sk_monster_arm3 );

	CVAR_REGISTER ( &sk_monster_leg1 );
	CVAR_REGISTER ( &sk_monster_leg2 );
	CVAR_REGISTER ( &sk_monster_leg3 );

// player damage adjusters
	CVAR_REGISTER ( &sk_player_head1 );
	CVAR_REGISTER ( &sk_player_head2 );
	CVAR_REGISTER ( &sk_player_head3 );

	CVAR_REGISTER ( &sk_player_chest1 );
	CVAR_REGISTER ( &sk_player_chest2 );
	CVAR_REGISTER ( &sk_player_chest3 );

	CVAR_REGISTER ( &sk_player_stomach1 );
	CVAR_REGISTER ( &sk_player_stomach2 );
	CVAR_REGISTER ( &sk_player_stomach3 );

	CVAR_REGISTER ( &sk_player_arm1 );
	CVAR_REGISTER ( &sk_player_arm2 );
	CVAR_REGISTER ( &sk_player_arm3 );

	CVAR_REGISTER ( &sk_player_leg1 );
	CVAR_REGISTER ( &sk_player_leg2 );
	CVAR_REGISTER ( &sk_player_leg3 );
// END REGISTER CVARS FOR SKILL LEVEL STUFF

// OZ CVARS BEGIN
	CVAR_REGISTER ( &suitchargercapacity );
	CVAR_REGISTER ( &healthchargercapacity );
	CVAR_REGISTER ( &suitchargerrecharge );
	CVAR_REGISTER ( &healthchargerrecharge );
	CVAR_REGISTER ( &suitchargeramount );
	CVAR_REGISTER ( &healthchargeramount );
	CVAR_REGISTER ( &healthkit );
	CVAR_REGISTER ( &battery );
	CVAR_REGISTER ( &starthealth );
	CVAR_REGISTER ( &startcharge );
	CVAR_REGISTER ( &maxhealth );
	CVAR_REGISTER ( &maxcharge );
	CVAR_REGISTER ( &jumpheight );
	CVAR_REGISTER ( &fallingdamage );
	CVAR_REGISTER ( &startlongjump );

	CVAR_REGISTER ( &headdamage );
	CVAR_REGISTER ( &chestdamage );
	CVAR_REGISTER ( &stomachdamage );
	CVAR_REGISTER ( &armdamage );
	CVAR_REGISTER ( &legdamage );

	CVAR_REGISTER ( &bloodamount );

	CVAR_REGISTER ( &grappleairspeed );
	CVAR_REGISTER ( &grapplewaterspeed );
	CVAR_REGISTER ( &grappledragspeed );
	CVAR_REGISTER ( &grappledisable );
	CVAR_REGISTER ( &grappledamage );
	CVAR_REGISTER ( &grappleglow );

	CVAR_REGISTER ( &disableglock );
	CVAR_REGISTER ( &disable357 );
	CVAR_REGISTER ( &disablemp5 );
	CVAR_REGISTER ( &disablem203 );
	CVAR_REGISTER ( &disableshotgun );
	CVAR_REGISTER ( &disablexbow );
	CVAR_REGISTER ( &disablerpg );
	CVAR_REGISTER ( &disablegauss );
	CVAR_REGISTER ( &disableegon );
	CVAR_REGISTER ( &disablehivehand );
	CVAR_REGISTER ( &disablehgrenade );
	CVAR_REGISTER ( &disablesatchel );
	CVAR_REGISTER ( &disabletripmine );
	CVAR_REGISTER ( &disablesnark );


	CVAR_REGISTER ( &startglock );
	CVAR_REGISTER ( &start357 );
	CVAR_REGISTER ( &startmp5 );
	CVAR_REGISTER ( &startshotgun );
	CVAR_REGISTER ( &startxbow );
	CVAR_REGISTER ( &startrpg );
	CVAR_REGISTER ( &startgauss );
	CVAR_REGISTER ( &startegon );
	CVAR_REGISTER ( &starthivehand );

	CVAR_REGISTER ( &start9mm );
	CVAR_REGISTER ( &startshells );
	CVAR_REGISTER ( &start357rounds );
	CVAR_REGISTER ( &startm203 );
	CVAR_REGISTER ( &starturanium );
	CVAR_REGISTER ( &startrockets );
	CVAR_REGISTER ( &startxbowbolts );
	CVAR_REGISTER ( &starthgrenades );
	CVAR_REGISTER ( &startsatchels );
	CVAR_REGISTER ( &starttripmines );
	CVAR_REGISTER ( &startsnarks );

	CVAR_REGISTER ( &maxuranium );
	CVAR_REGISTER ( &max9mm );
	CVAR_REGISTER ( &maxshells );
	CVAR_REGISTER ( &max357rounds );
	CVAR_REGISTER ( &maxm203 );
	CVAR_REGISTER ( &maxrockets );
	CVAR_REGISTER ( &maxsatchels );
	CVAR_REGISTER ( &maxtripmines );
	CVAR_REGISTER ( &maxhgrenades );
	CVAR_REGISTER ( &maxsnarks );
	CVAR_REGISTER ( &maxhornets );
	CVAR_REGISTER ( &maxxbowbolts );

	CVAR_REGISTER ( &giveuranium );
	CVAR_REGISTER ( &givechainbox );
	CVAR_REGISTER ( &giveshells );
	CVAR_REGISTER ( &give357rounds );
	CVAR_REGISTER ( &givem203 );
	CVAR_REGISTER ( &giveglockclip );
	CVAR_REGISTER ( &givemp5clip );
	CVAR_REGISTER ( &giverpgclip );
	CVAR_REGISTER ( &givexbowbolts );

	CVAR_REGISTER ( &givewithmp5 );
	CVAR_REGISTER ( &givewithrpg );
	CVAR_REGISTER ( &givewithglock );
	CVAR_REGISTER ( &givewithshotgun );
	CVAR_REGISTER ( &givewithgauss );
	CVAR_REGISTER ( &givewithegon );
	CVAR_REGISTER ( &givewithhgrenade );
	CVAR_REGISTER ( &givewithsatchel );
	CVAR_REGISTER ( &givewithtripmine );
	CVAR_REGISTER ( &givewithxbow );
	CVAR_REGISTER ( &givewithm203 );
	CVAR_REGISTER ( &givewithsnark );
	CVAR_REGISTER ( &givewithhivehand );
	CVAR_REGISTER ( &givewith357 );

	CVAR_REGISTER ( &magazineglock );
	CVAR_REGISTER ( &magazine357 );
	CVAR_REGISTER ( &magazinemp5 );
	CVAR_REGISTER ( &magazineshotgun );
	CVAR_REGISTER ( &magazinexbow );

	CVAR_REGISTER ( &damagecrowbar );
	CVAR_REGISTER ( &damageglock );
	CVAR_REGISTER ( &damageglocksniper );
	CVAR_REGISTER ( &damage357 );
	CVAR_REGISTER ( &damage357sniper );
	CVAR_REGISTER ( &damagemp5 );
	CVAR_REGISTER ( &damagemp5sniper );
	CVAR_REGISTER ( &damageshotgun );
	CVAR_REGISTER ( &damagegauss );
	CVAR_REGISTER ( &damageegonwide );
	CVAR_REGISTER ( &damagerpg );
	CVAR_REGISTER ( &damagem203 );
	CVAR_REGISTER ( &damagexbow );
	CVAR_REGISTER ( &damagexbowexplode );
	CVAR_REGISTER ( &damagexbowsniper );
	CVAR_REGISTER ( &damagegaussmax );
	CVAR_REGISTER ( &damageegonnarrow );
	CVAR_REGISTER ( &damagehgrenade );
	CVAR_REGISTER ( &damagehornet );
	CVAR_REGISTER ( &damagesatchel );
	CVAR_REGISTER ( &damagetripmine );
	CVAR_REGISTER ( &damagesnarkbite );
	CVAR_REGISTER ( &damagesnarkpop );

	CVAR_REGISTER ( &mp5tracers );
	CVAR_REGISTER ( &mp5spread );
	CVAR_REGISTER ( &mp5sniper );
	CVAR_REGISTER ( &silencemp5 );
	CVAR_REGISTER ( &glocksniper );
	CVAR_REGISTER ( &silenceglock );
	CVAR_REGISTER ( &snarkhealth );
	CVAR_REGISTER ( &snarktime );
	CVAR_REGISTER ( &itemrespawntime );
	CVAR_REGISTER ( &ammorespawntime );
	CVAR_REGISTER ( &weaponrespawntime );
	CVAR_REGISTER ( &gaussmaxchargetime );
	CVAR_REGISTER ( &nightops );
	CVAR_REGISTER ( &lightlevel );
	CVAR_REGISTER ( &autoswitch );
	CVAR_REGISTER ( &grenadetime );
	CVAR_REGISTER ( &flaretime );
	CVAR_REGISTER ( &pelletsshotgun );
	CVAR_REGISTER ( &pelletsdoubleshotgun );
	CVAR_REGISTER ( &waterweapons );
//	CVAR_REGISTER ( &randomspawn );

	CVAR_REGISTER ( &colorredrpg );
	CVAR_REGISTER ( &colorgreenrpg );
	CVAR_REGISTER ( &colorbluerpg );

	CVAR_REGISTER ( &colorredgrapple );
	CVAR_REGISTER ( &colorgreengrapple );
	CVAR_REGISTER ( &colorbluegrapple );

	CVAR_REGISTER ( &colorredhornet1 );
	CVAR_REGISTER ( &colorgreenhornet1 );
	CVAR_REGISTER ( &colorbluehornet1 );

	CVAR_REGISTER ( &colorredhornet2 );
	CVAR_REGISTER ( &colorgreenhornet2 );
	CVAR_REGISTER ( &colorbluehornet2 );

	CVAR_REGISTER ( &colorredgaussprimary );
	CVAR_REGISTER ( &colorgreengaussprimary );
	CVAR_REGISTER ( &colorbluegaussprimary );

	CVAR_REGISTER ( &colorredgausssecondary );
	CVAR_REGISTER ( &colorgreengausssecondary );
	CVAR_REGISTER ( &colorbluegausssecondary );

	CVAR_REGISTER ( &colorredtripmine );
	CVAR_REGISTER ( &colorgreentripmine );
	CVAR_REGISTER ( &colorbluetripmine );

	CVAR_REGISTER ( &recoilglock );
	CVAR_REGISTER ( &recoilmp5 );
	CVAR_REGISTER ( &recoilm203 );
	CVAR_REGISTER ( &recoilxbow );
	CVAR_REGISTER ( &recoilrpg );
	CVAR_REGISTER ( &recoilshotgun );
	CVAR_REGISTER ( &recoilshotgun2 );
	CVAR_REGISTER ( &recoil357 );
	CVAR_REGISTER ( &recoilgauss );
	CVAR_REGISTER ( &recoilhivehand );

	CVAR_REGISTER ( &speedhgrenade );
	CVAR_REGISTER ( &speedm203 );
	CVAR_REGISTER ( &speedrpgwater );
	CVAR_REGISTER ( &speedrpgair );
	CVAR_REGISTER ( &speedsatchel );
	CVAR_REGISTER ( &speedxbowair );
	CVAR_REGISTER ( &speedxbowwater );
	CVAR_REGISTER ( &speedhornet );
	CVAR_REGISTER ( &speedhornet2 );

	CVAR_REGISTER ( &cyclecrowbar );
	CVAR_REGISTER ( &cyclecrowbar2 );
	CVAR_REGISTER ( &cycleglock );
	CVAR_REGISTER ( &cycleglock2 );
	CVAR_REGISTER ( &cyclemp5 );
	CVAR_REGISTER ( &cyclemp5sniper );
	CVAR_REGISTER ( &cycle357 );
	CVAR_REGISTER ( &cycleshotgun );
	CVAR_REGISTER ( &cycleshotgun2 );
	CVAR_REGISTER ( &cyclegauss );
	CVAR_REGISTER ( &cyclem203 );
	CVAR_REGISTER ( &cyclexbow );
	CVAR_REGISTER ( &cyclexbow2 );
	CVAR_REGISTER ( &cyclehivehand );
	CVAR_REGISTER ( &cyclehivehand2 );

	CVAR_REGISTER ( &pointsperkill );
	CVAR_REGISTER ( &pointspersuicide );
	CVAR_REGISTER ( &pointsperworldkill );
	CVAR_REGISTER ( &pointsperteamkill );
	CVAR_REGISTER ( &pointsperdeath );

	CVAR_REGISTER ( &playerair );

	CVAR_REGISTER ( &weightcrowbar );
	CVAR_REGISTER ( &weightglock );
	CVAR_REGISTER ( &weight357 );
	CVAR_REGISTER ( &weightmp5 );
	CVAR_REGISTER ( &weightshotgun );
	CVAR_REGISTER ( &weightxbow );
	CVAR_REGISTER ( &weightrpg );
	CVAR_REGISTER ( &weightgauss );
	CVAR_REGISTER ( &weightegon );
	CVAR_REGISTER ( &weighthivehand );
	CVAR_REGISTER ( &weighthgrenade );
	CVAR_REGISTER ( &weightsnark );
	CVAR_REGISTER ( &weightsatchel );
	CVAR_REGISTER ( &weighttripmine );

	CVAR_REGISTER ( &randomgrapplecolor );

	CVAR_REGISTER ( &respawntimerune );
	CVAR_REGISTER ( &fragruneamount );
	CVAR_REGISTER ( &regenrunehealth );
	CVAR_REGISTER ( &regenrunearmor );
	CVAR_REGISTER ( &superhookrunedamage );
	CVAR_REGISTER ( &lowgravruneamount );

	CVAR_REGISTER ( &reloadglock );
	CVAR_REGISTER ( &reload357 );
	CVAR_REGISTER ( &reloadmp5 );
	CVAR_REGISTER ( &reloadshotgun );
	CVAR_REGISTER ( &reloadxbow );
	CVAR_REGISTER ( &reloadrpg );

	CVAR_REGISTER ( &playerid );

	CVAR_REGISTER ( &fragrunenumber );
	CVAR_REGISTER ( &regenrunenumber );
	CVAR_REGISTER ( &crowbarrunenumber );
	CVAR_REGISTER ( &hasterunenumber );
	CVAR_REGISTER ( &cloakrunenumber );
	CVAR_REGISTER ( &hookrunenumber );
	CVAR_REGISTER ( &protectrunenumber );
	CVAR_REGISTER ( &lowgravrunenumber );
	CVAR_REGISTER ( &vamprunenumber );
	CVAR_REGISTER ( &traprunenumber );
	//jammer superglock
	CVAR_REGISTER (&superglockrunenumber);

	CVAR_REGISTER ( &zoomamountglock );
	CVAR_REGISTER ( &zoomamountmp5 );
	CVAR_REGISTER ( &zoomamountxbow );

	CVAR_REGISTER ( &maxhealthwithvamprune );
	CVAR_REGISTER ( &boobytrapdamage );

	CVAR_REGISTER ( &infinity);
	CVAR_REGISTER ( &infinityctf);

//	CVAR_REGISTER ( &homing);

	CVAR_REGISTER ( &ctf);
	CVAR_REGISTER ( &ctfteamlist);
	CVAR_REGISTER ( &ctfflagscore);
	CVAR_REGISTER ( &ctfstealscore);
	CVAR_REGISTER ( &ctfreturnscore);

//	CVAR_REGISTER ( &damagehomingrpg);

	// OZ CVARS END

	// Oz fix
	//SERVER_COMMAND( "exec skill.cfg\n" );

	SERVER_COMMAND( "exec cfg/multiplayer.cfg\n" );
	SERVER_COMMAND( "exec cfg/weapons.cfg\n" );
	SERVER_COMMAND( "exec cfg/player.cfg\n" );
	SERVER_COMMAND( "exec cfg/grapple.cfg\n" );
	SERVER_COMMAND( "exec cfg/colors.cfg\n" );
	SERVER_COMMAND( "exec cfg/special.cfg\n" );
}

