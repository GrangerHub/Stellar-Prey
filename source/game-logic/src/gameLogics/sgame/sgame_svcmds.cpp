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
// File name:   sgame_svcmds.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description: this file holds commands that can be executed by the server
//              console, but not remote clients
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <sgame/sgame_precompiled.hpp>

/*
===============
idSGameSvcmds::idSGameSvcmds
===============
*/
idSGameSvcmds::idSGameSvcmds(void) {
}

/*
===============
idSGameSvcmds::~idSGameSvcmds
===============
*/
idSGameSvcmds::~idSGameSvcmds(void) {
}

/*
===================
idSGameSvcmds::EntityList_f
===================
*/
void idSGameSvcmds::EntityList_f(void) {
    sint       e;
    gentity_t *check;

    check = g_entities;

    for(e = 0; e < level.num_entities; e++, check++) {
        if(!check->inuse) {
            continue;
        }

        idSGameMain::Printf("%3i:", e);

        switch(check->s.eType) {
            case ET_GENERAL:
                idSGameMain::Printf("ET_GENERAL          ");
                break;

            case ET_PLAYER:
                idSGameMain::Printf("ET_PLAYER           ");
                break;

            case ET_ITEM:
                idSGameMain::Printf("ET_ITEM             ");
                break;

            case ET_BUILDABLE:
                idSGameMain::Printf("ET_BUILDABLE        ");
                break;

            case ET_MISSILE:
                idSGameMain::Printf("ET_MISSILE          ");
                break;

            case ET_MOVER:
                idSGameMain::Printf("ET_MOVER            ");
                break;

            case ET_BEAM:
                idSGameMain::Printf("ET_BEAM             ");
                break;

            case ET_PORTAL:
                idSGameMain::Printf("ET_PORTAL           ");
                break;

            case ET_SPEAKER:
                idSGameMain::Printf("ET_SPEAKER          ");
                break;

            case ET_PUSH_TRIGGER:
                idSGameMain::Printf("ET_PUSH_TRIGGER     ");
                break;

            case ET_TELEPORT_TRIGGER:
                idSGameMain::Printf("ET_TELEPORT_TRIGGER ");
                break;

            case ET_INVISIBLE:
                idSGameMain::Printf("ET_INVISIBLE        ");
                break;

            case ET_GRAPPLE:
                idSGameMain::Printf("ET_GRAPPLE          ");
                break;

            case ET_CORPSE:
                idSGameMain::Printf("ET_CORPSE           ");
                break;

            case ET_PARTICLE_SYSTEM:
                idSGameMain::Printf("ET_PARTICLE_SYSTEM  ");
                break;

            case ET_ANIMMAPOBJ:
                idSGameMain::Printf("ET_ANIMMAPOBJ       ");
                break;

            case ET_MODELDOOR:
                idSGameMain::Printf("ET_MODELDOOR        ");
                break;

            case ET_LIGHTFLARE:
                idSGameMain::Printf("ET_LIGHTFLARE       ");
                break;

            case ET_LEV2_ZAP_CHAIN:
                idSGameMain::Printf("ET_LEV2_ZAP_CHAIN   ");
                break;

            default:
                idSGameMain::Printf("%-3i                 ", check->s.eType);
                break;
        }

        if(check->classname) {
            idSGameMain::Printf("%s", check->classname);
        }

        idSGameMain::Printf("\n");
    }
}

gclient_t *idSGameSvcmds::ClientForString(valueType *s) {
    sint idnum, count;
    sint pids[ MAX_CLIENTS ];

    if((count = idSGameCmds::ClientNumbersFromString(s, pids,
                MAX_CLIENTS)) != 1) {
        idnum = idSGameCmds::ClientNumberFromString(s);

        if(idnum == -1) {
            valueType err[ MAX_STRING_CHARS ];
            idSGameCmds::MatchOnePlayer(pids, count, err, sizeof(err));
            idSGameMain::Printf("%s\n", err);
            return nullptr;
        }
    } else {
        idnum = pids[0];
    }

    return &level.clients[ idnum ];
}

