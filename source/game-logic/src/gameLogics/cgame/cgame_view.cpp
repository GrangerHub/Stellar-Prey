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
// File name:   cgame_weapons.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description: setup all the parameters (position, angle, etc) for a 3D rendering
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <cgame/cgame_precompiled.hpp>

/*
=============================================================================
MODEL TESTING

The viewthing and gun positioning tools from Q2 have been integrated and
enhanced into a single model testing facility.

Model viewing can begin with either "testmodel <modelname>" or "testgun <modelname>".

The names must be the full pathname after the basedir, like
"models/weapons/v_launch/tris.md3" or "players/male/tris.md3"

Testmodel will create a fake entity 100 units in front of the current view
position, directly facing the viewer.  It will remain immobile, so you can
move around it to view it from different angles.

Testgun will cause the model to follow the player around and supress the real
view weapon model.  The default frame 0 of most guns is completely off screen,
so you will probably have to cycle a couple frames to see it.

"nextframe", "prevframe", "nextskin", and "prevskin" commands will change the
frame or skin of the testmodel.  These are bound to F5, F6, F7, and F8 in
q3default.cfg.

If a gun is being tested, the "gun_x", "gun_y", and "gun_z" variables will let
you adjust the positioning.

Note that none of the model testing features update while the game is paused, so
it may be convenient to test with deathmatch set to 1 so that bringing down the
console doesn't pause the game.
=============================================================================
*/

/*
===============
idCGameView::idCGameView
===============
*/
idCGameView::idCGameView( void )
{
}

/*
===============
idCGameView::~idCGameView
===============
*/
idCGameView::~idCGameView( void )
{
}

/*
=================
idCGameView::TestModel_f

Creates an entity in front of the current position, which
can then be moved around
=================
*/
void idCGameView::TestModel_f( void )
{
    vec3_t angles;

    cg.testGun = false;

    ::memset( &cg.testModelEntity, 0, sizeof( cg.testModelEntity ) );

    if( trap_Argc() < 2 )
    {
        return;
    }
    
    Q_strncpyz( cg.testModelName, idCGameMain::Argv( 1 ), MAX_QPATH );
    cg.testModelEntity.hModel = trap_R_RegisterModel( cg.testModelName );
    
    if( trap_Argc( ) == 3 )
    {
        cg.testModelEntity.backlerp = atof( idCGameMain::Argv( 2 ) );
        cg.testModelEntity.frame = 1;
        cg.testModelEntity.oldframe = 0;
    }
    
    if( !cg.testModelEntity.hModel )
    {
        Printf( "Can't register model\n" );
        return;
    }
    
    VectorMA( cg.refdef.vieworg, 100, cg.refdef.viewaxis[ 0 ], cg.testModelEntity.origin );
    
    angles[ PITCH ] = 0;
    angles[ YAW ] = 180 + cg.refdefViewAngles[ 1 ];
    angles[ ROLL ] = 0;
    
    AnglesToAxis( angles, cg.testModelEntity.axis );
}

/*
=================
idCGameView::TestGun_f

Replaces the current view weapon with the given model
=================
*/
void idCGameView::TestGun_f( void )
{
    TestModel_f( );
    
    cg.testGun = true;
    cg.testModelEntity.renderfx = RF_MINLIGHT | RF_DEPTHHACK | RF_FIRST_PERSON;
}

/*
=================
idCGameView::TestModelNextFrame_f
=================
*/
void idCGameView::TestModelNextFrame_f( void )
{
    cg.testModelEntity.frame++;
    Printf( "frame %i\n", cg.testModelEntity.frame );
}

/*
=================
idCGameView::TestModelPrevFrame_f
=================
*/
void idCGameView::TestModelPrevFrame_f( void )
{
    cg.testModelEntity.frame--;
    
    if( cg.testModelEntity.frame < 0 )
    {
        cg.testModelEntity.frame = 0;
    }
    
    Printf( "frame %i\n", cg.testModelEntity.frame );
}

/*
=================
idCGameView::TestModelNextSkin_f
=================
*/
void idCGameView::TestModelNextSkin_f( void )
{
    cg.testModelEntity.skinNum++;
    Printf( "skin %i\n", cg.testModelEntity.skinNum );
}

/*
=================
idCGameView::TestModelPrevSkin_f
=================
*/
void idCGameView::TestModelPrevSkin_f( void )
{
    cg.testModelEntity.skinNum--;
    
    if( cg.testModelEntity.skinNum < 0 )
    {
        cg.testModelEntity.skinNum = 0;
    }
    
    Printf( "skin %i\n", cg.testModelEntity.skinNum );
}

/*
=================
idCGameView::AddTestModel
=================
*/
void idCGameView::AddTestModel( void )
{
    sint i;
    
    // re-register the model, because the level may have changed
    cg.testModelEntity.hModel = trap_R_RegisterModel( cg.testModelName );
    cg.testModelBarrelEntity.hModel = trap_R_RegisterModel( cg.testModelBarrelName );
    
    if( !cg.testModelEntity.hModel )
    {
        Printf( "Can't register model\n" );
        return;
    }
    
    // if testing a gun, set the origin reletive to the view origin
    if( cg.testGun )
    {
        VectorCopy( cg.refdef.vieworg, cg.testModelEntity.origin );
        VectorCopy( cg.refdef.viewaxis[ 0 ], cg.testModelEntity.axis[ 0 ] );
        VectorCopy( cg.refdef.viewaxis[ 1 ], cg.testModelEntity.axis[ 1 ] );
        VectorCopy( cg.refdef.viewaxis[ 2 ], cg.testModelEntity.axis[ 2 ] );
        
        // allow the position to be adjusted
        for( i = 0; i < 3; i++ )
        {
            cg.testModelEntity.origin[ i ] += cg.refdef.viewaxis[ 0 ][ i ] * cg_gun_x.value;
            cg.testModelEntity.origin[ i ] += cg.refdef.viewaxis[ 1 ][ i ] * cg_gun_y.value;
            cg.testModelEntity.origin[ i ] += cg.refdef.viewaxis[ 2 ][ i ] * cg_gun_z.value;
        }
    }
    
    trap_R_AddRefEntityToScene( &cg.testModelEntity );
    
    if( cg.testModelBarrelEntity.hModel )
    {
        idCGameEnts::PositionEntityOnTag( &cg.testModelBarrelEntity, &cg.testModelEntity, cg.testModelEntity.hModel, "tag_barrel" );
        
        trap_R_AddRefEntityToScene( &cg.testModelBarrelEntity );
    }
}

/*
=================
idCGameView::CalcVrect

Sets the coordinates of the rendered window
=================
*/
void idCGameView::CalcVrect( void )
{
    sint size;
    
    // the intermission should allways be full screen
    if( cg.snap->ps.pm_type == PM_INTERMISSION )
    {
        size = 100;
    }
    else
    {
        // bound normal viewsize
        if( cg_viewsize.integer < 30 )
        {
            trap_Cvar_Set( "cg_viewsize", "30" );
            size = 30;
        }
        else if( cg_viewsize.integer > 100 )
        {
            trap_Cvar_Set( "cg_viewsize", "100" );
            size = 100;
        }
        else
        {
            size = cg_viewsize.integer;
        }
    }
    
    cg.refdef.width = cgs.glconfig.vidWidth * size / 100;
    cg.refdef.width &= ~1;
    
    cg.refdef.height = cgs.glconfig.vidHeight * size / 100;
    cg.refdef.height &= ~1;
    
    cg.refdef.x = ( cgs.glconfig.vidWidth - cg.refdef.width ) / 2;
    cg.refdef.y = ( cgs.glconfig.vidHeight - cg.refdef.height ) / 2;
}

