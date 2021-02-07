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
// File name:   cgame_predict.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CGAME_PREDICT_H__
#define __CGAME_PREDICT_H__

class idCGameLocal;

class idCGamePredict : public idCGameLocal
{
public:
    idCGamePredict();
    ~idCGamePredict();
    
    static void BuildSolidList( void );
    static void ClipMoveToEntities( const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, sint skipNumber, sint mask, trace_t* tr, traceType_t collisionType );
    static void Trace( trace_t* result, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, sint skipNumber, sint mask );
    static void CapTrace( trace_t* result, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, sint skipNumber, sint mask );
    static void BiSphereTrace( trace_t* result, const vec3_t start, const vec3_t end, const float32 startRadius, const float32 endRadius, sint skipNumber, sint mask );
    static sint PointContents( const vec3_t point, sint passEntityNum );
    static void InterpolatePlayerState( bool grabAngles );
    static void TouchTriggerPrediction( void );
    static sint IsUnacceptableError( playerState_t* ps, playerState_t* pps );
    static void PredictPlayerState( void );
};

#endif //!__CGAME_PREDICT_H__
