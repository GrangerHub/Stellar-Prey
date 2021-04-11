////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 1999 - 2005 Id Software, Inc.
// Copyright(C) 2000 - 2006 Tim Angus
// Copyright(C) 2011 - 2021 Dusan Jocic <dusanjocic@msn.com>
//
// This file is part of OpenWolf.
//
// OpenWolf is free software; you can redistribute it
// and / or modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 3 of the License,
// or (at your option) any later version.
//
// OpenWolf is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenWolf; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110 - 1301  USA
//
// -------------------------------------------------------------------------------------
// File name:   sgame_local.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description: Local definitions for game module
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __G_LOCAL_H__
#define __G_LOCAL_H__

typedef struct gclient_s gclient_t;

//==================================================================

#define INFINITE 1000000

#define FRAMETIME 100         // msec
#define CARNAGE_REWARD_TIME 3000
#define REWARD_SPRITE_TIME 2000

#define INTERMISSION_DELAY_TIME 1000
#define SP_INTERMISSION_DELAY_TIME 5000

// gentity->flags
#define FL_GODMODE        0x00000010
#define FL_NOTARGET       0x00000020
#define FL_TEAMSLAVE      0x00000400  // not the first on the team
#define FL_NO_KNOCKBACK   0x00000800
#define FL_DROPPED_ITEM   0x00001000
#define FL_NO_BOTS        0x00002000  // spawn point not for bot use
#define FL_NO_HUMANS      0x00004000  // spawn point just for bots
#define FL_FORCE_GESTURE  0x00008000  // spawn point just for bots

// movers are things like doors, plats, buttons, etc
typedef enum
{
    MOVER_POS1,
    MOVER_POS2,
    MOVER_1TO2,
    MOVER_2TO1,
    
    ROTATOR_POS1,
    ROTATOR_POS2,
    ROTATOR_1TO2,
    ROTATOR_2TO1,
    
    MODEL_POS1,
    MODEL_POS2,
    MODEL_1TO2,
    MODEL_2TO1
} moverState_t;

#define SP_PODIUM_MODEL   "models/mapobjects/podium/podium4.md3"

//============================================================================

struct gentity_s
{
    entityState_t     s;        // communicated by server to clients
    entityShared_t    r;        // shared by both the server system and game
    
    // DO NOT MODIFY ANYTHING ABOVE THIS, THE SERVER
    // EXPECTS THE FIELDS IN THAT ORDER!
    //================================
    
    struct gclient_s*  client;        // nullptr if not a client
    
    bool              inuse;
    
    valueType*             classname;     // set in QuakeEd
    sint               spawnflags;     // set in QuakeEd
    
    valueType*             sound;
    
    bool              neverFree;      // if true, FreeEntity will only unlink
    // bodyque uses this
    
    sint               flags;          // FL_* variables
    
    valueType*             model;
    valueType*             model2;
    sint               freetime;       // level.time when the object was freed
    
    // bullet physics object
    class btRigidBody* body;
    
    sint               eventTime;      // events will be cleared EVENT_VALID_MSEC after set
    bool              freeAfterEvent;
    bool              unlinkAfterEvent;
    
    bool              physicsObject;  // if true, it can be pushed by movers and fall off edges
    // all game items are physicsObjects,
    float32               physicsBounce;  // 1.0 = continuous bounce, 0.0 = no bounce
    sint               clipmask;       // brushes with this content value will be collided against
    // when moving.  items and corpses do not collide against
    // players, for instance
    
    // movers
    moverState_t      moverState;
    sint               soundPos1;
    sint               sound1to2;
    sint               sound2to1;
    sint               soundPos2;
    sint               soundLoop;
    gentity_t*        parent;
    gentity_t*        nextTrain;
    gentity_t*        prevTrain;
    vec3_t            pos1, pos2;
    float32               rotatorAngle;
    gentity_t*        clipBrush;     // clipping brush for model doors
    
    valueType*             message;
    
    sint               timestamp;      // body queue sinking, etc
    
