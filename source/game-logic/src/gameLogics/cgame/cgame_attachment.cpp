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
// File name:   cgame_attachment.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description: an abstract attachment system
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <cgame/cgame_precompiled.hpp>

/*
===============
idCGameAnimMapObj::idCGameAnimMapObj
===============
*/
idCGameAttachment::idCGameAttachment( void )
{
}

/*
===============
idCGameAttachment::~idCGameAttachment
===============
*/
idCGameAttachment::~idCGameAttachment( void )
{
}

/*
===============
idCGameAttachment::AttachmentPoint

Return the attachment point
===============
*/
bool idCGameAttachment::AttachmentPoint( attachment_t* a, vec3_t v )
{
    centity_t* cent;
    
    if( !a )
    {
        return false;
    }
    
    // if it all breaks, then use the last point we know was correct
    VectorCopy( a->lastValidAttachmentPoint, v );
    
    switch( a->type )
    {
        case AT_STATIC:
            if( !a->staticValid )
            {
                return false;
            }
            
            VectorCopy( a->origin, v );
            break;
            
        case AT_TAG:
            if( !a->tagValid )
            {
                return false;
            }
            
            AxisCopy( axisDefault, a->re.axis );
            idCGameEnts::PositionRotatedEntityOnTag( &a->re, &a->parent, a->model, a->tagName );
            VectorCopy( a->re.origin, v );
            break;
            
        case AT_CENT:
            if( !a->centValid )
            {
                return false;
            }
            
            if( a->centNum == cg.predictedPlayerState.clientNum )
            {
                // this is smoother if it's the local client
                VectorCopy( cg.predictedPlayerState.origin, v );
            }
            else
            {
                cent = &cg_entities[ a->centNum ];
                VectorCopy( cent->lerpOrigin, v );
            }
            break;
            
        case AT_PARTICLE:
            if( !a->particleValid )
            {
                return false;
            }
            
            if( !a->particle->valid )
            {
                a->particleValid = false;
                return false;
            }
            else
            {
                VectorCopy( a->particle->origin, v );
            }
            break;
            
        default:
            Printf( S_COLOR_RED "ERROR: Invalid attachmentType_t in attachment\n" );
            break;
    }
    
    if( a->hasOffset )
    {
        VectorAdd( v, a->offset, v );
    }
    
    VectorCopy( v, a->lastValidAttachmentPoint );
    
    return true;
}

/*
===============
idCGameAttachment::AttachmentDir

Return the attachment direction
===============
*/
bool idCGameAttachment::AttachmentDir( attachment_t* a, vec3_t v )
{
    vec3_t forward;
    centity_t* cent;
    
    if( !a )
    {
        return false;
    }
    
    switch( a->type )
    {
        case AT_STATIC:
            return false;
            break;
            
        case AT_TAG:
            if( !a->tagValid )
            {
                return false;
            }
            
            VectorCopy( a->re.axis[ 0 ], v );
            break;
            
        case AT_CENT:
            if( !a->centValid )
            {
                return false;
            }
            
            cent = &cg_entities[ a->centNum ];
            AngleVectors( cent->lerpAngles, forward, nullptr, nullptr );
            VectorCopy( forward, v );
            break;
            
        case AT_PARTICLE:
            if( !a->particleValid )
            {
                return false;
            }
            
            if( !a->particle->valid )
            {
                a->particleValid = false;
                return false;
            }
            else
            {
                VectorCopy( a->particle->velocity, v );
            }
            break;
            
        default:
            Printf( S_COLOR_RED "ERROR: Invalid attachmentType_t in attachment\n" );
            break;
    }
    
    VectorNormalize( v );
    return true;
}

/*
===============
idCGameAttachment::AttachmentAxis

Return the attachment axis
===============
*/
bool idCGameAttachment::AttachmentAxis( attachment_t* a, vec3_t axis[ 3 ] )
{
    centity_t* cent;
    
    if( !a )
    {
        return false;
    }
    
    switch( a->type )
    {
        case AT_STATIC:
            return false;
            break;
            
        case AT_TAG:
            if( !a->tagValid )
            {
                return false;
            }
            
            AxisCopy( a->re.axis, axis );
            break;
            
        case AT_CENT:
            if( !a->centValid )
            {
                return false;
            }
            
            cent = &cg_entities[ a->centNum ];
            AnglesToAxis( cent->lerpAngles, axis );
            break;
            
        case AT_PARTICLE:
            return false;
            break;
            
        default:
            Printf( S_COLOR_RED "ERROR: Invalid attachmentType_t in attachment\n" );
            break;
    }
    
    return true;
}

