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
// File name:   cgame_ents.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description: present snapshot entities, happens every single frame
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <cgame/cgame_precompiled.hpp>

/*
===============
idCGameEnts::idCGameEnts
===============
*/
idCGameEnts::idCGameEnts( void )
{
}

/*
===============
idCGameEnts::~idCGameEnts
===============
*/
idCGameEnts::~idCGameEnts( void )
{
}

/*
======================
idCGameEnts::DrawBoxFace

Draws a bounding box face
======================
*/
void idCGameEnts::DrawBoxFace( vec3_t a, vec3_t b, vec3_t c, vec3_t d )
{
    polyVert_t verts[ 4 ];
    vec4_t color = { 255.0f, 0.0f, 0.0f, 128.0f };
    
    VectorCopy( d, verts[ 0 ].xyz );
    verts[ 0 ].st[ 0 ] = 1;
    verts[ 0 ].st[ 1 ] = 1;
    Vector4Copy( color, verts[ 0 ].modulate );
    
    VectorCopy( c, verts[ 1 ].xyz );
    verts[ 1 ].st[ 0 ] = 1;
    verts[ 1 ].st[ 1 ] = 0;
    Vector4Copy( color, verts[ 1 ].modulate );
    
    VectorCopy( b, verts[ 2 ].xyz );
    verts[ 2 ].st[ 0 ] = 0;
    verts[ 2 ].st[ 1 ] = 0;
    Vector4Copy( color, verts[ 2 ].modulate );
    
    VectorCopy( a, verts[ 3 ].xyz );
    verts[ 3 ].st[ 0 ] = 0;
    verts[ 3 ].st[ 1 ] = 1;
    Vector4Copy( color, verts[ 3 ].modulate );
    
    trap_R_AddPolyToScene( cgs.media.outlineShader, 4, verts );
}

/*
======================
idCGameEnts::DrawBoundingBox

Draws a bounding box
======================
*/
void idCGameEnts::DrawBoundingBox( vec3_t origin, vec3_t mins, vec3_t maxs )
{
    vec3_t ppp, mpp, mmp, pmp;
    vec3_t mmm, pmm, ppm, mpm;
    
    ppp[ 0 ] = origin[ 0 ] + maxs[ 0 ];
    ppp[ 1 ] = origin[ 1 ] + maxs[ 1 ];
    ppp[ 2 ] = origin[ 2 ] + maxs[ 2 ];
    
    mpp[ 0 ] = origin[ 0 ] + mins[ 0 ];
    mpp[ 1 ] = origin[ 1 ] + maxs[ 1 ];
    mpp[ 2 ] = origin[ 2 ] + maxs[ 2 ];
    
    mmp[ 0 ] = origin[ 0 ] + mins[ 0 ];
    mmp[ 1 ] = origin[ 1 ] + mins[ 1 ];
    mmp[ 2 ] = origin[ 2 ] + maxs[ 2 ];
    
    pmp[ 0 ] = origin[ 0 ] + maxs[ 0 ];
    pmp[ 1 ] = origin[ 1 ] + mins[ 1 ];
    pmp[ 2 ] = origin[ 2 ] + maxs[ 2 ];
    
    ppm[ 0 ] = origin[ 0 ] + maxs[ 0 ];
    ppm[ 1 ] = origin[ 1 ] + maxs[ 1 ];
    ppm[ 2 ] = origin[ 2 ] + mins[ 2 ];
    
    mpm[ 0 ] = origin[ 0 ] + mins[ 0 ];
    mpm[ 1 ] = origin[ 1 ] + maxs[ 1 ];
    mpm[ 2 ] = origin[ 2 ] + mins[ 2 ];
    
    mmm[ 0 ] = origin[ 0 ] + mins[ 0 ];
    mmm[ 1 ] = origin[ 1 ] + mins[ 1 ];
    mmm[ 2 ] = origin[ 2 ] + mins[ 2 ];
    
    pmm[ 0 ] = origin[ 0 ] + maxs[ 0 ];
    pmm[ 1 ] = origin[ 1 ] + mins[ 1 ];
    pmm[ 2 ] = origin[ 2 ] + mins[ 2 ];
    
    //phew!
    DrawBoxFace( ppp, mpp, mmp, pmp );
    DrawBoxFace( ppp, pmp, pmm, ppm );
    DrawBoxFace( mpp, ppp, ppm, mpm );
    DrawBoxFace( mmp, mpp, mpm, mmm );
    DrawBoxFace( pmp, mmp, mmm, pmm );
    DrawBoxFace( mmm, mpm, ppm, pmm );
}

/*
======================
idCGameEnts::PositionEntityOnTag

Modifies the entities position and axis by the given
tag location
======================
*/
void idCGameEnts::PositionEntityOnTag( refEntity_t* entity, const refEntity_t* parent, qhandle_t parentModel, valueType* tagName )
{
    sint i;
    orientation_t lerped;
    
    // lerp the tag
    trap_R_LerpTag( &lerped, parentModel, parent->oldframe, parent->frame, 1.0 - parent->backlerp, tagName );
    
    // FIXME: allow origin offsets along tag?
    VectorCopy( parent->origin, entity->origin );
    
    for( i = 0; i < 3; i++ )
    {
        VectorMA( entity->origin, lerped.origin[i], parent->axis[i], entity->origin );
    }
    
    // had to cast away the const to avoid compiler problems...
    AxisMultiply( lerped.axis, ( ( refEntity_t* )parent )->axis, entity->axis );
    entity->backlerp = parent->backlerp;
}

