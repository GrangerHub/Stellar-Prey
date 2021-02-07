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
// File name:   cgame_marks.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description: wall marks
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <cgame/cgame_precompiled.hpp>

/*
===================================================================
MARK POLYS
===================================================================
*/

markPoly_t cg_activeMarkPolys;     // double linked list
markPoly_t* cg_freeMarkPolys;      // single linked list
markPoly_t cg_markPolys[ MAX_MARK_POLYS ];
static sint markTotal;

/*
===============
idCGameMarks::idCGameMarks
===============
*/
idCGameMarks::idCGameMarks( void )
{
}

/*
===============
idCGameMarks::~idCGameMarks
===============
*/
idCGameMarks::~idCGameMarks( void )
{
}

/*
===================
idCGameMarks::InitMarkPolys

This is called at startup and for tournement restarts
===================
*/
void idCGameMarks::InitMarkPolys( void )
{
    sint i;
    
    ::memset( cg_markPolys, 0, sizeof( cg_markPolys ) );
    
    cg_activeMarkPolys.nextMark = &cg_activeMarkPolys;
    cg_activeMarkPolys.prevMark = &cg_activeMarkPolys;
    cg_freeMarkPolys = cg_markPolys;
    
    for( i = 0; i < MAX_MARK_POLYS - 1; i++ )
    {
        cg_markPolys[i].nextMark = &cg_markPolys[i + 1];
    }
}

/*
==================
idCGameMarks::FreeMarkPoly
==================
*/
void idCGameMarks::FreeMarkPoly( markPoly_t* le )
{
    if( !le->prevMark )
    {
        Error( "idCGameLocal::FreeLocalEntity: not active" );
    }
    
    // remove from the doubly linked active list
    le->prevMark->nextMark = le->nextMark;
    le->nextMark->prevMark = le->prevMark;
    
    // the free list is only singly linked
    le->nextMark = cg_freeMarkPolys;
    cg_freeMarkPolys = le;
}

/*
===================
idCGameMarks::AllocMark

Will allways succeed, even if it requires freeing an old active mark
===================
*/
markPoly_t* idCGameMarks::AllocMark( void )
{
    markPoly_t* le;
    sint time;
    
    if( !cg_freeMarkPolys )
    {
        // no free entities, so free the one at the end of the chain
        // remove the oldest active entity
        time = cg_activeMarkPolys.prevMark->time;
        
        while( cg_activeMarkPolys.prevMark && time == cg_activeMarkPolys.prevMark->time )
        {
            FreeMarkPoly( cg_activeMarkPolys.prevMark );
        }
    }
    
    le = cg_freeMarkPolys;
    cg_freeMarkPolys = cg_freeMarkPolys->nextMark;
    
    ::memset( le, 0, sizeof( *le ) );
    
    // link into the active list
    le->nextMark = cg_activeMarkPolys.nextMark;
    le->prevMark = &cg_activeMarkPolys;
    cg_activeMarkPolys.nextMark->prevMark = le;
    cg_activeMarkPolys.nextMark = le;
    
    return le;
}

