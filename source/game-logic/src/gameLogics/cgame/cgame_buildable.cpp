////////////////////////////////////////////////////////////////////////////////////////
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
// File name:   cgame_buildable.cpp
// Version:     v1.01
// Created:
// Compilers:   Visual Studio 2017, gcc 7.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <cgame/cgame_precompiled.h>

valueType* cg_buildableSoundNames[ MAX_BUILDABLE_ANIMATIONS ] =
{
    "construct1.ogg",
    "construct2.ogg",
    "idle1.ogg",
    "idle2.ogg",
    "idle3.ogg",
    "attack1.ogg",
    "attack2.ogg",
    "spawn1.ogg",
    "spawn2.ogg",
    "pain1.ogg",
    "pain2.ogg",
    "destroy1.ogg",
    "destroy2.ogg",
    "destroyed.ogg"
};

static sfxHandle_t defaultAlienSounds[ MAX_BUILDABLE_ANIMATIONS ];
static sfxHandle_t defaultHumanSounds[ MAX_BUILDABLE_ANIMATIONS ];

/*
===============
idCGameBuildable::idCGameBuildable
===============
*/
idCGameBuildable::idCGameBuildable( void )
{
}

/*
===============
idCGameBuildable::~idCGameAttachment
===============
*/
idCGameBuildable::~idCGameBuildable( void )
{
}

/*
===================
idCGameBuildable::AlienBuildableExplosion

Generated a bunch of gibs launching out from a location
===================
*/
void idCGameBuildable::AlienBuildableExplosion( vec3_t origin, vec3_t dir )
{
    particleSystem_t* ps;
    
    trap_S_StartSound( origin, ENTITYNUM_WORLD, CHAN_AUTO, cgs.media.alienBuildableExplosion );
    
    //particle system
    ps = idCGameParticles::SpawnNewParticleSystem( cgs.media.alienBuildableDestroyedPS );
    
    if( idCGameParticles::IsParticleSystemValid( &ps ) )
    {
        idCGameAttachment::SetAttachmentPoint( &ps->attachment, origin );
        idCGameParticles::SetParticleSystemNormal( ps, dir );
        idCGameAttachment::AttachToPoint( &ps->attachment );
    }
}

/*
=================
idCGameBuildable::HumanBuildableExplosion

Called for human buildables as they are destroyed
=================
*/
void idCGameBuildable::HumanBuildableExplosion( vec3_t origin, vec3_t dir )
{
    particleSystem_t* ps;
    
    trap_S_StartSound( origin, ENTITYNUM_WORLD, CHAN_AUTO, cgs.media.humanBuildableExplosion );
    
    //particle system
    ps = idCGameParticles::SpawnNewParticleSystem( cgs.media.humanBuildableDestroyedPS );
    
    if( idCGameParticles::IsParticleSystemValid( &ps ) )
    {
        idCGameAttachment::SetAttachmentPoint( &ps->attachment, origin );
        idCGameParticles::SetParticleSystemNormal( ps, dir );
        idCGameAttachment::AttachToPoint( &ps->attachment );
    }
}


#define CREEP_SIZE            64.0f
#define CREEP_DISTANCE        64.0f

/*
==================
Creep
==================
*/
void idCGameBuildable::Creep( centity_t* cent )
{
    sint msec;
    float32 size, frac;
    trace_t tr;
    vec3_t temp, origin;
    sint scaleUpTime = bggame->Buildable( ( buildable_t )cent->currentState.modelindex )->buildTime;
    sint time;
    
    time = cent->currentState.time;
    
    //should the creep be growing or receding?
    if( time >= 0 )
    {
        msec = cg.time - time;
        if( msec >= 0 && msec < scaleUpTime )
        {
            frac = ( float32 )msec / scaleUpTime;
        }
        else
        {
            frac = 1.0f;
        }
    }
    else if( time < 0 )
    {
        msec = cg.time + time;
        if( msec >= 0 && msec < CREEP_SCALEDOWN_TIME )
        {
            frac = 1.0f - ( ( float32 )msec / CREEP_SCALEDOWN_TIME );
        }
        else
        {
            frac = 0.0f;
        }
    }
    
    VectorCopy( cent->currentState.origin2, temp );
    VectorScale( temp, -CREEP_DISTANCE, temp );
    VectorAdd( temp, cent->lerpOrigin, temp );
    
    idCGamePredict::Trace( &tr, cent->lerpOrigin, nullptr, nullptr, temp, cent->currentState.number, MASK_SOLID );
    
    VectorCopy( tr.endpos, origin );
    
    size = CREEP_SIZE * frac;
    
    if( size > 0.0f && tr.fraction < 1.0f )
    {
        idCGameMarks::ImpactMark( cgs.media.creepShader, origin, cent->currentState.origin2, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, false, size, true );
    }
}

/*
======================
idCGameBuildable::ParseBuildableAnimationFile

Read a configuration file containing animation counts and rates
models/buildables/hivemind/animation.cfg, etc
======================
*/
bool idCGameBuildable::ParseBuildableAnimationFile( pointer filename, buildable_t buildable )
{
    valueType* text_p;
    sint len, i;
    valueType* token;
    float32 fps;
    valueType text[ 20000 ];
    fileHandle_t f;
    animation_t* animations;
    
    animations = cg_buildables[ buildable ].animations;
    
    // load the file
    len = trap_FS_FOpenFile( filename, &f, FS_READ );
    if( len < 0 )
    {
        return false;
    }
    
    if( len == 0 || len >= sizeof( text ) - 1 )
    {
        trap_FS_FCloseFile( f );
        Printf( "File %s is %s\n", filename, len == 0 ? "empty" : "too long" );
        return false;
    }
    
    trap_FS_Read( text, len, f );
    text[ len ] = 0;
    trap_FS_FCloseFile( f );
    
    // parse the text
    text_p = text;
    
    // read information for each frame
    for( i = BANIM_NONE + 1; i < MAX_BUILDABLE_ANIMATIONS; i++ )
    {
        token = COM_Parse2( &text_p );
        if( !*token )
        {
            break;
        }
        
        animations[ i ].firstFrame = atoi( token );
        
        token = COM_Parse2( &text_p );
        if( !*token )
        {
            break;
        }
        
        animations[ i ].numFrames = atoi( token );
        animations[ i ].reversed = false;
        animations[ i ].flipflop = false;
        
        // if numFrames is negative the animation is reversed
        if( animations[ i ].numFrames < 0 )
        {
            animations[ i ].numFrames = -animations[ i ].numFrames;
            animations[ i ].reversed = true;
        }
        
        token = COM_Parse2( &text_p );
        if( !*token )
        {
            break;
        }
        
        animations[i].loopFrames = atoi( token );
        
        token = COM_Parse2( &text_p );
        if( !*token )
        {
            break;
        }
        
        fps = atof( token );
        if( fps == 0 )
        {
            fps = 1;
        }
        
        animations[ i ].frameLerp = 1000 / fps;
        animations[ i ].initialLerp = 1000 / fps;
    }
    
    if( i != MAX_BUILDABLE_ANIMATIONS )
    {
        Printf( "Error parsing animation file: %s\n", filename );
        return false;
    }
    
    return true;
}

