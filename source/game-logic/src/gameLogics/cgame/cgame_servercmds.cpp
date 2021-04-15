////////////////////////////////////////////////////////////////////////////////////////
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
// File name:   cgame_servercmds.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description: reliably sequenced text commands sent by the server
//              these are processed at snapshot transition time, so there
//              will definately be a valid snapshot this frame
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <cgame/cgame_precompiled.hpp>

extern sint sortedTeamPlayers[TEAM_MAXOVERLAY];
extern sint numSortedTeamPlayers;

/*
===============
idCGameServerCmds::idCGameServerCmds
===============
*/
idCGameServerCmds::idCGameServerCmds(void) {
}

/*
===============
idCGameServerCmds::~idCGameServerCmds
===============
*/
idCGameServerCmds::~idCGameServerCmds(void) {
}

/*
=================
idCGameServerCmds::ParseScores
=================
*/
void idCGameServerCmds::ParseScores(void) {
    sint i;

    cg.numScores = atoi(idCGameMain::Argv(1));

    if(cg.numScores > MAX_CLIENTS) {
        cg.numScores = MAX_CLIENTS;
    }

    cg.teamScores[ 0 ] = atoi(idCGameMain::Argv(2));
    cg.teamScores[ 1 ] = atoi(idCGameMain::Argv(3));

    ::memset(cg.scores, 0, sizeof(cg.scores));

    if(cg_debugRandom.integer) {
        Printf("cg.numScores: %d\n", cg.numScores);
    }

    for(i = 0; i < cg.numScores; i++) {
        cg.scores[ i ].client = atoi(idCGameMain::Argv(i * 6 + 4));
        cg.scores[ i ].score = atoi(idCGameMain::Argv(i * 6 + 5));
        cg.scores[ i ].ping = atoi(idCGameMain::Argv(i * 6 + 6));
        cg.scores[ i ].time = atoi(idCGameMain::Argv(i * 6 + 7));
        cg.scores[ i ].weapon = (weapon_t)atoi(idCGameMain::Argv(i * 6 + 8));
        cg.scores[ i ].upgrade = (upgrade_t)atoi(idCGameMain::Argv(i * 6 + 9));

        if(cg.scores[i].client < 0 || cg.scores[i].client >= MAX_CLIENTS) {
            cg.scores[i].client = 0;
        }

        cgs.clientinfo[ cg.scores[ i ].client ].score = cg.scores[ i ].score;

        cg.scores[ i ].team = cgs.clientinfo[ cg.scores[ i ].client ].team;
    }
}

/*
=================
idCGameServerCmds::ParseTeamInfo
=================
*/
void idCGameServerCmds::ParseTeamInfo(void) {
    sint i, client;

    numSortedTeamPlayers = atoi(idCGameMain::Argv(1));

    if(numSortedTeamPlayers < 0 || numSortedTeamPlayers > TEAM_MAXOVERLAY) {
        Error("idCGameLocal::ParseTeamInfo: numSortedTeamPlayers out of range (%d)",
              numSortedTeamPlayers);
        return;
    }

    for(i = 0; i < numSortedTeamPlayers; i++) {
        client = atoi(idCGameMain::Argv(i * 5 + 2));

        if(client < 0 || client >= MAX_CLIENTS) {
            Error("idCGameLocal::ParseTeamInfo: bad client number: %d", client);
            return;
        }

        sortedTeamPlayers[ i ] = client;

        cgs.clientinfo[ client ].location = atoi(idCGameMain::Argv(i * 5 + 3));
        cgs.clientinfo[ client ].health = atoi(idCGameMain::Argv(i * 5 + 4));
        cgs.clientinfo[ client ].armor = atoi(idCGameMain::Argv(i * 5 + 5));
        cgs.clientinfo[ client ].curWeapon = atoi(idCGameMain::Argv(i * 5 + 6));
    }
}


/*
================
idCGameServerCmds::ParseServerinfo

This is called explicitly when the gamestate is first received,
and whenever the server updates any serverinfo flagged cvars
================
*/
void idCGameServerCmds::ParseServerinfo(void) {
    pointer info;
    valueType *mapname;

    info = idCGameMain::ConfigString(CS_SERVERINFO);
    cgs.timelimit = atoi(Info_ValueForKey(info, "timelimit"));
    cgs.maxclients = atoi(Info_ValueForKey(info, "sv_maxclients"));
    cgs.markDeconstruct = atoi(Info_ValueForKey(info, "g_markDeconstruct"));
    mapname = Info_ValueForKey(info, "mapname");
    Q_vsprintf_s(cgs.mapname, sizeof(cgs.mapname), sizeof(cgs.mapname),
                 "maps/%s.bsp", mapname);
}

/*
==================
idCGameServerCmds::ParseWarmup
==================
*/
void idCGameServerCmds::ParseWarmup(void) {
    pointer info;
    sint warmup;

    info = idCGameMain::ConfigString(CS_WARMUP);

    warmup = atoi(info);
    cg.warmupCount = -1;

    if(warmup == 0 && cg.warmup) {
    }

    cg.warmup = warmup;
}

