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

#ifndef GAME_H
#define GAME_H

extern void GameDLLInit( void );


extern cvar_t	displaysoundlist;
extern cvar_t	mapcyclefile;
extern cvar_t	servercfgfile;
extern cvar_t	lservercfgfile;

// multiplayer server rules
extern cvar_t	fraglimit;
extern cvar_t	timelimit;
extern cvar_t	friendlyfir;
extern cvar_t	falldamage;
extern cvar_t	weaponstay;
extern cvar_t	forcerespaw;
extern cvar_t	footsteps;
extern cvar_t	flashlight;
extern cvar_t	aimcrosshair;
extern cvar_t	decalfrequency;
extern cvar_t	teamlist;
extern cvar_t	teamoverride;
extern cvar_t	defaultteam;

// oz server rules
extern cvar_t	suitchargercapacity;
extern cvar_t	healthchargercapacity;
extern cvar_t	healthchargerrecharge;
extern cvar_t	suitchargerrecharge;
extern cvar_t	healthchargeramount;
extern cvar_t	suitchargeramount;
extern cvar_t	healthkit;
extern cvar_t	battery;
extern cvar_t	starthealth;
extern cvar_t	startcharge;
extern cvar_t	maxhealth;
extern cvar_t	maxcharge;
extern cvar_t	jumpheight;
extern cvar_t	fallingdamage;
extern cvar_t	startlongjump;

extern cvar_t	headdamage;
extern cvar_t	chestdamage;
extern cvar_t	stomachdamage;
extern cvar_t	armdamage;
extern cvar_t	legdamage;

extern cvar_t	bloodamount;

extern cvar_t	grappleairspeed;
extern cvar_t	grapplewaterspeed;
extern cvar_t	grappledragspeed;
extern cvar_t	grappledisable;
extern cvar_t	grappledamage;
extern cvar_t	grappleglow;

extern cvar_t	disableglock;
extern cvar_t	disable357;
extern cvar_t	disablemp5;
extern cvar_t	disablem203;
//25
extern cvar_t	disableshotgun;
extern cvar_t	disablexbow;
extern cvar_t	disablerpg;
extern cvar_t	disablegauss;
extern cvar_t	disableegon;
extern cvar_t	disablehivehand;
extern cvar_t	disablehgrenade;
extern cvar_t	disablesatchel;
extern cvar_t	disabletripmine;
extern cvar_t	disablesnark;

extern cvar_t	startglock;
extern cvar_t	start357;
extern cvar_t	startmp5;
extern cvar_t	startshotgun;
extern cvar_t	startxbow;
extern cvar_t	startrpg;
extern cvar_t	startgauss;
extern cvar_t	startegon;
extern cvar_t	starthivehand;

extern cvar_t	start9mm;
extern cvar_t	startshells;
//50
extern cvar_t	start357rounds;
extern cvar_t	startm203;
extern cvar_t	starturanium;
extern cvar_t	startrockets;
extern cvar_t	startxbowbolts;
extern cvar_t	starthgrenades;
extern cvar_t	startsatchels;
extern cvar_t	starttripmines;
extern cvar_t	startsnarks;

extern cvar_t	maxuranium;
extern cvar_t	max9mm;
extern cvar_t	maxshells;
extern cvar_t	max357rounds;
extern cvar_t	maxm203;
extern cvar_t	maxrockets;
extern cvar_t	maxsatchels;
extern cvar_t	maxtripmines;
extern cvar_t	maxhgrenades;
extern cvar_t	maxsnarks;
extern cvar_t	maxhornets;
extern cvar_t	maxxbowbolts;

extern cvar_t	giveuranium;
extern cvar_t	givechainbox;
extern cvar_t	giveshells;
extern cvar_t	give357rounds;
extern cvar_t	givem203;
extern cvar_t	giveglockclip;
extern cvar_t	givemp5clip;
extern cvar_t	giverpgclip;
//75
extern cvar_t	givexbowbolts;

extern cvar_t	givewithmp5;
extern cvar_t	givewithrpg;
extern cvar_t	givewithglock;
extern cvar_t	givewithshotgun;
extern cvar_t	givewithgauss;
extern cvar_t	givewithegon;
extern cvar_t	givewithhgrenade;
extern cvar_t	givewithsatchel;
extern cvar_t	givewithtripmine;
extern cvar_t	givewithxbow;
extern cvar_t	givewithm203;
extern cvar_t	givewithsnark;
extern cvar_t	givewithhivehand;
extern cvar_t	givewith357;

extern cvar_t	magazineglock;
extern cvar_t	magazine357;
extern cvar_t	magazinemp5;
extern cvar_t	magazineshotgun;
extern cvar_t	magazinexbow;

extern cvar_t	damagecrowbar;
extern cvar_t	damageglock;
extern cvar_t	damageglocksniper;
extern cvar_t	damage357;
extern cvar_t	damage357sniper;
extern cvar_t	damagemp5;
extern cvar_t	damagemp5sniper;
extern cvar_t	damageshotgun;
//100
extern cvar_t	damagegauss;
extern cvar_t	damageegonwide;
extern cvar_t	damagerpg;
extern cvar_t	damagem203;
extern cvar_t	damagexbow;
extern cvar_t	damagexbowexplode;
extern cvar_t	damagexbowsniper;
extern cvar_t	damagegaussmax;
extern cvar_t	damageegonnarrow;
extern cvar_t	damagehgrenade;
extern cvar_t	damagehornet;
extern cvar_t	damagesatchel;
extern cvar_t	damagetripmine;
extern cvar_t	damagesnarkbite;
extern cvar_t	damagesnarkpop;