void idSGameSvcmds::Svcmd_Status_f(void) {
    sint i;
    gclient_t *cl;
    valueType userinfo[ MAX_INFO_STRING ];

    idSGameMain::Printf("slot score ping address               rate     name\n");
    idSGameMain::Printf("---- ----- ---- -------               ----     ----\n");

    for(i = 0, cl = level.clients; i < level.maxclients; i++, cl++) {
        if(cl->pers.connected == CON_DISCONNECTED) {
            continue;
        }

        idSGameMain::Printf("%-4d ", i);
        idSGameMain::Printf("%-5d ", cl->ps.persistant[ PERS_SCORE ]);

        if(cl->pers.connected == CON_CONNECTING) {
            idSGameMain::Printf("CNCT ");
        } else {
            idSGameMain::Printf("%-4d ", cl->ps.ping);
        }

        trap_GetUserinfo(i, userinfo, sizeof(userinfo));
        idSGameMain::Printf("%-21s ", Info_ValueForKey(userinfo, "ip"));
        idSGameMain::Printf("%-8d ", Info_ValueForKey(userinfo, "rate"));
        idSGameMain::Printf("%s\n",
                            cl->pers.netname);   // Info_ValueForKey( userinfo, "name" )
    }
}

/*
===================
Svcmd_ForceTeam_f

forceteam <player> <team>
===================
*/
void idSGameSvcmds::Svcmd_ForceTeam_f(void) {
    gclient_t *cl;
    valueType str[ MAX_TOKEN_CHARS ];
    team_t team;

    if(trap_Argc() != 3) {
        idSGameMain::Printf("usage: forceteam <player> <team>\n");
        return;
    }

    trap_Argv(1, str, sizeof(str));
    cl = ClientForString(str);

    if(!cl) {
        return;
    }

    trap_Argv(2, str, sizeof(str));
    team = idSGameTeam::TeamFromString(str);

    if(team == NUM_TEAMS) {
        idSGameMain::Printf("forceteam: invalid team \"%s\"\n", str);
        return;
    }

    idSGameTeam::ChangeTeam(&g_entities[ cl - level.clients ], team);
}

/*
===================
Svcmd_LayoutSave_f

layoutsave <name>
===================
*/
void idSGameSvcmds::Svcmd_LayoutSave_f(void) {
    valueType str[ MAX_QPATH ];
    valueType str2[ MAX_QPATH - 4 ];
    valueType *s;
    sint i = 0;

    if(trap_Argc() != 2) {
        idSGameMain::Printf("usage: layoutsave <name>\n");
        return;
    }

    trap_Argv(1, str, sizeof(str));

    // sanitize name
    s = &str[ 0 ];

    while(*s && i < sizeof(str2) - 1) {
        if(isalnum(*s) || *s == '-' || *s == '_') {
            str2[ i++ ] = *s;
            str2[ i ] = '\0';
        }

        s++;
    }

    if(!str2[ 0 ]) {
        idSGameMain::Printf("layoutsave: invalid name \"%s\"\n", str);
        return;
    }

    idSGameBuildable::LayoutSave(str2);
}

/*
===================
Svcmd_LayoutLoad_f

layoutload [<name> [<name2> [<name3 [...]]]]

This is just a silly alias for doing:
 set g_layouts "name name2 name3"
 map_restart
===================
*/
void idSGameSvcmds::Svcmd_LayoutLoad_f(void) {
    valueType layouts[ MAX_CVAR_VALUE_STRING ];
    valueType *s;

    if(trap_Argc() < 2) {
        idSGameMain::Printf("usage: layoutload <name> ...\n");
        return;
    }

    s = idSGameCmds::ConcatArgs(1);
    Q_strncpyz(layouts, s, sizeof(layouts));
    trap_Cvar_Set("g_layouts", layouts);
    trap_SendConsoleCommand(EXEC_APPEND, "map_restart\n");
    level.restarted = true;
}

void idSGameSvcmds::Svcmd_AdmitDefeat_f(void) {
    sint  team;
    valueType teamNum[ 2 ];

    if(trap_Argc() != 2) {
        idSGameMain::Printf("admitdefeat: must provide a team\n");
        return;
    }

    trap_Argv(1, teamNum, sizeof(teamNum));
    team = idSGameTeam::TeamFromString(teamNum);

    if(team == TEAM_ALIENS) {
        idSGameUtils::TeamCommand(TEAM_ALIENS, "cp \"Hivemind Link Broken\" 1");
        trap_SendServerCommand(-1, "print \"Alien team has admitted defeat\n\"");
    } else if(team == TEAM_HUMANS) {
        idSGameUtils::TeamCommand(TEAM_HUMANS, "cp \"Life Support Terminated\" 1");
        trap_SendServerCommand(-1, "print \"Human team has admitted defeat\n\"");
    } else {
        idSGameMain::Printf("admitdefeat: invalid team\n");
        return;
    }

    level.surrenderTeam = (team_t)team;
    idSGameBuildable::BaseSelfDestruct((team_t)team);
}

