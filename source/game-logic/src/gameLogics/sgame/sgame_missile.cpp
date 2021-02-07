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
// File name:   sgame_missile.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <sgame/sgame_precompiled.hpp>

/*
===============
idSGameMissile::idSGameMissile
===============
*/
idSGameMissile::idSGameMissile( void )
{
}

/*
===============
idSGameMissile::~idSGameMissile
===============
*/
idSGameMissile::~idSGameMissile( void )
{
}

/*
================
idSGameMissile::BounceMissile
================
*/
void idSGameMissile::BounceMissile( gentity_t* ent, trace_t* trace )
{
    vec3_t velocity;
    float32 dot;
    sint hitTime;
    
    // reflect the velocity on the trace plane
    hitTime = level.previousTime + ( level.time - level.previousTime ) * trace->fraction;
    bggame->EvaluateTrajectoryDelta( &ent->s.pos, hitTime, velocity );
    dot = DotProduct( velocity, trace->plane.normal );
    VectorMA( velocity, -2 * dot, trace->plane.normal, ent->s.pos.trDelta );
    
    if( ent->s.eFlags & EF_BOUNCE_HALF )
    {
        VectorScale( ent->s.pos.trDelta, 0.65, ent->s.pos.trDelta );
        // check for stop
        if( trace->plane.normal[ 2 ] > 0.2 && VectorLength( ent->s.pos.trDelta ) < 40 )
        {
            idSGameUtils::SetOrigin( ent, trace->endpos );
            return;
        }
    }
    
    VectorAdd( ent->r.currentOrigin, trace->plane.normal, ent->r.currentOrigin );
    VectorCopy( ent->r.currentOrigin, ent->s.pos.trBase );
    ent->s.pos.trTime = level.time;
    
    if( ent->bounceCount != -5 )
    {
        ent->bounceCount--;
    }
}
/*
================
idSGameMissile::ExplodeMissile

Explode a missile without an impact
================
*/
void idSGameMissile::ExplodeMissile( gentity_t* ent )
{
    vec3_t dir, origin;
    
    bggame->EvaluateTrajectory( &ent->s.pos, level.time, origin );
    trap_SnapVector( origin );
    idSGameUtils::SetOrigin( ent, origin );
    
    // we don't have a valid direction, so just point straight up
    dir[ 0 ] = dir[ 1 ] = 0;
    dir[ 2 ] = 1;
    
    ent->s.eType = ET_GENERAL;
    
    if( ent->s.weapon != WP_LOCKBLOB_LAUNCHER &&
            ent->s.weapon != WP_FLAMER )
        idSGameUtils::AddEvent( ent, EV_MISSILE_MISS, DirToByte( dir ) );
        
    ent->freeAfterEvent = true;
    
    // splash damage
    if( ent->splashDamage )
        idSGameCombat::RadiusDamage( ent->r.currentOrigin, ent->parent, ent->splashDamage, ent->splashRadius, ent, ent->splashMethodOfDeath );
        
    trap_LinkEntity( ent );
}

