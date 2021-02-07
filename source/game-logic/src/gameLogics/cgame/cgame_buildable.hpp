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
// File name:   cgame_buildable.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CGAME_BUILDABLE_H__
#define __CGAME_BUILDABLE_H__

class idCGameLocal;

class idCGameBuildable : public idCGameLocal
{
public:
    idCGameBuildable();
    ~idCGameBuildable();
    
    static void AlienBuildableExplosion( vec3_t origin, vec3_t dir );
    static void HumanBuildableExplosion( vec3_t origin, vec3_t dir );
    static void Creep( centity_t* cent );
    static bool ParseBuildableAnimationFile( pointer filename, buildable_t buildable );
    static bool ParseBuildableSoundFile( pointer filename, buildable_t buildable );
    static void InitBuildables( void );
    static void SetBuildableLerpFrameAnimation( buildable_t buildable, lerpFrame_t* lf, sint newAnimation );
    static void RunBuildableLerpFrame( centity_t* cent );
    static void BuildableAnimation( centity_t* cent, sint* old, sint* now, float32* backLerp );
    static void PositionAndOrientateBuildable( const vec3_t angles, const vec3_t inOrigin, const vec3_t normal, const sint skipNumber, const vec3_t mins, const vec3_t maxs, vec3_t outAxis[3], vec3_t outOrigin );
    static void GhostBuildable( buildable_t buildable );
    static void BuildableParticleEffects( centity_t* cent );
    static void BuildableStatusParse( pointer filename, buildStat_t* bs );
    static void BuildableStatusDisplay( centity_t* cent );
    static sint SortDistance( const void* a, const void* b );
    static bool PlayerIsBuilder( buildable_t buildable );
    static bool BuildableRemovalPending( sint entityNum );
    static void DrawBuildableStatus( void );
    static void Buildable( centity_t* cent );
};

#endif //!__CGAME_BUILDABLE_H__
