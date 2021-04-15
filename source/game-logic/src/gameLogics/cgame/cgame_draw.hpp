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
// File name:   cgame_draw.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CGAME_DRAW_H__
#define __CGAME_DRAW_H__

class idCGameLocal;

class idCGameDraw : public idCGameLocal {
public:
    idCGameDraw();
    ~idCGameDraw();

    static void AlignText(rectDef_t *rect, pointer text, float32 scale,
                          float32 w, float32 h, sint align, sint valign, float32 *x, float32 *y);
    static void DrawFieldPadded(sint x, sint y, sint width, sint cw, sint ch,
                                sint value);
    static void DrawField(float32 x, float32 y, sint width, float32 cw,
                          float32 ch, sint value);
    static void DrawProgressBar(rectDef_t *rect, vec4_t color, float32 scale,
                                sint align, sint textalign, sint textStyle, sint special,
                                float32 progress);
    static void DrawPlayerCreditsValue(rectDef_t *rect, vec4_t color,
                                       bool padding);
    static void DrawAttackFeedback(rectDef_t *rect);
    static void DrawPlayerStamina(sint ownerDraw, rectDef_t *rect,
                                  vec4_t backColor, vec4_t foreColor, qhandle_t shader);
    static void DrawPlayerStaminaBolt(rectDef_t *rect, vec4_t backColor,
                                      vec4_t foreColor, qhandle_t shader);
    static void DrawPlayerClipsRing(rectDef_t *rect, vec4_t backColor,
                                    vec4_t foreColor, qhandle_t shader);
    static void DrawPlayerBuildTimerRing(rectDef_t *rect, vec4_t backColor,
                                         vec4_t foreColor, qhandle_t shader);
    static void DrawPlayerBoosted(rectDef_t *rect, vec4_t backColor,
                                  vec4_t foreColor, qhandle_t shader);
    static void DrawPlayerBoosterBolt(rectDef_t *rect, vec4_t backColor,
                                      vec4_t foreColor, qhandle_t shader);
    static void DrawPlayerPoisonBarbs(rectDef_t *rect, vec4_t color,
                                      qhandle_t shader);
    static void DrawPlayerWallclimbing(rectDef_t *rect, vec4_t backColor,
                                       vec4_t foreColor, qhandle_t shader);
    static void DrawPlayerAmmoValue(rectDef_t *rect, vec4_t color);
    static void DrawStack(rectDef_t *rect, vec4_t color, float32 fill,
                          sint align, sint valign, float32 val, sint max);
    static void DrawPlayerAmmoStack(rectDef_t *rect, vec4_t backColor,
                                    vec4_t foreColor, sint textalign, sint textvalign);
    static void DrawPlayerClipsStack(rectDef_t *rect, vec4_t backColor,
                                     vec4_t foreColor, sint textalign, sint textvalign);
    static void DrawAlienSense(rectDef_t *rect);
    static void DrawHumanScanner(rectDef_t *rect, qhandle_t shader,
                                 vec4_t color);
    static void DrawUsableBuildable(rectDef_t *rect, qhandle_t shader,
                                    vec4_t color);
    static void DrawPlayerBuildTimer(rectDef_t *rect, vec4_t color);
    static void DrawPlayerClipsValue(rectDef_t *rect, vec4_t color);
    static void DrawPlayerHealthValue(rectDef_t *rect, vec4_t color);
    static void DrawPlayerHealthCross(rectDef_t *rect, vec4_t ref_color);
    static float32 ChargeProgress(void);
    static void DrawPlayerChargeBarBG(rectDef_t *rect, vec4_t ref_color,
                                      qhandle_t shader);
    static void DrawPlayerChargeBar(rectDef_t *rect, vec4_t ref_color,
                                    qhandle_t shader);
    static void DrawProgressLabel(rectDef_t *rect, float32 text_x,
                                  float32 text_y, vec4_t color, float32 scale, sint textalign,
                                  sint textvalign, pointer s, float32 fraction);
    static void DrawMediaProgress(rectDef_t *rect, vec4_t color, float32 scale,
                                  sint align, sint textalign, sint textStyle, sint special);
    static void DrawMediaProgressLabel(rectDef_t *rect, float32 text_x,
                                       float32 text_y, vec4_t color, float32 scale, sint textalign,
                                       sint textvalign);
    static void DrawBuildablesProgress(rectDef_t *rect, vec4_t color,
                                       float32 scale, sint align, sint textalign, sint textStyle, sint special);
    static void DrawBuildablesProgressLabel(rectDef_t *rect, float32 text_x,
                                            float32 text_y, vec4_t color, float32 scale, sint textalign,
                                            sint textvalign);
    static void DrawCharModelProgress(rectDef_t *rect, vec4_t color,
                                      float32 scale, sint align, sint textalign, sint textStyle, sint special);
    static void DrawCharModelProgressLabel(rectDef_t *rect, float32 text_x,
                                           float32 text_y, vec4_t color, float32 scale, sint textalign,
                                           sint textvalign);
    static void DrawOverallProgress(rectDef_t *rect, vec4_t color,
                                    float32 scale, sint align, sint textalign, sint textStyle, sint special);
    static void DrawLevelShot(rectDef_t *rect);
    static void DrawLevelName(rectDef_t *rect, float32 text_x, float32 text_y,
                              vec4_t color, float32 scale, sint textalign, sint textvalign,
                              sint textStyle);
    static void DrawMOTD(rectDef_t *rect, float32 text_x, float32 text_y,
                         vec4_t color, float32 scale, sint textalign, sint textvalign,
                         sint textStyle);
    static void DrawHostname(rectDef_t *rect, float32 text_x, float32 text_y,
                             vec4_t color, float32 scale, sint textalign, sint textvalign,
                             sint textStyle);
    static void DrawDemoPlayback(rectDef_t *rect, vec4_t color,
                                 qhandle_t shader);
    static void DrawDemoRecording(rectDef_t *rect, vec4_t color,
                                  qhandle_t shader);
    static void UpdateMediaFraction(float32 newFract);
    static void DrawLoadingScreen(void);
    static float32 GetValue(sint ownerDraw);
    static pointer GetKillerText(void);
    static void DrawKiller(rectDef_t *rect, float32 scale, vec4_t color,
                           qhandle_t shader, sint textStyle);
    static void DrawTeamSpectators(rectDef_t *rect, float32 scale,
                                   sint textvalign, vec4_t color, qhandle_t shader);
    static void DrawTeamLabel(rectDef_t *rect, team_t team, float32 text_x,
                              float32 text_y, vec4_t color, float32 scale, sint textalign,
                              sint textvalign, sint textStyle);
    static void DrawStageReport(rectDef_t *rect, float32 text_x,
                                float32 text_y, vec4_t color, float32 scale, sint textalign,
                                sint textvalign, sint textStyle);
    static void DrawFPS(rectDef_t *rect, float32 text_x, float32 text_y,
                        float32 scale, vec4_t color, sint textalign, sint textvalign,
                        sint textStyle, bool scalableText);
    static void DrawSpeed(rectDef_t *rect, float32 text_x, float32 text_y,
                          float32 scale, vec4_t color, sint textalign, sint textvalign,
                          sint textStyle);
    static void DrawTimerMins(rectDef_t *rect, vec4_t color);
    static void DrawTimerSecs(rectDef_t *rect, vec4_t color);
    static void DrawTimer(rectDef_t *rect, float32 text_x, float32 text_y,
                          float32 scale, vec4_t color, sint textalign, sint textvalign,
                          sint textStyle);
    static void DrawClock(rectDef_t *rect, float32 text_x, float32 text_y,
                          float32 scale, vec4_t color, sint textalign, sint textvalign,
                          sint textStyle);
    static void DrawSnapshot(rectDef_t *rect, float32 text_x, float32 text_y,
                             float32 scale, vec4_t color, sint textalign, sint textvalign,
                             sint textStyle);
    static void AddLagometerFrameInfo(void);
    static void AddLagometerSnapshotInfo(snapshot_t *snap);
    static void DrawDisconnect(void);
    static void DrawLagometer(rectDef_t *rect, float32 text_x, float32 text_y,
                              float32 scale, vec4_t textColor);
    static void DrawConsole(rectDef_t *rect, float32 text_x, float32 text_y,
                            vec4_t color, float32 scale, sint textalign, sint textvalign,
                            sint textStyle);
    static void DrawTutorial(rectDef_t *rect, float32 text_x, float32 text_y,
                             vec4_t color, float32 scale, sint textalign, sint textvalign,
                             sint textStyle);
    static void DrawWeaponIcon(rectDef_t *rect, vec4_t color);
    static void DrawCrosshair(rectDef_t *rect, vec4_t color);
    static void ScanForCrosshairEntity(float32 *zChange, bool *hitClient);
    static void DrawLocation(rectDef_t *rect, float32 scale, sint textalign,
                             vec4_t color);
    static void DrawCrosshairNames(rectDef_t *rect, float32 scale,
                                   sint textStyle);
    static void DrawSquadMarkers(vec4_t color);
    static void OwnerDraw(float32 x, float32 y, float32 w, float32 h,
                          float32 text_x, float32 text_y, sint ownerDraw, sint ownerDrawFlags,
                          sint align, sint textalign, sint textvalign, float32 special,
                          float32 scale, vec4_t foreColor, vec4_t backColor, qhandle_t shader,
                          sint textStyle);
    static void HideTeamMenu(void);
    void ShowTeamMenu(void);
    sint ClientNumFromName(pointer p);
    static void RunMenuScript(valueType **args);
    static void DrawLighting(void);
    static void CenterPrint(pointer str, sint y, sint charWidth);
    static void DrawCenterString(void);
    static void DrawVote(void);
    static void DrawTeamVote(void);
    static bool DrawScoreboard(void);
    static void DrawIntermission(void);
    static bool DrawFollow(void);
    static bool DrawQueue(void);
    static void DrawBotInfo(void);
    static void Draw2D(void);
    static void ScalePainBlendTCs(float32 *s1, float32 *t1, float32 *s2,
                                  float32 *t2);
    static void PainBlend(void);
    static void ResetPainBlend(void);
    static void DrawActive(stereoFrame_t stereoView);
};

#endif //!__CGAME_DRAW_H__