/*
======================
idCGameBuildable::ParseBuildableSoundFile

Read a configuration file containing sound properties
sound/buildables/hivemind/sound.cfg, etc
======================
*/
bool idCGameBuildable::ParseBuildableSoundFile( pointer filename, buildable_t buildable )
{
    valueType* text_p;
    sint len, i;
    valueType* token;
    valueType text[ 20000 ];
    fileHandle_t f;
    sound_t* sounds;
    
    sounds = cg_buildables[ buildable ].sounds;
    
    // load the file
    len = trap_FS_FOpenFile( filename, &f, FS_READ );
    if( len < 0 )
    {
        return false;
    }
    
    if( len == 0 || len >= sizeof( text ) - 1 )
    {
        trap_FS_FCloseFile( f );
        Printf( "File %s is %s\n", filename, len == 0 ? "empty" : "too long" );
        return false;
    }
    
    trap_FS_Read( text, len, f );
    text[len] = 0;
    trap_FS_FCloseFile( f );
    
    // parse the text
    text_p = text;
    
    // read information for each frame
    for( i = BANIM_NONE + 1; i < MAX_BUILDABLE_ANIMATIONS; i++ )
    {
        token = COM_Parse2( &text_p );
        if( !*token )
        {
            break;
        }
        
        sounds[ i ].enabled = atoi( token );
        
        token = COM_Parse2( &text_p );
        if( !*token )
        {
            break;
        }
        
        sounds[ i ].looped = atoi( token );
    }
    
    if( i != MAX_BUILDABLE_ANIMATIONS )
    {
        Printf( "Error parsing sound file: %s\n", filename );
        return false;
    }
    
    return true;
}
/*
===============
idCGameBuildable::InitBuildables

Initialises the animation db
===============
*/
void idCGameBuildable::InitBuildables( void )
{
    valueType filename[ MAX_QPATH ];
    valueType soundfile[ MAX_QPATH ];
    valueType* buildableName;
    valueType* modelFile;
    sint i, j;
    fileHandle_t f;
    
    ::memset( cg_buildables, 0, sizeof( cg_buildables ) );
    
    //default sounds
    for( j = BANIM_NONE + 1; j < MAX_BUILDABLE_ANIMATIONS; j++ )
    {
        strcpy( soundfile, cg_buildableSoundNames[ j - 1 ] );
        
        Com_sprintf( filename, sizeof( filename ), "sound/buildables/alien/%s", soundfile );
        defaultAlienSounds[ j ] = trap_S_RegisterSound( filename );
        
        Com_sprintf( filename, sizeof( filename ), "sound/buildables/human/%s", soundfile );
        defaultHumanSounds[ j ] = trap_S_RegisterSound( filename );
    }
    
    cg.buildablesFraction = 0.0f;
    
    for( i = BA_NONE + 1; i < BA_NUM_BUILDABLES; i++ )
    {
        buildableName = bggame->Buildable( ( buildable_t )i )->name;
        
        //animation.cfg
        Com_sprintf( filename, sizeof( filename ), "models/buildables/%s/animation.cfg", buildableName );
        if( !ParseBuildableAnimationFile( filename, ( buildable_t )i ) )
        {
            Com_Printf( S_COLOR_YELLOW "WARNING: failed to load animation file %s\n", filename );
        }
        
        //sound.cfg
        Com_sprintf( filename, sizeof( filename ), "sound/buildables/%s/sound.cfg", buildableName );
        if( !ParseBuildableSoundFile( filename, ( buildable_t )i ) )
        {
            Com_Printf( S_COLOR_YELLOW "WARNING: failed to load sound file %s\n", filename );
        }
        
        //models
        for( j = 0; j <= 3; j++ )
        {
            modelFile = bggame->BuildableConfig( ( buildable_t )i )->models[ j ];
            if( strlen( modelFile ) > 0 )
            {
                cg_buildables[i].models[j] = trap_R_RegisterModel( modelFile );
            }
        }
        
        //sounds
        for( j = BANIM_NONE + 1; j < MAX_BUILDABLE_ANIMATIONS; j++ )
        {
            strcpy( soundfile, cg_buildableSoundNames[ j - 1 ] );
            Com_sprintf( filename, sizeof( filename ), "sound/buildables/%s/%s", buildableName, soundfile );
            
            if( cg_buildables[ i ].sounds[ j ].enabled )
            {
                if( trap_FS_FOpenFile( filename, &f, FS_READ ) > 0 )
                {
                    //file exists so close it
                    trap_FS_FCloseFile( f );
                    
                    cg_buildables[ i ].sounds[ j ].sound = trap_S_RegisterSound( filename );
                }
                else
                {
                    //file doesn't exist - use default
                    if( bggame->Buildable( ( buildable_t )i )->team == TEAM_ALIENS )
                    {
                        cg_buildables[i].sounds[j].sound = defaultAlienSounds[j];
                    }
                    else
                    {
                        cg_buildables[i].sounds[j].sound = defaultHumanSounds[j];
                    }
                }
            }
        }
        
        cg.buildablesFraction = ( float32 )i / ( float32 )( BA_NUM_BUILDABLES - 1 );
        trap_UpdateScreen( );
    }
    
    cgs.media.teslaZapTS = idCGameTrails::RegisterTrailSystem( "models/buildables/tesla/zap" );
}

/*
===============
idCGameBuildable::SetBuildableLerpFrameAnimation

may include ANIM_TOGGLEBIT
===============
*/
void idCGameBuildable::SetBuildableLerpFrameAnimation( buildable_t buildable, lerpFrame_t* lf, sint newAnimation )
{
    animation_t* anim;
    
    lf->animationNumber = newAnimation;
    
    if( newAnimation < 0 || newAnimation >= MAX_BUILDABLE_ANIMATIONS )
    {
        Error( "Bad animation number: %i", newAnimation );
    }
    
    anim = &cg_buildables[ buildable ].animations[ newAnimation ];
    
    //this item has just spawned so lf->frameTime will be zero
    if( !lf->animation )
    {
        lf->frameTime = cg.time + 1000; //1 sec delay before starting the spawn anim
    }
    
    lf->animation = anim;
    lf->animationTime = lf->frameTime + anim->initialLerp;
    
    if( cg_debugAnim.integer )
    {
        Printf( "Anim: %i\n", newAnimation );
    }
}

