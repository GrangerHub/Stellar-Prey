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
// File name:   cgame_playerstate.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CGAME_PLAYERSTATE_H__
#define __CGAME_PLAYERSTATE_H__

class idCGameLocal;

class idCGamePlayerState : public idCGameLocal
{
public:
    idCGamePlayerState();
    ~idCGamePlayerState();
    
    static void DamageFeedback( sint yawByte, sint pitchByte, sint damage );
    static void Respawn( void );
    static void CheckPlayerstateEvents( playerState_t* ps, playerState_t* ops );
    void CheckChangedPredictableEvents( playerState_t* ps );
    static void CheckLocalSounds( playerState_t* ps, playerState_t* ops );
    static void TransitionPlayerState( playerState_t* ps, playerState_t* ops );
};

#endif //!__CGAME_PLAYERSTATE_H__
