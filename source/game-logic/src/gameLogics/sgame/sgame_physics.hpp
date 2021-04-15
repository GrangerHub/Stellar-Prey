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
// File name:   sgame_physics.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SGAME_PHYSICS_H__
#define __SGAME_PHYSICS_H__

typedef struct {
    gentity_t *ent;
    vec3_t origin;
    vec3_t angles;
    float32 deltayaw;
} pushed_t;

static pushed_t pushed[MAX_GENTITIES], *pushed_p;

class idSGameLocal;

class idSGamePhysics : public idSGameLocal {
public:
    idSGamePhysics();
    ~idSGamePhysics();

    static void Bounce(gentity_t *ent, trace_t *trace);
    static void Physics(gentity_t *ent, sint msec);
};

#endif // !__SGAME_PHYSICS_H__