/*
================
idSGameMissile::MissileImpact
================
*/
void idSGameMissile::MissileImpact( gentity_t* ent, trace_t* trace )
{
    gentity_t*   other, *attacker;
    bool    returnAfterDamage = false;
    vec3_t      dir;
    
    if( trace->surfaceFlags & SURF_NOIMPACT )
    {
        idSGameUtils::FreeEntity( ent );
        return;
    }
    
    other = &g_entities[ trace->entityNum ];
    attacker = &g_entities[ ent->r.ownerNum ];
    
    // check for bounce
    if( !other->takedamage && ( ent->bounceCount > 0 || ent->bounceCount == -5 ) && ( ent->s.eFlags & ( EF_BOUNCE | EF_BOUNCE_HALF ) ) )
    {
        BounceMissile( ent, trace );
        
        //only play a sound if requested
        if( !( ent->s.eFlags & EF_NO_BOUNCE_SOUND ) )
            idSGameUtils::AddEvent( ent, EV_GRENADE_BOUNCE, 0 );
            
        return;
    }
    
    if( !strcmp( ent->classname, "grenade" ) )
    {
        //grenade doesn't explode on impact
        BounceMissile( ent, trace );
        
        //only play a sound if requested
        if( !( ent->s.eFlags & EF_NO_BOUNCE_SOUND ) )
            idSGameUtils::AddEvent( ent, EV_GRENADE_BOUNCE, 0 );
            
        return;
    }
    else if( !strcmp( ent->classname, "lockblob" ) )
    {
        if( other->client && other->client->ps.stats[ STAT_TEAM ] == TEAM_HUMANS )
        {
            other->client->ps.stats[ STAT_STATE ] |= SS_BLOBLOCKED;
            other->client->lastLockTime = level.time;
            AngleVectors( other->client->ps.viewangles, dir, nullptr, nullptr );
            other->client->ps.stats[ STAT_VIEWLOCK ] = DirToByte( dir );
        }
    }
    else if( !strcmp( ent->classname, "slowblob" ) )
    {
        if( other->client && other->client->ps.stats[ STAT_TEAM ] == TEAM_HUMANS )
        {
            other->client->ps.stats[ STAT_STATE ] |= SS_SLOWLOCKED;
            other->client->lastSlowTime = level.time;
            AngleVectors( other->client->ps.viewangles, dir, nullptr, nullptr );
            other->client->ps.stats[ STAT_VIEWLOCK ] = DirToByte( dir );
        }
    }
    else if( !strcmp( ent->classname, "hive" ) )
    {
        if( other->s.eType == ET_BUILDABLE && other->s.modelindex == BA_A_HIVE )
        {
            if( !ent->parent )
                idSGameMain::Printf( S_COLOR_YELLOW "WARNING: hive entity has no parent in G_MissileImpact\n" );
            else
                ent->parent->active = false;
                
            idSGameUtils::FreeEntity( ent );
            return;
        }
        else
        {
            //prevent collision with the client when returning
            ent->r.ownerNum = other->s.number;
            
            ent->think = ExplodeMissile;
            ent->nextthink = level.time + FRAMETIME;
            
            //only damage humans
            if( other->client && other->client->ps.stats[ STAT_TEAM ] == TEAM_HUMANS )
                returnAfterDamage = true;
            else
                return;
        }
    }
    
    // impact damage
    if( other->takedamage )
    {
        // FIXME: wrong damage direction?
        if( ent->damage )
        {
            vec3_t  velocity;
            
            bggame->EvaluateTrajectoryDelta( &ent->s.pos, level.time, velocity );
            if( VectorLength( velocity ) == 0 )
                velocity[ 2 ] = 1;  // stepped on a grenade
                
            idSGameCombat::Damage( other, ent, attacker, velocity, ent->s.origin, ent->damage, DAMAGE_NO_LOCDAMAGE, ent->methodOfDeath );
        }
    }
    
    if( returnAfterDamage )
        return;
        
    // is it cheaper in bandwidth to just remove this ent and create a new
    // one, rather than changing the missile into the explosion?
    
    if( other->takedamage && ( other->s.eType == ET_PLAYER || other->s.eType == ET_BUILDABLE ) )
    {
        idSGameUtils::AddEvent( ent, EV_MISSILE_HIT, DirToByte( trace->plane.normal ) );
        ent->s.otherEntityNum = other->s.number;
    }
    else if( trace->surfaceFlags & SURF_METALSTEPS )
        idSGameUtils::AddEvent( ent, EV_MISSILE_MISS_METAL, DirToByte( trace->plane.normal ) );
    else
        idSGameUtils::AddEvent( ent, EV_MISSILE_MISS, DirToByte( trace->plane.normal ) );
        
    ent->freeAfterEvent = true;
    
    // change over to a normal entity right at the point of impact
    ent->s.eType = ET_GENERAL;
    
    idSGameWeapons::SnapVectorTowards( trace->endpos, ent->s.pos.trBase );  // save net bandwidth
    
    idSGameUtils::SetOrigin( ent, trace->endpos );
    
    // splash damage (doesn't apply to person directly hit)
    if( ent->splashDamage )
        idSGameCombat::RadiusDamage( trace->endpos, ent->parent, ent->splashDamage, ent->splashRadius, other, ent->splashMethodOfDeath );
        
    trap_LinkEntity( ent );
}

