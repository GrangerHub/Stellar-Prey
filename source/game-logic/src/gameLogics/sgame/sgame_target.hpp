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
// File name:   sgame_target.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SGAME_TARGET_H__
#define __SGAME_TARGET_H__

class idSGameLocal;

class idSGameTarget : public idSGameLocal {
public:
    idSGameTarget();
    ~idSGameTarget();

    static void Think_Target_Delay(gentity_t *ent);
    static void Use_Target_Delay(gentity_t *ent, gentity_t *other,
                                 gentity_t *activator);
    static void SP_target_delay(gentity_t *ent);
    static void Use_Target_Score(gentity_t *ent, gentity_t *other,
                                 gentity_t *activator);
    static void SP_target_score(gentity_t *ent);
    static void Use_Target_Print(gentity_t *ent, gentity_t *other,
                                 gentity_t *activator);
    static void SP_target_print(gentity_t *ent);
    static void Use_Target_Speaker(gentity_t *ent, gentity_t *other,
                                   gentity_t *activator);
    static void SP_target_speaker(gentity_t *ent);
    static void target_teleporter_use(gentity_t *self, gentity_t *other,
                                      gentity_t *activator);
    static void SP_target_teleporter(gentity_t *self);
    static void target_relay_use(gentity_t *self, gentity_t *other,
                                 gentity_t *activator);
    static void SP_target_relay(gentity_t *self);
    static void target_kill_use(gentity_t *self, gentity_t *other,
                                gentity_t *activator);
    static void SP_target_kill(gentity_t *self);
    static void SP_target_position(gentity_t *self);
    static void target_location_linkup(gentity_t *ent);
    static void SP_target_location(gentity_t *self);
    static void target_rumble_think(gentity_t *self);
    static void target_rumble_use(gentity_t *self, gentity_t *other,
                                  gentity_t *activator);
    static void SP_target_rumble(gentity_t *self);
    static void target_alien_win_use(gentity_t *self, gentity_t *other,
                                     gentity_t *activator);
    static void SP_target_alien_win(gentity_t *self);
    static void target_human_win_use(gentity_t *self, gentity_t *other,
                                     gentity_t *activator);
    static void SP_target_human_win(gentity_t *self);
    static void target_hurt_use(gentity_t *self, gentity_t *other,
                                gentity_t *activator);
    static void SP_target_hurt(gentity_t *self);
};

#endif // !__SGAME_TARGET_H__
