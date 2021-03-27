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
// File name:   cgame_drawtools.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CGAME_DRAWTOOLS_H__
#define __CGAME_DRAWTOOLS_H__

class idCGameLocal;

class idCGameDrawTools : public idCGameLocal
{
public:
    idCGameDrawTools();
    ~idCGameDrawTools();
    
    void DrawPlane( vec3_t origin, vec3_t down, vec3_t right, qhandle_t shader );
    static void AdjustFrom640( float32* x, float32* y, float32* w, float32* h );
    static void DrawSides( float32 x, float32 y, float32 w, float32 h, float32 size );
    static void DrawTopBottom( float32 x, float32 y, float32 w, float32 h, float32 size );
    static void DrawCorners( float32 x, float32 y, float32 w, float32 h, float32 size, qhandle_t pic );
    static void DrawRect( float32 x, float32 y, float32 width, float32 height, float32 size, const float32* color );
    static void DrawRoundedRect( float32 x, float32 y, float32 width, float32 height, float32 size, const float32* color );
    static void FillRect( float32 x, float32 y, float32 width, float32 height, const float32* color );
    static void FillRoundedRect( float32 x, float32 y, float32 width, float32 height, float32 size, const float32* color );
    static void DrawPic( float32 x, float32 y, float32 width, float32 height, qhandle_t hShader );
    void DrawFadePic( float32 x, float32 y, float32 width, float32 height, vec4_t fcolor, vec4_t tcolor, float32 amount, qhandle_t hShader );
    static sint DrawStrlen( pointer str );
    static void TileClearBox( sint x, sint y, sint w, sint h, qhandle_t hShader );
    static void TileClear( void );
    static float32* FadeColor( sint startMsec, sint totalMsec );
    static bool WorldToScreen( vec3_t poS32, float32* x, float32* y );
    static bool WorldToScreenWrap( vec3_t poS32, float32* x, float32* y );
    static valueType* KeyBinding( pointer bind );
    
};

#endif //!__CGAME_DRAWTOOLS_H__
