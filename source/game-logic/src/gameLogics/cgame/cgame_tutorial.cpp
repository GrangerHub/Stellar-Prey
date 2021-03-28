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
// File name:   cgame_tutorial.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description: the tutorial system
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <cgame/cgame_precompiled.hpp>

typedef struct
{
    valueType* command;
    valueType* humanName;
    keyNum_t keys[ 2 ];
} bind_t;

static bind_t bindings[ ] =
{
    { "+button2",       "Activate Upgrade",       { ( keyNum_t ) - 1, ( keyNum_t ) - 1 } },
    { "+speed",         "Run/Walk",               { ( keyNum_t ) - 1, ( keyNum_t ) - 1 } },
    { "+button6",       "Sprint/Dodge",           { ( keyNum_t ) - 1, ( keyNum_t ) - 1 } },
    { "+moveup",        "Jump",                   { ( keyNum_t ) - 1, ( keyNum_t ) - 1 } },
    { "+movedown",      "Crouch",                 { ( keyNum_t ) - 1, ( keyNum_t ) - 1 } },
    { "+attack",        "Primary Attack",         { ( keyNum_t ) - 1, ( keyNum_t ) - 1 } },
    { "+button5",       "Secondary Attack",       { ( keyNum_t ) - 1, ( keyNum_t ) - 1 } },
    { "reload",         "Reload",                 { ( keyNum_t ) - 1, ( keyNum_t ) - 1 } },
    { "buy ammo",       "Buy Ammo",               { ( keyNum_t ) - 1, ( keyNum_t ) - 1 } },
    { "itemact medkit", "Use Medkit",             { ( keyNum_t ) - 1, ( keyNum_t ) - 1 } },
    { "+button7",       "Use Structure/Evolve",   { ( keyNum_t ) - 1, ( keyNum_t ) - 1 } },
    { "deconstruct",    "Deconstruct Structure",  { ( keyNum_t ) - 1, ( keyNum_t ) - 1 } },
    { "weapprev",       "Previous Upgrade",       { ( keyNum_t ) - 1, ( keyNum_t ) - 1 } },
    { "weapnext",       "Next Upgrade",           { ( keyNum_t ) - 1, ( keyNum_t ) - 1 } }
};

static const sint numBindings = sizeof( bindings ) / sizeof( bind_t );

/*
===============
idCGameTutorial::idCGameTutorial
===============
*/
idCGameTutorial::idCGameTutorial( void )
{
}

/*
===============
idCGameTutorial::~idCGameTutorial
===============
*/
idCGameTutorial::~idCGameTutorial( void )
{
}

/*
=================
idCGameTutorial::GetBindings
=================
*/
void idCGameTutorial::GetBindings( void )
{
    sint i, j, numKeys;
    valueType buffer[ MAX_STRING_CHARS ];
    
    for( i = 0; i < numBindings; i++ )
    {
        bindings[ i ].keys[ 0 ] = bindings[ i ].keys[ 1 ] = K_NONE;
        numKeys = 0;
        
        for( j = 0; j < K_LAST_KEY; j++ )
        {
            trap_Key_GetBindingBuf( j, buffer, MAX_STRING_CHARS );
            
            if( buffer[0] == 0 )
            {
                continue;
            }
            
            if( !Q_stricmp( buffer, bindings[ i ].command ) )
            {
                bindings[ i ].keys[ numKeys++ ] = ( keyNum_t )j;
                
                if( numKeys > 1 )
                {
                    break;
                }
            }
        }
    }
}

/*
=================
idCGameTutorial::FixBindings

Fix people who have "boost" bound instead of "+button6"
Could also extend this function for future bind changes
=================
*/
void idCGameTutorial::FixBindings( void )
{
    sint i;
    valueType buffer[ MAX_STRING_CHARS ];
    
    for( i = 0; i < K_LAST_KEY; i++ )
    {
        trap_Key_GetBindingBuf( i, buffer, sizeof( buffer ) );
        
        if( !Q_stricmp( buffer, "boost" ) )
        {
            trap_Key_SetBinding( i, "+button6" );
        }
    }
}

