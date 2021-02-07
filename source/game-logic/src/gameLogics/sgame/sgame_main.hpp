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
// File name:   sgame_main.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SGAME_MAIN_H__
#define __SGAME_MAIN_H__

class idSGameLocal;

class idSGameMain : public idSGameLocal
{
public:
    idSGameMain();
    ~idSGameMain();
    
    static void Printf( pointer fmt, ... );
    static void Error( pointer fmt, ... );
    static void ClearVotes( void );
    static sint SortRanks( const void* a, const void* b );
    static void ExitLevel( void );
    static void SendGameStat( team_t team );
    static void CheckIntermissionExit( void );
    static void CheckExitRules( void );
    static sint GetSpawnQueueLength( spawnQueue_t* sq );
    static sint PopSpawnQueue( spawnQueue_t* sq );
    static sint PeekSpawnQueue( spawnQueue_t* sq );
    static bool SearchSpawnQueue( spawnQueue_t* sq, sint clientNum );
    static bool PushSpawnQueue( spawnQueue_t* sq, sint clientNum );
    static bool RemoveFromSpawnQueue( spawnQueue_t* sq, sint clientNum );
    static sint GetPosInSpawnQueue( spawnQueue_t* sq, sint clientNum );
    static void MapConfigs( pointer mapname );
    static sint TimeTilSuddenDeath( void );
    static void CalculateRanks( void );
    static void DemoCommand( demoCommand_t cmd, pointer string );
    static void DemoSetClient( void );
    static void DemoRemoveClient( void );
    static void DemoSetStage( void );
    static void SendScoreboardMessageToAllClients( void );
    static void MoveClientToIntermission( gentity_t* client );
    static void FindIntermissionPoint( void );
    static void AdminMessage( pointer prefix, pointer fmt, ... );
    static void LogPrintf( pointer fmt, ... );
    static void Vote( gentity_t* ent, bool voting );
    static void TeamVote( gentity_t* ent, bool voting );
    static void CheckVote( void );
    static void CheckTeamVote( team_t teamnum );
    static void LogExit( pointer string );
    static void BeginIntermission( void );
    static void PrintSpawnQueue( spawnQueue_t* sq );
    static void RunThink( gentity_t* ent );
    static void EvaluateAcceleration( gentity_t* ent, sint msec );
    static void RegisterCvars( void );
    static void FindTeams( void );
    static void CountSpawns( void );
    static void InitSpawnQueue( spawnQueue_t* sq );
    static void UpdateCvars( void );
    static void CheckDemo( void );
    static void SpawnClients( team_t team );
    static void CalculateAvgPlayers( void );
    static void CalculateStages( void );
    static void CalculateBuildPoints( void );
    static bool ScoreIsTied( void );
    static void CheckCvars( void );
};

#endif // !__SGAME_MAIN_H__
