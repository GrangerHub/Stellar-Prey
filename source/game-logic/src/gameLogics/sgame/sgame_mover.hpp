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
// File name:   sgame_mover.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SGAME_MOVER_H__
#define __SGAME_MOVER_H__

class idSGameLocal;

class idSGameMover : public idSGameLocal
{
public:
    idSGameMover();
    ~idSGameMover();
    
    static gentity_t* TestEntityPosition( gentity_t* ent );
    static void CreateRotationMatrix( vec3_t angles, vec3_t matrix[3] );
    static void TransposeMatrix( vec3_t matrix[3], vec3_t transpose[3] );
    static void RotatePoint( vec3_t point, vec3_t matrix[3] );
    static bool TryPushingEntity( gentity_t* check, gentity_t* pusher, vec3_t move, vec3_t amove );
    static bool MoverPush( gentity_t* pusher, vec3_t move, vec3_t amove, gentity_t** obstacle );
    static void MoverTeam( gentity_t* ent );
    static void SetMoverState( gentity_t* ent, moverState_t moverState, sint time );
    static void MatchTeam( gentity_t* teamLeader, sint moverState, sint time );
    static void ReturnToPos1( gentity_t* ent );
    static void ReturnToApos1( gentity_t* ent );
    static void Think_ClosedModelDoor( gentity_t* ent );
    static void Think_CloseModelDoor( gentity_t* ent );
    static void Think_OpenModelDoor( gentity_t* ent );
    static void Reached_BinaryMover( gentity_t* ent );
    static void Use_BinaryMover( gentity_t* ent, gentity_t* other, gentity_t* activator );
    static void InitMover( gentity_t* ent );
    static void InitRotator( gentity_t* ent );
    static void Blocked_Door( gentity_t* ent, gentity_t* other );
    static void Touch_DoorTriggerSpectator( gentity_t* ent, gentity_t* other, trace_t* trace );
    static void manualDoorTriggerSpectator( gentity_t* door, gentity_t* player );
    static void Think_SpawnNewDoorTrigger( gentity_t* ent );
    static void Think_MatchTeam( gentity_t* ent );
    static void SP_func_door( gentity_t* ent );
    static void SP_func_door_rotating( gentity_t* ent );
    static void SP_func_door_model( gentity_t* ent );
    static void Touch_Plat( gentity_t* ent, gentity_t* other, trace_t* trace );
    static void Touch_PlatCenterTrigger( gentity_t* ent, gentity_t* other, trace_t* trace );
    static void SpawnPlatTrigger( gentity_t* ent );
    static void SP_func_plat( gentity_t* ent );
    static void Touch_Button( gentity_t* ent, gentity_t* other, trace_t* trace );
    static void SP_func_button( gentity_t* ent );
    static void Think_BeginMoving( gentity_t* ent );
    static void Reached_Train( gentity_t* ent );
    static void Start_Train( gentity_t* ent, gentity_t* other, gentity_t* activator );
    static void Stop_Train( gentity_t* ent, gentity_t* other, gentity_t* activator );
    static void Use_Train( gentity_t* ent, gentity_t* other, gentity_t* activator );
    static void Think_SetupTrainTargets( gentity_t* ent );
    static void SP_path_corner( gentity_t* self );
    static void Blocked_Train( gentity_t* self, gentity_t* other );
    static void SP_func_train( gentity_t* self );
    static void SP_func_static( gentity_t* ent );
    static void SP_func_rotating( gentity_t* ent );
    static void SP_func_bobbing( gentity_t* ent );
    static void SP_func_pendulum( gentity_t* ent );
    static void Touch_DoorTrigger( gentity_t* ent, gentity_t* other, trace_t* trace );
    static void RunMover( gentity_t* ent );
    static void manualTriggerSpectator( gentity_t* trigger, gentity_t* player );
};

#endif // !__SGAME_MOVER_H__
