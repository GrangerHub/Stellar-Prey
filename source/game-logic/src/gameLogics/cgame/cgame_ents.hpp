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
// File name:   cgame_ents.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CGAME_ENTS_H__
#define __CGAME_ENTS_H__

class idCGameLocal;

class idCGameEnts : public idCGameLocal
{
public:
    idCGameEnts();
    ~idCGameEnts();
    
    static void DrawBoxFace( vec3_t a, vec3_t b, vec3_t c, vec3_t d );
    static void DrawBoundingBox( vec3_t origin, vec3_t mins, vec3_t maxs );
    static void PositionEntityOnTag( refEntity_t* entity, const refEntity_t* parent, qhandle_t parentModel, valueType* tagName );
    static void PositionRotatedEntityOnTag( refEntity_t* entity, const refEntity_t* parent, qhandle_t parentModel, valueType* tagName );
    static void SetEntitySoundPosition( centity_t* cent );
    static void EntityEffects( centity_t* cent );
    static void General( centity_t* cent );
    static void Speaker( centity_t* cent );
    static void LaunchMissile( centity_t* cent );
    static void Missile( centity_t* cent );
    static void Mover( centity_t* cent );
    static void Beam( centity_t* cent );
    static void Portal( centity_t* cent );
    static void LightFlare( centity_t* cent );
    static void AdjustPositionForMover( const vec3_t in, sint moverNum, sint fromTime, sint toTime, vec3_t out );
    static void InterpolateEntityPosition( centity_t* cent );
    static void CalcEntityLerpPositions( centity_t* cent );
    static void CEntityPVSEnter( centity_t* cent );
    static void CEntityPVSLeave( centity_t* cent );
    static void AddCEntity( centity_t* cent );
    static void AddPacketEntities( void );
    static void LinkLocation( centity_t* cent );
    static bool InFOV( vec3_t spot, vec3_t from, vec3_t fromAngles, sint hFOV, sint vFOV );
};

#endif //!__CGAME_ENTS_H__
