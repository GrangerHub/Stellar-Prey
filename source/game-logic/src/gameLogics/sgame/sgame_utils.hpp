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
// File name:   sgame_utils.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SGAME_UTILS_H__
#define __SGAME_UTILS_H__

class idSGameLocal;

typedef struct {
    valueType oldShader[MAX_QPATH];
    valueType newShader[MAX_QPATH];
    float32 timeOffset;
} shaderRemap_t;

#define MAX_SHADER_REMAPS 128

static sint remapCount = 0;
static shaderRemap_t remappedShaders[MAX_SHADER_REMAPS];

class idSGameUtils : public idSGameLocal {
public:
    idSGameUtils();
    ~idSGameUtils();

    static void AddRemap(pointer oldShader, pointer newShader,
                         float32 timeOffset);
    static pointer BuildShaderStateConfig(void);
    static void TriggerMenu(sint clientNum, dynMenu_t menu);
    static void TriggerMenu2(sint clientNum, dynMenu_t menu, sint arg);
    static void CloseMenus(sint clientNum);
    static sint FindConfigstringIndex(valueType *name, sint start, sint max,
                                      bool create);
    static sint ParticleSystemIndex(valueType *name);
    static sint ShaderIndex(valueType *name);
    static sint ModelIndex(valueType *name);
    static sint SoundIndex(valueType *name);
    static void TeamCommand(team_t team, valueType *cmd);
    static void KillBox(gentity_t *ent);
    static gentity_t *Find(gentity_t *from, sint fieldofs, pointer match);
    static gentity_t *PickTarget(valueType *targetname);
    static void UseTargets(gentity_t *ent, gentity_t *activator);
    static void SetMovedir(vec3_t angles, vec3_t movedir);
    static void InitGentity(gentity_t *e);
    static gentity_t *Spawn(void);
    static gentity_t *TempEntity(vec3_t origin, sint _event);
    static void Sound(gentity_t *ent, sint channel, sint soundIndex);
    static void FreeEntity(gentity_t *e);
    static void AddPredictableEvent(gentity_t *ent, sint _event,
                                    sint eventParm);
    static void AddEvent(gentity_t *ent, sint _event, sint eventParm);
    static void BroadcastEvent(sint _event, sint eventParm);
    static void SetOrigin(gentity_t *ent, vec3_t origin);
    static void SetAngle(gentity_t *ent, vec3_t angle);
    static bool Visible(gentity_t *ent1, gentity_t *ent2, sint contents);
    static gentity_t *ClosestEnt(vec3_t origin, gentity_t **entities,
                                 sint numEntities);
    static valueType *vtos(const vec3_t v);
};

#endif //!__SGAME_UTILS_H__
