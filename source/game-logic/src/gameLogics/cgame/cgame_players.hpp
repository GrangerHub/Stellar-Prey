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
// File name:   cgame_players.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CGAME_PLAYERS_H__
#define __CGAME_PLAYERS_H__

class idCGameLocal;

class idCGamePlayers : public idCGameLocal {
public:
    idCGamePlayers();
    ~idCGamePlayers();

    static sfxHandle_t CustomSound(sint clientNum, pointer soundName);
    static bool ParseAnimationFile(pointer filename, clientInfo_t *ci);
    static bool RegisterClientSkin(clientInfo_t *ci, pointer modelName,
                                   pointer skinName);
    static bool RegisterClientModelname(clientInfo_t *ci, pointer modelName,
                                        pointer skinName);
    static void ColorFromString(pointer v, vec3_t color);
    static void LoadClientInfo(clientInfo_t *ci);
    static void CopyClientInfoModel(clientInfo_t *from, clientInfo_t *to);
    static sint GetCorpseNum(class_t _class);
    static bool ScanForExistingClientInfo(clientInfo_t *ci);
    static void PrecacheClientInfo(class_t _class, valueType *model,
                                   valueType *skin);
    static void TeamJoinMessage(clientInfo_t *newInfo, clientInfo_t *ci);
    static void NewClientInfo(sint clientNum);
    static void SetLerpFrameAnimation(clientInfo_t *ci, lerpFrame_t *lf,
                                      sint newAnimation);
    static void RunPlayerLerpFrame(clientInfo_t *ci, lerpFrame_t *lf,
                                   sint newAnimation, float32 speedScale);
    static void ClearLerpFrame(clientInfo_t *ci, lerpFrame_t *lf,
                               sint animationNumber);
    static void PlayerAnimation(centity_t *cent, sint *legsOld, sint *legs,
                                float32 *legsBackLerp, sint *torsoOld, sint *torso,
                                float32 *torsoBackLerp);
    static void PlayerNonSegAnimation(centity_t *cent, sint *nonSegOld,
                                      sint *nonSeg, float32 *nonSegBackLerp);
    static void SwingAngles(float32 destination, float32 swingTolerance,
                            float32 clampTolerance, float32 speed, float32 *angle, bool *swinging);
    static void AddPainTwitch(centity_t *cent, vec3_t torsoAngles);
    static void PlayerAngles(centity_t *cent, vec3_t srcAngles, vec3_t legs[3],
                             vec3_t torso[3], vec3_t head[3]);
    static void PlayerWWSmoothing(centity_t *cent, vec3_t in[3],
                                  vec3_t out[3]);
    static void PlayerNonSegAngles(centity_t *cent, vec3_t srcAngles,
                                   vec3_t nonSegAxis[3]);
    static void PlayerUpgrades(centity_t *cent, refEntity_t *torso);
    static void PlayerFloatSprite(centity_t *cent, qhandle_t shader);
    static void PlayerSprites(centity_t *cent);
    static bool PlayerShadow(centity_t *cent, float32 *shadowPlane,
                             class_t _class);
    static void PlayerSplash(centity_t *cent, class_t _class);
    static sint LightVerts(vec3_t normal, sint numVerts, polyVert_t *verts);
    static sint LightFromDirection(vec3_t point, vec3_t direction);
    static sint AmbientLight(vec3_t point);
    static void Player(centity_t *cent);
    static void Corpse(centity_t *cent);
    static void ResetPlayerEntity(centity_t *cent);
    static void PlayerDisconnect(vec3_t org);
    static void Bleed(vec3_t origin, vec3_t normal, sint entityNum);
    static void TeamStatusDisplay(centity_t *cent);
    static sint SortDistance2(const void *a, const void *b);
    static void DrawTeamStatus(void);
    static centity_t *GetPlayerLocation(void);
};

#endif //!__CGAME_PLAYERS_H__
