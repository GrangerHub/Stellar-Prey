////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 1999 - 2005 Id Software, Inc.
// Copyright(C) 2000 - 2006 Tim Angus
// Copyright(C) 2011 - 2021 Dusan Jocic <dusanjocic@msn.com>
//
// This file is part of OpenWolf.
//
// OpenWolf is free software; you can redistribute it
// and / or modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the License,
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
// File name:   bgame_local.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:  local definitions for the bg (both games) files
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __BG_LOCAL_H__
#define __BG_LOCAL_H__

#ifndef __BG_API_H__
#include <API/bgame_api.hpp>
#endif

#define MIN_WALK_NORMAL 0.7f   // can't walk on very steep slopes

#define STEPSIZE    18

#define TIMER_LAND        130
#define TIMER_GESTURE     (34*66+50)
#define TIMER_ATTACK      500 //nonsegmented models

#define OVERCLIP    1.001f

#define FALLING_THRESHOLD -900.0f //what vertical speed to start falling sound at

// all of the locals will be zeroed before each
// pmove, just to make damn sure we don't have
// any differences when running on client or server
typedef struct
{
    vec3_t forward, right, up;
    float32 frametime;
    sint msec;
    bool walking;
    bool groundPlane;
    bool ladder;
    trace_t groundTrace;
    float32 impactSpeed;
    vec3_t previous_origin;
    vec3_t previous_velocity;
    sint previous_waterlevel;
} pml_t;

extern pmove_t* pm;
extern pml_t pml;

// movement parameters
extern float32 pm_stopspeed;
extern float32 pm_duckScale;
extern float32 pm_swimScale;
extern float32 pm_wadeScale;

extern float32 pm_accelerate;
extern float32 pm_airaccelerate;
extern float32 pm_wateraccelerate;
extern float32 pm_flyaccelerate;

extern float32 pm_friction;
extern float32 pm_waterfriction;
extern float32 pm_flightfriction;

extern sint c_pmove;

class idCGameLocal;

