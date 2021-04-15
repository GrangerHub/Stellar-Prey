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
// File name:   sgame_misc.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SGAME_MISC_H__
#define __SGAME_MISC_H__

#define MISSILE_PRESTEP_TIME  50

class idSGameLocal;

class idSGameMisc : public idSGameLocal {
public:
    idSGameMisc();
    ~idSGameMisc();

    static void SP_info_null(gentity_t *self);
    static void SP_info_notnull(gentity_t *self);
    static void SP_func_sound(gentity_t *self);
    static void SP_light(gentity_t *self);
    static void SP_misc_teleporter_dest(gentity_t *ent);
    static void SP_misc_model(gentity_t *ent);
    static void locateCamera(gentity_t *ent);
    static void SP_misc_portal_surface(gentity_t *ent);
    static void SP_misc_portal_camera(gentity_t *ent);
    static void SP_toggle_particle_system(gentity_t *self);
    static void SP_use_particle_system(gentity_t *self, gentity_t *other,
                                       gentity_t *activator);
    static void SP_misc_particle_system(gentity_t *self);
    static void SP_use_anim_model(gentity_t *self, gentity_t *other,
                                  gentity_t *activator);
    static void SP_misc_anim_model(gentity_t *self);
    static void SP_use_light_flare(gentity_t *self, gentity_t *other,
                                   gentity_t *activator);
    static void findEmptySpot(vec3_t origin, float32 radius, vec3_t spot);
    static void SP_misc_light_flare(gentity_t *self);
    static void SP_misc_cubemap(gentity_t *ent);
    static void Sound_Think(gentity_t *ent);
    static void TeleportPlayer(gentity_t *player, vec3_t origin,
                               vec3_t angles);
};

#endif // !__SGAME_MISC_H__