/*
===============
idCGameAttachment::AttachmentVelocity

If the attachment can have velocity, return it
===============
*/
bool idCGameAttachment::AttachmentVelocity( attachment_t* a, vec3_t v )
{
    if( !a )
    {
        return false;
    }
    
    if( a->particleValid && a->particle->valid )
    {
        VectorCopy( a->particle->velocity, v );
        return true;
    }
    else if( a->centValid )
    {
        centity_t* cent = &cg_entities[ a->centNum ];
        
        VectorCopy( cent->currentState.pos.trDelta, v );
        return true;
    }
    
    return false;
}

/*
===============
idCGameAttachment::AttachmentCentNum

If the attachment has a centNum, return it
===============
*/
sint idCGameAttachment::AttachmentCentNum( attachment_t* a )
{
    if( !a || !a->centValid )
    {
        return -1;
    }
    
    return a->centNum;
}

/*
===============
idCGameAttachment::Attached

If the attachment is valid, return true
===============
*/
bool idCGameAttachment::Attached( attachment_t* a )
{
    if( !a )
    {
        return false;
    }
    
    return a->attached;
}

/*
===============
idCGameAttachment::AttachToPoint

Attach to a point in space
===============
*/
void idCGameAttachment::AttachToPoint( attachment_t* a )
{
    if( !a || !a->staticValid )
    {
        return;
    }
    
    a->type = AT_STATIC;
    a->attached = true;
}

/*
===============
idCGameAttachment::AttachToCent

Attach to a centity_t
===============
*/
void idCGameAttachment::AttachToCent( attachment_t* a )
{
    if( !a || !a->centValid )
    {
        return;
    }
    
    a->type = AT_CENT;
    a->attached = true;
}

/*
===============
idCGameAttachment::AttachToTag

Attach to a model tag
===============
*/
void idCGameAttachment::AttachToTag( attachment_t* a )
{
    if( !a || !a->tagValid )
    {
        return;
    }
    
    a->type = AT_TAG;
    a->attached = true;
}

/*
===============
idCGameAttachment::AttachToParticle

Attach to a particle
===============
*/
void idCGameAttachment::AttachToParticle( attachment_t* a )
{
    if( !a || !a->particleValid )
    {
        return;
    }
    
    a->type = AT_PARTICLE;
    a->attached = true;
}

/*
===============
idCGameAttachment::SetAttachmentPoint
===============
*/
void idCGameAttachment::SetAttachmentPoint( attachment_t* a, vec3_t v )
{
    if( !a )
    {
        return;
    }
    
    VectorCopy( v, a->origin );
    a->staticValid = true;
}

/*
===============
idCGameAttachment::SetAttachmentCent
===============
*/
void idCGameAttachment::SetAttachmentCent( attachment_t* a, centity_t* cent )
{
    if( !a || !cent )
    {
        return;
    }
    
    a->centNum = cent->currentState.number;
    a->centValid = true;
}

/*
===============
idCGameAttachment::SetAttachmentTag
===============
*/
void idCGameAttachment::SetAttachmentTag( attachment_t* a, refEntity_t parent, qhandle_t model, valueType* tagName )
{
    if( !a )
    {
        return;
    }
    
    a->parent = parent;
    a->model = model;
    strncpy( a->tagName, tagName, MAX_STRING_CHARS );
    a->tagValid = true;
}

/*
===============
idCGameAttachment::SetAttachmentParticle
===============
*/
void idCGameAttachment::SetAttachmentParticle( attachment_t* a, particle_t* p )
{
    if( !a )
    {
        return;
    }
    
    a->particle = p;
    a->particleValid = true;
}

/*
===============
idCGameAttachment::SetAttachmentOffset
===============
*/
void idCGameAttachment::SetAttachmentOffset( attachment_t* a, vec3_t v )
{
    if( !a )
    {
        return;
    }
    
    VectorCopy( v, a->offset );
    a->hasOffset = true;
}
