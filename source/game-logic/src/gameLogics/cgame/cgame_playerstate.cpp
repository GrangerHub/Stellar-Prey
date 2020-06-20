////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 1999 - 2005 Id Software, Inc.
// Copyright(C) 2000 - 2006 Tim Angus
// Copyright(C) 2011 - 2018 Dusan Jocic <dusanjocic@msn.com>
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
// File name:   cgame_playerstate.cpp
// Version:     v1.00
// Created:
// Compilers:   Visual Studio 2015
// Description: this file acts on changes in a new playerState_t
//              With normal play, this will be done after local prediction,
//              but when following another player or playing back a demo, it
//              will be checked following another player or playing back a demo,
//              it will be checked when the snapshot transitions like all the
//              other entities
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <cgame/cgame_precompiled.h>

/*
===============
idCGamePlayerState::idCGamePlayerState
===============
*/
idCGamePlayerState::idCGamePlayerState( void )
{
}

/*
===============
idCGamePlayers::~idCGamePlayers
===============
*/
idCGamePlayerState::~idCGamePlayerState( void )
{
}

/*
==============
idCGamePlayerState::DamageFeedback
==============
*/
void idCGamePlayerState::DamageFeedback( sint yawByte, sint pitchByte, sint damage )
{
    float32 left, front, up, kick, scale, dist, yaw, pitch;
    sint health;
    vec3_t dir, angles;
    
    // show the attacking player's head and name in corner
    cg.attackerTime = cg.time;
    
    // the lower on health you are, the greater the view kick will be
    health = cg.snap->ps.stats[STAT_HEALTH];
    
    if( health < 40 )
    {
        scale = 1;
    }
    else
    {
        scale = 40.0 / health;
    }
    
    kick = damage * scale;
    
    if( kick < 5 )
    {
        kick = 5;
    }
    
    if( kick > 10 )
    {
        kick = 10;
    }
    
    // if yaw and pitch are both 255, make the damage always centered (falling, etc)
    if( yawByte == 255 && pitchByte == 255 )
    {
        cg.damageX = 0;
        cg.damageY = 0;
        cg.v_dmg_roll = 0;
        cg.v_dmg_pitch = -kick;
    }
    else
    {
        // positional
        pitch = pitchByte / 255.0 * 360;
        yaw = yawByte / 255.0 * 360;
        
        angles[ PITCH ] = pitch;
        angles[ YAW ] = yaw;
        angles[ ROLL ] = 0;
        
        AngleVectors( angles, dir, nullptr, nullptr );
        VectorSubtract( vec3_origin, dir, dir );
        
        front = DotProduct( dir, cg.refdef.viewaxis[ 0 ] );
        left = DotProduct( dir, cg.refdef.viewaxis[ 1 ] );
        up = DotProduct( dir, cg.refdef.viewaxis[ 2 ] );
        
        dir[ 0 ] = front;
        dir[ 1 ] = left;
        dir[ 2 ] = 0;
        dist = VectorLength( dir );
        
        if( dist < 0.1f )
        {
            dist = 0.1f;
        }
        
        cg.v_dmg_roll = kick * left;
        
        cg.v_dmg_pitch = -kick * front;
        
        if( front <= 0.1 )
        {
            front = 0.1f;
        }
        
        cg.damageX = -left / front;
        cg.damageY = up / dist;
    }
    
    // clamp the position
    if( cg.damageX > 1.0 )
    {
        cg.damageX = 1.0;
    }
    
    if( cg.damageX < -1.0 )
    {
        cg.damageX = -1.0;
    }
    
    if( cg.damageY > 1.0 )
    {
        cg.damageY = 1.0;
    }
    
    if( cg.damageY < -1.0 )
    {
        cg.damageY = -1.0;
    }
    
    // don't let the screen flashes vary as much
    if( kick > 10 )
    {
        kick = 10;
    }
    
    cg.damageValue = kick;
    cg.v_dmg_time = cg.time + DAMAGE_TIME;
    cg.damageTime = cg.snap->serverTime;
}

/*
================
idCGamePlayerState::Respawn

A respawn happened this snapshot
================
*/
void idCGamePlayerState::Respawn( void )
{
    // no error decay on player movement
    cg.thisFrameTeleport = true;
    
    // display weapons available
    cg.weaponSelectTime = cg.time;
    
    // select the weapon the server says we are using
    cg.weaponSelect = cg.snap->ps.weapon;
    
    idCGameDraw::ResetPainBlend( );
}