/*
================
idCGameServerCmds::SetConfigValues

Called on load to set the initial values from configure strings
================
*/
void idCGameServerCmds::SetConfigValues(void) {
    sscanf(idCGameMain::ConfigString(CS_BUILDPOINTS),
           "%d %d %d %d", &cgs.alienBuildPoints,
           &cgs.alienBuildPointsTotal,
           &cgs.humanBuildPoints,
           &cgs.humanBuildPointsTotal);

    sscanf(idCGameMain::ConfigString(CS_STAGES), "%d %d %d %d %d %d",
           &cgs.alienStage, &cgs.humanStage,
           &cgs.alienCredits, &cgs.humanCredits, &cgs.alienNextStageThreshold,
           &cgs.humanNextStageThreshold);

    cgs.levelStartTime = atoi(idCGameMain::ConfigString(CS_LEVEL_START_TIME));
    cg.warmup = atoi(idCGameMain::ConfigString(CS_WARMUP));
}


/*
=====================
idCGameLocal::ShaderStateChanged
=====================
*/
void idCGameServerCmds::ShaderStateChanged(void) {
    valueType originalShader[ MAX_QPATH ];
    valueType newShader[ MAX_QPATH ];
    valueType timeOffset[ 16 ];
    pointer o;
    valueType *n, *t;

    o = idCGameMain::ConfigString(CS_SHADERSTATE);

    while(o && *o) {
        n = (valueType *)strstr(o, "=");

        if(n && *n) {
            strncpy(originalShader, o, n - o);
            originalShader[ n - o ] = 0;
            n++;
            t = strstr(n, ":");

            if(t && *t) {
                strncpy(newShader, n, t - n);
                newShader[ t - n ] = 0;
            } else {
                break;
            }

            t++;
            o = strstr(t, "@");

            if(o) {
                strncpy(timeOffset, t, o - t);
                timeOffset[ o - t ] = 0;
                o++;
                trap_R_RemapShader(originalShader, newShader, timeOffset);
            }
        } else {
            break;
        }
    }
}

/*
================
idCGameServerCmds::AnnounceAlienStageTransistion
================
*/
void idCGameServerCmds::AnnounceAlienStageTransistion(stage_t from,
        stage_t to) {
    if(cg.predictedPlayerState.stats[STAT_TEAM] != TEAM_ALIENS) {
        return;
    }

    trap_S_StartLocalSound(cgs.media.alienStageTransition, CHAN_ANNOUNCER);

    if(to > from) {
        idCGameDraw::CenterPrint("We have evolved!", 200, GIANTCHAR_WIDTH * 4);
    } else {
        idCGameDraw::CenterPrint("We have devolved!", 200, GIANTCHAR_WIDTH * 4);
    }
}

/*
================
idCGameServerCmds::AnnounceHumanStageTransistion
================
*/
void idCGameServerCmds::AnnounceHumanStageTransistion(stage_t from,
        stage_t to) {
    if(cg.predictedPlayerState.stats[STAT_TEAM] != TEAM_HUMANS) {
        return;
    }

    trap_S_StartLocalSound(cgs.media.humanStageTransition, CHAN_ANNOUNCER);

    if(to > from) {
        idCGameDraw::CenterPrint("Reinforcements have arrived!", 200,
                                 GIANTCHAR_WIDTH * 4);
    } else {
        idCGameDraw::CenterPrint("Reinforcements have been withdrawn!", 200,
                                 GIANTCHAR_WIDTH * 4);
    }
}