/*
================
idSGameMissile::RunMissile
================
*/
void idSGameMissile::RunMissile( gentity_t* ent )
{
    vec3_t    origin;
    trace_t   tr;
    sint     passent;
    
    // get current position
    bggame->EvaluateTrajectory( &ent->s.pos, level.time, origin );
    
    // ignore interactions with the missile owner
    passent = ent->r.ownerNum;
    
    // general trace to see if we hit anything at all
    trap_Trace( &tr, ent->r.currentOrigin, ent->r.mins, ent->r.maxs, origin, passent, ent->clipmask );
    
    if( tr.startsolid || tr.allsolid )
    {
        tr.fraction = 0;
        VectorCopy( ent->r.currentOrigin, tr.endpos );
    }
    
    // this is a bit nasty
    // keep in mind that some of the later else ifs change the contents of tr
    if( tr.fraction == 1.0f )
    {
        VectorCopy( tr.endpos, ent->r.currentOrigin );
    }
    // we hit something, then - check if it was an entity (or if we don't care)
    else if( !ent->pointAgainstWorld || tr.contents & CONTENTS_BODY ||
             ( tr.entityNum >= 0 && tr.entityNum != ENTITYNUM_WORLD ) )
    {
        VectorCopy( tr.endpos, ent->r.currentOrigin );
        
        MissileImpact( ent, &tr );
        
        if( ent->s.eType != ET_MISSILE )
            return;   // exploded
    }
    // only hit the world with zero width
    else if( trap_Trace( &tr, ent->r.currentOrigin, nullptr, nullptr, origin,
                         passent, ent->clipmask ), tr.fraction < 1.0f )
    {
        VectorCopy( tr.endpos, ent->r.currentOrigin );
        
        MissileImpact( ent, &tr );
        
        if( ent->s.eType != ET_MISSILE )
            return;   // exploded
    }
    // one last check to see if we can hit an entity near solid stuff
    // use conditional shortcutting and comma operators to only trace if necessary
    else if( tr.contents & CONTENTS_BODY ||
             ( tr.entityNum >= 0 && tr.entityNum != ENTITYNUM_WORLD ) ||
             ( trap_Trace( &tr, ent->r.currentOrigin, ent->r.mins, ent->r.maxs,
                           origin, passent, CONTENTS_BODY ), tr.fraction ) < 1.0f )
    {
        VectorCopy( tr.endpos, ent->r.currentOrigin );
        
        MissileImpact( ent, &tr );
        
        if( ent->s.eType != ET_MISSILE )
            return;   // exploded
    }
    
    ent->r.contents = CONTENTS_SOLID; //trick trap_LinkEntity into...
    trap_LinkEntity( ent );
    ent->r.contents = 0; //...encoding bbox information
    
    idSGameActive::TouchTriggers( ent );
    
    // check think function after bouncing
    idSGameMain::RunThink( ent );
}


//=============================================================================

/*
=================
idSGameMissile::fire_flamer
=================
*/
gentity_t* idSGameMissile::fire_flamer( gentity_t* self, vec3_t start, vec3_t dir )
{
    gentity_t* bolt;
    vec3_t    pvel;
    
    VectorNormalize( dir );
    
    bolt = idSGameUtils::Spawn();
    bolt->classname = "flame";
    bolt->pointAgainstWorld = false;
    bolt->nextthink = level.time + FLAMER_LIFETIME;
    bolt->think = ExplodeMissile;
    bolt->s.eType = ET_MISSILE;
    bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
    bolt->s.weapon = WP_FLAMER;
    bolt->s.generic1 = self->s.generic1; //weaponMode
    bolt->r.ownerNum = self->s.number;
    bolt->parent = self;
    bolt->damage = FLAMER_DMG;
    bolt->splashDamage = FLAMER_DMG;
    bolt->splashRadius = FLAMER_RADIUS;
    bolt->methodOfDeath = MOD_FLAMER;
    bolt->splashMethodOfDeath = MOD_FLAMER_SPLASH;
    bolt->clipmask = MASK_SHOT;
    bolt->target_ent = nullptr;
    bolt->r.mins[ 0 ] = bolt->r.mins[ 1 ] = bolt->r.mins[ 2 ] = -FLAMER_SIZE;
    bolt->r.maxs[ 0 ] = bolt->r.maxs[ 1 ] = bolt->r.maxs[ 2 ] = FLAMER_SIZE;
    
    bolt->s.pos.trType = TR_LINEAR;
    bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;   // move a bit on the very first frame
    VectorCopy( start, bolt->s.pos.trBase );
    VectorScale( self->client->ps.velocity, FLAMER_LAG, pvel );
    VectorMA( pvel, FLAMER_SPEED, dir, bolt->s.pos.trDelta );
    SnapVector( bolt->s.pos.trDelta );      // save net bandwidth
    
    VectorCopy( start, bolt->r.currentOrigin );
    
    return bolt;
}

