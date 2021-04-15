////////////////////////////////////////////////////////////////////////////////////////
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
// File name:   gui_api.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <GUI/gui_precompiled.hpp>

#define PASSFLOAT(x) x

guiImports_t *imports;
idSoundSystem *soundSystem;
idRenderSystem *renderSystem;
idFileSystem *fileSystem;
idCVarSystem *cvarSystem;
idCmdBufferSystem *cmdBufferSystem;
idCmdSystem *cmdSystem;
idSystem *idsystem;
idCGame *idCgame;
idClientLANSystem *idLANSystem;
idClientGUISystem *idGUISystem;
idClientScreenSystem *idScreenSystem;

#ifdef __LINUX__
extern "C" idUserInterfaceManager *guiEntry(guiImports_t *guiimports)
#else
Q_EXPORT idUserInterfaceManager *guiEntry(guiImports_t *guiimports)
#endif
{
    imports = guiimports;

    soundSystem = imports->soundSystem;
    renderSystem = imports->renderSystem;
    fileSystem = imports->fileSystem;
    cvarSystem = imports->cvarSystem;
    cmdBufferSystem = imports->cmdBufferSystem;
    cmdSystem = imports->cmdSystem;
    idsystem = imports->idsystem;
    idCgame = imports->idcgame;
    idLANSystem = imports->idLANSystem;
    idGUISystem = imports->idGUISystem;
    idScreenSystem = imports->clientScreenSystem;

    return uiManager;
}

void trap_Error(pointer string) {
    imports->Error(ERR_DROP, string);
}

void trap_Print(pointer string) {
    imports->Print(string);
}

sint trap_Milliseconds(void) {
    return idsystem->Milliseconds();
}

void trap_Cvar_Register(vmConvar_t *cvar, pointer var_name, pointer value,
                        sint flags, pointer description) {
    cvarSystem->Register(cvar, var_name, value, flags, description);
}

void trap_Cvar_Update(vmConvar_t *cvar) {
    cvarSystem->Update(cvar);
}

void trap_Cvar_Set(pointer var_name, pointer value) {
    cvarSystem->Set(var_name, value);
}

float32 trap_Cvar_VariableValue(pointer var_name) {
    return cvarSystem->VariableValue(var_name);;
}

void trap_Cvar_VariableStringBuffer(pointer var_name, valueType *buffer,
                                    uint64 bufsize) {
    cvarSystem->VariableStringBuffer(var_name, buffer, bufsize);
}

void trap_Cvar_LatchedVariableStringBuffer(pointer var_name,
        valueType *buffer, uint64 bufsize) {
    cvarSystem->VariableStringBuffer(var_name, buffer, bufsize);
}

void trap_Cvar_SetValue(pointer var_name, float32 value) {
    cvarSystem->SetValue(var_name, PASSFLOAT(value));
}

void trap_Cvar_Reset(pointer name) {
    cvarSystem->Reset(name);
}

void trap_Cvar_Create(pointer var_name, pointer var_value, sint flags,
                      pointer description) {
    cvarSystem->Get(var_name, var_value, flags, description);
}

void trap_Cvar_InfoStringBuffer(sint bit, valueType *buffer,
                                sint bufsize) {
    cvarSystem->InfoStringBuffer(bit, buffer, bufsize);
}

sint trap_Argc(void) {
    return cmdSystem->Argc();
}

void trap_Argv(sint n, valueType *buffer, sint bufferLength) {
    cmdSystem->ArgvBuffer(n, buffer, bufferLength);
}

void trap_Cmd_ExecuteText(sint exec_when, pointer text) {
    cmdBufferSystem->ExecuteText(exec_when, text);
}

void trap_AddCommand(pointer cmdName, pointer cmdDesc) {
    cmdSystem->AddCommand(cmdName, nullptr, cmdDesc);
}

sint trap_FS_FOpenFile(pointer qpath, fileHandle_t *f, fsMode_t mode) {
    return fileSystem->FOpenFileByMode(qpath, f, mode);
}

void trap_FS_Read(void *buffer, sint len, fileHandle_t f) {
    fileSystem->Read(buffer, len, f);
}

sint trap_FS_Write(const void *buffer, sint len, fileHandle_t f) {
    return fileSystem->Write(buffer, len, f);
}

