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
// File name:   sgame_buildable.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SGAME_BUILDABLE_H__
#define __SGAME_BUILDABLE_H__

static buildable_t  cmpBuildable;
static vec3_t       cmpOrigin;

class idSGameLocal;

class idSGameBuildable : public idSGameLocal {
public:
    idSGameBuildable();
    ~idSGameBuildable();

    static bool AHovel_Blocked(gentity_t *hovel, gentity_t *player,
                               bool provideExit);
    static gentity_t *CheckSpawnPoint(sint spawnNum, vec3_t origin,
                                      vec3_t normal, buildable_t spawn, vec3_t spawnOrigin);
    static buildable_t IsPowered(vec3_t origin);
    static bool IsDCCBuilt(void);
    static sint FindDCC(gentity_t *self);
    static bool FindOvermind(gentity_t *self);
    static bool FindCreep(gentity_t *self);
    static void BuildableThink(gentity_t *ent, sint msec);
    static bool BuildableRange(vec3_t origin, float32 r,
                               buildable_t buildable);
    static itemBuildError_t CanBuild(gentity_t *ent, buildable_t buildable,
                                     sint distance, vec3_t origin);
    static bool BuildIfValid(gentity_t *ent, buildable_t buildable);
    static void SetBuildableAnim(gentity_t *ent, buildableAnimNumber_t anim,
                                 bool force);
    static void SetIdleBuildableAnim(gentity_t *ent,
                                     buildableAnimNumber_t anim);
    static void SpawnBuildable(gentity_t *ent, buildable_t buildable);
    static void QueueBuildPoints(gentity_t *self);
    static gentity_t *FindBuildable(buildable_t buildable);
    static sint NumberOfDependants(gentity_t *self);
    static bool FindPower(gentity_t *self);
    static gentity_t *PowerEntityForPoint(vec3_t origin);
    static bool FindRepeater(gentity_t *self);
    static gentity_t *RepeaterEntityForPoint(vec3_t origin);
    static bool IsCreepHere(vec3_t origin);
    static void CreepSlow(gentity_t *self);
    static void nullDieFunction(gentity_t *self, gentity_t *inflictor,
                                gentity_t *attacker, sint damage, sint mod);
    static void AGeneric_CreepRecede(gentity_t *self);
    static void AGeneric_Blast(gentity_t *self);
    static void AGeneric_Die(gentity_t *self, gentity_t *inflictor,
                             gentity_t *attacker, sint damage, sint mod);
    static void AGeneric_CreepCheck(gentity_t *self);
    static void AGeneric_Think(gentity_t *self);
    static void AGeneric_Pain(gentity_t *self, gentity_t *attacker,
                              sint damage);
    static void ASpawn_Die(gentity_t *self, gentity_t *inflictor,
                           gentity_t *attacker, sint damage, sint mod);
    static void ASpawn_Think(gentity_t *self);
    static void AOvermind_Think(gentity_t *self);
    static void ABarricade_Pain(gentity_t *self, gentity_t *attacker,
                                sint damage);
    static void ABarricade_Shrink(gentity_t *self, bool shrink);
    static void ABarricade_Die(gentity_t *self, gentity_t *inflictor,
                               gentity_t *attacker, sint damage, sint mod);
    static void ABarricade_Think(gentity_t *self);
    static void ABarricade_Touch(gentity_t *self, gentity_t *other,
                                 trace_t *trace);
    static void AAcidTube_Think(gentity_t *self);
    static bool AHive_CheckTarget(gentity_t *self, gentity_t *enemy);
    static void AHive_Think(gentity_t *self);
    static void AHive_Pain(gentity_t *self, gentity_t *attacker, sint damage);
    static void AHive_Die(gentity_t *self, gentity_t *inflictor,
                          gentity_t *attacker, sint damage, sint mod);
    static bool APropHovel_Blocked(vec3_t origin, vec3_t angles, vec3_t normal,
                                   gentity_t *player);
    static void AHovel_Use(gentity_t *self, gentity_t *other,
                           gentity_t *activator);
    static void AHovel_Think(gentity_t *self);
    static void AHovel_Die(gentity_t *self, gentity_t *inflictor,
                           gentity_t *attacker, sint damage, sint mod);
    static void ABooster_Touch(gentity_t *self, gentity_t *other,
                               trace_t *trace);
    static void ATrapper_FireOnEnemy(gentity_t *self, sint firespeed,
                                     float32 range);
    static bool ATrapper_CheckTarget(gentity_t *self, gentity_t *target,
                                     sint range);
    static void ATrapper_FindEnemy(gentity_t *ent, sint range);
    static void ATrapper_Think(gentity_t *self);
    static void HRepeater_Think(gentity_t *self);
    static void HRepeater_Use(gentity_t *self, gentity_t *other,
                              gentity_t *activator);
    static void HReactor_Think(gentity_t *self);
    static void HArmoury_Activate(gentity_t *self, gentity_t *other,
                                  gentity_t *activator);
    static void HArmoury_Think(gentity_t *self);
    static void HDCC_Think(gentity_t *self);
    static void HMedistat_Die(gentity_t *self, gentity_t *inflictor,
                              gentity_t *attacker, sint damage, sint mod);
    static void HMedistat_Think(gentity_t *self);
    static bool HMGTurret_CheckTarget(gentity_t *self, gentity_t *target,
                                      bool los_check);
    static bool HMGTurret_TrackEnemy(gentity_t *self);
    static void HMGTurret_FindEnemy(gentity_t *self);
    static void HMGTurret_Think(gentity_t *self);
    static void HTeslaGen_Think(gentity_t *self);
    static void HSpawn_Disappear(gentity_t *self);
    static void HSpawn_Blast(gentity_t *self);
    static void HSpawn_Die(gentity_t *self, gentity_t *inflictor,
                           gentity_t *attacker, sint damage, sint mod);
    static void HSpawn_Think(gentity_t *self);
    static void FreeMarkedBuildables(gentity_t *deconner);
    static itemBuildError_t SufficientBPAvailable(buildable_t buildable,
            vec3_t origin);
    static bool BuildablesIntersect(buildable_t a, vec3_t originA,
                                    buildable_t b, vec3_t originB);
    static sint CompareBuildablesForRemoval(const void *a, const void *b);
    static void SetBuildableLinkState(bool link);
    static void SetBuildableMarkedLinkState(bool link);
    static gentity_t *Build(gentity_t *builder, buildable_t buildable,
                            vec3_t origin, vec3_t angles);
    static void FinishSpawningBuildable(gentity_t *ent);
    static void LayoutBuildItem(buildable_t buildable, vec3_t origin,
                                vec3_t angles, vec3_t origin2, vec3_t angles2);
    static void BaseSelfDestruct(team_t team);
    static void LayoutSave(valueType *name);
    static void LayoutSelect(void);
    static void BuildableTouchTriggers(gentity_t *ent);
    static sint LayoutList(pointer map, valueType *list, sint len);
    static void LayoutLoad(void);
};

#endif // !__SGAME_BUILDABLE_H__