/*
================
idCGameServerCmds::ConfigStringModified
================
*/
void idCGameServerCmds::ConfigStringModified(void) {
    pointer str;
    sint num;

    num = atoi(idCGameMain::Argv(1));

    // get the gamestate from the client system, which will have the
    // new configstring already integrated
    trap_GetGameState(&cgs.gameState);

    // look up the individual string that was modified
    str = idCGameMain::ConfigString(num);

    // do something with it if necessary
    if(num == CS_MUSIC) {
        idCGameMain::StartMusic();
    } else if(num == CS_SERVERINFO) {
        ParseServerinfo();
    } else if(num == CS_WARMUP) {
        ParseWarmup();
    } else if(num == CS_BUILDPOINTS) {
        sscanf(str, "%d %d %d %d", &cgs.alienBuildPoints,
               &cgs.alienBuildPointsTotal, &cgs.humanBuildPoints,
               &cgs.humanBuildPointsTotal);
    } else if(num == CS_STAGES) {
        stage_t oldAlienStage = (stage_t)cgs.alienStage;
        stage_t oldHumanStage = (stage_t)cgs.humanStage;

        sscanf(str, "%d %d %d %d %d %d",
               &cgs.alienStage, &cgs.humanStage,
               &cgs.alienCredits, &cgs.humanCredits,
               &cgs.alienNextStageThreshold, &cgs.humanNextStageThreshold);

        if(cgs.alienStage != oldAlienStage) {
            AnnounceAlienStageTransistion(oldAlienStage, (stage_t)cgs.alienStage);
        }

        if(cgs.humanStage != oldHumanStage) {
            AnnounceHumanStageTransistion(oldHumanStage, (stage_t)cgs.humanStage);
        }
    } else if(num == CS_LEVEL_START_TIME) {
        cgs.levelStartTime = atoi(str);
    } else if(num == CS_VOTE_TIME) {
        cgs.voteTime = atoi(str);
        cgs.voteModified = true;

        if(cgs.voteTime) {
            trap_Cvar_Set("ui_voteActive", "1");
        } else {
            trap_Cvar_Set("ui_voteActive", "0");
        }
    } else if(num == CS_VOTE_YES) {
        cgs.voteYes = atoi(str);
        cgs.voteModified = true;
    } else if(num == CS_VOTE_NO) {
        cgs.voteNo = atoi(str);
        cgs.voteModified = true;
    } else if(num == CS_VOTE_STRING) {
        Q_strncpyz(cgs.voteString, str, sizeof(cgs.voteString));
    } else if(num >= CS_TEAMVOTE_TIME && num <= CS_TEAMVOTE_TIME + 1) {
        sint cs_offset = num - CS_TEAMVOTE_TIME;

        cgs.teamVoteTime[ cs_offset ] = atoi(str);
        cgs.teamVoteModified[ cs_offset ] = true;

        if(cs_offset == 0) {
            if(cgs.teamVoteTime[cs_offset]) {
                trap_Cvar_Set("ui_humanTeamVoteActive", "1");
            } else {
                trap_Cvar_Set("ui_humanTeamVoteActive", "0");
            }
        } else if(cs_offset == 1) {
            if(cgs.teamVoteTime[cs_offset]) {
                trap_Cvar_Set("ui_alienTeamVoteActive", "1");
            } else {
                trap_Cvar_Set("ui_alienTeamVoteActive", "0");
            }
        }
    } else if(num >= CS_TEAMVOTE_YES && num <= CS_TEAMVOTE_YES + 1) {
        cgs.teamVoteYes[ num - CS_TEAMVOTE_YES ] = atoi(str);
        cgs.teamVoteModified[ num - CS_TEAMVOTE_YES ] = true;
    } else if(num >= CS_TEAMVOTE_NO && num <= CS_TEAMVOTE_NO + 1) {
        cgs.teamVoteNo[ num - CS_TEAMVOTE_NO ] = atoi(str);
        cgs.teamVoteModified[ num - CS_TEAMVOTE_NO ] = true;
    } else if(num >= CS_TEAMVOTE_STRING && num <= CS_TEAMVOTE_STRING + 1) {
        Q_strncpyz(cgs.teamVoteString[ num - CS_TEAMVOTE_STRING ], str,
                   sizeof(cgs.teamVoteString[ num - CS_TEAMVOTE_STRING ]));
    } else if(num == CS_INTERMISSION) {
        cg.intermissionStarted = atoi(str);
    } else if(num >= CS_MODELS && num < CS_MODELS + MAX_MODELS) {
        cgs.gameModels[num - CS_MODELS] = trap_R_RegisterModel(str);
    } else if(num >= CS_SHADERS && num < CS_SHADERS + MAX_GAME_SHADERS) {
        cgs.gameShaders[num - CS_SHADERS] = trap_R_RegisterShader(str);
    } else if(num >= CS_PARTICLE_SYSTEMS &&
              num < CS_PARTICLE_SYSTEMS + MAX_GAME_PARTICLE_SYSTEMS) {
        cgs.gameParticleSystems[num - CS_PARTICLE_SYSTEMS] =
            idCGameParticles::RegisterParticleSystem((valueType *)str);
    } else if(num >= CS_SOUNDS && num < CS_SOUNDS + MAX_SOUNDS) {
        if(str[ 0 ] != '*') {
            // player specific sounds don't register here
            cgs.gameSounds[ num - CS_SOUNDS ] = trap_S_RegisterSound(str);
        }
    } else if(num >= CS_PLAYERS && num < CS_PLAYERS + MAX_CLIENTS) {
        idCGamePlayers::NewClientInfo(num - CS_PLAYERS);
        idCGameMain::BuildSpectatorString();
    } else if(num == CS_WINNER) {
        trap_Cvar_Set("ui_winner", str);
    } else if(num == CS_SHADERSTATE) {
        ShaderStateChanged();
    }
}

/*
===============
idCGameServerCmds::MapRestart

The server has issued a map_restart, so the next snapshot
is completely new and should not be interpolated to.

A tournement restart will clear everything, but doesn't
require a reload of all the media
===============
*/
void idCGameServerCmds::MapRestart(void) {
    if(cg_showmiss.integer) {
        Printf("idCGameLocal::MapRestart\n");
    }

    idCGameMarks::InitMarkPolys();

    // make sure the "3 frags left" warnings play again
    cg.fraglimitWarnings = 0;

    cg.timelimitWarnings = 0;

    cg.intermissionStarted = false;

    cgs.voteTime = 0;

    cg.mapRestart = true;

    idCGameMain::StartMusic();

    trap_S_ClearLoopingSounds(true);

    // we really should clear more parts of cg here and stop sounds

    // play the "fight" sound if this is a restart without warmup
    if(cg.warmup == 0) {
        idCGameDraw::CenterPrint("FIGHT!", 120, GIANTCHAR_WIDTH * 2);
    }

    trap_Cvar_Set("cg_thirdPerson", "0");
}

