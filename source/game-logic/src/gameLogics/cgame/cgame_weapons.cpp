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
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <cgame/cgame_precompiled.hpp>

/*
===============
idCGameWeapons::idCGameWeapons
===============
*/
idCGameWeapons::idCGameWeapons( void )
{
}

/*
===============
idCGameWeapons::~idCGameWeapons
===============
*/
idCGameWeapons::~idCGameWeapons( void )
{
}

/*
=================
idCGameWeapons::RegisterUpgrade

The server says this item is used on this level
=================
*/
void idCGameWeapons::RegisterUpgrade( sint upgradeNum )
{
    upgradeInfo_t* upgradeInfo;
    valueType* icon;
    
    upgradeInfo = &cg_upgrades[upgradeNum];
    
    if( upgradeNum == 0 )
    {
        return;
    }
    
    if( upgradeInfo->registered )
    {
        return;
    }
    
    ::memset( upgradeInfo, 0, sizeof( *upgradeInfo ) );
    upgradeInfo->registered = true;
    
    if( strlen( bggame->Upgrade( ( upgrade_t )upgradeNum )->name ) <= 0 )
    {
        Error( "Couldn't find upgrade %i", upgradeNum );
    }
    
    upgradeInfo->humanName = bggame->Upgrade( ( upgrade_t )upgradeNum )->humanName;
    
    //la la la la la, i'm not listening!
    if( upgradeNum == UP_GRENADE )
    {
        upgradeInfo->upgradeIcon = cg_weapons[WP_GRENADE].weaponIcon;
    }
    else if( ( icon = bggame->Upgrade( ( upgrade_t )upgradeNum )->icon ) )
    {
        upgradeInfo->upgradeIcon = trap_R_RegisterShader( icon );
    }
}

/*
===============
idCGameWeapons::InitUpgrades

Precaches upgrades
===============
*/
void idCGameWeapons::InitUpgrades( void )
{
    sint i;
    
    ::memset( cg_upgrades, 0, sizeof( cg_upgrades ) );
    
    for( i = UP_NONE + 1; i < UP_NUM_UPGRADES; i++ )
    {
        RegisterUpgrade( i );
    }
}