/*
===============
idCGameView::OffsetThirdPersonView
===============
*/
void idCGameView::OffsetThirdPersonView( void )
{
    sint i;
    vec3_t forward, right, up, view;
    trace_t trace;
    static vec3_t mins = { -8, -8, -8 };
    static vec3_t maxs = { 8, 8, 8 };
    vec3_t focusPoint, surfNormal;
    sint cmdNum;
    usercmd_t cmd, oldCmd;
    float32 range;
    vec3_t mouseInputAngles, rotationAngles, axis[ 3 ], rotaxis[ 3 ];
    float32 deltaPitch;
    static float32 pitch;
    
    // If cg_thirdpersonShoulderViewMode == 2, do shoulder view instead
    // If cg_thirdpersonShoulderViewMode == 1, do shoulder view when chasing
    //   a wallwalker because it's really erratic to watch
    if( ( cg_thirdPersonShoulderViewMode.integer == 2 ) || ( ( cg_thirdPersonShoulderViewMode.integer == 1 ) && ( cg.snap->ps.stats[ STAT_STATE ] & SS_WALLCLIMBING ) && ( cg.snap->ps.stats[ STAT_HEALTH ] > 0 ) ) )
    {
        OffsetShoulderView( );
        return;
    }
    
    bggame->GetClientNormal( &cg.predictedPlayerState, surfNormal );
    // Set the view origin to the class's view height
    VectorMA( cg.refdef.vieworg, cg.predictedPlayerState.viewheight, surfNormal, cg.refdef.vieworg );
    
    // Set the focus point where the camera will look (at the player's vieworg)
    VectorCopy( cg.refdef.vieworg, focusPoint );
    
    // If player is dead, we want the player to be between us and the killer
    // so pretend that the player was looking at the killer, then place cam behind them.
    if( cg.predictedPlayerState.stats[ STAT_HEALTH ] <= 0 )
    {
        sint killerEntNum;
        vec3_t killerPos;
        
        killerEntNum = cg.predictedPlayerState.stats[ STAT_VIEWLOCK ];
        
        // already looking at ourself
        if( killerEntNum != cg.snap->ps.clientNum )
        {
            VectorCopy( cg_entities[ killerEntNum ].lerpOrigin, killerPos );
            
            VectorSubtract( killerPos, cg.refdef.vieworg, killerPos );
            vectoangles( killerPos, cg.refdefViewAngles );
        }
    }
    
    // get and rangecheck cg_thirdPersonRange
    range = cg_thirdPersonRange.value;
    if( range > 150.0f ) range = 150.0f;
    if( range < 30.0f ) range = 30.0f;
    
    // Calculate the angle of the camera's position around the player.
    // Unless in demo, PLAYING in third person, or in dead-third-person cam, allow the player
    // to control camera position offsets using the mouse position.
    if( cg.demoPlayback || ( ( cg.snap->ps.pm_flags & PMF_FOLLOW ) && ( cg.predictedPlayerState.stats[ STAT_HEALTH ] > 0 ) ) )
    {
        // Collect our input values from the mouse.
        cmdNum = trap_GetCurrentCmdNumber();
        trap_GetUserCmd( cmdNum, &cmd );
        trap_GetUserCmd( cmdNum - 1, &oldCmd );
        
        // Prevent pitch from wrapping and clamp it within a [-75, 90] range.
        // Cgame has no access to ps.delta_angles[] here, so we need to reproduce
        // it ourselves.
        deltaPitch = SHORT2ANGLE( cmd.angles[ PITCH ] - oldCmd.angles[ PITCH ] );
        if( fabs( deltaPitch ) < 200.0f )
        {
            pitch += deltaPitch;
        }
        
        mouseInputAngles[ PITCH ] = pitch;
        mouseInputAngles[ YAW ] = -1.0 * SHORT2ANGLE( cmd.angles[ YAW ] ); // yaw is inverted
        mouseInputAngles[ ROLL ] = 0.0;
        
        for( i = 0; i < 3; i++ )
        {
            mouseInputAngles[i] = AngleNormalize180( mouseInputAngles[i] );
        }
        
        // Set the rotation angles to be the view angles offset by the mouse input
        // Ignore the original pitch though; it's too jerky otherwise
        if( !cg_thirdPersonPitchFollow.integer )
        {
            cg.refdefViewAngles[PITCH] = 0.0;
        }
        
        for( i = 0; i < 3; i++ )
        {
            rotationAngles[ i ] = AngleNormalize180( cg.refdefViewAngles[ i ] ) + mouseInputAngles[ i ];
            AngleNormalize180( rotationAngles[ i ] );
        }
        
        // Don't let pitch go too high/too low or the camera flips around and
        // that's really annoying.
        // However, when we're not on the floor or ceiling (wallwalk) pitch
        // may not be pitch, so just let it go.
        if( surfNormal[ 2 ] > 0.5 || surfNormal[ 2 ] < -0.5 )
        {
            if( rotationAngles[PITCH] > 85 )
            {
                rotationAngles[PITCH] = 85;
            }
            else if( rotationAngles[PITCH] < -85 )
            {
                rotationAngles[PITCH] = -85;
            }
        }
        
        // Perform the rotations specified by rotationAngles.
        AnglesToAxis( rotationAngles, axis );
        if( !( cg.snap->ps.stats[STAT_STATE] & SS_WALLCLIMBING ) || !bggame->RotateAxis( cg.snap->ps.grapplePoint, axis, rotaxis, false, cg.snap->ps.eFlags & EF_WALLCLIMBCEILING ) )
        {
            AxisCopy( axis, rotaxis );
        }
        
        // Convert the new axis back to angles.
        AxisToAngles( rotaxis, rotationAngles );
    }
    else
    {
        if( cg.predictedPlayerState.stats[ STAT_HEALTH ] > 0 )
        {
            // If we're playing the game in third person, the viewangles already
            // take care of our mouselook, so just use them.
            for( i = 0; i < 3; i++ )
            {
                rotationAngles[i] = cg.refdefViewAngles[i];
            }
        }
        else // dead
        {
            rotationAngles[ PITCH ] = 20;
            rotationAngles[ YAW ] = cg.refdefViewAngles[ YAW ];
        }
    }
    
    // Move the camera range distance back.
    AngleVectors( rotationAngles, forward, right, up );
    VectorCopy( cg.refdef.vieworg, view );
    VectorMA( view, -range, forward, view );
    
    // Ensure that the current camera position isn't out of bounds and that there
    // is nothing between the camera and the player.
    if( !cg_cameraMode.integer )
    {
        // Trace a ray from the origin to the viewpoint to make sure the view isn't
        // in a solid block.  Use an 8 by 8 block to prevent the view from near clipping anything
        idCGamePredict::Trace( &trace, cg.refdef.vieworg, mins, maxs, view, cg.predictedPlayerState.clientNum, MASK_SOLID );
        
        if( trace.fraction != 1.0 )
        {
            VectorCopy( trace.endpos, view );
            view[ 2 ] += ( 1.0 - trace.fraction ) * 32;
            // Try another trace to this position, because a tunnel may have the ceiling
            // close enogh that this is poking out.
            
            idCGamePredict::Trace( &trace, cg.refdef.vieworg, mins, maxs, view, cg.predictedPlayerState.clientNum, MASK_SOLID );
            VectorCopy( trace.endpos, view );
        }
    }
    
    // Set the camera position to what we calculated.
    VectorCopy( view, cg.refdef.vieworg );
    
    // The above checks may have moved the camera such that the existing viewangles
    // may not still face the player. Recalculate them to do so.
    // but if we're dead, don't bother because we'd rather see what killed us
    if( cg.predictedPlayerState.stats[ STAT_HEALTH ] > 0 )
    {
        VectorSubtract( focusPoint, cg.refdef.vieworg, focusPoint );
        vectoangles( focusPoint, cg.refdefViewAngles );
    }
}