void trap_FS_FCloseFile(fileHandle_t f) {
    fileSystem->FCloseFile(f);
}

sint trap_FS_Delete(valueType *filename) {
    return fileSystem->Delete(filename);
}

sint trap_FS_GetFileList(pointer path, pointer extension,
                         valueType *listbuf, sint bufsize) {
    return fileSystem->GetFileList(path, extension, listbuf, bufsize);
}

sint trap_FS_Seek(fileHandle_t f, sint32 offset, sint origin) {
    return fileSystem->Seek(f, offset, origin);
}

qhandle_t trap_R_RegisterModel(pointer name) {
    return renderSystem->RegisterModel(name);
}

qhandle_t trap_R_RegisterSkin(pointer name) {
    return renderSystem->RegisterSkin(name);
}

qhandle_t trap_R_RegisterShaderNoMip(pointer name) {
    return renderSystem->RegisterShaderNoMip(name);
}

void trap_R_ClearScene(void) {
    renderSystem->ClearScene();
}

void trap_R_AddRefEntityToScene(const refEntity_t *re) {
    renderSystem->AddRefEntityToScene(re);
}

void trap_R_AddPolyToScene(qhandle_t hShader, sint numVerts,
                           const polyVert_t *verts) {
    renderSystem->AddPolyToScene(hShader, numVerts, verts, 1);
}

void trap_R_AddPolysToScene(qhandle_t hShader, sint numVerts,
                            const polyVert_t *verts, sint numPolys) {
    renderSystem->AddPolyToScene(hShader, numVerts, verts, numPolys);
}

void trap_R_AddLightToScene(const vec3_t org, float32 intensity, float32 r,
                            float32 g, float32 b) {
    renderSystem->AddLightToScene(org, PASSFLOAT(intensity), PASSFLOAT(r),
                                  PASSFLOAT(g), PASSFLOAT(b));
}

void trap_R_AddCoronaToScene(const vec3_t org, float32 r, float32 g,
                             float32 b, float32 scale, sint id, bool visible) {
    //renderSystem->AddCoronaToScene( org, PASSFLOAT( r ), PASSFLOAT( g ), PASSFLOAT( b ), PASSFLOAT( scale ), id, visible );
}
;
void trap_R_RenderScene(const refdef_t *fd) {
    renderSystem->RenderScene(fd);
}

void trap_R_SetColor(const float32 *rgba) {
    renderSystem->SetColor(rgba);
}

void trap_R_SetClipRegion(const float32 *region) {
    renderSystem->SetClipRegion(region);
}

void trap_R_Add2dPolys(polyVert_t *verts, sint numverts,
                       qhandle_t hShader) {
    //renderSystem->Add2dPolys( verts, numverts, hShader );
}

void trap_R_DrawStretchPic(float32 x, float32 y, float32 w, float32 h,
                           float32 s1, float32 t1, float32 s2, float32 t2, qhandle_t hShader) {
    renderSystem->DrawStretchPic(PASSFLOAT(x), PASSFLOAT(y), PASSFLOAT(w),
                                 PASSFLOAT(h), PASSFLOAT(s1), PASSFLOAT(t1), PASSFLOAT(s2), PASSFLOAT(t2),
                                 hShader);
}

void trap_R_DrawRotatedPic(float32 x, float32 y, float32 w, float32 h,
                           float32 s1, float32 t1, float32 s2, float32 t2, qhandle_t hShader,
                           float32 angle) {
    //renderSystem->DrawRotatedPic( PASSFLOAT( x ), PASSFLOAT( y ), PASSFLOAT( w ), PASSFLOAT( h ), PASSFLOAT( s1 ), PASSFLOAT( t1 ), PASSFLOAT( s2 ), PASSFLOAT( t2 ), hShader, PASSFLOAT( angle ) );
}

void trap_R_ModelBounds(clipHandle_t model, vec3_t mins, vec3_t maxs) {
    renderSystem->ModelBounds(model, mins, maxs);
}

void trap_UpdateScreen(void) {
    imports->clientScreenSystem->UpdateScreen();
}

sint trap_CM_LerpTag(orientation_t *tag, clipHandle_t mod, sint startFrame,
                     sint endFrame, float32 frac, pointer tagName) {
    return renderSystem->LerpTag(tag, mod, startFrame, endFrame,
                                 PASSFLOAT(frac), tagName);
}

