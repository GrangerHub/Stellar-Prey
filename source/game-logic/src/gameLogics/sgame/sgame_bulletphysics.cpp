////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 2012 V.
// Copyright(C) 2019 - 2020 Dusan Jocic <dusanjocic@msn.com>
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
// File name:   sgame_bulletphysics.cpp
// Version:     v1.00
// Created:
// Compilers:   Visual Studio 2019, gcc 7.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <sgame/sgame_precompiled.h>

/*
===============
idSGameBulletPhysics::idSGameBulletPhysics
===============
*/
idSGameBulletPhysics::idSGameBulletPhysics( void )
{
}

/*
===============
idSGamePtr::~idSGamePtr
===============
*/
idSGameBulletPhysics::~idSGameBulletPhysics( void )
{
}

btBroadphaseInterface* broadphase = 0;
btDefaultCollisionConfiguration* collisionConfiguration = 0;
btCollisionDispatcher* dispatcher = 0;
btSequentialImpulseConstraintSolver* solver = 0;
btDiscreteDynamicsWorld* dynamicsWorld = 0;
btKinematicCharacterController* character = 0;
btPairCachingGhostObject* ghostObject = 0;

/*
================
idBothGamesLocal::InitBullet
================
*/
void idSGameBulletPhysics::InitBullet( void )
{
    idSGameMain::Printf( "==== idSGameBulletPhysics::InitBullet ====\n" );
    
    // Build the broadphase
    broadphase = new btDbvtBroadphase();
    
    // Set up the collision configuration and dispatcher
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher( collisionConfiguration );
    
    // The actual physics solver
    solver = new btSequentialImpulseConstraintSolver;
    
    // The world.
    dynamicsWorld = new btDiscreteDynamicsWorld( dispatcher, broadphase, solver, collisionConfiguration );
    dynamicsWorld->setGravity( btVector3( 0, 0, -g_gravity.value ) );
    
    // add ghostPairCallback for character controller collision detection
    dynamicsWorld->getPairCache()->setInternalGhostPairCallback( new btGhostPairCallback() );
    
    btConvexShape* convexShape = new btCapsuleShapeZ( 48, 16 );
    btPairCachingGhostObject* ghostObject = new btPairCachingGhostObject();
    ghostObject->setCollisionShape( convexShape );
    ghostObject->setCollisionFlags( btCollisionObject::CF_CHARACTER_OBJECT );
    btTransform trans;
    trans.setIdentity();
    ghostObject->setWorldTransform( trans );
    
    character = new btKinematicCharacterController( ghostObject, convexShape, btScalar( .5f ) );
    character->setUseGhostSweepTest( true );
    character->setMaxSlope( DEG2RAD( 70 ) );
    character->setJumpSpeed( 200 );
    character->setFallSpeed( 800 );
    character->setGravity( g_gravity.value );
    
    dynamicsWorld->addCollisionObject( ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter );
    dynamicsWorld->addCharacter( character );
}

/*
================
idSGameBulletPhysics::TryToJump
================
*/
void idSGameBulletPhysics::TryToJump( void )
{
    character->jump();
}

/*
================
idSGameBulletPhysics::ShudownBullet
================
*/
void idSGameBulletPhysics::ShudownBullet( void )
{
    idSGameMain::Printf( "==== idSGameBulletPhysics::ShudownBullet ====\n" );
    
    // Clean up behind ourselves like good little programmers
    delete dynamicsWorld;
    dynamicsWorld = 0;
    delete solver;
    solver = 0;
    delete dispatcher;
    dispatcher = 0;
    delete collisionConfiguration;
    collisionConfiguration = 0;
    delete broadphase;
    broadphase = 0;
    delete character;
    character = 0;
    delete ghostObject;
    ghostObject = 0;
    last_time = 0;
}

/*
================
idSGameBulletPhysics::RunPhysics
================
*/
void idSGameBulletPhysics::RunPhysics( void )
{
    float32 time = level.time - last_time;
    dynamicsWorld->stepSimulation( time / 1000.0f, 10 );
    last_time = level.time;
}

