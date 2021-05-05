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
// File name:   sgame_main.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <sgame/sgame_precompiled.hpp>

vec3_t playerMins = { -15, -15, -24 };
vec3_t playerMaxs = { 15, 15, 32 };

level_locals_t  level;

typedef struct {
    vmConvar_t *vmCvar;
    valueType *cvarName;
    valueType *defaultString;
    sint cvarFlags;
    valueType *description;
    sint modificationCount;  // for tracking changes
    bool trackChange;       // track this variable, and announce if changed
} cvarTable_t;

gentity_t g_entities[ MAX_GENTITIES ];
gclient_t g_clients[ MAX_CLIENTS ];

vmConvar_t  mapname;
vmConvar_t  g_allowShare;
vmConvar_t  g_fraglimit;
vmConvar_t  g_timelimit;
vmConvar_t  g_suddenDeathTime;
vmConvar_t  g_suddenDeath;
vmConvar_t  g_friendlyFire;
vmConvar_t  g_friendlyFireAliens;
vmConvar_t  g_friendlyFireHumans;
vmConvar_t  g_friendlyBuildableFire;
vmConvar_t  g_password;
vmConvar_t  g_needpass;
vmConvar_t  g_maxclients;
vmConvar_t  g_maxGameClients;
vmConvar_t  g_dedicated;
vmConvar_t  g_speed;
vmConvar_t  g_gravity;
vmConvar_t  g_cheats;
vmConvar_t  g_demoState;
vmConvar_t  g_knockback;
vmConvar_t  g_quadfactor;
vmConvar_t  g_inactivity;
vmConvar_t  g_debugMove;
vmConvar_t  g_debugAlloc;
vmConvar_t  g_debugDamage;
vmConvar_t  g_weaponRespawn;
vmConvar_t  g_weaponTeamRespawn;
vmConvar_t  g_motd;
vmConvar_t  g_synchronousClients;
vmConvar_t  g_warmup;
vmConvar_t  g_doWarmup;
vmConvar_t  g_restarted;
vmConvar_t  g_logFile;
vmConvar_t  g_logFileSync;
vmConvar_t  g_blood;
vmConvar_t  g_podiumDist;
vmConvar_t  g_podiumDrop;
vmConvar_t  g_allowVote;
vmConvar_t  g_voteLimit;
vmConvar_t  g_suddenDeathVotePercent;
vmConvar_t  g_suddenDeathVoteDelay;
vmConvar_t  g_teamAutoJoin;
vmConvar_t  g_teamForceBalance;
vmConvar_t  g_banIPs;
vmConvar_t  g_filterBan;
vmConvar_t  g_smoothClients;
vmConvar_t  pmove_fixed;
vmConvar_t  pmove_msec;
vmConvar_t  g_rankings;
vmConvar_t  g_listEntity;
vmConvar_t  g_minCommandPeriod;
vmConvar_t  g_minNameChangePeriod;
vmConvar_t  g_maxNameChanges;
vmConvar_t  g_humanBuildPoints;
vmConvar_t  g_alienBuildPoints;
vmConvar_t  g_humanBuildQueueTime;
vmConvar_t  g_alienBuildQueueTime;
vmConvar_t  g_humanStage;
vmConvar_t  g_humanCredits;
vmConvar_t  g_humanMaxStage;
vmConvar_t  g_humanMaxReachedStage;
vmConvar_t  g_humanStageThreshold;
vmConvar_t  g_alienStage;
vmConvar_t  g_alienCredits;
vmConvar_t  g_alienMaxStage;
vmConvar_t  g_alienMaxReachedStage;
vmConvar_t  g_alienStageThreshold;
vmConvar_t  g_allowStageDowns;
vmConvar_t  g_unlagged;
vmConvar_t  g_disabledEquipment;
vmConvar_t  g_disabledClasses;
vmConvar_t  g_disabledBuildables;
vmConvar_t  g_markDeconstruct;
vmConvar_t  g_debugMapRotation;
vmConvar_t  g_currentMapRotation;
vmConvar_t  g_currentMap;
vmConvar_t  g_initialMapRotation;
vmConvar_t  g_debugVoices;
vmConvar_t  g_voiceChats;
vmConvar_t  g_shove;
vmConvar_t  g_mapConfigs;
vmConvar_t  g_chatTeamPrefix;
vmConvar_t  g_sayAreaRange;
vmConvar_t  g_floodMaxDemerits;
vmConvar_t  g_floodMinTime;
vmConvar_t  g_layouts;
vmConvar_t  g_layoutAuto;
vmConvar_t  g_emoticonsAllowedInNames;
vmConvar_t  g_newbieNameNumbering;
vmConvar_t  g_newbieNamePrefix;
vmConvar_t  g_admin;
vmConvar_t  g_adminLog;
vmConvar_t  g_adminParseSay;
vmConvar_t  g_adminNameProtect;
vmConvar_t  g_adminTempBan;
vmConvar_t  g_dretchPunt;
vmConvar_t  g_privateMessages;
vmConvar_t  g_specChat;
vmConvar_t  g_publicAdminMessages;
vmConvar_t  g_censorship;
vmConvar_t  g_tag;
vmConvar_t  bot_developer;
vmConvar_t  bot_challenge;
vmConvar_t  bot_thinktime;
vmConvar_t  bot_minaliens;
vmConvar_t  bot_minhumans;
vmConvar_t  bot_nochat;
vmConvar_t  bot_fastchat;

idSGameLocal sgameLocal;
idSGame *sgame = &sgameLocal;