//=============================================================================

/*
=================
idSGameMissile::fire_blaster
=================
*/
gentity_t* idSGameMissile::fire_blaster( gentity_t* self, vec3_t start, vec3_t dir )
{
    gentity_t* bolt;
    
    VectorNormalize( dir );
    
    bolt = idSGameUtils::Spawn();
    bolt->classname = "blaster";
    bolt->pointAgainstWorld = true;
    bolt->nextthink = level.time + 10000;
    bolt->think = ExplodeMissile;
    bolt->s.eType = ET_MISSILE;
    bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
    bolt->s.weapon = WP_BLASTER;
    bolt->s.generic1 = self->s.generic1; //weaponMode
    bolt->r.ownerNum = self->s.number;
    bolt->parent = self;
    bolt->damage = BLASTER_DMG;
    bolt->splashDamage = 0;
    bolt->splashRadius = 0;
    bolt->methodOfDeath = MOD_BLASTER;
    bolt->splashMethodOfDeath = MOD_BLASTER;
    bolt->clipmask = MASK_SHOT;
    bolt->target_ent = nullptr;
    bolt->r.mins[ 0 ] = bolt->r.mins[ 1 ] = bolt->r.mins[ 2 ] = -BLASTER_SIZE;
    bolt->r.maxs[ 0 ] = bolt->r.maxs[ 1 ] = bolt->r.maxs[ 2 ] = BLASTER_SIZE;
    
    bolt->s.pos.trType = TR_LINEAR;
    bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;   // move a bit on the very first frame
    VectorCopy( start, bolt->s.pos.trBase );
    VectorScale( dir, BLASTER_SPEED, bolt->s.pos.trDelta );
    SnapVector( bolt->s.pos.trDelta );      // save net bandwidth
    
    VectorCopy( start, bolt->r.currentOrigin );
    
    return bolt;
}

//=============================================================================

/*
=================
idSGameMissile::fire_pulseRifle
=================
*/
gentity_t* idSGameMissile::fire_pulseRifle( gentity_t* self, vec3_t start, vec3_t dir )
{
    gentity_t* bolt;
    
    VectorNormalize( dir );
    
    bolt = idSGameUtils::Spawn();
    bolt->classname = "pulse";
    bolt->pointAgainstWorld = true;
    bolt->nextthink = level.time + 10000;
    bolt->think = ExplodeMissile;
    bolt->s.eType = ET_MISSILE;
    bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
    bolt->s.weapon = WP_PULSE_RIFLE;
    bolt->s.generic1 = self->s.generic1; //weaponMode
    bolt->r.ownerNum = self->s.number;
    bolt->parent = self;
    bolt->damage = PRIFLE_DMG;
    bolt->splashDamage = 0;
    bolt->splashRadius = 0;
    bolt->methodOfDeath = MOD_PRIFLE;
    bolt->splashMethodOfDeath = MOD_PRIFLE;
    bolt->clipmask = MASK_SHOT;
    bolt->target_ent = nullptr;
    bolt->r.mins[ 0 ] = bolt->r.mins[ 1 ] = bolt->r.mins[ 2 ] = -PRIFLE_SIZE;
    bolt->r.maxs[ 0 ] = bolt->r.maxs[ 1 ] = bolt->r.maxs[ 2 ] = PRIFLE_SIZE;
    
    bolt->s.pos.trType = TR_LINEAR;
    bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;   // move a bit on the very first frame
    VectorCopy( start, bolt->s.pos.trBase );
    VectorScale( dir, PRIFLE_SPEED, bolt->s.pos.trDelta );
    SnapVector( bolt->s.pos.trDelta );      // save net bandwidth
    
    VectorCopy( start, bolt->r.currentOrigin );
    
    return bolt;
}

//=============================================================================