/*
======================
idCGameEnts::PositionRotatedEntityOnTag

Modifies the entities position and axis by the given
tag location
======================
*/
void idCGameEnts::PositionRotatedEntityOnTag( refEntity_t* entity, const refEntity_t* parent, qhandle_t parentModel, valueType* tagName )
{
    sint  i;
    orientation_t lerped;
    vec3_t tempAxis[ 3 ];
    
    //AxisClear( entity->axis );
    // lerp the tag
    trap_R_LerpTag( &lerped, parentModel, parent->oldframe, parent->frame, 1.0 - parent->backlerp, tagName );
    
    // FIXME: allow origin offsets along tag?
    VectorCopy( parent->origin, entity->origin );
    
    for( i = 0; i < 3; i++ )
    {
        VectorMA( entity->origin, lerped.origin[i], parent->axis[i], entity->origin );
    }
    
    // had to cast away the const to avoid compiler problems...
    AxisMultiply( entity->axis, lerped.axis, tempAxis );
    AxisMultiply( tempAxis, ( ( refEntity_t* )parent )->axis, entity->axis );
}

/*
==========================================================================
FUNCTIONS CALLED EACH FRAME
==========================================================================
*/

/*
======================
idCGameEnts::SetEntitySoundPosition

Also called by event processing code
======================
*/
void idCGameEnts::SetEntitySoundPosition( centity_t* cent )
{
    if( cent->currentState.solid == SOLID_BMODEL )
    {
        vec3_t origin;
        float32* v;
        
        v = cgs.inlineModelMidpoints[ cent->currentState.modelindex ];
        VectorAdd( cent->lerpOrigin, v, origin );
        trap_S_UpdateEntityPosition( cent->currentState.number, origin );
    }
    else
    {
        trap_S_UpdateEntityPosition( cent->currentState.number, cent->lerpOrigin );
    }
}

/*
==================
idCGameEnts::EntityEffects

Add continuous entity effects, like local entity emission and lighting
==================
*/
void idCGameEnts::EntityEffects( centity_t* cent )
{
    sint i;
    
    // update sound origins
    SetEntitySoundPosition( cent );
    
    // update sound origins
    SetEntitySoundPosition( cent );
    
    // add loop sound
    if( cent->currentState.loopSound )
    {
        if( cent->currentState.eType != ET_SPEAKER )
        {
            trap_S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, vec3_origin, cgs.gameSounds[cent->currentState.loopSound] );
        }
        else
        {
            trap_S_AddRealLoopingSound( cent->currentState.number, cent->lerpOrigin, vec3_origin, cgs.gameSounds[cent->currentState.loopSound] );
        }
    }
    else if( cent->soundTime )
    {
        cent->soundTime = 0;
    }
    
    // constant light glow
    if( cent->currentState.constantLight )
    {
        sint   cl;
        sint   i, r, g, b;
        
        cl = cent->currentState.constantLight;
        r = cl & 255;
        g = ( cl >> 8 ) & 255;
        b = ( cl >> 16 ) & 255;
        i = ( ( cl >> 24 ) & 255 ) * 4;
        trap_R_AddLightToScene( cent->lerpOrigin, i, r, g, b );
    }
    
    if( idCGameTrails::IsTrailSystemValid( &cent->muzzleTS ) )
    {
        //FIXME hack to prevent tesla trails reaching too far
        if( cent->currentState.eType == ET_BUILDABLE )
        {
            vec3_t  front, back;
            
            idCGameAttachment::AttachmentPoint( &cent->muzzleTS->frontAttachment, front );
            idCGameAttachment::AttachmentPoint( &cent->muzzleTS->backAttachment, back );
            
            if( Distance( front, back ) > ( TESLAGEN_RANGE * M_ROOT3 ) )
            {
                idCGameTrails::DestroyTrailSystem( &cent->muzzleTS );
            }
        }
        
        if( cg.time > cent->muzzleTSDeathTime && idCGameTrails::IsTrailSystemValid( &cent->muzzleTS ) )
        {
            idCGameTrails::DestroyTrailSystem( &cent->muzzleTS );
        }
    }
    
    
    if( cent->currentState.eType == ET_PLAYER )
    {
        centity_t* pcent = cent;
        
        // predicted entity doesn't have local cgame vars
        if( cent == &cg.predictedPlayerEntity )
        {
            pcent = &cg_entities[cg.clientNum];
        }
        
        for( i = 0; i <= 2; i++ )
        {
            if( idCGameTrails::IsTrailSystemValid( &pcent->level2ZapTS[ i ] ) )
            {
                vec3_t front, back;
                
                idCGameAttachment::AttachmentPoint( &pcent->level2ZapTS[ i ]->frontAttachment, front );
                idCGameAttachment::AttachmentPoint( &pcent->level2ZapTS[ i ]->backAttachment, back );
                
                if( cg.time - pcent->level2ZapTime > 100 || Distance( front, back ) > LEVEL2_AREAZAP_CUTOFF )
                {
                    idCGameTrails::DestroyTrailSystem( &pcent->level2ZapTS[ i ] );
                }
            }
        }
    }
}