/*
================
idSGameBulletPhysics::CreateWorldBrush
================
*/
float32 bt_collisionMargin = 4.f;
void idSGameBulletPhysics::CreateWorldBrush( btAlignedObjectArray<btVector3>& vertices )
{
    float32 mass = 0.f;
    btTransform startTransform;
    
    //can use a shift
    startTransform.setIdentity();
    
    //this create an internal copy of the vertices
    btCollisionShape* shape = new btConvexHullShape( &( vertices[0].getX() ), vertices.size() );
    shape->setMargin( bt_collisionMargin );
    
    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = ( mass != 0.f );
    
    btVector3 localInertia( 0, 0, 0 );
    
    if( isDynamic )
    {
        shape->calculateLocalInertia( mass, localInertia );
    }
    
#ifdef USE_MOTIONSTATE
    btDefaultMotionState* myMotionState = new btDefaultMotionState( startTransform );
    
    btRigidBody::btRigidBodyConstructionInfo cInfo( mass, myMotionState, shape, localInertia );
    
    btRigidBody* body = new btRigidBody( cInfo );
    //body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);
#else
    btRigidBody* body = new btRigidBody( mass, 0, shape, localInertia );
    body->setWorldTransform( startTransform );
#endif
    
    dynamicsWorld->addRigidBody( body );
    
}

/*
================
idSGameBulletPhysics::LoadMap
================
*/
void idSGameBulletPhysics::LoadMap( pointer mapName )
{
    idSGameMain::Printf( "==== idSGameBulletPhysics::LoadMap ====\n" );
    
    valueType buf[256];
    strcpy( buf, "maps/" );
    strcat( buf, mapName );
    strcat( buf, ".bsp" );
    fileHandle_t f;
    sint len = trap_FS_FOpenFile( buf, &f, fsMode_t::FS_READ );
    uchar8* data = ( uchar8* )malloc( len );
    trap_FS_Read( data, len, f );
    trap_FS_FCloseFile( f );
    
    dheader_t* h = ( dheader_t* )data;
    dbrush_t* b = ( dbrush_t* )( data + h->lumps[LUMP_BRUSHES].fileofs );
    dbrushside_t* sides = ( dbrushside_t* )( data + h->lumps[LUMP_BRUSHSIDES].fileofs );
    dshader_t* mats = ( dshader_t* )( data + h->lumps[LUMP_SHADERS].fileofs );
    dplane_t* planes = ( dplane_t* )( data + h->lumps[LUMP_PLANES].fileofs );
    sint numBrushes = h->lumps[LUMP_BRUSHES].filelen / sizeof( dbrush_t );
    
    for( sint i = 0; i < numBrushes; i++, b++ )
    {
        dshader_t& m = mats[b->shaderNum];
        
        if( ( m.contentFlags & 1 ) == false )
        {
            continue;
        }
        
        btAlignedObjectArray<btVector3> planeEquations;
        dbrushside_t* s = sides + b->firstSide;
        
        for( sint j = 0; j < b->numSides; j++, s++ )
        {
            dplane_t& plane = planes[s->planeNum];
            btVector3 planeEq;
            planeEq.setValue( plane.normal[0], plane.normal[1], plane.normal[2] );
            planeEq[3] = -plane.dist;
            planeEquations.push_back( planeEq );
        }
        
        btAlignedObjectArray<btVector3>	vertices;
        btGeometryUtil::getVerticesFromPlaneEquations( planeEquations, vertices );
        CreateWorldBrush( vertices );
    }
    
    free( data );
}

/*
================
idSGameBulletPhysics::RunCharacterController
================
*/
void idSGameBulletPhysics::RunCharacterController( vec3_t dir, vec3_t newPos )
{
    // set the forward direction of the character controller
    btVector3 walkDir( dir[0], dir[1], dir[2] );
    character->setWalkDirection( walkDir );
    
    //Com_Printf("moving at %f %f %f\n", pm->ps->origin[0], pm->ps->origin[1], pm->ps->origin[2]);
}

