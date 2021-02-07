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
// File name:   cgame_tutorial.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CGAME_TUTORIAL_H__
#define __CGAME_TUTORIAL_H__

class idCGameLocal;

class idCGameTutorial : public idCGameLocal
{
public:
    idCGameTutorial();
    ~idCGameTutorial();
    
    static void GetBindings( void );
    static void FixBindings( void );
    static pointer KeyNameForCommand( pointer command );
    static entityState_t* BuildableInRange( playerState_t* ps, float32* healthFraction );
    static void AlienBuilderText( valueType* text, playerState_t* ps );
    static void AlienLevel0Text( valueType* text, playerState_t* ps );
    static void AlienLevel1Text( valueType* text, playerState_t* ps );
    static void AlienLevel2Text( valueType* text, playerState_t* ps );
    static void AlienLevel3Text( valueType* text, playerState_t* ps );
    static void AlienLevel4Text( valueType* text, playerState_t* ps );
    static void HumanCkitText( valueType* text, playerState_t* ps );
    static void HumanText( valueType* text, playerState_t* ps );
    static void SpectatorText( valueType* text, playerState_t* ps );
    static pointer TutorialText( void );
};

#endif //!__CGAME_TUTORIAL_H__