/*
==================
idCGameEnts::General
==================
*/
void idCGameEnts::General( centity_t* cent )
{
    refEntity_t ent;
    entityState_t* s1;
    
    s1 = &cent->currentState;
    
    // if set to invisible, skip
    if( !s1->modelindex )
    {
        return;
    }
    
    ::memset( &ent, 0, sizeof( ent ) );
    
    // set frame
    ent.frame = s1->frame;
    ent.oldframe = ent.frame;
    ent.backlerp = 0;
    
    VectorCopy( cent->lerpOrigin, ent.origin );
    VectorCopy( cent->lerpOrigin, ent.oldorigin );
    
    if( s1->solid == SOLID_BMODEL )
    {
        ent.hModel = cgs.inlineDrawModel[s1->modelindex];
    }
    else
    {
        ent.hModel = cgs.gameModels[s1->modelindex];
    }
    
    ent.hModel = cgs.gameModels[ s1->modelindex ];
    
    // player model
    if( s1->number == cg.snap->ps.clientNum )
    {
        // only draw from mirrors
        ent.renderfx |= RF_THIRD_PERSON;
    }
    
    // convert angles to axis
    AnglesToAxis( cent->lerpAngles, ent.axis );
    
    // add to refresh list
    trap_R_AddRefEntityToScene( &ent );
    
    if( s1->modelindex2 )
    {
        ent.skinNum = 0;
        ent.hModel = cgs.gameModels[s1->modelindex2];
        trap_R_AddRefEntityToScene( &ent );
    }
}

/*
==================
idCGameEnts::Speaker

Speaker entities can automatically play sounds
==================
*/
void idCGameEnts::Speaker( centity_t* cent )
{
    if( !cent->currentState.clientNum )
    {
        // FIXME: use something other than clientNum...
        return;   // not auto triggering
    }
    
    if( cg.time < cent->miscTime )
    {
        return;
    }
    
    trap_S_StartSound( nullptr, cent->currentState.number, CHAN_ITEM, cgs.gameSounds[ cent->currentState.eventParm ] );
    
    //  ent->s.frame = ent->wait * 10;
    //  ent->s.clientNum = ent->random * 10;
    cent->miscTime = cg.time + cent->currentState.frame * 100 + cent->currentState.clientNum * 100 * crandom( );
}

/*
===============
idCGameEnts::LaunchMissile
===============
*/
void idCGameEnts::LaunchMissile( centity_t* cent )
{
    entityState_t* es;
    const weaponInfo_t* wi;
    particleSystem_t* ps;
    trailSystem_t* ts;
    weapon_t weapon;
    weaponMode_t weaponMode;
    
    es = &cent->currentState;
    
    weapon = ( weapon_t )es->weapon;
    
    if( weapon > WP_NUM_WEAPONS )
    {
        weapon = WP_NONE;
    }
    
    wi = &cg_weapons[ weapon ];
    weaponMode = ( weaponMode_t )es->generic1;
    
    if( wi->wim[ weaponMode ].missileParticleSystem )
    {
        ps = idCGameParticles::SpawnNewParticleSystem( wi->wim[ weaponMode ].missileParticleSystem );
        
        if( idCGameParticles::IsParticleSystemValid( &ps ) )
        {
            idCGameAttachment::SetAttachmentCent( &ps->attachment, cent );
            idCGameAttachment::AttachToCent( &ps->attachment );
            ps->charge = es->torsoAnim;
        }
    }
    
    if( wi->wim[ weaponMode ].missileTrailSystem )
    {
        ts = idCGameTrails::SpawnNewTrailSystem( wi->wim[ weaponMode ].missileTrailSystem );
        
        if( idCGameTrails::IsTrailSystemValid( &ts ) )
        {
            idCGameAttachment::SetAttachmentCent( &ts->frontAttachment, cent );
            idCGameAttachment::AttachToCent( &ts->frontAttachment );
        }
    }
}