    float32               angle;          // set in editor, -1 = up, -2 = down
    valueType*             target;
    valueType*             targetname;
    valueType*             team;
    valueType*             targetShaderName;
    valueType*             targetShaderNewName;
    gentity_t*        target_ent;
    
    float32               speed;
    float32               lastSpeed;      // used by trains that have been restarted
    vec3_t            movedir;
    
    // acceleration evaluation
    bool              evaluateAcceleration;
    vec3_t            oldVelocity;
    vec3_t            acceleration;
    vec3_t            oldAccel;
    vec3_t            jerk;
    
    sint               nextthink;
    void ( *think )( gentity_t* self );
    void ( *reached )( gentity_t* self );             // movers call this when hitting endpoint
    void ( *blocked )( gentity_t* self, gentity_t* other );
    void ( *touch )( gentity_t* self, gentity_t* other, trace_t* trace );
    void ( *use )( gentity_t* self, gentity_t* other, gentity_t* activator );
    void ( *pain )( gentity_t* self, gentity_t* attacker, sint damage );
    void ( *die )( gentity_t* self, gentity_t* inflictor, gentity_t* attacker, sint damage, sint mod );
    
    sint               pain_debounce_time;
    sint               fly_sound_debounce_time;  // wind tunnel
    sint               last_move_time;
    
    sint               health;
    sint               lastHealth; // currently only used for overmind
    
    bool              takedamage;
    
    sint               damage;
    sint               splashDamage; // quad will increase this without increasing radius
    sint               splashRadius;
    sint               methodOfDeath;
    sint               splashMethodOfDeath;
    
    sint               count;
    sint	              bounceCount;
    bool              altFire;
    
    gentity_t*        chain;
    gentity_t*        enemy;
    gentity_t*        activator;
    gentity_t*        teamchain;   // next entity in team
    gentity_t*        teammaster;  // master of the team
    
    sint               watertype;
    sint               waterlevel;
    
    sint               noise_index;
    
    // timing variables
    float32               wait;
    float32               random;
    
    team_t            stageTeam;
    stage_t           stageStage;
    
    team_t            buildableTeam;      // buildable item team
    gentity_t*        parentNode;        // for creep and defence/spawn dependencies
    bool              active;             // for power repeater, but could be useful elsewhere
    bool              locked;             // used for turret tracking
    bool              powered;            // for human buildables
    sint               builtBy;            // clientNum of person that built this
    gentity_t*        overmindNode;      // controlling overmind
    sint               dcc;                // controlled by how many dccs
    bool              spawned;            // whether or not this buildable has finished spawning
    sint               shrunkTime;         // time when a barricade shrunk or zero
    sint               buildTime;          // when this buildable was built
    sint               animTime;           // last animation change
    sint               time1000;           // timer evaluated every second
    bool              deconstruct;        // deconstruct if no BP left
    sint               deconstructTime;    // time at which structure marked
    sint               overmindAttackTimer;
    sint               overmindDyingTimer;
    sint               overmindSpawnsTimer;
    sint               nextPhysicsTime;    // buildables don't need to check what they're sitting on
    // every single frame.. so only do it periodically
    sint               clientSpawnTime;    // the time until this spawn can spawn a client
    bool              lev1Grabbed;        // for turrets interacting with lev1s
    sint               lev1GrabTime;       // for turrets interacting with lev1s
    
    sint               credits[ MAX_CLIENTS ];     // human credits for each client
    bool              creditsHash[ MAX_CLIENTS ]; // track who has claimed credit
    sint               killedBy;                   // clientNum of killer
    
    gentity_t*        targeted;          // true if the player is currently a valid target of a turret
    vec3_t            turretAim;          // aim vector for turrets
    vec3_t            turretAimRate;      // track turn speed for norfenturrets
    sint               turretSpinupTime;   // spinup delay for norfenturrets
    
    vec4_t            animation;          // animated map objects
    
    gentity_t*        builder;           // occupant of this hovel
    
    bool              nonSegModel;        // this entity uses a nonsegmented player model
    