/*
======================
idCGameWeapons::ParseWeaponAnimationFile

Read a configuration file containing animation counts and rates
models/weapons/rifle/animation.cfg, etc
======================
*/
bool idCGameWeapons::ParseWeaponAnimationFile( pointer filename, weaponInfo_t* weapon )
{
    valueType* text_p;
    sint len, i;
    valueType* token;
    float32 fps;
    valueType text[ 20000 ];
    fileHandle_t f;
    animation_t* animations;
    
    animations = weapon->animations;
    
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
    for( i = WANIM_NONE + 1; i < MAX_WEAPON_ANIMATIONS; i++ )
    {
        token = COM_Parse( &text_p );
        if( !*token )
        {
            break;
        }
        
        if( !Q_stricmp( token, "noDrift" ) )
        {
            weapon->noDrift = true;
            continue;
        }
        
        animations[ i ].firstFrame = atoi( token );
        
        token = COM_Parse( &text_p );
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
        
        token = COM_Parse( &text_p );
        if( !*token )
        {
            break;
        }
        
        animations[i].loopFrames = atoi( token );
        
        token = COM_Parse( &text_p );
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
    
    if( i != MAX_WEAPON_ANIMATIONS )
    {
        Printf( "Error parsing animation file: %s\n", filename );
        return false;
    }
    
    return true;
}

/*
===============
idCGameWeapons::ParseWeaponModeSection

Parse a weapon mode section
===============
*/
bool idCGameWeapons::ParseWeaponModeSection( weaponInfoMode_t* wim, valueType** text_p )
{
    valueType* token;
    sint i;
    
    // read optional parameters
    while( 1 )
    {
        token = COM_Parse( text_p );
        if( !token )
        {
            break;
        }
        
        if( !Q_stricmp( token, "" ) )
        {
            return false;
        }
        
        if( !Q_stricmp( token, "missileModel" ) )
        {
            token = COM_Parse( text_p );
            if( !token )
            {
                break;
            }
            
            wim->missileModel = trap_R_RegisterModel( token );
            
            if( !wim->missileModel )
            {
                Printf( S_COLOR_RED "ERROR: missile model not found %s\n", token );
            }
            
            continue;
        }
        else if( !Q_stricmp( token, "missileSprite" ) )
        {
            sint size = 0;
            
            token = COM_Parse( text_p );
            if( !token )
            {
                break;
            }
            
            size = atoi( token );
            
            if( size < 0 )
            {
                size = 0;
            }
            
            token = COM_Parse( text_p );
            if( !token )
            {
                break;
            }
            
            wim->missileSprite = trap_R_RegisterShader( token );
            wim->missileSpriteSize = size;
            wim->usesSpriteMissle = true;
            
            if( !wim->missileSprite )
            {
                Printf( S_COLOR_RED "ERROR: missile sprite not found %s\n", token );
            }
            
            continue;
        }
        else if( !Q_stricmp( token, "missileSpriteCharge" ) )
        {
            token = COM_Parse( text_p );
            if( !token )
            {
                break;
            }
            
            wim->missileSpriteCharge = atof( token );
            
            continue;
        }
        else if( !Q_stricmp( token, "missileRotates" ) )
        {
            wim->missileRotates = true;
            
            continue;
        }
        else if( !Q_stricmp( token, "missileAnimates" ) )
        {
            wim->missileAnimates = true;
            
            token = COM_Parse( text_p );
            if( !token )
            {
                break;
            }
            
            wim->missileAnimStartFrame = atoi( token );
            
            token = COM_Parse( text_p );
            if( !token )
            {
                break;
            }
            
            wim->missileAnimNumFrames = atoi( token );
            
            token = COM_Parse( text_p );
            if( !token )
            {
                break;
            }
            
            wim->missileAnimFrameRate = atoi( token );
            
            token = COM_Parse( text_p );
            if( !token )
            {
                break;
            }
            
            wim->missileAnimLooping = atoi( token );
            
            continue;
        }
        else if( !Q_stricmp( token, "missileParticleSystem" ) )
        {
            token = COM_Parse( text_p );
            if( !token )
            {
                break;
            }
            
            wim->missileParticleSystem = idCGameParticles::RegisterParticleSystem( token );
            
            if( !wim->missileParticleSystem )
            {
                Printf( S_COLOR_RED "ERROR: missile particle system not found %s\n", token );
            }
            
            continue;
        }
        else if( !Q_stricmp( token, "missileTrailSystem" ) )
        {
            token = COM_Parse( text_p );
            if( !token )
            {
                break;
            }
            
            wim->missileTrailSystem = idCGameTrails::RegisterTrailSystem( token );
            
            if( !wim->missileTrailSystem )
            {
                Printf( S_COLOR_RED "ERROR: missile trail system not found %s\n", token );
            }
            
            continue;
        }
        else if( !Q_stricmp( token, "muzzleParticleSystem" ) )
        {
            token = COM_Parse( text_p );
            if( !token )
            {
                break;
            }
            
            wim->muzzleParticleSystem = idCGameParticles::RegisterParticleSystem( token );
            
            if( !wim->muzzleParticleSystem )
            {
                Printf( S_COLOR_RED "ERROR: muzzle particle system not found %s\n", token );
            }
            
            continue;
        }
        else if( !Q_stricmp( token, "impactParticleSystem" ) )
        {
            token = COM_Parse( text_p );
            if( !token )
            {
                break;
            }
            
            wim->impactParticleSystem = idCGameParticles::RegisterParticleSystem( token );
            
            if( !wim->impactParticleSystem )
            {
                Printf( S_COLOR_RED "ERROR: impact particle system not found %s\n", token );
            }
            
            continue;
        }
        else if( !Q_stricmp( token, "impactMark" ) )
        {
            sint size = 0;
            
            token = COM_Parse( text_p );
            if( !token )
            {
                break;
            }
            
            size = atoi( token );
            
            if( size < 0 )
            {
                size = 0;
            }
            
            token = COM_Parse( text_p );
            if( !token )
            {
                break;
            }
            
            wim->impactMark = trap_R_RegisterShader( token );
            wim->impactMarkSize = size;
            
            if( !wim->impactMark )
            {
                Printf( S_COLOR_RED "ERROR: impact mark shader not found %s\n", token );
            }
            
            continue;
        }
        else if( !Q_stricmp( token, "impactSound" ) )
        {
            sint index = 0;
            
            token = COM_Parse( text_p );
            if( !token )
            {
                break;
            }
            
            index = atoi( token );
            
            if( index < 0 )
            {
                index = 0;
            }
            else if( index > 3 )
            {
                index = 3;
            }
            
            token = COM_Parse( text_p );
            if( !token )
            {
                break;
            }
            
            wim->impactSound[ index ] = trap_S_RegisterSound( token );
            
            continue;
        }
        else if( !Q_stricmp( token, "impactFleshSound" ) )
        {
            sint index = 0;
            
            token = COM_Parse( text_p );
            if( !token )
            {
                break;
            }
            
            index = atoi( token );
            
            if( index < 0 )
            {
                index = 0;
            }
            else if( index > 3 )
            {
                index = 3;
            }
            
            token = COM_Parse( text_p );
            if( !token )
            {
                break;
            }
            
            wim->impactFleshSound[ index ] = trap_S_RegisterSound( token );
            
            continue;
        }
        else if( !Q_stricmp( token, "alwaysImpact" ) )
        {
            wim->alwaysImpact = true;
            
            continue;
        }
        else if( !Q_stricmp( token, "flashDLightColor" ) )
        {
            for( i = 0 ; i < 3 ; i++ )
            {
                token = COM_Parse( text_p );
                if( !token )
                {
                    break;
                }
                
                wim->flashDlightColor[ i ] = atof( token );
            }
            
            continue;
        }
        else if( !Q_stricmp( token, "continuousFlash" ) )
        {
            wim->continuousFlash = true;
            
            continue;
        }
        else if( !Q_stricmp( token, "missileDlightColor" ) )
        {
            for( i = 0 ; i < 3 ; i++ )
            {
                token = COM_Parse( text_p );
                if( !token )
                {
                    break;
                }
                
                wim->missileDlightColor[ i ] = atof( token );
            }
            
            continue;
        }
        else if( !Q_stricmp( token, "missileDlight" ) )
        {
            sint size = 0;
            
            token = COM_Parse( text_p );
            if( !token )
            {
                break;
            }
            
            size = atoi( token );
            
            if( size < 0 )
            {
                size = 0;
            }
            
            wim->missileDlight = size;
            
            continue;
        }
        else if( !Q_stricmp( token, "firingSound" ) )
        {
            token = COM_Parse( text_p );
            if( !token )
            {
                break;
            }
            
            wim->firingSound = trap_S_RegisterSound( token );
            
            continue;
        }
        else if( !Q_stricmp( token, "missileSound" ) )
        {
            token = COM_Parse( text_p );
            if( !token )
            {
                break;
            }
            
            wim->missileSound = trap_S_RegisterSound( token );
            
            continue;
        }
        else if( !Q_stricmp( token, "flashSound" ) )
        {
            sint index = 0;
            
            token = COM_Parse( text_p );
            if( !token )
            {
                break;
            }
            
            index = atoi( token );
            
            if( index < 0 )
            {
                index = 0;
            }
            else if( index > 3 )
            {
                index = 3;
            }
            
            token = COM_Parse( text_p );
            if( !token )
            {
                break;
            }
            
            wim->flashSound[ index ] = trap_S_RegisterSound( token );
            
            continue;
        }
        else if( !Q_stricmp( token, "}" ) )
        {
            return true; //reached the end of this weapon section
        }
        else
        {
            Printf( S_COLOR_RED "ERROR: unknown token '%s' in weapon section\n", token );
            return false;
        }
    }
    
    return false;
}

/*
======================
idCGameWeapons::ParseWeaponFile

Parses a configuration file describing a weapon
======================
*/
bool idCGameWeapons::ParseWeaponFile( pointer filename, weaponInfo_t* wi )
{
    valueType* text_p, *token, text[20000];
    sint len;
    fileHandle_t f;
    weaponMode_t weaponMode = WPM_NONE;
    
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
    
    // read optional parameters
    while( 1 )
    {
        token = COM_Parse( &text_p );
        if( !token )
        {
            break;
        }
        
        if( !Q_stricmp( token, "" ) )
        {
            break;
        }
        
        if( !Q_stricmp( token, "{" ) )
        {
            if( weaponMode == WPM_NONE )
            {
                Printf( S_COLOR_RED "ERROR: weapon mode section started without a declaration\n" );
                return false;
            }
            else if( !ParseWeaponModeSection( &wi->wim[ weaponMode ], &text_p ) )
            {
                Printf( S_COLOR_RED "ERROR: failed to parse weapon mode section\n" );
                return false;
            }
            
            //start parsing ejectors again
            weaponMode = WPM_NONE;
            
            continue;
        }
        else if( !Q_stricmp( token, "primary" ) )
        {
            weaponMode = WPM_PRIMARY;
            continue;
        }
        else if( !Q_stricmp( token, "secondary" ) )
        {
            weaponMode = WPM_SECONDARY;
            continue;
        }
        else if( !Q_stricmp( token, "tertiary" ) )
        {
            weaponMode = WPM_TERTIARY;
            continue;
        }
        else if( !Q_stricmp( token, "weaponModel" ) )
        {
            valueType path[ MAX_QPATH ];
            
            token = COM_Parse( &text_p );
            if( !token )
            {
                break;
            }
            
            wi->weaponModel = trap_R_RegisterModel( token );
            
            if( !wi->weaponModel )
            {
                Printf( S_COLOR_RED "ERROR: weapon model not found %s\n", token );
            }
            
            strcpy( path, token );
            COM_StripExtension3( path, path, MAX_QPATH );
            strcat( path, "_flash.md3" );
            wi->flashModel = trap_R_RegisterModel( path );
            
            strcpy( path, token );
            COM_StripExtension3( path, path, MAX_QPATH );
            strcat( path, "_barrel.md3" );
            wi->barrelModel = trap_R_RegisterModel( path );
            
            strcpy( path, token );
            COM_StripExtension3( path, path, MAX_QPATH );
            strcat( path, "_hand.md3" );
            wi->handsModel = trap_R_RegisterModel( path );
            
            continue;
        }
        else if( !Q_stricmp( token, "weaponModel3rdPerson" ) )
        {
            valueType path[ MAX_QPATH ];
            
            token = COM_Parse( &text_p );
            if( !token )
            {
                break;
            }
            
            wi->weaponModel3rdPerson = trap_R_RegisterModel( token );
            
            if( !wi->weaponModel3rdPerson )
            {
                Printf( S_COLOR_RED "ERROR: 3rd person weapon model not found %s\n", token );
            }
            
            strcpy( path, token );
            COM_StripExtension3( path, path, MAX_QPATH );
            strcat( path, "_flash.md3" );
            wi->flashModel3rdPerson = trap_R_RegisterModel( path );
            
            strcpy( path, token );
            COM_StripExtension3( path, path, MAX_QPATH );
            strcat( path, "_barrel.md3" );
            wi->barrelModel3rdPerson = trap_R_RegisterModel( path );
            
            continue;
        }
        else if( !Q_stricmp( token, "idleSound" ) )
        {
            token = COM_Parse( &text_p );
            if( !token )
            {
                break;
            }
            
            wi->readySound = trap_S_RegisterSound( token );
            
            continue;
        }
        else if( !Q_stricmp( token, "icon" ) )
        {
            token = COM_Parse( &text_p );
            if( !token )
            {
                break;
            }
            
            wi->weaponIcon = wi->ammoIcon = trap_R_RegisterShader( token );
            
            if( !wi->weaponIcon )
            {
                Printf( S_COLOR_RED "ERROR: weapon icon not found %s\n", token );
            }
            
            continue;
        }
        else if( !Q_stricmp( token, "crosshair" ) )
        {
            sint size = 0;
            
            token = COM_Parse( &text_p );
            if( !token )
            {
                break;
            }
            
            size = atoi( token );
            if( size < 0 )
            {
                size = 0;
            }
            
            token = COM_Parse( &text_p );
            if( !token )
            {
                break;
            }
            
            wi->crossHair = trap_R_RegisterShader( token );
            wi->crossHairSize = size;
            
            if( !wi->crossHair )
            {
                Printf( S_COLOR_RED "ERROR: weapon crosshair not found %s\n", token );
            }
            
            continue;
        }
        else if( !Q_stricmp( token, "disableIn3rdPerson" ) )
        {
            wi->disableIn3rdPerson = true;
            
            continue;
        }
        
        Com_Printf( S_COLOR_RED "ERROR: unknown token '%s'\n", token );
        return false;
    }
    
    return true;
}

/*
=================
idCGameWeapons::RegisterWeapon
=================
*/
void idCGameWeapons::RegisterWeapon( sint weaponNum )
{
    weaponInfo_t* weaponInfo;
    valueType path[ MAX_QPATH ];
    vec3_t mins, maxs;
    sint i;
    
    weaponInfo = &cg_weapons[ weaponNum ];
    
    if( weaponNum == 0 )
    {
        return;
    }
    
    if( weaponInfo->registered )
    {
        return;
    }
    
    ::memset( weaponInfo, 0, sizeof( *weaponInfo ) );
    weaponInfo->registered = true;
    
    if( strlen( bggame->Weapon( ( weapon_t )weaponNum )->name ) <= 0 )
    {
        Error( "Couldn't find weapon %i", weaponNum );
    }
    
    Q_vsprintf_s( path, MAX_QPATH, MAX_QPATH, "models/weapons/%s/weapon.cfg", bggame->Weapon( ( weapon_t )weaponNum )->name );
    
    weaponInfo->humanName = bggame->Weapon( ( weapon_t )weaponNum )->humanName;
    
    if( !ParseWeaponFile( path, weaponInfo ) )
    {
        Com_Printf( S_COLOR_RED "ERROR: failed to parse %s\n", path );
    }
    
    Q_vsprintf_s( path, MAX_QPATH, MAX_QPATH, "models/weapons/%s/animation.cfg", bggame->Weapon( ( weapon_t )weaponNum )->name );
    
    if( !cg_suppressWAnimWarnings.integer && !ParseWeaponAnimationFile( path, weaponInfo ) )
    {
        Com_Printf( S_COLOR_RED "ERROR: failed to parse %s\n", path );
    }
    
    // calc midpoint for rotation
    trap_R_ModelBounds( weaponInfo->weaponModel, mins, maxs );
    for( i = 0; i < 3; i++ )
    {
        weaponInfo->weaponMidpoint[i] = mins[i] + 0.5 * ( maxs[i] - mins[i] );
    }
    
    //FIXME:
    for( i = WPM_NONE + 1; i < WPM_NUM_WEAPONMODES; i++ )
    {
        weaponInfo->wim[i].loopFireSound = false;
    }
}

/*
===============
idCGameWeapons::InitWeapons

Precaches weapons
===============
*/
void idCGameWeapons::InitWeapons( void )
{
    sint i;
    
    ::memset( cg_weapons, 0, sizeof( cg_weapons ) );
    
    for( i = WP_NONE + 1; i < WP_NUM_WEAPONS; i++ )
    {
        RegisterWeapon( i );
    }
    
    cgs.media.level2ZapTS = idCGameTrails::RegisterTrailSystem( "models/weapons/lev2zap/lightning" );
    cgs.media.massDriverTS = idCGameTrails::RegisterTrailSystem( "models/weapons/mdriver/fireTS" );
}

/*
========================================================================================
VIEW WEAPON
========================================================================================
*/

/*
===============
idCGameWeapons::SetWeaponLerpFrameAnimation

may include ANIM_TOGGLEBIT
===============
*/
void idCGameWeapons::SetWeaponLerpFrameAnimation( weapon_t weapon, lerpFrame_t* lf, sint newAnimation )
{
    animation_t* anim;
    
    lf->animationNumber = newAnimation;
    newAnimation &= ~ANIM_TOGGLEBIT;
    
    if( newAnimation < 0 || newAnimation >= MAX_WEAPON_ANIMATIONS )
    {
        Error( "Bad animation number: %i", newAnimation );
    }
    
    anim = &cg_weapons[ weapon ].animations[ newAnimation ];
    
    lf->animation = anim;
    lf->animationTime = lf->frameTime + anim->initialLerp;
    
    if( cg_debugAnim.integer )
    {
        Printf( "Anim: %i\n", newAnimation );
    }
}

/*
===============
idCGameWeapons::WeaponAnimation
===============
*/
void idCGameWeapons::WeaponAnimation( centity_t* cent, sint* old, sint* now, float32* backLerp )
{
    lerpFrame_t* lf = &cent->pe.weapon;
    entityState_t* es = &cent->currentState;
    
    // see if the animation sequence is switching
    if( es->weaponAnim != lf->animationNumber || !lf->animation )
    {
        SetWeaponLerpFrameAnimation( ( weapon_t )es->weapon, lf, es->weaponAnim );
    }
    
    idCGameAnimation::RunLerpFrame( lf, 1.0f );
    
    *old = lf->oldFrame;
    *now = lf->frame;
    *backLerp = lf->backlerp;
}

/*
=================
idCGameWeapons::MapTorsoToWeaponFrame
=================
*/
sint idCGameWeapons::MapTorsoToWeaponFrame( clientInfo_t* ci, sint frame )
{
    // change weapon
    if( frame >= ci->animations[TORSO_DROP].firstFrame && frame < ci->animations[TORSO_DROP].firstFrame + 9 )
    {
        return frame - ci->animations[TORSO_DROP].firstFrame + 6;
    }
    
    // stand attack
    if( frame >= ci->animations[TORSO_ATTACK].firstFrame && frame < ci->animations[TORSO_ATTACK].firstFrame + 6 )
    {
        return 1 + frame - ci->animations[TORSO_ATTACK].firstFrame;
    }
    
    // stand attack 2
    if( frame >= ci->animations[TORSO_ATTACK2].firstFrame && frame < ci->animations[TORSO_ATTACK2].firstFrame + 6 )
    {
        return 1 + frame - ci->animations[TORSO_ATTACK2].firstFrame;
    }
    
    return 0;
}

/*
==============
idCGameWeapons::CalculateWeaponPosition
==============
*/
void idCGameWeapons::CalculateWeaponPosition( vec3_t origin, vec3_t angles )
{
    float32 scale, fracsin, bob;
    sint delta;
    weaponInfo_t* weapon;
    
    weapon = &cg_weapons[ cg.predictedPlayerState.weapon ];
    
    VectorCopy( cg.refdef.vieworg, origin );
    VectorCopy( cg.refdefViewAngles, angles );
    
    // on odd legs, invert some angles
    if( cg.bobcycle & 1 )
    {
        scale = -cg.xyspeed;
    }
    else
    {
        scale = cg.xyspeed;
    }
    
    // gun angles from bobbing
    // bob amount is class dependant
    bob = bggame->Class( ( class_t )cg.predictedPlayerState.stats[ STAT_CLASS ] )->bob;
    
    if( bob != 0 )
    {
        angles[ ROLL ] += scale * cg.bobfracsin * 0.005;
        angles[ YAW ] += scale * cg.bobfracsin * 0.01;
        angles[ PITCH ] += cg.xyspeed * cg.bobfracsin * 0.005;
    }
    
    // drop the weapon when landing
    if( weapon->noDrift )
    {
        delta = cg.time - cg.landTime;
        
        if( delta < LAND_DEFLECT_TIME )
        {
            origin[2] += cg.landChange * 0.25 * delta / LAND_DEFLECT_TIME;
        }
        else if( delta < LAND_DEFLECT_TIME + LAND_RETURN_TIME )
        {
            origin[2] += cg.landChange * 0.25 * ( LAND_DEFLECT_TIME + LAND_RETURN_TIME - delta ) / LAND_RETURN_TIME;
        }
        
        // idle drift
        scale = cg.xyspeed + 40;
        fracsin = sin( cg.time * 0.001 );
        angles[ ROLL ] += scale * fracsin * 0.01;
        angles[ YAW ] += scale * fracsin * 0.01;
        angles[ PITCH ] += scale * fracsin * 0.01;
    }
}

/*
======================
idCGameWeapons::MachinegunSpinAngle
======================
*/
#define SPIN_SPEED 0.9
#define COAST_TIME 1000
float32 idCGameWeapons::MachinegunSpinAngle( centity_t* cent, bool firing )
{
    sint delta;
    float32 angle, speed;
    
    delta = cg.time - cent->pe.barrelTime;
    
    if( cent->pe.barrelSpinning )
    {
        angle = cent->pe.barrelAngle + delta * SPIN_SPEED;
    }
    else
    {
        if( delta > COAST_TIME )
        {
            delta = COAST_TIME;
        }
        
        speed = 0.5 * ( SPIN_SPEED + ( float32 )( COAST_TIME - delta ) / COAST_TIME );
        angle = cent->pe.barrelAngle + delta * speed;
    }
    
    if( cent->pe.barrelSpinning == !firing )
    {
        cent->pe.barrelTime = cg.time;
        cent->pe.barrelAngle = AngleMod( angle );
        cent->pe.barrelSpinning = firing;
    }
    
    return angle;
}

/*
=============
idCGameWeapons::AddPlayerWeapon

Used for both the view weapon (ps is valid) and the world modelother character models (ps is nullptr)
The main player will have this called for BOTH cases, so effects like light and
sound should only be done on the world model case.
=============
*/
void idCGameWeapons::AddPlayerWeapon( refEntity_t* parent, playerState_t* ps, centity_t* cent )
{
    refEntity_t gun, barrel, flash;
    vec3_t angles;
    weapon_t weaponNum;
    weaponMode_t  weaponMode;
    weaponInfo_t* weapon;
    bool noGunModel, firing;
    
    weaponNum = ( weapon_t )cent->currentState.weapon;
    weaponMode = ( weaponMode_t )cent->currentState.generic1;
    
    if( weaponMode <= WPM_NONE || weaponMode >= WPM_NUM_WEAPONMODES )
    {
        weaponMode = WPM_PRIMARY;
    }
    
    if( ( ( cent->currentState.eFlags & EF_FIRING ) && weaponMode == WPM_PRIMARY ) || ( ( cent->currentState.eFlags & EF_FIRING2 ) && weaponMode == WPM_SECONDARY ) || ( ( cent->currentState.eFlags & EF_FIRING3 ) && weaponMode == WPM_TERTIARY ) )
    {
        firing = true;
    }
    else
    {
        firing = false;
    }
    
    RegisterWeapon( weaponNum );
    weapon = &cg_weapons[ weaponNum ];
    
    // add the weapon
    ::memset( &gun, 0, sizeof( gun ) );
    ::memset( &barrel, 0, sizeof( barrel ) );
    ::memset( &flash, 0, sizeof( flash ) );
    
    VectorCopy( parent->lightingOrigin, gun.lightingOrigin );
    gun.shadowPlane = parent->shadowPlane;
    gun.renderfx = parent->renderfx;
    
    if( ps )
    {
        gun.shaderRGBA[ 0 ] = 255;
        gun.shaderRGBA[ 1 ] = 255;
        gun.shaderRGBA[ 2 ] = 255;
        gun.shaderRGBA[ 3 ] = 255;
        
        //set weapon[1/2]Time when respective buttons change state
        if( cg.weapon1Firing != ( cg.predictedPlayerState.eFlags & EF_FIRING ) )
        {
            cg.weapon1Time = cg.time;
            cg.weapon1Firing = ( cg.predictedPlayerState.eFlags & EF_FIRING );
        }
        
        if( cg.weapon2Firing != ( cg.predictedPlayerState.eFlags & EF_FIRING2 ) )
        {
            cg.weapon2Time = cg.time;
            cg.weapon2Firing = ( cg.predictedPlayerState.eFlags & EF_FIRING2 );
        }
        
        if( cg.weapon3Firing != ( cg.predictedPlayerState.eFlags & EF_FIRING3 ) )
        {
            cg.weapon3Time = cg.time;
            cg.weapon3Firing = ( cg.predictedPlayerState.eFlags & EF_FIRING3 );
        }
    }
    
    if( !ps )
    {
        gun.hModel = weapon->weaponModel3rdPerson;
        
        if( !gun.hModel )
        {
            gun.hModel = weapon->weaponModel;
        }
    }
    else
    {
        gun.hModel = weapon->weaponModel;
    }
    
    noGunModel = ( ( !ps || cg.renderingThirdPerson ) && weapon->disableIn3rdPerson ) || !gun.hModel;
    
    if( !ps )
    {
        // add weapon ready sound
        if( firing && weapon->wim[ weaponMode ].firingSound )
        {
            trap_S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, vec3_origin, weapon->wim[weaponMode].firingSound );
        }
        else if( weapon->readySound )
        {
            trap_S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, vec3_origin, weapon->readySound );
        }
    }
    
    // Lucifer cannon charge warning beep
    // FIXME: should not send this to aliens at all, rather than just ignoring it
    if( weaponNum == WP_LUCIFER_CANNON && ( cent->currentState.eFlags & EF_WARN_CHARGE ) && cg.snap->ps.stats[ STAT_TEAM ] != TEAM_ALIENS )
    {
        trap_S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, vec3_origin, ps ? cgs.media.lCannonWarningSound : cgs.media.lCannonWarningSound2 );
    }
    
    if( !noGunModel )
    {
        idCGameEnts::PositionEntityOnTag( &gun, parent, parent->hModel, "tag_weapon" );
        WeaponAnimation( cent, &gun.oldframe, &gun.frame, &gun.backlerp );
        
        trap_R_AddRefEntityToScene( &gun );
        
        if( !ps )
        {
            barrel.hModel = weapon->barrelModel3rdPerson;
            
            if( !barrel.hModel )
            {
                barrel.hModel = weapon->barrelModel;
            }
        }
        else
        {
            barrel.hModel = weapon->barrelModel;
        }
        
        // add the spinning barrel
        if( barrel.hModel )
        {
            VectorCopy( parent->lightingOrigin, barrel.lightingOrigin );
            barrel.shadowPlane = parent->shadowPlane;
            barrel.renderfx = parent->renderfx;
            
            angles[ YAW ] = 0;
            angles[ PITCH ] = 0;
            angles[ ROLL ] = MachinegunSpinAngle( cent, firing );
            AnglesToAxis( angles, barrel.axis );
            
            idCGameEnts::PositionRotatedEntityOnTag( &barrel, &gun, gun.hModel, "tag_barrel" );
            
            trap_R_AddRefEntityToScene( &barrel );
        }
    }
    
    if( idCGameParticles::IsParticleSystemValid( &cent->muzzlePS ) )
    {
        if( ps || cg.renderingThirdPerson || cent->currentState.number != cg.predictedPlayerState.clientNum )
        {
            if( noGunModel )
            {
                idCGameAttachment::SetAttachmentTag( &cent->muzzlePS->attachment, *parent, parent->hModel, "tag_weapon" );
            }
            else
            {
                idCGameAttachment::SetAttachmentTag( &cent->muzzlePS->attachment, gun, gun.hModel, "tag_flash" );
            }
        }
        
        //if the PS is infinite disable it when not firing
        if( !firing && idCGameParticles::IsParticleSystemInfinite( cent->muzzlePS ) )
        {
            idCGameParticles::DestroyParticleSystem( &cent->muzzlePS );
        }
    }
    
    // add the flash
    if( !weapon->wim[ weaponMode ].continuousFlash || !firing )
    {
        // impulse flash
        if( cg.time - cent->muzzleFlashTime > MUZZLE_FLASH_TIME )
        {
            return;
        }
    }
    
    ::memset( &flash, 0, sizeof( flash ) );
    VectorCopy( parent->lightingOrigin, flash.lightingOrigin );
    flash.shadowPlane = parent->shadowPlane;
    flash.renderfx = parent->renderfx;
    
    if( !ps )
    {
        flash.hModel = weapon->flashModel3rdPerson;
        
        if( !flash.hModel )
        {
            flash.hModel = weapon->flashModel;
        }
    }
    else
    {
        flash.hModel = weapon->flashModel;
    }
    
    if( flash.hModel )
    {
        angles[ YAW ] = 0;
        angles[ PITCH ] = 0;
        angles[ ROLL ] = crandom( ) * 10;
        AnglesToAxis( angles, flash.axis );
        
        if( noGunModel )
        {
            idCGameEnts::PositionRotatedEntityOnTag( &flash, parent, parent->hModel, "tag_weapon" );
        }
        else
        {
            idCGameEnts::PositionRotatedEntityOnTag( &flash, &gun, gun.hModel, "tag_flash" );
        }
        
        trap_R_AddRefEntityToScene( &flash );
    }
    
    if( ps || cg.renderingThirdPerson || cent->currentState.number != cg.predictedPlayerState.clientNum )
    {
        if( weapon->wim[ weaponMode ].muzzleParticleSystem && cent->muzzlePsTrigger )
        {
            cent->muzzlePS = idCGameParticles::SpawnNewParticleSystem( weapon->wim[ weaponMode ].muzzleParticleSystem );
            
            if( idCGameParticles::IsParticleSystemValid( &cent->muzzlePS ) )
            {
                if( noGunModel )
                {
                    idCGameAttachment::SetAttachmentTag( &cent->muzzlePS->attachment, *parent, parent->hModel, "tag_weapon" );
                }
                else
                {
                    idCGameAttachment::SetAttachmentTag( &cent->muzzlePS->attachment, gun, gun.hModel, "tag_flash" );
                }
                
                idCGameAttachment::SetAttachmentCent( &cent->muzzlePS->attachment, cent );
                idCGameAttachment::AttachToTag( &cent->muzzlePS->attachment );
            }
            
            cent->muzzlePsTrigger = false;
        }
        
        // make a dlight for the flash
        if( weapon->wim[ weaponMode ].flashDlightColor[ 0 ] || weapon->wim[ weaponMode ].flashDlightColor[ 1 ] || weapon->wim[ weaponMode ].flashDlightColor[ 2 ] )
        {
            trap_R_AddLightToScene( flash.origin, -300 + ( rand() & 31 ), weapon->wim[weaponMode].flashDlightColor[0], weapon->wim[weaponMode].flashDlightColor[1], weapon->wim[weaponMode].flashDlightColor[2] );
        }
    }
}