/*
===============
idCGameEnts::Missile
===============
*/
void idCGameEnts::Missile( centity_t* cent )
{
    refEntity_t ent;
    entityState_t* es;
    const weaponInfo_t* wi;
    weapon_t weapon;
    weaponMode_t weaponMode;
    const weaponInfoMode_t* wim;
    
    es = &cent->currentState;
    
    weapon = ( weapon_t )es->weapon;
    
    if( weapon > WP_NUM_WEAPONS )
    {
        weapon = WP_NONE;
    }
    
    wi = &cg_weapons[ weapon ];
    weaponMode = ( weaponMode_t )es->generic1;
    
    wim = &wi->wim[ weaponMode ];
    
    // calculate the axis
    VectorCopy( es->angles, cent->lerpAngles );
    
    // add dynamic light
    if( wim->missileDlight )
    {
        trap_R_AddLightToScene( cent->lerpOrigin, wim->missileDlight, wim->missileDlightColor[ 0 ], wim->missileDlightColor[ 1 ], wim->missileDlightColor[ 2 ] );
    }
    
    // add missile sound
    if( wim->missileSound )
    {
        vec3_t  velocity;
        
        bggame->EvaluateTrajectoryDelta( &cent->currentState.pos, cg.time, velocity );
        
        trap_S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, velocity, wim->missileSound );
    }
    
    // create the render entity
    ::memset( &ent, 0, sizeof( ent ) );
    VectorCopy( cent->lerpOrigin, ent.origin );
    VectorCopy( cent->lerpOrigin, ent.oldorigin );
    
    if( wim->usesSpriteMissle )
    {
        ent.reType = RT_SPRITE;
        ent.radius = wim->missileSpriteSize + wim->missileSpriteCharge * es->torsoAnim;
        ent.rotation = 0;
        ent.customShader = wim->missileSprite;
        ent.shaderRGBA[ 0 ] = 0xFF;
        ent.shaderRGBA[ 1 ] = 0xFF;
        ent.shaderRGBA[ 2 ] = 0xFF;
        ent.shaderRGBA[ 3 ] = 0xFF;
    }
    else
    {
        ent.hModel = wim->missileModel;
        ent.renderfx = wim->missileRenderfx;// | RF_NOSHADOW;
        
        // convert direction of travel into axis
        if( VectorNormalize2( es->pos.trDelta, ent.axis[0] ) == 0 )
        {
            ent.axis[0][2] = 1;
        }
        
        // spin as it moves
        if( es->pos.trType != TR_STATIONARY && wim->missileRotates )
        {
            RotateAroundDirection( ent.axis, cg.time / 4 );
        }
        else
        {
            RotateAroundDirection( ent.axis, es->time );
        }
        
        if( wim->missileAnimates )
        {
            sint timeSinceStart = cg.time - es->time;
            
            if( wim->missileAnimLooping )
            {
                ent.frame = wim->missileAnimStartFrame + ( sint )( ( timeSinceStart / 1000.0f ) * wim->missileAnimFrameRate ) % wim->missileAnimNumFrames;
            }
            else
            {
                ent.frame = wim->missileAnimStartFrame + ( sint )( ( timeSinceStart / 1000.0f ) * wim->missileAnimFrameRate );
                
                if( ent.frame > ( wim->missileAnimStartFrame + wim->missileAnimNumFrames ) )
                {
                    ent.frame = wim->missileAnimStartFrame + wim->missileAnimNumFrames;
                }
            }
        }
    }
    
    //only refresh if there is something to display
    if( wim->missileSprite || wim->missileModel )
    {
        trap_R_AddRefEntityToScene( &ent );
    }
}

/*
===============
idCGameEnts::Mover
===============
*/
void idCGameEnts::Mover( centity_t* cent )
{
    refEntity_t ent;
    entityState_t* s1;
    
    s1 = &cent->currentState;
    
    // create the render entity
    ::memset( &ent, 0, sizeof( ent ) );
    VectorCopy( cent->lerpOrigin, ent.origin );
    VectorCopy( cent->lerpOrigin, ent.oldorigin );
    AnglesToAxis( cent->lerpAngles, ent.axis );
    
    //Dushan - let movers cast shadows
    //ent.renderfx = RF_NOSHADOW;
    
    // flicker between two skins (FIXME?)
    ent.skinNum = ( cg.time >> 6 ) & 1;
    
    // get the model, either as a bmodel or a modelindex
    if( s1->solid == SOLID_BMODEL )
    {
        ent.hModel = cgs.inlineDrawModel[s1->modelindex];
    }
    else
    {
        ent.hModel = cgs.gameModels[s1->modelindex];
    }
    
    // add to refresh list
    trap_R_AddRefEntityToScene( &ent );
    
    // add the secondary model
    if( s1->modelindex2 )
    {
        ent.skinNum = 0;
        ent.hModel = cgs.gameModels[ s1->modelindex2 ];
        trap_R_AddRefEntityToScene( &ent );
    }
    
}

/*
===============
idCGameEnts::Beam

Also called as an event
===============
*/
void idCGameEnts::Beam( centity_t* cent )
{
    refEntity_t ent;
    entityState_t* s1;
    
    s1 = &cent->currentState;
    
    // create the render entity
    ::memset( &ent, 0, sizeof( ent ) );
    
    VectorCopy( s1->pos.trBase, ent.origin );
    VectorCopy( s1->origin2, ent.oldorigin );
    
    AxisClear( ent.axis );
    ent.reType = RT_BEAM;
    
    //ent.renderfx = RF_NOSHADOW;
    
    // add to refresh list
    trap_R_AddRefEntityToScene( &ent );
}

/*
===============
idCGameEnts::Portal
===============
*/
void idCGameEnts::Portal( centity_t* cent )
{
    refEntity_t ent;
    entityState_t* s1;
    
    s1 = &cent->currentState;
    
    // create the render entity
    ::memset( &ent, 0, sizeof( ent ) );
    VectorCopy( cent->lerpOrigin, ent.origin );
    VectorCopy( s1->origin2, ent.oldorigin );
    ByteToDir( s1->eventParm, ent.axis[ 0 ] );
    PerpendicularVector( ent.axis[ 1 ], ent.axis[ 0 ] );
    
    // negating this tends to get the directions like they want
    // we really should have a camera roll value
    VectorSubtract( vec3_origin, ent.axis[ 1 ], ent.axis[ 1 ] );
    
    CrossProduct( ent.axis[ 0 ], ent.axis[ 1 ], ent.axis[ 2 ] );
    ent.reType = RT_PORTALSURFACE;
    ent.oldframe = s1->misc;
    ent.frame = s1->frame; // rotation speed
    ent.skinNum = s1->clientNum / 256.0 * 360; // roll offset
    
    // add to refresh list
    trap_R_AddRefEntityToScene( &ent );
}