    buildable_t       bTriggers[ BA_NUM_BUILDABLES ]; // which buildables are triggers
    class_t           cTriggers[ PCL_NUM_CLASSES ];   // which classes are triggers
    weapon_t          wTriggers[ WP_NUM_WEAPONS ];    // which weapons are triggers
    upgrade_t         uTriggers[ UP_NUM_UPGRADES ];   // which upgrades are triggers
    
    sint               triggerGravity;                 // gravity for this trigger
    
    sint               suicideTime;                    // when the client will suicide
    
    sint               lastDamageTime;
    sint               nextRegenTime;
    
    bool              zapping;                        // adv maurader is zapping
    bool              wasZapping;                     // adv maurader was zapping
    sint               zapTargets[ LEVEL2_AREAZAP_MAX_TARGETS ];
    float32               zapDmg;                         // keep track of damage
    
    bool              ownerClear;                     // used for missle tracking
    
    bool              pointAgainstWorld;              // don't use the bbox for map collisions
};

typedef enum
{
    CON_DISCONNECTED,
    CON_CONNECTING,
    CON_CONNECTED
} clientConnected_t;

// the auto following clients don't follow a specific client
// number, but instead follow the first two active players
#define FOLLOW_ACTIVE1  -1
#define FOLLOW_ACTIVE2  -2

// client data that stays across multiple levels or tournament restarts
// this is achieved by writing all the data to cvar strings at game shutdown
// time and reading them back at connection time.  Anything added here
// MUST be dealt with in idSGameLocal::InitSessionData() / idSGameLocal::ReadSessionData() / idSGameLocal::WriteSessionData()
typedef struct
{
    sint spectatorTime;    // for determining next-in-line to play
    spectatorState_t spectatorState;
    sint spectatorClient;  // for chasecam and follow mode
    sint wins, losses;     // tournament stats
    bool teamLeader;       // true when this client is a team leader
    team_t restartTeam;      // for !restart keepteams and !restart switchteams
    clientList_t ignoreList;
} clientSession_t;

// data to store details of clients that have abnormally disconnected
typedef struct connectionRecord_s
{
    sint       clientNum;
    sint       oldClient;
    team_t    clientTeam;
    sint       clientCredit;
    
    sint       ptrCode;
} connectionRecord_t;

// client data that stays across multiple respawns, but is cleared
// on each level change or team change at ClientBegin()
typedef struct
{
    clientConnected_t   connected;
    usercmd_t           cmd;                // we would lose angles if not persistant
    bool                localClient;        // true if "ip" info key is "localhost"
    bool                initialSpawn;       // the first spawn should be at a cool location
    bool                stickySpec;         // don't stop spectating a player after they get killed
    bool                pmoveFixed;         //
    valueType                netname[ MAX_NAME_LENGTH ];
    sint                 maxHealth;          // for handicapping
    sint                 enterTime;          // level.time the client entered the game
    sint                 location;           // player locations
    sint                 voteCount;          // to prevent people from constantly calling votes
    bool                teamInfo;           // send team overlay updates?
    float32                 flySpeed;           // for spectator/noclip moves
    
    class_t             classSelection;     // player class (copied to ent->client->ps.stats[ STAT_CLASS ] once spawned)
    float32                 evolveHealthFraction;
    weapon_t            humanItemSelection; // humans have a starting item
    team_t              teamSelection;      // player team (copied to ps.stats[ STAT_TEAM ])
    
    sint                 teamChangeTime;     // level.time of last team change
    bool                joinedATeam;        // used to tell when a PTR code is valid
    connectionRecord_t*  connection;
    
    sint                 nameChangeTime;
    sint                 nameChanges;
    
    // used to save persistant[] values while in SPECTATOR_FOLLOW mode
    sint                 savedCredit;
    
    // votes
    bool                vote;
    bool                teamVote;
    
    // flood protection
    sint                 floodDemerits;
    sint                 floodTime;
    
    vec3_t              lastDeathLocation;
    valueType                guid[ 33 ];
    valueType                ip[ 40 ];
    bool                muted;
    bool                denyBuild;
    bool                demoClient;
    sint                 adminLevel;
    valueType                voice[ MAX_VOICE_NAME_LEN ];
    bool                useUnlagged;
    g_admin_admin_t*    admin;
} clientPersistant_t;