/*
==============
idCGameServerCmds::Menu
==============
*/
void idCGameServerCmds::Menu(sint menu, sint arg) {
    pointer cmd; // command to send
    pointer longMsg = nullptr;  // command parameter
    pointer shortMsg = nullptr; // non-modal version of message
    pointer dialog;
    cgameDialogType_t type = (cgameDialogType_t)
                             0;   // controls which cg_disable var will switch it off

    switch(cg.snap->ps.stats[ STAT_TEAM ]) {
        case TEAM_ALIENS:
            dialog = "menu tremulous_alien_dialog\n";
            break;

        case TEAM_HUMANS:
            dialog = "menu tremulous_human_dialog\n";
            break;

        default:
            dialog = "menu tremulous_default_dialog\n";
    }

    cmd = dialog;

    switch(menu) {
        case MN_TEAM:
            cmd = "menu tremulous_teamselect\n";
            type = DT_INTERACTIVE;
            break;

        case MN_A_CLASS:
            cmd = "menu tremulous_alienclass\n";
            type = DT_INTERACTIVE;
            break;

        case MN_H_SPAWN:
            cmd = "menu tremulous_humanitem\n";
            type = DT_INTERACTIVE;
            break;

        case MN_A_BUILD:
            cmd = "menu tremulous_alienbuild\n";
            type = DT_INTERACTIVE;
            break;

        case MN_H_BUILD:
            cmd = "menu tremulous_humanbuild\n";
            type = DT_INTERACTIVE;
            break;

        case MN_H_ARMOURY:
            cmd = "menu tremulous_humanarmoury\n";
            type = DT_INTERACTIVE;
            break;

        case MN_H_UNKNOWNITEM:
            shortMsg = "Unknown item";
            type = DT_ARMOURYEVOLVE;
            break;

        case MN_A_TEAMFULL:
            longMsg = "The alien team has too many players. Please wait until slots "
                      "become available or join the human team.";
            shortMsg = "The alien team has too many players";
            type = DT_COMMAND;
            break;

        case MN_H_TEAMFULL:
            longMsg = "The human team has too many players. Please wait until slots "
                      "become available or join the alien team.";
            shortMsg = "The human team has too many players";
            type = DT_COMMAND;
            break;

        case MN_A_TEAMCHANGEBUILDTIMER:
            longMsg = "You cannot leave the Alien team until your build timer "
                      "has expired.";
            shortMsg = "You cannot change teams until your build timer expires.";
            type = DT_COMMAND;
            break;

        case MN_H_TEAMCHANGEBUILDTIMER:
            longMsg = "You cannot leave the Human team until your build timer "
                      "has expired.";
            shortMsg = "You cannot change teams until your build timer expires.";
            type = DT_COMMAND;
            break;

        //===============================

        // Since cheating commands have no default binds, they will often be done
        // via console. In light of this, perhaps opening a menu is
        // counterintuitive
        case MN_CMD_CHEAT:
            //longMsg   = "This action is considered cheating. It can only be used "
            //            "in cheat mode, which is not enabled on this server.";
            shortMsg = "Cheats are not enabled on this server";
            type = DT_COMMAND;
            break;

        case MN_CMD_CHEAT_TEAM:
            shortMsg = "You may not use this command while on a team";
            type  = DT_COMMAND;
            break;

        case MN_CMD_TEAM:
            //longMsg   = "You must be on a team to perform this action. Join the alien"
            //            "or human team and try again.";
            shortMsg = "Join a team first";
            type = DT_COMMAND;
            break;

        case MN_CMD_SPEC:
            //longMsg   = "You may not perform this action while on a team. Become a "
            //            "spectator before trying again.";
            shortMsg = "You can only use this command when spectating";
            type = DT_COMMAND;
            break;

        case MN_CMD_ALIEN:
            //longMsg   = "You must be on the alien team to perform this action.";
            shortMsg = "Must be alien to use this command";
            type = DT_COMMAND;
            break;

        case MN_CMD_HUMAN:
            //longMsg   = "You must be on the human team to perform this action.";
            shortMsg = "Must be human to use this command";
            type = DT_COMMAND;
            break;

        case MN_CMD_LIVING:
            //longMsg   = "You must be living to perform this action.";
            shortMsg = "Must be living to use this command";
            type = DT_COMMAND;
            break;

        case MN_B_NOROOM:
            longMsg = "There is no room to build here. Move until the structure turns "
                      "translucent green indicating a valid build location.";
            shortMsg = "There is no room to build here";
            type = DT_BUILD;
            break;

        case MN_B_NORMAL:
            longMsg = "Cannot build on this surface. The surface is too steep or "
                      "unsuitable to build on. Please choose another site for this "
                      "structure.";
            shortMsg = "Cannot build on this surface";
            type = DT_BUILD;
            break;

        case MN_B_CANNOT:
            longMsg = nullptr;
            shortMsg = "You cannot build that structure";
            type = DT_BUILD;
            break;

        // FIXME: MN_H_ and MN_A_?
        case MN_B_LASTSPAWN:
            longMsg = "This action would remove your team's last spawn point, "
                      "which often quickly results in a loss. Try building more "
                      "spawns.";
            shortMsg = "You may not deconstruct the last spawn";
            break;

        case MN_B_SUDDENDEATH:
            longMsg = "Neither team has prevailed after a certain time and the "
                      "game has entered Sudden Death. During Sudden Death "
                      "building is not allowed.";
            shortMsg = "Cannot build during Sudden Death";
            type = DT_BUILD;
            break;

        case MN_B_REVOKED:
            longMsg = "Your teammates have lost faith in your ability to build "
                      "for the team. You will not be allowed to build until your "
                      "team votes to reinstate your building rights.";
            shortMsg = "Your building rights have been revoked";
            type = DT_BUILD;
            break;

        case MN_B_SURRENDER:
            longMsg = "Your team has decided to admit defeat and concede the game:"
                      "traitors and cowards are not allowed to build.";
            // too harsh?
            shortMsg = "Building is denied to traitorous cowards";
            break;

        //===============================

        case MN_H_NOBP:
            if(cgs.markDeconstruct) {
                longMsg = "There is no power remaining. Free up power by marking "
                          "existing buildable objects.";
            } else {
                longMsg = "There is no power remaining. Free up power by deconstructing "
                          "existing buildable objects.";
            }

            shortMsg = "There is no power remaining";
            type = DT_BUILD;
            break;

        case MN_H_NOTPOWERED:
            longMsg = "This buildable is not powered. Build a Reactor and/or Repeater "
                      "in order to power it.";
            shortMsg = "This buildable is not powered";
            type = DT_BUILD;
            break;

        case MN_H_ONEREACTOR:
            longMsg = "There can only be one Reactor. Deconstruct the existing one if you "
                      "wish to move it.";
            shortMsg = "There can only be one Reactor";
            type = DT_BUILD;
            break;

        case MN_H_NOPOWERHERE:
            longMsg = "There is no power here. If available, a Repeater may be used to "
                      "transmit power to this location.";
            shortMsg = "There is no power here";
            type = DT_BUILD;
            break;

        case MN_H_NODCC:
            longMsg = "There is no Defense Computer. A Defense Computer is needed to "
                      "build this.";
            shortMsg = "There is no Defense Computer";
            type = DT_BUILD;
            break;

        case MN_H_RPTPOWERHERE:
            longMsg = "This area already has power. A Repeater is not required here.";
            shortMsg = "This area already has power";
            type = DT_BUILD;
            break;

        case MN_H_NOSLOTS:
            longMsg = "You have no room to carry this. Please sell any conflicting "
                      "upgrades before purchasing this item.";
            shortMsg = "You have no room to carry this";
            type = DT_ARMOURYEVOLVE;
            break;

        case MN_H_NOFUNDS:
            longMsg = "Insufficient funds. You do not have enough credits to perform "
                      "this action.";
            shortMsg = "Insufficient funds";
            type = DT_ARMOURYEVOLVE;
            break;

        case MN_H_ITEMHELD:
            longMsg = "You already hold this item. It is not possible to carry multiple "
                      "items of the same type.";
            shortMsg = "You already hold this item";
            type = DT_ARMOURYEVOLVE;
            break;

        case MN_H_NOARMOURYHERE:
            longMsg = "You must be near a powered Armoury in order to purchase "
                      "weapons, upgrades or ammunition.";
            shortMsg = "You must be near a powered Armoury";
            type = DT_ARMOURYEVOLVE;
            break;

        case MN_H_NOENERGYAMMOHERE:
            longMsg = "You must be near a Reactor or a powered Armoury or Repeater "
                      "in order to purchase energy ammunition.";
            shortMsg = "You must be near a Reactor or a powered Armoury or Repeater";
            type = DT_ARMOURYEVOLVE;
            break;

        case MN_H_NOROOMBSUITON:
            longMsg = "There is not enough room here to put on a Battle Suit. "
                      "Make sure you have enough head room to climb in.";
            shortMsg = "Not enough room here to put on a Battle Suit";
            type = DT_ARMOURYEVOLVE;
            break;

        case MN_H_NOROOMBSUITOFF:
            longMsg = "There is not enough room here to take off your Battle Suit. "
                      "Make sure you have enough head room to climb out.";
            shortMsg = "Not enough room here to take off your Battle Suit";
            type = DT_ARMOURYEVOLVE;
            break;

        case MN_H_ARMOURYBUILDTIMER:
            longMsg = "You are not allowed to buy or sell weapons until your "
                      "build timer has expired.";
            shortMsg = "You can not buy or sell weapons until your build timer "
                       "expires";
            type = DT_ARMOURYEVOLVE;
            break;

        case MN_H_DEADTOCLASS:
            shortMsg = "You must be dead to use the class command";
            type = DT_COMMAND;
            break;

        case MN_H_UNKNOWNSPAWNITEM:
            shortMsg = "Unknown starting item";
            type = DT_COMMAND;
            break;

        //===============================

        case MN_A_NOCREEP:
            longMsg = "There is no creep here. You must build near existing Eggs or "
                      "the Overmind. Alien structures will not support themselves.";
            shortMsg = "There is no creep here";
            type = DT_BUILD;
            break;

        case MN_A_NOOVMND:
            longMsg = "There is no Overmind. An Overmind must be built to control "
                      "the structure you tried to place";
            shortMsg = "There is no Overmind";
            type = DT_BUILD;
            break;

        case MN_A_ONEOVERMIND:
            longMsg = "There can only be one Overmind. Deconstruct the existing one if you "
                      "wish to move it.";
            shortMsg = "There can only be one Overmind";
            type = DT_BUILD;
            break;

        case MN_A_ONEHOVEL:
            longMsg = "There can only be one Hovel. Deconstruct the existing one if you "
                      "wish to move it.";
            shortMsg = "There can only be one Hovel";
            type = DT_BUILD;
            break;

        case MN_A_NOBP:
            longMsg = "The Overmind cannot control any more structures. Deconstruct existing "
                      "structures to build more.";
            shortMsg = "The Overmind cannot control any more structures";
            type = DT_BUILD;
            break;

        case MN_A_NOEROOM:
            longMsg = "There is no room to evolve here. Move away from walls or other "
                      "nearby objects and try again.";
            shortMsg = "There is no room to evolve here";
            type = DT_ARMOURYEVOLVE;
            break;

        case MN_A_TOOCLOSE:
            longMsg = "This location is too close to the enemy to evolve. Move away "
                      "until you are no longer aware of the enemy's presence and try "
                      "again.";
            shortMsg = "This location is too close to the enemy to evolve";
            type = DT_ARMOURYEVOLVE;
            break;

        case MN_A_NOOVMND_EVOLVE:
            longMsg = "There is no Overmind. An Overmind must be built to allow "
                      "you to upgrade.";
            shortMsg = "There is no Overmind";
            type = DT_ARMOURYEVOLVE;
            break;

        case MN_A_EVOLVEBUILDTIMER:
            longMsg = "You cannot Evolve until your build timer has expired.";
            shortMsg = "You cannot Evolve until your build timer expires";
            type = DT_ARMOURYEVOLVE;
            break;

        case MN_A_HOVEL_OCCUPIED:
            longMsg = "This Hovel is already occupied by another builder.";
            shortMsg = "This Hovel is already occupied by another builder";
            type = DT_COMMAND;
            break;

        case MN_A_HOVEL_BLOCKED:
            longMsg = "The exit to this Hovel is currently blocked. Please wait until it "
                      "becomes clear then try again.";
            shortMsg = "The exit to this Hovel is currently blocked";
            type = DT_COMMAND;
            break;

        case MN_A_HOVEL_EXIT:
            longMsg = "The exit to this Hovel would always be blocked. Please choose "
                      "a more suitable location.";
            shortMsg = "The exit to this Hovel would always be blocked";
            type = DT_BUILD;
            break;

        case MN_A_INFEST:
            trap_Cvar_Set("ui_currentClass", va("%d %d",
                                                cg.snap->ps.stats[ STAT_CLASS ], cg.snap->ps.persistant[ PERS_CREDIT ]));
            cmd = "menu tremulous_alienupgrade\n";
            type = DT_INTERACTIVE;
            break;

        case MN_A_CANTEVOLVE:
            shortMsg = va("You cannot evolve into a %s",
                          bggame->ClassConfig((class_t)arg)->humanName);
            type = DT_ARMOURYEVOLVE;
            break;

        case MN_A_EVOLVEWALLWALK:
            shortMsg = "You cannot evolve while wallwalking";
            type = DT_ARMOURYEVOLVE;
            break;

        case MN_A_UNKNOWNCLASS:
            shortMsg = "Unknown class";
            type = DT_ARMOURYEVOLVE;
            break;

        case MN_A_CLASSNOTSPAWN:
            shortMsg = va("You cannot spawn as a %s",
                          bggame->ClassConfig((class_t)arg)->humanName);
            type = DT_ARMOURYEVOLVE;
            break;

        case MN_A_CLASSNOTALLOWED:
            shortMsg = va("The %s is not allowed",
                          bggame->ClassConfig((class_t)arg)->humanName);
            type = DT_ARMOURYEVOLVE;
            break;

        case MN_A_CLASSNOTATSTAGE:
            shortMsg = va("The %s is not allowed at Stage %d",
                          bggame->ClassConfig((class_t)arg)->humanName, cgs.alienStage + 1);
            type = DT_ARMOURYEVOLVE;
            break;

        default:
            Com_Printf("cgame: debug: no such menu %d\n", menu);
    }

    if(type == DT_ARMOURYEVOLVE && cg_disableUpgradeDialogs.integer) {
        return;
    }

    if(type == DT_BUILD && cg_disableBuildDialogs.integer) {
        return;
    }

    if(type == DT_COMMAND && cg_disableCommandDialogs.integer) {
        return;
    }

    if(cmd != dialog) {
        trap_SendConsoleCommand(cmd);
    } else if(longMsg && cg_disableWarningDialogs.integer == 0) {
        trap_Cvar_Set("ui_dialog", longMsg);
        trap_SendConsoleCommand(cmd);
    } else if(shortMsg && cg_disableWarningDialogs.integer < 2) {
        Printf("%s\n", shortMsg);
    }
}

