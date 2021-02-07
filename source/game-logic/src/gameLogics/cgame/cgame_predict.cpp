////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 1999 - 2005 Id Software, Inc.
// Copyright(C) 2000 - 2006 Tim Angus
// Copyright(C) 2011 - 2021 Dusan Jocic <dusanjocic@msn.com>
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
// File name:   cgame_predict.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description: this file generates cg.predictedPlayerState by either
//              interpolating between snapshots from the server or locally
//              predicting ahead the client's movement.
//              It also handles local physics interaction, like fragments
//              bouncing off walls
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <cgame/cgame_precompiled.hpp>

static pmove_t cg_pmove;

static sint cg_numSolidEntities;
static centity_t* cg_solidEntities[MAX_GENTITIES];
static sint cg_numTriggerEntities;
static centity_t* cg_triggerEntities[MAX_ENTITIES_IN_SNAPSHOT];

/*
===============
idCGamePredict::idCGamePredict
===============
*/
idCGamePredict::idCGamePredict( void )
{
}

/*
===============
idCGamePredict::~idCGamePredict
===============
*/
idCGamePredict::~idCGamePredict( void )
{
}

/*
====================
idCGamePredict::BuildSolidList

When a new cg.snap has been set, this function builds a sublist
of the entities that are actually solid, to make for more
efficient collision detection
====================
*/
void idCGamePredict::BuildSolidList( void )
{
    sint i;
    centity_t* cent;
    snapshot_t* snap;
    entityState_t* ent;
    
    cg_numSolidEntities = 0;
    cg_numTriggerEntities = 0;
    
    if( cg.nextSnap && !cg.nextFrameTeleport && !cg.thisFrameTeleport )
        snap = cg.nextSnap;
    else
        snap = cg.snap;
    
    for( i = 0; i < snap->numEntities; i++ )
    {
        cent = &cg_entities[snap->entities[i].number];
        ent = &cent->currentState;
        
        if( ent->eType == ET_ITEM || ent->eType == ET_PUSH_TRIGGER || ent->eType == ET_TELEPORT_TRIGGER )
        {
            cg_triggerEntities[cg_numTriggerEntities] = cent;
            cg_numTriggerEntities++;
            continue;
        }
        
        if( cent->nextState.solid && ent->eType != ET_MISSILE )
        {
            cg_solidEntities[cg_numSolidEntities] = cent;
            cg_numSolidEntities++;
            continue;
        }
    }
}

/*
====================
idCGamePredict::ClipMoveToEntities
====================
*/
void idCGamePredict::ClipMoveToEntities( const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, sint skipNumber, sint mask, trace_t* tr, traceType_t collisionType )
{
    sint i, j, x, zd, zu;
    trace_t trace;
    entityState_t* ent;
    clipHandle_t cmodel;
    vec3_t bmins, bmaxs, origin, angles;
    centity_t* cent;
    
    //SUPAR HACK
    //this causes a trace to collide with the local player
    if( skipNumber == MAGIC_TRACE_HACK )
    {
        j = cg_numSolidEntities + 1;
    }
    else
    {
        j = cg_numSolidEntities;
    }
    
    for( i = 0; i < j; i++ )
    {
        if( i < cg_numSolidEntities )
        {
            cent = cg_solidEntities[i];
        }
        else
        {
            cent = &cg.predictedPlayerEntity;
        }
        
        ent = &cent->currentState;
        
        if( ent->number == skipNumber )
        {
            continue;
        }
        
        if( ent->solid == SOLID_BMODEL )
        {
            // special value for bmodel
            cmodel = trap_CM_InlineModel( ent->modelindex );
            VectorCopy( cent->lerpAngles, angles );
            bggame->EvaluateTrajectory( &cent->currentState.pos, cg.physicsTime, origin );
        }
        else
        {
            // encoded bbox
            x = ( ent->solid & 255 );
            zd = ( ( ent->solid >> 8 ) & 255 );
            zu = ( ( ent->solid >> 16 ) & 255 ) - 32;
            
            bmins[ 0 ] = bmins[ 1 ] = -x;
            bmaxs[ 0 ] = bmaxs[ 1 ] = x;
            bmins[ 2 ] = -zd;
            bmaxs[ 2 ] = zu;
            
            if( i == cg_numSolidEntities )
            {
                bggame->ClassBoundingBox( ( class_t )( ( ent->misc >> 8 ) & 0xFF ), bmins, bmaxs, nullptr, nullptr, nullptr );
            }
            
            cmodel = trap_CM_TempBoxModel( bmins, bmaxs );
            VectorCopy( vec3_origin, angles );
            VectorCopy( cent->lerpOrigin, origin );
        }
        
        if( collisionType == TT_CAPSULE )
        {
            trap_CM_TransformedCapsuleTrace( &trace, start, end, mins, maxs, cmodel,  mask, origin, angles );
        }
        else if( collisionType == TT_AABB )
        {
            trap_CM_TransformedBoxTrace( &trace, start, end, mins, maxs, cmodel,  mask, origin, angles );
        }
        else if( collisionType == TT_BISPHERE )
        {
            trap_CM_TransformedBiSphereTrace( &trace, start, end, mins[ 0 ], maxs[ 0 ], cmodel, mask, origin );
        }
        
        if( trace.allsolid || trace.fraction < tr->fraction )
        {
            trace.entityNum = ent->number;
            
            if( tr->lateralFraction < trace.lateralFraction )
            {
                float32 oldLateralFraction = tr->lateralFraction;
                *tr = trace;
                tr->lateralFraction = oldLateralFraction;
            }
            else
            {
                *tr = trace;
            }
        }
        else if( trace.startsolid )
        {
            tr->startsolid = true;
            tr->entityNum = ent->number;
        }
        
        if( tr->allsolid )
        {
            return;
        }
    }
}

