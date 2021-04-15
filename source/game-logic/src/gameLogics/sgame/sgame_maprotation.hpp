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
// File name:   sgame_maprotation.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SGAME_MAPROTATION_H__
#define __SGAME_MAPROTATION_H__

static mapRotations_t mapRotations;

class idSGameLocal;

class idSGameMaprotation : public idSGameLocal {
public:
    idSGameMaprotation();
    ~idSGameMaprotation();

    static sint *GetCurrentMapArray(void);
    static void SetCurrentMap(sint currentMap, sint rotation);
    static sint GetCurrentMap(sint rotation);
    static void IssueMapChange(sint rotation);
    static void AdvanceMapRotation(void);
    static bool StartMapRotation(valueType *name, bool changeMap);
    static void StopMapRotation(void);
    static bool MapRotationActive(void);
    static mapConditionType_t ResolveConditionDestination(sint *n,
            valueType *name);
    static bool EvaluateMapCondition(mapRotationCondition_t *mrc);
    static void InitMapRotations(void);
    static bool MapExists(valueType *name);
    static bool RotationExists(valueType *name);
    static bool ParseMapCommandSection(mapRotationEntry_t *mre,
                                       valueType **text_p);
    static bool ParseMapRotation(mapRotation_t *mr, valueType **text_p);
    static bool ParseMapRotationFile(pointer fileName);
    static void PrintRotations(void);
};

#endif // !__SGAME_MAPROTATION_H__