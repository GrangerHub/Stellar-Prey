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
// File name:   cgame_consolecmds.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CGAME_CONSOLECMDS_H__
#define __CGAME_CONSOLECMDS_H__

class idCGameLocal;

class idCGameConsoleCmds : public idCGameLocal
{
public:
    idCGameConsoleCmds();
    ~idCGameConsoleCmds();
    
    static void SizeUp_f( void );
    static void SizeDown_f( void );
    static void Viewpos_f( void );
    static bool RequestScores( void );
    static void scrollScoresDown_f( void );
    static void scrollScoresUp_f( void );
    static void ScoresDown_f( void );
    static void ScoresUp_f( void );
    static void TellTarget_f( void );
    static void TellAttacker_f( void );
    static void SquadMark_f( void );
    static void InitConsoleCommands( void );
};

#endif //!__CGAME_CONSOLECMDS_H__
