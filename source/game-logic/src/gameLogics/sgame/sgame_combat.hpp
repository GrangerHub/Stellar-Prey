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
// File name:   sgame_combat.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SGAME_COMBAT_H__
#define __SGAME_COMBAT_H__

static damageRegion_t g_damageRegions[PCL_NUM_CLASSES][MAX_DAMAGE_REGIONS];
static sint g_numDamageRegions[PCL_NUM_CLASSES];

static damageRegion_t g_armourRegions[UP_NUM_UPGRADES][MAX_DAMAGE_REGIONS];
static sint g_numArmourRegions[UP_NUM_UPGRADES];

class idSGameLocal;

class idSGameCombat : public idSGameLocal
{
public:
    idSGameCombat();
    ~idSGameCombat();
    
    static bool CanDamage( gentity_t* targ, vec3_t origin );
    static void Damage( gentity_t* targ, gentity_t* inflictor, gentity_t* attacker, vec3_t dir, vec3_t point, sint damage, sint dflags, sint mod );
    static bool RadiusDamage( vec3_t origin, gentity_t* attacker, float32 damage, float32 radius, gentity_t* ignore, sint mod );
    static bool SelectiveRadiusDamage( vec3_t origin, gentity_t* attacker, float32 damage, float32 radius, gentity_t* ignore, sint mod, sint team );
    static float32 RewardAttackers( gentity_t* self );
    static void AddScore( gentity_t* ent, sint score );
    static void LogDestruction( gentity_t* self, gentity_t* actor, sint mod );
    static void LookAtKiller( gentity_t* self, gentity_t* inflictor, gentity_t* attacker );
    static void player_die( gentity_t* self, gentity_t* inflictor, gentity_t* attacker, sint damage, sint mod );
    static sint ParseDmgScript( damageRegion_t* regions, valueType* buf );
    static float32 GetRegionDamageModifier( gentity_t* targ, sint _class, sint piece );
    static float32 GetNonLocDamageModifier( gentity_t* targ, sint _class );
    static float32 GetPointDamageModifier( gentity_t* targ, damageRegion_t* regions, sint len, float32 angle, float32 height );
    static float32 CalcDamageModifier( vec3_t point, gentity_t* targ, gentity_t* attacker, sint _class, sint dflags );
    static void InitDamageLocations( void );
};

#endif // !__SGAME_COMBAT_H__