/*
================
idCGamePredict::Trace
================
*/
void idCGamePredict::Trace( trace_t* result, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, sint skipNumber, sint mask )
{
    trace_t t;
    
    trap_CM_BoxTrace( &t, start, end, mins, maxs, 0, mask );
    t.entityNum = t.fraction != 1.0 ? ENTITYNUM_WORLD : ENTITYNUM_NONE;
    
    // check all other solid models
    ClipMoveToEntities( start, mins, maxs, end, skipNumber, mask, &t, TT_AABB );
    
    *result = t;
}

/*
================
idCGamePredict::CapTrace
================
*/
void idCGamePredict::CapTrace( trace_t* result, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, sint skipNumber, sint mask )
{
    trace_t t;
    
    trap_CM_CapsuleTrace( &t, start, end, mins, maxs, 0, mask );
    t.entityNum = t.fraction != 1.0 ? ENTITYNUM_WORLD : ENTITYNUM_NONE;
    
    // check all other solid models
    ClipMoveToEntities( start, mins, maxs, end, skipNumber, mask, &t, TT_CAPSULE );
    
    *result = t;
}

/*
================
idCGamePredict::BiSphereTrace
================
*/
void idCGamePredict::BiSphereTrace( trace_t* result, const vec3_t start, const vec3_t end, const float32 startRadius, const float32 endRadius, sint skipNumber, sint mask )
{
    trace_t t;
    vec3_t mins, maxs;
    
    mins[ 0 ] = startRadius;
    maxs[ 0 ] = endRadius;
    
    trap_CM_BiSphereTrace( &t, start, end, startRadius, endRadius, 0, mask );
    t.entityNum = t.fraction != 1.0 ? ENTITYNUM_WORLD : ENTITYNUM_NONE;
    
    // check all other solid models
    ClipMoveToEntities( start, mins, maxs, end, skipNumber, mask, &t, TT_BISPHERE );
    
    *result = t;
}

/*
================
idCGamePredict::PointContents
================
*/
sint idCGamePredict::PointContents( const vec3_t point, sint passEntityNum )
{
    sint i, contents;
    entityState_t* ent;
    centity_t* cent;
    clipHandle_t  cmodel;
    
    contents = trap_CM_PointContents( point, 0 );
    
    for( i = 0; i < cg_numSolidEntities; i++ )
    {
        cent = cg_solidEntities[ i ];
        
        ent = &cent->currentState;
        
        if( ent->number == passEntityNum )
        {
            continue;
        }
        
        if( ent->solid != SOLID_BMODEL )  // special value for bmodel
        {
            continue;
        }
        
        cmodel = trap_CM_InlineModel( ent->modelindex );
        
        if( !cmodel )
        {
            continue;
        }
        
        contents |= trap_CM_TransformedPointContents( point, cmodel, cent->lerpOrigin, cent->lerpAngles );
    }
    
    return contents;
}