/*
==============
idCGameWeapons::AddViewWeapon

Add the weapon, and flash for the player's view
==============
*/
#define WEAPON_CLICK_REPEAT 500
void idCGameWeapons::AddViewWeapon( playerState_t* ps )
{
    refEntity_t hand;
    centity_t* cent;
    clientInfo_t* ci;
    float32 fovOffset;
    vec3_t angles;
    weaponInfo_t*  wi;
    weapon_t weapon = ( weapon_t )ps->weapon;
    weaponMode_t weaponMode = ( weaponMode_t )ps->generic1;
    
    if( weaponMode <= WPM_NONE || weaponMode >= WPM_NUM_WEAPONMODES )
    {
        weaponMode = WPM_PRIMARY;
    }
    
    RegisterWeapon( weapon );
    wi = &cg_weapons[ weapon ];
    cent = &cg.predictedPlayerEntity; // &cg_entities[cg.snap->ps.clientNum];
    
    if( ( ps->persistant[PERS_SPECSTATE] != SPECTATOR_NOT ) || ( ps->stats[STAT_STATE] & SS_HOVELING ) )
    {
        return;
    }
    
    // no weapon carried - can't draw it
    if( weapon == WP_NONE )
    {
        return;
    }
    
    if( ps->pm_type == PM_INTERMISSION )
    {
        return;
    }
    
    // draw a prospective buildable infront of the player
    if( ( ps->stats[STAT_BUILDABLE] & ~SB_VALID_TOGGLEBIT ) > BA_NONE )
    {
        idCGameBuildable::GhostBuildable( ( buildable_t )( ps->stats[STAT_BUILDABLE] & ~SB_VALID_TOGGLEBIT ) );
    }
    
    // no gun if in third person view
    if( cg.renderingThirdPerson )
    {
        return;
    }
    
    // allow the gun to be completely removed
    if( !cg_drawGun.integer )
    {
        vec3_t origin;
        
        VectorCopy( cg.refdef.vieworg, origin );
        VectorMA( origin, -8, cg.refdef.viewaxis[ 2 ], origin );
        
        if( cent->muzzlePS )
        {
            idCGameAttachment::SetAttachmentPoint( &cent->muzzlePS->attachment, origin );
        }
        
        //check for particle systems
        if( wi->wim[ weaponMode ].muzzleParticleSystem && cent->muzzlePsTrigger )
        {
            cent->muzzlePS = idCGameParticles::SpawnNewParticleSystem( wi->wim[ weaponMode ].muzzleParticleSystem );
            
            if( idCGameParticles::IsParticleSystemValid( &cent->muzzlePS ) )
            {
                idCGameAttachment::SetAttachmentPoint( &cent->muzzlePS->attachment, origin );
                idCGameAttachment::SetAttachmentCent( &cent->muzzlePS->attachment, cent );
                idCGameAttachment::AttachToPoint( &cent->muzzlePS->attachment );
            }
            cent->muzzlePsTrigger = false;
        }
        
        return;
    }
    
    // don't draw if testing a gun model
    if( cg.testGun )
    {
        return;
    }
    
    // drop gun lower at higher fov
    if( cg.refdef.fov_y > 90 )
    {
        fovOffset = -0.4 * ( cg.refdef.fov_y - 90 );
    }
    else
    {
        fovOffset = 0;
    }
    
    ::memset( &hand, 0, sizeof( hand ) );
    
    // set up gun position
    CalculateWeaponPosition( hand.origin, angles );
    
    VectorMA( hand.origin, cg_gun_x.value, cg.refdef.viewaxis[ 0 ], hand.origin );
    VectorMA( hand.origin, cg_gun_y.value, cg.refdef.viewaxis[ 1 ], hand.origin );
    VectorMA( hand.origin, ( cg_gun_z.value + fovOffset ), cg.refdef.viewaxis[ 2 ], hand.origin );
    
    // Lucifer Cannon vibration effect
    if( weapon == WP_LUCIFER_CANNON && ps->stats[ STAT_MISC ] > 0 )
    {
        float32 fraction;
        
        fraction = ( float32 )ps->stats[ STAT_MISC ] / LCANNON_CHARGE_TIME_MAX;
        VectorMA( hand.origin, random( ) * fraction, cg.refdef.viewaxis[ 0 ], hand.origin );
        VectorMA( hand.origin, random( ) * fraction, cg.refdef.viewaxis[ 1 ], hand.origin );
    }
    
    AnglesToAxis( angles, hand.axis );
    
    // map torso animations to weapon animations
    if( cg_gun_frame.integer )
    {
        // development tool
        hand.frame = hand.oldframe = cg_gun_frame.integer;
        hand.backlerp = 0;
    }
    else
    {
        // get clientinfo for animation map
        ci = &cgs.clientinfo[ cent->currentState.clientNum ];
        hand.frame = MapTorsoToWeaponFrame( ci, cent->pe.torso.frame );
        hand.oldframe = MapTorsoToWeaponFrame( ci, cent->pe.torso.oldFrame );
        hand.backlerp = cent->pe.torso.backlerp;
    }
    
    hand.hModel = wi->handsModel;
    hand.renderfx = RF_DEPTHHACK | RF_FIRST_PERSON | RF_MINLIGHT;
    
    // add everything onto the hand
    AddPlayerWeapon( &hand, ps, &cg.predictedPlayerEntity );
}