/*
=================
idSGameMissile::fire_luciferCannon
=================
*/
gentity_t* idSGameMissile::fire_luciferCannon( gentity_t* self, vec3_t start, vec3_t dir, sint damage, sint radius, sint speed )
{
    gentity_t* bolt;
    float32 charge;
    
    VectorNormalize( dir );
    
    bolt = idSGameUtils::Spawn( );
    bolt->classname = "lcannon";
    bolt->pointAgainstWorld = true;
    
    if( damage == LCANNON_DAMAGE )
        bolt->nextthink = level.time;
    else
        bolt->nextthink = level.time + 10000;
        
    bolt->think = ExplodeMissile;
    bolt->s.eType = ET_MISSILE;
    bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
    bolt->s.weapon = WP_LUCIFER_CANNON;
    bolt->s.generic1 = self->s.generic1; //weaponMode
    bolt->r.ownerNum = self->s.number;
    bolt->parent = self;
    bolt->damage = damage;
    bolt->splashDamage = damage / 2;
    bolt->splashRadius = radius;
    bolt->methodOfDeath = MOD_LCANNON;
    bolt->splashMethodOfDeath = MOD_LCANNON_SPLASH;
    bolt->clipmask = MASK_SHOT;
    bolt->target_ent = nullptr;
    
    // Give the missile a small bounding box
    bolt->r.mins[ 0 ] = bolt->r.mins[ 1 ] = bolt->r.mins[ 2 ] = -LCANNON_SIZE;
    bolt->r.maxs[ 0 ] = bolt->r.maxs[ 1 ] = bolt->r.maxs[ 2 ] = -bolt->r.mins[ 0 ];
    
    // Pass the missile charge through
    charge = ( float32 )( damage - LCANNON_SECONDARY_DAMAGE ) / LCANNON_DAMAGE;
    bolt->s.torsoAnim = charge * 255;
    if( bolt->s.torsoAnim < 0 )
        bolt->s.torsoAnim = 0;
        
    bolt->s.pos.trType = TR_LINEAR;
    bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;   // move a bit on the very first frame
    VectorCopy( start, bolt->s.pos.trBase );
    VectorScale( dir, speed, bolt->s.pos.trDelta );
    SnapVector( bolt->s.pos.trDelta );      // save net bandwidth
    
    VectorCopy( start, bolt->r.currentOrigin );
    
    return bolt;
}

/*
=================
idSGameMissile::launch_grenade
=================
*/
gentity_t* idSGameMissile::launch_grenade( gentity_t* self, vec3_t start, vec3_t dir )
{
    gentity_t* bolt;
    
    VectorNormalize( dir );
    
    bolt = idSGameUtils::Spawn( );
    bolt->classname = "grenade";
    bolt->pointAgainstWorld = false;
    bolt->nextthink = level.time + 5000;
    bolt->think = ExplodeMissile;
    bolt->s.eType = ET_MISSILE;
    bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
    bolt->s.weapon = WP_GRENADE;
    bolt->s.eFlags = EF_BOUNCE_HALF;
    bolt->s.generic1 = WPM_PRIMARY; //weaponMode
    bolt->r.ownerNum = self->s.number;
    bolt->parent = self;
    bolt->damage = GRENADE_DAMAGE;
    bolt->splashDamage = GRENADE_DAMAGE;
    bolt->splashRadius = GRENADE_RANGE;
    bolt->methodOfDeath = MOD_GRENADE;
    bolt->splashMethodOfDeath = MOD_GRENADE;
    bolt->clipmask = MASK_SHOT;
    bolt->target_ent = nullptr;
    bolt->r.mins[ 0 ] = bolt->r.mins[ 1 ] = bolt->r.mins[ 2 ] = -3.0f;
    bolt->r.maxs[ 0 ] = bolt->r.maxs[ 1 ] = bolt->r.maxs[ 2 ] = 3.0f;
    bolt->s.time = level.time;
    
    bolt->s.pos.trType = TR_GRAVITY;
    bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;   // move a bit on the very first frame
    VectorCopy( start, bolt->s.pos.trBase );
    VectorScale( dir, GRENADE_SPEED, bolt->s.pos.trDelta );
    SnapVector( bolt->s.pos.trDelta );      // save net bandwidth
    
    VectorCopy( start, bolt->r.currentOrigin );
    
    return bolt;
}