/*
========================
idCGamePredict::InterpolatePlayerState

Generates cg.predictedPlayerState by interpolating between
cg.snap->player_state and cg.nextFrame->player_state
========================
*/
void idCGamePredict::InterpolatePlayerState( bool grabAngles )
{
    float32 f;
    sint i;
    playerState_t* out;
    snapshot_t* prev, *next;
    
    out = &cg.predictedPlayerState;
    prev = cg.snap;
    next = cg.nextSnap;
    
    *out = cg.snap->ps;
    
    // if we are still allowing local input, short circuit the view angles
    if( grabAngles )
    {
        usercmd_t cmd;
        sint cmdNum;
        
        cmdNum = trap_GetCurrentCmdNumber( );
        trap_GetUserCmd( cmdNum, &cmd );
        
        bggame->UpdateViewAngles( out, &cmd );
    }
    
    // if the next frame is a teleport, we can't lerp to it
    if( cg.nextFrameTeleport )
    {
        return;
    }
    
    if( !next || next->serverTime <= prev->serverTime )
    {
        return;
    }
    
    f = ( float32 )( cg.time - prev->serverTime ) / ( next->serverTime - prev->serverTime );
    
    i = next->ps.bobCycle;
    if( i < prev->ps.bobCycle )
    {
        i += 256;   // handle wraparound
    }
    
    out->bobCycle = prev->ps.bobCycle + f * ( i - prev->ps.bobCycle );
    
    for( i = 0; i < 3; i++ )
    {
        out->origin[ i ] = prev->ps.origin[ i ] + f * ( next->ps.origin[ i ] - prev->ps.origin[ i ] );
        
        if( !grabAngles )
        {
            out->viewangles[i] = LerpAngle( prev->ps.viewangles[i], next->ps.viewangles[i], f );
        }
        
        out->velocity[ i ] = prev->ps.velocity[ i ] + f * ( next->ps.velocity[ i ] - prev->ps.velocity[ i ] );
    }
}

/*
=========================
idCGamePredict::TouchTriggerPrediction

Predict push triggers and items
=========================
*/
void idCGamePredict::TouchTriggerPrediction( void )
{
    sint i;
    trace_t trace;
    entityState_t* ent;
    clipHandle_t cmodel;
    centity_t* cent;
    bool spectator;
    
    // dead clients don't activate triggers
    if( cg.predictedPlayerState.stats[STAT_HEALTH] <= 0 )
    {
        return;
    }
    
    spectator = ( cg.predictedPlayerState.pm_type == PM_SPECTATOR );
    
    if( cg.predictedPlayerState.pm_type != PM_NORMAL && !spectator )
    {
        return;
    }
    
    for( i = 0; i < cg_numTriggerEntities; i++ )
    {
        cent = cg_triggerEntities[ i ];
        ent = &cent->currentState;
        
        if( ent->solid != SOLID_BMODEL )
        {
            continue;
        }
        
        cmodel = trap_CM_InlineModel( ent->modelindex );
        if( !cmodel )
        {
            continue;
        }
        
        trap_CM_BoxTrace( &trace, cg.predictedPlayerState.origin, cg.predictedPlayerState.origin, cg_pmove.mins, cg_pmove.maxs, cmodel, -1 );
        
        if( !trace.startsolid )
        {
            continue;
        }
        
        if( ent->eType == ET_TELEPORT_TRIGGER )
        {
            cg.hyperspace = true;
        }
    }
}