/*
=========================
idCGameEnts::LightFlare
=========================
*/
#define SETBOUNDS(v1,v2,r)  ((v1)[0]=(-r/2),(v1)[1]=(-r/2),(v1)[2]=(-r/2),\
                             (v2)[0]=(r/2),(v2)[1]=(r/2),(v2)[2]=(r/2))
#define RADIUSSTEP          0.5f

#define FLARE_OFF 0
#define FLARE_NOFADE 1
#define FLARE_TIMEFADE 2
#define FLARE_REALFADE 3
void idCGameEnts::LightFlare( centity_t* cent )
{
    refEntity_t flare;
    entityState_t* es;
    vec3_t forward, delta;
    trace_t tr;
    vec3_t mins, maxs, start, end;
    float32 len, maxAngle, srcRadius, srLocal, ratio = 1.0f;
    sint entityNum;
    
    es = &cent->currentState;
    
    if( cg.renderingThirdPerson )
    {
        entityNum = MAGIC_TRACE_HACK;
    }
    else
    {
        entityNum = cg.predictedPlayerState.clientNum;
    }
    
    //don't draw light flares
    if( cg_lightFlare.integer == FLARE_OFF )
    {
        return;
    }
    
    //flare is "off"
    if( es->eFlags & EF_NODRAW )
    {
        return;
    }
    
    idCGamePredict::Trace( &tr, cg.refdef.vieworg, nullptr, nullptr, es->angles2, entityNum, MASK_SHOT );
    
    //if there is no los between the view and the flare source
    //it definately cannot be seen
    if( tr.fraction < 1.0f || tr.allsolid )
    {
        return;
    }
    
    ::memset( &flare, 0, sizeof( flare ) );
    flare.reType = RT_SPRITE;
    flare.customShader = cgs.gameShaders[ es->modelindex ];
    flare.shaderRGBA[ 0 ] = 0xFF;
    flare.shaderRGBA[ 1 ] = 0xFF;
    flare.shaderRGBA[ 2 ] = 0xFF;
    flare.shaderRGBA[ 3 ] = 0xFF;
    
    //flares always drawn before the rest of the scene
    flare.renderfx |= RF_DEPTHHACK;
    
    //bunch of geometry
    AngleVectors( es->angles, forward, nullptr, nullptr );
    VectorCopy( cent->lerpOrigin, flare.origin );
    VectorSubtract( flare.origin, cg.refdef.vieworg, delta );
    len = VectorLength( delta );
    VectorNormalize( delta );
    
    //flare is too close to camera to be drawn
    if( len < es->generic1 )
    {
        return;
    }
    
    //don't bother for flares behind the view plane
    if( DotProduct( delta, cg.refdef.viewaxis[0] ) < 0.0 )
    {
        return;
    }
    
    //only recalculate radius and ratio every three frames
    if( !( cg.clientFrame % 2 ) )
    {
        //can only see the flare when in front of it
        flare.radius = len / es->origin2[ 0 ];
        
        if( es->origin2[2] == 0 )
        {
            srcRadius = srLocal = flare.radius / 2.0f;
        }
        else
        {
            srcRadius = srLocal = len / es->origin2[2];
        }
        
        maxAngle = es->origin2[ 1 ];
        
        if( maxAngle > 0.0f )
        {
            float32 radiusMod = 1.0f - ( 180.0f - RAD2DEG( acos( DotProduct( delta, forward ) ) ) ) / maxAngle;
            
            if( radiusMod < 0.0f )
            {
                radiusMod = 0.0f;
            }
            
            flare.radius *= radiusMod;
        }
        
        if( flare.radius < 0.0f )
        {
            flare.radius = 0.0f;
        }
        
        VectorMA( flare.origin, -flare.radius, delta, end );
        VectorMA( cg.refdef.vieworg, flare.radius, delta, start );
        
        if( cg_lightFlare.integer == FLARE_REALFADE )
        {
            //"correct" flares
            idCGamePredict::BiSphereTrace( &tr, cg.refdef.vieworg, end, 1.0f, srcRadius, entityNum, MASK_SHOT );
            
            if( tr.fraction < 1.0f )
            {
                ratio = tr.lateralFraction;
            }
            else
            {
                ratio = 1.0f;
            }
        }
        else if( cg_lightFlare.integer == FLARE_TIMEFADE )
        {
            //draw timed flares
            SETBOUNDS( mins, maxs, srcRadius );
            idCGamePredict::Trace( &tr, start, mins, maxs, end, entityNum, MASK_SHOT );
            
            if( ( tr.fraction < 1.0f || tr.startsolid ) && cent->lfs.status )
            {
                cent->lfs.status = false;
                cent->lfs.lastTime = cg.time;
            }
            else if( ( tr.fraction == 1.0f && !tr.startsolid ) && !cent->lfs.status )
            {
                cent->lfs.status = true;
                cent->lfs.lastTime = cg.time;
            }
            
            //fade flare up
            if( cent->lfs.status )
            {
                if( cent->lfs.lastTime + es->time > cg.time )
                {
                    ratio = ( float32 )( cg.time - cent->lfs.lastTime ) / es->time;
                }
            }
            
            //fade flare down
            if( !cent->lfs.status )
            {
                if( cent->lfs.lastTime + es->time > cg.time )
                {
                    ratio = ( float32 )( cg.time - cent->lfs.lastTime ) / es->time;
                    ratio = 1.0f - ratio;
                }
                else
                {
                    ratio = 0.0f;
                }
            }
        }
        else if( cg_lightFlare.integer == FLARE_NOFADE )
        {
            //draw nofade flares
            SETBOUNDS( mins, maxs, srcRadius );
            idCGamePredict::Trace( &tr, start, mins, maxs, end, entityNum, MASK_SHOT );
            
            //flare source occluded
            if( ( tr.fraction < 1.0f || tr.startsolid ) )
            {
                ratio = 0.0f;
            }
        }
    }
    else
    {
        ratio = cent->lfs.lastRatio;
        flare.radius = cent->lfs.lastRadius;
    }
    
    cent->lfs.lastRatio  = ratio;
    cent->lfs.lastRadius = flare.radius;
    
    if( ratio < 1.0f )
    {
        flare.radius *= ratio;
        flare.shaderRGBA[ 3 ] = ( uchar8 )( ( float32 )flare.shaderRGBA[ 3 ] * ratio );
    }
    
    if( flare.radius <= 0.0f )
    {
        return;
    }
    
    trap_R_AddRefEntityToScene( &flare );
}

