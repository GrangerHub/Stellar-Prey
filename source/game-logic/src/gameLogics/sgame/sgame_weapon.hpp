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
// File name:   sgame_weapon.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SGAME_WEAPONS_H__
#define __SGAME_WEAPONS_H__

static vec3_t sortReference;
static vec3_t forward, right, up;
static vec3_t muzzle;

class idSGameLocal;

class idSGameWeapons : public idSGameLocal {
public:
    idSGameWeapons();
    ~idSGameWeapons();

    static void FireWeapon(gentity_t *ent);
    static void FireWeapon2(gentity_t *ent);
    static void FireWeapon3(gentity_t *ent);
    static void ForceWeaponChange(gentity_t *ent, weapon_t weapon);
    static void poisonCloud(gentity_t *ent);
    static void WideTrace(trace_t *tr, gentity_t *ent, float32 range,
                          float32 width, float32 height, gentity_t **target);
    static void BounceProjectile(vec3_t start, vec3_t impact, vec3_t dir,
                                 vec3_t endout);
    static void SnapVectorNormal(vec3_t v, vec3_t normal);
    static void BloodSpurt(gentity_t *attacker, gentity_t *victim,
                           trace_t *tr);
    static void WideBloodSpurt(gentity_t *attacker, gentity_t *victim,
                               trace_t *tr);
    static void meleeAttack(gentity_t *ent, float32 range, float32 width,
                            float32 height, sint damage, meansOfDeath_t mod);
    static void bulletFire(gentity_t *ent, float32 spread, sint damage,
                           sint mod);
    static void ShotgunPattern(vec3_t origin, vec3_t origin2, sint seed,
                               gentity_t *ent);
    static void shotgunFire(gentity_t *ent);
    static void massDriverFire(gentity_t *ent);
    static void lockBlobLauncherFire(gentity_t *ent);
    static void hiveFire(gentity_t *ent);
    static void blasterFire(gentity_t *ent);
    static void pulseRifleFire(gentity_t *ent);
    static void flamerFire(gentity_t *ent);
    static void throwGrenade(gentity_t *ent);
    static void lasGunFire(gentity_t *ent);
    static void painSawFire(gentity_t *ent);
    static void LCChargeFire(gentity_t *ent, bool secondary);
    static void teslaFire(gentity_t *self);
    static void cancelBuildFire(gentity_t *ent);
    static void buildFire(gentity_t *ent, dynMenu_t menu);
    static void slowBlobFire(gentity_t *ent);
    static void bounceBallFire(gentity_t *ent);
    static void CalcMuzzlePoint(gentity_t *ent, vec3_t forward, vec3_t right,
                                vec3_t up, vec3_t muzzlePoint);
    static void UpdateZaps(gentity_t *ent);
    static void GiveClientMaxAmmo(gentity_t *ent, bool buyingEnergyAmmo);
    static void SnapVectorTowards(vec3_t v, vec3_t to);
    static sint SortDistance(const void *a, const void *b);
    static void CheckCkitRepair(gentity_t *ent);
    static void ChargeAttack(gentity_t *ent, gentity_t *victim);
    static void CrushAttack(gentity_t *ent, gentity_t *victim);
    static void CheckGrabAttack(gentity_t *ent);
    static bool CheckPounceAttack(gentity_t *ent);
    static bool CheckVenomAttack(gentity_t *ent);
};

#endif //!__SGAME_WEAPONS_H__
