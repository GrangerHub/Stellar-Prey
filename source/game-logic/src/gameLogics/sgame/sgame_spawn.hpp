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
// File name:   sgame_spawn.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SGAME_SPAWN_H__
#define __SGAME_SPAWN_H__

class idSGameLocal;

class idSGameSpawn : public idSGameLocal {
public:
    idSGameSpawn();
    ~idSGameSpawn();

    static bool SpawnString(pointer key, pointer defaultString,
                            valueType **out);
    static bool SpawnFloat(pointer key, pointer defaultString, float32 *out);
    static bool SpawnInt(pointer key, pointer defaultString, sint *out);
    static bool SpawnVector(pointer key, pointer defaultString, float32 *out);
    static bool SpawnVector4(pointer key, pointer defaultString, float32 *out);
    static void SP_worldspawn(void);
    static void SpawnEntitiesFromString(void);
    static valueType *NewString(pointer string);
    static bool CallSpawn(gentity_t *ent);
    static void ParseField(pointer key, pointer value, gentity_t *ent);
    static void SpawnGEntityFromSpawnVars(void);
    static valueType *AddSpawnVarToken(pointer string);
    static bool ParseSpawnVars(void);
    static bool ParseSpawnVarsFromFile(char **data);
    static bool SpawnEntitiesFromFile(const char *filename, bool num);
};

#endif // !__SGAME_SPAWN_H__
