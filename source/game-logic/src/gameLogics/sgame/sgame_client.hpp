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
// File name:   sgame_client.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SGAME_CLIENT_H__
#define __SGAME_CLIENT_H__

class idSGameLocal;

class idSGameClient : public idSGameLocal {
public:
    idSGameClient();
    ~idSGameClient();

    static void SP_info_human_intermission(gentity_t *ent);
    static void SP_info_alien_intermission(gentity_t *ent);
    static void SP_info_player_intermission(gentity_t *ent);
    static void SP_info_player_start(gentity_t *ent);
    static void SP_info_player_deathmatch(gentity_t *ent);
    static bool SpotWouldTelefrag(gentity_t *spot);
    static void AddCreditToClient(gclient_t *client, schar16 credit, bool cap);
    static void SetClientViewAngle(gentity_t *ent, vec3_t angle);
    static gentity_t *SelectTremulousSpawnPoint(team_t  team,
            vec3_t preference, vec3_t origin, vec3_t angles);
    static gentity_t *SelectSpawnPoint(vec3_t avoidPoint, vec3_t origin,
                                       vec3_t angles);
    static gentity_t *SelectAlienLockSpawnPoint(vec3_t origin, vec3_t angles);
    static gentity_t *SelectHumanLockSpawnPoint(vec3_t origin, vec3_t angles);
    static void SpawnCorpse(gentity_t *ent);
    static void respawn(gentity_t *ent);
    static void ClientSpawn(gentity_t *ent, gentity_t *spawn, vec3_t origin,
                            vec3_t angles);
    static gentity_t *SelectSpectatorSpawnPoint(vec3_t origin, vec3_t angles);
    static gentity_t *SelectInitialSpawnPoint(vec3_t origin, vec3_t angles);
    static gentity_t *SelectHumanSpawnPoint(vec3_t preference);
    static gentity_t *SelectAlienSpawnPoint(vec3_t preference);
    static gentity_t *SelectRandomFurthestSpawnPoint(vec3_t avoidPoint,
            vec3_t origin, vec3_t angles);
    static gentity_t *SelectRandomDeathmatchSpawnPoint(void);
    static gentity_t *SelectNearestDeathmatchSpawnPoint(vec3_t from);
    static void BodySink(gentity_t *ent);
    static void BodyFree(gentity_t *ent);
    static bool IsEmoticon(pointer s, bool *escaped);
    static bool NonSegModel(pointer filename);
    static void ClientCleanName(pointer in, valueType *out, sint outSize);
    static void BotBegin(sint clientNum);
};

#endif // !__SGAME_CLIENT_H__