/*
===============
idCGameTutorial::KeyNameForCommand
===============
*/
pointer idCGameTutorial::KeyNameForCommand( pointer command )
{
    sint i, j, firstKeyLength;
    static valueType buffer[ MAX_STRING_CHARS ];
    
    buffer[ 0 ] = '\0';
    
    for( i = 0; i < numBindings; i++ )
    {
        if( !Q_stricmp( command, bindings[ i ].command ) )
        {
            if( bindings[ i ].keys[ 0 ] != K_NONE )
            {
                trap_Key_KeynumToStringBuf( bindings[ i ].keys[ 0 ], buffer, MAX_STRING_CHARS );
                firstKeyLength = strlen( buffer );
                
                for( j = 0; j < firstKeyLength; j++ )
                {
                    buffer[j] = toupper( buffer[j] );
                }
                
                if( bindings[ i ].keys[ 1 ] != K_NONE )
                {
                    Q_strcat( buffer, MAX_STRING_CHARS, " or " );
                    trap_Key_KeynumToStringBuf( bindings[ i ].keys[ 1 ], buffer + strlen( buffer ), MAX_STRING_CHARS - strlen( buffer ) );
                    
                    for( j = firstKeyLength + 4; j < strlen( buffer ); j++ )
                    {
                        buffer[j] = toupper( buffer[j] );
                    }
                }
            }
            else
            {
                Q_vsprintf_s( buffer, MAX_STRING_CHARS, MAX_STRING_CHARS, "\"%s\" (unbound)", bindings[ i ].humanName );
            }
            
            return buffer;
        }
    }
    
    return "";
}

#define MAX_TUTORIAL_TEXT 4096

/*
===============
idCGameTutorial::BuildableInRange
===============
*/
entityState_t* idCGameTutorial::BuildableInRange( playerState_t* ps, float32* healthFraction )
{
    vec3_t view, point;
    trace_t trace;
    entityState_t* es;
    sint health;
    
    AngleVectors( cg.refdefViewAngles, view, nullptr, nullptr );
    VectorMA( cg.refdef.vieworg, 64, view, point );
    idCGamePredict::Trace( &trace, cg.refdef.vieworg, nullptr, nullptr, point, ps->clientNum, MASK_SHOT );
    
    es = &cg_entities[ trace.entityNum ].currentState;
    
    if( healthFraction )
    {
        health = ( uchar8 )es->generic1;
        *healthFraction = ( float32 )health / B_HEALTH_MASK;
    }
    
    if( es->eType == ET_BUILDABLE && ps->stats[STAT_TEAM] == bggame->Buildable( ( buildable_t )es->modelindex )->team )
    {
        return es;
    }
    else
    {
        return nullptr;
    }
}

/*
===============
idCGameTutorial::AlienBuilderText
===============
*/
void idCGameTutorial::AlienBuilderText( valueType* text, playerState_t* ps )
{
    buildable_t buildable = ( buildable_t )( ps->stats[ STAT_BUILDABLE ] & ~SB_VALID_TOGGLEBIT );
    entityState_t* es;
    
    if( buildable > BA_NONE )
    {
        Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to place the %s\n", KeyNameForCommand( "+attack" ), bggame->Buildable( buildable )->humanName ) );
        
        Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to cancel placing the %s\n", KeyNameForCommand( "+button5" ), bggame->Buildable( buildable )->humanName ) );
    }
    else
    {
        Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to build a structure\n", KeyNameForCommand( "+attack" ) ) );
    }
    
    if( ( es = BuildableInRange( ps, nullptr ) ) )
    {
        if( cgs.markDeconstruct )
        {
            if( es->eFlags & EF_B_MARKED )
            {
                Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to unmark this structure\n", KeyNameForCommand( "deconstruct" ) ) );
            }
            else
            {
                Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to mark this structure\n", KeyNameForCommand( "deconstruct" ) ) );
            }
        }
        else
        {
            Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to destroy this structure\n", KeyNameForCommand( "deconstruct" ) ) );
        }
    }
    
    if( ps->stats[ STAT_CLASS ] == PCL_ALIEN_BUILDER0 || ps->stats[ STAT_CLASS ] == PCL_ALIEN_BUILDER0_UPG )
    {
        if( ( ps->stats[ STAT_BUILDABLE ] & ~SB_VALID_TOGGLEBIT ) == BA_NONE )
        {
            Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to swipe\n", KeyNameForCommand( "+button5" ) ) );
        }
        
        Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to launch a projectile\n", KeyNameForCommand( "+button2" ) ) );
        
        Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to walk on walls\n", KeyNameForCommand( "+movedown" ) ) );
    }
}