/*
==============
idCGamePlayerState::CheckPlayerstateEvents
==============
*/
void idCGamePlayerState::CheckPlayerstateEvents( playerState_t* ps, playerState_t* ops )
{
    sint i, _event;
    centity_t* cent;
    
    if( ps->externalEvent && ps->externalEvent != ops->externalEvent )
    {
        cent = &cg_entities[ ps->clientNum ];
        cent->currentState._event = ps->externalEvent;
        cent->currentState.eventParm = ps->externalEventParm;
        idCGameEvent::EntityEvent( cent, cent->lerpOrigin );
    }
    
    cent = &cg.predictedPlayerEntity; // cg_entities[ ps->clientNum ];
    
    // go through the predictable events buffer
    for( i = ps->eventSequence - MAX_EVENTS; i < ps->eventSequence; i++ )
    {
        // if we have a new predictable event
        if( i >= ops->eventSequence ||
                // or the server told us to play another event instead of a predicted event we already issued
                // or something the server told us changed our prediction causing a different event
                ( i > ops->eventSequence - MAX_EVENTS && ps->events[ i & ( MAX_EVENTS - 1 ) ] != ops->events[ i & ( MAX_EVENTS - 1 ) ] ) )
        {
            _event = ps->events[ i & ( MAX_EVENTS - 1 ) ];
            
            cent->currentState._event = _event;
            cent->currentState.eventParm = ps->eventParms[ i & ( MAX_EVENTS - 1 ) ];
            idCGameEvent::EntityEvent( cent, cent->lerpOrigin );
            cg.predictableEvents[ i & ( MAX_PREDICTED_EVENTS - 1 ) ] = _event;
            
            cg.eventSequence++;
        }
    }
}

/*
==================
idCGamePlayerState::CheckChangedPredictableEvents
==================
*/
void idCGamePlayerState::CheckChangedPredictableEvents( playerState_t* ps )
{
    sint i, _event;
    centity_t* cent;
    
    cent = &cg.predictedPlayerEntity;
    
    for( i = ps->eventSequence - MAX_EVENTS; i < ps->eventSequence; i++ )
    {
        if( i >= cg.eventSequence )
        {
            continue;
        }
        
        // if this event is not further back in than the maximum predictable events we remember
        if( i > cg.eventSequence - MAX_PREDICTED_EVENTS )
        {
            // if the new playerstate event is different from a previously predicted one
            if( ps->events[ i & ( MAX_EVENTS - 1 ) ] != cg.predictableEvents[ i & ( MAX_PREDICTED_EVENTS - 1 ) ] )
            {
                _event = ps->events[ i & ( MAX_EVENTS - 1 ) ];
                cent->currentState._event = _event;
                cent->currentState.eventParm = ps->eventParms[ i & ( MAX_EVENTS - 1 ) ];
                idCGameEvent::EntityEvent( cent, cent->lerpOrigin );
                
                cg.predictableEvents[ i & ( MAX_PREDICTED_EVENTS - 1 ) ] = _event;
                
                if( cg_showmiss.integer )
                {
                    Printf( "WARNING: changed predicted event\n" );
                }
            }
        }
    }
}

/*
==================
idCGamePlayerState::CheckLocalSounds
==================
*/
void idCGamePlayerState::CheckLocalSounds( playerState_t* ps, playerState_t* ops )
{
    sint reward;
    
    // don't play the sounds if the player just spawned
    if( ps->persistant[PERS_SPECSTATE] != ops->persistant[PERS_SPECSTATE] )
    {
        return;
    }
    
    // health changes of more than -1 should make pain sounds
    if( ps->stats[ STAT_HEALTH ] < ops->stats[ STAT_HEALTH ] - 1 )
    {
        if( ps->stats[STAT_HEALTH] > 0 )
        {
            idCGameEvent::PainEvent( &cg.predictedPlayerEntity, ps->stats[STAT_HEALTH] );
        }
    }
    
    // if we are going into the intermission, don't start any voices
    if( cg.intermissionStarted )
    {
        return;
    }
    
    // reward sounds
    reward = false;
}

/*
===============
idCGamePlayerState::TransitionPlayerState
===============
*/
void idCGamePlayerState::TransitionPlayerState( playerState_t* ps, playerState_t* ops )
{
    // check for changing follow mode
    if( ps->clientNum != ops->clientNum )
    {
        cg.thisFrameTeleport = true;
        // make sure we don't get any unwanted transition effects
        *ops = *ps;
        
        idCGameDraw::ResetPainBlend( );
    }
    
    // damage events (player is getting wounded)
    if( ps->damageEvent != ops->damageEvent && ps->damageCount )
    {
        DamageFeedback( ps->damageYaw, ps->damagePitch, ps->damageCount );
    }
    
    // respawning
    if( ps->persistant[PERS_SPAWN_COUNT] != ops->persistant[PERS_SPAWN_COUNT] )
    {
        Respawn();
    }
    
    if( cg.mapRestart )
    {
        Respawn( );
        cg.mapRestart = false;
    }
    
    if( cg.snap->ps.pm_type != PM_INTERMISSION && ps->persistant[PERS_SPECSTATE] == SPECTATOR_NOT )
    {
        CheckLocalSounds( ps, ops );
    }
    
    // run events
    CheckPlayerstateEvents( ps, ops );
    
    // smooth the ducking viewheight change
    if( ps->viewheight != ops->viewheight )
    {
        cg.duckChange = ps->viewheight - ops->viewheight;
        cg.duckTime = cg.time;
    }
    
    // changed team
    if( ps->stats[ STAT_TEAM ] != ops->stats[ STAT_TEAM ] )
    {
        cg.lastHealthCross = 0;
        cg.chargeMeterAlpha = 0.f;
    }
}