/*
=================
idCGameServerCmds::Say
=================
*/
void idCGameServerCmds::Say(sint clientNum, valueType *text) {
    clientInfo_t *ci;
    valueType sayText[ MAX_SAY_TEXT ] = {""};

    if(clientNum < 0 || clientNum >= MAX_CLIENTS) {
        return;
    }

    ci = &cgs.clientinfo[ clientNum ];
    Q_vsprintf_s(sayText, sizeof(sayText), sizeof(sayText),
                 "%s: " S_COLOR_WHITE S_COLOR_GREEN "%s" S_COLOR_WHITE "\n", ci->name,
                 text);

    if(bggame->ClientListTest(&cgs.ignoreList, clientNum)) {
        Printf("[skipnotify]%s", sayText);
    } else {
        Printf("%s", sayText);
    }
}

/*
=================
idCGameServerCmds::SayTeam
=================
*/
void idCGameServerCmds::SayTeam(sint clientNum, valueType *text) {
    clientInfo_t *ci;
    valueType sayText[ MAX_SAY_TEXT ] = {""};

    if(clientNum < 0 || clientNum >= MAX_CLIENTS) {
        return;
    }

    ci = &cgs.clientinfo[ clientNum ];
    Q_vsprintf_s(sayText, sizeof(sayText), sizeof(sayText),
                 "%s: " S_COLOR_CYAN "%s" S_COLOR_WHITE "\n", ci->name, text);

    if(bggame->ClientListTest(&cgs.ignoreList, clientNum)) {
        Printf("[skipnotify]%s", sayText);
    } else {
        Printf("%s", sayText);
    }
}