/*
===============
idCGameView::OffsetShoulderView
===============
*/
void idCGameView::OffsetShoulderView( void )
{
    sint i, cmdNum;
    usercmd_t cmd, oldCmd;
    vec3_t rotationAngles, axis[3], rotaxis[3];
    float32 deltaMousePitch;
    static float32 mousePitch;
    vec3_t forward, right, up;
    float32 classFwdOffset = 0, classUpOffset = 0, classRightOffset = 0;
    
    AngleVectors( cg.refdefViewAngles, forward, right, up );
    
    // Set a nice view by offsetting from vieworigin to get to the "shoulder"
    // for each class.
    // FIXME: These need to not be hard-coded so hackishly
    switch( cg.snap->ps.stats[STAT_CLASS] )
    {
        case PCL_ALIEN_BUILDER0:
        case PCL_ALIEN_BUILDER0_UPG:
            classFwdOffset = -8;
            classRightOffset = 15;
            classUpOffset = 13;
            break;
        case PCL_ALIEN_LEVEL0:
            classFwdOffset = -5;
            classRightOffset = 0;
            classUpOffset = 17; // +10 for dev from svn
            break;
        case PCL_ALIEN_LEVEL1:
        case PCL_ALIEN_LEVEL1_UPG:
            classFwdOffset = -10;
            classRightOffset = 0;
            classUpOffset = 18;
            break;
        case PCL_ALIEN_LEVEL2:
        case PCL_ALIEN_LEVEL2_UPG:
            classFwdOffset = 0;
            classRightOffset = 12;
            classUpOffset = 5;
            break;
        case PCL_ALIEN_LEVEL3:
            classFwdOffset = -10;
            classRightOffset = 15;
            classUpOffset = 8;
            break;
        case PCL_ALIEN_LEVEL3_UPG:
            classFwdOffset = -10;
            classRightOffset = 17;
            classUpOffset = 12;
            break;
        case PCL_ALIEN_LEVEL4:
            classFwdOffset = -20;
            classRightOffset = -25;
            classUpOffset = 30; // -40 for dev from svn
            break;
        case PCL_HUMAN:
            classFwdOffset = -10;
            classRightOffset = 15;
            classUpOffset = 0;
            break;
        case PCL_HUMAN_BSUIT:
            classFwdOffset = -30;
            classRightOffset = 25;
            classUpOffset = -2; // +6 for dev from svn
            break;
    }
    
    // The override is temporary so that people can help find good offset positions for me.
    // It will not remain in final code.
    if( !cg_shoulderViewOverride.integer )
    {
        VectorMA( cg.refdef.vieworg, classFwdOffset, forward, cg.refdef.vieworg );
        VectorMA( cg.refdef.vieworg, classUpOffset, up, cg.refdef.vieworg );
        VectorMA( cg.refdef.vieworg, classRightOffset, right, cg.refdef.vieworg );
    }
    else
    {
        VectorMA( cg.refdef.vieworg, cg_shoulderViewForward.value, forward, cg.refdef.vieworg );
        VectorMA( cg.refdef.vieworg, cg_shoulderViewUp.value, up, cg.refdef.vieworg );
        VectorMA( cg.refdef.vieworg, cg_shoulderViewRight.value, right, cg.refdef.vieworg );
    }
    
    // If someone is playing like this, the rest is already taken care of
    // so just get the firstperson effects and leave.
    if( !cg.demoPlayback && !( cg.snap->ps.pm_flags & PMF_FOLLOW ) )
    {
        OffsetFirstPersonView();
        return;
    }
    
    // Get mouse input for camera rotation.
    cmdNum = trap_GetCurrentCmdNumber();
    trap_GetUserCmd( cmdNum, &cmd );
    trap_GetUserCmd( cmdNum - 1, &oldCmd );
    
    // Prevent pitch from wrapping and clamp it within a [30, -50] range.
    // Cgame has no access to ps.delta_angles[] here, so we need to reproduce
    // it ourselves here.
    deltaMousePitch = SHORT2ANGLE( cmd.angles[PITCH] - oldCmd.angles[PITCH] );
    
    if( fabs( deltaMousePitch ) < 200.0f )
    {
        mousePitch += deltaMousePitch;
    }
    
    // Handle pitch.
    rotationAngles[PITCH] = mousePitch;
    
    // Ignore following pitch; it's too jerky otherwise.
    if( cg_thirdPersonPitchFollow.integer )
    {
        mousePitch += cg.refdefViewAngles[PITCH];
    }
    
    rotationAngles[PITCH] = AngleNormalize180( rotationAngles[PITCH] );
    
    if( rotationAngles[PITCH] < -90 )
    {
        rotationAngles[PITCH] = -90;
    }
    
    if( rotationAngles[PITCH] > 90 )
    {
        rotationAngles[PITCH] = 90;
    }
    
    // Yaw and Roll are much easier.
    rotationAngles[ YAW ] = SHORT2ANGLE( cmd.angles[ YAW ] ) + cg.refdefViewAngles[ YAW ];
    rotationAngles[ ROLL ] = 0;
    
    // Perform the rotations.
    AnglesToAxis( rotationAngles, axis );
    
    if( !( cg.snap->ps.stats[STAT_STATE] & SS_WALLCLIMBING ) || !bggame->RotateAxis( cg.snap->ps.grapplePoint, axis, rotaxis, false, cg.snap->ps.eFlags & EF_WALLCLIMBCEILING ) )
    {
        AxisCopy( axis, rotaxis );
    }
    
    AxisToAngles( rotaxis, rotationAngles );
    
    // Actually set the viewangles.
    for( i = 0; i < 3; i++ )
    {
        cg.refdefViewAngles[i] = rotationAngles[i];
    }
    
    // Now run the first person stuff so we get various effects added.
    OffsetFirstPersonView( );
}

/*
===============
idCGameView::StepOffset

this causes a compiler bug on mac MrC compiler
===============
*/
void idCGameView::StepOffset( void )
{
    float32 steptime;
    sint timeDelta;
    vec3_t normal;
    playerState_t* ps = &cg.predictedPlayerState;
    
    bggame->GetClientNormal( ps, normal );
    
    steptime = bggame->Class( ( class_t )ps->stats[ STAT_CLASS ] )->steptime;
    
    // smooth out stair climbing
    timeDelta = cg.time - cg.stepTime;
    
    if( timeDelta < steptime )
    {
        float32 stepChange = cg.stepChange * ( steptime - timeDelta ) / steptime;
        
        VectorMA( cg.refdef.vieworg, -stepChange, normal, cg.refdef.vieworg );
    }
}