static cvarTable_t   gameCvarTable[ ] = {
    // don't override the cheat state set by the system
    { &g_cheats, "sv_cheats", "", 0, 0, false },

    // demo state
    { &g_demoState, "sv_demoState", "", 0, 0, false },

    // noset vars
    { nullptr, "gamename", GAME_VERSION, CVAR_SERVERINFO | CVAR_ROM, 0, false  },
    { nullptr, "gamedate", __DATE__, CVAR_ROM, 0, false  },
    { &g_restarted, "g_restarted", "0", CVAR_ROM, 0, false  },
    { nullptr, "sv_mapname", "", CVAR_SERVERINFO | CVAR_ROM, 0, false  },
    { nullptr, "P", "", CVAR_SERVERINFO | CVAR_ROM, 0, false  },
    { nullptr, "ff", "0", CVAR_SERVERINFO | CVAR_ROM, 0, false  },

    // latched vars
    { &g_maxclients, "sv_maxclients", "8", CVAR_SERVERINFO | CVAR_LATCH | CVAR_ARCHIVE, 0, false  },

    // change anytime vars
    { &g_maxGameClients, "g_maxGameClients", "0", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, false  },

    { &g_timelimit, "timelimit", "0", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, true },
    { &g_suddenDeathTime, "g_suddenDeathTime", "0", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, true },
    { &g_suddenDeath, "g_suddenDeath", "0", CVAR_SERVERINFO | CVAR_NORESTART, 0, true },

    { &g_synchronousClients, "g_synchronousClients", "0", CVAR_SYSTEMINFO, 0, false  },

    { &g_friendlyFire, "g_friendlyFire", "0", CVAR_ARCHIVE, 0, true  },
    { &g_friendlyFireAliens, "g_friendlyFireAliens", "0", CVAR_ARCHIVE, 0, true  },
    { &g_friendlyFireHumans, "g_friendlyFireHumans", "0", CVAR_ARCHIVE, 0, true  },
    { &g_friendlyBuildableFire, "g_friendlyBuildableFire", "0", CVAR_ARCHIVE, 0, true  },

    { &g_teamAutoJoin, "g_teamAutoJoin", "0", CVAR_ARCHIVE  },
    { &g_teamForceBalance, "g_teamForceBalance", "0", CVAR_ARCHIVE  },

    { &g_warmup, "g_warmup", "20", CVAR_ARCHIVE, 0, true  },
    { &g_doWarmup, "g_doWarmup", "0", 0, 0, true  },
    { &g_logFile, "g_logFile", "games.log", CVAR_ARCHIVE, 0, false  },
    { &g_logFileSync, "g_logFileSync", "0", CVAR_ARCHIVE, 0, false  },

    { &g_password, "g_password", "", CVAR_USERINFO, 0, false  },

    { &g_banIPs, "g_banIPs", "", CVAR_ARCHIVE, 0, false  },
    { &g_filterBan, "g_filterBan", "1", CVAR_ARCHIVE, 0, false  },

    { &g_needpass, "g_needpass", "0", CVAR_SERVERINFO | CVAR_ROM, 0, false },

    { &g_dedicated, "dedicated", "0", 0, 0, false  },
    { &g_allowShare, "g_allowShare", "1", CVAR_ARCHIVE | CVAR_SERVERINFO, 0, false },
    { &g_speed, "g_speed", "320", 0, 0, true  },
    { &g_gravity, "g_gravity", "800", 0, 0, true  },
    { &g_knockback, "g_knockback", "1000", 0, 0, true  },
    { &g_quadfactor, "g_quadfactor", "3", 0, 0, true  },
    { &g_weaponRespawn, "g_weaponrespawn", "5", 0, 0, true  },
    { &g_weaponTeamRespawn, "g_weaponTeamRespawn", "30", 0, 0, true },
    { &g_inactivity, "g_inactivity", "0", 0, 0, true },
    { &g_debugMove, "g_debugMove", "0", 0, 0, false },
    { &g_debugAlloc, "g_debugAlloc", "0", 0, 0, false },
    { &g_debugDamage, "g_debugDamage", "0", 0, 0, false },
    { &g_motd, "g_motd", "", 0, 0, false },
    { &g_blood, "com_blood", "1", 0, 0, false },

    { &g_podiumDist, "g_podiumDist", "80", 0, 0, false },
    { &g_podiumDrop, "g_podiumDrop", "70", 0, 0, false },

    { &g_allowVote, "g_allowVote", "1", CVAR_ARCHIVE, 0, false },
    { &g_voteLimit, "g_voteLimit", "5", CVAR_ARCHIVE, 0, false },
    { &g_suddenDeathVotePercent, "g_suddenDeathVotePercent", "74", CVAR_ARCHIVE, 0, false },
    { &g_suddenDeathVoteDelay, "g_suddenDeathVoteDelay", "180", CVAR_ARCHIVE, 0, false },
    { &g_listEntity, "g_listEntity", "0", 0, 0, false },
    { &g_minCommandPeriod, "g_minCommandPeriod", "500", 0, 0, false},
    { &g_minNameChangePeriod, "g_minNameChangePeriod", "5", 0, 0, false},
    { &g_maxNameChanges, "g_maxNameChanges", "5", 0, 0, false},

    { &g_smoothClients, "g_smoothClients", "1", 0, 0, false},
    { &pmove_fixed, "pmove_fixed", "0", CVAR_SYSTEMINFO, 0, false},
    { &pmove_msec, "pmove_msec", "8", CVAR_SYSTEMINFO, 0, false},

    { &g_humanBuildPoints, "g_humanBuildPoints", DEFAULT_HUMAN_BUILDPOINTS, 0, 0, false  },
    { &g_alienBuildPoints, "g_alienBuildPoints", DEFAULT_ALIEN_BUILDPOINTS, 0, 0, false  },
    { &g_humanBuildQueueTime, "g_humanBuildQueueTime", DEFAULT_HUMAN_QUEUE_TIME, 0, 0, false  },
    { &g_alienBuildQueueTime, "g_alienBuildQueueTime", DEFAULT_ALIEN_QUEUE_TIME, 0, 0, false  },
    { &g_humanStage, "g_humanStage", "0", 0, 0, false  },
    { &g_humanCredits, "g_humanCredits", "0", 0, 0, false  },
    { &g_humanMaxStage, "g_humanMaxStage", DEFAULT_HUMAN_MAX_STAGE, 0, 0, false  },
    { &g_humanMaxReachedStage, "g_humanMaxReachedStage", "0", 0, 0, false },
    { &g_humanStageThreshold, "g_humanStageThreshold", DEFAULT_HUMAN_STAGE_THRESH, 0, 0, false  },
    { &g_alienStage, "g_alienStage", "0", 0, 0, false  },
    { &g_alienCredits, "g_alienCredits", "0", 0, 0, false  },
    { &g_alienMaxStage, "g_alienMaxStage", DEFAULT_ALIEN_MAX_STAGE, 0, 0, false  },
    { &g_alienMaxReachedStage, "g_alienMaxReachedStage", "0", 0, 0, false },
    { &g_alienStageThreshold, "g_alienStageThreshold", DEFAULT_ALIEN_STAGE_THRESH, 0, 0, false  },

    { &g_unlagged, "g_unlagged", "1", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, false  },

    { &g_disabledEquipment, "g_disabledEquipment", "", CVAR_ROM, 0, false  },
    { &g_disabledClasses, "g_disabledClasses", "", CVAR_ROM, 0, false  },
    { &g_disabledBuildables, "g_disabledBuildables", "", CVAR_ROM, 0, false  },

    { &g_chatTeamPrefix, "g_chatTeamPrefix", "0", CVAR_ARCHIVE, 0, false  },
    { &g_sayAreaRange, "g_sayAreaRange", "1000.0", CVAR_ARCHIVE, 0, true  },

    { &g_floodMaxDemerits, "g_floodMaxDemerits", "5000", CVAR_ARCHIVE, 0, false  },
    { &g_floodMinTime, "g_floodMinTime", "2000", CVAR_ARCHIVE, 0, false  },

    { &g_markDeconstruct, "g_markDeconstruct", "1", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, false  },

    { &g_debugMapRotation, "g_debugMapRotation", "0", 0, 0, false  },
    { &g_currentMapRotation, "g_currentMapRotation", "-1", 0, 0, false  }, // -1 = NOT_ROTATING
    { &g_currentMap, "g_currentMap", "0", 0, 0, false  },
    { &g_initialMapRotation, "g_initialMapRotation", "", CVAR_ARCHIVE, 0, false  },
    { &g_debugVoices, "g_debugVoices", "0", 0, 0, false  },
    { &g_voiceChats, "g_voiceChats", "1", CVAR_ARCHIVE, 0, false },
    { &g_shove, "g_shove", "0.0", CVAR_ARCHIVE, 0, false  },
    { &g_mapConfigs, "g_mapConfigs", "", CVAR_ARCHIVE, 0, false  },
    { nullptr, "g_mapConfigsLoaded", "0", CVAR_ROM, 0, false  },

    { &g_layouts, "g_layouts", "", CVAR_LATCH, 0, false  },
    { &g_layoutAuto, "g_layoutAuto", "1", CVAR_ARCHIVE, 0, false  },

    { &g_emoticonsAllowedInNames, "g_emoticonsAllowedInNames", "1", CVAR_LATCH | CVAR_ARCHIVE, 0, false  },
    { &g_newbieNameNumbering, "g_newbieNameNumbering", "1", CVAR_ARCHIVE, 0, false  },
    { &g_newbieNamePrefix, "g_newbieNamePrefix", "UnnamedPlayer#", CVAR_ARCHIVE, 0, false  },

    { &g_admin, "g_admin", "admin.dat", CVAR_ARCHIVE, 0, false  },
    { &g_adminLog, "g_adminLog", "admin.log", CVAR_ARCHIVE, 0, false  },
    { &g_adminParseSay, "g_adminParseSay", "1", CVAR_ARCHIVE, 0, false  },
    { &g_adminNameProtect, "g_adminNameProtect", "1", CVAR_ARCHIVE, 0, false  },
    { &g_adminTempBan, "g_adminTempBan", "2m", CVAR_ARCHIVE, 0, false  },

    { &g_dretchPunt, "g_dretchPunt", "0", CVAR_ARCHIVE, 0, false  },

    { &g_privateMessages, "g_privateMessages", "1", CVAR_ARCHIVE, 0, false  },
    { &g_specChat, "g_specChat", "1", CVAR_ARCHIVE, 0, false  },
    { &g_publicAdminMessages, "g_publicAdminMessages", "1", CVAR_ARCHIVE, 0, false  },

    { &g_censorship, "g_censorship", "", CVAR_ARCHIVE, 0, false  },

    { &g_tag, "g_tag", "main", CVAR_INIT, 0, false },

    //Start Champ bot cvars
    { &bot_thinktime, "bot_thinktime", "100", CVAR_CHEAT, 0, false  },
    { &bot_minaliens, "bot_minaliens", "0", CVAR_SERVERINFO, 0, false  },
    { &bot_minhumans, "bot_minhumans", "0", CVAR_SERVERINFO, 0, false  },
    { &bot_developer, "bot_developer", "0", CVAR_SERVERINFO, 0, false  },
    { &bot_challenge, "bot_challenge", "0", CVAR_SERVERINFO, 0, false  },
    { &bot_nochat,    "bot_nochat",    "0", CVAR_TEMP, 0, false  },
    { &bot_fastchat,  "bot_fastchat",  "0", CVAR_TEMP, 0, false  },
    //End Champ bot cvars

    { &g_rankings, "g_rankings", "0", 0, 0, false}
};

static sint gameCvarTableSize = sizeof(gameCvarTable) / sizeof(
                                    gameCvarTable[ 0 ]);

bool idSGameLocal::SnapshotCallback(sint entityNum, sint clientNum) {
#if 0
    gentity_t *ent = &g_entities[ entityNum ];

    if(ent->s.eType == ET_MISSILE) {
        if(ent->s.weapon == WP_LANDMINE) {
            return G_LandmineSnapshotCallback(entityNum, clientNum);
        }
    }

#endif

    return true;
}

void idSGameMain::Printf(pointer fmt, ...) {
    va_list argptr;
    valueType    text[ 1024 ];

    va_start(argptr, fmt);
    Q_vsprintf_s(text, sizeof(text), fmt, argptr);
    va_end(argptr);

    trap_Print(text);
}

void idSGameMain::Error(pointer fmt, ...) {
    va_list argptr;
    valueType    text[ 1024 ];

    va_start(argptr, fmt);
    Q_vsprintf_s(text, sizeof(text), fmt, argptr);
    va_end(argptr);

    trap_Error(text);
}

/*
================
idSGameMain::FindTeams

Chain together all entities with a matching team field.
Entity teams are used for item groups and multi-entity mover groups.

All but the first will have the FL_TEAMSLAVE flag set and teammaster field set
All but the last will have the teamchain field set to the next one
================
*/
void idSGameMain::FindTeams(void) {
    gentity_t *e, *e2;
    sint       i, j;
    sint       c, c2;

    c = 0;
    c2 = 0;

    for(i = 1, e = g_entities + i; i < level.num_entities; i++, e++) {
        if(!e->inuse) {
            continue;
        }

        if(!e->team) {
            continue;
        }

        if(e->flags & FL_TEAMSLAVE) {
            continue;
        }

        e->teammaster = e;
        c++;
        c2++;

        for(j = i + 1, e2 = e + 1; j < level.num_entities; j++, e2++) {
            if(!e2->inuse) {
                continue;
            }

            if(!e2->team) {
                continue;
            }

            if(e2->flags & FL_TEAMSLAVE) {
                continue;
            }

            if(!strcmp(e->team, e2->team)) {
                c2++;
                e2->teamchain = e->teamchain;
                e->teamchain = e2;
                e2->teammaster = e;
                e2->flags |= FL_TEAMSLAVE;

                // make sure that targets only point at the master
                if(e2->targetname) {
                    e->targetname = e2->targetname;
                    e2->targetname = nullptr;
                }
            }
        }
    }

    Printf("%i teams with %i entities\n", c, c2);
}


/*
=================
idSGameMain::RegisterCvars
=================
*/
void idSGameMain::RegisterCvars(void) {
    sint         i;
    cvarTable_t *cv;

    for(i = 0, cv = gameCvarTable; i < gameCvarTableSize; i++, cv++) {
        trap_Cvar_Register(cv->vmCvar, cv->cvarName, cv->defaultString,
                           cv->cvarFlags, cv->description);

        if(cv->vmCvar) {
            cv->modificationCount = cv->vmCvar->modificationCount;
        }
    }

    // check some things
    level.warmupModificationCount = g_warmup.modificationCount;
}

/*
=================
idSGameMain::UpdateCvars
=================
*/
void idSGameMain::UpdateCvars(void) {
    sint         i;
    cvarTable_t *cv;

    for(i = 0, cv = gameCvarTable; i < gameCvarTableSize; i++, cv++) {
        if(cv->vmCvar) {
            trap_Cvar_Update(cv->vmCvar);

            if(cv->modificationCount != cv->vmCvar->modificationCount) {
                if(cv->trackChange) {
                    trap_SendServerCommand(-1, va("print \"Server: %s changed to %s\n\"",
                                                  cv->cvarName, cv->vmCvar->string));

                    // update serverinfo in case this cvar is passed to clients indirectly
                    CalculateRanks();
                }
            }
        }
    }
}

/*
=================
idSGameMain::MapConfigs
=================
*/
void idSGameMain::MapConfigs(pointer mapname) {
    if(!g_mapConfigs.string[0]) {
        return;
    }

    if(trap_Cvar_VariableIntegerValue("g_mapConfigsLoaded")) {
        return;
    }

    trap_SendConsoleCommand(EXEC_APPEND, va("exec \"%s/default.cfg\"\n",
                                            g_mapConfigs.string));

    trap_SendConsoleCommand(EXEC_APPEND, va("exec \"%s/%s.cfg\"\n",
                                            g_mapConfigs.string, mapname));

    trap_Cvar_Set("g_mapConfigsLoaded", "1");
}

idSGameLocal::idSGameLocal(void) {
}

idSGameLocal::~idSGameLocal(void) {
}

idSGameMain::idSGameMain(void) {
}

idSGameMain::~idSGameMain(void) {
}