/*
==============================================================================
WEAPON SELECTION
==============================================================================
*/

/*
===============
idCGameWeapons::WeaponSelectable
===============
*/
bool idCGameWeapons::WeaponSelectable( weapon_t weapon )
{
    if( !bggame->InventoryContainsWeapon( weapon, cg.snap->ps.stats ) )
    {
        return false;
    }
    
    return true;
}

/*
===============
idCGameWeapons::UpgradeSelectable
===============
*/
bool idCGameWeapons::UpgradeSelectable( upgrade_t upgrade )
{
    if( !bggame->InventoryContainsUpgrade( upgrade, cg.snap->ps.stats ) )
    {
        return false;
    }
    
    return bggame->Upgrade( upgrade )->usable;
}

/*
===================
idCGameWeapons::DrawItemSelect
===================
*/
#define ICON_BORDER 4
void idCGameWeapons::DrawItemSelect( rectDef_t* rect, vec4_t color )
{
    sint i;
    float32 x = rect->x;
    float32 y = rect->y;
    float32 width = rect->w;
    float32 height = rect->h;
    float32 iconWidth;
    float32 iconHeight;
    sint items[ 64 ];
    sint colinfo[ 64 ];
    sint numItems = 0, selectedItem = 0;
    sint length;
    bool vertical;
    centity_t* cent;
    playerState_t* ps;
    
    cent = &cg_entities[ cg.snap->ps.clientNum ];
    ps = &cg.snap->ps;
    
    // don't display if dead
    if( cg.predictedPlayerState.stats[STAT_HEALTH] <= 0 )
    {
        return;
    }
    
    if( !( cg.snap->ps.pm_flags & PMF_FOLLOW ) )
    {
        // first make sure that whatever it selected is actually selectable
        if( cg.weaponSelect <= 32 && !WeaponSelectable( ( weapon_t )cg.weaponSelect ) )
        {
            NextWeapon_f();
        }
        else if( cg.weaponSelect > 32 && !UpgradeSelectable( ( upgrade_t )( cg.weaponSelect - 32 ) ) )
        {
            NextWeapon_f();
        }
    }
    
    // showing weapon select clears pickup item display, but not the blend blob
    cg.itemPickupTime = 0;
    
    // put all weapons in the items list
    for( i = WP_NONE + 1; i < WP_NUM_WEAPONS; i++ )
    {
        if( !bggame->InventoryContainsWeapon( i, cg.snap->ps.stats ) )
        {
            continue;
        }
        
        if( !ps->ammo && !ps->clips && !bggame->Weapon( ( weapon_t )i )->infiniteAmmo )
        {
            colinfo[numItems] = 1;
        }
        else
        {
            colinfo[numItems] = 0;
        }
        
        if( i == cg.weaponSelect )
        {
            selectedItem = numItems;
        }
        
        RegisterWeapon( i );
        items[ numItems ] = i;
        numItems++;
    }
    
    // put all upgrades in the weapons list
    for( i = UP_NONE + 1; i < UP_NUM_UPGRADES; i++ )
    {
        if( !bggame->InventoryContainsUpgrade( i, cg.snap->ps.stats ) )
        {
            continue;
        }
        
        colinfo[ numItems ] = 0;
        if( !bggame->Upgrade( ( upgrade_t )i )->usable )
        {
            colinfo[numItems] = 2;
        }
        
        if( i == cg.weaponSelect - 32 )
        {
            selectedItem = numItems;
        }
        
        RegisterUpgrade( i );
        items[ numItems ] = i + 32;
        numItems++;
    }
    
    // compute the length of the display window and determine orientation
    vertical = height > width;
    if( vertical )
    {
        iconWidth = width * cgDC.aspectScale;
        iconHeight = width;
        length = height / ( width * cgDC.aspectScale );
    }
    else
    {
        iconWidth = height * cgDC.aspectScale;
        iconHeight = height;
        length = width / ( height * cgDC.aspectScale );
    }
    
    // render icon ring
    for( i = 0; i < length; i++ )
    {
        sint item = i - length / 2 + selectedItem;
        
        if( item < 0 )
        {
            item += length;
        }
        else if( item >= length )
        {
            item -= length;
        }
        
        if( item >= 0 && item < numItems )
        {
            switch( colinfo[ item ] )
            {
                case 0:
                    color = colorCyan;
                    break;
                case 1:
                    color = colorRed;
                    break;
                case 2:
                    color = colorMdGrey;
                    break;
            }
            color[3] = 0.5;
            
            trap_R_SetColor( color );
            
            if( items[item] <= 32 )
            {
                idCGameDrawTools::DrawPic( x, y, iconWidth, iconHeight, cg_weapons[items[item]].weaponIcon );
            }
            else if( items[item] > 32 )
            {
                idCGameDrawTools::DrawPic( x, y, iconWidth, iconHeight, cg_upgrades[items[item] - 32].upgradeIcon );
            }
        }
        
        if( vertical )
        {
            y += iconHeight;
        }
        else
        {
            x += iconWidth;
        }
    }
    
    trap_R_SetColor( nullptr );
}