/*
===============
idCGameView::OffsetFirstPersonView
===============
*/
#define PCLOUD_ROLL_AMPLITUDE   25.0f
#define PCLOUD_ROLL_FREQUENCY   0.4f
#define PCLOUD_ZOOM_AMPLITUDE   15
#define PCLOUD_ZOOM_FREQUENCY   0.7f
void idCGameView::OffsetFirstPersonView( void )
{
    float32* origin;
    float32* angles;
    float32 bob, ratio, delta, speed, f;
    vec3_t predictedVelocity;
    sint timeDelta;
    float32 bob2;
    vec3_t normal, baseOrigin;
    playerState_t* ps = &cg.predictedPlayerState;
    
    bggame->GetClientNormal( ps, normal );
    
    if( cg.snap->ps.pm_type == PM_INTERMISSION )
    {
        return;
    }
    
    origin = cg.refdef.vieworg;
    angles = cg.refdefViewAngles;
    
    VectorCopy( origin, baseOrigin );
    
    // if dead, fix the angle and don't add any kick
    if( cg.snap->ps.stats[ STAT_HEALTH ] <= 0 )
    {
        angles[ ROLL ] = 40;
        angles[ PITCH ] = -15;
        angles[ YAW ] = cg.snap->ps.stats[ STAT_VIEWLOCK ];
        origin[ 2 ] += cg.predictedPlayerState.viewheight;
        return;
    }
    
    // add angles based on weapon kick
    VectorAdd( angles, cg.kick_angles, angles );
    
    // add angles based on damage kick
    if( cg.damageTime )
    {
        ratio = cg.time - cg.damageTime;
        if( ratio < DAMAGE_DEFLECT_TIME )
        {
            ratio /= DAMAGE_DEFLECT_TIME;
            angles[ PITCH ] += ratio * cg.v_dmg_pitch;
            angles[ ROLL ] += ratio * cg.v_dmg_roll;
        }
        else
        {
            ratio = 1.0 - ( ratio - DAMAGE_DEFLECT_TIME ) / DAMAGE_RETURN_TIME;
            if( ratio > 0 )
            {
                angles[ PITCH ] += ratio * cg.v_dmg_pitch;
                angles[ ROLL ] += ratio * cg.v_dmg_roll;
            }
        }
    }
    
    // add pitch based on fall kick
#if 0
    ratio = ( cg.time - cg.landTime ) / FALL_TIME;
    if( ratio < 0 )
        ratio = 0;
    angles[PITCH] += ratio * cg.fall_value;
#endif
    
    // add angles based on velocity
    VectorCopy( cg.predictedPlayerState.velocity, predictedVelocity );
    
    delta = DotProduct( predictedVelocity, cg.refdef.viewaxis[ 0 ] );
    angles[ PITCH ] += delta * cg_runpitch.value;
    
    delta = DotProduct( predictedVelocity, cg.refdef.viewaxis[ 1 ] );
    angles[ ROLL ] -= delta * cg_runroll.value;
    
    // add angles based on bob
    // bob amount is class dependant
    
    if( cg.snap->ps.persistant[PERS_SPECSTATE] != SPECTATOR_NOT )
    {
        bob2 = 0.0f;
    }
    else
    {
        bob2 = bggame->Class( ( class_t )cg.predictedPlayerState.stats[STAT_CLASS] )->bob;
    }
    
#define LEVEL4_FEEDBACK  10.0f
    
    //give a charging player some feedback
    if( ps->weapon == WP_ALEVEL4 )
    {
        if( ps->stats[ STAT_MISC ] > 0 )
        {
            float32 fraction = ( float32 )ps->stats[ STAT_MISC ] / LEVEL4_TRAMPLE_CHARGE_MAX;
            
            if( fraction > 1.0f )
            {
                fraction = 1.0f;
            }
            
            bob2 *= ( 1.0f + fraction * LEVEL4_FEEDBACK );
        }
    }
    
    if( bob2 != 0.0f )
    {
        // make sure the bob is visible even at low speeds
        speed = cg.xyspeed > 200 ? cg.xyspeed : 200;
        
        delta = cg.bobfracsin * ( bob2 ) * speed;
        if( cg.predictedPlayerState.pm_flags & PMF_DUCKED )
        {
            delta *= 3;   // crouching
        }
        
        angles[ PITCH ] += delta;
        delta = cg.bobfracsin * ( bob2 ) * speed;
        if( cg.predictedPlayerState.pm_flags & PMF_DUCKED )
        {
            delta *= 3;   // crouching accentuates roll
        }
        
        if( cg.bobcycle & 1 )
        {
            delta = -delta;
        }
        
        angles[ ROLL ] += delta;
    }
    
#define LEVEL3_FEEDBACK  20.0f
    
    //provide some feedback for pouncing
    if( ( cg.predictedPlayerState.weapon == WP_ALEVEL3 || cg.predictedPlayerState.weapon == WP_ALEVEL3_UPG ) && cg.predictedPlayerState.stats[ STAT_MISC ] > 0 )
    {
        float32 fraction1, fraction2;
        vec3_t forward;
        
        AngleVectors( angles, forward, nullptr, nullptr );
        VectorNormalize( forward );
        
        fraction1 = ( float32 )cg.predictedPlayerState.stats[ STAT_MISC ] / LEVEL3_POUNCE_TIME_UPG;
        
        if( fraction1 > 1.0f )
        {
            fraction1 = 1.0f;
        }
        
        fraction2 = -sin( fraction1 * M_PI / 2 );
        
        VectorMA( origin, LEVEL3_FEEDBACK * fraction2, forward, origin );
    }
    
#define STRUGGLE_DIST 5.0f
#define STRUGGLE_TIME 250
    
    //allow the player to struggle a little whilst grabbed
    if( cg.predictedPlayerState.pm_type == PM_GRABBED )
    {
        vec3_t forward, right, up;
        usercmd_t cmd;
        sint cmdNum;
        float32 fFraction, rFraction, uFraction, fFraction2, rFraction2, uFraction2;
        
        cmdNum = trap_GetCurrentCmdNumber();
        trap_GetUserCmd( cmdNum, &cmd );
        
        AngleVectors( angles, forward, right, up );
        
        fFraction = ( float32 )( cg.time - cg.forwardMoveTime ) / STRUGGLE_TIME;
        rFraction = ( float32 )( cg.time - cg.rightMoveTime ) / STRUGGLE_TIME;
        uFraction = ( float32 )( cg.time - cg.upMoveTime ) / STRUGGLE_TIME;
        
        if( fFraction > 1.0f )
        {
            fFraction = 1.0f;
        }
        
        if( rFraction > 1.0f )
        {
            rFraction = 1.0f;
        }
        
        if( uFraction > 1.0f )
        {
            uFraction = 1.0f;
        }
        
        fFraction2 = -sin( fFraction * M_PI / 2 );
        rFraction2 = -sin( rFraction * M_PI / 2 );
        uFraction2 = -sin( uFraction * M_PI / 2 );
        
        if( cmd.forwardmove > 0 )
        {
            VectorMA( origin, STRUGGLE_DIST * fFraction, forward, origin );
        }
        else if( cmd.forwardmove < 0 )
        {
            VectorMA( origin, -STRUGGLE_DIST * fFraction, forward, origin );
        }
        else
        {
            cg.forwardMoveTime = cg.time;
        }
        
        if( cmd.rightmove > 0 )
        {
            VectorMA( origin, STRUGGLE_DIST * rFraction, right, origin );
        }
        else if( cmd.rightmove < 0 )
        {
            VectorMA( origin, -STRUGGLE_DIST * rFraction, right, origin );
        }
        else
        {
            cg.rightMoveTime = cg.time;
        }
        
        if( cmd.upmove > 0 )
        {
            VectorMA( origin, STRUGGLE_DIST * uFraction, up, origin );
        }
        else if( cmd.upmove < 0 )
        {
            VectorMA( origin, -STRUGGLE_DIST * uFraction, up, origin );
        }
        else
        {
            cg.upMoveTime = cg.time;
        }
    }
    
    if( ( cg.predictedPlayerEntity.currentState.eFlags & EF_POISONCLOUDED ) && !( cg.snap->ps.pm_flags & PMF_FOLLOW ) )
    {
        float32 scale, fraction, pitchFraction;
        
        scale = 1.0f - ( float32 )( cg.time - cg.poisonedTime ) / bggame->PlayerPoisonCloudTime( &cg.predictedPlayerState );
        
        if( scale < 0.f )
        {
            scale = 0.f;
        }
        
        fraction = sin( cg.time / 500.0f * M_PI * PCLOUD_ROLL_FREQUENCY ) * scale;
        pitchFraction = sin( cg.time / 200.0f * M_PI * PCLOUD_ROLL_FREQUENCY ) * scale;
        
        angles[ ROLL ] += fraction * PCLOUD_ROLL_AMPLITUDE;
        angles[ YAW ] += fraction * PCLOUD_ROLL_AMPLITUDE;
        angles[ PITCH ] += pitchFraction * PCLOUD_ROLL_AMPLITUDE / 2.0f;
    }
    
    // this *feels* more realisitic for humans
    if( cg.predictedPlayerState.stats[ STAT_TEAM ] == TEAM_HUMANS && ( cg.predictedPlayerState.pm_type == PM_NORMAL || cg.predictedPlayerState.pm_type == PM_JETPACK ) )
    {
        angles[PITCH] += cg.bobfracsin * bob2 * 0.5;
        
        // heavy breathing effects //FIXME: sound
        if( cg.predictedPlayerState.stats[ STAT_STAMINA ] < STAMINA_BREATHING_LEVEL )
        {
            float32 deltaBreath = ( cg.predictedPlayerState.stats[ STAT_STAMINA ] - STAMINA_BREATHING_LEVEL ) / -250.0;
            float32 deltaAngle = cos( ( float32 )cg.time / 150.0 ) * deltaBreath;
            
            deltaAngle += ( deltaAngle < 0 ? -deltaAngle : deltaAngle ) * 0.5;
            
            angles[ PITCH ] -= deltaAngle;
        }
    }
    
    //===================================
    
    // add view height
    VectorMA( origin, ps->viewheight, normal, origin );
    
    // smooth out duck height changes
    timeDelta = cg.time - cg.duckTime;
    if( timeDelta < DUCK_TIME )
    {
        cg.refdef.vieworg[ 2 ] -= cg.duckChange * ( DUCK_TIME - timeDelta ) / DUCK_TIME;
    }
    
    // add bob height
    bob = cg.bobfracsin * cg.xyspeed * bob2;
    
    if( bob > 6 )
    {
        bob = 6;
    }
    
    VectorMA( origin, bob, normal, origin );
    
    // add fall height
    delta = cg.time - cg.landTime;
    
    if( delta < LAND_DEFLECT_TIME )
    {
        f = delta / LAND_DEFLECT_TIME;
        cg.refdef.vieworg[ 2 ] += cg.landChange * f;
    }
    else if( delta < LAND_DEFLECT_TIME + LAND_RETURN_TIME )
    {
        delta -= LAND_DEFLECT_TIME;
        f = 1.0 - ( delta / LAND_RETURN_TIME );
        cg.refdef.vieworg[ 2 ] += cg.landChange * f;
    }
    
    // add step offset
    StepOffset( );
    
    // add kick offset
    VectorAdd( origin, cg.kick_origin, origin );
}