/*
=================
idCGameServerCmds::VoiceTrack

return the voice indexed voice track or print errors quietly to console
in case someone is on an unpure server and wants to know which voice pak
is missing or incomplete
=================
*/
voiceTrack_t *idCGameServerCmds::VoiceTrack(valueType *voice, sint cmd,
        sint track) {
    voice_t *v;
    voiceCmd_t *c;
    voiceTrack_t *t;

    v = bggame->VoiceByName(cgs.voices, voice);

    if(!v) {
        Printf("[skipnotify]WARNING: could not find voice \"%s\"\n", voice);
        return nullptr;
    }

    c = bggame->VoiceCmdByNum(v->cmds, cmd);

    if(!c) {
        Printf("[skipnotify]WARNING: could not find command %d "
               "in voice \"%s\"\n", cmd, voice);
        return nullptr;
    }

    t = bggame->VoiceTrackByNum(c->tracks, track);

    if(!t) {
        Printf("[skipnotify]WARNING: could not find track %d for command %d in voice \"%s\"\n",
               track, cmd, voice);
        return nullptr;
    }

    return t;
}

/*
=================
idCGameServerCmds::ParseVoice

voice clientNum vChan cmdNum trackNum [sayText]
=================
*/
void idCGameServerCmds::ParseVoice(void) {
    sint clientNum;
    voiceChannel_t vChan;
    valueType sayText[ MAX_SAY_TEXT] = {""};
    voiceTrack_t *track;
    clientInfo_t *ci;

    if(trap_Argc() < 5 || trap_Argc() > 6) {
        return;
    }

    if(trap_Argc() == 6) {
        Q_strncpyz(sayText, idCGameMain::Argv(5), sizeof(sayText));
    }

    clientNum = atoi(idCGameMain::Argv(1));

    if(clientNum < 0 || clientNum >= MAX_CLIENTS) {
        return;
    }

    vChan = (voiceChannel_t)atoi(idCGameMain::Argv(2));

    if(vChan < 0 || vChan >= VOICE_CHAN_NUM_CHANS) {
        return;
    }

    if(cg_teamChatsOnly.integer && vChan != VOICE_CHAN_TEAM) {
        return;
    }

    ci = &cgs.clientinfo[ clientNum ];

    // this joker is still talking
    if(ci->voiceTime > cg.time) {
        return;
    }

    track = VoiceTrack(ci->voice, atoi(idCGameMain::Argv(3)),
                       atoi(idCGameMain::Argv(4)));

    // keep track of how long the player will be speaking
    // assume it takes 3s to say "*unintelligible gibberish*"
    if(track) {
        ci->voiceTime = cg.time + track->duration;
    } else {
        ci->voiceTime = cg.time + 3000;
    }

    if(!sayText[ 0 ]) {
        if(track) {
            Q_strncpyz(sayText, track->text, sizeof(sayText));
        } else {
            Q_strncpyz(sayText, "*unintelligible gibberish*", sizeof(sayText));
        }
    }

    if(!cg_noVoiceText.integer) {
        switch(vChan) {
            case VOICE_CHAN_ALL:
                Say(clientNum, sayText);
                break;

            case VOICE_CHAN_TEAM:
                SayTeam(clientNum, sayText);
                break;

            default:
                break;
        }
    }

    // playing voice audio tracks disabled
    if(cg_noVoiceChats.integer) {
        return;
    }

    // no audio track to play
    if(!track) {
        return;
    }

    // don't play audio track for lamers
    if(bggame->ClientListTest(&cgs.ignoreList, clientNum)) {
        return;
    }

    switch(vChan) {
        case VOICE_CHAN_ALL:
            trap_S_StartLocalSound(track->track, CHAN_VOICE);
            break;

        case VOICE_CHAN_TEAM:
            trap_S_StartLocalSound(track->track, CHAN_VOICE);
            break;

        case VOICE_CHAN_LOCAL:
            trap_S_StartSound(nullptr, clientNum, CHAN_VOICE, track->track);
            break;

        default:
            break;
    }
}

