////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 2018 - 2021 Dusan Jocic <dusanjocic@msn.com>
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
// File name:   cgame_servercmds.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CGAME_SERVERCMDS_H__
#define __CGAME_SERVERCMDS_H__

class idCGameServerCmds;

class idCGameServerCmds : public idCGameLocal {
public:
    idCGameServerCmds();
    ~idCGameServerCmds();

    static void ParseScores(void);
    static void ParseTeamInfo(void);
    static void ParseServerinfo(void);
    static void ParseWarmup(void);
    static void SetConfigValues(void);
    static void ShaderStateChanged(void);
    static void AnnounceAlienStageTransistion(stage_t from, stage_t to);
    static void AnnounceHumanStageTransistion(stage_t from, stage_t to);
    static void ConfigStringModified(void);
    static void MapRestart(void);
    static void Menu(sint menu, sint arg);
    static void Say(sint clientNum, valueType *text);
    static void SayTeam(sint clientNum, valueType *text);
    static voiceTrack_t *VoiceTrack(valueType *voice, sint cmd, sint track);
    static void ParseVoice(void);
    static void CenterPrint_f(void);
    static void Print_f(void);
    static void Chat_f(void);
    static void ClientLevelShot_f(void);
    static void ServerMenu_f(void);
    static void ServerCloseMenus_f(void);
    static void PoisonCloud_f(void);
    static void PTRRequest_f(void);
    static void PTRIssue_f(void);
    static void PTRConfirm_f(void);
    static void ServerCommand(void);
    static void ExecuteNewServerCommands(sint latestSequence);
    static void PlaySound(void);
    static void SpawnServer(void);
};

#endif //!__CGAME_SERVERCMDS_H__