/*
=========================
refEntity_tAdjustPositionForMover

Also called by client movement prediction code
=========================
*/
void idCGameEnts::AdjustPositionForMover( const vec3_t in, sint moverNum, sint fromTime, sint toTime, vec3_t out )
{
    centity_t* cent;
    vec3_t oldOrigin, origin, deltaOrigin, oldAngles, angles, deltaAngles;
    
    if( moverNum <= 0 || moverNum >= ENTITYNUM_MAX_NORMAL )
    {
        VectorCopy( in, out );
        return;
    }
    
    cent = &cg_entities[ moverNum ];
    
    if( cent->currentState.eType != ET_MOVER )
    {
        VectorCopy( in, out );
        return;
    }
    
    bggame->EvaluateTrajectory( &cent->currentState.pos, fromTime, oldOrigin );
    bggame->EvaluateTrajectory( &cent->currentState.apos, fromTime, oldAngles );
    
    bggame->EvaluateTrajectory( &cent->currentState.pos, toTime, origin );
    bggame->EvaluateTrajectory( &cent->currentState.apos, toTime, angles );
    
    VectorSubtract( origin, oldOrigin, deltaOrigin );
    VectorSubtract( angles, oldAngles, deltaAngles );
    
    VectorAdd( in, deltaOrigin, out );
    
    // FIXME: origin change when on a rotating object
}

/*
=============================
idCGameEnts::InterpolateEntityPosition
=============================
*/
void idCGameEnts::InterpolateEntityPosition( centity_t* cent )
{
    vec3_t current, next;
    float32 f;
    
    // it would be an internal error to find an entity that interpolates without
    // a snapshot ahead of the current one
    if( cg.nextSnap == nullptr )
    {
        Error( "idCGameLocal::InterpoateEntityPosition: cg.nextSnap == nullptr" );
    }
    
    f = cg.frameInterpolation;
    
    // this will linearize a sine or parabolic curve, but it is important
    // to not extrapolate player positions if more recent data is available
    bggame->EvaluateTrajectory( &cent->currentState.pos, cg.snap->serverTime, current );
    bggame->EvaluateTrajectory( &cent->nextState.pos, cg.nextSnap->serverTime, next );
    
    cent->lerpOrigin[ 0 ] = current[ 0 ] + f * ( next[ 0 ] - current[ 0 ] );
    cent->lerpOrigin[ 1 ] = current[ 1 ] + f * ( next[ 1 ] - current[ 1 ] );
    cent->lerpOrigin[ 2 ] = current[ 2 ] + f * ( next[ 2 ] - current[ 2 ] );
    
    bggame->EvaluateTrajectory( &cent->currentState.apos, cg.snap->serverTime, current );
    bggame->EvaluateTrajectory( &cent->nextState.apos, cg.nextSnap->serverTime, next );
    
    cent->lerpAngles[ 0 ] = LerpAngle( current[ 0 ], next[ 0 ], f );
    cent->lerpAngles[ 1 ] = LerpAngle( current[ 1 ], next[ 1 ], f );
    cent->lerpAngles[ 2 ] = LerpAngle( current[ 2 ], next[ 2 ], f );
    
}

