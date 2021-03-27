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
// File name:   sgame_svcmds.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SGAME_SVCMDS_H__
#define __SGAME_SVCMDS_H__

class idSGameLocal;

class idSGameSvcmds : public idSGameLocal
{
public:
    idSGameSvcmds();
    ~idSGameSvcmds();
    
    static void EntityList_f( void );
    static gclient_t* ClientForString( valueType* s );
    static void Svcmd_Status_f( void );
    static void Svcmd_ForceTeam_f( void );
    static void Svcmd_LayoutSave_f( void );
    static void Svcmd_LayoutLoad_f( void );
    static void Svcmd_AdmitDefeat_f( void );
    static void Svcmd_TeamWin_f( void );
    static void Svcmd_Evacuation_f( void );
    static void Svcmd_MapRotation_f( void );
    static void Svcmd_TeamMessage_f( void );
    static void Svcmd_SendMessage( void );
    static void Svcmd_CenterPrint_f( void );
    static void Svcmd_EjectClient_f( void );
    static void Svcmd_DumpUser_f( void );
    static void Svcmd_PrintQueue_f( void );
    static void Svcmd_MessageWrapper( void );
};

#endif //!__SGAME_SVCMDS_H__