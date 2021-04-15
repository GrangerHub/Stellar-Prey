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
// File name:   cgame_weapons.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CGAME_WEAPONS_H__
#define __CGAME_WEAPONS_H__

class idCGameLocal;

class idCGameWeapons : public idCGameLocal {
public:
    idCGameWeapons();
    ~idCGameWeapons();

    static void RegisterUpgrade(sint upgradeNum);
    static void InitUpgrades(void);
    static bool ParseWeaponAnimationFile(pointer filename,
                                         weaponInfo_t *weapon);
    static bool ParseWeaponModeSection(weaponInfoMode_t *wim,
                                       valueType **text_p);
    static bool ParseWeaponFile(pointer filename, weaponInfo_t *wi);
    static void RegisterWeapon(sint weaponNum);
    static void InitWeapons(void);
    static void SetWeaponLerpFrameAnimation(weapon_t weapon, lerpFrame_t *lf,
                                            sint newAnimation);
    static void WeaponAnimation(centity_t *cent, sint *old, sint *now,
                                float32 *backLerp);
    static sint MapTorsoToWeaponFrame(clientInfo_t *ci, sint frame);
    static void CalculateWeaponPosition(vec3_t origin, vec3_t angles);
    static float32 MachinegunSpinAngle(centity_t *cent, bool firing);
    static void AddPlayerWeapon(refEntity_t *parent, playerState_t *ps,
                                centity_t *cent);
    static void AddViewWeapon(playerState_t *ps);
    static bool WeaponSelectable(weapon_t weapon);
    static bool UpgradeSelectable(upgrade_t upgrade);
    static void DrawItemSelect(rectDef_t *rect, vec4_t color);
    static void DrawItemSelectText(rectDef_t *rect, float32 scale,
                                   sint textStyle);
    static void NextWeapon_f(void);
    static void PrevWeapon_f(void);
    static void Weapon_f(void);
    static void FireWeapon(centity_t *cent, weaponMode_t weaponMode);
    static void HandleAlienFeedback(centity_t *cent,
                                    alienFeedback_t feedbackType);
    static void MissileHitWall(weapon_t weaponNum, weaponMode_t weaponMode,
                               sint clientNum, vec3_t origin, vec3_t dir, impactSound_t soundType,
                               sint charge);
    static void MissileHitPlayer(weapon_t weaponNum, weaponMode_t weaponMode,
                                 vec3_t origin, vec3_t dir, sint entityNum, sint charge);
    static void MassDriverFire(entityState_t *es);
    static void Tracer(vec3_t source, vec3_t dest);
    static bool CalcMuzzlePoint(sint entityNum, vec3_t muzzle);
    static void Bullet(vec3_t end, sint sourceEntityNum, vec3_t normal,
                       bool flesh, sint fleshEntityNum);
    static void ShotgunPattern(vec3_t origin, vec3_t origin2, sint seed,
                               sint otherEntNum);
    static void ShotgunFire(entityState_t *es);
};

#endif //!__CGAME_WEAPONS_H__
