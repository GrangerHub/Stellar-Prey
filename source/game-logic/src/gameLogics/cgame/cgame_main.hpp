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
// File name:   cgame_main.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CGAME_MAIN_H__
#define __CGAME_MAIN_H__

class idCGameLocal;

class idCGameMain : public idCGameLocal {
public:
    idCGameMain();
    ~idCGameMain();

    static void RegisterCvars(void);
    static void SetUIVars(void);
    static void SetPVars(void);
    static void UpdateCvars(void);
    static void RemoveNotifyLine(void);
    static void AddNotifyText(void);
    static pointer Argv(sint arg);
    static bool FileExists(valueType *filename);
    static void RegisterSounds(void);
    static void RegisterGraphics(void);
    static void BuildSpectatorString(void);
    static void RegisterClients(void);
    static pointer ConfigString(sint index);
    static void StartMusic(void);
    static sint PlayerCount(void);
    static valueType *GetMenuBuffer(pointer filename);
    static bool Asset_Parse(sint handle);
    static void ParseMenu(pointer menuFile);
    static bool Load_Menu(valueType **p);
    static void LoadMenus(pointer menuFile);
    static bool OwnerDrawHandleKey(sint ownerDraw, sint flags,
                                   float32 *special, sint key);
    static sint FeederCount(float32 feederID);
    static void SetScoreSelection(void *p);
    static clientInfo_t *InfoFromScoreIndex(sint index, sint team,
                                            sint *scoreIndex);
    static bool ClientIsReady(sint clientNum);
    static pointer FeederItemText(float32 feederID, sint index, sint column,
                                  qhandle_t *handle);
    static qhandle_t FeederItemImage(float32 feederID, sint index);
    static void FeederSelection(float32 feederID, sint index);
    static float32 Cvar_Get(pointer cvar);
    static void Text_PaintWithCursor(float32 x, float32 y, float32 scale,
                                     vec4_t color, pointer text, sint cursorPos, valueType cursor, sint limit,
                                     sint style);
    static sint OwnerDrawWidth(sint ownerDraw, float32 scale);
    static sint PlayCinematic(pointer name, float32 x, float32 y, float32 w,
                              float32 h);
    static void StopCinematic(sint handle);
    static void DrawCinematic(sint handle, float32 x, float32 y, float32 w,
                              float32 h);
    static void RunCinematicFrame(sint handle);
    static bool OwnerDrawVisible(sint parameter);
    static void LoadHudMenu(void);
    static void AssetCache(void);
};

#endif //!__CGAME_MAIN_H__