#define MAX_UNLAGGED_MARKERS 10
typedef struct unlagged_s
{
    vec3_t      origin;
    vec3_t      mins;
    vec3_t      maxs;
    bool    used;
} unlagged_t;

// this structure is cleared on each ClientSpawn(),
// except for 'client->pers' and 'client->sess'
struct gclient_s
{
    // ps MUST be the first element, because the server expects it
    playerState_t       ps;       // communicated by server to clients
    
    // exported into pmove, but not communicated to clients
    pmoveExt_t          pmext;
    
    // the rest of the structure is private to game
    clientPersistant_t  pers;
    clientSession_t     sess;
    
    class btKinematicCharacterController* characterController;
    
    bool                readyToExit;    // wishes to leave the intermission
    
    bool                noclip;
    
    sint                 lastCmdTime;    // level.time of last usercmd_t, for EF_CONNECTION
    // we can't just use pers.lastCommand.time, because
    // of the g_sycronousclients case
    sint                 buttons;
    sint                 oldbuttons;
    sint                 latched_buttons;
    
    vec3_t              oldOrigin;
    
    // sum up damage over an entire frame, so
    // shotgun blasts give a single big kick
    sint                 damage_armor;     // damage absorbed by armor
    sint                 damage_blood;     // damage taken out of health
    sint                 damage_knockback; // impact damage
    vec3_t              damage_from;      // origin for vector calculation
    bool                damage_fromWorld; // if true, don't use the damage_from vector
    
    //
    sint                 lastkilled_client;// last client that this client killed
    sint                 lasthurt_client;  // last client that damaged this client
    sint                 lasthurt_mod;     // type of damage the client did
    
    // timers
    sint                 respawnTime;      // can respawn when time > this
    sint                 inactivityTime;   // kick players when time > this
    bool                inactivityWarning;// true if the five seoond warning has been given
    sint                 rewardTime;       // clear the EF_AWARD_IMPRESSIVE, etc when time > this
    sint                 boostedTime;      // last time we touched a booster
    
    sint                 airOutTime;
    
    sint                 lastKillTime;     // for multiple kill rewards
    
    bool                fireHeld;         // used for hook
    bool                fire2Held;        // used for alt fire
    gentity_t*           hook;            // grapple hook if out
    
    sint                 switchTeamTime;   // time the player switched teams
    
    sint                 time100;          // timer for 100ms interval events
    sint                 time1000;         // timer for one second interval events
    
    valueType*                areabits;
    
    gentity_t*           hovel;
    
    sint                 lastPoisonTime;
    sint                 poisonImmunityTime;
    gentity_t*          lastPoisonClient;
    sint                 lastPoisonCloudedTime;
    sint                 grabExpiryTime;
    sint                 lastLockTime;
    sint                 lastSlowTime;
    sint                 lastMedKitTime;
    sint                 medKitHealthToRestore;
    sint                 medKitIncrementTime;
    sint                 lastCreepSlowTime;    // time until creep can be removed
    
    bool                charging;
    
    vec3_t              hovelOrigin;          // player origin before entering hovel
    
    sint                 lastFlameBall;        // s.number of the last flame ball fired
    
    unlagged_t          unlaggedHist[ MAX_UNLAGGED_MARKERS ];
    unlagged_t          unlaggedBackup;
    unlagged_t          unlaggedCalc;
    sint                 unlaggedTime;
    
    float32                 voiceEnthusiasm;
    valueType                lastVoiceCmd[ MAX_VOICE_CMD_LEN ];
    
    sint                 lcannonStartTime;
    
    sint                 lastCrushTime;        // Tyrant crush
};

typedef struct spawnQueue_s
{
    sint clients[ MAX_CLIENTS ];
    sint front, back;
} spawnQueue_t;