void idSGameSvcmds::Svcmd_TeamWin_f(void) {
    valueType cmd[ 6 ];
    trap_Argv(0, cmd, sizeof(cmd));
    idSGameBuildable::BaseSelfDestruct(idSGameTeam::TeamFromString(cmd));
}

void idSGameSvcmds::Svcmd_Evacuation_f(void) {
    trap_SendServerCommand(-1, "print \"Evacuation ordered\n\"");
    level.lastWin = TEAM_NONE;
    trap_SetConfigstring(CS_WINNER, "Evacuation");
    idSGameMain::LogExit("Evacuation.");
}

void idSGameSvcmds::Svcmd_MapRotation_f(void) {
    valueType rotationName[ MAX_QPATH ];

    if(trap_Argc() != 2) {
        idSGameMain::Printf("usage: maprotation <name>\n");
        return;
    }

    trap_Argv(1, rotationName, sizeof(rotationName));

    if(!idSGameMaprotation::StartMapRotation(rotationName, false)) {
        idSGameMain::Printf("maprotation: invalid map rotation \"%s\"\n",
                            rotationName);
    }
}

void idSGameSvcmds::Svcmd_TeamMessage_f(void) {
    valueType   teamNum[ 2 ];
    team_t team;

    if(trap_Argc() < 3) {
        idSGameMain::Printf("usage: say_team <team> <message>\n");
        return;
    }

    trap_Argv(1, teamNum, sizeof(teamNum));
    team = idSGameTeam::TeamFromString(teamNum);

    if(team == NUM_TEAMS) {
        idSGameMain::Printf("say_team: invalid team \"%s\"\n", teamNum);
        return;
    }

    idSGameUtils::TeamCommand(team, va("tchat \"console: ^5%s\"",
                                       idSGameCmds::ConcatArgs(2)));
}

void idSGameSvcmds::Svcmd_SendMessage(void) {
    valueType cmd[ 5 ];
    trap_Argv(1, cmd, sizeof(cmd));

    if(trap_Argc() < 2) {
        idSGameMain::Printf("usage: %s <message>\n", cmd);
        return;
    }

    trap_SendServerCommand(-1, va("chat \"console: ^2%s\"",
                                  idSGameCmds::ConcatArgs(1)));
}

void idSGameSvcmds::Svcmd_CenterPrint_f(void) {
    if(trap_Argc() < 2) {
        idSGameMain::Printf("usage: cp <message>\n");
        return;
    }

    trap_SendServerCommand(-1, va("cp \"%s\"", idSGameCmds::ConcatArgs(1)));
}

void idSGameSvcmds::Svcmd_EjectClient_f(void) {
    valueType *reason, name[ MAX_STRING_CHARS ];

    if(trap_Argc() < 2) {
        idSGameMain::Printf("usage: eject <player|-1> <reason>\n");
        return;
    }

    trap_Argv(1, name, sizeof(name));
    reason = idSGameCmds::ConcatArgs(2);

    if(atoi(name) == -1) {
        sint i;

        for(i = 0; i < level.maxclients; i++) {
            if(level.clients[ i ].pers.connected == CON_DISCONNECTED) {
                continue;
            }

            if(level.clients[ i ].pers.localClient) {
                continue;
            }

            trap_DropClient(i, reason, 0);
        }
    } else {
        gclient_t *cl = ClientForString(name);

        if(!cl) {
            return;
        }

        if(cl->pers.localClient) {
            idSGameMain::Printf("eject: cannot eject local clients\n");
            return;
        }

        trap_DropClient(cl - level.clients, reason, 0);
    }
}

void idSGameSvcmds::Svcmd_DumpUser_f(void) {
    valueType name[ MAX_STRING_CHARS ], userinfo[ MAX_INFO_STRING ];
    valueType key[ BIG_INFO_KEY ], value[ BIG_INFO_VALUE ];
    pointer info;
    gclient_t *cl;

    if(trap_Argc() != 2) {
        idSGameMain::Printf("usage: dumpuser <player>\n");
        return;
    }

    trap_Argv(1, name, sizeof(name));
    cl = ClientForString(name);

    if(!cl) {
        return;
    }

    trap_GetUserinfo(cl - level.clients, userinfo, sizeof(userinfo));
    info = &userinfo[ 0 ];
    idSGameMain::Printf("userinfo\n--------\n");

    //Info_Print( userinfo );
    while(1) {
        Info_NextPair(&info, key, value);

        if(!*info) {
            return;
        }

        idSGameMain::Printf("%-20s%s\n", key, value);
    }
}