/*
============
idSGameLocal::Init
============
*/
void idSGameLocal::Init(sint levelTime, sint randomSeed, sint restart) {
    sint i;
    valueType buffer[MAX_CVAR_VALUE_STRING];
    sint a, b;
    valueType mapName[128];

    srand(randomSeed);

    idSGameMain::RegisterCvars();

    idSGameMain::Printf("------- idSGameLocal::Init : Game Initialization -------\n");
    idSGameMain::Printf("idSGameLocal::Init:Gamename: %s\n", GAME_VERSION);
    idSGameMain::Printf("idSGameLocal::Init:Gamedate: %s\n", __DATE__);

    trap_Cvar_Register(&mapname, "mapname", "", CVAR_SERVERINFO | CVAR_ROM,
                       "description");
    idSGameMain::Printf("mapname: %s\n", mapname.string);

    bggame->InitMemory();

    // set some level globals
    ::memset(&level, 0, sizeof(level));
    level.time = levelTime;
    level.startTime = levelTime;
    level.alienStage2Time = level.alienStage3Time = level.humanStage2Time =
                                level.humanStage3Time = level.startTime;
    level.alienStagedownCredits = level.humanStagedownCredits = -1;
    trap_Cvar_VariableStringBuffer("session", buffer, sizeof(buffer));
    sscanf(buffer, "%i %i", &a, &b);

    if(a != trap_Cvar_VariableIntegerValue("sv_maxclients") ||
            b != trap_Cvar_VariableIntegerValue("sv_democlients")) {
        level.newSession = true;
    }

    level.snd_fry =
        idSGameUtils::SoundIndex("sound/misc/fry.wav");   // FIXME standing in lava / slime

    if(g_logFile.string[0]) {
        if(g_logFileSync.integer) {
            trap_FS_FOpenFile(g_logFile.string, &level.logFile, FS_APPEND_SYNC);
        } else {
            trap_FS_FOpenFile(g_logFile.string, &level.logFile, FS_APPEND);
        }

        if(!level.logFile) {
            idSGameMain::Printf("WARNING: Couldn't open logfile: %s\n",
                                g_logFile.string);
        } else {
            valueType serverinfo[MAX_INFO_STRING];
            qtime_t qt;
            sint t;

            trap_GetServerinfo(serverinfo, sizeof(serverinfo));

            idSGameMain::LogPrintf("------------------------------------------------------------\n");
            idSGameMain::LogPrintf("InitGame: %s\n", serverinfo);

            t = trap_RealTime(&qt);
            idSGameMain::LogPrintf("RealTime: %04i/%02i/%02i %02i:%02i:%02i\n",
                                   qt.tm_year + 1900, qt.tm_mon + 1, qt.tm_mday,
                                   qt.tm_hour, qt.tm_min, qt.tm_sec);

        }
    } else {
        idSGameMain::Printf("Not logging to disk\n");
    }

    valueType map[MAX_CVAR_VALUE_STRING] = { "" };

    trap_Cvar_VariableStringBuffer("mapname", map, sizeof(map));
    idSGameMain::MapConfigs(map);

    // we're done with g_mapConfigs, so reset this for the next map
    trap_Cvar_Set("g_mapConfigsLoaded", "0");

    adminLocal.AdminReadconfig(nullptr, 0);
    idSGameCmds::LoadCensors();

    // initialize all entities for this game
    ::memset(g_entities, 0, MAX_GENTITIES * sizeof(g_entities[0]));
    level.gentities = g_entities;

    // initialize all clients for this game
    level.maxclients = g_maxclients.integer;
    ::memset(g_clients, 0, MAX_CLIENTS * sizeof(g_clients[0]));
    level.clients = g_clients;

    // set client fields on player ents
    for(i = 0; i < level.maxclients; i++) {
        g_entities[i].client = level.clients + i;
    }

    // always leave room for the max number of clients,
    // even if they aren't all used, so numbers inside that
    // range are NEVER anything but clients
    level.num_entities = MAX_CLIENTS;

    // let the server system know where the entites are
    trap_LocateGameData(level.gentities, level.num_entities, sizeof(gentity_t),
                        &level.clients[0].ps, sizeof(level.clients[0]));

    level.emoticonCount = bggame->LoadEmoticons(level.emoticons, nullptr);

    trap_SetConfigstring(CS_INTERMISSION, "0");

    // test to see if a custom buildable layout will be loaded
    idSGameBuildable::LayoutSelect();

    // parse the key/value pairs and spawn gentities
    idSGameSpawn::SpawnEntitiesFromString();

    // load up a custom building layout if there is one
    idSGameBuildable::LayoutLoad();

    // the map might disable some things
    bggame->InitAllowedGameElements();

    //Init bullet physics
    trap_Cvar_VariableStringBuffer("mapname", mapName, sizeof(mapName));

    // general initialization
    idSGameMain::FindTeams();

    bggame->InitClassConfigs();
    bggame->InitBuildableConfigs();
    idSGameCombat::InitDamageLocations();
    idSGameMaprotation::InitMapRotations();
    idSGameMain::InitSpawnQueue(&level.alienSpawnQueue);
    idSGameMain::InitSpawnQueue(&level.humanSpawnQueue);

    if(g_debugMapRotation.integer) {
        idSGameMaprotation::PrintRotations();
    }

    level.voices = bggame->VoiceInit();
    bggame->PrintVoices(level.voices, g_debugVoices.integer);

    //reset stages
    trap_Cvar_Set("g_alienStage", va("%d", S1));
    trap_Cvar_Set("g_humanStage", va("%d", S1));
    trap_Cvar_Set("g_humanMaxReachedStage", va("%d", S1));
    trap_Cvar_Set("g_alienMaxReachedStage", va("%d", S1));
    trap_Cvar_Set("g_alienCredits", 0);
    trap_Cvar_Set("g_humanCredits", 0);
    trap_Cvar_Set("g_suddenDeath", 0);
    level.suddenDeathBeginTime = g_suddenDeathTime.integer * 60000;

    idSGameMain::Printf("-----------------------------------\n");

    // so the server counts the spawns without a client attached
    idSGameMain::CountSpawns();

    idSGamePtr::ResetPTRConnections();
}

/*
==================
idSGameMain::ClearVotes

remove all currently active votes
==================
*/
void idSGameMain::ClearVotes(void) {
    level.voteTime = 0;
    trap_SetConfigstring(CS_VOTE_TIME, "");
    level.teamVoteTime[ 0 ] = 0;
    trap_SetConfigstring(CS_TEAMVOTE_TIME, "");
    level.teamVoteTime[ 1 ] = 0;
    trap_SetConfigstring(CS_TEAMVOTE_TIME + 1, "");
}

/*
=================
idSGameLocal::Shutdown
=================
*/
void idSGameLocal::Shutdown(sint restart) {
    sint i, clients;

    // in case of a map_restart
    idSGameMain::ClearVotes();

    idSGameMain::Printf("==== idGameLocal::Shutdown ====\n");

    if(level.logFile) {
        idSGameMain::LogPrintf("idGameLocal::Shutdown:\n");
        idSGameMain::LogPrintf("------------------------------------------------------------\n");
        trap_FS_FCloseFile(level.logFile);
    }

    // write all the client session data so we can get it back
    idSGameSession::WriteSessionData();

    adminLocal.AdminCleanup();
    adminLocal.AdminNamelogCleanup();

    level.restarted = false;
    level.surrenderTeam = TEAM_NONE;
    trap_SetConfigstring(CS_WINNER, "");

    // clear all demo clients
    clients = trap_Cvar_VariableIntegerValue("sv_democlients");

    for(i = 0; i < clients; i++) {
        trap_SetConfigstring(CS_PLAYERS + i, nullptr);
    }
}

void Com_Error(sint level, pointer error, ...) {
    va_list argptr;
    valueType    text[ 1024 ];

    va_start(argptr, error);
    Q_vsprintf_s(text, sizeof(text), error, argptr);
    va_end(argptr);

    idSGameMain::Error("%s", text);
}

void Com_Printf(pointer msg, ...) {
    va_list argptr;
    valueType    text[ 1024 ];

    va_start(argptr, msg);
    Q_vsprintf_s(text, sizeof(text), msg, argptr);
    va_end(argptr);

    idSGameMain::Printf("%s", text);
}

/*
========================================================================

PLAYER COUNTING / SCORE SORTING

========================================================================
*/


/*
=============
idSGameMain::SortRanks
=============
*/
sint idSGameMain::SortRanks(const void *a, const void *b) {
    gclient_t *ca, *cb;

    ca = &level.clients[ *(sint *)a ];
    cb = &level.clients[ *(sint *)b ];

    // then sort by score
    if(ca->ps.persistant[ PERS_SCORE ] > cb->ps.persistant[ PERS_SCORE ]) {
        return -1;
    }

    if(ca->ps.persistant[ PERS_SCORE ] < cb->ps.persistant[ PERS_SCORE ]) {
        return 1;
    }

    return 0;
}

/*
============
idSGameMain::InitSpawnQueue

Initialise a spawn queue
============
*/
void idSGameMain::InitSpawnQueue(spawnQueue_t *sq) {
    sint i;

    sq->back = sq->front = 0;
    sq->back = QUEUE_MINUS1(sq->back);

    //0 is a valid clientNum, so use something else
    for(i = 0; i < MAX_CLIENTS; i++) {
        sq->clients[ i ] = -1;
    }
}

/*
============
idSGameMain::GetSpawnQueueLength

Return the length of a spawn queue
============
*/
sint idSGameMain::GetSpawnQueueLength(spawnQueue_t *sq) {
    sint length = sq->back - sq->front + 1;

    while(length < 0) {
        length += MAX_CLIENTS;
    }

    while(length >= MAX_CLIENTS) {
        length -= MAX_CLIENTS;
    }

    return length;
}

/*
============
idSGameMain::PopSpawnQueue

Remove from front element from a spawn queue
============
*/
sint idSGameMain::PopSpawnQueue(spawnQueue_t *sq) {
    sint clientNum = sq->clients[ sq->front ];

    if(GetSpawnQueueLength(sq) > 0) {
        sq->clients[ sq->front ] = -1;
        sq->front = QUEUE_PLUS1(sq->front);
        idSGameCmds::StopFollowing(g_entities + clientNum);
        g_entities[ clientNum ].client->ps.pm_flags &= ~PMF_QUEUED;

        return clientNum;
    }

    return -1;
}

/*
============
idSGameMain::PeekSpawnQueue

Look at front element from a spawn queue
============
*/
sint idSGameMain::PeekSpawnQueue(spawnQueue_t *sq) {
    return sq->clients[ sq->front ];
}

/*
============
idSGameMain::SearchSpawnQueue

Look to see if clientNum is already in the spawnQueue
============
*/
bool idSGameMain::SearchSpawnQueue(spawnQueue_t *sq, sint clientNum) {
    sint i;

    for(i = 0; i < MAX_CLIENTS; i++)
        if(sq->clients[ i ] == clientNum) {
            return true;
        }

    return false;
}

/*
============
idSGameMain::PushSpawnQueue

Add an element to the back of the spawn queue
============
*/
bool idSGameMain::PushSpawnQueue(spawnQueue_t *sq, sint clientNum) {
    // don't add the same client more than once
    if(SearchSpawnQueue(sq, clientNum)) {
        return false;
    }

    sq->back = QUEUE_PLUS1(sq->back);
    sq->clients[ sq->back ] = clientNum;

    g_entities[ clientNum ].client->ps.pm_flags |= PMF_QUEUED;
    return true;
}

