////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 1999 - 2005 Id Software, Inc.
// Copyright(C) 2000 - 2006 Tim Angus
// Copyright(C) 2011 - 2021 Dusan Jocic <dusanjocic@msn.com>
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
// File name:   sgame_maprotation.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description: the map rotation system
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <sgame/sgame_precompiled.hpp>

/*
===============
idSGameMaprotation::idSGameMaprotation
===============
*/
idSGameMaprotation::idSGameMaprotation(void) {
}

/*
===============
idSGameMaprotation::~idSGameMaprotation
===============
*/
idSGameMaprotation::~idSGameMaprotation(void) {
}

/*
===============
idSGameMaprotation::MapExists

Check if a map exists
===============
*/
bool idSGameMaprotation::MapExists(valueType *name) {
    return trap_FS_FOpenFile(va("maps/%s.world", name), nullptr, FS_READ);
}

/*
===============
idSGameMaprotation::RotationExists

Check if a rotation exists
===============
*/
bool idSGameMaprotation::RotationExists(valueType *name) {
    sint i;

    for(i = 0; i < mapRotations.numRotations; i++) {
        if(Q_strncmp(mapRotations.rotations[ i ].name, name, MAX_QPATH) == 0) {
            return true;
        }
    }

    return false;
}

/*
===============
idSGameMaprotation::ParseCommandSection

Parse a map rotation command section
===============
*/
bool idSGameMaprotation::ParseMapCommandSection(mapRotationEntry_t *mre,
        valueType **text_p) {
    valueType *token;

    // read optional parameters
    while(1) {
        token = COM_Parse(text_p);

        if(!token) {
            break;
        }

        if(!Q_stricmp(token, "")) {
            return false;
        }

        if(!Q_stricmp(token, "}")) {
            return true;    //reached the end of this command section
        }

        if(!Q_stricmp(token, "layouts")) {
            token = COM_ParseExt(text_p, false);
            mre->layouts[ 0 ] = '\0';

            while(token && token[ 0 ] != 0) {
                Q_strcat(mre->layouts, sizeof(mre->layouts), token);
                Q_strcat(mre->layouts, sizeof(mre->layouts), " ");
                token = COM_ParseExt(text_p, false);
            }

            continue;
        }

        if(mre->numCmds == MAX_MAP_COMMANDS) {
            idSGameMain::Printf(S_COLOR_RED
                                "ERROR: maximum number of map commands (%d) reached\n", MAX_MAP_COMMANDS);
            return false;
        }

        Q_strncpyz(mre->postCmds[ mre->numCmds ], token,
                   sizeof(mre->postCmds[ 0 ]));
        Q_strcat(mre->postCmds[ mre->numCmds ], sizeof(mre->postCmds[ 0 ]), " ");

        token = COM_ParseExt(text_p, false);

        while(token && token[ 0 ] != 0) {
            Q_strcat(mre->postCmds[ mre->numCmds ], sizeof(mre->postCmds[ 0 ]), token);
            Q_strcat(mre->postCmds[ mre->numCmds ], sizeof(mre->postCmds[ 0 ]), " ");
            token = COM_ParseExt(text_p, false);
        }

        mre->numCmds++;
    }

    return false;
}