/*
====================
idCGameView::CalcFov

Fixed fov at intermissions, otherwise account for fov variable and zooms.
====================
*/
#define WAVE_AMPLITUDE  1
#define WAVE_FREQUENCY  0.4
#define FOVWARPTIME     400.0
sint idCGameView::CalcFov( void )
{
    float32 x, phase, v, fov_x, fov_y, zoomFov, f;
    sint contents, cmdNum, inwater, attribFov;
    usercmd_t cmd, oldcmd;
    
    cmdNum = trap_GetCurrentCmdNumber( );
    trap_GetUserCmd( cmdNum, &cmd );
    trap_GetUserCmd( cmdNum - 1, &oldcmd );
    
    // switch follow modes if necessary: cycle between free -> follow -> third-person follow
    if( cmd.buttons & BUTTON_USE_HOLDABLE && !( oldcmd.buttons & BUTTON_USE_HOLDABLE ) )
    {
        if( cg.snap->ps.pm_flags & PMF_FOLLOW )
        {
            if( !cg.chaseFollow )
            {
                cg.chaseFollow = true;
            }
            else
            {
                cg.chaseFollow = false;
                trap_SendClientCommand( "follow\n" );
            }
        }
        else if( cg.snap->ps.persistant[PERS_SPECSTATE] != SPECTATOR_NOT )
        {
            trap_SendClientCommand( "follow\n" );
        }
    }
    
    if( cg.predictedPlayerState.pm_type == PM_INTERMISSION || ( cg.snap->ps.persistant[ PERS_SPECSTATE ] != SPECTATOR_NOT ) || ( cg.renderingThirdPerson ) )
    {
        // if in intermission or third person, use a fixed value
        fov_x = 90;
    }
    else
    {
        // don't lock the fov globally - we need to be able to change it
        attribFov = bggame->Class( ( class_t )cg.predictedPlayerState.stats[ STAT_CLASS ] )->fov;
        fov_x = attribFov;
        
        if( fov_x < 1 )
        {
            fov_x = 1;
        }
        else if( fov_x > 160 )
        {
            fov_x = 160;
        }
        
        if( cg.spawnTime > ( cg.time - FOVWARPTIME ) && bggame->ClassHasAbility( ( class_t )cg.predictedPlayerState.stats[ STAT_CLASS ], SCA_FOVWARPS ) )
        {
            float32 temp, temp2;
            
            temp = ( float32 )( cg.time - cg.spawnTime ) / FOVWARPTIME;
            temp2 = ( 170 - fov_x ) * temp;
            
            //Com_Printf( "%f %f\n", temp*100, temp2*100 );
            
            fov_x = 170 - temp2;
        }
        
        // account for zooms
        zoomFov = bggame->Weapon( ( weapon_t )cg.predictedPlayerState.weapon )->zoomFov;
        if( zoomFov < 1 )
        {
            zoomFov = 1;
        }
        else if( zoomFov > attribFov )
        {
            zoomFov = attribFov;
        }
        
        // only do all the zoom stuff if the client CAN zoom
        // FIXME: zoom control is currently hard coded to BUTTON_ATTACK2
        if( bggame->Weapon( ( weapon_t )cg.predictedPlayerState.weapon )->canZoom )
        {
            if( cg.zoomed )
            {
                f = ( cg.time - cg.zoomTime ) / ( float32 )ZOOM_TIME;
                
                if( f > 1.0 )
                {
                    fov_x = zoomFov;
                }
                else
                {
                    fov_x = fov_x + f * ( zoomFov - fov_x );
                }
                
                // BUTTON_ATTACK2 isn't held so unzoom next time
                if( !( cmd.buttons & BUTTON_ATTACK2 ) )
                {
                    cg.zoomed = false;
                    cg.zoomTime = MIN( cg.time, cg.time + cg.time - cg.zoomTime - ZOOM_TIME );
                }
            }
            else
            {
                f = ( cg.time - cg.zoomTime ) / ( float32 )ZOOM_TIME;
                
                if( f > 1.0 )
                {
                    fov_x = fov_x;
                }
                else
                {
                    fov_x = zoomFov + f * ( fov_x - zoomFov );
                }
                
                // BUTTON_ATTACK2 is held so zoom next time
                if( cmd.buttons & BUTTON_ATTACK2 )
                {
                    cg.zoomed   = true;
                    cg.zoomTime = MIN( cg.time, cg.time + cg.time - cg.zoomTime - ZOOM_TIME );
                }
            }
        }
    }
    
    x = cg.refdef.width / tan( fov_x / 360 * M_PI );
    fov_y = atan2( cg.refdef.height, x );
    fov_y = fov_y * 360 / M_PI;
    
    // warp if underwater
    contents = idCGamePredict::PointContents( cg.refdef.vieworg, -1 );
    
    if( contents & ( CONTENTS_WATER | CONTENTS_SLIME | CONTENTS_LAVA ) )
    {
        phase = cg.time / 1000.0 * WAVE_FREQUENCY * M_PI * 2;
        v = WAVE_AMPLITUDE * sin( phase );
        fov_x += v;
        fov_y -= v;
        inwater = true;
        cg.refdef.rdflags |= RDF_UNDERWATER;
    }
    else
    {
        inwater = false;
    }
    
    if( ( cg.predictedPlayerEntity.currentState.eFlags & EF_POISONCLOUDED ) && cg.predictedPlayerState.stats[ STAT_HEALTH ] > 0 && !( cg.snap->ps.pm_flags & PMF_FOLLOW ) )
    {
        float32 scale = 1.0f - ( float32 )( cg.time - cg.poisonedTime ) / bggame->PlayerPoisonCloudTime( &cg.predictedPlayerState );
        
        phase = cg.time / 1000.0 * PCLOUD_ZOOM_FREQUENCY * M_PI * 2;
        v = PCLOUD_ZOOM_AMPLITUDE * sin( phase ) * scale;
        fov_x += v;
        fov_y += v;
    }
    
    
    // set it
    cg.refdef.fov_x = fov_x;
    cg.refdef.fov_y = fov_y;
    
    if( !cg.zoomed )
    {
        cg.zoomSensitivity = 1;
    }
    else
    {
        cg.zoomSensitivity = cg.refdef.fov_y / 75.0;
    }
    
    return inwater;
}

