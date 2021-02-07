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
// File name:   sgame_team.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SGAME_TEAM_H__
#define __SGAME_TEAM_H__

class idSGameLocal;

class idSGameTeam : public idSGameLocal
{
public:
    idSGameTeam();
    ~idSGameTeam();
    
    static void PrintMsg( gentity_t* ent, pointer fmt, ... );
    static team_t TeamFromString( valueType* str );
    static bool OnSameTeam( gentity_t* ent1, gentity_t* ent2 );
    static void LeaveTeam( gentity_t* self );
    static void ChangeTeam( gentity_t* ent, team_t newTeam );
    static gentity_t* Team_GetLocation( gentity_t* ent );
    static bool Team_GetLocationMsg( gentity_t* ent, valueType* loc, sint loclen );
    static void TeamplayInfoMessage( gentity_t* ent );
    static void CheckTeamStatus( void );
    static sint SortClients( const void* a, const void* b );
};

#endif //!__SGAME_TEAM_H__
