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
// File name:   cgame_animmapobj.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CGAME_ANIMMAPOBJ_H__
#define __CGAME_ANIMMAPOBJ_H__

class idCGameLocal;

class idCGameAnimMapObj : public idCGameLocal
{
public:
    idCGameAnimMapObj();
    ~idCGameAnimMapObj();
    
    static void DoorAnimation( centity_t* cent, sint* old, sint* now, float32* backLerp );
    static void ModelDoor( centity_t* cent );
    static void AMOAnimation( centity_t* cent, sint* old, sint* now, float32* backLerp );
    static void AnimMapObj( centity_t* cent );
    
    friend class idCGameAnimation;
};

#endif //!__CGAME_ANIMMAPOBJ_H__
