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
// File name:   cgame_consolecmds.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description: text commands typed in at the local console, or
//              executed by a key binding
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <cgame/cgame_precompiled.hpp>

/*
===============
idCGameConsoleCmds::idCGameConsoleCmds
===============
*/
idCGameConsoleCmds::idCGameConsoleCmds(void) {
}

/*
===============
idCGameConsoleCmds::~idCGameConsoleCmds
===============
*/
idCGameConsoleCmds::~idCGameConsoleCmds(void) {
}

/*
=================
idCGameConsoleCmds::SizeUp_f

Keybinding command
=================
*/
void idCGameConsoleCmds::SizeUp_f(void) {
    trap_Cvar_Set("cg_viewsize", va("%i", (sint)(cg_viewsize.integer + 10)));
}


/*
=================
idCGameConsoleCmds::SizeDown_f

Keybinding command
=================
*/
void idCGameConsoleCmds::SizeDown_f(void) {
    trap_Cvar_Set("cg_viewsize", va("%i", (sint)(cg_viewsize.integer - 10)));
}

/*
=============
idCGameConsoleCmds::Viewpos_f

Debugging command to print the current position
=============
*/
void idCGameConsoleCmds::Viewpos_f(void) {
    Printf("(%i %i %i) : %i\n", (sint)cg.refdef.vieworg[ 0 ],
           (sint)cg.refdef.vieworg[ 1 ], (sint)cg.refdef.vieworg[ 2 ],
           (sint)cg.refdefViewAngles[ YAW ]);
}

/*
===============
idCGameConsoleCmds::RequestScores
===============
*/
bool idCGameConsoleCmds::RequestScores(void) {
    if(cg.scoresRequestTime + 2000 < cg.time) {
        // the scores are more than two seconds out of data,
        // so request new ones
        cg.scoresRequestTime = cg.time;
        trap_SendClientCommand("score\n");

        return true;
    } else {
        return false;
    }
}

extern menuDef_t *menuScoreboard;

/*
===============
idCGameConsoleCmds::scrollScoresDown_f
===============
*/
void idCGameConsoleCmds::scrollScoresDown_f(void) {
    if(menuScoreboard && cg.scoreBoardShowing) {
        Menu_ScrollFeeder(menuScoreboard, FEEDER_ALIENTEAM_LIST, true);
        Menu_ScrollFeeder(menuScoreboard, FEEDER_HUMANTEAM_LIST, true);
    }
}

/*
===============
idCGameConsoleCmds::scrollScoresUp_f
===============
*/
void idCGameConsoleCmds::scrollScoresUp_f(void) {
    if(menuScoreboard && cg.scoreBoardShowing) {
        Menu_ScrollFeeder(menuScoreboard, FEEDER_ALIENTEAM_LIST, false);
        Menu_ScrollFeeder(menuScoreboard, FEEDER_HUMANTEAM_LIST, false);
    }
}

/*
===============
idCGameConsoleCmds::ScoresDown_f
===============
*/
void idCGameConsoleCmds::ScoresDown_f(void) {
    if(!cg.showScores) {
        Menu_SetFeederSelection(menuScoreboard, FEEDER_ALIENTEAM_LIST, 0, nullptr);
        Menu_SetFeederSelection(menuScoreboard, FEEDER_HUMANTEAM_LIST, 0, nullptr);
    }

    if(RequestScores()) {
        // leave the current scores up if they were already
        // displayed, but if this is the first hit, clear them out
        if(!cg.showScores) {
            if(cg_debugRandom.integer) {
                Printf("ScoresDown_f: scores out of date\n");
            }

            cg.showScores = true;
            cg.numScores = 0;
        }
    } else {
        // show the cached contents even if they just pressed if it
        // is within two seconds
        cg.showScores = true;
    }
}

/*
===============
idCGameConsoleCmds::ScoresUp_f
===============
*/
void idCGameConsoleCmds::ScoresUp_f(void) {
    if(cg.showScores) {
        cg.showScores = false;
        cg.scoreFadeTime = cg.time;
    }
}

/*
===============
idCGameConsoleCmds::TellTarget_f
===============
*/
void idCGameConsoleCmds::TellTarget_f(void) {
    sint clientNum;
    valueType command[ 128 ];
    valueType message[ 128 ];

    clientNum = cgameLocal.CrosshairPlayer();

    if(clientNum == -1) {
        return;
    }

    trap_Args(message, 128);
    Q_vsprintf_s(command, 128, 128, "tell %i %s", clientNum, message);
    trap_SendClientCommand(command);
}

/*
===============
idCGameConsoleCmds::TellAttacker_f
===============
*/
void idCGameConsoleCmds::TellAttacker_f(void) {
    sint clientNum;
    valueType command[ 128 ];
    valueType message[ 128 ];

    clientNum = cgameLocal.LastAttacker();

    if(clientNum == -1) {
        return;
    }

    trap_Args(message, 128);
    Q_vsprintf_s(command, 128, 128, "tell %i %s", clientNum, message);
    trap_SendClientCommand(command);
}

/*
===============
idCGameConsoleCmds::SquadMark_f
===============
*/
void idCGameConsoleCmds::SquadMark_f(void) {
    centity_t *cent;
    vec3_t end;
    trace_t trace;

    // Find the player we are looking at
    VectorMA(cg.refdef.vieworg, 131072, cg.refdef.viewaxis[ 0 ], end);
    idCGamePredict::Trace(&trace, cg.refdef.vieworg, nullptr, nullptr, end,
                          cg.snap->ps.clientNum, CONTENTS_SOLID | CONTENTS_BODY);

    if(trace.entityNum >= MAX_CLIENTS) {
        return;
    }

    // Only mark teammates
    cent = cg_entities + trace.entityNum;

    if(cent->currentState.eType != ET_PLAYER ||
            cgs.clientinfo[trace.entityNum].team != cg.snap->ps.stats[STAT_TEAM]) {
        return;
    }

    cent->pe.squadMarked = !cent->pe.squadMarked;
}