sfxHandle_t trap_S_RegisterSound(pointer sample) {
    sint i = soundSystem->RegisterSound(sample, false);
#ifdef _DEBUG

    if(i == 0) {
        Com_Printf("^1Warning: Failed to load sound: %s\n", sample);
    }

#endif
    return i;
}

void trap_S_StartLocalSound(sfxHandle_t sfx, sint channelNum) {
    soundSystem->StartLocalSound(sfx, channelNum);
}

void trap_Key_KeynumToStringBuf(sint keynum, valueType *buf, sint buflen) {
    imports->idGUISystem->KeynumToStringBuf(keynum, buf, buflen);
}

void trap_Key_GetBindingBuf(sint keynum, valueType *buf, sint buflen) {
    imports->idGUISystem->GetBindingBuf(keynum, buf, buflen);
}

void trap_Key_SetBinding(sint keynum, pointer binding) {
    imports->SetBinding(keynum, binding);
}

void trap_Key_KeysForBinding(pointer binding, sint *key1, sint *key2) {
    imports->GetBindingByString(binding, key1, key2);
}

bool trap_Key_IsDown(sint keynum) {
    return imports->IsDown(keynum);
}

bool trap_Key_GetOverstrikeMode(void) {
    return imports->GetOverstrikeMode();
}

void trap_Key_SetOverstrikeMode(bool state) {
    imports->SetOverstrikeMode(state);
}

void trap_Key_ClearStates(void) {
    imports->ClearStates();
}

sint trap_Key_GetCatcher(void) {
    return imports->idGUISystem->GetCatcher();
}

void trap_Key_SetCatcher(sint catcher) {
    imports->idGUISystem->SetCatcher(catcher);
}

void trap_GetClipboardData(valueType *buf, sint bufsize) {
    imports->idGUISystem->GUIGetClipboardData(buf, bufsize);
}

void trap_GetClientState(uiClientState_t *state) {
    imports->idGUISystem->GetClientState(state);
}

void trap_GetGlconfig(vidconfig_t *glconfig) {
    imports->idGUISystem->GetGlconfig(glconfig);
}

sint trap_GetConfigString(sint index, valueType *buff, uint64 buffsize) {
    return imports->idGUISystem->GetConfigString(index, buff, buffsize);
}

void trap_LAN_LoadCachedServers(void) {
    imports->idLANSystem->LoadCachedServers();
}

void trap_LAN_SaveCachedServers(void) {
    imports->idLANSystem->SaveServersToCache();
}

sint trap_LAN_AddServer(sint source, pointer name, pointer addr) {
    return imports->idLANSystem->AddServer(source, name, addr);
}

void trap_LAN_RemoveServer(sint source, pointer addr) {
    imports->idLANSystem->RemoveServer(source, addr);
}


sint trap_LAN_GetPingQueueCount(void) {
    return imports->idLANSystem->GetPingQueueCount();
}

void trap_LAN_ClearPing(sint n) {
    imports->idLANSystem->ClearPing(n);
}

void trap_LAN_GetPing(sint n, valueType *buf, sint buflen,
                      sint *pingtime) {
    imports->idLANSystem->GetPing(n, buf, buflen, pingtime);
}

void trap_LAN_GetPingInfo(sint n, valueType *buf, sint buflen) {
    imports->idLANSystem->GetPingInfo(n, buf, buflen);
}

sint trap_LAN_GetServerCount(sint source) {
    return imports->idLANSystem->GetServerCount(source);
}

void trap_LAN_GetServerAddressString(sint source, sint n, valueType *buf,
                                     sint buflen) {
    imports->idLANSystem->GetServerAddressString(source, n, buf, buflen);
}

void trap_LAN_GetServerInfo(sint source, sint n, valueType *buf,
                            sint buflen) {
    imports->idLANSystem->GetServerInfo(source, n, buf, buflen);
}

sint trap_LAN_GetServerPing(sint source, sint n) {
    return imports->idLANSystem->GetServerPing(source, n);
}

void trap_LAN_MarkServerVisible(sint source, sint n, bool visible) {
    imports->idLANSystem->MarkServerVisible(source, n, visible);
}

sint trap_LAN_ServerIsVisible(sint source, sint n) {
    return imports->idLANSystem->ServerIsVisible(source, n);
}