/*
===================
idCGameWeapons::DrawItemSelectText
===================
*/
void idCGameWeapons::DrawItemSelectText( rectDef_t* rect, float32 scale, sint textStyle )
{
    sint x, w;
    valueType* name;
    float32* color;
    
    color = idCGameDrawTools::FadeColor( cg.weaponSelectTime, WEAPON_SELECT_TIME );
    if( !color )
    {
        return;
    }
    
    trap_R_SetColor( color );
    
    // draw the selected name
    if( cg.weaponSelect <= 32 )
    {
        if( cg_weapons[ cg.weaponSelect ].registered && bggame->InventoryContainsWeapon( cg.weaponSelect, cg.snap->ps.stats ) )
        {
            if( ( name = cg_weapons[ cg.weaponSelect ].humanName ) )
            {
                w = UI_Text_Width( name, scale, 0 );
                x = rect->x + rect->w / 2;
                UI_Text_Paint( x - w / 2, rect->y + rect->h, scale, color, name, 0, 0, textStyle );
            }
        }
    }
    else if( cg.weaponSelect > 32 )
    {
        if( cg_upgrades[ cg.weaponSelect - 32 ].registered && bggame->InventoryContainsUpgrade( cg.weaponSelect - 32, cg.snap->ps.stats ) )
        {
            if( ( name = cg_upgrades[ cg.weaponSelect - 32 ].humanName ) )
            {
                w = UI_Text_Width( name, scale, 0 );
                x = rect->x + rect->w / 2;
                UI_Text_Paint( x - w / 2, rect->y + rect->h, scale, color, name, 0, 0, textStyle );
            }
        }
    }
    
    trap_R_SetColor( nullptr );
}