/*
===============
idCGameTutorial::AlienLevel0Text
===============
*/
void idCGameTutorial::AlienLevel0Text( valueType* text, playerState_t* ps )
{
    Q_strcat( text, MAX_TUTORIAL_TEXT, "Touch a human to damage it\n" );
    
    Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to walk on walls\n", KeyNameForCommand( "+movedown" ) ) );
}

/*
===============
idCGameTutorial::AlienLevel1Text
===============
*/
void idCGameTutorial::AlienLevel1Text( valueType* text, playerState_t* ps )
{
    Q_strcat( text, MAX_TUTORIAL_TEXT, "Touch a human to grab it\n" );
    
    Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to swipe\n", KeyNameForCommand( "+attack" ) ) );
    
    if( ps->stats[ STAT_CLASS ] == PCL_ALIEN_LEVEL1_UPG )
    {
        Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to spray poisonous gas\n", KeyNameForCommand( "+button5" ) ) );
    }
    
    Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to walk on walls\n", KeyNameForCommand( "+movedown" ) ) );
}

/*
===============
idCGameTutorial::AlienLevel2Text
===============
*/
void idCGameTutorial::AlienLevel2Text( valueType* text, playerState_t* ps )
{
    Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to bite\n", KeyNameForCommand( "+attack" ) ) );
    
    if( ps->stats[ STAT_CLASS ] == PCL_ALIEN_LEVEL2_UPG )
    {
        Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to invoke an electrical attack\n", KeyNameForCommand( "+button5" ) ) );
    }
    
    Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Hold down %s then touch a wall to wall jump\n", KeyNameForCommand( "+moveup" ) ) );
}

/*
===============
idCGameTutorial::AlienLevel3Text
===============
*/
void idCGameTutorial::AlienLevel3Text( valueType* text, playerState_t* ps )
{
    Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to bite\n", KeyNameForCommand( "+attack" ) ) );
    
    if( ps->stats[ STAT_CLASS ] == PCL_ALIEN_LEVEL3_UPG )
    {
        Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to launch a projectile\n", KeyNameForCommand( "+button2" ) ) );
    }
    
    Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Hold down and release %s to pounce\n", KeyNameForCommand( "+button5" ) ) );
}

/*
===============
idCGameTutorial::AlienLevel4Text
===============
*/
void idCGameTutorial::AlienLevel4Text( valueType* text, playerState_t* ps )
{
    Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to swipe\n", KeyNameForCommand( "+attack" ) ) );
    
    Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Hold down and release %s to trample\n", KeyNameForCommand( "+button5" ) ) );
}

/*
===============
idCGameTutorial::HumanCkitText
===============
*/
void idCGameTutorial::HumanCkitText( valueType* text, playerState_t* ps )
{
    buildable_t buildable = ( buildable_t )( ps->stats[ STAT_BUILDABLE ] & ~SB_VALID_TOGGLEBIT );
    entityState_t* es;
    
    if( buildable > BA_NONE )
    {
        Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to place the %s\n", KeyNameForCommand( "+attack" ), bggame->Buildable( buildable )->humanName ) );
        
        Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to cancel placing the %s\n", KeyNameForCommand( "+button5" ), bggame->Buildable( buildable )->humanName ) );
    }
    else
    {
        Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to build a structure\n", KeyNameForCommand( "+attack" ) ) );
    }
    
    if( ( es = BuildableInRange( ps, nullptr ) ) )
    {
        if( cgs.markDeconstruct )
        {
            if( es->eFlags & EF_B_MARKED )
            {
                Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to unmark this structure\n", KeyNameForCommand( "deconstruct" ) ) );
            }
            else
            {
                Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to mark this structure\n", KeyNameForCommand( "deconstruct" ) ) );
            }
        }
        else
        {
            Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to destroy this structure\n", KeyNameForCommand( "deconstruct" ) ) );
        }
    }
}