/*
===============
idCGameEnts::CalcEntityLerpPositions
===============
*/
void idCGameEnts::CalcEntityLerpPositions( centity_t* cent )
{
    // this will be set to how far forward projectiles will be extrapolated
    sint timeshift = 0;
    
    // if this player does not want to see extrapolated players
    if( !cg_smoothClients.integer )
    {
        // make sure the clients use TR_INTERPOLATE
        if( cent->currentState.number < MAX_CLIENTS )
        {
            cent->currentState.pos.trType = TR_INTERPOLATE;
            cent->nextState.pos.trType = TR_INTERPOLATE;
        }
    }
    
    if( cent->interpolate && cent->currentState.pos.trType == TR_INTERPOLATE )
    {
        InterpolateEntityPosition( cent );
        return;
    }
    
    // first see if we can interpolate between two snaps for
    // linear extrapolated clients
    if( cent->interpolate && cent->currentState.pos.trType == TR_LINEAR_STOP && cent->currentState.number < MAX_CLIENTS )
    {
        InterpolateEntityPosition( cent );
        return;
    }
    
    if( cg_projectileNudge.integer > 0 && cent->currentState.eType == ET_MISSILE && !( cg.snap->ps.pm_flags & PMF_FOLLOW ) )
    {
        timeshift = cg.ping;
    }
    
    // just use the current frame and evaluate as best we can
    bggame->EvaluateTrajectory( &cent->currentState.pos, ( cg.time + timeshift ), cent->lerpOrigin );
    bggame->EvaluateTrajectory( &cent->currentState.apos, ( cg.time + timeshift ), cent->lerpAngles );
    
    if( timeshift )
    {
        trace_t tr;
        vec3_t lastOrigin;
        
        bggame->EvaluateTrajectory( &cent->currentState.pos, cg.time, lastOrigin );
        
        idCGamePredict::Trace( &tr, lastOrigin, vec3_origin, vec3_origin, cent->lerpOrigin, cent->currentState.number, MASK_SHOT );
        
        // don't let the projectile go through the floor
        if( tr.fraction < 1.0f )
        {
            VectorLerpTrem( tr.fraction, lastOrigin, cent->lerpOrigin, cent->lerpOrigin );
        }
    }
    
    // adjust for riding a mover if it wasn't rolled into the predicted
    // player state
    if( cent != &cg.predictedPlayerEntity )
    {
        AdjustPositionForMover( cent->lerpOrigin, cent->currentState.groundEntityNum, cg.snap->serverTime, cg.time, cent->lerpOrigin );
    }
}

/*
===============
idCGameEnts::CEntityPVSEnter
===============
*/
void idCGameEnts::CEntityPVSEnter( centity_t* cent )
{
    entityState_t* es = &cent->currentState;
    
    if( cg_debugPVS.integer )
    {
        Printf( "Entity %d entered PVS\n", cent->currentState.number );
    }
    
    switch( es->eType )
    {
        case ET_MISSILE:
            LaunchMissile( cent );
            break;
    }
    
    //clear any particle systems from previous uses of this centity_t
    cent->muzzlePS = nullptr;
    cent->muzzlePsTrigger = false;
    cent->jetPackPS = nullptr;
    cent->jetPackState = JPS_OFF;
    cent->buildablePS = nullptr;
    cent->entityPS = nullptr;
    cent->entityPSMissing = false;
    
    //make sure that the buildable animations are in a consistent state
    //when a buildable enters the PVS
    cent->buildableAnim = ( buildableAnimNumber_t )( cent->lerpFrame.animationNumber = BANIM_NONE );
    cent->oldBuildableAnim = ( buildableAnimNumber_t )es->legsAnim;
}

/*
===============
idCGameEnts::CEntityPVSLeave
===============
*/
void idCGameEnts::CEntityPVSLeave( centity_t* cent )
{
    if( cg_debugPVS.integer )
    {
        Printf( "Entity %d left PVS\n", cent->currentState.number );
    }
}

/*
===============
idCGameEnts::InFOV
===============
*/
bool idCGameEnts::InFOV( vec3_t spot, vec3_t from, vec3_t fromAngles, sint hFOV, sint vFOV )
{
    vec3_t	deltaVector, angles, deltaAngles;
    
    VectorSubtract( spot, from, deltaVector );
    vectoangles( deltaVector, angles );
    
    deltaAngles[PITCH] = AngleDelta( fromAngles[PITCH], angles[PITCH] );
    deltaAngles[YAW] = AngleDelta( fromAngles[YAW], angles[YAW] );
    
    if( fabs( deltaAngles[PITCH] ) <= vFOV && fabs( deltaAngles[YAW] ) <= hFOV )
    {
        return true;
    }
    
    return false;
}

/*
===============
idCGameEnts::AddCEntity
===============
*/
void idCGameEnts::AddCEntity( centity_t* cent )
{
    // event-only entities will have been dealt with already
    if( cent->currentState.eType >= ET_EVENTS )
    {
        return;
    }
    
    // calculate the current origin
    CalcEntityLerpPositions( cent );
    
    // add automatic effects
    EntityEffects( cent );
    
    switch( cent->currentState.eType )
    {
        default:
            Error( "Bad entity type: %i\n", cent->currentState.eType );
            break;
            
        case ET_INVISIBLE:
        case ET_PUSH_TRIGGER:
        case ET_TELEPORT_TRIGGER:
            break;
            
        case ET_GENERAL:
            General( cent );
            break;
            
        case ET_CORPSE:
            idCGamePlayers::Corpse( cent );
            break;
            
        case ET_PLAYER:
            idCGamePlayers::Player( cent );
            break;
            
        case ET_BUILDABLE:
            idCGameBuildable::Buildable( cent );
            break;
            
        case ET_MISSILE:
            Missile( cent );
            break;
            
        case ET_MOVER:
            Mover( cent );
            break;
            
        case ET_BEAM:
            Beam( cent );
            break;
            
        case ET_PORTAL:
            Portal( cent );
            break;
            
        case ET_SPEAKER:
            Speaker( cent );
            break;
            
        case ET_PARTICLE_SYSTEM:
            idCGameParticles::ParticleSystemEntity( cent );
            break;
            
        case ET_ANIMMAPOBJ:
            idCGameAnimMapObj::AnimMapObj( cent );
            break;
            
        case ET_MODELDOOR:
            idCGameAnimMapObj::ModelDoor( cent );
            break;
            
        case ET_LIGHTFLARE:
            LightFlare( cent );
            break;
        case ET_LOCATION:
            LinkLocation( cent );
            break;
    }
}