/*
=================
idCGameServerCmds::CenterPrint_f
=================
*/
void idCGameServerCmds::CenterPrint_f(void) {
    idCGameDraw::CenterPrint(idCGameMain::Argv(1), SCREEN_HEIGHT * 0.30,
                             BIGCHAR_WIDTH);
}

/*
=================
idCGameServerCmds::Print_f
=================
*/
void idCGameServerCmds::Print_f(void) {
    Printf("%s", idCGameMain::Argv(1));
}

/*
=================
idCGameServerCmds::Chat_f
=================
*/
void idCGameServerCmds::Chat_f(void) {
    valueType cmd[ 6 ], text[ MAX_SAY_TEXT ];
    bool team;

    trap_Argv(0, cmd, sizeof(cmd));
    team = Q_stricmp(cmd, "chat");

    if(team && cg_teamChatsOnly.integer) {
        return;
    }

    Q_strncpyz(text, idCGameMain::Argv(1), sizeof(text));

    if(Q_stricmpn(text, "[skipnotify]", 12)) {
        if(team && cg.snap->ps.stats[STAT_TEAM] == TEAM_ALIENS) {
            trap_S_StartLocalSound(cgs.media.alienTalkSound, CHAN_LOCAL_SOUND);
        } else if(team && cg.snap->ps.stats[STAT_TEAM] == TEAM_HUMANS) {
            trap_S_StartLocalSound(cgs.media.humanTalkSound, CHAN_LOCAL_SOUND);
        } else {
            trap_S_StartLocalSound(cgs.media.talkSound, CHAN_LOCAL_SOUND);
        }
    }

    Printf("%s\n", text);
}