/*
===============
G_ParseMapRotation

Parse a map rotation section
===============
*/
bool idSGameMaprotation::ParseMapRotation(mapRotation_t *mr,
        valueType **text_p) {
    valueType *token;
    bool mnSet = false;
    mapRotationEntry_t *mre = nullptr;
    mapRotationCondition_t  *mrc;

    // read optional parameters
    while(1) {
        token = COM_Parse(text_p);

        if(!token) {
            break;
        }

        if(!Q_stricmp(token, "")) {
            return false;
        }

        if(!Q_stricmp(token, "{")) {
            if(!mnSet) {
                idSGameMain::Printf(S_COLOR_RED
                                    "ERROR: map settings section with no name\n");
                return false;
            }

            if(!ParseMapCommandSection(mre, text_p)) {
                idSGameMain::Printf(S_COLOR_RED
                                    "ERROR: failed to parse map command section\n");
                return false;
            }

            mnSet = false;
            continue;
        } else if(!Q_stricmp(token, "goto")) {
            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            if(mre->numConditions == MAX_MAP_ROTATION_CONDS) {
                idSGameMain::Printf(S_COLOR_RED
                                    "ERROR: maximum number of conditions for one map (%d) reached\n",
                                    MAX_MAP_ROTATION_CONDS);
                return false;
            }

            mrc = &mre->conditions[ mre->numConditions++ ];
            mrc->unconditional = true;
            Q_strncpyz(mrc->dest, token, sizeof(mrc->dest));

            continue;
        } else if(!Q_stricmp(token, "if")) {
            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            if(mre->numConditions == MAX_MAP_ROTATION_CONDS) {
                idSGameMain::Printf(S_COLOR_RED
                                    "ERROR: maximum number of conditions for one map (%d) reached\n",
                                    MAX_MAP_ROTATION_CONDS);
                return false;
            }

            mrc = &mre->conditions[ mre->numConditions++ ];

            if(!Q_stricmp(token, "numClients")) {
                mrc->lhs = MCV_NUMCLIENTS;

                token = COM_Parse(text_p);

                if(!token) {
                    break;
                }

                if(!Q_stricmp(token, "<")) {
                    mrc->op = MCO_LT;
                } else if(!Q_stricmp(token, ">")) {
                    mrc->op = MCO_GT;
                } else if(!Q_stricmp(token, "=")) {
                    mrc->op = MCO_EQ;
                } else {
                    idSGameMain::Printf(S_COLOR_RED
                                        "ERROR: invalid operator in expression: %s\n", token);
                    return false;
                }

                token = COM_Parse(text_p);

                if(!token) {
                    break;
                }

                mrc->numClients = atoi(token);
            } else if(!Q_stricmp(token, "lastWin")) {
                mrc->lhs = MCV_LASTWIN;

                token = COM_Parse(text_p);

                if(!token) {
                    break;
                }

                if(!Q_stricmp(token, "aliens")) {
                    mrc->lastWin = TEAM_ALIENS;
                } else if(!Q_stricmp(token, "humans")) {
                    mrc->lastWin = TEAM_HUMANS;
                } else {
                    idSGameMain::Printf(S_COLOR_RED
                                        "ERROR: invalid right hand side in expression: %s\n", token);
                    return false;
                }
            } else if(!Q_stricmp(token, "random")) {
                mrc->lhs = MCV_RANDOM;
            } else {
                idSGameMain::Printf(S_COLOR_RED
                                    "ERROR: invalid left hand side in expression: %s\n", token);
                return false;
            }

            token = COM_Parse(text_p);

            if(!token) {
                break;
            }

            mrc->unconditional = false;
            Q_strncpyz(mrc->dest, token, sizeof(mrc->dest));

            continue;
        } else if(!Q_stricmp(token, "}")) {
            return true;    //reached the end of this map rotation
        }

        if(mr->numMaps == MAX_MAP_ROTATION_MAPS) {
            idSGameMain::Printf(S_COLOR_RED
                                "ERROR: maximum number of maps in one rotation (%d) reached\n",
                                MAX_MAP_ROTATION_MAPS);
            return false;
        }

        mre = &mr->maps[ mr->numMaps++ ];

        Q_strncpyz(mre->name, token, sizeof(mre->name));
        mnSet = true;
    }

    return false;
}