void idSGameSvcmds::Svcmd_PrintQueue_f(void) {
    valueType team[ MAX_STRING_CHARS ];

    if(trap_Argc() != 2) {
        idSGameMain::Printf("usage: printqueue <team>\n");
        return;
    }

    trap_Argv(1, team, sizeof(team));

    switch(team[0]) {
        case 'a':
            idSGameMain::PrintSpawnQueue(&level.alienSpawnQueue);
            break;

        case 'h':
            idSGameMain::PrintSpawnQueue(&level.humanSpawnQueue);
            break;

        default:
            idSGameMain::Printf("unknown team\n");
    }
}

// dumb wrapper for "a" and "m"
void idSGameSvcmds::Svcmd_MessageWrapper(void) {
    valueType cmd[ 2 ];

    trap_Argv(0, cmd, sizeof(cmd));

    if(!Q_stricmp(cmd, "a")) {
        idSGameCmds::AdminMessage_f(nullptr);
    } else {
        idSGameCmds::PrivateMessage_f(nullptr);
    }
}

struct {
    valueType     *cmd;
    bool dedicated;
    void (*function)(void);
} svcmds[ ] = {
    { "entityList", false, &idSGameSvcmds::EntityList_f },
    { "status", false, &idSGameSvcmds::Svcmd_Status_f },
    { "forceTeam", false, &idSGameSvcmds::Svcmd_ForceTeam_f },
    { "mapRotation", false, &idSGameSvcmds::Svcmd_MapRotation_f },
    { "stopMapRotation", false, &idSGameMaprotation::StopMapRotation },
    { "advanceMapRotation", false, &idSGameMaprotation::AdvanceMapRotation },
    { "alienWin", false, &idSGameSvcmds::Svcmd_TeamWin_f },
    { "humanWin", false, &idSGameSvcmds::Svcmd_TeamWin_f },
    { "layoutSave", false, &idSGameSvcmds::Svcmd_LayoutSave_f },
    { "layoutLoad", false, &idSGameSvcmds::Svcmd_LayoutLoad_f },
    { "eject", false, &idSGameSvcmds::Svcmd_EjectClient_f },
    { "dumpuser", false, &idSGameSvcmds::Svcmd_DumpUser_f },
    { "admitDefeat", false, &idSGameSvcmds::Svcmd_AdmitDefeat_f },
    { "evacuation", false, &idSGameSvcmds::Svcmd_Evacuation_f },
    { "printqueue", false, &idSGameSvcmds::Svcmd_PrintQueue_f },

    // don't handle communication commands unless dedicated
    { "say_team", true, &idSGameSvcmds::Svcmd_TeamMessage_f },
    { "say", true, &idSGameSvcmds::Svcmd_SendMessage },
    { "chat", true, &idSGameSvcmds::Svcmd_SendMessage },
    { "cp", true, &idSGameSvcmds::Svcmd_CenterPrint_f },
    { "m", true, &idSGameSvcmds::Svcmd_MessageWrapper },
    { "a", true, &idSGameSvcmds::Svcmd_MessageWrapper }
};

/*
=================
idSGameLocal::ConsoleCommand
=================
*/
bool idSGameLocal::ConsoleCommand(void) {
    valueType cmd[ MAX_TOKEN_CHARS ];
    sint i;

    trap_Argv(0, cmd, sizeof(cmd));

    for(i = 0; i < sizeof(svcmds) / sizeof(svcmds[ 0 ]); i++) {
        if(!Q_stricmp(cmd, svcmds[ i ].cmd)) {
            if(svcmds[i].dedicated && !g_dedicated.integer) {
                return false;
            }

            svcmds[ i ].function();
            return true;
        }
    }

    // see if this is an admin command
    if(adminLocal.AdminCmdCheck(nullptr, false)) {
        return true;
    }

    if(g_dedicated.integer) {
        idSGameMain::Printf("unknown command: %s\n", cmd);
    }

    return false;
}

