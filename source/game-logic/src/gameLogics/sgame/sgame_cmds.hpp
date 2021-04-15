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
// File name:   sgame_cmds.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SGAME_CMDS_H__
#define __SGAME_CMDS_H__

class idSGameLocal;

class idSGameCmds : public idSGameLocal {
public:
    idSGameCmds();
    ~idSGameCmds();

    static void AdminMessage_f(gentity_t *ent);
    static void SanitiseString(valueType *in, valueType *out, sint len);
    static void PrivateMessage_f(gentity_t *ent);
    static sint ClientNumberFromString(valueType *s);
    static sint ClientNumbersFromString(valueType *s, sint *plist, sint max);
    static sint SayArgc(void);
    static bool SayArgv(sint n, valueType *buffer, sint bufferLength);
    static valueType *SayConcatArgs(sint start);
    static void DecolorString(valueType *in, valueType *out, sint len);
    static void MatchOnePlayer(sint *plist, sint num, valueType *err,
                               sint len);
    static void Cmd_Give_f(gentity_t *ent);
    static void Cmd_God_f(gentity_t *ent);
    static void Cmd_Notarget_f(gentity_t *ent);
    static void Cmd_Noclip_f(gentity_t *ent);
    static void Cmd_LevelShot_f(gentity_t *ent);
    static void Cmd_Kill_f(gentity_t *ent);
    static void Cmd_Team_f(gentity_t *ent);
    static void SayTo(gentity_t *ent, gentity_t *other, sint mode, sint color,
                      pointer name, pointer message);
    static void Say(gentity_t *ent, gentity_t *target, sint mode,
                    pointer chatText);
    static void Cmd_SayArea_f(gentity_t *ent);
    static void Cmd_Say_f(gentity_t *ent);
    static void Cmd_Tell_f(gentity_t *ent);
    static void Cmd_VSay_f(gentity_t *ent);
    static void Cmd_Where_f(gentity_t *ent);
    static void Cmd_CallVote_f(gentity_t *ent);
    static void Cmd_Vote_f(gentity_t *ent);
    static void Cmd_CallTeamVote_f(gentity_t *ent);
    static void Cmd_TeamVote_f(gentity_t *ent);
    static void Cmd_SetViewpos_f(gentity_t *ent);
    static bool RoomForClassChange(gentity_t *ent, class_t _class,
                                   vec3_t newOrigin);
    static void Cmd_Class_f(gentity_t *ent);
    static void Cmd_Destroy_f(gentity_t *ent);
    static void Cmd_ActivateItem_f(gentity_t *ent);
    static void Cmd_DeActivateItem_f(gentity_t *ent);
    static void Cmd_ToggleItem_f(gentity_t *ent);
    static void Cmd_Buy_f(gentity_t *ent);
    static void Cmd_Sell_f(gentity_t *ent);
    static void Cmd_Build_f(gentity_t *ent);
    static void Cmd_Reload_f(gentity_t *ent);
    static void Cmd_Follow_f(gentity_t *ent);
    static void Cmd_FollowCycle_f(gentity_t *ent);
    static void Cmd_PTRCVerify_f(gentity_t *ent);
    static void Cmd_PTRCRestore_f(gentity_t *ent);
    static void Cmd_Ignore_f(gentity_t *ent);
    static void Cmd_Test_f(gentity_t *ent);
    static void Cmd_Damage_f(gentity_t *ent);
    static void Cmd_EditBotInv_f(gentity_t *ent);
    static void Cmd_Stats_f(gentity_t *ent);
    static valueType *ConcatArgs(sint start);
    static void EditPlayerInventory(gentity_t *ent);
    static void FollowLockView(gentity_t *ent);
    static bool FollowNewClient(gentity_t *ent, sint dir);
    static void ScoreboardMessage(gentity_t *client);
    static void Share_f(gentity_t *ent);
    static void StopFromFollowing(gentity_t *ent);
    static void StopFollowing(gentity_t *ent);
    static void ToggleFollow(gentity_t *ent);
    static sint FloodLimited(gentity_t *ent);

};

#endif // !__SGAME_CMDS_H__