/*
===============
idCGameTutorial::HumanText
===============
*/
void idCGameTutorial::HumanText( valueType* text, playerState_t* ps )
{
    valueType* name;
    upgrade_t upgrade = UP_NONE;
    
    if( cg.weaponSelect <= 32 )
    {
        name = cg_weapons[cg.weaponSelect].humanName;
    }
    else if( cg.weaponSelect > 32 )
    {
        name = cg_upgrades[ cg.weaponSelect - 32 ].humanName;
        upgrade = ( upgrade_t )( cg.weaponSelect - 32 );
    }
    
    if( !ps->ammo && !ps->clips && !bggame->Weapon( ( weapon_t )ps->weapon )->infiniteAmmo )
    {
        //no ammo
        switch( ps->weapon )
        {
            case WP_MACHINEGUN:
            case WP_CHAINGUN:
            case WP_SHOTGUN:
            case WP_FLAMER:
                Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Find an Armoury and press %s for more ammo\n", KeyNameForCommand( "buy ammo" ) ) );
                break;
                
            case WP_LAS_GUN:
            case WP_PULSE_RIFLE:
            case WP_MASS_DRIVER:
            case WP_LUCIFER_CANNON:
                Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Find a Reactor or Repeater and press %s for more ammo\n", KeyNameForCommand( "buy ammo" ) ) );
                break;
                
            default:
                break;
        }
    }
    else
    {
        switch( ps->weapon )
        {
            case WP_BLASTER:
            case WP_MACHINEGUN:
            case WP_SHOTGUN:
            case WP_LAS_GUN:
            case WP_CHAINGUN:
            case WP_PULSE_RIFLE:
            case WP_FLAMER:
                Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to fire the %s\n", KeyNameForCommand( "+attack" ), bggame->Weapon( ( weapon_t )ps->weapon )->humanName ) );
                break;
                
            case WP_MASS_DRIVER:
                Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to fire the %s\n", KeyNameForCommand( "+attack" ), bggame->Weapon( ( weapon_t )ps->weapon )->humanName ) );
                
                Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Hold %s to zoom\n", KeyNameForCommand( "+button5" ) ) );
                break;
                
            case WP_PAIN_SAW:
                Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Hold %s to activate the %s\n", KeyNameForCommand( "+attack" ), bggame->Weapon( ( weapon_t )ps->weapon )->humanName ) );
                break;
                
            case WP_LUCIFER_CANNON:
                Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Hold and release %s to fire a charged shot\n", KeyNameForCommand( "+attack" ) ) );
                
                Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to fire the %s\n", KeyNameForCommand( "+button5" ), bggame->Weapon( ( weapon_t )ps->weapon )->humanName ) );
                break;
                
            case WP_HBUILD:
                HumanCkitText( text, ps );
                break;
                
            default:
                break;
        }
    }
    
    Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s and ", KeyNameForCommand( "weapprev" ) ) );
    Q_strcat( text, MAX_TUTORIAL_TEXT, va( "%s to select an upgrade\n", KeyNameForCommand( "weapnext" ) ) );
    
    if( upgrade == UP_NONE || ( upgrade > UP_NONE && bggame->Upgrade( upgrade )->usable ) )
    {
        Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to use the %s\n", KeyNameForCommand( "+button2" ), name ) );
    }
    
    if( ps->stats[ STAT_HEALTH ] <= 35 && bggame->InventoryContainsUpgrade( UP_MEDKIT, ps->stats ) )
    {
        Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to use your %s\n", KeyNameForCommand( "itemact medkit" ), bggame->Upgrade( UP_MEDKIT )->humanName ) );
    }
    
    Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to use a structure\n", KeyNameForCommand( "+button7" ) ) );
    
    Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s and forward to sprint\n", KeyNameForCommand( "+button6" ) ) );
    
    Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s and another direction to dodge\n", KeyNameForCommand( "+button6" ) ) );
}