/*
===============
RunBuildableLerpFrame

Sets cg.snap, cg.oldFrame, and cg.backlerp
cg.time should be between oldFrameTime and frameTime after exit
===============
*/
void idCGameBuildable::RunBuildableLerpFrame( centity_t* cent )
{
    buildable_t buildable = ( buildable_t )cent->currentState.modelindex;
    lerpFrame_t* lf = &cent->lerpFrame;
    buildableAnimNumber_t newAnimation = ( buildableAnimNumber_t )( cent->buildableAnim & ~( ANIM_TOGGLEBIT | ANIM_FORCEBIT ) );
    
    // see if the animation sequence is switching
    if( newAnimation != lf->animationNumber || !lf->animation )
    {
        if( cg_debugRandom.integer )
        {
            Printf( "newAnimation: %d lf->animationNumber: %d lf->animation: %d\n", newAnimation, lf->animationNumber, lf->animation );
        }
        
        SetBuildableLerpFrameAnimation( buildable, lf, newAnimation );
        
        if( !cg_buildables[ buildable ].sounds[ newAnimation ].looped && cg_buildables[ buildable ].sounds[ newAnimation ].enabled )
        {
            if( cg_debugRandom.integer )
            {
                Printf( "Sound for animation %d for a %s\n", newAnimation, bggame->Buildable( buildable )->humanName );
            }
            
            trap_S_StartSound( cent->lerpOrigin, cent->currentState.number, CHAN_AUTO, cg_buildables[buildable].sounds[newAnimation].sound );
        }
    }
    
    if( cg_buildables[buildable].sounds[lf->animationNumber].looped && cg_buildables[buildable].sounds[lf->animationNumber].enabled )
    {
        trap_S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, vec3_origin, cg_buildables[buildable].sounds[lf->animationNumber].sound );
    }
    
    idCGameAnimation::RunLerpFrame( lf, 1.0f );
    
    // animation ended
    if( lf->frameTime == cg.time )
    {
        cent->buildableAnim = ( buildableAnimNumber_t )cent->currentState.torsoAnim;
    }
}

/*
===============
BuildableAnimation
===============
*/
void idCGameBuildable::BuildableAnimation( centity_t* cent, sint* old, sint* now, float32* backLerp )
{
    entityState_t* es = &cent->currentState;
    
    //if no animation is set default to idle anim
    if( cent->buildableAnim == BANIM_NONE )
    {
        cent->buildableAnim = ( buildableAnimNumber_t )es->torsoAnim;
    }
    
    //display the first frame of the construction anim if not yet spawned
    if( !( es->eFlags & EF_B_SPAWNED ) )
    {
        animation_t* anim = &cg_buildables[ es->modelindex ].animations[ BANIM_CONSTRUCT1 ];
        
        //so that when animation starts for real it has sensible numbers
        cent->lerpFrame.oldFrameTime = cent->lerpFrame.frameTime = cent->lerpFrame.animationTime = cg.time;
        
        *old = cent->lerpFrame.oldFrame = anim->firstFrame;
        *now = cent->lerpFrame.frame = anim->firstFrame;
        *backLerp = cent->lerpFrame.backlerp = 0.0f;
        
        //ensure that an animation is triggered once the buildable has spawned
        cent->oldBuildableAnim = BANIM_NONE;
    }
    else
    {
        if( ( cent->oldBuildableAnim ^ es->legsAnim ) & ANIM_TOGGLEBIT )
        {
            if( cg_debugAnim.integer )
            {
                Printf( "%d->%d l:%d t:%d %s(%d)\n", cent->oldBuildableAnim, cent->buildableAnim, es->legsAnim, es->torsoAnim,
                        bggame->Buildable( ( buildable_t )es->modelindex )->humanName, es->number );
            }
            
            if( cent->buildableAnim == es->torsoAnim || es->legsAnim & ANIM_FORCEBIT )
            {
                cent->buildableAnim = cent->oldBuildableAnim = ( buildableAnimNumber_t )es->legsAnim;
            }
            else
            {
                cent->buildableAnim = cent->oldBuildableAnim = ( buildableAnimNumber_t )es->torsoAnim;
            }
        }
        
        RunBuildableLerpFrame( cent );
        
        *old = cent->lerpFrame.oldFrame;
        *now = cent->lerpFrame.frame;
        *backLerp = cent->lerpFrame.backlerp;
    }
}

#define TRACE_DEPTH 64.0f

/*
===============
idCGameBuildable::PositionAndOrientateBuildable
===============
*/
void idCGameBuildable::PositionAndOrientateBuildable( const vec3_t angles, const vec3_t inOrigin, const vec3_t normal, const sint skipNumber, const vec3_t mins, const vec3_t maxs, vec3_t outAxis[ 3 ], vec3_t outOrigin )
{
    vec3_t forward, start, end;
    trace_t tr;
    
    AngleVectors( angles, forward, nullptr, nullptr );
    VectorCopy( normal, outAxis[ 2 ] );
    ProjectPointOnPlane( outAxis[ 0 ], forward, outAxis[ 2 ] );
    
    if( !VectorNormalize( outAxis[ 0 ] ) )
    {
        AngleVectors( angles, nullptr, nullptr, forward );
        ProjectPointOnPlane( outAxis[ 0 ], forward, outAxis[ 2 ] );
        VectorNormalize( outAxis[ 0 ] );
    }
    
    CrossProduct( outAxis[ 0 ], outAxis[ 2 ], outAxis[ 1 ] );
    outAxis[ 1 ][ 0 ] = -outAxis[ 1 ][ 0 ];
    outAxis[ 1 ][ 1 ] = -outAxis[ 1 ][ 1 ];
    outAxis[ 1 ][ 2 ] = -outAxis[ 1 ][ 2 ];
    
    VectorMA( inOrigin, -TRACE_DEPTH, normal, end );
    VectorMA( inOrigin, 1.0f, normal, start );
    idCGamePredict::Trace( &tr, start, mins, maxs, end, skipNumber, MASK_PLAYERSOLID );
    
    if( tr.fraction == 1.0f )
    {
        //erm we missed completely - try again with a box trace
        idCGamePredict::Trace( &tr, start, mins, maxs, end, skipNumber, CONTENTS_SOLID | CONTENTS_PLAYERCLIP );
    }
    
    VectorMA( inOrigin, tr.fraction * -TRACE_DEPTH, normal, outOrigin );
}

