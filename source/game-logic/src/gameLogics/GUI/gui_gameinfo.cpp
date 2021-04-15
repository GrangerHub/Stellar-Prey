////////////////////////////////////////////////////////////////////////////////////////
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
// File name:   gui_gameinfo.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <GUI/gui_precompiled.hpp>

// arena and bot info
sint ui_numBots;
static valueType *ui_botInfos[MAX_BOTS];

static sint ui_numArenas;
static valueType *ui_arenaInfos[MAX_ARENAS];

/*
===============
UI_ParseInfos
===============
*/
sint UI_ParseInfos(valueType *buf, sint max, valueType *infos[]) {
    valueType   *token;
    sint   count;
    valueType  key[MAX_TOKEN_CHARS];
    valueType  info[MAX_INFO_STRING];

    count = 0;

    while(1) {
        token = COM_Parse(&buf);

        if(!token[0]) {
            break;
        }

        if(strcmp(token, "{")) {
            Com_Printf("Missing { in info file\n");
            break;
        }

        if(count == max) {
            Com_Printf("Max infos exceeded\n");
            break;
        }

        info[0] = '\0';

        while(1) {
            token = COM_ParseExt(&buf, true);

            if(!token[0]) {
                Com_Printf("Unexpected end of info file\n");
                break;
            }

            if(!strcmp(token, "}")) {
                break;
            }

            Q_strncpyz(key, token, sizeof(key));

            token = COM_ParseExt(&buf, false);

            if(!token[0]) {
                strcpy(token, "<nullptr>");
            }

            Info_SetValueForKey(info, key, token);
        }

        //NOTE: extra space for arena number
        infos[count] = (valueType *)UI_Alloc(strlen(info) + strlen("\\num\\") +
                                             strlen(va("%d", MAX_ARENAS)) + 1);

        if(infos[count]) {
            strcpy(infos[count], info);
            count++;
        }
    }

    return count;
}

/*
===============
UI_LoadArenasFromFile
===============
*/
static void UI_LoadArenasFromFile(valueType *filename) {
    sint       len;
    fileHandle_t  f;
    valueType      buf[MAX_ARENAS_TEXT];

    len = trap_FS_FOpenFile(filename, &f, FS_READ);

    if(!f) {
        trap_Print(va(S_COLOR_RED "file not found: %s\n", filename));
        return;
    }

    if(len >= MAX_ARENAS_TEXT) {
        trap_Print(va(S_COLOR_RED "file too large: %s is %i, max allowed is %i",
                      filename, len, MAX_ARENAS_TEXT));
        trap_FS_FCloseFile(f);
        return;
    }

    trap_FS_Read(buf, len, f);
    buf[len] = 0;
    trap_FS_FCloseFile(f);

    ui_numArenas += UI_ParseInfos(buf, MAX_ARENAS - ui_numArenas,
                                  &ui_arenaInfos[ui_numArenas]);
}

/*
=================
UI_MapNameCompare
=================
*/
static sint UI_MapNameCompare(const void *a, const void *b) {
    mapInfo *A = (mapInfo *)a;
    mapInfo *B = (mapInfo *)b;

    return Q_stricmp(A->mapName, B->mapName);
}

/*
===============
UI_LoadArenas
===============
*/
void UI_LoadArenas(void) {
    sint     numdirs;
    valueType    filename[128];
    valueType    dirlist[1024];
    valueType   *dirptr;
    sint     i, n;
    sint     dirlen;
    valueType    *type;

    ui_numArenas = 0;
    uiInfo.mapCount = 0;

    // get all arenas from .arena files
    numdirs = trap_FS_GetFileList("scripts", ".arena", dirlist, 1024);
    dirptr  = dirlist;

    for(i = 0; i < numdirs; i++, dirptr += dirlen + 1) {
        dirlen = strlen(dirptr);
        strcpy(filename, "scripts/");
        strcat(filename, dirptr);
        UI_LoadArenasFromFile(filename);
    }

    trap_Print(va("[skipnotify]%i arenas parsed\n", ui_numArenas));

    if(UI_OutOfMemory()) {
        trap_Print(
            S_COLOR_YELLOW"WARNING: not anough memory in pool to load all arenas\n");
    }

    for(n = 0; n < ui_numArenas; n++) {
        // determine type
        type = Info_ValueForKey(ui_arenaInfos[ n ], "type");
        // if no type specified, it will be treated as "ffa"

        uiInfo.mapList[uiInfo.mapCount].cinematic = -1;
        uiInfo.mapList[uiInfo.mapCount].mapLoadName = String_Alloc(
                    Info_ValueForKey(ui_arenaInfos[n], "map"));
        uiInfo.mapList[uiInfo.mapCount].mapName = String_Alloc(Info_ValueForKey(
                    ui_arenaInfos[n], "longname"));
        uiInfo.mapList[uiInfo.mapCount].levelShot = -1;
        uiInfo.mapList[uiInfo.mapCount].imageName = String_Alloc(
                    va("levelshots/%s", uiInfo.mapList[uiInfo.mapCount].mapLoadName));

        uiInfo.mapCount++;

        if(uiInfo.mapCount >= MAX_MAPS) {
            break;
        }
    }

    qsort(uiInfo.mapList, uiInfo.mapCount, sizeof(mapInfo), UI_MapNameCompare);
}