/*
=========================
idCGamePredict::IsUnacceptableError
=========================
*/
sint idCGamePredict::IsUnacceptableError( playerState_t* ps, playerState_t* pps )
{
    vec3_t delta;
    sint i;
    
    if( pps->pm_type != ps->pm_type || pps->pm_flags != ps->pm_flags || pps->pm_time != ps->pm_time )
    {
        return 1;
    }
    
    VectorSubtract( pps->origin, ps->origin, delta );
    if( VectorLengthSquared( delta ) > 0.1f * 0.1f )
    {
        if( cg_showmiss.integer )
        {
            Printf( "origin delta: %.2f  ", VectorLength( delta ) );
        }
        return 2;
    }
    
    VectorSubtract( pps->velocity, ps->velocity, delta );
    if( VectorLengthSquared( delta ) > 0.1f * 0.1f )
    {
        if( cg_showmiss.integer )
        {
            Printf( "velocity delta: %.2f  ", VectorLength( delta ) );
        }
        return 3;
    }
    
    if( pps->weaponTime != ps->weaponTime ||
            pps->gravity != ps->gravity ||
            pps->speed != ps->speed ||
            pps->delta_angles[ 0 ] != ps->delta_angles[ 0 ] ||
            pps->delta_angles[ 1 ] != ps->delta_angles[ 1 ] ||
            pps->delta_angles[ 2 ] != ps->delta_angles[ 2 ] ||
            pps->groundEntityNum != ps->groundEntityNum )
    {
        return 4;
    }
    
    if( pps->legsTimer != ps->legsTimer ||
            pps->legsAnim != ps->legsAnim ||
            pps->torsoTimer != ps->torsoTimer ||
            pps->torsoAnim != ps->torsoAnim ||
            pps->movementDir != ps->movementDir )
    {
        return 5;
    }
    
    VectorSubtract( pps->grapplePoint, ps->grapplePoint, delta );
    if( VectorLengthSquared( delta ) > 0.1f * 0.1f )
    {
        return 6;
    }
    
    if( pps->eFlags != ps->eFlags )
    {
        return 7;
    }
    
    if( pps->eventSequence != ps->eventSequence )
    {
        return 8;
    }
    
    for( i = 0; i < MAX_EVENTS; i++ )
    {
        if( pps->events[ i ] != ps->events[ i ] || pps->eventParms[ i ] != ps->eventParms[ i ] )
        {
            return 9;
        }
    }
    
    if( pps->externalEvent != ps->externalEvent || pps->externalEventParm != ps->externalEventParm || pps->externalEventTime != ps->externalEventTime )
    {
        return 10;
    }
    
    if( pps->clientNum != ps->clientNum || pps->weapon != ps->weapon || pps->weaponstate != ps->weaponstate )
    {
        return 11;
    }
    
    if( fabs( AngleDelta( ps->viewangles[ 0 ], pps->viewangles[ 0 ] ) ) > 1.0f ||
            fabs( AngleDelta( ps->viewangles[ 1 ], pps->viewangles[ 1 ] ) ) > 1.0f ||
            fabs( AngleDelta( ps->viewangles[ 2 ], pps->viewangles[ 2 ] ) ) > 1.0f )
    {
        return 12;
    }
    
    if( pps->viewheight != ps->viewheight )
    {
        return 13;
    }
    
    if( pps->damageEvent != ps->damageEvent || pps->damageYaw != ps->damageYaw || pps->damagePitch != ps->damagePitch || pps->damageCount != ps->damageCount )
    {
        return 14;
    }
    
    for( i = 0; i < MAX_STATS; i++ )
    {
        if( pps->stats[i] != ps->stats[i] )
        {
            return 15;
        }
    }
    
    for( i = 0; i < MAX_PERSISTANT; i++ )
    {
        if( pps->persistant[i] != ps->persistant[i] )
        {
            return 16;
        }
    }
    
    if( pps->generic1 != ps->generic1 || pps->loopSound != ps->loopSound )
    {
        return 19;
    }
    
    return 0;
}