#define QUEUE_PLUS1(x)  (((x)+1)%MAX_CLIENTS)
#define QUEUE_MINUS1(x) (((x)+MAX_CLIENTS-1)%MAX_CLIENTS)

#define MAX_DAMAGE_REGION_TEXT    8192
#define MAX_DAMAGE_REGIONS 16

#define MAX_COUNTRY_LENGTH 50

// store locational damage regions
typedef struct damageRegion_s
{
    valueType name[ 32 ];
    float32 area, modifier, minHeight, maxHeight;
    sint minAngle, maxAngle;
    bool crouch;
} damageRegion_t;

//status of the warning of certain events
typedef enum
{
    TW_NOT = 0,
    TW_IMMINENT,
    TW_PASSED
} timeWarning_t;

//
// this structure is cleared as each map is entered
//
#define MAX_SPAWN_VARS      64
#define MAX_SPAWN_VARS_CHARS  4096

typedef struct
{
    struct gclient_s*  clients;   // [maxclients]
    
    struct gentity_s*  gentities;
    sint               gentitySize;
    sint               num_entities;   // current number, <= MAX_GENTITIES
    
    sint               warmupTime;     // restart match at this time
    
    fileHandle_t      logFile;
    
    // store latched cvars here that we want to get at often
    sint               maxclients;
    
    sint               framenum;
    sint               time;                         // in msec
    sint               previousTime;                 // so movers can back up when blocked
    sint               frameMsec;                    // trap_Milliseconds() at end frame
    
    sint               startTime;                    // level.time the map was started
    
    sint               teamScores[ NUM_TEAMS ];
    sint               lastTeamLocationTime;         // last time of client team location update
    
    bool              newSession;                   // don't use any old session data, because
    // we changed gametype
    bool              restarted;                    // waiting for a map_restart to fire
    
    sint               numConnectedClients;
    sint               numNonSpectatorClients;       // includes connecting clients
    sint               numPlayingClients;            // connected, non-spectators
    sint               sortedClients[MAX_CLIENTS];   // sorted by score
    
    sint               snd_fry;                      // sound index for standing in lava
    
    sint               warmupModificationCount;      // for detecting if g_warmup is changed
    
    // voting state
    valueType              voteString[MAX_STRING_CHARS];
    valueType              voteDisplayString[MAX_STRING_CHARS];
    sint               voteTime;                     // level.time vote was called
    sint               votePassThreshold;            // need at least this percent to pass
    sint               voteExecuteTime;              // time the vote is executed
    sint               voteYes;
    sint               voteNo;
    sint               numVotingClients;             // set by CalculateRanks
    
    // team voting state
    valueType              teamVoteString[ 2 ][ MAX_STRING_CHARS ];
    valueType              teamVoteDisplayString[ 2 ][ MAX_STRING_CHARS ];
    sint               teamVoteTime[ 2 ];            // level.time vote was called
    sint               teamVoteYes[ 2 ];
    sint               teamVoteNo[ 2 ];
    sint               numteamVotingClients[ 2 ];    // set by CalculateRanks
    
    // spawn variables
    bool              spawning;                     // the G_Spawn*() functions are valid
    sint               numSpawnVars;
    valueType*             spawnVars[ MAX_SPAWN_VARS ][ 2 ];  // key / value pairs
    sint               numSpawnVarChars;
    valueType              spawnVarChars[ MAX_SPAWN_VARS_CHARS ];
    
    // intermission state
    sint               intermissionQueued;           // intermission was qualified, but
    // wait INTERMISSION_DELAY_TIME before
    // actually going there so the last
    // frag can be watched.  Disable future
    // kills during this delay
    sint               intermissiontime;             // time the intermission was started
    valueType*             changemap;
    bool              readyToExit;                  // at least one client wants to exit
    sint               exitTime;
    vec3_t            intermission_origin;          // also used for spectator spawns
    vec3_t            intermission_angle;
    
    bool              locationLinked;               // target_locations get linked
    gentity_t*        locationHead;                // head of the location list
    
    sint               numAlienSpawns;
    sint               numHumanSpawns;
    
    sint               numAlienClients;
    sint               numHumanClients;
    
    float32               averageNumAlienClients;
    sint               numAlienSamples;
    float32               averageNumHumanClients;
    sint               numHumanSamples;
    
    sint               numLiveAlienClients;
    sint               numLiveHumanClients;
    
    sint               alienBuildPoints;
    sint               alienBuildPointQueue;
    sint               alienNextQueueTime;
    sint               humanBuildPoints;
    sint               humanBuildPointQueue;
    sint               humanNextQueueTime;
    
    gentity_t*        markedBuildables[ MAX_GENTITIES ];
    sint               numBuildablesForRemoval;
    
    sint               alienKills;
    sint               humanKills;
    
    bool              reactorPresent;
    bool              overmindPresent;
    bool              overmindMuted;
    
    sint               humanBaseAttackTimer;
    
    team_t            lastWin;
    
    bool              suddenDeath;
    sint               suddenDeathBeginTime;
    timeWarning_t     suddenDeathWarning;
    timeWarning_t     timelimitWarning;
    
    spawnQueue_t      alienSpawnQueue;
    spawnQueue_t      humanSpawnQueue;
    
    sint               alienStage2Time;
    sint               alienStage3Time;
    sint               humanStage2Time;
    sint               humanStage3Time;
    
    sint               alienStagedownCredits;    //credits at the time the opposing team
    sint               humanStagedownCredits;   //became vulnerable to a stage-down
    
    
    bool              uncondAlienWin;
    bool              uncondHumanWin;
    bool              alienTeamLocked;
    bool              humanTeamLocked;
    
    sint               unlaggedIndex;
    sint               unlaggedTimes[ MAX_UNLAGGED_MARKERS ];
    
    valueType              layout[ MAX_QPATH ];
    
    team_t            surrenderTeam;
    
    voice_t*          voices;
    
    valueType              emoticons[ MAX_EMOTICONS ][ MAX_EMOTICON_NAME_LEN ];
    sint               emoticonCount;
    sint               numBots;
} level_locals_t;

