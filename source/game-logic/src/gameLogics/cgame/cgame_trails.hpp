////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 2018 - 2021 Dusan Jocic <dusanjocic@msn.com>
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
// File name:   cgame_trails.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CGAME_TRAILS_H__
#define __CGAME_TRAILS_H__

class idCGameLocal;

class idCGameTrails : public idCGameLocal
{
public:
    idCGameTrails();
    ~idCGameTrails();
    
    static void CalculateBeamNodeProperties( trailBeam_t* tb );
    static void LightVertex( vec3_t point, uchar8 alpha, uchar8* rgba );
    static void RenderBeam( trailBeam_t* tb );
    static trailBeamNode_t* AllocateBeamNode( trailBeam_t* tb );
    static trailBeamNode_t* DestroyBeamNode( trailBeamNode_t* tbn );
    static trailBeamNode_t* FindLastBeamNode( trailBeam_t* tb );
    static sint CountBeamNodes( trailBeam_t* tb );
    static trailBeamNode_t* PrependBeamNode( trailBeam_t* tb );
    static trailBeamNode_t* AppendBeamNode( trailBeam_t* tb );
    static void ApplyJitters( trailBeam_t* tb );
    static void UpdateBeam( trailBeam_t* tb );
    static bool ParseTrailBeamColor( uchar8* c, valueType** text_p );
    static bool ParseTrailBeam( baseTrailBeam_t* btb, valueType** text_p );
    static void InitialiseBaseTrailBeam( baseTrailBeam_t* btb );
    static bool ParseTrailSystem( baseTrailSystem_t* bts, valueType** text_p, pointer name );
    static bool ParseTrailFile( pointer fileName );
    static void LoadTrailSystems( void );
    static qhandle_t RegisterTrailSystem( valueType* name );
    static trailBeam_t* SpawnNewTrailBeam( baseTrailBeam_t* btb, trailSystem_t* parent );
    static trailSystem_t* SpawnNewTrailSystem( qhandle_t psHandle );
    static void DestroyTrailSystem( trailSystem_t** ts );
    static bool IsTrailSystemValid( trailSystem_t** ts );
    static void GarbageCollectTrailSystems( void );
    static void AddTrails( void );
    static void DestroyTestTS_f( void );
    static void TestTS_f( void );
};

#endif //!__CGAME_TRAILS_H__