/*
===============
idCGameWeapons::NextWeapon_f
===============
*/
void idCGameWeapons::NextWeapon_f( void )
{
    sint i, original;
    
    if( !cg.snap )
    {
        return;
    }
    
    if( cg.snap->ps.pm_flags & PMF_FOLLOW )
    {
        trap_SendClientCommand( "followprev\n" );
        return;
    }
    
    cg.weaponSelectTime = cg.time;
    original = cg.weaponSelect;
    
    for( i = 0; i < 64; i++ )
    {
        cg.weaponSelect++;
        
        if( cg.weaponSelect == 64 )
        {
            cg.weaponSelect = 0;
        }
        
        if( cg.weaponSelect <= 32 )
        {
            if( WeaponSelectable( ( weapon_t )cg.weaponSelect ) )
                break;
        }
        else if( cg.weaponSelect > 32 )
        {
            if( UpgradeSelectable( ( upgrade_t )( cg.weaponSelect - 32 ) ) )
            {
                break;
            }
        }
    }
    
    if( i == 64 )
    {
        cg.weaponSelect = original;
    }
}

/*
===============
idCGameWeapons::PrevWeapon_f
===============
*/
void idCGameWeapons::PrevWeapon_f( void )
{
    sint i, original;
    
    if( !cg.snap )
    {
        return;
    }
    
    if( cg.snap->ps.pm_flags & PMF_FOLLOW )
    {
        trap_SendClientCommand( "follownext\n" );
        return;
    }
    
    cg.weaponSelectTime = cg.time;
    original = cg.weaponSelect;
    
    for( i = 0; i < 64; i++ )
    {
        cg.weaponSelect--;
        
        if( cg.weaponSelect == -1 )
        {
            cg.weaponSelect = 63;
        }
        
        if( cg.weaponSelect <= 32 )
        {
            if( WeaponSelectable( ( weapon_t )cg.weaponSelect ) )
            {
                break;
            }
        }
        else if( cg.weaponSelect > 32 )
        {
            if( UpgradeSelectable( ( upgrade_t )( cg.weaponSelect - 32 ) ) )
            {
                break;
            }
        }
    }
    
    if( i == 64 )
    {
        cg.weaponSelect = original;
    }
}

/*
===============
idCGameWeapons::Weapon_f
===============
*/
void idCGameWeapons::Weapon_f( void )
{
    sint num;
    
    if( !cg.snap )
    {
        return;
    }
    
    if( cg.snap->ps.pm_flags & PMF_FOLLOW )
    {
        return;
    }
    
    num = atoi( idCGameMain::Argv( 1 ) );
    
    if( num < 1 || num > 31 )
    {
        return;
    }
    
    cg.weaponSelectTime = cg.time;
    
    if( !bggame->InventoryContainsWeapon( num, cg.snap->ps.stats ) )
    {
        return; // don't have the weapon
    }
    
    cg.weaponSelect = num;
}