#define CMD_CHEAT         0x0001
#define CMD_CHEAT_TEAM    0x0002 // is a cheat when used on a team
#define CMD_MESSAGE       0x0004 // sends message to others (skip when muted)
#define CMD_TEAM          0x0008 // must be on a team
#define CMD_SPEC          0x0010 // must be a spectator
#define CMD_ALIEN         0x0020
#define CMD_HUMAN         0x0040
#define CMD_LIVING        0x0080
#define CMD_INTERMISSION  0x0100 // valid during intermission

typedef struct
{
    valueType* cmdName;
    sint  cmdFlags;
    void ( *cmdHandler )( gentity_t* ent );
} commands_t;

#define MAX_ALIEN_BBOX  25

typedef enum
{
    IBE_NONE,
    
    IBE_NOOVERMIND,
    IBE_ONEOVERMIND,
    IBE_NOALIENBP,
    IBE_SPWNWARN, // not currently used
    IBE_NOCREEP,
    IBE_ONEHOVEL,
    IBE_HOVELEXIT,
    
    IBE_ONEREACTOR,
    IBE_NOPOWERHERE,
    IBE_TNODEWARN, // not currently used
    IBE_RPTNOREAC,
    IBE_RPTPOWERHERE,
    IBE_NOHUMANBP,
    IBE_NODCC,
    
    IBE_NORMAL, // too steep
    IBE_NOROOM,
    IBE_PERMISSION,
    IBE_LASTSPAWN,
    
    IBE_MAXERRORS
} itemBuildError_t;

// damage flags
#define DAMAGE_RADIUS         0x00000001  // damage was indirect
#define DAMAGE_NO_ARMOR       0x00000002  // armour does not protect from this damage
#define DAMAGE_NO_KNOCKBACK   0x00000004  // do not affect velocity, just view angles
#define DAMAGE_NO_PROTECTION  0x00000008  // armor, shields, invulnerability, and godmode have no effect
#define DAMAGE_NO_LOCDAMAGE   0x00000010  // do not apply locational damage