/*
==================
GhostBuildable
==================
*/
void idCGameBuildable::GhostBuildable( buildable_t buildable )
{
    refEntity_t ent;
    playerState_t* ps;
    vec3_t angles, entity_origin, mins, maxs;
    trace_t tr;
    float32 scale;
    
    ps = &cg.predictedPlayerState;
    
    ::memset( &ent, 0, sizeof( ent ) );
    
    bggame->BuildableBoundingBox( buildable, mins, maxs );
    
    bggame->PositionBuildableRelativeToPlayer( ps, mins, maxs, idCGamePredict::Trace, entity_origin, angles, &tr );
    
    PositionAndOrientateBuildable( ps->viewangles, entity_origin, tr.plane.normal, ps->clientNum, mins, maxs, ent.axis, ent.origin );
    
    //offset on the Z axis if required
    VectorMA( ent.origin, bggame->BuildableConfig( buildable )->zOffset, tr.plane.normal, ent.origin );
    
    VectorCopy( ent.origin, ent.lightingOrigin );
    VectorCopy( ent.origin, ent.oldorigin ); // don't positionally lerp at all
    
    ent.hModel = cg_buildables[buildable].models[0];
    
    if( ps->stats[STAT_BUILDABLE] & SB_VALID_TOGGLEBIT )
    {
        ent.customShader = cgs.media.greenBuildShader;
    }
    else
    {
        ent.customShader = cgs.media.redBuildShader;
    }
    
    //rescale the model
    scale = bggame->BuildableConfig( buildable )->modelScale;
    
    if( scale != 1.0f )
    {
        VectorScale( ent.axis[ 0 ], scale, ent.axis[ 0 ] );
        VectorScale( ent.axis[ 1 ], scale, ent.axis[ 1 ] );
        VectorScale( ent.axis[ 2 ], scale, ent.axis[ 2 ] );
        
        ent.nonNormalizedAxes = true;
    }
    else
    {
        ent.nonNormalizedAxes = false;
    }
    
    // add to refresh list
    trap_R_AddRefEntityToScene( &ent );
}

/*
==================
BuildableParticleEffects
==================
*/
void idCGameBuildable::BuildableParticleEffects( centity_t* cent )
{
    entityState_t* es = &cent->currentState;
    team_t team = bggame->Buildable( ( buildable_t )es->modelindex )->team;
    sint health = es->generic1 & B_HEALTH_MASK;
    float32 healthFrac = ( float32 )health / B_HEALTH_MASK;
    
    if( !( es->eFlags & EF_B_SPAWNED ) )
    {
        return;
    }
    
    if( team == TEAM_HUMANS )
    {
        if( healthFrac < 0.33f && !idCGameParticles::IsParticleSystemValid( &cent->buildablePS ) )
        {
            cent->buildablePS = idCGameParticles::SpawnNewParticleSystem( cgs.media.humanBuildableDamagedPS );
            
            if( idCGameParticles::IsParticleSystemValid( &cent->buildablePS ) )
            {
                idCGameAttachment::SetAttachmentCent( &cent->buildablePS->attachment, cent );
                idCGameAttachment::AttachToCent( &cent->buildablePS->attachment );
            }
        }
        else if( healthFrac >= 0.33f && idCGameParticles::IsParticleSystemValid( &cent->buildablePS ) )
        {
            idCGameParticles::DestroyParticleSystem( &cent->buildablePS );
        }
    }
    else if( team == TEAM_ALIENS )
    {
        if( healthFrac < 0.33f && !idCGameParticles::IsParticleSystemValid( &cent->buildablePS ) )
        {
            cent->buildablePS = idCGameParticles::SpawnNewParticleSystem( cgs.media.alienBuildableDamagedPS );
            
            if( idCGameParticles::IsParticleSystemValid( &cent->buildablePS ) )
            {
                idCGameAttachment::SetAttachmentCent( &cent->buildablePS->attachment, cent );
                idCGameParticles::SetParticleSystemNormal( cent->buildablePS, es->origin2 );
                idCGameAttachment::AttachToCent( &cent->buildablePS->attachment );
            }
        }
        else if( healthFrac >= 0.33f && idCGameParticles::IsParticleSystemValid( &cent->buildablePS ) )
        {
            idCGameParticles::DestroyParticleSystem( &cent->buildablePS );
        }
    }
}

/*
==================
idCGameBuildable::BuildableStatusParse
==================
*/
void idCGameBuildable::BuildableStatusParse( pointer filename, buildStat_t* bs )
{
    pc_token_t token;
    sint i, handle;
    pointer s;
    float32 f;
    vec4_t c;
    
    handle = trap_PC_LoadSource( filename );
    if( !handle )
    {
        return;
    }
    
    while( 1 )
    {
        if( !trap_PC_ReadToken( handle, &token ) )
        {
            break;
        }
        if( !Q_stricmp( token.string, "frameShader" ) )
        {
            if( PC_String_Parse( handle, &s ) )
            {
                bs->frameShader = trap_R_RegisterShader( s );
            }
            continue;
        }
        else if( !Q_stricmp( token.string, "overlayShader" ) )
        {
            if( PC_String_Parse( handle, &s ) )
            {
                bs->overlayShader = trap_R_RegisterShader( s );
            }
            continue;
        }
        else if( !Q_stricmp( token.string, "noPowerShader" ) )
        {
            if( PC_String_Parse( handle, &s ) )
            {
                bs->noPowerShader = trap_R_RegisterShader( s );
            }
            continue;
        }
        else if( !Q_stricmp( token.string, "markedShader" ) )
        {
            if( PC_String_Parse( handle, &s ) )
            {
                bs->markedShader = trap_R_RegisterShader( s );
            }
            continue;
        }
        else if( !Q_stricmp( token.string, "healthSevereColor" ) )
        {
            if( PC_Color_Parse( handle, &c ) )
            {
                Vector4Copy( c, bs->healthSevereColor );
            }
            continue;
        }
        else if( !Q_stricmp( token.string, "healthHighColor" ) )
        {
            if( PC_Color_Parse( handle, &c ) )
            {
                Vector4Copy( c, bs->healthHighColor );
            }
            continue;
        }
        else if( !Q_stricmp( token.string, "healthElevatedColor" ) )
        {
            if( PC_Color_Parse( handle, &c ) )
            {
                Vector4Copy( c, bs->healthElevatedColor );
            }
            continue;
        }
        else if( !Q_stricmp( token.string, "healthGuardedColor" ) )
        {
            if( PC_Color_Parse( handle, &c ) )
            {
                Vector4Copy( c, bs->healthGuardedColor );
            }
            continue;
        }
        else if( !Q_stricmp( token.string, "healthLowColor" ) )
        {
            if( PC_Color_Parse( handle, &c ) )
            {
                Vector4Copy( c, bs->healthLowColor );
            }
            continue;
        }
        else if( !Q_stricmp( token.string, "foreColor" ) )
        {
            if( PC_Color_Parse( handle, &c ) )
            {
                Vector4Copy( c, bs->foreColor );
            }
            continue;
        }
        else if( !Q_stricmp( token.string, "backColor" ) )
        {
            if( PC_Color_Parse( handle, &c ) )
            {
                Vector4Copy( c, bs->backColor );
            }
            continue;
        }
        else if( !Q_stricmp( token.string, "frameHeight" ) )
        {
            if( PC_Int_Parse( handle, &i ) )
            {
                bs->frameHeight = i;
            }
            continue;
        }
        else if( !Q_stricmp( token.string, "frameWidth" ) )
        {
            if( PC_Int_Parse( handle, &i ) )
            {
                bs->frameWidth = i;
            }
            continue;
        }
        else if( !Q_stricmp( token.string, "healthPadding" ) )
        {
            if( PC_Int_Parse( handle, &i ) )
            {
                bs->healthPadding = i;
            }
            continue;
        }
        else if( !Q_stricmp( token.string, "overlayHeight" ) )
        {
            if( PC_Int_Parse( handle, &i ) )
            {
                bs->overlayHeight = i;
            }
            continue;
        }
        else if( !Q_stricmp( token.string, "overlayWidth" ) )
        {
            if( PC_Int_Parse( handle, &i ) )
            {
                bs->overlayWidth = i;
            }
            continue;
        }
        else if( !Q_stricmp( token.string, "verticalMargin" ) )
        {
            if( PC_Float_Parse( handle, &f ) )
            {
                bs->verticalMargin = f;
            }
            continue;
        }
        else if( !Q_stricmp( token.string, "horizontalMargin" ) )
        {
            if( PC_Float_Parse( handle, &f ) )
            {
                bs->horizontalMargin = f;
            }
            continue;
        }
        else
        {
            Com_Printf( "BuildableStatusParse: unknown token %s in %s\n", token.string, filename );
            bs->loaded = false;
            return;
        }
    }
    bs->loaded = true;
}