static consoleCommand_t commands[ ] = {
    { "testgun", &idCGameView::TestGun_f, "description" },
    { "testmodel", &idCGameView::TestModel_f, "description" },
    { "nextframe", &idCGameView::TestModelNextFrame_f, "description" },
    { "prevframe", &idCGameView::TestModelPrevFrame_f, "description" },
    { "nextskin", &idCGameView::TestModelNextSkin_f, "description" },
    { "prevskin", &idCGameView::TestModelPrevSkin_f, "description" },
    { "viewpos", &idCGameConsoleCmds::Viewpos_f, "description" },
    { "+scores", &idCGameConsoleCmds::ScoresDown_f, "description" },
    { "-scores", &idCGameConsoleCmds::ScoresUp_f, "description" },
    { "scoresUp", &idCGameConsoleCmds::scrollScoresUp_f, "description" },
    { "scoresDown", &idCGameConsoleCmds::scrollScoresDown_f, "description" },
    { "sizeup", &idCGameConsoleCmds::SizeUp_f, "description" },
    { "sizedown", &idCGameConsoleCmds::SizeDown_f, "description" },
    { "weapnext", &idCGameWeapons::NextWeapon_f, "description" },
    { "weapprev", &idCGameWeapons::PrevWeapon_f, "description" },
    { "weapon", &idCGameWeapons::Weapon_f, "description" },
    { "tell_target", &idCGameConsoleCmds::TellTarget_f, "description" },
    { "tell_attacker", &idCGameConsoleCmds::TellAttacker_f, "description" },
    { "testPS", &idCGameParticles::TestPS_f, "description" },
    { "destroyTestPS", &idCGameParticles::DestroyTestPS_f, "description" },
    { "testTS", &idCGameTrails::TestTS_f, "description" },
    { "destroyTestTS", &idCGameTrails::DestroyTestTS_f, "description" },
    { "reloadhud", &idCGameMain::LoadHudMenu, "description" },
    { "squadmark", &idCGameConsoleCmds::SquadMark_f, "description" },
};

/*
=================
idCGameConsoleCmds::ConsoleCommand

The string has been tokenized and can be retrieved with
Cmd_Argc() / Cmd_Argv()
=================
*/
bool idCGameLocal::ConsoleCommand(void) {
    pointer cmd;
    sint i;

    cmd = idCGameMain::Argv(0);

    for(i = 0; i < sizeof(commands) / sizeof(commands[ 0 ]); i++) {
        if(!Q_stricmp(cmd, commands[ i ].cmd)) {
            commands[ i ].function();
            return true;
        }
    }

    return false;
}


/*
=================
CG_InitConsoleCommands

Let the client system know about all of our commands
so it can perform tab completion
=================
*/
void idCGameConsoleCmds::InitConsoleCommands(void) {
    sint i;

    for(i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        trap_AddCommand(commands[i].cmd, commands[i].cmdDesc);
    }

    // the game server will interpret these commands, which will be automatically
    // forwarded to the server after they are not recognized locally
    trap_AddCommand("kill", "description");
    trap_AddCommand("messagemode", "General chat");
    trap_AddCommand("messagemode2", "Team chat");
    trap_AddCommand("messagemode3", "Crosshair chat");
    trap_AddCommand("messagemode4", "Area chat");
    trap_AddCommand("messagemode5", "Admin chat");
    trap_AddCommand("messagemode6", "Clan chat");
    trap_AddCommand("prompt", "description");
    trap_AddCommand("say", "description");
    trap_AddCommand("say_team", "description");
    trap_AddCommand("vsay", "description");
    trap_AddCommand("vsay_team", "description");
    trap_AddCommand("vsay_local", "description");
    trap_AddCommand("m", "description");
    trap_AddCommand("mt", "description");
    trap_AddCommand("tell", "description");
    trap_AddCommand("give", "description");
    trap_AddCommand("god", "description");
    trap_AddCommand("notarget", "description");
    trap_AddCommand("noclip", "description");
    trap_AddCommand("team", "description");
    trap_AddCommand("follow", "description");
    trap_AddCommand("levelshot", "description");
    trap_AddCommand("setviewpos", "description");
    trap_AddCommand("callvote", "description");
    trap_AddCommand("vote", "description");
    trap_AddCommand("callteamvote", "description");
    trap_AddCommand("teamvote", "description");
    trap_AddCommand("class", "description");
    trap_AddCommand("build", "description");
    trap_AddCommand("buy", "description");
    trap_AddCommand("sell", "description");
    trap_AddCommand("reload", "description");
    trap_AddCommand("boost", "description");
    trap_AddCommand("itemact", "description");
    trap_AddCommand("itemdeact", "description");
    trap_AddCommand("itemtoggle", "description");
    trap_AddCommand("destroy", "description");
    trap_AddCommand("deconstruct", "description");
    trap_AddCommand("ignore", "description");
    trap_AddCommand("unignore", "description");
    trap_AddCommand("stats", "description");
}
/*
=================
idCGameLocal::CompleteCommand

The command has been tokenized and can be retrieved with
Cmd_Argc() / Cmd_Argv()
=================
*/
void idCGameLocal::CompleteCommand(sint argNum) {
    pointer cmd;
    sint i;

    cmd = idCGameMain::Argv(0);

    for(i = 0; i < sizeof(commands) / sizeof(commands[ 0 ]); i++) {
        if(!Q_stricmp(cmd, commands[ i ].cmd)) {
            commands[ i ].completer();
            return;
        }
    }
}