/*
===================================================================================================
WEAPON EVENTS
===================================================================================================
*/

/*
================
idCGameWeapons::FireWeapon

Caused by an EV_FIRE_WEAPON event
================
*/
void idCGameWeapons::FireWeapon( centity_t* cent, weaponMode_t weaponMode )
{
    entityState_t* es;
    sint c;
    weaponInfo_t* wi;
    weapon_t weaponNum;
    
    es = &cent->currentState;
    
    weaponNum = ( weapon_t )es->weapon;
    
    if( weaponNum == WP_NONE )
    {
        return;
    }
    
    if( weaponMode <= WPM_NONE || weaponMode >= WPM_NUM_WEAPONMODES )
    {
        weaponMode = WPM_PRIMARY;
    }
    
    if( weaponNum >= WP_NUM_WEAPONS )
    {
        Error( "idCGameLocal::FireWeapon: ent->weapon >= WP_NUM_WEAPONS" );
        return;
    }
    
    wi = &cg_weapons[ weaponNum ];
    
    // mark the entity as muzzle flashing, so when it is added it will
    // append the flash to the weapon model
    cent->muzzleFlashTime = cg.time;
    
    if( wi->wim[ weaponMode ].muzzleParticleSystem )
    {
        if( !idCGameParticles::IsParticleSystemValid( &cent->muzzlePS ) || !idCGameParticles::IsParticleSystemInfinite( cent->muzzlePS ) )
        {
            cent->muzzlePsTrigger = true;
        }
    }
    
    // play a sound
    for( c = 0; c < 4; c++ )
    {
        if( !wi->wim[ weaponMode ].flashSound[ c ] )
            break;
    }
    
    if( c > 0 )
    {
        c = rand( ) % c;
        if( wi->wim[weaponMode].flashSound[c] )
        {
            trap_S_StartSound( nullptr, es->number, CHAN_WEAPON, wi->wim[weaponMode].flashSound[c] );
        }
    }
}

/*
=================
idCGameWeapons::HandleAlienFeedback

Caused by an EV_ALIEN_HIT, EV_ALIEN_MISS, or EV_ALIEN_TEAMHIT event. Used to cause a ui feedback
effect for visual information about a hit
=================
*/
void idCGameWeapons::HandleAlienFeedback( centity_t* cent, alienFeedback_t feedbackType )
{
    entityState_t* es;
    
    es = &cent->currentState;
    
    // show the alien feedback, if the entity matches this player
    if( es->number == cg.predictedPlayerState.clientNum )
    {
        cg.feedbackAnimation = 1;
        cg.feedbackAnimationType = feedbackType;
    }
}

/*
=================
idCGameWeapons::MissileHitWall

Caused by an EV_MISSILE_MISS event, or directly by local bullet tracing
=================
*/
void idCGameWeapons::MissileHitWall( weapon_t weaponNum, weaponMode_t weaponMode, sint clientNum, vec3_t origin, vec3_t dir, impactSound_t soundType, sint charge )
{
    qhandle_t mark = 0, ps = 0;
    sint c;
    float32 radius = 1.0f;
    weaponInfo_t* weapon = &cg_weapons[ weaponNum ];
    
    if( weaponMode <= WPM_NONE || weaponMode >= WPM_NUM_WEAPONMODES )
    {
        weaponMode = WPM_PRIMARY;
    }
    
    mark = weapon->wim[ weaponMode ].impactMark;
    radius = weapon->wim[ weaponMode ].impactMarkSize;
    ps = weapon->wim[ weaponMode ].impactParticleSystem;
    
    if( soundType == IMPACTSOUND_FLESH )
    {
        //flesh sound
        for( c = 0; c < 4; c++ )
        {
            if( !weapon->wim[weaponMode].impactFleshSound[c] )
            {
                break;
            }
        }
        
        if( c > 0 )
        {
            c = rand( ) % c;
            
            if( weapon->wim[weaponMode].impactFleshSound[c] )
            {
                trap_S_StartSound( origin, ENTITYNUM_WORLD, CHAN_AUTO, weapon->wim[weaponMode].impactFleshSound[c] );
            }
        }
    }
    else
    {
        //normal sound
        for( c = 0; c < 4; c++ )
        {
            if( !weapon->wim[weaponMode].impactSound[c] )
            {
                break;
            }
        }
        
        if( c > 0 )
        {
            c = rand( ) % c;
            
            if( weapon->wim[weaponMode].impactSound[c] )
            {
                trap_S_StartSound( origin, ENTITYNUM_WORLD, CHAN_AUTO, weapon->wim[weaponMode].impactSound[c] );
            }
        }
    }
    
    //create impact particle system
    if( ps )
    {
        particleSystem_t* partSystem = idCGameParticles::SpawnNewParticleSystem( ps );
        
        if( idCGameParticles::IsParticleSystemValid( &partSystem ) )
        {
            idCGameAttachment::SetAttachmentPoint( &partSystem->attachment, origin );
            idCGameParticles::SetParticleSystemNormal( partSystem, dir );
            idCGameAttachment::AttachToPoint( &partSystem->attachment );
            partSystem->charge = charge;
        }
    }
    
    // impact mark
    if( radius > 0.0f )
    {
        idCGameMarks::ImpactMark( mark, origin, dir, random() * 360, 1, 1, 1, 1, false, radius, false );
    }
}

/*
=================
idCGameWeapons::MissileHitPlayer
=================
*/
void idCGameWeapons::MissileHitPlayer( weapon_t weaponNum, weaponMode_t weaponMode, vec3_t origin, vec3_t dir, sint entityNum, sint charge )
{
    vec3_t normal;
    weaponInfo_t* weapon = &cg_weapons[ weaponNum ];
    
    VectorCopy( dir, normal );
    VectorInverse( normal );
    
    idCGamePlayers::Bleed( origin, normal, entityNum );
    
    if( weaponMode <= WPM_NONE || weaponMode >= WPM_NUM_WEAPONMODES )
        weaponMode = WPM_PRIMARY;
        
    if( weapon->wim[ weaponMode ].alwaysImpact )
    {
        //note to self: this is why i shouldn't code while sleepy
        sint sound = IMPACTSOUND_DEFAULT;
        if( cg_entities[entityNum].currentState.eType == ET_PLAYER || ( cg_entities[entityNum].currentState.eType == ET_BUILDABLE &&
                bggame->Buildable( ( buildable_t )cg_entities[entityNum].currentState.modelindex )->team == TEAM_ALIENS ) )
        {
            sound = IMPACTSOUND_FLESH;
        }
        
        MissileHitWall( weaponNum, weaponMode, 0, origin, dir, ( impactSound_t )sound, charge );
    }
}

/*
==============
idCGameWeapons::MassDriverFire

Draws the mass driver trail
==============
*/
#define MDRIVER_MUZZLE_OFFSET 48.f
void idCGameWeapons::MassDriverFire( entityState_t* es )
{
    vec3_t front, frontToBack;
    trailSystem_t* ts;
    float32 length;
    
    ts = idCGameTrails::SpawnNewTrailSystem( cgs.media.massDriverTS );
    if( !idCGameTrails::IsTrailSystemValid( &ts ) )
    {
        return;
    }
    
    // trail front attaches to the player, needs to be pushed forward a bit
    // so that it doesn't look like it shot out of the wrong location
    VectorCopy( es->origin2, front );
    VectorSubtract( es->pos.trBase, front, frontToBack );
    
    length = VectorLength( frontToBack );
    if( length - MDRIVER_MUZZLE_OFFSET < 0.f )
    {
        return;
    }
    
    VectorScale( frontToBack, 1 / length, frontToBack );
    VectorMA( front, MDRIVER_MUZZLE_OFFSET, frontToBack, front );
    idCGameAttachment::SetAttachmentPoint( &ts->frontAttachment, front );
    idCGameAttachment::AttachToPoint( &ts->frontAttachment );
    
    // trail back attaches to the impact point
    idCGameAttachment::SetAttachmentPoint( &ts->backAttachment, es->pos.trBase );
    idCGameAttachment::AttachToPoint( &ts->backAttachment );
}


/*
============================================================================
BULLETS
============================================================================
*/