#define MAX_ZAP_TARGETS LEVEL2_AREAZAP_MAX_TARGETS

typedef struct zap_s
{
    bool           used;
    gentity_t*     creator;
    gentity_t*     targets[ MAX_ZAP_TARGETS ];
    sint            numTargets;
    sint            timeToLive;
    sint            damageUsed;
    gentity_t*     effectChannel;
} zap_t;

#define MAX_MAP_ROTATIONS       16
#define MAX_MAP_ROTATION_MAPS   64
#define MAX_MAP_COMMANDS        16
#define MAX_MAP_ROTATION_CONDS  4
#define NOT_ROTATING           -1

typedef enum
{
    MCV_ERR,
    MCV_RANDOM,
    MCV_NUMCLIENTS,
    MCV_LASTWIN
} mapConditionVariable_t;

typedef enum
{
    MCO_LT,
    MCO_EQ,
    MCO_GT
} mapConditionOperator_t;

typedef enum
{
    MCT_ERR,
    MCT_MAP,
    MCT_ROTATION
} mapConditionType_t;

typedef struct mapRotationCondition_s
{
    valueType                    dest[ MAX_QPATH ];
    bool                    unconditional;
    mapConditionVariable_t  lhs;
    mapConditionOperator_t  op;
    sint                     numClients;
    team_t                  lastWin;
} mapRotationCondition_t;

typedef struct mapRotationEntry_s
{
    valueType                    name[ MAX_QPATH ];
    valueType                    postCmds[ MAX_MAP_COMMANDS ][ MAX_STRING_CHARS ];
    valueType                    layouts[ MAX_CVAR_VALUE_STRING ];
    sint                     numCmds;
    mapRotationCondition_t  conditions[ MAX_MAP_ROTATION_CONDS ];
    sint                     numConditions;
} mapRotationEntry_t;

typedef struct mapRotation_s
{
    valueType                name[ MAX_QPATH ];
    mapRotationEntry_t  maps[ MAX_MAP_ROTATION_MAPS ];
    sint                 numMaps;
    sint                 currentMap;
} mapRotation_t;

typedef struct mapRotations_s
{
    mapRotation_t   rotations[ MAX_MAP_ROTATIONS ];
    sint             numRotations;
} mapRotations_t;

extern  level_locals_t  level;
extern  gentity_t       g_entities[ MAX_GENTITIES ];

#define FOFS(x)(intptr_t)&(((gentity_t *)0)->x)