/*
===============
CG_DrawSurfNormal

Draws a vector against
the surface player is looking at
===============
*/
#define NORMAL_HEIGHT 64.0f
#define NORMAL_WIDTH  6.0f
void idCGameView::DrawSurfNormal( void )
{
    trace_t tr;
    vec3_t end, temp;
    polyVert_t normal[ 4 ];
    vec4_t color = { 0.0f, 255.0f, 0.0f, 128.0f };
    
    VectorMA( cg.refdef.vieworg, 8192, cg.refdef.viewaxis[ 0 ], end );
    
    idCGamePredict::Trace( &tr, cg.refdef.vieworg, nullptr, nullptr, end, cg.predictedPlayerState.clientNum, MASK_SOLID );
    
    VectorCopy( tr.endpos, normal[ 0 ].xyz );
    normal[ 0 ].st[ 0 ] = 0;
    normal[ 0 ].st[ 1 ] = 0;
    Vector4Copy( color, normal[ 0 ].modulate );
    
    VectorMA( tr.endpos, NORMAL_WIDTH, cg.refdef.viewaxis[ 1 ], temp );
    VectorCopy( temp, normal[ 1 ].xyz );
    normal[ 1 ].st[ 0 ] = 0;
    normal[ 1 ].st[ 1 ] = 1;
    Vector4Copy( color, normal[ 1 ].modulate );
    
    VectorMA( tr.endpos, NORMAL_HEIGHT, tr.plane.normal, temp );
    VectorMA( temp, NORMAL_WIDTH, cg.refdef.viewaxis[ 1 ], temp );
    VectorCopy( temp, normal[ 2 ].xyz );
    normal[ 2 ].st[ 0 ] = 1;
    normal[ 2 ].st[ 1 ] = 1;
    Vector4Copy( color, normal[ 2 ].modulate );
    
    VectorMA( tr.endpos, NORMAL_HEIGHT, tr.plane.normal, temp );
    VectorCopy( temp, normal[ 3 ].xyz );
    normal[ 3 ].st[ 0 ] = 1;
    normal[ 3 ].st[ 1 ] = 0;
    Vector4Copy( color, normal[ 3 ].modulate );
    
    trap_R_AddPolyToScene( cgs.media.outlineShader, 4, normal );
}

/*
===============
idCGameView::addSmoothOp
===============
*/
void idCGameView::addSmoothOp( vec3_t rotAxis, float32 rotAngle, float32 timeMod )
{
    sint i;
    
    //iterate through smooth array
    for( i = 0; i < MAXSMOOTHS; i++ )
    {
        //found an unused index in the smooth array
        if( cg.sList[ i ].time + cg_wwSmoothTime.integer < cg.time )
        {
            //copy to array and stop
            VectorCopy( rotAxis, cg.sList[ i ].rotAxis );
            cg.sList[ i ].rotAngle = rotAngle;
            cg.sList[ i ].time = cg.time;
            cg.sList[ i ].timeMod = timeMod;
            return;
        }
    }
    
    //no free indices in the smooth array
}

