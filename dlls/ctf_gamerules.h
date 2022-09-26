
//
// ctf_gamerules.h
//

#define MAX_CTF_TEAMNAME_LENGTH	16
#define MAX_CTF_TEAMS			2

#define CTF_TEAMLISTLENGTH		MAX_CTF_TEAMS*MAX_CTF_TEAMNAME_LENGTH

class CHalfLifeCTF : public CHalfLifeMultiplay
{
public:
	CHalfLifeCTF();

	virtual BOOL ClientCommand( CBasePlayer *pPlayer, const char *pcmd );
	virtual void ClientUserInfoChanged( CBasePlayer *pPlayer, char *infobuffer );
	virtual BOOL IsCTF( void );
	virtual BOOL FPlayerCanTakeDamage( CBasePlayer *pPlayer, CBaseEntity *pAttacker );
	virtual int PlayerRelationship( CBaseEntity *pPlayer, CBaseEntity *pTarget );
	virtual const char *GetTeamID( CBaseEntity *pEntity );
	virtual BOOL ShouldAutoAim( CBasePlayer *pPlayer, edict_t *target );
	virtual int IPointsForKill( CBasePlayer *pAttacker, CBasePlayer *pKilled );
	virtual void InitHUD( CBasePlayer *pl );
	virtual void DeathNotice( CBasePlayer *pVictim, entvars_t *pKiller, entvars_t *pevInflictor );
	virtual const char *GetGameDescription( void ) { return "Oz CTF"; }  // this is the game name that gets seen in the server browser
	virtual void UpdateGameMode( CBasePlayer *pPlayer );  // the client needs to be informed of the current game mode
	virtual void PlayerKilled( CBasePlayer *pVictim, entvars_t *pKiller, entvars_t *pInflictor );
	virtual void Think ( void );
	virtual int GetTeamIndex( const char *pTeamName );
	virtual const char *GetIndexedTeamName( int teamIndex );
	virtual BOOL IsValidTeam( const char *pTeamName );
	const char *SetDefaultPlayerTeam( CBasePlayer *pPlayer );
	virtual void ChangePlayerTeam( CBasePlayer *pPlayer, const char *pTeamName, BOOL bKill, BOOL bGib );

	// Oz CTF
	// flags
	// returns pointer to the flag it has just created
/*	virtual edict_t *MakeFlagHere( const Vector &VecHere, int iTeam );	//****
	virtual void PlayerHasFlag( CBasePlayer *pPlayer );		//****
	virtual void RemoveFlag( CBasePlayer *pPlayer );		//****
	virtual void DropFlag( CBasePlayer *pPlayer );			//****
	// simple anim handling, just here to keep it seperate yet convenient
	virtual void AnimateThisEnt( entvars_t *pevEnt );	//****
*/
private:
	void RecountTeams( void );
	const char *TeamWithFewestPlayers( void );

	BOOL m_DisableDeathMessages;
	BOOL m_DisableDeathPenalty;
	BOOL m_teamLimit;				// This means the server set only some teams as valid
	char m_szTeamList[CTF_TEAMLISTLENGTH];
};
