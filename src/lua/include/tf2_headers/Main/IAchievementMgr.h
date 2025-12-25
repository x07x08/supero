#if !defined(MainIAchievementMgr_Included)
#define MainIAchievementMgr_Included

#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
#include <stdbool.h>
#endif

typedef struct CBaseAchievement CBaseAchievement;

typedef struct IAchievement IAchievement;

typedef struct IAchievement_VMT
{
	int (*GetAchievementID)(IAchievement*);
	const char* (*GetName)(IAchievement*);
	int (*GetFlags)(IAchievement*);
	int (*GetGoal)(IAchievement*);
	int (*GetCount)(IAchievement*);
	bool (*IsAchieved)(IAchievement*);
	int (*GetPointValue)(IAchievement*);
	bool (*ShouldSaveWithGame)(IAchievement*);
	bool (*ShouldHideUntilAchieved)(IAchievement*);
	bool (*ShouldShowOnHUD)(IAchievement*);
	void (*SetShowOnHUD)(IAchievement*, bool bShow);
} IAchievement_VMT;

struct IAchievement
{
	IAchievement_VMT* vmt;
};

typedef struct IAchievementMgr IAchievementMgr;

typedef struct IAchievementMgr_VMT
{
	IAchievement* (*GetAchievementByIndex)(IAchievementMgr*, int index);
	CBaseAchievement* (*GetAchievementByID)(IAchievementMgr*, int id);
	int (*GetAchievementCount)(IAchievementMgr*);
	void (*InitializeAchievements)(IAchievementMgr*);
	void (*AwardAchievement)(IAchievementMgr*, int iAchievementID);
	void (*OnMapEvent)(IAchievementMgr*, const char* pchEventName);
	void (*DownloadUserData)(IAchievementMgr*);
	void (*EnsureGlobalStateLoaded)(IAchievementMgr*);
	void (*SaveGlobalStateIfDirty)(IAchievementMgr*, bool bAsync);
	bool (*HasAchieved)(IAchievementMgr*, const char* pchName);
	bool (*WereCheatsEverOn)(IAchievementMgr*);
} IAchievementMgr_VMT;

struct IAchievementMgr
{
	IAchievementMgr_VMT* vmt;
};

#define ACH_LISTEN_KILL_EVENTS 0x0001
#define ACH_LISTEN_MAP_EVENTS 0x0002
#define ACH_LISTEN_COMPONENT_EVENTS 0x0004
#define ACH_HAS_COMPONENTS 0x0020
#define ACH_SAVE_WITH_GAME 0x0040
#define ACH_SAVE_GLOBAL 0x0080
#define ACH_FILTER_ATTACKER_IS_PLAYER 0x0100
#define ACH_FILTER_VICTIM_IS_PLAYER_ENEMY 0x0200
#define ACH_FILTER_FULL_ROUND_ONLY 0x0400

#define ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS ACH_LISTEN_KILL_EVENTS | ACH_FILTER_ATTACKER_IS_PLAYER | ACH_FILTER_VICTIM_IS_PLAYER_ENEMY
#define ACH_LISTEN_KILL_ENEMY_EVENTS ACH_LISTEN_KILL_EVENTS | ACH_FILTER_VICTIM_IS_PLAYER_ENEMY

#endif