/*
============
idSGameMain::RemoveFromSpawnQueue

remove a specific client from a spawn queue
============
*/
bool idSGameMain::RemoveFromSpawnQueue(spawnQueue_t *sq, sint clientNum) {
    sint i = sq->front;

    if(GetSpawnQueueLength(sq)) {
        do {
            if(sq->clients[ i ] == clientNum) {
                //and this kids is why it would have
                //been better to use an LL for internal
                //representation
                do {
                    sq->clients[ i ] = sq->clients[ QUEUE_PLUS1(i) ];

                    i = QUEUE_PLUS1(i);
                } while(i != QUEUE_PLUS1(sq->back));

                sq->back = QUEUE_MINUS1(sq->back);
                g_entities[ clientNum ].client->ps.pm_flags &= ~PMF_QUEUED;

                return true;
            }

            i = QUEUE_PLUS1(i);
        } while(i != QUEUE_PLUS1(sq->back));
    }

    return false;
}

/*
============
idSGameMain::GetPosInSpawnQueue

Get the position of a client in a spawn queue
============
*/
sint idSGameMain::GetPosInSpawnQueue(spawnQueue_t *sq, sint clientNum) {
    sint i = sq->front;

    if(GetSpawnQueueLength(sq)) {
        do {
            if(sq->clients[ i ] == clientNum) {
                if(i < sq->front) {
                    return i + MAX_CLIENTS - sq->front;
                } else {
                    return i - sq->front;
                }
            }

            i = QUEUE_PLUS1(i);
        } while(i != QUEUE_PLUS1(sq->back));
    }

    return -1;
}

/*
============
idSGameMain::PrintSpawnQueue

Print the contents of a spawn queue
============
*/
void idSGameMain::PrintSpawnQueue(spawnQueue_t *sq) {
    sint i = sq->front;
    sint length = GetSpawnQueueLength(sq);

    Printf("l:%d f:%d b:%d    :", length, sq->front, sq->back);

    if(length > 0) {
        do {
            if(sq->clients[ i ] == -1) {
                Printf("*:");
            } else {
                Printf("%d:", sq->clients[ i ]);
            }

            i = QUEUE_PLUS1(i);
        } while(i != QUEUE_PLUS1(sq->back));
    }

    Printf("\n");
}

/*
============
idSGameMain::SpawnClients

Spawn queued clients
============
*/
void idSGameMain::SpawnClients(team_t team) {
    sint           clientNum;
    gentity_t     *ent, *spawn;
    vec3_t        spawn_origin, spawn_angles;
    spawnQueue_t  *sq = nullptr;
    sint           numSpawns = 0;

    if(team == TEAM_ALIENS) {
        sq = &level.alienSpawnQueue;
        numSpawns = level.numAlienSpawns;
    } else if(team == TEAM_HUMANS) {
        sq = &level.humanSpawnQueue;
        numSpawns = level.numHumanSpawns;
    }

    if(GetSpawnQueueLength(sq) > 0 && numSpawns > 0) {
        clientNum = PeekSpawnQueue(sq);
        ent = &g_entities[ clientNum ];

        if((spawn = idSGameClient::SelectTremulousSpawnPoint(team,
                    ent->client->pers.lastDeathLocation, spawn_origin, spawn_angles))) {
            clientNum = PopSpawnQueue(sq);

            if(clientNum < 0) {
                return;
            }

            ent = &g_entities[ clientNum ];

            ent->client->sess.spectatorState = SPECTATOR_NOT;
            sgameLocal.ClientUserinfoChanged(clientNum);
            idSGameClient::ClientSpawn(ent, spawn, spawn_origin, spawn_angles);
        }
    }
}

/*
============
G_CountSpawns

Counts the number of spawns for each team
============
*/
void idSGameMain::CountSpawns(void) {
    sint i;
    gentity_t *ent;

    level.numAlienSpawns = 0;
    level.numHumanSpawns = 0;

    for(i = 1, ent = g_entities + i ; i < level.num_entities ; i++, ent++) {
        if(!ent->inuse) {
            continue;
        }

        if(ent->s.modelindex == BA_A_SPAWN && ent->health > 0) {
            level.numAlienSpawns++;
        }

        if(ent->s.modelindex == BA_H_SPAWN && ent->health > 0) {
            level.numHumanSpawns++;
        }
    }
}

/*
============
idSGameMain::TimeTilSuddenDeath
============
*/
sint idSGameMain::TimeTilSuddenDeath(void) {
    if((!g_suddenDeathTime.integer && level.suddenDeathBeginTime == 0) ||
            (level.suddenDeathBeginTime < 0)) {
        return 1;    // Always some time away
    }

    return ((level.suddenDeathBeginTime) - (level.time - level.startTime));
}


#define PLAYER_COUNT_MOD 5.0f

/*
============
G_CalculateBuildPoints

Recalculate the quantity of building points available to the teams
============
*/
void idSGameMain::CalculateBuildPoints(void) {
    sint         i;
    buildable_t buildable;
    gentity_t   *ent;
    sint         localHTP = level.humanBuildPoints =
                                g_humanBuildPoints.integer,
                                localATP = level.alienBuildPoints = g_alienBuildPoints.integer;

    // BP queue updates
    while(level.alienBuildPointQueue > 0 &&
            level.alienNextQueueTime < level.time) {
        level.alienBuildPointQueue--;
        level.alienNextQueueTime += g_alienBuildQueueTime.integer;
    }

    while(level.humanBuildPointQueue > 0 &&
            level.humanNextQueueTime < level.time) {
        level.humanBuildPointQueue--;
        level.humanNextQueueTime += g_alienBuildQueueTime.integer;
    }

    // Sudden Death checks

    // Note: g_suddenDeath sets what is going on and level.suddenDeath
    // trails it to run stuff here. They're only inconsistent for as long
    // as it takes this function to run once and update

    // reset if SD was on, but now it's off
    if(!g_suddenDeath.integer && level.suddenDeath) {
        level.suddenDeath = false;
        level.suddenDeathWarning = (timeWarning_t)0;
        level.suddenDeathBeginTime = -1;

        if((level.time - level.startTime) < (g_suddenDeathTime.integer * 60000)) {
            level.suddenDeathBeginTime = g_suddenDeathTime.integer * 60000;
        } else {
            level.suddenDeathBeginTime = -1;
        }
    }

    // SD checks and warnings
    if(!level.suddenDeath) {
        // check conditions to enter sudden death
        if(!level.warmupTime && (g_suddenDeath.integer ||
                                 TimeTilSuddenDeath() <= 0)) {
            // begin sudden death
            localHTP = 0;
            localATP = 0;

            if(level.suddenDeathWarning < TW_PASSED) {
                LogPrintf("Beginning Sudden Death\n");
                trap_SendServerCommand(-1, "cp \"Sudden Death!\"");

                level.suddenDeathBeginTime = level.time;
                level.suddenDeath = true;
                trap_Cvar_Set("g_suddenDeath", "1");

                level.suddenDeathWarning = TW_PASSED;
            }
        } else {
            // warn about sudden death
            if(TimeTilSuddenDeath() <= 60000 &&
                    level.suddenDeathWarning < TW_IMMINENT) {
                trap_SendServerCommand(-1, va("cp \"Sudden Death in %d seconds!\"",
                                              (sint)(TimeTilSuddenDeath() / 1000)));
                level.suddenDeathWarning = TW_IMMINENT;
            }
        }
    } else { // it is SD already
        localHTP = 0;
        localATP = 0;
    }

    level.humanBuildPoints = localHTP - level.humanBuildPointQueue;
    level.alienBuildPoints = localATP - level.alienBuildPointQueue;

    level.reactorPresent = false;
    level.overmindPresent = false;

    for(i = 1, ent = g_entities + i ; i < level.num_entities ; i++, ent++) {
        if(!ent->inuse) {
            continue;
        }

        if(ent->s.eType != ET_BUILDABLE) {
            continue;
        }

        if(ent->s.eFlags & EF_DEAD) {
            continue;
        }

        buildable = (buildable_t)ent->s.modelindex;

        if(buildable != BA_NONE) {
            if(buildable == BA_H_REACTOR && ent->spawned && ent->health > 0) {
                level.reactorPresent = true;
            }

            if(buildable == BA_A_OVERMIND && ent->spawned && ent->health > 0) {
                level.overmindPresent = true;
            }

            if(bggame->Buildable(buildable)->team == TEAM_HUMANS) {
                level.humanBuildPoints -= bggame->Buildable(buildable)->buildPoints;
            } else if(bggame->Buildable(buildable)->team == TEAM_ALIENS) {
                level.alienBuildPoints -= bggame->Buildable(buildable)->buildPoints;
            }
        }
    }

    if(level.humanBuildPoints < 0) {
        localHTP -= level.humanBuildPoints;
        level.humanBuildPoints = 0;
    }

    if(level.alienBuildPoints < 0) {
        localATP -= level.alienBuildPoints;
        level.alienBuildPoints = 0;
    }

    trap_SetConfigstring(CS_BUILDPOINTS, va("%d %d %d %d",
                                            level.alienBuildPoints, localATP,
                                            level.humanBuildPoints, localHTP));

    //may as well pump the stages here too
    {
        float32 alienPlayerCountMod;
        float32 humanPlayerCountMod;

        alienPlayerCountMod = level.averageNumAlienClients / PLAYER_COUNT_MOD;
        humanPlayerCountMod = level.averageNumHumanClients / PLAYER_COUNT_MOD;

        if(alienPlayerCountMod < 0.1f) {
            alienPlayerCountMod = 0.1f;
        }

        if(humanPlayerCountMod < 0.1f) {
            humanPlayerCountMod = 0.1f;
        }

        sint alienNextStageThreshold, humanNextStageThreshold;

        if(g_alienStage.integer < g_alienMaxStage.integer) {
            alienNextStageThreshold = (sint)(ceil((float32)
                                                  g_alienStageThreshold.integer * (g_alienStage.integer + 1) *
                                                  alienPlayerCountMod));
        } else if(g_humanStage.integer > S1) {
            alienNextStageThreshold = (sint)(ceil((float32)level.alienStagedownCredits
                                                  + g_alienStageThreshold.integer * alienPlayerCountMod));
        } else {
            alienNextStageThreshold = -1;
        }

        if(g_humanStage.integer < g_humanMaxStage.integer) {
            humanNextStageThreshold = (sint)(ceil((float32)
                                                  g_humanStageThreshold.integer * (g_humanStage.integer + 1) *
                                                  humanPlayerCountMod));
        } else if(g_alienStage.integer > S1) {
            humanNextStageThreshold = (sint)(ceil((float32)level.humanStagedownCredits
                                                  + g_humanStageThreshold.integer * humanPlayerCountMod));
        } else {
            humanNextStageThreshold = -1;
        }

        // save a lot of bandwidth by rounding thresholds up to the nearest 100
        if(alienNextStageThreshold > 0) {
            alienNextStageThreshold = ceil((float32)alienNextStageThreshold / 100) *
                                      100;
        }

        if(humanNextStageThreshold > 0) {
            humanNextStageThreshold = ceil((float32)humanNextStageThreshold / 100) *
                                      100;
        }

        trap_SetConfigstring(CS_STAGES, va("%d %d %d %d %d %d",
                                           g_alienStage.integer, g_humanStage.integer,
                                           g_alienCredits.integer, g_humanCredits.integer,
                                           alienNextStageThreshold, humanNextStageThreshold));
    }
}