/*
===============
idSGameMaprotation::ParseMapRotationFile

Load the map rotations from a map rotation file
===============
*/
bool idSGameMaprotation::ParseMapRotationFile(pointer fileName) {
    valueType *text_p;
    sint i, j, k, len;
    valueType *token;
    valueType text[ 20000 ];
    valueType mrName[ MAX_QPATH ];
    bool mrNameSet = false;
    fileHandle_t  f;

    // load the file
    len = trap_FS_FOpenFile(fileName, &f, FS_READ);

    if(len < 0) {
        return false;
    }

    if(len == 0 || len >= sizeof(text) - 1) {
        trap_FS_FCloseFile(f);
        idSGameMain::Printf(S_COLOR_RED "ERROR: map rotation file %s is %s\n",
                            fileName, len == 0 ? "empty" : "too long");
        return false;
    }

    trap_FS_Read(text, len, f);
    text[ len ] = 0;
    trap_FS_FCloseFile(f);

    // parse the text
    text_p = text;

    // read optional parameters
    while(1) {
        token = COM_Parse(&text_p);

        if(!token) {
            break;
        }

        if(!Q_stricmp(token, "")) {
            break;
        }

        if(!Q_stricmp(token, "{")) {
            if(mrNameSet) {
                //check for name space clashes
                for(i = 0; i < mapRotations.numRotations; i++) {
                    if(!Q_stricmp(mapRotations.rotations[ i ].name, mrName)) {
                        idSGameMain::Printf(S_COLOR_RED
                                            "ERROR: a map rotation is already named %s\n", mrName);
                        return false;
                    }
                }

                if(mapRotations.numRotations == MAX_MAP_ROTATIONS) {
                    idSGameMain::Printf(S_COLOR_RED
                                        "ERROR: maximum number of map rotations (%d) reached\n",
                                        MAX_MAP_ROTATIONS);
                    return false;
                }

                Q_strncpyz(mapRotations.rotations[ mapRotations.numRotations ].name,
                           mrName, MAX_QPATH);

                if(!ParseMapRotation(&mapRotations.rotations[ mapRotations.numRotations ],
                                     &text_p)) {
                    idSGameMain::Printf(S_COLOR_RED
                                        "ERROR: %s: failed to parse map rotation %s\n", fileName, mrName);
                    return false;
                }

                mapRotations.numRotations++;

                //start parsing map rotations again
                mrNameSet = false;

                continue;
            } else {
                idSGameMain::Printf(S_COLOR_RED "ERROR: unnamed map rotation\n");
                return false;
            }
        }

        if(!mrNameSet) {
            Q_strncpyz(mrName, token, sizeof(mrName));
            mrNameSet = true;
        } else {
            idSGameMain::Printf(S_COLOR_RED "ERROR: map rotation already named\n");
            return false;
        }
    }

    for(i = 0; i < mapRotations.numRotations; i++) {
        for(j = 0; j < mapRotations.rotations[ i ].numMaps; j++) {
            if(!MapExists(mapRotations.rotations[ i ].maps[ j ].name)) {
                idSGameMain::Printf(S_COLOR_RED "ERROR: map \"%s\" doesn't exist\n",
                                    mapRotations.rotations[ i ].maps[ j ].name);
                return false;
            }

            for(k = 0; k < mapRotations.rotations[ i ].maps[ j ].numConditions; k++) {
                if(!MapExists(mapRotations.rotations[ i ].maps[ j ].conditions[ k ].dest)
                        &&
                        !RotationExists(
                            mapRotations.rotations[ i ].maps[ j ].conditions[ k ].dest)) {
                    idSGameMain::Printf(S_COLOR_RED
                                        "ERROR: conditional destination \"%s\" doesn't exist\n",
                                        mapRotations.rotations[ i ].maps[ j ].conditions[ k ].dest);
                    return false;
                }

            }

        }
    }

    return true;
}

/*
===============
idSGameMaprotation::PrintRotations

Print the parsed map rotations
===============
*/
void idSGameMaprotation::PrintRotations(void) {
    sint i, j, k;

    idSGameMain::Printf("Map rotations as parsed:\n\n");

    for(i = 0; i < mapRotations.numRotations; i++) {
        idSGameMain::Printf("rotation: %s\n{\n", mapRotations.rotations[ i ].name);

        for(j = 0; j < mapRotations.rotations[ i ].numMaps; j++) {
            idSGameMain::Printf("  map: %s\n  {\n",
                                mapRotations.rotations[ i ].maps[ j ].name);

            for(k = 0; k < mapRotations.rotations[ i ].maps[ j ].numCmds; k++) {
                idSGameMain::Printf("    command: %s\n",
                                    mapRotations.rotations[ i ].maps[ j ].postCmds[ k ]);
            }

            idSGameMain::Printf("  }\n");

            for(k = 0; k < mapRotations.rotations[ i ].maps[ j ].numConditions; k++) {
                idSGameMain::Printf("  conditional: %s\n",
                                    mapRotations.rotations[ i ].maps[ j ].conditions[ k ].dest);
            }

        }

        idSGameMain::Printf("}\n");
    }

    idSGameMain::Printf("Total memory used: %d bytes\n", sizeof(mapRotations));
}