/*
================
idSGameMissile::AHive_SearchAndDestroy

Adjust the trajectory to point towards the target
================
*/
void idSGameMissile::AHive_SearchAndDestroy( gentity_t* self )
{
    vec3_t dir;
    trace_t tr;
    gentity_t* ent;
    sint i;
    float32 d, nearest;
    
    if( level.time > self->timestamp )
    {
        VectorCopy( self->r.currentOrigin, self->s.pos.trBase );
        self->s.pos.trType = TR_STATIONARY;
        self->s.pos.trTime = level.time;
        
        self->think = ExplodeMissile;
        self->nextthink = level.time + 50;
        self->parent->active = false; //allow the parent to start again
        return;
    }
    
    nearest = DistanceSquared( self->r.currentOrigin, self->target_ent->r.currentOrigin );
    //find the closest human
    for( i = 0; i < MAX_CLIENTS; i++ )
    {
        ent = &g_entities[ i ];
        if( ent->client &&
                ent->health > 0 &&
                ent->client->ps.stats[ STAT_TEAM ] == TEAM_HUMANS &&
                nearest > ( d = DistanceSquared( ent->r.currentOrigin, self->r.currentOrigin ) ) )
        {
            trap_Trace( &tr, self->r.currentOrigin, self->r.mins, self->r.maxs,
                        ent->r.currentOrigin, self->r.ownerNum, self->clipmask );
            if( tr.entityNum != ENTITYNUM_WORLD )
            {
                nearest = d;
                self->target_ent = ent;
            }
        }
    }
    VectorSubtract( self->target_ent->r.currentOrigin, self->r.currentOrigin, dir );
    VectorNormalize( dir );
    
    //change direction towards the player
    VectorScale( dir, HIVE_SPEED, self->s.pos.trDelta );
    SnapVector( self->s.pos.trDelta );      // save net bandwidth
    VectorCopy( self->r.currentOrigin, self->s.pos.trBase );
    self->s.pos.trTime = level.time;
    
    self->nextthink = level.time + HIVE_DIR_CHANGE_PERIOD;
    
}

/*
=================
idSGameMissile::fire_hive
=================
*/
gentity_t* idSGameMissile::fire_hive( gentity_t* self, vec3_t start, vec3_t dir )
{
    gentity_t* bolt;
    
    VectorNormalize( dir );
    
    bolt = idSGameUtils::Spawn( );
    bolt->classname = "hive";
    bolt->pointAgainstWorld = false;
    bolt->nextthink = level.time + HIVE_DIR_CHANGE_PERIOD;
    bolt->think = AHive_SearchAndDestroy;
    bolt->s.eType = ET_MISSILE;
    bolt->s.eFlags |= EF_BOUNCE | EF_NO_BOUNCE_SOUND;
    bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
    bolt->s.weapon = WP_HIVE;
    bolt->s.generic1 = WPM_PRIMARY; //weaponMode
    bolt->r.ownerNum = self->s.number;
    bolt->parent = self;
    bolt->damage = HIVE_DMG;
    bolt->splashDamage = 0;
    bolt->splashRadius = 0;
    bolt->methodOfDeath = MOD_SWARM;
    bolt->clipmask = MASK_SHOT;
    bolt->target_ent = self->target_ent;
    bolt->timestamp = level.time + HIVE_LIFETIME;
    
    bolt->s.pos.trType = TR_LINEAR;
    bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;   // move a bit on the very first frame
    VectorCopy( start, bolt->s.pos.trBase );
    VectorScale( dir, HIVE_SPEED, bolt->s.pos.trDelta );
    SnapVector( bolt->s.pos.trDelta );      // save net bandwidth
    VectorCopy( start, bolt->r.currentOrigin );
    
    return bolt;
}

//=============================================================================

