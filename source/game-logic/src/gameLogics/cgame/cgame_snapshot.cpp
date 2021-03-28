////////////////////////////////////////////////////////////////////////////////////////
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
// File name:   cgame_trails.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description: things that happen on snapshot transition,
//              not necessarily every single rendered frame
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <cgame/cgame_precompiled.hpp>

/*
===============
idCGameSnapshot::idCGameSnapshot
===============
*/
idCGameSnapshot::idCGameSnapshot( void )
{
}

/*
===============
idCGameServerCmds::~idCGameServerCmds
===============
*/
idCGameSnapshot::~idCGameSnapshot( void )
{
}

/*
==================
idCGameSnapshot::ResetEntity
==================
*/
void idCGameSnapshot::ResetEntity( centity_t* cent )
{
    // if the previous snapshot this entity was updated in is at least
    // an event window back in time then we can reset the previous event
    if( cent->snapShotTime < cg.time - EVENT_VALID_MSEC )
    {
        cent->previousEvent = 0;
    }
    
    cent->trailTime = cg.snap->serverTime;
    
    VectorCopy( cent->currentState.origin, cent->lerpOrigin );
    VectorCopy( cent->currentState.angles, cent->lerpAngles );
    
    if( cent->currentState.eType == ET_PLAYER )
    {
        idCGamePlayers::ResetPlayerEntity( cent );
    }
}

/*
===============
idCGameSnapshot::TransitionEntity

cent->nextState is moved to cent->currentState and events are fired
===============
*/
void idCGameSnapshot::TransitionEntity( centity_t* cent )
{
    cent->currentState = cent->nextState;
    cent->currentValid = true;
    
    // reset if the entity wasn't in the last frame or was teleported
    if( !cent->interpolate )
    {
        ResetEntity( cent );
    }
    
    // clear the next state.  if will be set by the next CG_SetNextSnap
    cent->interpolate = false;
    
    // check for events
    idCGameEvent::CheckEvents( cent );
}


/*
==================
idCGameSnapshot::SetInitialSnapshot

This will only happen on the very first snapshot, or
on tourney restarts.  All other times will use
CG_TransitionSnapshot instead.

FIXME: Also called by map_restart?
==================
*/
void idCGameSnapshot::SetInitialSnapshot( snapshot_t* snap )
{
    sint i;
    centity_t* cent;
    entityState_t* state;
    
    cg.snap = snap;
    
    bggame->PlayerStateToEntityState( &snap->ps, &cg_entities[ snap->ps.clientNum ].currentState, false );
    
    // sort out solid entities
    idCGamePredict::BuildSolidList( );
    
    idCGameServerCmds::ExecuteNewServerCommands( snap->serverCommandSequence );
    
    // set our local weapon selection pointer to
    // what the server has indicated the current weapon is
    idCGamePlayerState::Respawn( );
    
    for( i = 0; i < cg.snap->numEntities; i++ )
    {
        state = &cg.snap->entities[ i ];
        cent = &cg_entities[ state->number ];
        
        ::memcpy( &cent->currentState, state, sizeof( entityState_t ) );
        
        //cent->currentState = *state;
        cent->interpolate = false;
        cent->currentValid = true;
        
        ResetEntity( cent );
        
        // check for events
        idCGameEvent::CheckEvents( cent );
    }
}