/*
============
idSGameMain_CalculateStages
============
*/
void idSGameMain::CalculateStages(void) {
    float32         alienPlayerCountMod     = level.averageNumAlienClients /
            PLAYER_COUNT_MOD;
    float32         humanPlayerCountMod     = level.averageNumHumanClients /
            PLAYER_COUNT_MOD;
    static sint    lastAlienStageModCount  = 1;
    static sint    lastHumanStageModCount  = 1;
    static sint    lastAlienStage  = -1;
    static sint    lastHumanStage  = -1;


    if(alienPlayerCountMod < 0.1f) {
        alienPlayerCountMod = 0.1f;
    }

    if(humanPlayerCountMod < 0.1f) {
        humanPlayerCountMod = 0.1f;
    }

    if(g_alienCredits.integer >=
            (sint)(ceil((float32)g_alienStageThreshold.integer * S2 *
                        alienPlayerCountMod)) &&
            g_alienStage.integer == S1 && g_alienMaxStage.integer > S1) {
        trap_Cvar_Set("g_alienStage", va("%d", S2));
        level.alienStage2Time = level.time;
        lastAlienStageModCount = g_alienMaxReachedStage.modificationCount;

        if(g_humanStage.integer == S3) {
            level.humanStagedownCredits = g_humanCredits.integer;
        }

        LogPrintf("stagedownlog: aliens s2, humans s%d; %d %d %d %d\n",
                  g_humanStage.integer + 1,
                  g_humanCredits.integer,
                  level.humanStagedownCredits,
                  g_alienCredits.integer,
                  level.alienStagedownCredits);
    }

    if(g_alienCredits.integer >=
            (sint)(ceil((float32)g_alienStageThreshold.integer * S3 *
                        alienPlayerCountMod)) &&
            g_alienStage.integer == S2 && g_alienMaxStage.integer > S2) {
        trap_Cvar_Set("g_alienStage", va("%d", S3));
        level.alienStage3Time = level.time;
        lastAlienStageModCount = g_alienMaxReachedStage.modificationCount;

        if(g_humanStage.integer > S1) {
            level.alienStagedownCredits = g_alienCredits.integer;
        }

        LogPrintf("stagedownlog: aliens s3, humans s%d; %d %d %d %d\n",
                  g_humanStage.integer + 1,
                  g_humanCredits.integer,
                  level.humanStagedownCredits,
                  g_alienCredits.integer,
                  level.alienStagedownCredits);
    }

    if(g_humanCredits.integer >=
            (sint)(ceil((float32)g_humanStageThreshold.integer * S2 *
                        humanPlayerCountMod)) &&
            g_humanStage.integer == S1 && g_humanMaxStage.integer > S1) {
        trap_Cvar_Set("g_humanStage", va("%d", S2));

        level.humanStage2Time = level.time;
        lastHumanStageModCount = g_humanMaxReachedStage.modificationCount;

        if(g_alienStage.integer == S3) {
            level.alienStagedownCredits = g_alienCredits.integer;
        }

        LogPrintf("stagedownlog: humans s2, aliens s%d; %d %d %d %d\n",
                  g_alienStage.integer + 1,
                  g_humanCredits.integer,
                  level.humanStagedownCredits,
                  g_alienCredits.integer,
                  level.alienStagedownCredits);
    }

    if(g_humanCredits.integer >=
            (sint)(ceil((float32)g_humanStageThreshold.integer * S3 *
                        humanPlayerCountMod)) &&
            g_humanStage.integer == S2 && g_humanMaxStage.integer > S2) {
        trap_Cvar_Set("g_humanStage", va("%d", S3));
        level.humanStage3Time = level.time;
        lastHumanStageModCount = g_humanMaxReachedStage.modificationCount;

        if(g_alienStage.integer > S1) {
            level.humanStagedownCredits = g_humanCredits.integer;
        }

        LogPrintf("stagedownlog: humans s3, aliens s%d; %d %d %d %d\n",
                  g_alienStage.integer + 1,
                  g_humanCredits.integer,
                  level.humanStagedownCredits,
                  g_alienCredits.integer,
                  level.alienStagedownCredits);
    }

    if(g_alienStage.integer > S1 &&
            g_humanStage.integer == S3 &&
            g_humanCredits.integer - level.humanStagedownCredits >=
            (sint) ceil(g_humanStageThreshold.integer * humanPlayerCountMod)) {
        LogPrintf("stagedownlog: aliens staging down\n"
                  "stagedownlog: before: %d %d %d %d %d %d\n",
                  g_humanStage.integer,
                  g_humanCredits.integer,
                  level.humanStagedownCredits,
                  g_alienStage.integer,
                  g_alienCredits.integer,
                  level.alienStagedownCredits);

        if(g_alienStage.integer == S3) {
            level.alienStage3Time = level.startTime;
        }

        if(g_alienStage.integer == S2) {
            level.alienStage2Time = level.startTime;
        }

        lastAlienStageModCount = g_alienStage.modificationCount;
        trap_Cvar_Set("g_alienStage", va("%d", g_alienStage.integer - 1));
        trap_Cvar_Set("g_alienCredits", va("%d",
                                           (sint)ceil(g_alienCredits.integer - g_alienStageThreshold.integer *
                                                   alienPlayerCountMod)));
        trap_Cvar_Set("g_humanCredits", va("%d",
                                           (sint)ceil(g_humanCredits.integer - g_humanStageThreshold.integer *
                                                   humanPlayerCountMod)));
        LogPrintf("stagedownlog: after: %d %d %d %d %d %d\n",
                  trap_Cvar_VariableIntegerValue("g_humanStage"),
                  trap_Cvar_VariableIntegerValue("g_humanCredits"),
                  level.humanStagedownCredits,
                  trap_Cvar_VariableIntegerValue("g_alienStage"),
                  trap_Cvar_VariableIntegerValue("g_alienCredits"),
                  level.alienStagedownCredits);
        trap_Cvar_Update(&g_humanCredits);
        trap_Cvar_Update(&g_alienCredits);
        trap_Cvar_Update(&g_humanStage);

    }

    if(g_humanStage.integer > S1 &&
            g_alienStage.integer == S3 &&
            g_alienCredits.integer - level.alienStagedownCredits >=
            (sint) ceil(g_alienStageThreshold.integer * alienPlayerCountMod)) {
        LogPrintf("stagedownlog: humans staging down\n"
                  "stagedownlog: before: %d %d %d %d %d %d\n",
                  g_humanStage.integer,
                  g_humanCredits.integer,
                  level.humanStagedownCredits,
                  g_alienStage.integer,
                  g_alienCredits.integer,
                  level.alienStagedownCredits);

        if(g_humanStage.integer == S3) {
            level.humanStage3Time = level.startTime;
        }

        if(g_humanStage.integer == S2) {
            level.humanStage2Time = level.startTime;
        }

        lastHumanStageModCount = g_humanStage.modificationCount;
        //trap_Cvar_Set( "g_humanStage", va( "%d", g_humanStage.integer - 1 ) );
        //trap_Cvar_Set( "g_humanCredits", va( "%d", ( sint )ceil( g_humanCredits.integer - g_humanStageThreshold.integer * humanPlayerCountMod ) ) );
        //trap_Cvar_Set( "g_alienCredits", va( "%d", ( sint )ceil( g_alienCredits.integer - g_alienStageThreshold.integer * alienPlayerCountMod ) ) );
        LogPrintf("stagedownlog: after: %d %d %d %d %d %d\n",
                  trap_Cvar_VariableIntegerValue("g_humanStage"),
                  trap_Cvar_VariableIntegerValue("g_humanCredits"),
                  level.humanStagedownCredits,
                  trap_Cvar_VariableIntegerValue("g_alienStage"),
                  trap_Cvar_VariableIntegerValue("g_alienCredits"),
                  level.alienStagedownCredits);
        trap_Cvar_Update(&g_humanCredits);
        trap_Cvar_Update(&g_alienCredits);
        trap_Cvar_Update(&g_humanStage);
    }

    if(g_alienMaxReachedStage.modificationCount > lastAlienStageModCount) {
        idSGameTrigger::Checktrigger_stages(TEAM_ALIENS,
                                            (stage_t)g_alienMaxReachedStage.integer);

        if(g_alienStage.integer == S2) {
            level.alienStage2Time = level.time;
        } else if(g_alienStage.integer == S3) {
            level.alienStage3Time = level.time;
        }

        lastAlienStageModCount = g_alienMaxReachedStage.modificationCount;
    }

    if(g_humanMaxReachedStage.modificationCount > lastHumanStageModCount) {
        idSGameTrigger::Checktrigger_stages(TEAM_HUMANS,
                                            (stage_t)g_humanMaxReachedStage.integer);

        if(g_humanStage.integer == S2) {
            level.humanStage2Time = level.time;
        } else if(g_humanStage.integer == S3) {
            level.humanStage3Time = level.time;
        }

        lastHumanStageModCount = g_humanMaxReachedStage.modificationCount;
    }
}

/*
============
idSGameMain::CalculateAvgPlayers

Calculates the average number of players playing this game
============
*/
void idSGameMain::CalculateAvgPlayers(void) {
    //there are no clients or only spectators connected, so
    //reset the number of samples in order to avoid the situation
    //where the average tends to 0
    if(!level.numAlienClients) {
        level.numAlienSamples = 0;
        //trap_Cvar_Set( "g_alienCredits", "0" );
    }

    if(!level.numHumanClients) {
        level.numHumanSamples = 0;
        //trap_Cvar_Set( "g_humanCredits", "0" );
    }

    //calculate average number of clients for stats
    level.averageNumAlienClients =
        ((level.averageNumAlienClients * level.numAlienSamples)
         + level.numAlienClients) /
        (float32)(level.numAlienSamples + 1);
    level.numAlienSamples++;

    level.averageNumHumanClients =
        ((level.averageNumHumanClients * level.numHumanSamples)
         + level.numHumanClients) /
        (float32)(level.numHumanSamples + 1);
    level.numHumanSamples++;
}

