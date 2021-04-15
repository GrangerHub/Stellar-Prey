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
// File name:   gui_local.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GUI_LOCAL_H__
#define __GUI_LOCAL_H__

void UI_Report(void);
void UI_Load(void);
void UI_LoadMenus(pointer menuFile, bool reset);
sint UI_AdjustTimeByGame(sint time);
void UI_ClearScores(void);
void UI_LoadArenas(void);
void UI_ServerInfo(void);

void UI_RegisterCvars(void);
void UI_UpdateCvars(void);
void UI_DrawConnectScreen(bool overlay);

// new ui stuff
#define MAX_MAPS 128
//#define MAX_PINGREQUESTS 32
#define MAX_ADDRESSLENGTH 64
#define MAX_DISPLAY_SERVERS 2048
#define MAX_SERVERSTATUS_LINES 128
#define MAX_SERVERSTATUS_TEXT 1024
#define MAX_FOUNDPLAYER_SERVERS 16
#define MAX_MODS 64
#define MAX_DEMOS 256
#define MAX_MOVIES 256
#define MAX_HELP_INFOPANES 32
#define MAX_RESOLUTIONS 32
#define MAX_PROFILES 64

typedef struct {
    pointer mapName;
    pointer mapLoadName;
    pointer imageName;
    sint cinematic;
    qhandle_t levelShot;
}
mapInfo;

typedef struct {
    pointer name;
    pointer dir;
} profileInfo_t;

typedef struct serverFilter_s {
    pointer description;
    pointer basedir;
}
serverFilter_t;

typedef struct {
    valueType  adrstr[MAX_ADDRESSLENGTH];
    sint    start;
}
pinglist_t;


typedef struct guiServerStatus_s {
    pinglist_t pingList[MAX_PINGREQUESTS];
    sint    numqueriedservers;
    sint    currentping;
    sint    nextpingtime;
    sint    maxservers;
    sint    refreshtime;
    sint    numServers;
    sint    sortKey;
    sint    sortDir;
    bool sorted;
    sint    lastCount;
    bool refreshActive;
    sint    currentServer;
    sint    displayServers[MAX_DISPLAY_SERVERS];
    sint    numDisplayServers;
    sint    numPlayersOnServers;
    sint    nextDisplayRefresh;
    sint    nextSortTime;
    qhandle_t currentServerPreview;
    sint    currentServerCinematic;
    sint    motdLen;
    sint    motdWidth;
    sint    motdPaintX;
    sint    motdPaintX2;
    sint    motdOffset;
    sint    motdTime;
    valueType  motd[MAX_STRING_CHARS];
}
guiServerStatus_t;


typedef struct {
    valueType    adrstr[MAX_ADDRESSLENGTH];
    valueType    name[MAX_ADDRESSLENGTH];
    sint      startTime;
    sint      serverNum;
    bool  valid;
}
pendingServer_t;

typedef struct {
    sint num;
    pendingServer_t server[MAX_SERVERSTATUSREQUESTS];
}
pendingServerStatus_t;

typedef struct {
    valueType address[MAX_ADDRESSLENGTH];
    valueType *lines[MAX_SERVERSTATUS_LINES][4];
    valueType text[MAX_SERVERSTATUS_TEXT];
    valueType pings[MAX_CLIENTS * 3];
    sint numLines;
}
serverStatusInfo_t;

typedef struct {
    pointer modName;
    pointer modDescr;
}
modInfo_t;

typedef enum {
    INFOTYPE_TEXT,
    INFOTYPE_BUILDABLE,
    INFOTYPE_CLASS,
    INFOTYPE_WEAPON,
    INFOTYPE_UPGRADE
} infoType_t;

typedef struct {
    pointer    text;
    pointer    cmd;
    infoType_t    type;
    union {
        pointer  text;
        buildable_t buildable;
        class_t     pclass;
        weapon_t    weapon;
        upgrade_t   upgrade;
    } v;
}
menuItem_t;

typedef struct {
    sint w;
    sint h;
}
resolution_t;

