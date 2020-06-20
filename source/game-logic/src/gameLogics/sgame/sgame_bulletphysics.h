////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 2019 Dusan Jocic <dusanjocic@msn.com>
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
// File name:   sgame_bulletphysics.h
// Version:     v1.00
// Created:
// Compilers:   Visual Studio 2019, gcc 7.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SGAME_BULLETPHYSICS_H__
#define __SGAME_BULLETPHYSICS_H__

static sint last_time = 0;

//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
#define USE_MOTIONSTATE 1

class idSGameLocal;

class idSGameBulletPhysics : public idSGameLocal
{
public:
    idSGameBulletPhysics();
    ~idSGameBulletPhysics();
    
    static void InitBullet( void );
    static void ShudownBullet( void );
    static void RunPhysics( void );
    static void CreateWorldBrush( btAlignedObjectArray<btVector3>& vertices );
    static void LoadMap( pointer mapName );
    static void RunCharacterController( vec3_t dir, vec3_t newPos );
    static void TryToJump( void );
    static void UpdatePhysicsObject( gentity_s* ent );
    static void QuatToAngles( const float32* q, vec3_t angles );
    static btRigidBody* CreateRigidBody( float32 mass, const btTransform& startTransform, btCollisionShape* shape );
    static void FreeCharacter( btKinematicCharacterController* c );
    static btKinematicCharacterController* CreateCharacter( float32 stepHeight, vec3_t pos, float32 characterHeight, float32 characterWidth );
};


#endif // !__SGAME_BULLETPHYSICS_H__