/*
===============
idSGameMaprotation::GetCurrentMapArray

Fill a static array with the current map of each rotation
===============
*/
sint *idSGameMaprotation::GetCurrentMapArray(void) {
    static sint  currentMap[ MAX_MAP_ROTATIONS ];
    sint         i = 0;
    valueType        text[ MAX_MAP_ROTATIONS * 2 ];
    valueType        *text_p, *token;

    Q_strncpyz(text, g_currentMap.string, sizeof(text));

    text_p = text;

    while(1) {
        token = COM_Parse(&text_p);

        if(!token) {
            break;
        }

        if(!Q_stricmp(token, "")) {
            break;
        }

        currentMap[ i++ ] = atoi(token);
    }

    return currentMap;
}

/*
===============
idSGameMaprotation::SetCurrentMap

Set the current map in some rotation
===============
*/
void idSGameMaprotation::SetCurrentMap(sint currentMap, sint rotation) {
    valueType  text[ MAX_MAP_ROTATIONS * 2 ] = { 0 };
    sint   *p = GetCurrentMapArray();
    sint   i;

    p[ rotation ] = currentMap;

    for(i = 0; i < mapRotations.numRotations; i++) {
        Q_strcat(text, sizeof(text), va("%d ", p[ i ]));
    }

    trap_Cvar_Set("g_currentMap", text);
    trap_Cvar_Update(&g_currentMap);
}

/*
===============
G_GetCurrentMap

Return the current map in some rotation
===============
*/
sint idSGameMaprotation::GetCurrentMap(sint rotation) {
    sint *p = GetCurrentMapArray();

    return p[ rotation ];
}

/*
===============
idSGameMaprotation::IssueMapChange

Send commands to the server to actually change the map
===============
*/
void idSGameMaprotation::IssueMapChange(sint rotation) {
    sint i;
    sint map = GetCurrentMap(rotation);
    valueType cmd[ MAX_TOKEN_CHARS ];

    // allow a manually defined g_layouts setting to override the maprotation
    if(!g_layouts.string[ 0 ] &&
            mapRotations.rotations[ rotation ].maps[ map ].layouts[ 0 ]) {
        trap_Cvar_Set("g_layouts",
                      mapRotations.rotations[ rotation ].maps[ map ].layouts);
    }

    trap_SendConsoleCommand(EXEC_APPEND, va("map %s\n",
                                            mapRotations.rotations[ rotation ].maps[ map ].name));

    // load up map defaults if g_mapConfigs is set
    idSGameMain::MapConfigs(
        mapRotations.rotations[ rotation ].maps[ map ].name);

    for(i = 0; i < mapRotations.rotations[ rotation ].maps[ map ].numCmds;
            i++) {
        Q_strncpyz(cmd,
                   mapRotations.rotations[ rotation ].maps[ map ].postCmds[ i ], sizeof(cmd));
        Q_strcat(cmd, sizeof(cmd), "\n");
        trap_SendConsoleCommand(EXEC_APPEND, cmd);
    }
}

/*
===============
idSGameMaprotation::ResolveConditionDestination

Resolve the destination of some condition
===============
*/
mapConditionType_t idSGameMaprotation::ResolveConditionDestination(sint *n,
        valueType *name) {
    sint i;

    //search the current rotation first...
    for(i = 0;
            i < mapRotations.rotations[ g_currentMapRotation.integer ].numMaps; i++) {
        if(!Q_stricmp(
                    mapRotations.rotations[ g_currentMapRotation.integer ].maps[ i ].name,
                    name)) {
            *n = i;
            return MCT_MAP;
        }
    }

    //...then search the rotation names
    for(i = 0; i < mapRotations.numRotations; i++) {
        if(!Q_stricmp(mapRotations.rotations[ i ].name, name)) {
            *n = i;
            return MCT_ROTATION;
        }
    }

    return MCT_ERR;
}