typedef struct {
    displayContextDef_t uiDC;

    sint playerCount;
    sint myTeamCount;
    sint teamPlayerIndex;
    sint playerRefresh;
    sint playerIndex;
    sint playerNumber;
    sint myPlayerIndex;
    sint ignoreIndex;
    valueType playerNames[MAX_CLIENTS][MAX_NAME_LENGTH];
    valueType rawPlayerNames[MAX_CLIENTS][MAX_NAME_LENGTH];
    valueType teamNames[MAX_CLIENTS][MAX_NAME_LENGTH];
    valueType rawTeamNames[MAX_CLIENTS][MAX_NAME_LENGTH];
    sint clientNums[MAX_CLIENTS];
    sint teamClientNums[MAX_CLIENTS];
    clientList_t ignoreList[MAX_CLIENTS];

    sint mapCount;
    mapInfo mapList[MAX_MAPS];

    modInfo_t modList[MAX_MODS];
    sint modCount;
    sint modIndex;

    profileInfo_t profileList[MAX_PROFILES];
    sint profileCount;
    sint profileIndex;

    pointer demoList[MAX_DEMOS];
    sint demoCount;
    sint demoIndex;

    pointer movieList[MAX_MOVIES];
    sint movieCount;
    sint movieIndex;
    sint previewMovie;

    menuItem_t  teamList[ 4 ];
    sint         teamCount;
    sint         teamIndex;

    menuItem_t  alienClassList[ 3 ];
    sint         alienClassCount;
    sint         alienClassIndex;

    menuItem_t  humanItemList[ 3 ];
    sint         humanItemCount;
    sint         humanItemIndex;

    menuItem_t  humanArmouryBuyList[ 32 ];
    sint         humanArmouryBuyCount;
    sint         humanArmouryBuyIndex;

    menuItem_t  humanArmourySellList[ 32 ];
    sint         humanArmourySellCount;
    sint         humanArmourySellIndex;

    menuItem_t  alienUpgradeList[ 16 ];
    sint         alienUpgradeCount;
    sint         alienUpgradeIndex;

    menuItem_t  alienBuildList[ 32 ];
    sint         alienBuildCount;
    sint         alienBuildIndex;

    menuItem_t  humanBuildList[ 32 ];
    sint         humanBuildCount;
    sint         humanBuildIndex;

    menuItem_t  helpList[ MAX_HELP_INFOPANES ];
    sint         helpCount;
    sint         helpIndex;

    sint         weapon;
    sint         upgrades;
    sint         credits;

    guiServerStatus_t serverStatus;

    // for the showing the status of a server
    valueType serverStatusAddress[MAX_ADDRESSLENGTH];
    serverStatusInfo_t serverStatusInfo;
    sint nextServerStatusRefresh;

    // to retrieve the status of server to find a player
    pendingServerStatus_t pendingServerStatus;
    valueType findPlayerName[MAX_STRING_CHARS];
    valueType foundPlayerServerAddresses[MAX_FOUNDPLAYER_SERVERS][MAX_ADDRESSLENGTH];
    valueType foundPlayerServerNames[MAX_FOUNDPLAYER_SERVERS][MAX_ADDRESSLENGTH];
    sint currentFoundPlayerServer;
    sint numFoundPlayerServers;
    sint nextFindPlayerRefresh;

    resolution_t  resolutions[ MAX_RESOLUTIONS ];
    sint numResolutions;
    bool oldResolutions;

    bool inGameLoad;

    bool  chatTeam;
    bool  chatAdmins;
    bool  chatClan;
    bool  chatPrompt;
    valueType      chatPromptCallback[ MAX_STRING_CHARS ];
    sint       chatTargetClientNum;
}
uiInfo_t;

extern uiInfo_t uiInfo;

extern vmConvar_t cl_profile;
extern vmConvar_t cl_defaultProfile;

valueType      *UI_Cvar_VariableString(pointer var_name);
void      UI_SetColor(const float32 *rgba);
void      UI_AdjustFrom640(float32 *x, float32 *y, float32 *w, float32 *h);
void      UI_DrawHandlePic(float32 x, float32 y, float32 w, float32 h,
                           qhandle_t hShader);

//
// idUserInterfaceManagerLocal
//
class idUserInterfaceManagerLocal : public idUserInterfaceManager {
public:
    virtual void Init(bool inGameLoad);
    virtual void Shutdown(void);

    virtual void KeyEvent(sint key, bool down);
    virtual void MouseEvent(sint dx, sint dy);
    virtual void Refresh(sint time);

    virtual bool IsFullscreen(void);

    virtual sint MousePosition(void);
    virtual void SetMousePosition(sint x, sint y);

    virtual void SetActiveMenu(uiMenuCommand_t menu);

    virtual uiMenuCommand_t GetActiveMenu(void);
    virtual bool ConsoleCommand(sint realTime);

    // if !overlay, the background will be drawn, otherwise it will be
    // overlayed over whatever the cgame has drawn.
    // a GetClientState syscall will be made to get the current strings
    virtual void DrawConnectScreen(bool overlay);
    virtual bool CheckExecKey(sint key);
    virtual bool WantsBindKeys(void);
};

extern idUserInterfaceManagerLocal uiManagerLocal;

#endif //!__GUI_LOCAL_H__