/*
===================
idCGameSnapshot::TransitionSnapshot

The transition point from snap to nextSnap has passed
===================
*/
void idCGameSnapshot::TransitionSnapshot( void )
{
    centity_t* cent;
    snapshot_t* oldFrame;
    sint i;
    
    if( !cg.snap )
    {
        Error( "idCGameSnapshot::TransitionSnapshot: nullptr cg.snap" );
    }
    
    if( !cg.nextSnap )
    {
        Error( "idCGameSnapshot::TransitionSnapshot: nullptr cg.nextSnap" );
    }
    
    // execute any server string commands before transitioning entities
    idCGameServerCmds::ExecuteNewServerCommands( cg.nextSnap->serverCommandSequence );
    
    // if we had a map_restart, set everything with initial
    if( cg.mapRestart )
    {
    }
    
    // clear the currentValid flag for all entities in the existing snapshot
    for( i = 0; i < cg.snap->numEntities; i++ )
    {
        cent = &cg_entities[ cg.snap->entities[ i ].number ];
        cent->currentValid = false;
    }
    
    // move nextSnap to snap and do the transitions
    oldFrame = cg.snap;
    cg.snap = cg.nextSnap;
    
    bggame->PlayerStateToEntityState( &cg.snap->ps, &cg_entities[ cg.snap->ps.clientNum ].currentState, false );
    cg_entities[ cg.snap->ps.clientNum ].interpolate = false;
    
    for( i = 0; i < cg.snap->numEntities; i++ )
    {
        cent = &cg_entities[ cg.snap->entities[ i ].number ];
        TransitionEntity( cent );
        
        // remember time of snapshot this entity was last updated in
        cent->snapShotTime = cg.snap->serverTime;
    }
    
    cg.nextSnap = nullptr;
    
    // check for playerstate transition events
    if( oldFrame )
    {
        playerState_t* ops, *ps;
    
        ops = &oldFrame->ps;
        ps = &cg.snap->ps;
        // teleporting checks are irrespective of prediction
        if( ( ps->eFlags ^ ops->eFlags ) & EF_TELEPORT_BIT )
        {
            cg.thisFrameTeleport = true; // will be cleared by prediction code
        }
        
        // if we are not doing client side movement prediction for any
        // reason, then the client events and view changes will be issued now
        if( cg.demoPlayback || ( cg.snap->ps.pm_flags & PMF_FOLLOW ) || cg_nopredict.integer || cg_synchronousClients.integer )
        {
            idCGamePlayerState::TransitionPlayerState( ps, ops );
        }
    }
}


/*
===================
idCGameSnapshot::SetNextSnap

A new snapshot has just been read in from the client system.
===================
*/
void idCGameSnapshot::SetNextSnap( snapshot_t* snap )
{
    sint num;
    entityState_t* es;
    centity_t* cent;
    
    cg.nextSnap = snap;
    
    bggame->PlayerStateToEntityState( &snap->ps, &cg_entities[ snap->ps.clientNum ].nextState, false );
    cg_entities[ cg.snap->ps.clientNum ].interpolate = true;
    
    // check for extrapolation errors
    for( num = 0 ; num < snap->numEntities ; num++ )
    {
        es = &snap->entities[ num ];
        cent = &cg_entities[ es->number ];
        
        ::memcpy( &cent->nextState, es, sizeof( entityState_t ) );
        //cent->nextState = *es;
        
        // if this frame is a teleport, or the entity wasn't in the
        // previous frame, don't interpolate
        if( !cent->currentValid || ( ( cent->currentState.eFlags ^ es->eFlags ) & EF_TELEPORT_BIT ) )
        {
            cent->interpolate = false;
        }
        else
        {
            cent->interpolate = true;
        }
    }
    
    // if the next frame is a teleport for the playerstate, we
    // can't interpolate during demos
    if( cg.snap && ( ( snap->ps.eFlags ^ cg.snap->ps.eFlags ) & EF_TELEPORT_BIT ) )
    {
        cg.nextFrameTeleport = true;
    }
    else
    {
        cg.nextFrameTeleport = false;
    }
    
    // if changing follow mode, don't interpolate
    if( cg.nextSnap->ps.clientNum != cg.snap->ps.clientNum )
    {
        cg.nextFrameTeleport = true;
    }
    
    // if changing server restarts, don't interpolate
    if( ( cg.nextSnap->snapFlags ^ cg.snap->snapFlags ) & SNAPFLAG_SERVERCOUNT )
    {
        cg.nextFrameTeleport = true;
    }
    
    // sort out solid entities
    idCGamePredict::BuildSolidList( );
}


