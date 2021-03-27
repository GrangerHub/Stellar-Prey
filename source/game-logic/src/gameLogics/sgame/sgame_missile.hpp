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
// File name:   sgame_missle.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SGAME_MISSILE_H__
#define __SGAME_MISSILE_H__

#define MISSILE_PRESTEP_TIME  50

class idSGameLocal;

class idSGameMissile : public idSGameLocal
{
public:
    idSGameMissile();
    ~idSGameMissile();
    
    static void BounceMissile( gentity_t* ent, trace_t* trace );
    static void ExplodeMissile( gentity_t* ent );
    static void MissileImpact( gentity_t* ent, trace_t* trace );
    static void AHive_SearchAndDestroy( gentity_t* self );
    static gentity_t* fire_flamer( gentity_t* self, vec3_t start, vec3_t aimdir );
    static gentity_t* fire_blaster( gentity_t* self, vec3_t start, vec3_t dir );
    static gentity_t* fire_pulseRifle( gentity_t* self, vec3_t start, vec3_t dir );
    static gentity_t* fire_luciferCannon( gentity_t* self, vec3_t start, vec3_t dir, sint damage, sint radius, sint speed );
    static gentity_t* fire_lockblob( gentity_t* self, vec3_t start, vec3_t dir );
    static gentity_t* fire_slowBlob( gentity_t* self, vec3_t start, vec3_t dir );
    static gentity_t* fire_bounceBall( gentity_t* self, vec3_t start, vec3_t dir );
    static gentity_t* fire_hive( gentity_t* self, vec3_t start, vec3_t dir );
    static gentity_t* launch_grenade( gentity_t* self, vec3_t start, vec3_t dir );
    static void RunMissile( gentity_t* ent );
    
};

#endif // !__SGAME_MISSILE_H__
