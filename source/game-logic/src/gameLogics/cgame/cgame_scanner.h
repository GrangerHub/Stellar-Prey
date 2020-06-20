////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 2018 Dusan Jocic <dusanjocic@msn.com>
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
// File name:   cgame_scanner.h
// Version:     v1.01
// Created:
// Compilers:   Visual Studio 2017, gcc 7.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CGAME_SCANNER_H__
#define __CGAME_SCANNER_H__

class idCGameLocal;

class idCGameScanner : public idCGameLocal
{
public:
    idCGameScanner();
    ~idCGameScanner();
    
    static void UpdateEntityPositions( void );
    static void DrawBlips( rectDef_t* rect, vec3_t origin, vec4_t colour );
    static void DrawDir( rectDef_t* rect, vec3_t origin, vec4_t colour );
    static void AlienSense( rectDef_t* rect );
    static void Scanner( rectDef_t* rect, qhandle_t shader, vec4_t color );
};

#endif //!__CGAME_SCANNER_H__