extern vmConvar_t mapname;
extern vmConvar_t g_allowShare;
extern vmConvar_t g_dedicated;
extern vmConvar_t g_cheats;
extern vmConvar_t g_maxclients;     // allow this many total, including spectators
extern vmConvar_t g_maxGameClients;   // allow this many active
extern vmConvar_t g_restarted;
extern vmConvar_t g_minCommandPeriod;
extern vmConvar_t g_minNameChangePeriod;
extern vmConvar_t g_maxNameChanges;
extern vmConvar_t g_timelimit;
extern vmConvar_t g_suddenDeathTime;
extern vmConvar_t g_suddenDeath;
extern vmConvar_t g_friendlyFire;
extern vmConvar_t g_friendlyFireHumans;
extern vmConvar_t g_friendlyFireAliens;
extern vmConvar_t g_friendlyBuildableFire;
extern vmConvar_t g_password;
extern vmConvar_t g_needpass;
extern vmConvar_t g_gravity;
extern vmConvar_t g_speed;
extern vmConvar_t g_knockback;
extern vmConvar_t g_quadfactor;
extern vmConvar_t g_inactivity;
extern vmConvar_t g_debugMove;
extern vmConvar_t g_debugAlloc;
extern vmConvar_t g_debugDamage;
extern vmConvar_t g_weaponRespawn;
extern vmConvar_t g_weaponTeamRespawn;
extern vmConvar_t g_synchronousClients;
extern vmConvar_t g_motd;
extern vmConvar_t g_warmup;
extern vmConvar_t g_doWarmup;
extern vmConvar_t g_blood;
extern vmConvar_t g_allowVote;
extern vmConvar_t g_voteLimit;
extern vmConvar_t g_suddenDeathVotePercent;
extern vmConvar_t g_suddenDeathVoteDelay;
extern vmConvar_t g_teamAutoJoin;
extern vmConvar_t g_teamForceBalance;
extern vmConvar_t g_banIPs;
extern vmConvar_t g_filterBan;
extern vmConvar_t g_smoothClients;
extern vmConvar_t pmove_fixed;
extern vmConvar_t pmove_msec;
extern vmConvar_t g_rankings;
extern vmConvar_t g_enableDust;
extern vmConvar_t g_enableBreath;
extern vmConvar_t g_singlePlayer;
extern vmConvar_t g_humanBuildPoints;
extern vmConvar_t g_alienBuildPoints;
extern vmConvar_t g_humanBuildQueueTime;
extern vmConvar_t g_alienBuildQueueTime;
extern vmConvar_t g_humanStage;
extern vmConvar_t g_humanCredits;
extern vmConvar_t g_humanMaxStage;
extern vmConvar_t g_humanMaxReachedStage;
extern vmConvar_t g_humanStageThreshold;
extern vmConvar_t g_alienStage;
extern vmConvar_t g_alienCredits;
extern vmConvar_t g_alienMaxStage;
extern vmConvar_t g_alienMaxReachedStage;
extern vmConvar_t g_alienStageThreshold;
extern vmConvar_t g_unlagged;
extern vmConvar_t g_disabledEquipment;
extern vmConvar_t g_disabledClasses;
extern vmConvar_t g_disabledBuildables;
extern vmConvar_t g_markDeconstruct;
extern vmConvar_t g_debugMapRotation;
extern vmConvar_t g_currentMapRotation;
extern vmConvar_t g_currentMap;
extern vmConvar_t g_initialMapRotation;
extern vmConvar_t g_chatTeamPrefix;
extern vmConvar_t g_sayAreaRange;
extern vmConvar_t g_debugVoices;
extern vmConvar_t g_voiceChats;
extern vmConvar_t g_floodMaxDemerits;
extern vmConvar_t g_floodMinTime;
extern vmConvar_t g_shove;
extern vmConvar_t g_mapConfigs;
extern vmConvar_t g_layouts;
extern vmConvar_t g_layoutAuto;
extern vmConvar_t g_emoticonsAllowedInNames;
extern vmConvar_t g_admin;
extern vmConvar_t g_adminLog;
extern vmConvar_t g_adminParseSay;
extern vmConvar_t g_adminNameProtect;
extern vmConvar_t g_adminTempBan;
extern vmConvar_t g_dretchPunt;
extern vmConvar_t g_privateMessages;
extern vmConvar_t g_publicAdminMessages;
extern vmConvar_t bot_developer;
extern vmConvar_t bot_challenge;
extern vmConvar_t bot_thinktime;
extern vmConvar_t bot_minaliens;
extern vmConvar_t bot_minhumans;
extern vmConvar_t bot_nochat;
extern vmConvar_t bot_fastchat;

//
// idGame
//
class idSGameLocal : public idSGame
{
public:
    idSGameLocal();
    ~idSGameLocal();
    
    virtual void Init( sint levelTime, sint randomSeed, sint restart );
    virtual void Shutdown( sint restart );
    virtual void ClientBegin( sint clientNum );
    virtual valueType* ClientConnect( sint clientNum, bool firstTime );
    virtual void ClientThink( sint clientNum );
    virtual void ClientUserinfoChanged( sint clientNum );
    virtual void ClientDisconnect( sint clientNum );
    virtual void ClientCommand( sint clientNum );
    virtual void RunFrame( sint levelTime );
    virtual bool ConsoleCommand( void );
    virtual bool SnapshotCallback( sint entityNum, sint clientNum );
};

extern idSGameLocal sgameLocal;

#endif //!__G_LOCAL_H__