/*
===============
idCGameWeapons::Tracer
===============
*/
void idCGameWeapons::Tracer( vec3_t source, vec3_t dest )
{
    vec3_t forward, right;
    polyVert_t verts[ 4 ];
    vec3_t line;
    float32 len, begin, end;
    vec3_t  start, finish, midpoint;
    
    // tracer
    VectorSubtract( dest, source, forward );
    len = VectorNormalize( forward );
    
    // start at least a little ways from the muzzle
    if( len < 100 )
    {
        return;
    }
    
    begin = 50 + random( ) * ( len - 60 );
    end = begin + cg_tracerLength.value;
    if( end > len )
    {
        end = len;
    }
    
    VectorMA( source, begin, forward, start );
    VectorMA( source, end, forward, finish );
    
    line[ 0 ] = DotProduct( forward, cg.refdef.viewaxis[ 1 ] );
    line[ 1 ] = DotProduct( forward, cg.refdef.viewaxis[ 2 ] );
    
    VectorScale( cg.refdef.viewaxis[ 1 ], line[ 1 ], right );
    VectorMA( right, -line[ 0 ], cg.refdef.viewaxis[ 2 ], right );
    VectorNormalize( right );
    
    VectorMA( finish, cg_tracerWidth.value, right, verts[ 0 ].xyz );
    verts[ 0 ].st[ 0 ] = 0;
    verts[ 0 ].st[ 1 ] = 1;
    verts[ 0 ].modulate[ 0 ] = 255;
    verts[ 0 ].modulate[ 1 ] = 255;
    verts[ 0 ].modulate[ 2 ] = 255;
    verts[ 0 ].modulate[ 3 ] = 255;
    
    VectorMA( finish, -cg_tracerWidth.value, right, verts[ 1 ].xyz );
    verts[ 1 ].st[ 0 ] = 1;
    verts[ 1 ].st[ 1 ] = 0;
    verts[ 1 ].modulate[ 0 ] = 255;
    verts[ 1 ].modulate[ 1 ] = 255;
    verts[ 1 ].modulate[ 2 ] = 255;
    verts[ 1 ].modulate[ 3 ] = 255;
    
    VectorMA( start, -cg_tracerWidth.value, right, verts[ 2 ].xyz );
    verts[ 2 ].st[ 0 ] = 1;
    verts[ 2 ].st[ 1 ] = 1;
    verts[ 2 ].modulate[ 0 ] = 255;
    verts[ 2 ].modulate[ 1 ] = 255;
    verts[ 2 ].modulate[ 2 ] = 255;
    verts[ 2 ].modulate[ 3 ] = 255;
    
    VectorMA( start, cg_tracerWidth.value, right, verts[ 3 ].xyz );
    verts[ 3 ].st[ 0 ] = 0;
    verts[ 3 ].st[ 1 ] = 0;
    verts[ 3 ].modulate[ 0 ] = 255;
    verts[ 3 ].modulate[ 1 ] = 255;
    verts[ 3 ].modulate[ 2 ] = 255;
    verts[ 3 ].modulate[ 3 ] = 255;
    
    trap_R_AddPolyToScene( cgs.media.tracerShader, 4, verts );
    
    midpoint[ 0 ] = ( start[ 0 ] + finish[ 0 ] ) * 0.5;
    midpoint[ 1 ] = ( start[ 1 ] + finish[ 1 ] ) * 0.5;
    midpoint[ 2 ] = ( start[ 2 ] + finish[ 2 ] ) * 0.5;
    
    // add the tracer sound
    trap_S_StartSound( midpoint, ENTITYNUM_WORLD, CHAN_AUTO, cgs.media.tracerSound );
}

/*
======================
idCGameWeapons::CalcMuzzlePoint
======================
*/
bool idCGameWeapons::CalcMuzzlePoint( sint entityNum, vec3_t muzzle )
{
    vec3_t forward;
    centity_t* cent;
    sint anim;
    
    if( entityNum == cg.snap->ps.clientNum )
    {
        VectorCopy( cg.snap->ps.origin, muzzle );
        muzzle[ 2 ] += cg.snap->ps.viewheight;
        AngleVectors( cg.snap->ps.viewangles, forward, nullptr, nullptr );
        VectorMA( muzzle, 14, forward, muzzle );
        return true;
    }
    
    cent = &cg_entities[entityNum];
    
    if( !cent->currentValid )
    {
        return false;
    }
    
    VectorCopy( cent->currentState.pos.trBase, muzzle );
    
    AngleVectors( cent->currentState.apos.trBase, forward, nullptr, nullptr );
    anim = cent->currentState.legsAnim & ~ANIM_TOGGLEBIT;
    
    if( anim == LEGS_WALKCR || anim == LEGS_IDLECR )
    {
        muzzle[2] += CROUCH_VIEWHEIGHT;
    }
    else
    {
        muzzle[2] += DEFAULT_VIEWHEIGHT;
    }
    
    VectorMA( muzzle, 14, forward, muzzle );
    
    return true;
}

/*
======================
idCGameWeapons::Bullet

Renders bullet effects.
======================
*/
void idCGameWeapons::Bullet( vec3_t end, sint sourceEntityNum, vec3_t normal, bool flesh, sint fleshEntityNum )
{
    vec3_t start;
    
    // if the shooter is currently valid, calc a source point and possibly
    // do trail effects
    if( sourceEntityNum >= 0 && cg_tracerChance.value > 0 )
    {
        if( CalcMuzzlePoint( sourceEntityNum, start ) )
        {
            // draw a tracer
            if( random() < cg_tracerChance.value )
            {
                Tracer( start, end );
            }
        }
    }
    
    // impact splash and mark
    if( flesh )
    {
        idCGamePlayers::Bleed( end, normal, fleshEntityNum );
    }
    else
    {
        MissileHitWall( WP_MACHINEGUN, WPM_PRIMARY, 0, end, normal, IMPACTSOUND_DEFAULT, 0 );
    }
}

/*
============================================================================
SHOTGUN TRACING
============================================================================
*/

/*
================
idCGameWeapons::ShotgunPattern

Perform the same traces the server did to locate the
hit splashes
================
*/
void idCGameWeapons::ShotgunPattern( vec3_t origin, vec3_t origin2, sint seed, sint otherEntNum )
{
    sint i;
    float32 r, u;
    vec3_t end, forward, right, up;
    trace_t tr;
    
    // derive the right and up vectors from the forward vector, because
    // the client won't have any other information
    VectorNormalize2( origin2, forward );
    PerpendicularVector( right, forward );
    CrossProduct( forward, right, up );
    
    // generate the "random" spread pattern
    for( i = 0; i < SHOTGUN_PELLETS; i++ )
    {
        r = Q_crandom( &seed ) * SHOTGUN_SPREAD * 16;
        u = Q_crandom( &seed ) * SHOTGUN_SPREAD * 16;
        VectorMA( origin, 8192 * 16, forward, end );
        VectorMA( end, r, right, end );
        VectorMA( end, u, up, end );
        
        idCGamePredict::Trace( &tr, origin, nullptr, nullptr, end, otherEntNum, MASK_SHOT );
        
        if( !( tr.surfaceFlags & SURF_NOIMPACT ) )
        {
            if( cg_entities[tr.entityNum].currentState.eType == ET_PLAYER )
            {
                MissileHitPlayer( WP_SHOTGUN, WPM_PRIMARY, tr.endpos, tr.plane.normal, tr.entityNum, 0 );
            }
            else if( tr.surfaceFlags & SURF_METALSTEPS )
            {
                MissileHitWall( WP_SHOTGUN, WPM_PRIMARY, 0, tr.endpos, tr.plane.normal, IMPACTSOUND_METAL, 0 );
            }
            else
            {
                MissileHitWall( WP_SHOTGUN, WPM_PRIMARY, 0, tr.endpos, tr.plane.normal, IMPACTSOUND_DEFAULT, 0 );
            }
        }
    }
}

/*
==============
idCGameWeapons::ShotgunFire
==============
*/
void idCGameWeapons::ShotgunFire( entityState_t* es )
{
    vec3_t  v;
    
    VectorSubtract( es->origin2, es->pos.trBase, v );
    VectorNormalize( v );
    VectorScale( v, 32, v );
    VectorAdd( es->pos.trBase, v, v );
    
    ShotgunPattern( es->pos.trBase, es->origin2, es->eventParm, es->otherEntityNum );
}
