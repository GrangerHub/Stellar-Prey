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
// File name:   sgame_active.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SGAME_ACTIVE_H__
#define __SGAME_ACTIVE_H__

class idSGameLocal;

class idSGameActive: public idSGameLocal
{
public:
    idSGameActive();
    ~idSGameActive();
    
    static void UnlaggedStore( void );
    static void UnlaggedClear( gentity_t* ent );
    static void UnlaggedCalc( sint time, gentity_t* skipEnt );
    static void UnlaggedOn( gentity_t* attacker, vec3_t muzzle, float32 range );
    static void UnlaggedOff( void );
    static void ClientEndFrame( gentity_t* ent );
    static void DamageFeedback( gentity_t* player );
    static void WorldEffects( gentity_t* ent );
    static void SendPendingPredictableEvents( playerState_t* ps );
    static void SpectatorClientEndFrame( gentity_t* ent );
    static void TouchTriggers( gentity_t* ent );
    static void RunClient( gentity_t* ent );
    static void SetClientSound( gentity_t* ent );
    static void ClientThink_real( gentity_t* ent );
    static sint GetClientMass( gentity_t* ent );
    static void ClientShove( gentity_t* ent, gentity_t* victim );
    static void ClientImpacts( gentity_t* ent, pmove_t* pm );
    static void SpectatorThink( gentity_t* ent, usercmd_t* ucmd );
    static bool ClientInactivityTimer( gclient_t* client );
    static void ClientTimerActions( gentity_t* ent, sint msec );
    static void ClientIntermissionThink( gclient_t* client );
    static void ClientEvents( gentity_t* ent, sint oldEventSequence );
    static void UnlaggedDetectCollisions( gentity_t* ent );
    static void CheckZap( gentity_t* ent );
    static void OtherTouchTriggers( gentity_t* ent );
};

#endif // !__SGAME_ACTIVE_H__