//
// idGame
//
class idBothGamesLocal : public bgGame
{
public:
    virtual const buildableAttributes_t* BuildableByName( pointer name );
    virtual const buildableAttributes_t* BuildableByEntityName( pointer name );
    virtual const buildableAttributes_t* Buildable( buildable_t buildable );
    virtual bool BuildableAllowedInStage( buildable_t buildable, stage_t stage );
    virtual buildableConfig_t* BuildableConfig( buildable_t buildable );
    virtual void BuildableBoundingBox( buildable_t buildable, vec3_t mins, vec3_t maxs );
    virtual bool ParseBuildableFile( pointer filename, buildableConfig_t* bc );
    virtual void InitBuildableConfigs( void );
    virtual const classAttributes_t* ClassByName( pointer name );
    virtual const classAttributes_t* Class( class_t _class );
    virtual bool ClassAllowedInStage( class_t _class, stage_t stage );
    virtual classConfig_t* ClassConfig( class_t _class );
    virtual void ClassBoundingBox( class_t _class, vec3_t mins, vec3_t maxs, vec3_t cmaxs, vec3_t dmins, vec3_t dmaxs );
    virtual bool ClassHasAbility( class_t _class, sint ability );
    virtual sint ClassCanEvolveFromTo( class_t fclass, class_t tclass, sint credits, sint stage, sint cost );
    virtual bool AlienCanEvolve( class_t pclass, sint credits, sint stage );
    virtual bool ParseClassFile( pointer filename, classConfig_t* cc );
    virtual void InitClassConfigs( void );
    virtual const weaponAttributes_t* WeaponByName( pointer name );
    virtual const weaponAttributes_t* Weapon( weapon_t weapon );
    virtual bool WeaponAllowedInStage( weapon_t weapon, stage_t stage );
    virtual const upgradeAttributes_t* UpgradeByName( pointer name );
    virtual const upgradeAttributes_t* Upgrade( upgrade_t upgrade );
    virtual bool UpgradeAllowedInStage( upgrade_t upgrade, stage_t stage );
    virtual void EvaluateTrajectory( const trajectory_t* tr, sint atTime, vec3_t result );
    virtual void EvaluateTrajectoryDelta( const trajectory_t* tr, sint atTime, vec3_t result );
    virtual pointer EventName( sint num );
    virtual void AddPredictableEventToPlayerstate( sint newEvent, sint eventParm, playerState_t* ps );
    virtual void PlayerStateToEntityState( playerState_t* ps, entityState_t* s, bool snap );
    virtual void PlayerStateToEntityStateExtraPolate( playerState_t* ps, entityState_t* s, sint time, bool snap );
    virtual bool WeaponIsFull( weapon_t weapon, sint stats[], sint ammo, sint clips );
    virtual bool InventoryContainsWeapon( sint weapon, sint stats[] );
    virtual sint CalculateSlotsForInventory( sint stats[] );
    virtual void AddUpgradeToInventory( sint item, sint stats[] );
    virtual void RemoveUpgradeFromInventory( sint item, sint stats[] );
    virtual bool InventoryContainsUpgrade( sint item, sint stats[] );
    virtual void ActivateUpgrade( sint item, sint stats[] );
    virtual void DeactivateUpgrade( sint item, sint stats[] );
    virtual bool UpgradeIsActive( sint item, sint stats[] );
    virtual bool RotateAxis( vec3_t surfNormal, vec3_t inAxis[3], vec3_t outAxis[3], bool inverse, bool ceiling );
    virtual void GetClientNormal( const playerState_t* ps, vec3_t normal );
    virtual void PositionBuildableRelativeToPlayer( const playerState_t* ps, const vec3_t mins, const vec3_t maxs, void( *trace )( trace_t*, const vec3_t, const vec3_t, const vec3_t, const vec3_t, sint, sint ), vec3_t outOrigin, vec3_t outAngles, trace_t* tr );
    virtual sint GetValueOfPlayer( playerState_t* ps );
    virtual sint PlayerPoisonCloudTime( playerState_t* ps );
    virtual weapon_t GetPlayerWeapon( playerState_t* ps );
    virtual bool PlayerCanChangeWeapon( playerState_t* ps );
    virtual float32 atof_neg( valueType* token, bool allowNegative );
    virtual sint atoi_neg( valueType* token, bool allowNegative );
    virtual void ParseCSVEquipmentList( pointer string, weapon_t* weapons, sint weaponsSize, upgrade_t* upgrades, sint upgradesSize );
    virtual void ParseCSVClassList( pointer string, class_t* classes, sint classesSize );
    virtual void ParseCSVBuildableList( pointer string, buildable_t* buildables, sint buildablesSize );
    virtual void InitAllowedGameElements( void );
    virtual bool WeaponIsAllowed( weapon_t weapon );
    virtual bool UpgradeIsAllowed( upgrade_t upgrade );
    virtual bool ClassIsAllowed( class_t _class );
    virtual bool BuildableIsAllowed( buildable_t buildable );
    virtual bool ClientListTest( clientList_t* list, sint clientNum );
    virtual void ClientListAdd( clientList_t* list, sint clientNum );
    virtual void ClientListRemove( clientList_t* list, sint clientNum );
    virtual valueType* ClientListString( clientList_t* list );
    virtual void ClientListParse( clientList_t* list, pointer s );
    virtual weapon_t PrimaryWeapon( sint stats[] );
    virtual sint LoadEmoticons( valueType names[][MAX_EMOTICON_NAME_LEN], sint widths[] );
    virtual valueType* TeamName( team_t team );
    virtual bool SlideMove( bool gravity );
    virtual void StepEvent( vec3_t from, vec3_t to, vec3_t normal );
    virtual bool StepSlideMove( bool gravity, bool predictive );
    virtual bool PredictStepMove( void );
    virtual void AddEvent( sint newEvent );
    virtual void AddTouchEnt( sint entityNum );
    virtual void StartTorsoAnim( sint anim );
    virtual void StartLegsAnim( sint anim );
    virtual void ContinueLegsAnim( sint anim );
    virtual void ContinueTorsoAnim( sint anim );
    virtual void ForceLegsAnim( sint anim );
    virtual void ClipVelocity( vec3_t in, vec3_t normal, vec3_t out, float32 overbounce );
    virtual void Friction( void );
    virtual void Accelerate( vec3_t wishdir, float32 wishspeed, float32 accel );
    virtual float32 CmdScale( usercmd_t* cmd );
    virtual void SetMovementDir( void );
    virtual void CheckCharge( void );
    virtual bool CheckPounce( void );
    virtual bool CheckWallJump( void );
    virtual bool CheckJump( void );
    virtual bool CheckWaterJump( void );
    virtual bool CheckDodge( void );
    virtual void WaterJumpMove( void );
    virtual void WaterMove( void );
    virtual void JetPackMove( void );
    virtual void FlyMove( void );
    virtual void AirMove( void );
    virtual void ClimbMove( void );
    virtual void WalkMove( void );
    virtual void LadderMove( void );
    virtual void CheckLadder( void );
    virtual void DeadMove( void );
    virtual void NoclipMove( void );
    virtual sint FootstepForSurface( void );
    virtual void CrashLand( void );
    virtual sint CorrectAllSolid( trace_t* trace );
    virtual void GroundTraceMissed( void );
    virtual void GroundClimbTrace( void );
    virtual void GroundTrace( void );
    virtual void SetWaterLevel( void );
    virtual void SetViewheight( void );
    virtual void CheckDuck( void );
    virtual void Footsteps( void );
    virtual void WaterEvents( void );
    virtual void BeginWeaponChange( sint weapon );
    virtual void FinishWeaponChange( void );
    virtual void TorsoAnimation( void );
    virtual void Weapon( void );
    virtual void Animate( void );
    virtual void DropTimers( void );
    virtual void UpdateViewAngles( playerState_t* ps, const usercmd_t* cmd );
    virtual void PmoveSingle( pmove_t* pmove );
    virtual void Pmove( pmove_t* pmove );
    virtual void VoiceParseError( fileHandle_t handle, valueType* err );
    virtual voice_t* VoiceList( void );
    virtual bool VoiceParseTrack( sint handle, voiceTrack_t* voiceTrack );
    virtual voiceTrack_t* VoiceParseCommand( sint handle );
    virtual voiceCmd_t* VoiceParse( valueType* name );
    virtual voice_t* VoiceInit( void );
    virtual void PrintVoices( voice_t* voices, sint debugLevel );
    virtual voice_t* VoiceByName( voice_t* head, valueType* name );
    virtual voiceCmd_t* VoiceCmdFind( voiceCmd_t* head, valueType* name, sint* cmdNum );
    virtual voiceCmd_t* VoiceCmdByNum( voiceCmd_t* head, sint num );
    virtual voiceTrack_t* VoiceTrackByNum( voiceTrack_t* head, sint num );
    virtual voiceTrack_t* VoiceTrackFind( voiceTrack_t* head, team_t team, class_t _class, weapon_t weapon, sint enthusiasm, sint* trackNum );
    virtual void* Alloc( size_t size );
    virtual void Free( void* ptr );
    virtual void InitMemory( void );
    virtual void DefragmentMemory( void );
    
};

extern idBothGamesLocal bgLocal;

#endif // !__BG_LOCAL_H__