/*
===============
G_EvaluateMapCondition

Evaluate a map condition
===============
*/
bool idSGameMaprotation::EvaluateMapCondition(mapRotationCondition_t
        *mrc) {
    switch(mrc->lhs) {
        case MCV_RANDOM:
            return rand() & 1;
            break;

        case MCV_NUMCLIENTS:
            switch(mrc->op) {
                case MCO_LT:
                    return level.numConnectedClients < mrc->numClients;
                    break;

                case MCO_GT:
                    return level.numConnectedClients > mrc->numClients;
                    break;

                case MCO_EQ:
                    return level.numConnectedClients == mrc->numClients;
                    break;
            }

            break;

        case MCV_LASTWIN:
            return level.lastWin == mrc->lastWin;
            break;

        default:
        case MCV_ERR:
            idSGameMain::Printf(S_COLOR_RED "ERROR: malformed map switch condition\n");
            break;
    }

    return false;
}

/*
===============
idSGameMaprotation::AdvanceMapRotation

Increment the current map rotation
===============
*/
void idSGameMaprotation::AdvanceMapRotation(void) {
    mapRotation_t *mr;
    mapRotationEntry_t *mre;
    mapRotationCondition_t *mrc;
    sint currentRotation, currentMap, nextMap, i, n;
    mapConditionType_t mct;

    if((currentRotation = g_currentMapRotation.integer) == NOT_ROTATING) {
        return;
    }

    currentMap = GetCurrentMap(currentRotation);

    mr = &mapRotations.rotations[ currentRotation ];
    mre = &mr->maps[ currentMap ];
    nextMap = (currentMap + 1) % mr->numMaps;

    for(i = 0; i < mre->numConditions; i++) {
        mrc = &mre->conditions[ i ];

        if(mrc->unconditional || EvaluateMapCondition(mrc)) {
            mct = ResolveConditionDestination(&n, mrc->dest);

            switch(mct) {
                case MCT_MAP:
                    nextMap = n;
                    break;

                case MCT_ROTATION:
                    //need to increment the current map before changing the rotation
                    //or you get infinite loops with some conditionals
                    SetCurrentMap(nextMap, currentRotation);
                    StartMapRotation(mrc->dest, true);
                    return;

                default:
                case MCT_ERR:
                    idSGameMain::Printf(S_COLOR_YELLOW
                                        "WARNING: map switch destination could not be resolved: %s\n", mrc->dest);
                    break;
            }
        }
    }

    SetCurrentMap(nextMap, currentRotation);
    IssueMapChange(currentRotation);
}

/*
===============
idSGameMaprotation::StartMapRotation

Switch to a new map rotation
===============
*/
bool idSGameMaprotation::StartMapRotation(valueType *name,
        bool changeMap) {
    sint i;

    for(i = 0; i < mapRotations.numRotations; i++) {
        if(!Q_stricmp(mapRotations.rotations[ i ].name, name)) {
            trap_Cvar_Set("g_currentMapRotation", va("%d", i));
            trap_Cvar_Update(&g_currentMapRotation);

            if(changeMap) {
                IssueMapChange(i);
            }

            break;
        }
    }

    if(i == mapRotations.numRotations) {
        return false;
    } else {
        return true;
    }
}

/*
===============
idSGameMaprotation::StopMapRotation

Stop the current map rotation
===============
*/
void idSGameMaprotation::StopMapRotation(void) {
    trap_Cvar_Set("g_currentMapRotation", va("%d", NOT_ROTATING));
    trap_Cvar_Update(&g_currentMapRotation);
}

/*
===============
idSGameMaprotation::MapRotationActive

Test if any map rotation is currently active
===============
*/
bool idSGameMaprotation::MapRotationActive(void) {
    return (g_currentMapRotation.integer != NOT_ROTATING);
}

/*
===============
idSGameMaprotation::InitMapRotations

Load and intialise the map rotations
===============
*/
void idSGameMaprotation::InitMapRotations(void) {
    pointer fileName = "maprotation.cfg";

    //load the file if it exists
    if(trap_FS_FOpenFile(fileName, nullptr, FS_READ)) {
        if(!ParseMapRotationFile(fileName)) {
            idSGameMain::Printf(S_COLOR_RED "ERROR: failed to parse %s file\n",
                                fileName);
        }
    } else {
        idSGameMain::Printf("%s file not found.\n", fileName);
    }

    if(g_currentMapRotation.integer == NOT_ROTATING) {
        if(g_initialMapRotation.string[ 0 ] != 0) {
            StartMapRotation(g_initialMapRotation.string, false);

            trap_Cvar_Set("g_initialMapRotation", "");
            trap_Cvar_Update(&g_initialMapRotation);
        }
    }
}