#define STATUS_FADE_TIME      200
#define STATUS_MAX_VIEW_DIST  900.0f
#define STATUS_PEEK_DIST      20

/*
==================
idCGameBuildable::BuildableStatusDisplay
==================
*/
void idCGameBuildable::BuildableStatusDisplay( centity_t* cent )
{
    entityState_t* es = &cent->currentState;
    vec3_t origin;
    float32 healthScale, x, y, d;
    sint health, i, j, entNum, anim;
    vec4_t color;
    bool powered, marked;
    trace_t tr;
    buildStat_t* bs;
    vec3_t trOrigin, right;
    bool visible = false;
    vec3_t mins, maxs;
    entityState_t* hit;
    float32 transparency = 1.0f;
    
    if( bggame->Buildable( ( buildable_t )es->modelindex )->team == TEAM_ALIENS )
    {
        bs = &cgs.alienBuildStat;
    }
    else
    {
        bs = &cgs.humanBuildStat;
    }
    
    if( !bs->loaded )
    {
        return;
    }
    
    d = Distance( cent->lerpOrigin, cg.refdef.vieworg );
    if( d > STATUS_MAX_VIEW_DIST )
    {
        return;
    }
    
    Vector4Copy( bs->foreColor, color );
    
    // trace for center point
    bggame->BuildableBoundingBox( ( buildable_t )es->modelindex, mins, maxs );
    
    // hack for shrunken barricades
    anim = es->torsoAnim & ~( ANIM_FORCEBIT | ANIM_TOGGLEBIT );
    if( es->modelindex == BA_A_BARRICADE && ( anim == BANIM_DESTROYED || !( es->eFlags & EF_B_SPAWNED ) ) )
    {
        maxs[2] = ( sint )( maxs[2] * BARRICADE_SHRINKPROP );
    }
    
    VectorCopy( cent->lerpOrigin, origin );
    
    // center point
    origin[ 2 ] += mins[ 2 ];
    origin[ 2 ] += ( fabs( mins[ 2 ] ) + fabs( maxs[ 2 ] ) ) / 2;
    
    entNum = cg.predictedPlayerState.clientNum;
    
    // if first try fails, step left, step right
    for( j = 0; j < 3; j++ )
    {
        VectorCopy( cg.refdef.vieworg, trOrigin );
        switch( j )
        {
            case 1:
                // step right
                AngleVectors( cg.refdefViewAngles, nullptr, right, nullptr );
                VectorMA( trOrigin, STATUS_PEEK_DIST, right, trOrigin );
                break;
            case 2:
                // step left
                AngleVectors( cg.refdefViewAngles, nullptr, right, nullptr );
                VectorMA( trOrigin, -STATUS_PEEK_DIST, right, trOrigin );
                break;
            default:
                break;
        }
        
        // look through up to 3 players and/or transparent buildables
        for( i = 0; i < 3; i++ )
        {
            idCGamePredict::Trace( &tr, trOrigin, nullptr, nullptr, origin, entNum, MASK_SHOT );
            if( tr.entityNum == cent->currentState.number )
            {
                visible = true;
                break;
            }
            
            if( tr.entityNum == ENTITYNUM_WORLD )
            {
                break;
            }
            
            hit  = &cg_entities[ tr.entityNum ].currentState;
            
            if( tr.entityNum < MAX_CLIENTS || ( hit->eType == ET_BUILDABLE && ( !( es->eFlags & EF_B_SPAWNED ) ||
                                                bggame->Buildable( ( buildable_t )hit->modelindex )->transparentTest ) ) )
            {
                entNum = tr.entityNum;
                VectorCopy( tr.endpos, trOrigin );
            }
            else
            {
                break;
            }
        }
    }
    // hack to make the kit obscure view
    if( cg_drawGun.integer && visible && cg.predictedPlayerState.stats[ STAT_TEAM ] == TEAM_HUMANS && idCGameDrawTools::WorldToScreen( origin, &x, &y ) )
    {
        if( x > 450 && y > 290 )
        {
            visible = false;
        }
    }
    
    health = ( uchar8 )es->generic1;
    healthScale = ( float32 )health / B_HEALTH_MASK;
    
    if( healthScale < 0.0f )
    {
        healthScale = 0.0f;
    }
    else if( healthScale > 1.0f )
    {
        healthScale = 1.0f;
    }
    
    powered = es->eFlags & EF_B_POWERED;
    marked = es->eFlags & EF_B_MARKED;
    
    if( cg_hideHealthyBuildableStatus.integer && healthScale == 1.0f && powered && !marked )
    {
        visible = false;
    }
    
    if( !visible && cent->buildableStatus.visible )
    {
        cent->buildableStatus.visible   = false;
        cent->buildableStatus.lastTime  = cg.time;
    }
    else if( visible && !cent->buildableStatus.visible )
    {
        cent->buildableStatus.visible   = true;
        cent->buildableStatus.lastTime  = cg.time;
    }
    
    color[ 3 ] = transparency;
    
    // Fade up
    if( cent->buildableStatus.visible )
    {
        if( cent->buildableStatus.lastTime + STATUS_FADE_TIME > cg.time )
        {
            color[3] = transparency * ( float32 )( cg.time - cent->buildableStatus.lastTime ) / STATUS_FADE_TIME;
        }
    }
    
    // Fade down
    if( !cent->buildableStatus.visible )
    {
        if( cent->buildableStatus.lastTime + STATUS_FADE_TIME > cg.time )
        {
            color[3] = transparency * ( 1.0f - ( float32 )( cg.time - cent->buildableStatus.lastTime ) / STATUS_FADE_TIME );
        }
        else
        {
            return;
        }
    }
    
    if( idCGameDrawTools::WorldToScreen( origin, &x, &y ) )
    {
        float32 picH = bs->frameHeight;
        float32 picW = bs->frameWidth;
        float32 picX = x;
        float32 picY = y;
        float32 scale;
        float32 subH, subY;
        vec4_t frameColor;
        
        // this is fudged to get the width/height in the cfg to be more realistic
        scale = ( picH / d ) * 3;
        
        picH *= scale;
        picW *= scale;
        picX -= ( picW * 0.5f );
        picY -= ( picH * 0.5f );
        
        // sub-elements such as icons and number
        subH = picH - ( picH * bs->verticalMargin );
        subY = picY + ( picH * 0.5f ) - ( subH * 0.5f );
        
        if( bs->frameShader )
        {
            Vector4Copy( bs->backColor, frameColor );
            frameColor[ 3 ] = color[ 3 ];
            trap_R_SetColor( frameColor );
            idCGameDrawTools::DrawPic( picX, picY, picW, picH, bs->frameShader );
            trap_R_SetColor( nullptr );
        }
        
        if( health > 0 )
        {
            float32 hX, hY, hW, hH;
            vec4_t healthColor;
            
            hX = picX + ( bs->healthPadding * scale );
            hY = picY + ( bs->healthPadding * scale );
            hH = picH - ( bs->healthPadding * 2.0f * scale );
            hW = picW * healthScale - ( bs->healthPadding * 2.0f * scale );
            
            if( healthScale == 1.0f )
            {
                Vector4Copy( bs->healthLowColor, healthColor );
            }
            else if( healthScale >= 0.75f )
            {
                Vector4Copy( bs->healthGuardedColor, healthColor );
            }
            else if( healthScale >= 0.50f )
            {
                Vector4Copy( bs->healthElevatedColor, healthColor );
            }
            else if( healthScale >= 0.25f )
            {
                Vector4Copy( bs->healthHighColor, healthColor );
            }
            else
            {
                Vector4Copy( bs->healthSevereColor, healthColor );
            }
            
            healthColor[ 3 ] = color[ 3 ];
            trap_R_SetColor( healthColor );
            
            idCGameDrawTools::DrawPic( hX, hY, hW, hH, cgs.media.whiteShader );
            trap_R_SetColor( nullptr );
        }
        
        if( bs->overlayShader )
        {
            float32 oW = bs->overlayWidth;
            float32 oH = bs->overlayHeight;
            float32 oX = x;
            float32 oY = y;
            
            oH *= scale;
            oW *= scale;
            oX -= ( oW * 0.5f );
            oY -= ( oH * 0.5f );
            
            trap_R_SetColor( frameColor );
            idCGameDrawTools::DrawPic( oX, oY, oW, oH, bs->overlayShader );
            trap_R_SetColor( nullptr );
        }
        
        trap_R_SetColor( color );
        if( !powered )
        {
            float32 pX;
            
            pX = picX + ( subH * bs->horizontalMargin );
            idCGameDrawTools::DrawPic( pX, subY, subH, subH, bs->noPowerShader );
        }
        
        if( marked )
        {
            float32 mX;
            
            mX = picX + picW - ( subH * bs->horizontalMargin ) - subH;
            idCGameDrawTools::DrawPic( mX, subY, subH, subH, bs->markedShader );
        }
        
        {
            float32 nX;
            sint healthMax;
            sint healthPoints;
            
            healthMax = bggame->Buildable( ( buildable_t )es->modelindex )->health;
            healthPoints = ( sint )( healthScale * healthMax );
            
            if( health > 0 && healthPoints < 1 )
            {
                healthPoints = 1;
            }
            
            nX = picX + ( picW * 0.5f ) - 2.0f - ( ( subH * 4 ) * 0.5f );
            
            if( healthPoints > 999 )
            {
                nX -= 0.0f;
            }
            else if( healthPoints > 99 )
            {
                nX -= subH * 0.5f;
            }
            else if( healthPoints > 9 )
            {
                nX -= subH * 1.0f;
            }
            else
            {
                nX -= subH * 1.5f;
            }
            
            idCGameDraw::DrawField( nX, subY, 4, subH, subH, healthPoints );
        }
        trap_R_SetColor( nullptr );
    }
}