/*
===============
idCGameView::smoothWWTransitions
===============
*/
void idCGameView::smoothWWTransitions( playerState_t* ps, const vec3_t in, vec3_t out )
{
    vec3_t surfNormal, rotAxis, temp;
    vec3_t refNormal     = { 0.0f, 0.0f,  1.0f };
    vec3_t ceilingNormal = { 0.0f, 0.0f, -1.0f };
    sint i;
    float32 stLocal, sFraction, rotAngle, smoothTime, timeMod;
    bool performed = false;
    vec3_t inAxis[ 3 ], lastAxis[ 3 ], outAxis[ 3 ];
    
    if( cg.snap->ps.pm_flags & PMF_FOLLOW )
    {
        VectorCopy( in, out );
        return;
    }
    
    //set surfNormal
    bggame->GetClientNormal( ps, surfNormal );
    
    AnglesToAxis( in, inAxis );
    
    //if we are moving from one surface to another smooth the transition
    if( !VectorCompare( surfNormal, cg.lastNormal ) )
    {
        //if we moving from the ceiling to the floor special case
        //( x product of colinear vectors is undefined)
        if( VectorCompare( ceilingNormal, cg.lastNormal ) && VectorCompare( refNormal, surfNormal ) )
        {
            AngleVectors( in, temp, nullptr, nullptr );
            ProjectPointOnPlane( rotAxis, temp, refNormal );
            VectorNormalize( rotAxis );
            rotAngle = 180.0f;
            timeMod = 1.5f;
        }
        else
        {
            AnglesToAxis( cg.lastVangles, lastAxis );
            rotAngle = DotProduct( inAxis[ 0 ], lastAxis[ 0 ] ) + DotProduct( inAxis[ 1 ], lastAxis[ 1 ] ) + DotProduct( inAxis[ 2 ], lastAxis[ 2 ] );
            
            rotAngle = RAD2DEG( acos( ( rotAngle - 1.0f ) / 2.0f ) );
            
            CrossProduct( lastAxis[ 0 ], inAxis[ 0 ], temp );
            VectorCopy( temp, rotAxis );
            CrossProduct( lastAxis[ 1 ], inAxis[ 1 ], temp );
            VectorAdd( rotAxis, temp, rotAxis );
            CrossProduct( lastAxis[ 2 ], inAxis[ 2 ], temp );
            VectorAdd( rotAxis, temp, rotAxis );
            
            VectorNormalize( rotAxis );
            
            timeMod = 1.0f;
        }
        
        //add the op
        addSmoothOp( rotAxis, rotAngle, timeMod );
    }
    
    //iterate through ops
    for( i = MAXSMOOTHS - 1; i >= 0; i-- )
    {
        smoothTime = ( sint )( cg_wwSmoothTime.integer * cg.sList[ i ].timeMod );
        
        //if this op has time remaining, perform it
        if( cg.time < cg.sList[ i ].time + smoothTime )
        {
            stLocal = 1.0f - ( ( ( cg.sList[ i ].time + smoothTime ) - cg.time ) / smoothTime );
            sFraction = -( cos( stLocal * M_PI ) + 1.0f ) / 2.0f;
            
            RotatePointAroundVector( outAxis[ 0 ], cg.sList[ i ].rotAxis, inAxis[ 0 ], sFraction * cg.sList[ i ].rotAngle );
            RotatePointAroundVector( outAxis[ 1 ], cg.sList[ i ].rotAxis, inAxis[ 1 ], sFraction * cg.sList[ i ].rotAngle );
            RotatePointAroundVector( outAxis[ 2 ], cg.sList[ i ].rotAxis, inAxis[ 2 ], sFraction * cg.sList[ i ].rotAngle );
            
            AxisCopy( outAxis, inAxis );
            performed = true;
        }
    }
    
    //if we performed any ops then return the smoothed angles
    //otherwise simply return the in angles
    if( performed )
    {
        AxisToAngles( outAxis, out );
    }
    else
    {
        VectorCopy( in, out );
    }
    
    //copy the current normal to the lastNormal
    VectorCopy( in, cg.lastVangles );
    VectorCopy( surfNormal, cg.lastNormal );
}

/*
===============
idCGameView::smoothWJTransitions
===============
*/
void idCGameView::smoothWJTransitions( playerState_t* ps, const vec3_t in, vec3_t out )
{
    sint i;
    float32 stLocal, sFraction;
    bool  performed = false;
    vec3_t inAxis[ 3 ], outAxis[ 3 ];
    
    if( cg.snap->ps.pm_flags & PMF_FOLLOW )
    {
        VectorCopy( in, out );
        return;
    }
    
    AnglesToAxis( in, inAxis );
    
    //iterate through ops
    for( i = MAXSMOOTHS - 1; i >= 0; i-- )
    {
        //if this op has time remaining, perform it
        if( cg.time < cg.sList[ i ].time + cg_wwSmoothTime.integer )
        {
            stLocal = ( ( cg.sList[ i ].time + cg_wwSmoothTime.integer ) - cg.time ) / cg_wwSmoothTime.integer;
            sFraction = 1.0f - ( ( cos( stLocal * M_PI * 2.0f ) + 1.0f ) / 2.0f );
            
            RotatePointAroundVector( outAxis[ 0 ], cg.sList[ i ].rotAxis, inAxis[ 0 ], sFraction * cg.sList[ i ].rotAngle );
            RotatePointAroundVector( outAxis[ 1 ], cg.sList[ i ].rotAxis, inAxis[ 1 ], sFraction * cg.sList[ i ].rotAngle );
            RotatePointAroundVector( outAxis[ 2 ], cg.sList[ i ].rotAxis, inAxis[ 2 ], sFraction * cg.sList[ i ].rotAngle );
            
            AxisCopy( outAxis, inAxis );
            performed = true;
        }
    }
    
    //if we performed any ops then return the smoothed angles
    //otherwise simply return the in angles
    if( performed )
    {
        AxisToAngles( outAxis, out );
    }
    else
    {
        VectorCopy( in, out );
    }
}


/*
===============
idCGameView::CalcViewValues

Sets cg.refdef view values
===============
*/
sint idCGameView::CalcViewValues( void )
{
    playerState_t* ps;
    
    ::memset( &cg.refdef, 0, sizeof( cg.refdef ) );
    
    // calculate size of 3D view
    CalcVrect( );
    
    ps = &cg.predictedPlayerState;
    
    // intermission view
    if( ps->pm_type == PM_INTERMISSION || ps->pm_type == PM_FREEZE || ps->pm_type == PM_SPECTATOR )
    {
        VectorCopy( ps->origin, cg.refdef.vieworg );
        VectorCopy( ps->viewangles, cg.refdefViewAngles );
        AnglesToAxis( cg.refdefViewAngles, cg.refdef.viewaxis );
        
        return CalcFov( );
    }
    
    cg.bobcycle = ( ps->bobCycle & 128 ) >> 7;
    cg.bobfracsin = fabs( sin( ( ps->bobCycle & 127 ) / 127.0 * M_PI ) );
    cg.xyspeed = sqrt( ps->velocity[ 0 ] * ps->velocity[ 0 ] + ps->velocity[ 1 ] * ps->velocity[ 1 ] );
    
    // the bob velocity should't get too fast to avoid jerking
    if( cg.xyspeed > 300.f )
    {
        cg.xyspeed = 300.f;
    }
    
    VectorCopy( ps->origin, cg.refdef.vieworg );
    
    if( bggame->ClassHasAbility( ( class_t )ps->stats[STAT_CLASS], SCA_WALLCLIMBER ) )
    {
        smoothWWTransitions( ps, ps->viewangles, cg.refdefViewAngles );
    }
    else if( bggame->ClassHasAbility( ( class_t )ps->stats[STAT_CLASS], SCA_WALLJUMPER ) )
    {
        smoothWJTransitions( ps, ps->viewangles, cg.refdefViewAngles );
    }
    else
    {
        VectorCopy( ps->viewangles, cg.refdefViewAngles );
    }
    
    //clumsy logic, but it needs to be this way round because the CS propogation
    //delay screws things up otherwise
    if( !bggame->ClassHasAbility( ( class_t )ps->stats[ STAT_CLASS ], SCA_WALLJUMPER ) )
    {
        if( !( ps->stats[STAT_STATE] & SS_WALLCLIMBING ) )
        {
            VectorSet( cg.lastNormal, 0.0f, 0.0f, 1.0f );
        }
    }
    
    // add error decay
    if( cg_errorDecay.value > 0 )
    {
        sint t;
        float32 f;
        
        t = cg.time - cg.predictedErrorTime;
        f = ( cg_errorDecay.value - t ) / cg_errorDecay.value;
        
        if( f > 0 && f < 1 )
        {
            VectorMA( cg.refdef.vieworg, f, cg.predictedError, cg.refdef.vieworg );
        }
        else
        {
            cg.predictedErrorTime = 0;
        }
    }
    
    //shut off the poison cloud effect if it's still on the go
    if( cg.snap->ps.stats[ STAT_HEALTH ] <= 0 )
    {
        if( idCGameParticles::IsParticleSystemValid( &cg.poisonCloudPS ) )
        {
            idCGameParticles::DestroyParticleSystem( &cg.poisonCloudPS );
        }
    }
    
    if( cg.renderingThirdPerson )
    {
        // back away from character
        OffsetThirdPersonView( );
    }
    else
    {
        // offset for local bobbing and kicks
        OffsetFirstPersonView( );
    }
    
    // position eye reletive to origin
    AnglesToAxis( cg.refdefViewAngles, cg.refdef.viewaxis );
    
    if( cg.hyperspace )
    {
        cg.refdef.rdflags |= RDF_NOWORLDMODEL | RDF_HYPERSPACE;
    }
    
    //draw the surface normal looking at
    if( cg_drawSurfNormal.integer )
    {
        DrawSurfNormal();
    }
    
    // field of view
    return CalcFov( );
}