/*
============
idSGameMain::CalculateRanks

Recalculates the score ranks of all players
This will be called on every client connect, begin, disconnect, death,
and team change.
============
*/
void idSGameMain::CalculateRanks(void) {
    sint       i;
    valueType      P[ MAX_CLIENTS + 1 ] = {""};
    sint       ff = 0;
    sint        numBots = 0;

    level.numConnectedClients = 0;
    level.numPlayingClients = 0;
    level.numVotingClients = 0;   // don't count bots
    level.numAlienClients = 0;
    level.numHumanClients = 0;
    level.numLiveAlienClients = 0;
    level.numLiveHumanClients = 0;

    for(i = 0; i < level.maxclients; i++) {
        P[ i ] = '-';

        if(level.clients[ i ].pers.connected != CON_DISCONNECTED ||
                level.clients[ i ].pers.demoClient) {
            level.sortedClients[ level.numConnectedClients ] = i;
            level.numConnectedClients++;
            P[ i ] = (valueType)'0' + level.clients[ i ].pers.teamSelection;

            if(level.clients[ i ].pers.connected != CON_CONNECTED &&
                    !level.clients[ i ].pers.demoClient) {
                continue;
            }

            level.numVotingClients++;

            if(level.clients[ i ].pers.teamSelection != TEAM_NONE) {
                level.numPlayingClients++;

                if(level.clients[ i ].pers.teamSelection == TEAM_ALIENS) {
                    level.numAlienClients++;

                    if(level.clients[ i ].sess.spectatorState == SPECTATOR_NOT) {
                        level.numLiveAlienClients++;
                    }
                } else if(level.clients[ i ].pers.teamSelection == TEAM_HUMANS) {
                    level.numHumanClients++;

                    if(level.clients[ i ].sess.spectatorState == SPECTATOR_NOT) {
                        level.numLiveHumanClients++;
                    }
                }
            }
        }
    }

    level.numNonSpectatorClients = level.numLiveAlienClients +
                                   level.numLiveHumanClients;
    level.numVotingClients = level.numConnectedClients - numBots;
    level.numteamVotingClients[ 0 ] = level.numHumanClients;
    level.numteamVotingClients[ 1 ] = level.numAlienClients;
    P[ i ] = '\0';
    trap_Cvar_Set("P", P);

    if(g_friendlyFire.integer) {
        ff |= (FFF_HUMANS | FFF_ALIENS);
    }

    if(g_friendlyFireHumans.integer) {
        ff |=  FFF_HUMANS;
    }

    if(g_friendlyFireAliens.integer) {
        ff |=  FFF_ALIENS;
    }

    if(g_friendlyBuildableFire.integer) {
        ff |=  FFF_BUILDABLES;
    }

    trap_Cvar_Set("ff", va("%i", ff));

    qsort(level.sortedClients, level.numConnectedClients,
          sizeof(level.sortedClients[ 0 ]), &idSGameMain::SortRanks);

    // see if it is time to end the level
    CheckExitRules();

    // if we are at the intermission, send the new info to everyone
    if(level.intermissiontime) {
        SendScoreboardMessageToAllClients();
    }
}


/*
========================================================================

MAP CHANGING

========================================================================
*/

/*
========================
idSGameMain::SendScoreboardMessageToAllClients

Do this at BeginIntermission time and whenever ranks are recalculated
due to enters/exits/forced team changes
========================
*/
void idSGameMain::SendScoreboardMessageToAllClients(void) {
    sint   i;

    for(i = 0; i < level.maxclients; i++) {
        if(level.clients[ i ].pers.connected == CON_CONNECTED) {
            idSGameCmds::ScoreboardMessage(g_entities + i);
        }
    }
}

/*
========================
idSGameMain::MoveClientToIntermission

When the intermission starts, this will be called for all players.
If a new client connects, this will be called after the spawn function.
========================
*/
void idSGameMain::MoveClientToIntermission(gentity_t *ent) {
    // take out of follow mode if needed
    if(ent->client->sess.spectatorState == SPECTATOR_FOLLOW) {
        idSGameCmds::StopFollowing(ent);
    }

    // move to the spot
    VectorCopy(level.intermission_origin, ent->s.origin);
    VectorCopy(level.intermission_origin, ent->r.currentOrigin);
    VectorCopy(level.intermission_origin, ent->client->ps.origin);
    VectorCopy(level.intermission_angle, ent->client->ps.viewangles);
    ent->client->ps.pm_type = PM_INTERMISSION;

    // clean up powerup info
    ::memset(ent->client->ps.misc, 0, sizeof(ent->client->ps.misc));

    ent->client->ps.eFlags = 0;
    ent->s.eFlags = 0;
    ent->s.eType = ET_GENERAL;
    ent->s.modelindex = 0;
    ent->s.loopSound = 0;
    ent->s._event = 0;
    ent->r.contents = 0;
}

/*
==================
idSGameMain::FindIntermissionPoint

This is also used for spectator spawns
==================
*/
void idSGameMain::FindIntermissionPoint(void) {
    gentity_t *ent, *target;
    vec3_t    dir;

    // find the intermission spot
    ent = idSGameUtils::Find(nullptr, FOFS(classname),
                             "info_player_intermission");

    if(!ent) {
        // the map creator forgot to put in an intermission point...
        idSGameClient::SelectSpawnPoint(vec3_origin, level.intermission_origin,
                                        level.intermission_angle);
    } else {
        VectorCopy(ent->s.origin, level.intermission_origin);
        VectorCopy(ent->s.angles, level.intermission_angle);

        // if it has a target, look towards it
        if(ent->target) {
            target = idSGameUtils::PickTarget(ent->target);

            if(target) {
                VectorSubtract(target->s.origin, level.intermission_origin, dir);
                vectoangles(dir, level.intermission_angle);
            }
        }
    }

}

/*
==================
idSGameMain::BeginIntermission
==================
*/
void idSGameMain::BeginIntermission(void) {
    sint     i;
    gentity_t *client;

    if(level.intermissiontime) {
        return;    // already active
    }

    level.intermissiontime = level.time;

    ClearVotes();

    FindIntermissionPoint();

    // move all clients to the intermission point
    for(i = 0; i < level.maxclients; i++) {
        client = g_entities + i;

        if(!client->inuse) {
            continue;
        }

        // respawn if dead
        if(client->health <= 0) {
            idSGameClient::respawn(client);
        }

        MoveClientToIntermission(client);
    }

    // send the current scoring to all clients
    SendScoreboardMessageToAllClients();
}


/*
=============
ExitLevel

When the intermission has been exited, the server is either moved
to a new map based on the map rotation or the current map restarted
=============
*/
void idSGameMain::ExitLevel(void) {
    sint       i;
    gclient_t *cl;

    if(idSGameMaprotation::MapRotationActive()) {
        idSGameMaprotation::AdvanceMapRotation();
    } else {
        trap_SendConsoleCommand(EXEC_APPEND, "map_restart\n");
    }

    level.restarted = true;
    level.changemap = nullptr;
    level.intermissiontime = 0;

    // reset all the scores so we don't enter the intermission again
    for(i = 0; i < g_maxclients.integer; i++) {
        cl = level.clients + i;

        if(cl->pers.connected != CON_CONNECTED) {
            continue;
        }

        cl->ps.persistant[ PERS_SCORE ] = 0;
    }

    // we need to do this here before chaning to CON_CONNECTING
    idSGameSession::WriteSessionData();

    // change all client states to connecting, so the early players into the
    // next level will know the others aren't done reconnecting
    for(i = 0; i < g_maxclients.integer; i++) {
        if(level.clients[ i ].pers.connected == CON_CONNECTED) {
            level.clients[ i ].pers.connected = CON_CONNECTING;
        }
    }
}

/*
=================
idSGameMain::AdminMessage

Print to all active server admins, and to the logfile, and to the server console
Prepend *prefix, or '[SERVER]' if no *prefix is given
=================
*/
void idSGameMain::AdminMessage(pointer prefix, pointer fmt, ...) {
    va_list argptr;
    valueType    string[ 1024 ];
    valueType    outstring[ 1024 ];
    sint     i;

    // Format the text
    va_start(argptr, fmt);
    Q_vsprintf_s(string, sizeof(string), sizeof(string), fmt, argptr);
    va_end(argptr);

    // If there is no prefix, assume that this function was called directly
    // and we should add one
    if(!prefix || !prefix[ 0 ]) {
        prefix = "[SERVER]:";
    }

    // Create the final string
    Q_vsprintf_s(outstring, sizeof(outstring), sizeof(outstring),
                 "%s " S_COLOR_MAGENTA "%s",
                 prefix, string);

    // Send to all appropriate clients
    for(i = 0; i < level.maxclients; i++)
        if(adminLocal.AdminPermission(&g_entities[ i ], ADMF_ADMINCHAT)) {
            trap_SendServerCommand(i, va("chat \"%s\"", outstring));
        }

    // Send to the logfile and server console
    LogPrintf("adminmsg: %s\n", outstring);
}


/*
=================
idSGameMain::LogPrintf

Print to the logfile with a time stamp if it is open, and to the server console
=================
*/
void idSGameMain::LogPrintf(pointer fmt, ...) {
    va_list argptr;
    valueType string[BIG_INFO_STRING], decolored[BIG_INFO_STRING];
    sint min, tens, sec;
    uint32 tslen;

    sec = level.time / 1000;

    min = sec / 60;
    sec -= min * 60;
    tens = sec / 10;
    sec -= tens * 10;

    Q_vsprintf_s(string, sizeof(string), sizeof(string), "%3i:%i%i ", min,
                 tens, sec);

    tslen = strlen(string);

    va_start(argptr, fmt);
    Q_vsprintf_s(string + tslen, sizeof(string) - tslen, fmt, argptr);
    va_end(argptr);

    if(!level.logFile) {
        return;
    }

    idSGameCmds::DecolorString(string, decolored, sizeof(decolored));
    trap_FS_Write(decolored, strlen(decolored), level.logFile);
}

/*
=================
idSGameMain::SendGameStat
=================
*/
void idSGameMain::SendGameStat(team_t team) {
    valueType      map[ MAX_STRING_CHARS ];
    valueType      teamChar;
    valueType      data[ BIG_INFO_STRING ];
    valueType      entry[ MAX_STRING_CHARS ];
    sint       i, dataLength, entryLength;
    gclient_t *cl;

    trap_Cvar_VariableStringBuffer("mapname", map, sizeof(map));

    switch(team) {
        case TEAM_ALIENS:
            teamChar = 'A';
            break;

        case TEAM_HUMANS:
            teamChar = 'H';
            break;

        case TEAM_NONE:
            teamChar = 'L';
            break;

        default:
            return;
    }

    Q_vsprintf_s(data, BIG_INFO_STRING, BIG_INFO_STRING,
                 "%s %s T:%c A:%f H:%f M:%s D:%d SD:%d AS:%d AS2T:%d AS3T:%d HS:%d HS2T:%d HS3T:%d CL:%d",
                 Q3_VERSION,
                 g_tag.string,
                 teamChar,
                 level.averageNumAlienClients,
                 level.averageNumHumanClients,
                 map,
                 level.time - level.startTime,
                 TimeTilSuddenDeath(),
                 g_alienStage.integer,
                 level.alienStage2Time - level.startTime,
                 level.alienStage3Time - level.startTime,
                 g_humanStage.integer,
                 level.humanStage2Time - level.startTime,
                 level.humanStage3Time - level.startTime,
                 level.numConnectedClients);

    dataLength = strlen(data);

    for(i = 0; i < level.numConnectedClients; i++) {
        sint ping;

        cl = &level.clients[ level.sortedClients[ i ] ];

        if(cl->pers.connected == CON_CONNECTING) {
            ping = -1;
        } else {
            ping = cl->ps.ping < 999 ? cl->ps.ping : 999;
        }

        switch(cl->ps.stats[ STAT_TEAM ]) {
            case TEAM_ALIENS:
                teamChar = 'A';
                break;

            case TEAM_HUMANS:
                teamChar = 'H';
                break;

            case TEAM_NONE:
                teamChar = 'S';
                break;

            default:
                return;
        }

        Q_vsprintf_s(entry, MAX_STRING_CHARS, MAX_STRING_CHARS,
                     " \"%s\" %c %d %d %d",
                     cl->pers.netname,
                     teamChar,
                     cl->ps.persistant[ PERS_SCORE ],
                     ping,
                     (level.time - cl->pers.enterTime) / 60000);

        entryLength = strlen(entry);

        if(dataLength + entryLength >= BIG_INFO_STRING) {
            break;
        }

        strcpy(data + dataLength, entry);
        dataLength += entryLength;
    }

    trap_SendGameStat(data);
}