extern cvar_t	speedhgrenade;
extern cvar_t	speedm203;
extern cvar_t	speedrpgwater;
extern cvar_t	speedrpgair;
extern cvar_t	speedsatchel;
extern cvar_t	speedxbowwater;
extern cvar_t	speedxbowair;
extern cvar_t	speedhornet;
extern cvar_t	speedhornet2;

extern cvar_t	mp5tracers;
//125
extern cvar_t	mp5spread;
extern cvar_t	mp5sniper;
extern cvar_t	silencemp5;
extern cvar_t	glocksniper;
extern cvar_t	silenceglock;
extern cvar_t	pelletsshotgun;
extern cvar_t	pelletsdoubleshotgun;
extern cvar_t	snarkhealth;
extern cvar_t	snarktime;
extern cvar_t	itemrespawntime;
extern cvar_t	ammorespawntime;
extern cvar_t	weaponrespawntime;
extern cvar_t	gaussmaxchargetime;
extern cvar_t	nightops;
extern cvar_t	lightlevel;
extern cvar_t	autoswitch;
extern cvar_t	grenadetime;
extern cvar_t	flaretime;
extern cvar_t	waterweapons;
extern cvar_t	randomspawn;

extern cvar_t	colorredrpg;
extern cvar_t	colorgreenrpg;
extern cvar_t	colorbluerpg;

extern cvar_t	colorredgrapple;
extern cvar_t	colorgreengrapple;
//150
extern cvar_t	colorbluegrapple;

extern cvar_t	colorredhornet1;
extern cvar_t	colorgreenhornet1;
extern cvar_t	colorbluehornet1;

extern cvar_t	colorredhornet2;
extern cvar_t	colorgreenhornet2;
extern cvar_t	colorbluehornet2;

extern cvar_t	colorredgaussprimary;
extern cvar_t	colorgreengaussprimary;
extern cvar_t	colorbluegaussprimary;

extern cvar_t	colorredgausssecondary;
extern cvar_t	colorgreengausssecondary;
extern cvar_t	colorbluegausssecondary;

extern cvar_t	colorredtripmine;
extern cvar_t	colorgreentripmine;
extern cvar_t	colorbluetripmine;

extern cvar_t	recoilglock;
extern cvar_t	recoilmp5;
extern cvar_t	recoilm203;
extern cvar_t	recoilxbow;
extern cvar_t	recoilrpg;
extern cvar_t	recoil357;
extern cvar_t	recoilshotgun;
extern cvar_t	recoilshotgun2;
extern cvar_t	recoilhivehand;
//175
extern cvar_t	recoilgauss;

extern cvar_t	cyclecrowbar;
extern cvar_t	cyclecrowbar2;
extern cvar_t	cycleglock;
extern cvar_t	cycleglock2;
extern cvar_t	cycle357;
extern cvar_t	cyclemp5;
extern cvar_t	cyclemp5sniper;
extern cvar_t	cycleshotgun;
extern cvar_t	cycleshotgun2;
extern cvar_t	cyclegauss;
extern cvar_t	cyclem203;
extern cvar_t	cyclexbow;
extern cvar_t	cyclexbow2;
extern cvar_t	cyclehivehand;
extern cvar_t	cyclehivehand2;

extern cvar_t	pointsperkill;
extern cvar_t	pointspersuicide;
extern cvar_t	pointsperworldkill;
extern cvar_t	pointsperteamkill;
extern cvar_t	pointsperdeath;

extern cvar_t	playerair;

extern cvar_t	weightcrowbar;
extern cvar_t	weightglock;
extern cvar_t	weight357;
extern cvar_t	weightmp5;
//200
extern cvar_t	weightshotgun;
extern cvar_t	weightxbow;
extern cvar_t	weightrpg;
extern cvar_t	weightgauss;
extern cvar_t	weightegon;
extern cvar_t	weighthivehand;
extern cvar_t	weighthgrenade;
extern cvar_t	weightsnark;
extern cvar_t	weightsatchel;
extern cvar_t	weighttripmine;

extern cvar_t	randomgrapplecolor;

extern cvar_t	respawntimerune;
extern cvar_t	fragruneamount;
extern cvar_t	regenrunehealth;
extern cvar_t	regenrunearmor;
extern cvar_t	superhookrunedamage;
extern cvar_t	lowgravruneamount;

extern cvar_t	reloadglock;
extern cvar_t	reload357;
extern cvar_t	reloadmp5;
extern cvar_t	reloadshotgun;
extern cvar_t	reloadrpg;
extern cvar_t	reloadxbow;

extern cvar_t	playerid;

extern cvar_t	fragrunenumber;
extern cvar_t	regenrunenumber;
extern cvar_t	crowbarrunenumber;
extern cvar_t	hasterunenumber;
extern cvar_t	cloakrunenumber;
extern cvar_t	hookrunenumber;
extern cvar_t	protectrunenumber;
extern cvar_t	lowgravrunenumber;
extern cvar_t	vamprunenumber;
extern cvar_t	traprunenumber;
//jammer superglock
extern cvar_t	superglockrunenumber;

extern cvar_t	zoomamountglock;
extern cvar_t	zoomamountmp5;
extern cvar_t	zoomamountxbow;

extern cvar_t	maxhealthwithvamprune;
extern cvar_t	boobytrapdamage;

extern cvar_t	infinity;
extern cvar_t	infinityctf;

extern cvar_t	homing;

extern cvar_t	ctf;
extern cvar_t	ctfteamlist;
extern cvar_t	ctfflagscore;
extern cvar_t	ctfstealscore;
extern cvar_t	ctfreturnscore;


extern cvar_t	damagehomingrpg;

#endif		// GAME_H