/*
=================
idCGameServerCmds::ClientLevelShot_f
=================
*/
void idCGameServerCmds::ClientLevelShot_f(void) {
    cg.levelShot = true;
}

/*
=================
idCGameServerCmds::ServerMenu_f
=================
*/
void idCGameServerCmds::ServerMenu_f(void) {
    if(trap_Argc() == 2 && !cg.demoPlayback) {
        Menu(atoi(idCGameMain::Argv(1)), 0);
    } else if(trap_Argc() == 3 && !cg.demoPlayback) {
        Menu(atoi(idCGameMain::Argv(1)), atoi(idCGameMain::Argv(2)));
    }
}

/*
=================
idCGameServerCmds::ServerCloseMenus_f
=================
*/
void idCGameServerCmds::ServerCloseMenus_f(void) {
    trap_SendConsoleCommand("closemenus\n");
}

/*
=================
idCGameServerCmds::PoisonCloud_f
=================
*/
void idCGameServerCmds::PoisonCloud_f(void) {
    cg.poisonedTime = cg.time;

    if(idCGameParticles::IsParticleSystemValid(&cg.poisonCloudPS)) {
        cg.poisonCloudPS = idCGameParticles::SpawnNewParticleSystem(
                               cgs.media.poisonCloudPS);
        idCGameAttachment::SetAttachmentCent(&cg.poisonCloudPS->attachment,
                                             &cg.predictedPlayerEntity);
        idCGameAttachment::AttachToCent(&cg.poisonCloudPS->attachment);
    }
}

/*
=================
idCGameServerCmds::PTRRequest_f
=================
*/
void idCGameServerCmds::PTRRequest_f(void) {
    trap_SendClientCommand(va("ptrcverify %d", idCGamePtr::ReadPTRCode()));
}

/*
=================
idCGameServerCmds::PTRIssue_f
=================
*/
void idCGameServerCmds::PTRIssue_f(void) {
    if(trap_Argc() == 2) {
        idCGamePtr::WritePTRCode(atoi(idCGameMain::Argv(1)));
    }
}

/*
=================
idCGameServerCmds::PTRConfirm_f
=================
*/
void idCGameServerCmds::PTRConfirm_f(void) {
    trap_SendConsoleCommand("menu ptrc_popmenu\n");
}

/*
=================
idCGameServerCmds::Play
=================
*/
void idCGameServerCmds::PlaySound(void) {
    sfxHandle_t sfx;
    vec3_t origin;
    sfx = trap_S_RegisterSound(idCGameMain::Argv(1));

    VectorSet(origin, atof(idCGameMain::Argv(2)), atof(idCGameMain::Argv(3)),
              atof(idCGameMain::Argv(4)));

    trap_S_StartSound(origin, 0, 0, sfx);
}

/*
=================
idCGameServerCmds::SpawnServer
=================
*/
void idCGameServerCmds::SpawnServer(void) {
    cg.serverRespawning = true;
}


static consoleCommand_t svcommands[ ] = {
    { "spawnserver", &idCGameServerCmds::SpawnServer, "description" },
    { "cp", &idCGameServerCmds::CenterPrint_f, "description" },
    { "cs", &idCGameServerCmds::ConfigStringModified, "description" },
    { "print", &idCGameServerCmds::Print_f, "description" },
    { "chat", &idCGameServerCmds::Chat_f, "description" },
    { "tchat", &idCGameServerCmds::Chat_f, "description" },
    { "scores", &idCGameServerCmds::ParseScores, "description" },
    { "tinfo", &idCGameServerCmds::ParseTeamInfo, "description" },
    { "map_restart", &idCGameServerCmds::MapRestart, "description" },
    { "clientLevelShot", &idCGameServerCmds::ClientLevelShot_f, "description" },
    { "servermenu", &idCGameServerCmds::ServerMenu_f, "description" },
    { "serverclosemenus", &idCGameServerCmds::ServerCloseMenus_f, "description" },
    { "poisoncloud", &idCGameServerCmds::PoisonCloud_f, "description" },
    { "voice", &idCGameServerCmds::ParseVoice, "description" },
    { "ptrcrequest", &idCGameServerCmds::PTRRequest_f, "description" },
    { "ptrcissue", &idCGameServerCmds::PTRIssue_f, "description" },
    { "ptrcconfirm", &idCGameServerCmds::PTRConfirm_f, "description" },
    { "playsound", &idCGameServerCmds::PlaySound, "description" },
};

/*
=================
idCGameServerCmds::ServerCommand

The string has been tokenized and can be retrieved with
Cmd_Argc() / Cmd_Argv()
=================
*/
void idCGameServerCmds::ServerCommand(void) {
    pointer cmd;
    sint i;

    cmd = idCGameMain::Argv(0);

    for(i = 0; i < sizeof(svcommands) / sizeof(svcommands[ 0 ]); i++) {
        if(!Q_stricmp(cmd, svcommands[ i ].cmd)) {
            svcommands[ i ].function();
            return;
        }
    }

    Printf("idCGameServerCmds::ServerCommand - Unknown client game command: %s\n",
           cmd);
}

/*
====================
idCGameServerCmds::ExecuteNewServerCommands

Execute all of the server commands that were received along
with this this snapshot.
====================
*/
void idCGameServerCmds::ExecuteNewServerCommands(sint latestSequence) {
    while(cgs.serverCommandSequence < latestSequence) {
        if(trap_GetServerCommand(++cgs.serverCommandSequence)) {
            ServerCommand();
        }
    }
}