/*
=================
idSGameMissile::fire_lockblob
=================
*/
gentity_t* idSGameMissile::fire_lockblob( gentity_t* self, vec3_t start, vec3_t dir )
{
    gentity_t* bolt;
    
    VectorNormalize( dir );
    
    bolt = idSGameUtils::Spawn( );
    bolt->classname = "lockblob";
    bolt->pointAgainstWorld = true;
    bolt->nextthink = level.time + 15000;
    bolt->think = ExplodeMissile;
    bolt->s.eType = ET_MISSILE;
    bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
    bolt->s.weapon = WP_LOCKBLOB_LAUNCHER;
    bolt->s.generic1 = WPM_PRIMARY; //weaponMode
    bolt->r.ownerNum = self->s.number;
    bolt->parent = self;
    bolt->damage = 0;
    bolt->splashDamage = 0;
    bolt->splashRadius = 0;
    bolt->methodOfDeath = MOD_UNKNOWN; //doesn't do damage so will never kill
    bolt->clipmask = MASK_SHOT;
    bolt->target_ent = nullptr;
    
    bolt->s.pos.trType = TR_LINEAR;
    bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;   // move a bit on the very first frame
    VectorCopy( start, bolt->s.pos.trBase );
    VectorScale( dir, 500, bolt->s.pos.trDelta );
    SnapVector( bolt->s.pos.trDelta );      // save net bandwidth
    VectorCopy( start, bolt->r.currentOrigin );
    
    return bolt;
}

/*
=================
idSGameMissile::fire_slowBlob
=================
*/
gentity_t* idSGameMissile::fire_slowBlob( gentity_t* self, vec3_t start, vec3_t dir )
{
    gentity_t* bolt;
    
    VectorNormalize( dir );
    
    bolt = idSGameUtils::Spawn( );
    bolt->classname = "slowblob";
    bolt->pointAgainstWorld = true;
    bolt->nextthink = level.time + 15000;
    bolt->think = ExplodeMissile;
    bolt->s.eType = ET_MISSILE;
    bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
    bolt->s.weapon = WP_ABUILD2;
    bolt->s.generic1 = self->s.generic1; //weaponMode
    bolt->r.ownerNum = self->s.number;
    bolt->parent = self;
    bolt->damage = ABUILDER_BLOB_DMG;
    bolt->splashDamage = 0;
    bolt->splashRadius = 0;
    bolt->methodOfDeath = MOD_SLOWBLOB;
    bolt->splashMethodOfDeath = MOD_SLOWBLOB;
    bolt->clipmask = MASK_SHOT;
    bolt->target_ent = nullptr;
    
    bolt->s.pos.trType = TR_GRAVITY;
    bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;   // move a bit on the very first frame
    VectorCopy( start, bolt->s.pos.trBase );
    VectorScale( dir, ABUILDER_BLOB_SPEED, bolt->s.pos.trDelta );
    SnapVector( bolt->s.pos.trDelta );      // save net bandwidth
    VectorCopy( start, bolt->r.currentOrigin );
    
    return bolt;
}

/*
=================
idSGameMissile::fire_bounceBall
=================
*/
gentity_t* idSGameMissile::fire_bounceBall( gentity_t* self, vec3_t start, vec3_t dir )
{
    gentity_t* bolt;
    
    VectorNormalize( dir );
    
    bolt = idSGameUtils::Spawn( );
    bolt->classname = "bounceball";
    bolt->pointAgainstWorld = true;
    bolt->nextthink = level.time + 3000;
    bolt->think = ExplodeMissile;
    bolt->s.eType = ET_MISSILE;
    bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
    bolt->s.weapon = WP_ALEVEL3_UPG;
    bolt->s.generic1 = self->s.generic1; //weaponMode
    bolt->r.ownerNum = self->s.number;
    bolt->parent = self;
    bolt->damage = LEVEL3_BOUNCEBALL_DMG;
    bolt->splashDamage = LEVEL3_BOUNCEBALL_DMG;
    bolt->splashRadius = LEVEL3_BOUNCEBALL_RADIUS;
    bolt->methodOfDeath = MOD_LEVEL3_BOUNCEBALL;
    bolt->splashMethodOfDeath = MOD_LEVEL3_BOUNCEBALL;
    bolt->clipmask = MASK_SHOT;
    bolt->target_ent = nullptr;
    
    bolt->s.pos.trType = TR_GRAVITY;
    bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;   // move a bit on the very first frame
    VectorCopy( start, bolt->s.pos.trBase );
    VectorScale( dir, LEVEL3_BOUNCEBALL_SPEED, bolt->s.pos.trDelta );
    SnapVector( bolt->s.pos.trDelta );      // save net bandwidth
    VectorCopy( start, bolt->r.currentOrigin );
    /*bolt->s.eFlags |= EF_BOUNCE;*/
    
    return bolt;
}