/*
=================
idCGameMarks::ImpactMark

origin should be a point within a unit of the plane
dir should be the plane normal

temporary marks will not be stored or randomly oriented, but immediately
passed to the renderer.
=================
*/
#define MAX_MARK_FRAGMENTS  128
#define MAX_MARK_POINTS   384
void idCGameMarks::ImpactMark( qhandle_t markShader, const vec3_t origin, const vec3_t dir, float32 orientation, float32 red, float32 green, float32 blue, float32 alpha, bool alphaFade, float32 radius, bool temporary )
{
    vec3_t axis[ 3 ];
    float32 texCoordScale;
    vec3_t originalPoints[ 4 ];
    uchar8 colors[ 4 ];
    sint i, j, numFragments;
    markFragment_t markFragments[ MAX_MARK_FRAGMENTS ], *mf;
    vec3_t markPoints[ MAX_MARK_POINTS ];
    vec3_t projection;
    
    if( !cg_addMarks.integer )
    {
        return;
    }
    
    if( radius <= 0 )
    {
        Error( "CG_ImpactMark called with <= 0 radius" );
    }
    
    //if ( markTotal >= MAX_MARK_POLYS )
    //{
    //    return;
    //}
    
    // create the texture axis
    VectorNormalize2( dir, axis[ 0 ] );
    PerpendicularVector( axis[ 1 ], axis[ 0 ] );
    RotatePointAroundVector( axis[ 2 ], axis[ 0 ], axis[ 1 ], orientation );
    CrossProduct( axis[ 0 ], axis[ 2 ], axis[ 1 ] );
    
    texCoordScale = 0.5 * 1.0 / radius;
    
    // create the full polygon
    for( i = 0; i < 3; i++ )
    {
        originalPoints[ 0 ][ i ] = origin[ i ] - radius * axis[ 1 ][ i ] - radius * axis[ 2 ][ i ];
        originalPoints[ 1 ][ i ] = origin[ i ] + radius * axis[ 1 ][ i ] - radius * axis[ 2 ][ i ];
        originalPoints[ 2 ][ i ] = origin[ i ] + radius * axis[ 1 ][ i ] + radius * axis[ 2 ][ i ];
        originalPoints[ 3 ][ i ] = origin[ i ] - radius * axis[ 1 ][ i ] + radius * axis[ 2 ][ i ];
    }
    
    // get the fragments
    VectorScale( dir, -20, projection );
    numFragments = trap_CM_MarkFragments( 4, ( vec3_t* )originalPoints, projection, MAX_MARK_POINTS, markPoints[ 0 ], MAX_MARK_FRAGMENTS, markFragments );
    
    colors[ 0 ] = red * 255;
    colors[ 1 ] = green * 255;
    colors[ 2 ] = blue * 255;
    colors[ 3 ] = alpha * 255;
    
    for( i = 0, mf = markFragments; i < numFragments; i++, mf++ )
    {
        polyVert_t* v;
        polyVert_t verts[ MAX_VERTS_ON_POLY ];
        markPoly_t* mark;
        
        // we have an upper limit on the complexity of polygons
        // that we store persistantly
        if( mf->numPoints > MAX_VERTS_ON_POLY )
        {
            mf->numPoints = MAX_VERTS_ON_POLY;
        }
        
        for( j = 0, v = verts; j < mf->numPoints; j++, v++ )
        {
            vec3_t delta;
            
            VectorCopy( markPoints[ mf->firstPoint + j ], v->xyz );
            
            VectorSubtract( v->xyz, origin, delta );
            v->st[ 0 ] = 0.5 + DotProduct( delta, axis[ 1 ] ) * texCoordScale;
            v->st[ 1 ] = 0.5 + DotProduct( delta, axis[ 2 ] ) * texCoordScale;
            *( sint* )v->modulate = *( sint* )colors;
        }
        
        // if it is a temporary (shadow) mark, add it immediately and forget about it
        if( temporary )
        {
            trap_R_AddPolyToScene( markShader, mf->numPoints, verts );
            continue;
        }
        
        // otherwise save it persistantly
        mark = AllocMark( );
        mark->time = cg.time;
        mark->alphaFade = alphaFade;
        mark->markShader = markShader;
        mark->poly.numVerts = mf->numPoints;
        mark->color[ 0 ] = red;
        mark->color[ 1 ] = green;
        mark->color[ 2 ] = blue;
        mark->color[ 3 ] = alpha;
        ::memcpy( mark->verts, verts, mf->numPoints * sizeof( verts[ 0 ] ) );
        markTotal++;
    }
}

/*
===============
idCGameMarks::AddMarks
===============
*/
#define MARK_TOTAL_TIME   10000
#define MARK_FADE_TIME    1000
void idCGameMarks::AddMarks( void )
{
    sint j;
    markPoly_t* mp, *next;
    sint t, fade;
    
    if( !cg_addMarks.integer )
    {
        return;
    }
    
    mp = cg_activeMarkPolys.nextMark;
    for( ; mp != &cg_activeMarkPolys; mp = next )
    {
        // grab next now, so if the local entity is freed we
        // still have it
        next = mp->nextMark;
        
        // see if it is time to completely remove it
        if( cg.time > mp->time + MARK_TOTAL_TIME )
        {
            FreeMarkPoly( mp );
            continue;
        }
        
        // fade all marks out with time
        t = mp->time + MARK_TOTAL_TIME - cg.time;
        if( t < MARK_FADE_TIME )
        {
            fade = 255 * t / MARK_FADE_TIME;
            if( mp->alphaFade )
            {
                for( j = 0; j < mp->poly.numVerts; j++ )
                {
                    mp->verts[j].modulate[3] = fade;
                }
            }
            else
            {
                for( j = 0; j < mp->poly.numVerts; j++ )
                {
                    mp->verts[ j ].modulate[ 0 ] = mp->color[ 0 ] * fade;
                    mp->verts[ j ].modulate[ 1 ] = mp->color[ 1 ] * fade;
                    mp->verts[ j ].modulate[ 2 ] = mp->color[ 2 ] * fade;
                }
            }
        }
        
        trap_R_AddPolyToScene( mp->markShader, mp->poly.numVerts, mp->verts );
    }
}