/*
==================
idCGameBuildable::SortDistance
==================
*/
sint idCGameBuildable::SortDistance( const void* a, const void* b )
{
    centity_t* aent, *bent;
    float32 adist, bdist;
    
    aent = &cg_entities[*( sint* )a];
    bent = &cg_entities[*( sint* )b];
    adist = Distance( cg.refdef.vieworg, aent->lerpOrigin );
    bdist = Distance( cg.refdef.vieworg, bent->lerpOrigin );
    
    if( adist > bdist )
    {
        return -1;
    }
    else if( adist < bdist )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
==================
idCGameBuildable::PlayerIsBuilder
==================
*/
bool idCGameBuildable::PlayerIsBuilder( buildable_t buildable )
{
    switch( cg.predictedPlayerState.weapon )
    {
        case WP_ABUILD:
        case WP_ABUILD2:
        case WP_HBUILD:
            return bggame->Buildable( buildable )->team == bggame->Weapon( ( weapon_t )cg.predictedPlayerState.weapon )->team;
            
        default:
            return false;
    }
}

/*
==================
idCGameBuildable::BuildableRemovalPending
==================
*/
bool idCGameBuildable::BuildableRemovalPending( sint entityNum )
{
    sint i;
    playerState_t* ps = &cg.snap->ps;
    
    if( !( ps->stats[STAT_BUILDABLE] & SB_VALID_TOGGLEBIT ) )
    {
        return false;
    }
    
    for( i = 0; i < MAX_MISC; i++ )
    {
        if( ps->misc[i] == entityNum )
        {
            return true;
        }
    }
    
    return false;
}

/*
==================
DrawBuildableStatus
==================
*/
void idCGameBuildable::DrawBuildableStatus( void )
{
    sint i;
    centity_t* cent;
    entityState_t* es;
    sint buildableList[ MAX_ENTITIES_IN_SNAPSHOT ];
    sint buildables = 0;
    
    for( i = 0; i < cg.snap->numEntities; i++ )
    {
        cent = &cg_entities[ cg.snap->entities[ i ].number ];
        es = &cent->currentState;
        
        if( es->eType == ET_BUILDABLE && ( PlayerIsBuilder( ( buildable_t )es->modelindex ) ||
                                           ( cg_drawBuildableStatus.integer && bggame->Buildable( ( buildable_t )es->modelindex )->team ==
                                             bggame->Weapon( ( weapon_t )cg.predictedPlayerState.weapon )->team ) ) )
            buildableList[ buildables++ ] = cg.snap->entities[ i ].number;
    }
    
    qsort( buildableList, buildables, sizeof( sint ), SortDistance );
    
    for( i = 0; i < buildables; i++ )
    {
        BuildableStatusDisplay( &cg_entities[buildableList[i]] );
    }
}

#define BUILDABLE_SOUND_PERIOD  500

/*
==================
idCGameBuildable::Buildable
==================
*/
void idCGameBuildable::Buildable( centity_t* cent )
{
    refEntity_t ent;
    entityState_t* es = &cent->currentState;
    vec3_t angles;
    vec3_t surfNormal, xNormal, mins, maxs;
    vec3_t refNormal = { 0.0f, 0.0f, 1.0f };
    float32 rotAngle;
    team_t team = bggame->Buildable( ( buildable_t )es->modelindex )->team;
    float32 scale, health, healthScale;
    
    //must be before EF_NODRAW check
    if( team == TEAM_ALIENS )
    {
        Creep( cent );
    }
    
    // if set to invisible, skip
    if( es->eFlags & EF_NODRAW )
    {
        if( idCGameParticles::IsParticleSystemValid( &cent->buildablePS ) )
        {
            idCGameParticles::DestroyParticleSystem( &cent->buildablePS );
        }
        
        return;
    }
    
    ::memset( &ent, 0, sizeof( ent ) );
    
    VectorCopy( cent->lerpOrigin, ent.origin );
    VectorCopy( cent->lerpOrigin, ent.oldorigin );
    VectorCopy( cent->lerpOrigin, ent.lightingOrigin );
    
    VectorCopy( es->origin2, surfNormal );
    
    VectorCopy( es->angles, angles );
    bggame->BuildableBoundingBox( ( buildable_t )es->modelindex, mins, maxs );
    
    if( es->pos.trType == TR_STATIONARY )
    {
        // Positioning a buildable involves potentially up to two traces, and
        // seeing as buildables rarely move, we cache the results and recalculate
        // only if the buildable moves or changes orientation
        if( VectorCompare( cent->buildableCache.cachedOrigin, cent->lerpOrigin ) && VectorCompare( cent->buildableCache.cachedNormal, surfNormal ) )
        {
            VectorCopy( cent->buildableCache.axis[ 0 ], ent.axis[ 0 ] );
            VectorCopy( cent->buildableCache.axis[ 1 ], ent.axis[ 1 ] );
            VectorCopy( cent->buildableCache.axis[ 2 ], ent.axis[ 2 ] );
            VectorCopy( cent->buildableCache.origin, ent.origin );
        }
        else
        {
            PositionAndOrientateBuildable( angles, ent.origin, surfNormal, es->number, mins, maxs, ent.axis, ent.origin );
            
            VectorCopy( ent.axis[ 0 ], cent->buildableCache.axis[ 0 ] );
            VectorCopy( ent.axis[ 1 ], cent->buildableCache.axis[ 1 ] );
            VectorCopy( ent.axis[ 2 ], cent->buildableCache.axis[ 2 ] );
            VectorCopy( ent.origin, cent->buildableCache.origin );
            VectorCopy( cent->lerpOrigin, cent->buildableCache.cachedOrigin );
            VectorCopy( surfNormal, cent->buildableCache.cachedNormal );
        }
    }
    
    //offset on the Z axis if required
    VectorMA( ent.origin, bggame->BuildableConfig( ( buildable_t )es->modelindex )->zOffset, surfNormal, ent.origin );
    
    VectorCopy( ent.origin, ent.oldorigin ); // don't positionally lerp at all
    VectorCopy( ent.origin, ent.lightingOrigin );
    
    ent.hModel = cg_buildables[ es->modelindex ].models[ 0 ];
    
    if( !( es->eFlags & EF_B_SPAWNED ) )
    {
        sfxHandle_t prebuildSound = cgs.media.humanBuildablePrebuild;
        
        if( team == TEAM_HUMANS )
        {
            ent.customShader = cgs.media.humanSpawningShader;
            prebuildSound = cgs.media.humanBuildablePrebuild;
        }
        else if( team == TEAM_ALIENS )
        {
            prebuildSound = cgs.media.alienBuildablePrebuild;
        }
        
        trap_S_AddLoopingSound( es->number, cent->lerpOrigin, vec3_origin, prebuildSound );
    }
    
    BuildableAnimation( cent, &ent.oldframe, &ent.frame, &ent.backlerp );
    
    //rescale the model
    scale = bggame->BuildableConfig( ( buildable_t )es->modelindex )->modelScale;
    
    if( scale != 1.0f )
    {
        VectorScale( ent.axis[ 0 ], scale, ent.axis[ 0 ] );
        VectorScale( ent.axis[ 1 ], scale, ent.axis[ 1 ] );
        VectorScale( ent.axis[ 2 ], scale, ent.axis[ 2 ] );
        
        ent.nonNormalizedAxes = true;
    }
    else
    {
        ent.nonNormalizedAxes = false;
    }
    
    if( PlayerIsBuilder( ( buildable_t )es->modelindex ) && BuildableRemovalPending( es->number ) )
    {
        ent.customShader = cgs.media.redBuildShader;
    }
    
    //add to refresh list
    trap_R_AddRefEntityToScene( &ent );
    
    CrossProduct( surfNormal, refNormal, xNormal );
    VectorNormalize( xNormal );
    rotAngle = RAD2DEG( acos( DotProduct( surfNormal, refNormal ) ) );
    
    //turret barrel bit
    if( cg_buildables[ es->modelindex ].models[ 1 ] )
    {
        refEntity_t turretBarrel;
        vec3_t flatAxis[ 3 ];
        
        ::memset( &turretBarrel, 0, sizeof( turretBarrel ) );
        
        turretBarrel.hModel = cg_buildables[ es->modelindex ].models[ 1 ];
        
        idCGameEnts::PositionEntityOnTag( &turretBarrel, &ent, ent.hModel, "tag_turret" );
        VectorCopy( cent->lerpOrigin, turretBarrel.lightingOrigin );
        AnglesToAxis( es->angles2, flatAxis );
        
        RotatePointAroundVector( turretBarrel.axis[ 0 ], xNormal, flatAxis[ 0 ], -rotAngle );
        RotatePointAroundVector( turretBarrel.axis[ 1 ], xNormal, flatAxis[ 1 ], -rotAngle );
        RotatePointAroundVector( turretBarrel.axis[ 2 ], xNormal, flatAxis[ 2 ], -rotAngle );
        
        turretBarrel.oldframe = ent.oldframe;
        turretBarrel.frame    = ent.frame;
        turretBarrel.backlerp = ent.backlerp;
        
        turretBarrel.customShader = ent.customShader;
        
        if( scale != 1.0f )
        {
            VectorScale( turretBarrel.axis[ 0 ], scale, turretBarrel.axis[ 0 ] );
            VectorScale( turretBarrel.axis[ 1 ], scale, turretBarrel.axis[ 1 ] );
            VectorScale( turretBarrel.axis[ 2 ], scale, turretBarrel.axis[ 2 ] );
            
            turretBarrel.nonNormalizedAxes = true;
        }
        else
        {
            turretBarrel.nonNormalizedAxes = false;
        }
        
        if( PlayerIsBuilder( ( buildable_t )es->modelindex ) && BuildableRemovalPending( es->number ) )
        {
            turretBarrel.customShader = cgs.media.redBuildShader;
        }
        
        trap_R_AddRefEntityToScene( &turretBarrel );
    }
    
    //turret barrel bit
    if( cg_buildables[ es->modelindex ].models[ 2 ] )
    {
        refEntity_t turretTop;
        vec3_t flatAxis[ 3 ];
        vec3_t swivelAngles;
        
        ::memset( &turretTop, 0, sizeof( turretTop ) );
        
        VectorCopy( es->angles2, swivelAngles );
        swivelAngles[ PITCH ] = 0.0f;
        
        turretTop.hModel = cg_buildables[ es->modelindex ].models[ 2 ];
        
        idCGameEnts::PositionRotatedEntityOnTag( &turretTop, &ent, ent.hModel, "tag_turret" );
        VectorCopy( cent->lerpOrigin, turretTop.lightingOrigin );
        AnglesToAxis( swivelAngles, flatAxis );
        
        RotatePointAroundVector( turretTop.axis[ 0 ], xNormal, flatAxis[ 0 ], -rotAngle );
        RotatePointAroundVector( turretTop.axis[ 1 ], xNormal, flatAxis[ 1 ], -rotAngle );
        RotatePointAroundVector( turretTop.axis[ 2 ], xNormal, flatAxis[ 2 ], -rotAngle );
        
        turretTop.oldframe = ent.oldframe;
        turretTop.frame = ent.frame;
        turretTop.backlerp = ent.backlerp;
        
        turretTop.customShader = ent.customShader;
        
        if( scale != 1.0f )
        {
            VectorScale( turretTop.axis[ 0 ], scale, turretTop.axis[ 0 ] );
            VectorScale( turretTop.axis[ 1 ], scale, turretTop.axis[ 1 ] );
            VectorScale( turretTop.axis[ 2 ], scale, turretTop.axis[ 2 ] );
            
            turretTop.nonNormalizedAxes = true;
        }
        else
        {
            turretTop.nonNormalizedAxes = false;
        }
        
        if( PlayerIsBuilder( ( buildable_t )es->modelindex ) && BuildableRemovalPending( es->number ) )
        {
            turretTop.customShader = cgs.media.redBuildShader;
        }
        
        trap_R_AddRefEntityToScene( &turretTop );
    }
    
    //weapon effects for turrets
    if( es->eFlags & EF_FIRING )
    {
        weaponInfo_t*  weapon = &cg_weapons[ es->weapon ];
        
        if( cg.time - cent->muzzleFlashTime > MUZZLE_FLASH_TIME || bggame->Buildable( ( buildable_t )es->modelindex )->turretProjType == WP_TESLAGEN )
        {
            if( weapon->wim[ WPM_PRIMARY ].flashDlightColor[ 0 ] || weapon->wim[ WPM_PRIMARY ].flashDlightColor[ 1 ]
                    || weapon->wim[ WPM_PRIMARY ].flashDlightColor[ 2 ] )
            {
                trap_R_AddLightToScene( cent->lerpOrigin, 300 + ( rand() & 31 ), weapon->wim[WPM_PRIMARY].flashDlightColor[0],
                                        weapon->wim[WPM_PRIMARY].flashDlightColor[1], weapon->wim[WPM_PRIMARY].flashDlightColor[2] );
            }
        }
        
        if( weapon->wim[WPM_PRIMARY].firingSound )
        {
            trap_S_AddLoopingSound( es->number, cent->lerpOrigin, vec3_origin, weapon->wim[WPM_PRIMARY].firingSound );
        }
        else if( weapon->readySound )
        {
            trap_S_AddLoopingSound( es->number, cent->lerpOrigin, vec3_origin, weapon->readySound );
        }
    }
    
    health = ( uchar8 )es->generic1;
    healthScale = ( float32 )health / B_HEALTH_MASK;
    
    if( healthScale < cent->lastBuildableHealthScale && ( es->eFlags & EF_B_SPAWNED ) )
    {
        if( cent->lastBuildableDamageSoundTime + BUILDABLE_SOUND_PERIOD < cg.time )
        {
            if( team == TEAM_HUMANS )
            {
                sint i = rand( ) % 4;
                trap_S_StartSound( nullptr, es->number, CHAN_BODY, cgs.media.humanBuildableDamage[ i ] );
                
                if( cent->lastBuildableHealthScale - healthScale > 99 )
                {
                    cent->buildableHitPS = idCGameParticles::SpawnNewParticleSystem( cgs.media.humanBuildableHitLargePS );
                }
                else
                {
                    cent->buildableHitPS = idCGameParticles::SpawnNewParticleSystem( cgs.media.humanBuildableHitSmallPS );
                }
            }
            else if( team == TEAM_ALIENS )
            {
                trap_S_StartSound( nullptr, es->number, CHAN_BODY, cgs.media.alienBuildableDamage );
                
                if( cent->lastBuildableHealthScale - healthScale > 30 )
                {
                    cent->buildableHitPS = idCGameParticles::SpawnNewParticleSystem( cgs.media.alienBuildableHitLargePS );
                }
                else
                {
                    cent->buildableHitPS = idCGameParticles::SpawnNewParticleSystem( cgs.media.alienBuildableHitSmallPS );
                }
            }
            if( idCGameParticles::IsParticleSystemValid( &cent->buildableHitPS ) )
            {
                idCGameAttachment::SetAttachmentCent( &cent->buildableHitPS->attachment, cent );
                idCGameAttachment::AttachToCent( &cent->buildableHitPS->attachment );
            }
            
            cent->lastBuildableDamageSoundTime = cg.time;
        }
    }
    
    cent->lastBuildableHealthScale = healthScale;
    
    //smoke etc for damaged buildables
    BuildableParticleEffects( cent );
}