/*
========================
idCGameSnapshot::ReadNextSnapshot

This is the only place new snapshots are requested
This may increment cgs.processedSnapshotNum multiple
times if the client system fails to return a
valid snapshot.
========================
*/
snapshot_t* idCGameSnapshot::ReadNextSnapshot( void )
{
    bool r;
    snapshot_t*  dest;
    
    if( cg.latestSnapshotNum > cgs.processedSnapshotNum + 1000 )
    {
        Printf( "WARNING: idCGameSnapshot::ReadNextSnapshot: way out of range, %i > %i", cg.latestSnapshotNum, cgs.processedSnapshotNum );
    }
    
    while( cgs.processedSnapshotNum < cg.latestSnapshotNum )
    {
        // decide which of the two slots to load it into
        if( cg.snap == &cg.activeSnapshots[0] )
        {
            dest = &cg.activeSnapshots[1];
        }
        else
        {
            dest = &cg.activeSnapshots[0];
        }
        
        // try to read the snapshot from the client system
        cgs.processedSnapshotNum++;
        r = trap_GetSnapshot( cgs.processedSnapshotNum, dest );
        
        // FIXME: why would trap_GetSnapshot return a snapshot with the same server time
        if( cg.nextSnap && r && dest->serverTime == cg.nextSnap->serverTime )
        {
            //continue;
        }
        
        // if it succeeded, return
        if( r )
        {
            idCGameDraw::AddLagometerSnapshotInfo( dest );
            return dest;
        }
        
        // a GetSnapshot will return failure if the snapshot
        // never arrived, or  is so old that its entities
        // have been shoved off the end of the circular
        // buffer in the client system.
        
        // record as a dropped packet
        idCGameDraw::AddLagometerSnapshotInfo( nullptr );
        
        // If there are additional snapshots, continue trying to
        // read them.
    }
    
    // nothing left to read
    return nullptr;
}

/*
============
idCGameSnapshot::ProcessSnapshots

We are trying to set up a renderable view, so determine
what the simulated time is, and try to get snapshots
both before and after that time if available.

If we don't have a valid cg.snap after exiting this function,
then a 3D game view cannot be rendered.  This should only happen
right after the initial connection.  After cg.snap has been valid
once, it will never turn invalid.

Even if cg.snap is valid, cg.nextSnap may not be, if the snapshot
hasn't arrived yet (it becomes an extrapolating situation instead
of an interpolating one)

============
*/
void idCGameSnapshot::ProcessSnapshots( void )
{
    snapshot_t* snap;
    sint n;
    
    // see what the latest snapshot the client system has is
    trap_GetCurrentSnapshotNumber( &n, &cg.latestSnapshotTime );
    
    if( n != cg.latestSnapshotNum )
    {
        if( n < cg.latestSnapshotNum )
        {
            // this should never happen
            Error( "idCGameSnapshot::ProcessSnapshots: n < cg.latestSnapshotNum" );
        }
        
        cg.latestSnapshotNum = n;
    }
    
    // If we have yet to receive a snapshot, check for it.
    // Once we have gotten the first snapshot, cg.snap will
    // always have valid data for the rest of the game
    while( !cg.snap )
    {
        snap = ReadNextSnapshot( );
        
        if( !snap )
        {
            // we can't continue until we get a snapshot
            return;
        }
        
        // set our weapon selection to what
        // the playerstate is currently using
        if( !( snap->snapFlags & SNAPFLAG_NOT_ACTIVE ) )
            SetInitialSnapshot( snap );
    }
    
    // loop until we either have a valid nextSnap with a serverTime
    // greater than cg.time to interpolate towards, or we run
    // out of available snapshots
    do
    {
        // if we don't have a nextframe, try and read a new one in
        if( !cg.nextSnap )
        {
            snap = ReadNextSnapshot( );
            
            // if we still don't have a nextframe, we will just have to
            // extrapolate
            if( !snap )
            {
                break;
            }
            
            SetNextSnap( snap );
            
            // if time went backwards, we have a level restart
            if( cg.nextSnap->serverTime < cg.snap->serverTime )
            {
                Error( "idCGameSnapshot::ProcessSnapshots: Server time went backwards" );
            }
        }
        
        // if our time is < nextFrame's, we have a nice interpolating state
        if( cg.time >= cg.snap->serverTime && cg.time < cg.nextSnap->serverTime )
        {
            break;
        }
        
        // we have passed the transition from nextFrame to frame
        TransitionSnapshot( );
    }
    while( 1 );
    
    // assert our valid conditions upon exiting
    if( cg.snap == nullptr )
    {
        Error( "idCGameSnapshot::ProcessSnapshots: cg.snap == nullptr" );
    }
    
    if( cg.time < cg.snap->serverTime )
    {
        // this can happen right after a vid_restart
        cg.time = cg.snap->serverTime;
    }
    
    if( cg.nextSnap != nullptr && cg.nextSnap->serverTime <= cg.time )
    {
        Error( "idCGameSnapshot::ProcessSnapshots: cg.nextSnap->serverTime <= cg.time" );
    }
}