/*
================
idSGameBulletPhysics::FreeCharacter
================
*/
void idSGameBulletPhysics::FreeCharacter( btKinematicCharacterController* character )
{
    if( character == 0 )
    {
        return;
    }
    
    dynamicsWorld->removeCharacter( character );
    delete character;
    
}
/*
================
idSGameBulletPhysics::CreateCharacter
================
*/
btKinematicCharacterController* idSGameBulletPhysics::CreateCharacter( float32 stepHeight, vec3_t pos, float32 characterHeight, float32 characterWidth )
{
    btPairCachingGhostObject* ghostObject = new btPairCachingGhostObject();
    btConvexShape* characterShape = new btCapsuleShapeZ( characterWidth, characterHeight );
    btTransform trans;
    trans.setIdentity();
    btVector3 vPos( pos[0], pos[1], pos[2] );
    trans.setOrigin( vPos );
    ghostObject->setWorldTransform( trans );
    ghostObject->setCollisionShape( characterShape );
    btKinematicCharacterController* character = new btKinematicCharacterController( ghostObject, characterShape, stepHeight, 2 );
    character->setMaxSlope( DEG2RAD( 70 ) );
    character->setJumpSpeed( 200 );
    character->setFallSpeed( 800 );
    character->setGravity( 600 );
    
    dynamicsWorld->addCollisionObject( ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter | btBroadphaseProxy::CharacterFilter );
    
    dynamicsWorld->addCharacter( character );
    return character;
};

/*
================
idSGameBulletPhysics::RunCharacterController
================
*/
btRigidBody* idSGameBulletPhysics::CreateRigidBody( float32 mass, const btTransform& startTransform, btCollisionShape* shape )
{
    btAssert( ( !shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE ) );
    
    shape->setMargin( bt_collisionMargin );
    
    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = ( mass != 0.f );
    
    btVector3 localInertia( 0, 0, 0 );
    if( isDynamic )
    {
        shape->calculateLocalInertia( mass, localInertia );
    }
    
    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
#ifdef USE_MOTIONSTATE
    btDefaultMotionState* motionState = new btDefaultMotionState( startTransform );
    
    btRigidBody::btRigidBodyConstructionInfo cInfo( mass, motionState, shape, localInertia );
    
    btRigidBody* body = new btRigidBody( cInfo );
    body->setContactProcessingThreshold( BT_LARGE_FLOAT );
    
#else
    btRigidBody* body = new btRigidBody( mass, 0, shape, localInertia );
    body->setWorldTransform( startTransform );
#endif//
    
    dynamicsWorld->addRigidBody( body );
    
    return body;
}

/*
================
idSGameBulletPhysics::RunCharacterController
================
*/
void idSGameBulletPhysics::QuatToAngles( const float* q, vec3_t angles )
{
    float32 q2[4];
    
    q2[0] = q[0] * q[0];
    q2[1] = q[1] * q[1];
    q2[2] = q[2] * q[2];
    q2[3] = q[3] * q[3];
    
    angles[PITCH] = RAD2DEG( asin( -2 * ( q[2] * q[0] - q[3] * q[1] ) ) );
    angles[YAW] = RAD2DEG( atan2( 2 * ( q[2] * q[3] + q[0] * q[1] ), ( q2[2] - q2[3] - q2[0] + q2[1] ) ) );
    angles[ROLL] = RAD2DEG( atan2( 2 * ( q[3] * q[0] + q[2] * q[1] ), ( -q2[2] - q2[3] + q2[0] + q2[1] ) ) );
}

// NOTE: w is a fourth component
class quat_c
{
public:
    union
    {
        struct
        {
            float32 x, y, z, w;
        };
        float32 val[4];
    };
    
    quat_c( float32 nX, float32 nY, float32 nZ, float32 nW )
    {
        x = nX;
        y = nY;
        z = nZ;
        w = nW;
    }
    // fast-access operators
    inline operator float32* () const
    {
        return ( float32* )& x;
    }
};

/*
================
idSGameBulletPhysics::RunCharacterController
================
*/
void idSGameBulletPhysics::UpdatePhysicsObject( gentity_s* ent )
{
    btRigidBody* body = ent->body;
    
    if( body == 0 )
    {
        return;
    }
    
    btTransform trans;
    body->getMotionState()->getWorldTransform( trans );
    VectorSet( ent->s.origin, trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z() );
    
    //idSGameMain::Printf( "idSGameBulletPhysics::UpdatePhysicsObject: at %f %f %f\n", ent->s.origin[0], ent->s.origin[1], ent->s.origin[2] );
    
    btQuaternion q = trans.getRotation();
    quat_c q2( q.x(), q.y(), q.z(), q.w() );
    QuatToAngles( q2, ent->s.angles );
}