/*
===============
idCGameEnts::AddPacketEntities
===============
*/
void idCGameEnts::AddPacketEntities( void )
{
    sint num;
    centity_t* cent;
    playerState_t* ps;
    
    // set cg.frameInterpolation
    if( cg.nextSnap )
    {
        sint delta;
        
        delta = ( cg.nextSnap->serverTime - cg.snap->serverTime );
        
        if( delta == 0 )
        {
            cg.frameInterpolation = 0;
        }
        else
        {
            cg.frameInterpolation = ( float32 )( cg.time - cg.snap->serverTime ) / delta;
        }
    }
    else
    {
        cg.frameInterpolation = 0;  // actually, it should never be used, because
        // no entities should be marked as interpolating
    }
    
    // the auto-rotating items will all have the same axis
    cg.autoAngles[ 0 ] = 0;
    cg.autoAngles[ 1 ] = ( cg.time & 2047 ) * 360 / 2048.0;
    cg.autoAngles[ 2 ] = 0;
    
    cg.autoAnglesFast[ 0 ] = 0;
    cg.autoAnglesFast[ 1 ] = ( cg.time & 1023 ) * 360 / 1024.0f;
    cg.autoAnglesFast[ 2 ] = 0;
    
    AnglesToAxis( cg.autoAngles, cg.autoAxis );
    AnglesToAxis( cg.autoAnglesFast, cg.autoAxisFast );
    
    // generate and add the entity from the playerstate
    ps = &cg.predictedPlayerState;
    bggame->PlayerStateToEntityState( ps, &cg.predictedPlayerEntity.currentState, false );
    cg.predictedPlayerEntity.valid = true;
    AddCEntity( &cg.predictedPlayerEntity );
    
    // lerp the non-predicted value for lightning gun origins
    CalcEntityLerpPositions( &cg_entities[ cg.snap->ps.clientNum ] );
    
    // scanner
    idCGameScanner::UpdateEntityPositions( );
    
    for( num = 0; num < MAX_GENTITIES; num++ )
    {
        cg_entities[num].valid = false;
    }
    
    // add each entity sent over by the server
    for( num = 0; num < cg.snap->numEntities; num++ )
    {
        cent = &cg_entities[ cg.snap->entities[ num ].number ];
        cent->valid = true;
    }
    
    for( num = 0; num < MAX_GENTITIES; num++ )
    {
        cent = &cg_entities[ num ];
        
        if( cent->valid && !cent->oldValid )
        {
            CEntityPVSEnter( cent );
        }
        else if( !cent->valid && cent->oldValid )
        {
            CEntityPVSLeave( cent );
        }
        
        cent->oldValid = cent->valid;
    }
    
    // add each entity sent over by the server
    for( num = 0; num < cg.snap->numEntities; num++ )
    {
        cent = &cg_entities[ cg.snap->entities[ num ].number ];
        AddCEntity( cent );
    }
    
    //make an attempt at drawing bounding boxes of selected entity types
    if( cg_drawBBOX.integer )
    {
        for( num = 0; num < cg.snap->numEntities; num++ )
        {
            float32 x, zd, zu;
            vec3_t  mins, maxs;
            entityState_t* es;
            
            cent = &cg_entities[ cg.snap->entities[ num ].number ];
            es = &cent->currentState;
            
            switch( es->eType )
            {
                case ET_BUILDABLE:
                case ET_MISSILE:
                case ET_CORPSE:
                    x = ( es->solid & 255 );
                    zd = ( ( es->solid >> 8 ) & 255 );
                    zu = ( ( es->solid >> 16 ) & 255 ) - 32;
                    
                    mins[ 0 ] = mins[ 1 ] = -x;
                    maxs[ 0 ] = maxs[ 1 ] = x;
                    mins[ 2 ] = -zd;
                    maxs[ 2 ] = zu;
                    
                    DrawBoundingBox( cent->lerpOrigin, mins, maxs );
                    break;
                    
                default:
                    break;
            }
        }
    }
}

/*
===============
idCGameEnts::LinkLocation
===============
*/
void idCGameEnts::LinkLocation( centity_t* cent )
{
    centity_t* tempent;
    
    for( tempent = cg.locationHead; tempent; tempent = tempent->nextLocation )
    {
        if( tempent == cent )
        {
            return;
        }
    }
    
    cent->nextLocation = cg.locationHead;
    cg.locationHead = cent;
}