/*
================
idSGameMain::LogExit

Append information about this game to the log file
================
*/
void idSGameMain::LogExit(pointer string) {
    sint         i, numSorted;
    gclient_t   *cl;
    gentity_t   *ent;

    LogPrintf("Exit: %s\n", string);

    level.intermissionQueued = level.time;

    // this will keep the clients from playing any voice sounds
    // that will get cut off when the queued intermission starts
    trap_SetConfigstring(CS_INTERMISSION, "1");

    // don't send more than 32 scores (FIXME?)
    numSorted = level.numConnectedClients;

    if(numSorted > 32) {
        numSorted = 32;
    }

    for(i = 0; i < numSorted; i++) {
        sint   ping;

        cl = &level.clients[ level.sortedClients[ i ] ];

        if(cl->ps.stats[ STAT_TEAM ] == TEAM_NONE) {
            continue;
        }

        if(cl->pers.connected == CON_CONNECTING) {
            continue;
        }

        ping = cl->ps.ping < 999 ? cl->ps.ping : 999;

        LogPrintf("score: %i  ping: %i  client: %i %s\n",
                  cl->ps.persistant[ PERS_SCORE ], ping, level.sortedClients[ i ],
                  cl->pers.netname);
    }

    for(i = 1, ent = g_entities + i ; i < level.num_entities ; i++, ent++) {
        if(!ent->inuse) {
            continue;
        }

        if(!Q_stricmp(ent->classname, "trigger_win")) {
            if(level.lastWin == ent->stageTeam) {
                ent->use(ent, ent, ent);
            }
        }
    }

    SendGameStat(level.lastWin);
}


/*
=================
CheckIntermissionExit

The level will stay at the intermission for a minimum of 5 seconds
If all players wish to continue, the level will then exit.
If one or more players have not acknowledged the continue, the game will
wait 10 seconds before going on.
=================
*/
void idSGameMain::CheckIntermissionExit(void) {
    sint       ready, notReady;
    sint       i;
    gclient_t *cl;
    uchar8      readyMasks[(MAX_CLIENTS + 7) / 8 ];

    // each byte in readyMasks will become two characters 00 - ff in the string
    valueType      readyString[ 2 * sizeof(readyMasks) + 1 ];

    //if no clients are connected, just exit
    if(level.numConnectedClients == 0) {
        ExitLevel();
        return;
    }

    // see which players are ready
    ready = 0;
    notReady = 0;
    ::memset(readyMasks, 0, sizeof(readyMasks));

    for(i = 0; i < g_maxclients.integer; i++) {
        cl = level.clients + i;

        if(cl->pers.connected != CON_CONNECTED) {
            continue;
        }

        if(cl->ps.stats[ STAT_TEAM ] == TEAM_NONE) {
            continue;
        }

        if(cl->readyToExit) {
            ready++;
            // the nth bit of readyMasks is for client (n - 1)
            readyMasks[ i / 8 ] |= 1 << (7 - (i % 8));
        } else {
            notReady++;
        }
    }

    // this is hex because we can convert bits to a hex string in pieces,
    // whereas a decimal string would have to all be written at once
    // (and we can't fit a number that large in an sint)
    for(i = 0; i < (g_maxclients.integer + 7) / 8; i++)
        Q_vsprintf_s(&readyString[ i * 2 ], sizeof(readyString) - i * 2,
                     sizeof(readyString) - i * 2,
                     "%2.2x", readyMasks[ i ]);

    trap_SetConfigstring(CS_CLIENTS_READY, readyString);

    // never exit in less than five seconds
    if(level.time < level.intermissiontime + 5000) {
        return;
    }

    // never let intermission go on for over 1 minute
    if(level.time > level.intermissiontime + 60000) {
        ExitLevel();
        return;
    }

    // if nobody wants to go, clear timer
    if(ready == 0 && notReady > 0) {
        level.readyToExit = false;
        return;
    }

    // if everyone wants to go, go now
    if(notReady == 0) {
        ExitLevel();
        return;
    }

    // the first person to ready starts the thirty second timeout
    if(!level.readyToExit) {
        level.readyToExit = true;
        level.exitTime = level.time;
    }

    // if we have waited thirty seconds since at least one player
    // wanted to exit, go ahead
    if(level.time < level.exitTime + 30000) {
        return;
    }

    ExitLevel();
}

/*
=============
ScoreIsTied
=============
*/
bool idSGameMain::ScoreIsTied(void) {
    sint   a, b;

    if(level.numPlayingClients < 2) {
        return false;
    }

    a = level.clients[ level.sortedClients[ 0 ] ].ps.persistant[ PERS_SCORE ];
    b = level.clients[ level.sortedClients[ 1 ] ].ps.persistant[ PERS_SCORE ];

    return a == b;
}

/*
=================
CheckExitRules

There will be a delay between the time the exit is qualified for
and the time everyone is moved to the intermission spot, so you
can see the last frag.
=================
*/
void idSGameMain::CheckExitRules(void) {
    // if at the intermission, wait for all non-bots to
    // signal ready, then go to next level
    if(level.intermissiontime) {
        CheckIntermissionExit();
        return;
    }

    if(level.intermissionQueued) {
        if(level.time - level.intermissionQueued >= INTERMISSION_DELAY_TIME) {
            level.intermissionQueued = 0;
            BeginIntermission();
        }

        return;
    }

    if(g_timelimit.integer && !level.warmupTime) {
        if(level.time - level.startTime >= g_timelimit.integer * 60000) {
            level.lastWin = TEAM_NONE;
            trap_SendServerCommand(-1, "print \"Timelimit hit\n\"");
            trap_SetConfigstring(CS_WINNER, "Stalemate");
            idSGameMain::LogExit("Timelimit hit.");
            return;
        } else if(level.time - level.startTime >= (g_timelimit.integer - 5) * 60000
                  &&
                  level.timelimitWarning < TW_IMMINENT) {
            trap_SendServerCommand(-1, "cp \"5 minutes remaining!\"");
            level.timelimitWarning = TW_IMMINENT;
        } else if(level.time - level.startTime >= (g_timelimit.integer - 1) * 60000
                  &&
                  level.timelimitWarning < TW_PASSED) {
            trap_SendServerCommand(-1, "cp \"1 minute remaining!\"");
            level.timelimitWarning = TW_PASSED;
        }
    }

#if 0

    if(level.uncondHumanWin || ((level.time > level.startTime + 1000) &&
                                (level.numAlienSpawns == 0) && (level.numLiveAlienClients == 0))) {
        //humans win
        level.lastWin = TEAM_HUMANS;

        trap_SendServerCommand(-1, "print \"Humans win\n\"");
        trap_SetConfigstring(CS_WINNER, "Humans Win");

        idSGameMain::LogExit("Humans win.");
    } else if(level.uncondAlienWin || ((level.time > level.startTime + 1000) &&
                                       (level.numHumanSpawns == 0) && (level.numLiveHumanClients == 0))) {
        //aliens win
        level.lastWin = TEAM_ALIENS;

        trap_SendServerCommand(-1, "print \"Aliens win\n\"");
        trap_SetConfigstring(CS_WINNER, "Aliens Win");

        idSGameMain::LogExit("Aliens win.");
    }

#endif
}

/*
==================
idSGameMain::Vote
==================
*/
void idSGameMain::Vote(gentity_t *ent, bool voting) {
    if(!level.voteTime) {
        return;
    }

    if(voting) {
        if(ent->client->ps.eFlags & EF_VOTED) {
            return;
        }

        ent->client->ps.eFlags |= EF_VOTED;
    } else {
        if(!(ent->client->ps.eFlags & EF_VOTED)) {
            return;
        }

        ent->client->ps.eFlags &= ~EF_VOTED;
    }

    if(ent->client->pers.vote) {
        if(voting) {
            level.voteYes++;
        } else {
            level.voteYes--;
        }

        trap_SetConfigstring(CS_VOTE_YES, va("%d", level.voteYes));
    } else {
        if(voting) {
            level.voteNo++;
        } else {
            level.voteNo--;
        }

        trap_SetConfigstring(CS_VOTE_NO, va("%d", level.voteNo));
    }
}

/*
==================
idSGameMain::TeamVote
==================
*/
void idSGameMain::TeamVote(gentity_t *ent, bool voting) {
    sint cs_offset;

    if(ent->client->pers.teamSelection == TEAM_HUMANS) {
        cs_offset = 0;
    } else if(ent->client->pers.teamSelection == TEAM_ALIENS) {
        cs_offset = 1;
    } else {
        return;
    }

    if(!level.teamVoteTime[ cs_offset ]) {
        return;
    }

    if(voting) {
        if(ent->client->ps.eFlags & EF_TEAMVOTED) {
            return;
        }

        ent->client->ps.eFlags |= EF_TEAMVOTED;
    } else {
        if(!(ent->client->ps.eFlags & EF_TEAMVOTED)) {
            return;
        }

        ent->client->ps.eFlags &= ~EF_TEAMVOTED;
    }

    if(ent->client->pers.teamVote) {
        if(voting) {
            level.teamVoteYes[ cs_offset ]++;
        } else {
            level.teamVoteYes[ cs_offset ]--;
        }

        trap_SetConfigstring(CS_TEAMVOTE_YES + cs_offset, va("%d",
                             level.teamVoteYes[ cs_offset ]));
    } else {
        if(voting) {
            level.teamVoteNo[ cs_offset ]++;
        } else {
            level.teamVoteNo[ cs_offset ]--;
        }

        trap_SetConfigstring(CS_TEAMVOTE_NO + cs_offset, va("%d",
                             level.teamVoteNo[ cs_offset ]));
    }
}


/*
========================================================================

FUNCTIONS CALLED EVERY FRAME

========================================================================
*/