/*
===============
idCGameTutorial::SpectatorText
===============
*/
void idCGameTutorial::SpectatorText( valueType* text, playerState_t* ps )
{
    if( cgs.clientinfo[ cg.clientNum ].team != TEAM_NONE )
    {
        if( ps->pm_flags & PMF_QUEUED )
        {
            Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to leave spawn queue\n", KeyNameForCommand( "+attack" ) ) );
        }
        else
        {
            Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to spawn\n", KeyNameForCommand( "+attack" ) ) );
        }
    }
    else
    {
        Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to join a team\n", KeyNameForCommand( "+attack" ) ) );
    }
    
    if( ps->pm_flags & PMF_FOLLOW )
    {
        if( !cg.chaseFollow )
        {
            Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to switch to chase-cam spectator mode\n", KeyNameForCommand( "+button2" ) ) );
        }
        else
        {
            Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to return to free spectator mode\n", KeyNameForCommand( "+button2" ) ) );
        }
        
        Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s or ", KeyNameForCommand( "weapprev" ) ) );
        Q_strcat( text, MAX_TUTORIAL_TEXT, va( "%s to change player\n", KeyNameForCommand( "weapnext" ) ) );
    }
    else
    {
        Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to follow a player\n", KeyNameForCommand( "+button2" ) ) );
    }
}

/*
===============
idCGameTutorial::TutorialText

Returns context help for the current class/weapon
===============
*/
pointer idCGameTutorial::TutorialText( void )
{
    playerState_t* ps;
    static valueType text[ MAX_TUTORIAL_TEXT ];
    
    FixBindings( );
    GetBindings( );
    
    text[ 0 ] = '\0';
    ps = &cg.snap->ps;
    
    if( !cg.intermissionStarted && !cg.demoPlayback )
    {
        if( ps->persistant[ PERS_SPECSTATE ] != SPECTATOR_NOT || ps->pm_flags & PMF_FOLLOW )
        {
            SpectatorText( text, ps );
        }
        else if( ps->stats[ STAT_HEALTH ] > 0 )
        {
            switch( ps->stats[ STAT_CLASS ] )
            {
                case PCL_ALIEN_BUILDER0:
                case PCL_ALIEN_BUILDER0_UPG:
                    AlienBuilderText( text, ps );
                    break;
                    
                case PCL_ALIEN_LEVEL0:
                    AlienLevel0Text( text, ps );
                    break;
                    
                case PCL_ALIEN_LEVEL1:
                case PCL_ALIEN_LEVEL1_UPG:
                    AlienLevel1Text( text, ps );
                    break;
                    
                case PCL_ALIEN_LEVEL2:
                case PCL_ALIEN_LEVEL2_UPG:
                    AlienLevel2Text( text, ps );
                    break;
                    
                case PCL_ALIEN_LEVEL3:
                case PCL_ALIEN_LEVEL3_UPG:
                    AlienLevel3Text( text, ps );
                    break;
                    
                case PCL_ALIEN_LEVEL4:
                    AlienLevel4Text( text, ps );
                    break;
                    
                case PCL_HUMAN:
                case PCL_HUMAN_BSUIT:
                    HumanText( text, ps );
                    break;
                    
                default:
                    break;
            }
            
            if( ps->stats[ STAT_TEAM ] == TEAM_ALIENS )
            {
                entityState_t* es = BuildableInRange( ps, nullptr );
                
                if( ps->stats[ STAT_STATE ] & SS_HOVELING )
                {
                    Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to exit the hovel\n", KeyNameForCommand( "+button7" ) ) );
                }
                else if( es && es->modelindex == BA_A_HOVEL &&
                         ( es->eFlags & EF_B_SPAWNED ) &&
                         ( ps->stats[ STAT_CLASS ] == PCL_ALIEN_BUILDER0 ||
                           ps->stats[ STAT_CLASS ] == PCL_ALIEN_BUILDER0_UPG ) )
                {
                    Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to enter the hovel\n", KeyNameForCommand( "+button7" ) ) );
                }
                else if( bggame->AlienCanEvolve( ( class_t )ps->stats[ STAT_CLASS ], ps->persistant[ PERS_CREDIT ], cgs.alienStage ) )
                {
                    Q_strcat( text, MAX_TUTORIAL_TEXT, va( "Press %s to evolve\n", KeyNameForCommand( "+button7" ) ) );
                }
            }
        }
        
        Q_strcat( text, MAX_TUTORIAL_TEXT, "Press ESC for the menu" );
    }
    
    return text;
}