/*
===============
UI_LoadBotsFromFile
===============
*/
static void UI_LoadBotsFromFile(valueType *filename) {
    sint       len;
    fileHandle_t  f;
    valueType      buf[MAX_BOTS_TEXT];

    len = trap_FS_FOpenFile(filename, &f, FS_READ);

    if(!f) {
        trap_Print(va(S_COLOR_RED "file not found: %s\n", filename));
        return;
    }

    if(len >= MAX_BOTS_TEXT) {
        trap_Print(va(S_COLOR_RED "file too large: %s is %i, max allowed is %i",
                      filename, len, MAX_BOTS_TEXT));
        trap_FS_FCloseFile(f);
        return;
    }

    trap_FS_Read(buf, len, f);
    buf[len] = 0;
    trap_FS_FCloseFile(f);

    COM_Compress(buf);

    ui_numBots += UI_ParseInfos(buf, MAX_BOTS - ui_numBots,
                                &ui_botInfos[ui_numBots]);
}

/*
===============
UI_LoadBots
===============
*/
void UI_LoadBots(void) {
    vmConvar_t  botsFile;
    sint     numdirs;
    valueType    filename[128];
    valueType    dirlist[1024];
    valueType   *dirptr;
    sint     i;
    sint     dirlen;

    ui_numBots = 0;

    trap_Cvar_Register(&botsFile, "g_botsFile", "", CVAR_INIT | CVAR_ROM,
                       "description");

    if(*botsFile.string) {
        UI_LoadBotsFromFile(botsFile.string);
    } else {
        UI_LoadBotsFromFile("scripts/bots.txt");
    }

    // get all bots from .bot files
    numdirs = trap_FS_GetFileList("scripts", ".bot", dirlist, 1024);

    dirptr  = dirlist;

    for(i = 0; i < numdirs; i++, dirptr += dirlen + 1) {
        dirlen = strlen(dirptr);
        strcpy(filename, "scripts/");
        strcat(filename, dirptr);
        UI_LoadBotsFromFile(filename);
    }

    trap_Print(va("%i bots parsed\n", ui_numBots));
}


/*
===============
UI_GetBotInfoByNumber
===============
*/
valueType *UI_GetBotInfoByNumber(sint num) {
    if(num < 0 || num >= ui_numBots) {
        trap_Print(va(S_COLOR_RED "Invalid bot number: %i\n", num));
        return nullptr;
    }

    return ui_botInfos[num];
}


/*
===============
UI_GetBotInfoByName
===============
*/
valueType *UI_GetBotInfoByName(pointer name) {
    sint   n;
    valueType  *value;

    for(n = 0; n < ui_numBots ; n++) {
        value = Info_ValueForKey(ui_botInfos[n], "name");

        if(!Q_stricmp(value, name)) {
            return ui_botInfos[n];
        }
    }

    return nullptr;
}

sint UI_GetNumBots(void) {
    return ui_numBots;
}


valueType *UI_GetBotNameByNumber(sint num) {
    valueType *info = UI_GetBotInfoByNumber(num);

    if(info) {
        return Info_ValueForKey(info, "name");
    }

    return "";
}

void UI_ServerInfo(void) {
    valueType      info[ MAX_INFO_VALUE ];

    info[0] = '\0';

    if(trap_GetConfigString(CS_SERVERINFO, info, sizeof(info))) {
        trap_Cvar_Set("ui_serverinfo_mapname",
                      Info_ValueForKey(info, "mapname"));
        trap_Cvar_Set("ui_serverinfo_timelimit",
                      Info_ValueForKey(info, "timelimit"));
        trap_Cvar_Set("ui_serverinfo_sd",
                      Info_ValueForKey(info, "g_suddenDeathTime"));
        trap_Cvar_Set("ui_serverinfo_hostname",
                      Info_ValueForKey(info, "sv_hostname"));
        trap_Cvar_Set("ui_serverinfo_maxclients",
                      Info_ValueForKey(info, "sv_maxclients"));
        trap_Cvar_Set("ui_serverinfo_version",
                      Info_ValueForKey(info, "version"));
        trap_Cvar_Set("ui_serverinfo_unlagged",
                      Info_ValueForKey(info, "g_unlagged"));
        trap_Cvar_Set("ui_serverinfo_ff",
                      Info_ValueForKey(info, "ff"));
    }
}