bool trap_LAN_UpdateVisiblePings(sint source) {
    return imports->idLANSystem->UpdateVisiblePings(source);
}

void trap_LAN_ResetPings(sint n) {
    imports->idLANSystem->ResetPings(n);
}

sint trap_LAN_ServerStatus(valueType *serverAddress,
                           valueType *serverStatus, sint maxLen) {
    return imports->idLANSystem->GetServerStatus(serverAddress, serverStatus,
            maxLen);
}

bool trap_LAN_ServerIsInFavoriteList(sint source, sint n) {
    return imports->idLANSystem->ServerIsInFavoriteList(source, n);
}

bool trap_GetNews(bool force) {
    return imports->idGUISystem->GetNews(force);
}

sint trap_LAN_CompareServers(sint source, sint sortKey, sint sortDir,
                             sint s1, sint s2) {
    return imports->idLANSystem->CompareServers(source, sortKey, sortDir, s1,
            s2);
}

sint trap_MemoryRemaining(void) {
    return imports->Hunk_MemoryRemaining();
}

void trap_R_RegisterFont(pointer fontName, sint pointSize,
                         fontInfo_t *font) {
    renderSystem->RegisterFont(fontName, pointSize, font);
}

sint trap_PC_LoadSource(pointer filename) {
    return imports->parseSystem->LoadSourceHandle(filename);
}

sint trap_PC_FreeSource(sint handle) {
    return imports->parseSystem->FreeSourceHandle(handle);
}

sint trap_PC_ReadToken(sint handle, pc_token_t *pc_token) {
    return imports->parseSystem->ReadTokenHandle(handle, pc_token);
}

sint trap_PC_SourceFileAndLine(sint handle, valueType *filename,
                               sint *line) {
    return imports->parseSystem->SourceFileAndLine(handle, filename, line);
}

void trap_S_StopBackgroundTrack(void) {
    soundSystem->StopBackgroundTrack();
}

void trap_S_StartBackgroundTrack(pointer intro, pointer loop) {
    soundSystem->StartBackgroundTrack(intro, loop);
}

sint trap_RealTime(qtime_t *qtime) {
    return imports->RealTime(qtime);
}

sint trap_CIN_PlayCinematic(pointer arg0, sint xpos, sint ypos, sint width,
                            sint height, sint bits) {
    return imports->PlayCinematic(arg0, xpos, ypos, width, height, bits);
}

e_status trap_CIN_StopCinematic(sint handle) {
    return imports->StopCinematic(handle);
}

e_status trap_CIN_RunCinematic(sint handle) {
    return imports->RunCinematic(handle);
}

void trap_CIN_DrawCinematic(sint handle) {
    imports->DrawCinematic(handle);
}

void trap_CIN_SetExtents(sint handle, sint x, sint y, sint w, sint h) {
    imports->SetExtents(handle, x, y, w, h);
}

void trap_R_RemapShader(pointer oldShader, pointer newShader,
                        pointer timeOffset) {
    renderSystem->RemapShader(oldShader, newShader, timeOffset);
}

valueType *trap_TranslateString(pointer string) {
    static valueType     staticbuf[2][32000];
    static sint      bufcount = 0;
    valueType *buf;

    buf = staticbuf[bufcount++ % 2];

#ifdef LOCALIZATION_SUPPORT
    imports->TranslateString(string, buf);
#else
    Q_strncpyz(buf, string, MAX_VA_STRING);
#endif                          // LOCALIZATION_SUPPORT
    return buf;
}

void trap_CheckAutoUpdate(void) {
    imports->CheckAutoUpdate();
}

void trap_GetAutoUpdate(void) {
    imports->GetAutoUpdate();
}

void trap_OpenURL(pointer s) {
    imports->OpenURL(s);
}

void trap_GetHunkData(sint *hunkused, sint *hunkexpected) {
    imports->GetHunkInfo(hunkused, hunkexpected);
}

sint trap_Cvar_VariableInt(pointer var_name) {
    return cvarSystem->VariableIntegerValue(var_name);
}

sint trap_CrosshairPlayer(void) {
    return imports->idcgame->CrosshairPlayer();
}

sint trap_LastAttacker(void) {
    return imports->idcgame->LastAttacker();
}