/*
==================
idSGameMain::CheckVote
==================
*/
void idSGameMain::CheckVote(void) {
    sint votePassThreshold = level.votePassThreshold;
    sint voteYesPercent;

    if(level.voteExecuteTime && level.voteExecuteTime < level.time) {
        level.voteExecuteTime = 0;

        trap_SendConsoleCommand(EXEC_APPEND, va("%s\n", level.voteString));

        if(!Q_stricmp(level.voteString, "map_restart") ||
                !Q_stricmpn(level.voteString, "map", 3)) {
            level.restarted = true;
        }

        if(!Q_stricmp(level.voteString, "suddendeath")) {
            level.suddenDeathBeginTime = level.time +
                                         (1000 * g_suddenDeathVoteDelay.integer) - level.startTime;
            level.voteString[0] = '\0';

            if(g_suddenDeathVoteDelay.integer) {
                trap_SendServerCommand(-1,
                                       va("cp \"Sudden Death will begin in %d seconds\n\"",
                                          g_suddenDeathVoteDelay.integer));
            }
        }
    }

    if(!level.voteTime) {
        return;
    }

    if(level.voteYes + level.voteNo > 0) {
        voteYesPercent = (sint)(100 * (level.voteYes) / (level.voteYes +
                                level.voteNo));
    } else {
        voteYesPercent = 0;
    }

    if((level.time - level.voteTime >= VOTE_TIME) ||
            (level.voteYes + level.voteNo == level.numConnectedClients)) {
        if(voteYesPercent > votePassThreshold || level.voteNo == 0) {
            // execute the command, then remove the vote
            trap_SendServerCommand(-1, va("print \"Vote passed (%d - %d)\n\"",
                                          level.voteYes, level.voteNo));
            LogPrintf("Vote: Vote passed (%d-%d)\n", level.voteYes, level.voteNo);
            level.voteExecuteTime = level.time + 3000;
        } else {
            // same behavior as a timeout
            trap_SendServerCommand(-1, va("print \"Vote failed (%d - %d)\n\"",
                                          level.voteYes, level.voteNo));
            LogPrintf("Vote: Vote failed (%d - %d)\n", level.voteYes, level.voteNo);
        }
    } else {
        if(level.voteYes > (sint)((float64) level.numConnectedClients *
                                  ((float64) votePassThreshold / 100.0))) {
            // execute the command, then remove the vote
            trap_SendServerCommand(-1, va("print \"Vote passed (%d - %d)\n\"",
                                          level.voteYes, level.voteNo));
            LogPrintf("Vote: Vote passed (%d - %d)\n", level.voteYes, level.voteNo);
            level.voteExecuteTime = level.time + 3000;
        } else if(level.voteNo > (sint)((float64) level.numConnectedClients *
                                        ((float64)(100.0 - votePassThreshold) / 100.0))) {
            // same behavior as a timeout
            trap_SendServerCommand(-1, va("print \"Vote failed (%d - %d)\n\"",
                                          level.voteYes, level.voteNo));
            LogPrintf("Vote failed\n");
        } else {
            // still waiting for a majority
            return;
        }
    }

    level.voteTime = 0;
    trap_SetConfigstring(CS_VOTE_TIME, "");
}


/*
==================
idSGameMain::CheckTeamVote
==================
*/
void idSGameMain::CheckTeamVote(team_t team) {
    sint cs_offset;

    if(team == TEAM_HUMANS) {
        cs_offset = 0;
    } else if(team == TEAM_ALIENS) {
        cs_offset = 1;
    } else {
        return;
    }

    if(!level.teamVoteTime[ cs_offset ]) {
        return;
    }

    if(level.time - level.teamVoteTime[ cs_offset ] >= VOTE_TIME) {
        idSGameUtils::TeamCommand(team, "print \"Team vote failed\n\"");
    } else {
        if(level.teamVoteYes[ cs_offset ] >
                level.numteamVotingClients[ cs_offset ] / 2) {
            // execute the command, then remove the vote
            idSGameUtils::TeamCommand(team, "print \"Team vote passed\n\"");
            trap_SendConsoleCommand(EXEC_APPEND, va("%s\n",
                                                    level.teamVoteString[ cs_offset ]));
        } else if(level.teamVoteNo[ cs_offset ] >=
                  level.numteamVotingClients[ cs_offset ] / 2) {
            // same behavior as a timeout
            idSGameUtils::TeamCommand(team, "print \"Team vote failed\n\"");
        } else {
            // still waiting for a majority
            return;
        }
    }

    level.teamVoteTime[ cs_offset ] = 0;
    trap_SetConfigstring(CS_TEAMVOTE_TIME + cs_offset, "");
}

/*
==================
CheckCvars
==================
*/
void idSGameMain::CheckCvars(void) {
    static sint lastPasswordModCount   = -1;
    static sint lastMarkDeconModCount  = -1;
    static sint lastSDTimeModCount = -1;

    if(g_password.modificationCount != lastPasswordModCount) {
        lastPasswordModCount = g_password.modificationCount;

        if(*g_password.string && Q_stricmp(g_password.string, "none")) {
            trap_Cvar_Set("g_needpass", "1");
        } else {
            trap_Cvar_Set("g_needpass", "0");
        }
    }

    // Unmark any structures for deconstruction when
    // the server setting is changed
    if(g_markDeconstruct.modificationCount != lastMarkDeconModCount) {
        sint       i;
        gentity_t *ent;

        lastMarkDeconModCount = g_markDeconstruct.modificationCount;

        for(i = 1, ent = g_entities + i ; i < level.num_entities ; i++, ent++) {
            if(!ent->inuse) {
                continue;
            }

            if(ent->s.eType != ET_BUILDABLE) {
                continue;
            }

            ent->deconstruct = false;
        }
    }

    // If we change g_suddenDeathTime during a map, we need to update
    // when sd will begin
    if(g_suddenDeathTime.modificationCount != lastSDTimeModCount) {
        lastSDTimeModCount = g_suddenDeathTime.modificationCount;
        level.suddenDeathBeginTime = g_suddenDeathTime.integer * 60000;
    }

    level.frameMsec = trap_Milliseconds();
}

/*
=============
idSGameMain::RunThink

Runs thinking code for this frame if necessary
=============
*/
void idSGameMain::RunThink(gentity_t *ent) {
    float32 thinktime;

    thinktime = ent->nextthink;

    if(thinktime <= 0) {
        return;
    }

    if(thinktime > level.time) {
        return;
    }

    ent->nextthink = 0;

    if(!ent->think) {
        Error("nullptr ent->think");
    }

    ent->think(ent);
}

/*
=============
G_EvaluateAcceleration

Calculates the acceleration for an entity
=============
*/
void idSGameMain::EvaluateAcceleration(gentity_t *ent, sint msec) {
    vec3_t  deltaVelocity;
    vec3_t  deltaAccel;

    VectorSubtract(ent->s.pos.trDelta, ent->oldVelocity, deltaVelocity);
    VectorScale(deltaVelocity, 1.0f / (float32)msec, ent->acceleration);

    VectorSubtract(ent->acceleration, ent->oldAccel, deltaAccel);
    VectorScale(deltaAccel, 1.0f / (float32)msec, ent->jerk);

    VectorCopy(ent->s.pos.trDelta, ent->oldVelocity);
    VectorCopy(ent->acceleration, ent->oldAccel);
}

/*
================
idSGameLocal::RunFrame

Advances the non-player objects in the world
================
*/
void idSGameLocal::RunFrame(sint levelTime) {
    sint       i;
    gentity_t *ent;
    sint       msec;
    sint       start, end;

    // if we are waiting for the level to restart, do nothing
    if(level.restarted) {
        return;
    }

    level.framenum++;
    level.previousTime = level.time;
    level.time = levelTime;
    msec = level.time - level.previousTime;

    // seed the rng
    srand(level.framenum);

    // get any cvar changes
    idSGameMain::UpdateCvars();

    //
    // go through all allocated objects
    //
    start = trap_Milliseconds();
    ent = &g_entities[ 0 ];

    for(i = 0; i < level.num_entities; i++, ent++)    {
        if(!ent->inuse) {
            continue;
        }

        // clear events that are too old
        if(level.time - ent->eventTime > EVENT_VALID_MSEC) {
            if(ent->s._event) {
                ent->s._event = 0; // &= EV_EVENT_BITS;

                if(ent->client) {
                    ent->client->ps.externalEvent = 0;
                    //ent->client->ps.events[0] = 0;
                    //ent->client->ps.events[1] = 0;
                }
            }

            if(ent->freeAfterEvent) {
                // tempEntities or dropped items completely go away after their event
                idSGameUtils::FreeEntity(ent);
                continue;
            } else if(ent->unlinkAfterEvent) {
                // items that will respawn will hide themselves after their pickup event
                ent->unlinkAfterEvent = false;
                trap_UnlinkEntity(ent);
            }
        }

        // temporary entities don't think
        if(ent->freeAfterEvent) {
            continue;
        }

        // calculate the acceleration of this entity
        if(ent->evaluateAcceleration) {
            idSGameMain::EvaluateAcceleration(ent, msec);
        }

        if(!ent->r.linked && ent->neverFree) {
            continue;
        }

        if(ent->s.eType == ET_MISSILE) {
            idSGameMissile::RunMissile(ent);
            continue;
        }

        if(ent->s.eType == ET_BUILDABLE) {
            idSGameBuildable::BuildableThink(ent, msec);
            continue;
        }

        if(ent->s.eType == ET_CORPSE || ent->physicsObject) {
            idSGamePhysics::Physics(ent, msec);
            continue;
        }

        if(ent->s.eType == ET_MOVER) {
            idSGameMover::RunMover(ent);
            continue;
        }

        if(i < MAX_CLIENTS) {
            idSGameActive::RunClient(ent);
            continue;
        }

        idSGameMain::RunThink(ent);
    }

    end = trap_Milliseconds();

    start = trap_Milliseconds();

    // perform final fixups on the players
    ent = &g_entities[ 0 ];

    for(i = 0; i < level.maxclients; i++, ent++) {
        if(ent->inuse) {
            idSGameActive::ClientEndFrame(ent);
        }
    }

    // save position information for all active clients
    idSGameActive::UnlaggedStore();

    end = trap_Milliseconds();

    idSGameMain::CountSpawns();
    idSGameMain::CalculateBuildPoints();
    idSGameMain::CalculateStages();
    idSGameMain::SpawnClients(TEAM_ALIENS);
    idSGameMain::SpawnClients(TEAM_HUMANS);
    idSGameMain::CalculateAvgPlayers();
    //idSGameWeapons::UpdateZaps( msec );

    // see if it is time to end the level
    idSGameMain::CheckExitRules();

    // update to team status?
    idSGameTeam::CheckTeamStatus();

    // cancel vote if timed out
    idSGameMain::CheckVote();

    // check team votes
    idSGameMain::CheckTeamVote(TEAM_HUMANS);
    idSGameMain::CheckTeamVote(TEAM_ALIENS);

    // for tracking changes
    idSGameMain::CheckCvars();

    if(g_listEntity.integer) {
        for(i = 0; i < MAX_GENTITIES; i++) {
            idSGameMain::Printf("%4i: %s\n", i, g_entities[ i ].classname);
        }

        trap_Cvar_Set("g_listEntity", "0");
    }

    level.frameMsec = trap_Milliseconds();
}
