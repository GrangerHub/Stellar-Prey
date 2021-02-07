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
// File name:   sgame_trigger.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SGAME_TRIGGER_H__
#define __SGAME_TRIGGER_H__

class idSGameLocal;

class idSGameTrigger : public idSGameLocal
{
public:
    idSGameTrigger();
    ~idSGameTrigger();
    
    static void InitTrigger( gentity_t* self );
    static void multi_wait( gentity_t* ent );
    static void multi_trigger( gentity_t* ent, gentity_t* activator );
    static void Use_Multi( gentity_t* ent, gentity_t* other, gentity_t* activator );
    static void Touch_Multi( gentity_t* self, gentity_t* other, trace_t* trace );
    static void SP_trigger_multiple( gentity_t* ent );
    static void trigger_always_think( gentity_t* ent );
    static void SP_trigger_always( gentity_t* ent );
    static void trigger_push_touch( gentity_t* self, gentity_t* other, trace_t* trace );
    static void AimAtTarget( gentity_t* self );
    static void SP_trigger_push( gentity_t* self );
    static void Use_target_push( gentity_t* self, gentity_t* other, gentity_t* activator );
    static void SP_target_push( gentity_t* self );
    static void trigger_teleporter_use( gentity_t* ent, gentity_t* other, gentity_t* activator );
    static void SP_trigger_teleport( gentity_t* self );
    static void hurt_use( gentity_t* self, gentity_t* other, gentity_t* activator );
    static void hurt_touch( gentity_t* self, gentity_t* other, trace_t* trace );
    static void SP_trigger_hurt( gentity_t* self );
    static void func_timer_think( gentity_t* self );
    static void func_timer_use( gentity_t* self, gentity_t* other, gentity_t* activator );
    static void SP_func_timer( gentity_t* self );
    static void trigger_stage_use( gentity_t* self, gentity_t* other, gentity_t* activator );
    static void SP_trigger_stage( gentity_t* self );
    static void trigger_win( gentity_t* self, gentity_t* other, gentity_t* activator );
    static void SP_trigger_win( gentity_t* self );
    static bool trigger_buildable_match( gentity_t* self, gentity_t* activator );
    static void trigger_buildable_trigger( gentity_t* self, gentity_t* activator );
    static void trigger_buildable_touch( gentity_t* ent, gentity_t* other, trace_t* trace );
    static void trigger_buildable_use( gentity_t* ent, gentity_t* other, gentity_t* activator );
    static void SP_trigger_buildable( gentity_t* self );
    static bool trigger_class_match( gentity_t* self, gentity_t* activator );
    static void trigger_class_trigger( gentity_t* self, gentity_t* activator );
    static void trigger_class_touch( gentity_t* ent, gentity_t* other, trace_t* trace );
    static void trigger_class_use( gentity_t* ent, gentity_t* other, gentity_t* activator );
    static void SP_trigger_class( gentity_t* self );
    static bool trigger_equipment_match( gentity_t* self, gentity_t* activator );
    static void trigger_equipment_trigger( gentity_t* self, gentity_t* activator );
    static void trigger_equipment_touch( gentity_t* ent, gentity_t* other, trace_t* trace );
    static void trigger_equipment_use( gentity_t* ent, gentity_t* other, gentity_t* activator );
    static void SP_trigger_equipment( gentity_t* self );
    static void trigger_gravity_touch( gentity_t* ent, gentity_t* other, trace_t* trace );
    static void trigger_gravity_use( gentity_t* ent, gentity_t* other, gentity_t* activator );
    static void SP_trigger_gravity( gentity_t* self );
    static void trigger_heal_use( gentity_t* self, gentity_t* other, gentity_t* activator );
    static void trigger_heal_touch( gentity_t* self, gentity_t* other, trace_t* trace );
    static void SP_trigger_heal( gentity_t* self );
    static void trigger_ammo_touch( gentity_t* self, gentity_t* other, trace_t* trace );
    static void SP_trigger_ammo( gentity_t* self );
    static void trigger_teleporter_touch( gentity_t* self, gentity_t* other, trace_t* trace );
    static void Checktrigger_stages( team_t team, stage_t stage );
};

#endif //!__SGAME_TRIGGER_H__