/*
=====================
idCGameView::AddBufferedSound
=====================
*/
void idCGameView::AddBufferedSound( sfxHandle_t sfx )
{
    if( !sfx )
    {
        return;
    }
    
    cg.soundBuffer[ cg.soundBufferIn ] = sfx;
    cg.soundBufferIn = ( cg.soundBufferIn + 1 ) % MAX_SOUNDBUFFER;
    
    if( cg.soundBufferIn == cg.soundBufferOut )
    {
        cg.soundBufferOut++;
    }
}

/*
=====================
idCGameView::PlayBufferedSounds
=====================
*/
void idCGameView::PlayBufferedSounds( void )
{
    if( cg.soundTime < cg.time )
    {
        if( cg.soundBufferOut != cg.soundBufferIn && cg.soundBuffer[ cg.soundBufferOut ] )
        {
            trap_S_StartLocalSound( cg.soundBuffer[ cg.soundBufferOut ], CHAN_ANNOUNCER );
            cg.soundBuffer[ cg.soundBufferOut ] = 0;
            cg.soundBufferOut = ( cg.soundBufferOut + 1 ) % MAX_SOUNDBUFFER;
            cg.soundTime = cg.time + 750;
        }
    }
}

/*
=================
idCGameLocal::DrawActiveFrame

Generates and draws a game scene and status information at the given time.
=================
*/
void idCGameLocal::DrawActiveFrame( sint serverTime, stereoFrame_t stereoView, bool demoPlayback )
{
    sint inwater;
    
    cg.time = serverTime;
    cg.demoPlayback = demoPlayback;
    
    // update cvars
    idCGameMain::UpdateCvars( );
    
    // if we are only updating the screen as a loading
    // pacifier, don't even try to read snapshots
    if( cg.infoScreenText[ 0 ] != 0 )
    {
        idCGameDraw::DrawLoadingScreen( );
        return;
    }
    
    // any looped sounds will be respecified as entities
    // are added to the render list
    trap_S_ClearLoopingSounds( false );
    
    // clear all the render lists
    trap_R_ClearScene( );
    
    // set up cg.snap and possibly cg.nextSnap
    idCGameSnapshot::ProcessSnapshots( );
    
    // if we haven't received any snapshots yet, all
    // we can draw is the information screen
    if( !cg.snap || ( cg.snap->snapFlags & SNAPFLAG_NOT_ACTIVE ) )
    {
        idCGameDraw::DrawLoadingScreen( );
        return;
    }
    
    // let the client system know what our weapon and zoom settings are
    trap_SetUserCmdValue( cg.weaponSelect, cg.zoomSensitivity ? 0x01 : 0x00, cg.zoomSensitivity, cg.identifyClientRequest );
    
    // this counter will be bumped for every valid scene we generate
    cg.clientFrame++;
    
    // update cg.predictedPlayerState
    idCGamePredict::PredictPlayerState( );
    
    // cg.refdef is 100% inisialized here -> set stereo flag
    //cg.refdef.stereoFrame = stereoView;
    
    // decide on third person view
    cg.renderingThirdPerson = ( cg_thirdPerson.integer || ( cg.snap->ps.stats[ STAT_HEALTH ] <= 0 ) || ( cg.chaseFollow && cg.snap->ps.pm_flags & PMF_FOLLOW ) );
    
    // build cg.refdef
    inwater = idCGameView::CalcViewValues( );
    
    // build the render lists
    if( !cg.hyperspace )
    {
        idCGameEnts::AddPacketEntities( ); // after calcViewValues, so predicted player state is correct
        idCGameMarks::AddMarks( );
    }
    
    idCGameWeapons::AddViewWeapon( &cg.predictedPlayerState );
    
    //after CG_AddViewWeapon
    if( !cg.hyperspace )
    {
        idCGameParticles::AddParticles( );
        idCGameTrails::AddTrails( );
    }
    
    // add buffered sounds
    idCGameView::PlayBufferedSounds( );
    
    // finish up the rest of the refdef
    if( cg.testModelEntity.hModel )
    {
        idCGameView::AddTestModel();
    }
    
    cg.refdef.time = cg.time;
    ::memcpy( cg.refdef.areamask, cg.snap->areamask, sizeof( cg.refdef.areamask ) );
    
    //remove expired console lines
    if( cg.consoleLines[0].time + cg_consoleLatency.integer < cg.time && cg_consoleLatency.integer > 0 )
    {
        idCGameMain::RemoveNotifyLine();
    }
    
    // update audio positions
    trap_S_Respatialize( cg.snap->ps.clientNum, cg.refdef.vieworg, cg.refdef.viewaxis, inwater );
    
    // make sure the lagometerSample and frame timing isn't done twice when in stereo
    if( stereoView != STEREO_RIGHT )
    {
        cg.frametime = cg.time - cg.oldTime;
        
        if( cg.frametime < 0 )
        {
            cg.frametime = 0;
        }
        
        cg.oldTime = cg.time;
        idCGameDraw::AddLagometerFrameInfo( );
    }
    
    if( cg_timescale.value != cg_timescaleFadeEnd.value )
    {
        if( cg_timescale.value < cg_timescaleFadeEnd.value )
        {
            cg_timescale.value += cg_timescaleFadeSpeed.value * ( ( float32 )cg.frametime ) / 1000;
            if( cg_timescale.value > cg_timescaleFadeEnd.value )
            {
                cg_timescale.value = cg_timescaleFadeEnd.value;
            }
        }
        else
        {
            cg_timescale.value -= cg_timescaleFadeSpeed.value * ( ( float32 )cg.frametime ) / 1000;
            if( cg_timescale.value < cg_timescaleFadeEnd.value )
            {
                cg_timescale.value = cg_timescaleFadeEnd.value;
            }
        }
        
        if( cg_timescaleFadeSpeed.value )
        {
            trap_Cvar_Set( "timescale", va( "%f", cg_timescale.value ) );
        }
    }
    
    // actually issue the rendering calls
    idCGameDraw::DrawActive( stereoView );
    
    if( cg_stats.integer )
    {
        Printf( "cg.clientFrame:%i\n", cg.clientFrame );
    }
}
