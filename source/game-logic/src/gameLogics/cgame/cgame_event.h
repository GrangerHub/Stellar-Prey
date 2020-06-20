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
// File name:   cgame_event.h
// Version:     v1.00
// Created:
// Compilers:   Visual Studio 2015
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CGAME_EVENT_H__
#define __CGAME_EVENT_H__

class idCGameLocal;

class idCGameEvent : public idCGameLocal
{
public:
    idCGameEvent();
    ~idCGameEvent();
    
    static void Obituary( entityState_t* ent );
    static void PainEvent( centity_t* cent, sint health );
    static void Level2Zap( entityState_t* es );
    static void EntityEvent( centity_t* cent, vec3_t position );
    static void CheckEvents( centity_t* cent );
    
};

#endif //!__CGAME_ANIMINATION_H__