/*
=================
idCGamePredict::PredictPlayerState

Generates cg.predictedPlayerState for the current cg.time
cg.predictedPlayerState is guaranteed to be valid after exiting.

For demo playback, this will be an interpolation between two valid
playerState_t.

For normal gameplay, it will be the result of predicted usercmd_t on
top of the most recent playerState_t received from the server.

Each new snapshot will usually have one or more new usercmd over the last,
but we simulate all unacknowledged commands each time, not just the new ones.
This means that on an internet connection, quite a few pmoves may be issued
each frame.

OPTIMIZE: don't re-simulate unless the newly arrived snapshot playerState_t
differs from the predicted one.  Would require saving all intermediate
playerState_t during prediction.

We detect prediction errors and allow them to be decayed off over several frames
to ease the jerk.
=================
*/
void idCGamePredict::PredictPlayerState( void )
{
    sint cmdNum, current, i;
    playerState_t oldPlayerState;
    bool  moved;
    usercmd_t oldestCmd, latestCmd;
    sint stateIndex = 0, predictCmd = 0;
    
    cg.hyperspace = false; // will be set if touching a trigger_teleport
    
    // if this is the first frame we must guarantee
    // predictedPlayerState is valid even if there is some
    // other error condition
    if( !cg.validPPS )
    {
        cg.validPPS = true;
        cg.predictedPlayerState = cg.snap->ps;
    }
    
    
    // demo playback just copies the moves
    if( cg.demoPlayback || ( cg.snap->ps.pm_flags & PMF_FOLLOW ) )
    {
        InterpolatePlayerState( false );
        return;
    }
    
    // non-predicting local movement will grab the latest angles
    if( cg_nopredict.integer || cg_synchronousClients.integer )
    {
        InterpolatePlayerState( true );
        return;
    }
    
    // prepare for pmove
    cg_pmove.ps = &cg.predictedPlayerState;
    cg_pmove.pmext = &cg.pmext;
    cg_pmove.trace = Trace;
    cg_pmove.pointcontents = PointContents;
    cg_pmove.debugLevel = cg_debugMove.integer;
    
    if( cg_pmove.ps->pm_type == PM_DEAD )
    {
        cg_pmove.tracemask = MASK_DEADSOLID;
    }
    else
    {
        cg_pmove.tracemask = MASK_PLAYERSOLID;
    }
    
    if( cg.snap->ps.persistant[PERS_SPECSTATE] != SPECTATOR_NOT )
    {
        cg_pmove.tracemask = MASK_DEADSOLID; // spectators can fly through bodies
    }
    
    cg_pmove.noFootsteps = 0;
    
    // save the state before the pmove so we can detect transitions
    oldPlayerState = cg.predictedPlayerState;
    
    current = trap_GetCurrentCmdNumber( );
    
    // if we don't have the commands right after the snapshot, we
    // can't accurately predict a current position, so just freeze at
    // the last good position we had
    cmdNum = current - CMD_BACKUP + 1;
    trap_GetUserCmd( cmdNum, &oldestCmd );
    
    if( oldestCmd.serverTime > cg.snap->ps.commandTime && oldestCmd.serverTime < cg.time )
    {
        // special check for map_restart
        if( cg_showmiss.integer )
        {
            Printf( "exceeded PACKET_BACKUP on commands\n" );
        }
        
        return;
    }
    
    // get the latest command so we can know which commands are from previous map_restarts
    trap_GetUserCmd( current, &latestCmd );
    
    // get the most recent information we have, even if
    // the server time is beyond our current cg.time,
    // because predicted player positions are going to
    // be ahead of everything else anyway
    if( cg.nextSnap && !cg.nextFrameTeleport && !cg.thisFrameTeleport )
    {
        cg.predictedPlayerState = cg.nextSnap->ps;
        cg.physicsTime = cg.nextSnap->serverTime;
    }
    else
    {
        cg.predictedPlayerState = cg.snap->ps;
        cg.physicsTime = cg.snap->serverTime;
    }
    
    if( pmove_msec.integer < 8 )
    {
        trap_Cvar_Set( "pmove_msec", "8" );
        trap_Cvar_Update( &pmove_msec );
    }
    else if( pmove_msec.integer > 33 )
    {
        trap_Cvar_Set( "pmove_msec", "33" );
        trap_Cvar_Update( &pmove_msec );
    }
    
    cg_pmove.pmove_fixed = pmove_fixed.integer;// | cg_pmove_fixed.integer;
    cg_pmove.pmove_msec = pmove_msec.integer;
    
    // Like the comments described above, a player's state is entirely
    // re-predicted from the last valid snapshot every client frame, which
    // can be really, really, really slow.  Every old command has to be
    // run again.  For every client frame that is *not* directly after a
    // snapshot, this is unnecessary, since we have no new information.
    // For those, we'll play back the predictions from the last frame and
    // predict only the newest commands.  Essentially, we'll be doing
    // an incremental predict instead of a full predict.
    //
    // If we have a new snapshot, we can compare its player state's command
    // time to the command times in the queue to find a match.  If we find
    // a matching state, and the predicted version has not deviated, we can
    // use the predicted state as a base - and also do an incremental predict.
    //
    // With this method, we get incremental predicts on every client frame
    // except a frame following a new snapshot in which there was a prediction
    // error.  This yeilds anywhere from a 15% to 40% performance increase,
    // depending on how much of a bottleneck the CPU is.
    if( cg_optimizePrediction.integer )
    {
        if( cg.nextFrameTeleport || cg.thisFrameTeleport )
        {
            // do a full predict
            cg.lastPredictedCommand = 0;
            cg.stateTail = cg.stateHead;
            predictCmd = current - CMD_BACKUP + 1;
        }
        // cg.physicsTime is the current snapshot's serverTime if it's the same
        // as the last one
        else if( cg.physicsTime == cg.lastServerTime )
        {
            // we have no new information, so do an incremental predict
            predictCmd = cg.lastPredictedCommand + 1;
        }
        else
        {
            // we have a new snapshot
            sint i;
            sint errorcode;
            bool error = true;
            
            // loop through the saved states queue
            for( i = cg.stateHead; i != cg.stateTail; i = ( i + 1 ) % NUM_SAVED_STATES )
            {
                // if we find a predicted state whose commandTime matches the snapshot
                // player state's commandTime
                if( cg.savedPmoveStates[ i ].commandTime != cg.predictedPlayerState.commandTime )
                {
                    continue;
                }
                // make sure the state differences are acceptable
                errorcode = IsUnacceptableError( &cg.predictedPlayerState, &cg.savedPmoveStates[ i ] );
                
                if( errorcode )
                {
                    if( cg_showmiss.integer )
                    {
                        Printf( "errorcode %d at %d\n", errorcode, cg.time );
                    }
                    break;
                }
                
                // this one is almost exact, so we'll copy it in as the starting point
                *cg_pmove.ps = cg.savedPmoveStates[ i ];
                // advance the head
                cg.stateHead = ( i + 1 ) % NUM_SAVED_STATES;
                
                // set the next command to predict
                predictCmd = cg.lastPredictedCommand + 1;
                
                // a saved state matched, so flag it
                error = false;
                break;
            }
            
            // if no saved states matched
            if( error )
            {
                // do a full predict
                cg.lastPredictedCommand = 0;
                cg.stateTail = cg.stateHead;
                predictCmd = current - CMD_BACKUP + 1;
            }
        }
        
        // keep track of the server time of the last snapshot so we
        // know when we're starting from a new one in future calls
        cg.lastServerTime = cg.physicsTime;
        stateIndex = cg.stateHead;
    }
    
    // run cmds
    moved = false;
    
    for( cmdNum = current - CMD_BACKUP + 1; cmdNum <= current; cmdNum++ )
    {
        // get the command
        trap_GetUserCmd( cmdNum, &cg_pmove.cmd );
        
        if( cg_pmove.pmove_fixed )
        {
            bggame->UpdateViewAngles( cg_pmove.ps, &cg_pmove.cmd );
        }
        
        // don't do anything if the time is before the snapshot player time
        if( cg_pmove.cmd.serverTime <= cg.predictedPlayerState.commandTime )
        {
            continue;
        }
        
        // don't do anything if the command was from a previous map_restart
        if( cg_pmove.cmd.serverTime > latestCmd.serverTime )
        {
            continue;
        }
        
        // check for a prediction error from last frame
        // on a lan, this will often be the exact value
        // from the snapshot, but on a wan we will have
        // to predict several commands to get to the point
        // we want to compare
        if( cg.predictedPlayerState.commandTime == oldPlayerState.commandTime )
        {
            vec3_t delta;
            float32 len;
            
            if( cg.thisFrameTeleport )
            {
                // a teleport will not cause an error decay
                VectorClear( cg.predictedError );
                
                if( cg_showmiss.integer )
                {
                    Printf( "PredictionTeleport\n" );
                }
                
                cg.thisFrameTeleport = false;
            }
            else
            {
                vec3_t adjusted;
                idCGameEnts::AdjustPositionForMover( cg.predictedPlayerState.origin, cg.predictedPlayerState.groundEntityNum, cg.physicsTime, cg.oldTime, adjusted );
                
                if( cg_showmiss.integer )
                {
                    if( !VectorCompare( oldPlayerState.origin, adjusted ) )
                    {
                        Printf( "prediction error\n" );
                    }
                }
                
                VectorSubtract( oldPlayerState.origin, adjusted, delta );
                len = VectorLength( delta );
                
                if( len > 0.1 )
                {
                    if( cg_showmiss.integer )
                    {
                        Printf( "Prediction miss: %f\n", len );
                    }
                    
                    if( cg_errorDecay.integer )
                    {
                        sint t;
                        float32 f;
                        
                        t = cg.time - cg.predictedErrorTime;
                        f = ( cg_errorDecay.value - t ) / cg_errorDecay.value;
                        
                        if( f < 0 )
                        {
                            f = 0;
                        }
                        
                        if( f > 0 && cg_showmiss.integer )
                        {
                            Printf( "Double prediction decay: %f\n", f );
                        }
                        
                        VectorScale( cg.predictedError, f, cg.predictedError );
                    }
                    else
                    {
                        VectorClear( cg.predictedError );
                    }
                    
                    VectorAdd( delta, cg.predictedError, cg.predictedError );
                    cg.predictedErrorTime = cg.oldTime;
                }
            }
        }
        
        // don't predict gauntlet firing, which is only supposed to happen
        // when it actually inflicts damage
        for( i = WP_NONE + 1; i < WP_NUM_WEAPONS; i++ )
        {
            cg_pmove.autoWeaponHit[i] = false;
        }
        
        if( cg_pmove.pmove_fixed )
        {
            cg_pmove.cmd.serverTime = ( ( cg_pmove.cmd.serverTime + pmove_msec.integer - 1 ) / pmove_msec.integer ) * pmove_msec.integer;
        }
        
        if( cg.serverRespawning )
        {
            cg_pmove.ps->pm_type = PM_FREEZE;
        }
        
        if( !cg_optimizePrediction.integer )
        {
            bggame->Pmove( &cg_pmove );
        }
        else if( cg_optimizePrediction.integer && ( cmdNum >= predictCmd || ( stateIndex + 1 ) % NUM_SAVED_STATES == cg.stateHead ) )
        {
            bggame->Pmove( &cg_pmove );
            // record the last predicted command
            cg.lastPredictedCommand = cmdNum;
            
            // if we haven't run out of space in the saved states queue
            if( ( stateIndex + 1 ) % NUM_SAVED_STATES != cg.stateHead )
            {
                // save the state for the false case ( of cmdNum >= predictCmd )
                // in later calls to this function
                cg.savedPmoveStates[ stateIndex ] = *cg_pmove.ps;
                stateIndex = ( stateIndex + 1 ) % NUM_SAVED_STATES;
                cg.stateTail = stateIndex;
            }
        }
        else
        {
            *cg_pmove.ps = cg.savedPmoveStates[ stateIndex ];
            stateIndex = ( stateIndex + 1 ) % NUM_SAVED_STATES;
        }
        
        moved = true;
        
        // add push trigger movement effects
        TouchTriggerPrediction( );
        
        // check for predictable events that changed from previous predictions
        //CG_CheckChangedPredictableEvents(&cg.predictedPlayerState);
    }
    
    // adjust for the movement of the groundentity
    idCGameEnts::AdjustPositionForMover( cg.predictedPlayerState.origin, cg.predictedPlayerState.groundEntityNum, cg.physicsTime, cg.time, cg.predictedPlayerState.origin );
    
    
    // fire events and other transition triggered things
    idCGamePlayerState::TransitionPlayerState( &cg.predictedPlayerState, &oldPlayerState );
}
