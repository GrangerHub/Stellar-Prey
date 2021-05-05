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
// File name:   cgame_draw.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description: draw all of the graphical elements during
//              active (after loading) gameplay
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <cgame/cgame_precompiled.hpp>

menuDef_t *menuScoreboard = nullptr;

sint drawTeamOverlayModificationCount = -1;
sint sortedTeamPlayers[ TEAM_MAXOVERLAY ];
sint numSortedTeamPlayers;

/*
===============
idCGameDraw::idCGameDraw
===============
*/
idCGameDraw::idCGameDraw(void) {
}

/*
===============
idCGameDraw::~idCGameDraw
===============
*/
idCGameDraw::~idCGameDraw(void) {
}

/*
===============
idCGameDraw::AlignText
===============
*/
void idCGameDraw::AlignText(rectDef_t *rect, pointer text, float32 scale,
                            float32 w, float32 h, sint align, sint valign, float32 *x, float32 *y) {
    float32 tx, ty;

    if(scale > 0.0f) {
        w = UI_Text_Width(text, scale, 0);
        h = UI_Text_Height(text, scale, 0);
    }

    switch(align) {
        default:
        case ALIGN_LEFT:
            tx = 0.0f;
            break;

        case ALIGN_RIGHT:
            tx = rect->w - w;
            break;

        case ALIGN_CENTER:
            tx = (rect->w - w) / 2.0f;
            break;
    }

    switch(valign) {
        default:
        case VALIGN_BOTTOM:
            ty = rect->h;
            break;

        case VALIGN_TOP:
            ty = h;
            break;

        case VALIGN_CENTER:
            ty = h + ((rect->h - h) / 2.0f);
            break;
    }

    if(x) {
        *x = rect->x + tx;
    }

    if(y) {
        *y = rect->y + ty;
    }
}

/*
==============
idCGameDraw::DrawFieldPadded

Draws large numbers for status bar
==============
*/
void idCGameDraw::DrawFieldPadded(sint x, sint y, sint width, sint cw,
                                  sint ch, sint value) {
    valueType num[ 16 ], *ptr;
    sint l, orgL, frame, charWidth, charHeight;

    if(!(charWidth = cw)) {
        charWidth = CHAR_WIDTH;
    }

    if(!(charHeight = ch)) {
        charHeight = CHAR_HEIGHT;
    }

    if(width < 1) {
        return;
    }

    // draw number string
    if(width > 4) {
        width = 4;
    }

    switch(width) {
        case 1:
            value = value > 9 ? 9 : value;
            value = value < 0 ? 0 : value;
            break;

        case 2:
            value = value > 99 ? 99 : value;
            value = value < -9 ? -9 : value;
            break;

        case 3:
            value = value > 999 ? 999 : value;
            value = value < -99 ? -99 : value;
            break;

        case 4:
            value = value > 9999 ? 9999 : value;
            value = value < -999 ? -999 : value;
            break;
    }

    Q_vsprintf_s(num, sizeof(num), sizeof(num), "%d", value);
    l = strlen(num);

    if(l > width) {
        l = width;
    }

    orgL = l;

    x += (2.0f * cgDC.aspectScale);

    ptr = num;

    while(*ptr && l) {
        if(width > orgL) {
            idCGameDrawTools::DrawPic(x, y, charWidth, charHeight,
                                      cgs.media.numberShaders[ 0 ]);
            width--;
            x += charWidth;
            continue;
        }

        if(*ptr == '-') {
            frame = STAT_MINUS;
        } else {
            frame = *ptr - '0';
        }

        idCGameDrawTools::DrawPic(x, y, charWidth, charHeight,
                                  cgs.media.numberShaders[ frame ]);
        x += charWidth;
        ptr++;
        l--;
    }
}

/*
==============
idCGameDraw::DrawField

Draws large numbers for status bar
==============
*/
void idCGameDraw::DrawField(float32 x, float32 y, sint width, float32 cw,
                            float32 ch, sint value) {
    valueType num[ 16 ], *ptr;
    sint l, frame;
    float32 charWidth, charHeight;

    if(!(charWidth = cw)) {
        charWidth = CHAR_WIDTH;
    }

    if(!(charHeight = ch)) {
        charHeight = CHAR_HEIGHT;
    }

    if(width < 1) {
        return;
    }

    // draw number string
    if(width > 4) {
        width = 4;
    }

    switch(width) {
        case 1:
            value = value > 9 ? 9 : value;
            value = value < 0 ? 0 : value;
            break;

        case 2:
            value = value > 99 ? 99 : value;
            value = value < -9 ? -9 : value;
            break;

        case 3:
            value = value > 999 ? 999 : value;
            value = value < -99 ? -99 : value;
            break;

        case 4:
            value = value > 9999 ? 9999 : value;
            value = value < -999 ? -999 : value;
            break;
    }

    Q_vsprintf_s(num, sizeof(num), sizeof(num), "%d", value);
    l = strlen(num);

    if(l > width) {
        l = width;
    }

    x += (2.0f * cgDC.aspectScale) + charWidth * (width - l);

    ptr = num;

    while(*ptr && l) {
        if(*ptr == '-') {
            frame = STAT_MINUS;
        } else {
            frame = *ptr - '0';
        }

        idCGameDrawTools::DrawPic(x, y, charWidth, charHeight,
                                  cgs.media.numberShaders[ frame ]);
        x += charWidth;
        ptr++;
        l--;
    }
}

/*
===============
idCGameDraw::DrawProgressBar
===============
*/
void idCGameDraw::DrawProgressBar(rectDef_t *rect, vec4_t color,
                                  float32 scale, sint align, sint textalign, sint textStyle, sint special,
                                  float32 progress) {
    float32 rimWidth = rect->h / 20.0f;
    float32 doneWidth, leftWidth, tx, ty;
    valueType textBuffer[ 8 ];

    if(rimWidth < 0.6f) {
        rimWidth = 0.6f;
    }

    if(special >= 0.0f) {
        rimWidth = special;
    }

    if(progress < 0.0f) {
        progress = 0.0f;
    } else if(progress > 1.0f) {
        progress = 1.0f;
    }

    doneWidth = (rect->w - 2 * rimWidth) * progress;
    leftWidth = (rect->w - 2 * rimWidth) - doneWidth;

    trap_R_SetColor(color);

    //draw rim and bar
    if(align == ALIGN_RIGHT) {
        idCGameDrawTools::DrawPic(rect->x, rect->y, rimWidth, rect->h,
                                  cgs.media.whiteShader);
        idCGameDrawTools::DrawPic(rect->x + rimWidth, rect->y, leftWidth, rimWidth,
                                  cgs.media.whiteShader);
        idCGameDrawTools::DrawPic(rect->x + rimWidth, rect->y + rect->h - rimWidth,
                                  leftWidth, rimWidth, cgs.media.whiteShader);
        idCGameDrawTools::DrawPic(rect->x + rimWidth + leftWidth, rect->y,
                                  rimWidth + doneWidth, rect->h, cgs.media.whiteShader);
    } else {
        idCGameDrawTools::DrawPic(rect->x, rect->y, rimWidth + doneWidth, rect->h,
                                  cgs.media.whiteShader);
        idCGameDrawTools::DrawPic(rimWidth + rect->x + doneWidth, rect->y,
                                  leftWidth, rimWidth, cgs.media.whiteShader);
        idCGameDrawTools::DrawPic(rimWidth + rect->x + doneWidth,
                                  rect->y + rect->h - rimWidth, leftWidth, rimWidth, cgs.media.whiteShader);
        idCGameDrawTools::DrawPic(rect->x + rect->w - rimWidth, rect->y, rimWidth,
                                  rect->h, cgs.media.whiteShader);
    }

    trap_R_SetColor(nullptr);

    //draw text
    if(scale > 0.0) {
        Q_vsprintf_s(textBuffer, sizeof(textBuffer), sizeof(textBuffer), "%d%%",
                     (sint)(progress * 100));
        AlignText(rect, textBuffer, scale, 0.0f, 0.0f, textalign, VALIGN_CENTER,
                  &tx, &ty);

        UI_Text_Paint(tx, ty, scale, color, textBuffer, 0, 0, textStyle);
    }
}

/*
===============
idCGameDraw::DrawPlayerCreditsValue
===============
*/
#define NO_CREDITS_TIME 2000
void idCGameDraw::DrawPlayerCreditsValue(rectDef_t *rect, vec4_t color,
        bool padding) {
    sint value;
    playerState_t *ps;
    centity_t *cent;

    cent = &cg_entities[ cg.snap->ps.clientNum ];
    ps = &cg.snap->ps;

    //if the build timer pie is showing don't show this
    if((cent->currentState.weapon == WP_ABUILD ||
            cent->currentState.weapon == WP_ABUILD2) && ps->stats[STAT_MISC]) {
        return;
    }

    value = ps->persistant[ PERS_CREDIT ];

    if(value > -1) {
        if(cg.predictedPlayerState.stats[ STAT_TEAM ] == TEAM_ALIENS) {
            if(!bggame->AlienCanEvolve((class_t)
                                       cg.predictedPlayerState.stats[STAT_CLASS], value, cgs.alienStage) &&
                    cg.time - cg.lastEvolveAttempt <= NO_CREDITS_TIME &&
                    ((cg.time - cg.lastEvolveAttempt) / 300) & 1) {
                color[3] = 0.0f;
            }

            value /= ALIEN_CREDITS_PER_FRAG;
        }

        trap_R_SetColor(color);

        if(padding) {
            DrawFieldPadded(rect->x, rect->y, 4, rect->w / 4, rect->h, value);
        } else {
            DrawField(rect->x, rect->y, 1, rect->w, rect->h, value);
        }

        trap_R_SetColor(nullptr);
    }
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
void idCGameDraw::DrawAttackFeedback(rectDef_t *rect) {
    static bool flipAttackFeedback = false;
    sint frame = cg.feedbackAnimation;
    qhandle_t shader;
    vec4_t hit_color = { 1.0f, 0.0f, 0.5f };
    vec4_t miss_color = { 0.3f, 0.3f, 0.3f, 0.5f };
    vec4_t teamhit_color = { 0.39f, 0.80f, 0.00f, 0.5f };

    if(frame == 1) {
        flipAttackFeedback = !flipAttackFeedback;
    }

    //when a new feedback animation event is received, the fame number is set to 1 - so
    //if it is zero, we don't need to draw anything
    if(frame == 0 ||
            !cg_drawAlienFeedback.integer) {   //drop out if we aren't currently needing to draw any feedback
        //Com_Printf(".");
        return;
    } else {
        switch(cg.feedbackAnimationType) {
            case AFEEDBACK_HIT:
            case AFEEDBACK_MISS:
            case AFEEDBACK_TEAMHIT:
                if(flipAttackFeedback) {
                    shader = cgs.media.alienAttackFeedbackShadersFlipped[frame - 1];
                } else {
                    shader = cgs.media.alienAttackFeedbackShaders[frame - 1];
                }

                break;

            case AFEEDBACK_RANGED_HIT:
            case AFEEDBACK_RANGED_MISS:
            case AFEEDBACK_RANGED_TEAMHIT:
                if(flipAttackFeedback) {
                    shader = cgs.media.alienAttackFeedbackShadersFlipped[frame - 1];
                } else {
                    shader = cgs.media.alienAttackFeedbackShaders[frame - 1];
                }

                break;

            default:
                shader = cgs.media.alienAttackFeedbackShaders[ frame - 1 ];
                break;

        }

        cg.feedbackAnimation++;

        if(cg.feedbackAnimation > 10) {
            cg.feedbackAnimation = 0;
        }

        switch(cg.feedbackAnimationType) {
            case AFEEDBACK_HIT:
            case AFEEDBACK_RANGED_HIT:
                trap_R_SetColor(hit_color);
                break;

            case AFEEDBACK_MISS:
            case AFEEDBACK_RANGED_MISS:
                trap_R_SetColor(miss_color);
                break;

            case AFEEDBACK_TEAMHIT:
            case AFEEDBACK_RANGED_TEAMHIT:
                trap_R_SetColor(teamhit_color);
                break;
        }

        idCGameDrawTools::DrawPic(rect->x, rect->y, rect->w, rect->h, shader);
        trap_R_SetColor(nullptr);
    }
}

/*
==============
idCGameDraw::DrawPlayerStamina
==============
*/
void idCGameDraw::DrawPlayerStamina(sint ownerDraw, rectDef_t *rect,
                                    vec4_t backColor, vec4_t foreColor, qhandle_t shader) {
    playerState_t *ps = &cg.snap->ps;
    float32 stamina = ps->stats[ STAT_STAMINA ];
    float32 maxStaminaBy3 = (float32)MAX_STAMINA / 3.0f;
    float32 progress;
    vec4_t color;

    switch(ownerDraw) {
        case CG_PLAYER_STAMINA_1:
            progress = (stamina - 2 * (sint)maxStaminaBy3) / maxStaminaBy3;
            break;

        case CG_PLAYER_STAMINA_2:
            progress = (stamina - (sint)maxStaminaBy3) / maxStaminaBy3;
            break;

        case CG_PLAYER_STAMINA_3:
            progress = stamina / maxStaminaBy3;
            break;

        case CG_PLAYER_STAMINA_4:
            progress = (stamina + MAX_STAMINA) / MAX_STAMINA;
            break;

        default:
            return;
    }

    if(progress > 1.0f) {
        progress = 1.0f;
    } else if(progress < 0.0f) {
        progress = 0.0f;
    }

    Vector4Lerp(progress, backColor, foreColor, color);

    trap_R_SetColor(color);
    idCGameDrawTools::DrawPic(rect->x, rect->y, rect->w, rect->h, shader);
    trap_R_SetColor(nullptr);
}

/*
==============
idCGameDraw::DrawPlayerStaminaBolt
==============
*/
void idCGameDraw::DrawPlayerStaminaBolt(rectDef_t *rect, vec4_t backColor,
                                        vec4_t foreColor, qhandle_t shader) {
    float32 stamina = cg.snap->ps.stats[ STAT_STAMINA ];
    vec4_t color;

    if(stamina < 0) {
        Vector4Copy(backColor, color);
    } else if(cg.predictedPlayerState.stats[STAT_STATE] & SS_SPEEDBOOST) {
        Vector4Lerp((sin(cg.time / 150.f) + 1) / 2, backColor, foreColor, color);
    } else {
        Vector4Copy(foreColor, color);
    }

    trap_R_SetColor(color);
    idCGameDrawTools::DrawPic(rect->x, rect->y, rect->w, rect->h, shader);
    trap_R_SetColor(nullptr);
}

/*
==============
DrawPlayerClipsRing
==============
*/
void idCGameDraw::DrawPlayerClipsRing(rectDef_t *rect, vec4_t backColor,
                                      vec4_t foreColor, qhandle_t shader) {
    playerState_t *ps = &cg.snap->ps;
    centity_t *cent;
    float32 buildTime = ps->stats[ STAT_MISC ];
    float32 progress, maxDelay;
    weapon_t weapon;
    vec4_t color;

    cent = &cg_entities[ cg.snap->ps.clientNum ];
    weapon = bggame->GetPlayerWeapon(ps);

    switch(weapon) {
        case WP_ABUILD:
        case WP_ABUILD2:
        case WP_HBUILD:
            if(buildTime > MAXIMUM_BUILD_TIME) {
                buildTime = MAXIMUM_BUILD_TIME;
            }

            progress = (MAXIMUM_BUILD_TIME - buildTime) / MAXIMUM_BUILD_TIME;

            Vector4Lerp(progress, backColor, foreColor, color);
            break;

        default:
            if(ps->weaponstate == WEAPON_RELOADING) {
                maxDelay = (float32)bggame->Weapon((weapon_t)
                                                   cent->currentState.weapon)->reloadTime;
                progress = (maxDelay - (float32)ps->weaponTime) / maxDelay;

                Vector4Lerp(progress, backColor, foreColor, color);
            } else {
                ::memcpy(color, foreColor, sizeof(color));
            }

            break;
    }

    trap_R_SetColor(color);
    idCGameDrawTools::DrawPic(rect->x, rect->y, rect->w, rect->h, shader);
    trap_R_SetColor(nullptr);
}

/*
==============
idCGameDraw::DrawPlayerBuildTimerRing
==============
*/
void idCGameDraw::DrawPlayerBuildTimerRing(rectDef_t *rect,
        vec4_t backColor, vec4_t foreColor, qhandle_t shader) {
    playerState_t *ps = &cg.snap->ps;
    centity_t *cent;
    float32 buildTime = ps->stats[ STAT_MISC ];
    float32 progress;
    vec4_t color;

    cent = &cg_entities[ cg.snap->ps.clientNum ];

    if(buildTime > MAXIMUM_BUILD_TIME) {
        buildTime = MAXIMUM_BUILD_TIME;
    }

    progress = (MAXIMUM_BUILD_TIME - buildTime) / MAXIMUM_BUILD_TIME;

    Vector4Lerp(progress, backColor, foreColor, color);

    trap_R_SetColor(color);
    idCGameDrawTools::DrawPic(rect->x, rect->y, rect->w, rect->h, shader);
    trap_R_SetColor(nullptr);
}

/*
==============
idCGameDraw::DrawPlayerBoosted
==============
*/
void idCGameDraw::DrawPlayerBoosted(rectDef_t *rect, vec4_t backColor,
                                    vec4_t foreColor, qhandle_t shader) {
    if(cg.snap->ps.stats[STAT_STATE] & SS_BOOSTED) {
        trap_R_SetColor(foreColor);
    } else {
        trap_R_SetColor(backColor);
    }

    idCGameDrawTools::DrawPic(rect->x, rect->y, rect->w, rect->h, shader);
    trap_R_SetColor(nullptr);
}

/*
==============
idCGameDraw::DrawPlayerBoosterBolt
==============
*/
void idCGameDraw::DrawPlayerBoosterBolt(rectDef_t *rect, vec4_t backColor,
                                        vec4_t foreColor, qhandle_t shader) {
    vec4_t color;

    // Flash bolts when the boost is almost out
    if((cg.snap->ps.stats[STAT_STATE] & SS_BOOSTED) &&
            (cg.snap->ps.stats[STAT_STATE] & SS_BOOSTEDWARNING)) {
        Vector4Lerp((sin(cg.time / 100.f) + 1) / 2, backColor, foreColor, color);
    } else {
        Vector4Copy(foreColor, color);
    }

    trap_R_SetColor(color);
    idCGameDrawTools::DrawPic(rect->x, rect->y, rect->w, rect->h, shader);
    trap_R_SetColor(nullptr);
}

/*
==============
idCGameDraw::DrawPlayerPoisonBarbs
==============
*/
void idCGameDraw::DrawPlayerPoisonBarbs(rectDef_t *rect, vec4_t color,
                                        qhandle_t shader) {
    bool vertical;
    float32 x = rect->x, y = rect->y;
    float32 width = rect->w, height = rect->h;
    float32 diff;
    sint iconsize, numBarbs, maxBarbs;

    maxBarbs = bggame->Weapon((weapon_t)cg.snap->ps.weapon)->maxAmmo;
    numBarbs = cg.snap->ps.ammo;

    if(maxBarbs <= 0 || numBarbs <= 0) {
        return;
    }

    // adjust these first to ensure the aspect ratio of the barb image is
    // preserved
    idCGameDrawTools::AdjustFrom640(&x, &y, &width, &height);

    if(height > width) {
        vertical = true;
        iconsize = width;

        if(maxBarbs != 1) {  // avoid division by zero
            diff = (height - iconsize) / (float32)(maxBarbs - 1);
        } else {
            diff = 0; // doesn't matter, won't be used
        }
    } else {
        vertical = false;
        iconsize = height;

        if(maxBarbs != 1) {
            diff = (width - iconsize) / (float32)(maxBarbs - 1);
        } else {
            diff = 0;
        }
    }

    trap_R_SetColor(color);

    for(; numBarbs > 0; numBarbs--) {
        trap_R_DrawStretchPic(x, y, iconsize, iconsize, 0, 0, 1, 1, shader);

        if(vertical) {
            y += diff;
        } else {
            x += diff;
        }
    }

    trap_R_SetColor(nullptr);
}

/*
==============
idCGameDraw::DrawPlayerWallclimbing
==============
*/
void idCGameDraw::DrawPlayerWallclimbing(rectDef_t *rect, vec4_t backColor,
        vec4_t foreColor, qhandle_t shader) {
    if(cg.snap->ps.stats[STAT_STATE] & SS_WALLCLIMBING) {
        trap_R_SetColor(foreColor);
    } else {
        trap_R_SetColor(backColor);
    }

    idCGameDrawTools::DrawPic(rect->x, rect->y, rect->w, rect->h, shader);
    trap_R_SetColor(nullptr);
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
void idCGameDraw::DrawPlayerAmmoValue(rectDef_t *rect, vec4_t color) {
    sint value;

    switch(bggame->PrimaryWeapon(cg.snap->ps.stats)) {
        case WP_NONE:
        case WP_BLASTER:
            return;

        case WP_ABUILD:
        case WP_ABUILD2:
            // BP remaining
            value = cgs.alienBuildPoints;
            break;

        case WP_HBUILD:
            // BP remaining
            value = cgs.humanBuildPoints;
            break;

        default:
            value = cg.snap->ps.ammo;
            break;
    }

    if(value > 999) {
        value = 999;
    }

    if(value > -1) {
        trap_R_SetColor(color);
        DrawField(rect->x, rect->y, 4, rect->w / 4, rect->h, value);
        trap_R_SetColor(nullptr);
    }
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
void idCGameDraw::DrawStack(rectDef_t *rect, vec4_t color, float32 fill,
                            sint align, sint valign, float32 val, sint max) {
    sint i;
    float32 each;
    sint ival;
    float32 frac;
    float32 nudge = 0;
    float32 fmax = max; // otherwise we'd be (float32) casting everywhere

    if(val <= 0 || max <= 0) {
        return;
    }

    ival = (sint)val;
    frac = val - ival;

    trap_R_SetColor(color);

    if(rect->h >= rect->w) {  // vertical stack
        each = fill * rect->h / fmax;

        if(each * cgs.screenYScale < 4.f) {  // FIXME: magic number
            float32 offy, h = rect->h * val / fmax;

            switch(valign) {
                case VALIGN_TOP:
                    offy = 0;
                    break;

                case VALIGN_CENTER:
                    offy = (rect->h - h) / 2;
                    break;

                case VALIGN_BOTTOM:
                default:
                    offy = rect->h - h;
                    break;
            }

            idCGameDrawTools::DrawPic(rect->x, rect->y + offy, rect->w, h,
                                      cgs.media.whiteShader);
            trap_R_SetColor(nullptr);

            return;
        }

        if(fmax > 1) {
            nudge = (1 - fill) / (fmax - 1);
        } else {
            return;
        }

        for(i = 0; i < ival; i++) {
            float32 start;

            switch(valign) {
                case VALIGN_TOP:
                    start = (i * (1 + nudge) + frac) / fmax;
                    break;

                case VALIGN_CENTER:

                // TODO (fallthrough for now)

                default:
                case VALIGN_BOTTOM:
                    start = 1 - (val - i - (i + fmax - val) * nudge) / fmax;
                    break;
            }

            idCGameDrawTools::DrawPic(rect->x, rect->y + rect->h * start, rect->w,
                                      each, cgs.media.whiteShader);
        }

        color[ 3 ] *= frac;

        trap_R_SetColor(color);

        switch(valign) {
            case VALIGN_TOP:
                idCGameDrawTools::DrawPic(rect->x, rect->y - rect->h * (1 - frac) / fmax,
                                          rect->w, each, cgs.media.whiteShader);
                break;

            case VALIGN_CENTER:

            // fallthrough

            default:
            case VALIGN_BOTTOM:
                idCGameDrawTools::DrawPic(rect->x,
                                          rect->y + rect->h * (1 + ((1 - fill) / fmax) - frac / fmax), rect->w, each,
                                          cgs.media.whiteShader);
        }
    } else { // horizontal stack
        each = fill * rect->w / fmax;

        if(each < 4.f) {
            float32 offx, w = rect->w * val / fmax;

            switch(align) {
                case ALIGN_LEFT:

                default:
                    offx = 0;
                    break;

                case ALIGN_CENTER:
                    offx = (rect->w - w) / 2;
                    break;

                case ALIGN_RIGHT:
                    offx = rect->w - w;
                    break;
            }

            idCGameDrawTools::DrawPic(rect->x + offx, rect->y, w, rect->h,
                                      cgs.media.whiteShader);

            trap_R_SetColor(nullptr);
            return;
        }

        if(fmax > 1) {
            nudge = (1 - fill) / (fmax - 1);
        }

        for(i = 0; i < ival; i++) {
            float32 start;

            switch(align) {
                case ALIGN_LEFT:
                    start = (i * (1 + nudge) + frac) / fmax;
                    break;

                case ALIGN_CENTER:

                // TODO (fallthrough for now)

                default:
                case ALIGN_RIGHT:
                    start = 1 - (val - i - (i + fmax - val) * nudge) / fmax;
                    break;
            }

            idCGameDrawTools::DrawPic(rect->x + rect->w * start, rect->y, each,
                                      rect->h, cgs.media.whiteShader);
        }

        color[ 3 ] *= frac;

        trap_R_SetColor(color);

        switch(align) {
            case ALIGN_LEFT:
                idCGameDrawTools::DrawPic(rect->x - (1 - frac) * rect->w / fmax, rect->y,
                                          each, rect->h, cgs.media.whiteShader);
                break;

            case ALIGN_CENTER:

            // fallthrough

            default:
            case ALIGN_RIGHT:
                idCGameDrawTools::DrawPic(rect->x + rect->w * (1 + ((
                                              1 - fill) / fmax) - frac / fmax), rect->y, each, rect->h,
                                          cgs.media.whiteShader);
        }
    }

    trap_R_SetColor(nullptr);
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
void idCGameDraw::DrawPlayerAmmoStack(rectDef_t *rect, vec4_t backColor,
                                      vec4_t foreColor, sint textalign, sint textvalign) {
    float32 val;
    sint maxVal;
    static sint lastws, maxwt, lastammo, ammodiff;
    playerState_t *ps = &cg.snap->ps;
    weapon_t primary = bggame->PrimaryWeapon(ps->stats);

    if(!cg_drawAmmoStack.integer) {
        return;
    }

    switch(primary) {
        case WP_NONE:
        case WP_BLASTER:
            return;

        case WP_ABUILD:
        case WP_ABUILD2:
        case WP_HBUILD:
            // FIXME: send max BP values over the network
            return;

        default:
            val = ps->ammo;
            maxVal = bggame->Weapon(primary)->maxAmmo;

            if(bggame->Weapon(primary)->usesEnergy &&
                    bggame->InventoryContainsUpgrade(UP_BATTPACK, ps->stats)) {
                maxVal *= BATTPACK_MODIFIER;
            }

            break;
    }

    if(ps->weaponstate != lastws || ps->weaponTime > maxwt) {
        maxwt = ps->weaponTime;
        lastws = ps->weaponstate;
    } else if(ps->weaponTime == 0) {
        maxwt = 0;
    }

    if(lastammo != ps->ammo) {
        ammodiff = lastammo - ps->ammo;
        lastammo = ps->ammo;
    }

    // smoothing effects if we're holding primary weapon
    if(primary == bggame->GetPlayerWeapon(ps)) {
        switch(ps->weaponstate) {
            case WEAPON_FIRING:
                if(maxwt > 0) {
                    float32 f = ps->weaponTime / (float32)maxwt;
                    val += ammodiff * f * f;
                }

                break;

            case WEAPON_RELOADING:
                val = ps->weaponTime / (float32)maxwt;
                val *= val;
                val = (1 - val) * (maxVal - ps->ammo) + ps->ammo;
                break;

            default:
                maxwt = 0;
                break;
        }
    }

    trap_R_SetColor(backColor);
    idCGameDrawTools::DrawPic(rect->x, rect->y, rect->w, rect->h,
                              cgs.media.whiteShader);
    trap_R_SetColor(nullptr);
    DrawStack(rect, foreColor, 0.8, textalign, textvalign, val, maxVal);
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
void idCGameDraw::DrawPlayerClipsStack(rectDef_t *rect, vec4_t backColor,
                                       vec4_t foreColor, sint textalign, sint textvalign) {
    float32 val;
    sint maxVal;
    static sint lastws, maxwt;
    playerState_t *ps = &cg.snap->ps;
    weapon_t primary = bggame->PrimaryWeapon(ps->stats);

    if(!cg_drawAmmoStack.integer) {
        return;
    }

    switch(primary) {
        case WP_NONE:
        case WP_BLASTER:
        case WP_ABUILD:
        case WP_ABUILD2:
        case WP_HBUILD:
            return;

        default:
            val = ps->clips;
            maxVal = bggame->Weapon(primary)->maxClips;
            break;
    }

    if(ps->weaponstate != lastws || ps->weaponTime > maxwt) {
        maxwt = ps->weaponTime;
        lastws = ps->weaponstate;
    } else if(ps->weaponTime == 0) {
        maxwt = 0;
    }

    switch(ps->weaponstate) {
        case WEAPON_RELOADING:
            if(maxwt > 0) {
                float32 f = ps->weaponTime / (float32)maxwt;
                val += f * f - 1;
            }

            break;

        default:
            maxwt = 0;
            break;
    }

    trap_R_SetColor(backColor);
    idCGameDrawTools::DrawPic(rect->x, rect->y, rect->w, rect->h,
                              cgs.media.whiteShader);
    trap_R_SetColor(nullptr);
    DrawStack(rect, foreColor, 0.8, textalign, textvalign, val, maxVal);
}

/*
==============
idCGameDraw::DrawAlienSense
==============
*/
void idCGameDraw::DrawAlienSense(rectDef_t *rect) {
    if(bggame->ClassHasAbility((class_t)cg.snap->ps.stats[STAT_CLASS],
                               SCA_ALIENSENSE)) {
        idCGameScanner::AlienSense(rect);
    }
}

/*
==============
idCGameDraw::DrawHumanScanner
==============
*/
void idCGameDraw::DrawHumanScanner(rectDef_t *rect, qhandle_t shader,
                                   vec4_t color) {
    if(bggame->InventoryContainsUpgrade(UP_HELMET, cg.snap->ps.stats)) {
        idCGameScanner::Scanner(rect, shader, color);
    }
}

/*
==============
idCGameDraw::DrawUsableBuildable
==============
*/
void idCGameDraw::DrawUsableBuildable(rectDef_t *rect, qhandle_t shader,
                                      vec4_t color) {
    vec3_t view, point;
    trace_t trace;
    entityState_t *es;

    AngleVectors(cg.refdefViewAngles, view, nullptr, nullptr);
    VectorMA(cg.refdef.vieworg, 64, view, point);
    idCGamePredict::Trace(&trace, cg.refdef.vieworg, nullptr, nullptr, point,
                          cg.predictedPlayerState.clientNum, MASK_SHOT);

    es = &cg_entities[ trace.entityNum ].currentState;

    if(es->eType == ET_BUILDABLE &&
            bggame->Buildable((buildable_t)es->modelindex)->usable &&
            cg.predictedPlayerState.stats[ STAT_TEAM ] == bggame->Buildable((
                        buildable_t)es->modelindex)->team) {
        //hack to prevent showing the usable buildable when you aren't carrying an energy weapon
        if((es->modelindex == BA_H_REACTOR || es->modelindex == BA_H_REPEATER) &&
                (!bggame->Weapon((weapon_t)cg.snap->ps.weapon)->usesEnergy ||
                 bggame->Weapon((weapon_t)cg.snap->ps.weapon)->infiniteAmmo)) {
            return;
        }

        trap_R_SetColor(color);
        idCGameDrawTools::DrawPic(rect->x, rect->y, rect->w, rect->h, shader);
        trap_R_SetColor(nullptr);
    }
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
#define BUILD_DELAY_TIME  2000
void idCGameDraw::DrawPlayerBuildTimer(rectDef_t *rect, vec4_t color) {
    sint index;
    playerState_t *ps;

    ps = &cg.snap->ps;

    if(ps->stats[STAT_MISC] <= 0) {
        return;
    }

    switch(bggame->PrimaryWeapon(ps->stats)) {
        case WP_ABUILD:
        case WP_ABUILD2:
        case WP_HBUILD:
            break;

        default:
            return;
    }

    index = 8 * (ps->stats[ STAT_MISC ] - 1) / MAXIMUM_BUILD_TIME;

    if(index > 7) {
        index = 7;
    } else if(index < 0) {
        index = 0;
    }

    if(cg.time - cg.lastBuildAttempt <= BUILD_DELAY_TIME &&
            ((cg.time - cg.lastBuildAttempt) / 300) % 2) {
        color[ 0 ] = 1.0f;
        color[ 1 ] = color[ 2 ] = 0.0f;
        color[ 3 ] = 1.0f;
    }

    trap_R_SetColor(color);
    idCGameDrawTools::DrawPic(rect->x, rect->y, rect->w, rect->h,
                              cgs.media.buildWeaponTimerPie[ index ]);
    trap_R_SetColor(nullptr);
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
void idCGameDraw::DrawPlayerClipsValue(rectDef_t *rect, vec4_t color) {
    sint value;
    playerState_t *ps = &cg.snap->ps;

    switch(bggame->PrimaryWeapon(ps->stats)) {
        case WP_NONE:
        case WP_BLASTER:
        case WP_ABUILD:
        case WP_ABUILD2:
        case WP_HBUILD:
            return;

        default:
            value = ps->clips;

            if(value > -1) {
                trap_R_SetColor(color);
                DrawField(rect->x, rect->y, 4, rect->w / 4, rect->h, value);
                trap_R_SetColor(nullptr);
            }

            break;
    }
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
void idCGameDraw::DrawPlayerHealthValue(rectDef_t *rect, vec4_t color) {
    trap_R_SetColor(color);
    DrawField(rect->x, rect->y, 4, rect->w / 4, rect->h,
              cg.snap->ps.stats[ STAT_HEALTH ]);
    trap_R_SetColor(nullptr);
}

/*
==============
DrawPlayerHealthCross
==============
*/
void idCGameDraw::DrawPlayerHealthCross(rectDef_t *rect,
                                        vec4_t ref_color) {
    qhandle_t shader;
    vec4_t color;
    float32 ref_alpha;

    // Pick the current icon
    shader = cgs.media.healthCross;

    if(cg.snap->ps.stats[STAT_STATE] & SS_HEALING_3X) {
        shader = cgs.media.healthCross3X;
    } else if(cg.snap->ps.stats[ STAT_STATE ] & SS_HEALING_2X) {
        if(cg.snap->ps.stats[STAT_TEAM] == TEAM_ALIENS) {
            shader = cgs.media.healthCross2X;
        } else {
            shader = cgs.media.healthCrossMedkit;
        }
    } else if(cg.snap->ps.stats[STAT_STATE] & SS_POISONED) {
        shader = cgs.media.healthCrossPoisoned;
    }

    // Pick the alpha value
    Vector4Copy(ref_color, color);

    if(cg.snap->ps.stats[ STAT_TEAM ] == TEAM_HUMANS &&
            cg.snap->ps.stats[ STAT_HEALTH ] < 10) {
        color[ 0 ] = 1.0f;
        color[ 1 ] = color[ 2 ] = 0.0f;
    }

    ref_alpha = ref_color[ 3 ];

    if(cg.snap->ps.stats[STAT_STATE] & SS_HEALING_ACTIVE) {
        ref_alpha = 1.f;
    }

    // Don't fade from nothing
    if(!cg.lastHealthCross) {
        cg.lastHealthCross = shader;
    }

    // Fade the icon during transition
    if(cg.lastHealthCross != shader) {
        cg.healthCrossFade += cg.frametime / 500.f;

        if(cg.healthCrossFade > 1.f) {
            cg.healthCrossFade = 0.f;
            cg.lastHealthCross = shader;
        } else {
            // Fading between two icons
            color[ 3 ] = ref_alpha * cg.healthCrossFade;
            trap_R_SetColor(color);
            idCGameDrawTools::DrawPic(rect->x, rect->y, rect->w, rect->h, shader);
            color[ 3 ] = ref_alpha * (1.f - cg.healthCrossFade);
            trap_R_SetColor(color);
            idCGameDrawTools::DrawPic(rect->x, rect->y, rect->w, rect->h,
                                      cg.lastHealthCross);
            trap_R_SetColor(nullptr);
            return;
        }
    }

    // Not fading, draw a single icon
    color[ 3 ] = ref_alpha;
    trap_R_SetColor(color);
    idCGameDrawTools::DrawPic(rect->x, rect->y, rect->w, rect->h, shader);
    trap_R_SetColor(nullptr);
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
float32 idCGameDraw::ChargeProgress(void) {
    float32 progress;
    sint min = 0, max = 0;

    if(cg.snap->ps.weapon == WP_ALEVEL3) {
        min = LEVEL3_POUNCE_TIME_MIN;
        max = LEVEL3_POUNCE_TIME;
    } else if(cg.snap->ps.weapon == WP_ALEVEL3_UPG) {
        min = LEVEL3_POUNCE_TIME_MIN;
        max = LEVEL3_POUNCE_TIME_UPG;
    } else if(cg.snap->ps.weapon == WP_ALEVEL4) {
        if(cg.predictedPlayerState.stats[ STAT_STATE ] & SS_CHARGING) {
            min = 0;
            max = LEVEL4_TRAMPLE_DURATION;
        } else {
            min = LEVEL4_TRAMPLE_CHARGE_MIN;
            max = LEVEL4_TRAMPLE_CHARGE_MAX;
        }
    } else if(cg.snap->ps.weapon == WP_LUCIFER_CANNON) {
        min = LCANNON_CHARGE_TIME_MIN;
        max = LCANNON_CHARGE_TIME_MAX;
    }

    if(max - min <= 0.f) {
        return 0.f;
    }

    progress = ((float32)cg.predictedPlayerState.stats[ STAT_MISC ] - min) /
               (max - min);

    if(progress > 1.f) {
        return 1.f;
    }

    if(progress < 0.f) {
        return 0.f;
    }

    return progress;
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
#define CHARGE_BAR_FADE_RATE 0.002f
void idCGameDraw::DrawPlayerChargeBarBG(rectDef_t *rect, vec4_t ref_color,
                                        qhandle_t shader) {
    vec4_t color;

    if(!cg_drawChargeBar.integer || cg.chargeMeterAlpha <= 0.f) {
        return;
    }

    color[ 0 ] = ref_color[ 0 ];
    color[ 1 ] = ref_color[ 1 ];
    color[ 2 ] = ref_color[ 2 ];
    color[ 3 ] = ref_color[ 3 ] * cg.chargeMeterAlpha;

    // Draw meter background
    trap_R_SetColor(color);
    idCGameDrawTools::DrawPic(rect->x, rect->y, rect->w, rect->h, shader);
    trap_R_SetColor(nullptr);
}

/*
===============
idCGameDraw::DrawAttackFeedback

FIXME: This should come from the element info
===============
*/
#define CHARGE_BAR_CAP_SIZE 3
void idCGameDraw::DrawPlayerChargeBar(rectDef_t *rect, vec4_t ref_color,
                                      qhandle_t shader) {
    vec4_t color;
    float32 x, y, width, height, cap_size, progress;

    if(!cg_drawChargeBar.integer) {
        return;
    }

    // Get progress proportion and pump fade
    progress = ChargeProgress();

    if(progress <= 0.f) {
        cg.chargeMeterAlpha -= CHARGE_BAR_FADE_RATE * cg.frametime;

        if(cg.chargeMeterAlpha <= 0.f) {
            cg.chargeMeterAlpha = 0.f;
            return;
        }
    } else {
        cg.chargeMeterValue = progress;
        cg.chargeMeterAlpha += CHARGE_BAR_FADE_RATE * cg.frametime;

        if(cg.chargeMeterAlpha > 1.f) {
            cg.chargeMeterAlpha = 1.f;
        }
    }

    color[ 0 ] = ref_color[ 0 ];
    color[ 1 ] = ref_color[ 1 ];
    color[ 2 ] = ref_color[ 2 ];
    color[ 3 ] = ref_color[ 3 ] * cg.chargeMeterAlpha;

    // Flash red for Lucifer Cannon warning
    if(cg.snap->ps.weapon == WP_LUCIFER_CANNON &&
            cg.snap->ps.stats[ STAT_MISC ] >= LCANNON_CHARGE_TIME_WARN &&
            (cg.time & 128)) {
        color[ 0 ] = 1.f;
        color[ 1 ] = 0.f;
        color[ 2 ] = 0.f;
    }

    x = rect->x;
    y = rect->y;

    // Horizontal charge bar
    if(rect->w >= rect->h) {
        width = (rect->w - CHARGE_BAR_CAP_SIZE * 2) * cg.chargeMeterValue;
        height = rect->h;
        idCGameDrawTools::AdjustFrom640(&x, &y, &width, &height);
        cap_size = CHARGE_BAR_CAP_SIZE * cgs.screenXScale;

        // Draw the meter
        trap_R_SetColor(color);
        trap_R_DrawStretchPic(x, y, cap_size, height, 0, 0, 1, 1, shader);
        trap_R_DrawStretchPic(x + width + cap_size, y, cap_size, height, 1, 0, 0,
                              1, shader);
        trap_R_DrawStretchPic(x + cap_size, y, width, height, 1, 0, 1, 1, shader);
        trap_R_SetColor(nullptr);
    }

    // Vertical charge bar
    else {
        y += rect->h;
        width = rect->w;
        height = (rect->h - CHARGE_BAR_CAP_SIZE * 2) * cg.chargeMeterValue;
        idCGameDrawTools::AdjustFrom640(&x, &y, &width, &height);
        cap_size = CHARGE_BAR_CAP_SIZE * cgs.screenYScale;

        // Draw the meter
        trap_R_SetColor(color);
        trap_R_DrawStretchPic(x, y - cap_size, width, cap_size, 0, 1, 1, 0,
                              shader);
        trap_R_DrawStretchPic(x, y - height - cap_size * 2, width, cap_size, 0, 0,
                              1, 1, shader);
        trap_R_DrawStretchPic(x, y - height - cap_size, width, height, 0, 1, 1, 1,
                              shader);
        trap_R_SetColor(nullptr);
    }
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
void idCGameDraw::DrawProgressLabel(rectDef_t *rect, float32 text_x,
                                    float32 text_y, vec4_t color, float32 scale, sint textalign,
                                    sint textvalign, pointer s, float32 fraction) {
    vec4_t white = { 1.0f, 1.0f, 1.0f, 1.0f };
    float32 tx, ty;

    AlignText(rect, s, scale, 0.0f, 0.0f, textalign, textvalign, &tx, &ty);

    if(fraction < 1.0f) {
        UI_Text_Paint(text_x + tx, text_y + ty, scale, white, s, 0, 0,
                      ITEM_TEXTSTYLE_NORMAL);
    } else {
        UI_Text_Paint(text_x + tx, text_y + ty, scale, color, s, 0, 0,
                      ITEM_TEXTSTYLE_NEON);
    }
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
void idCGameDraw::DrawMediaProgress(rectDef_t *rect, vec4_t color,
                                    float32 scale, sint align, sint textalign, sint textStyle, sint special) {
    DrawProgressBar(rect, color, scale, align, textalign, textStyle, special,
                    cg.mediaFraction);
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
void idCGameDraw::DrawMediaProgressLabel(rectDef_t *rect, float32 text_x,
        float32 text_y, vec4_t color, float32 scale, sint textalign,
        sint textvalign) {
    DrawProgressLabel(rect, text_x, text_y, color, scale, textalign,
                      textvalign, "Map and Textures", cg.mediaFraction);
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
void idCGameDraw::DrawBuildablesProgress(rectDef_t *rect, vec4_t color,
        float32 scale, sint align, sint textalign, sint textStyle, sint special) {
    DrawProgressBar(rect, color, scale, align, textalign, textStyle, special,
                    cg.buildablesFraction);
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
void idCGameDraw::DrawBuildablesProgressLabel(rectDef_t *rect,
        float32 text_x, float32 text_y, vec4_t color, float32 scale,
        sint textalign, sint textvalign) {
    DrawProgressLabel(rect, text_x, text_y, color, scale, textalign,
                      textvalign, "Buildable Models", cg.buildablesFraction);
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
void idCGameDraw::DrawCharModelProgress(rectDef_t *rect, vec4_t color,
                                        float32 scale, sint align, sint textalign, sint textStyle, sint special) {
    DrawProgressBar(rect, color, scale, align, textalign, textStyle, special,
                    cg.charModelFraction);
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
void idCGameDraw::DrawCharModelProgressLabel(rectDef_t *rect,
        float32 text_x, float32 text_y, vec4_t color, float32 scale,
        sint textalign, sint textvalign) {
    DrawProgressLabel(rect, text_x, text_y, color, scale, textalign,
                      textvalign, "Character Models", cg.charModelFraction);
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
void idCGameDraw::DrawOverallProgress(rectDef_t *rect, vec4_t color,
                                      float32 scale, sint align, sint textalign, sint textStyle, sint special) {
    float32 total;

    total = (cg.charModelFraction + cg.buildablesFraction + cg.mediaFraction) /
            3.0f;
    DrawProgressBar(rect, color, scale, align, textalign, textStyle, special,
                    total);
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
void idCGameDraw::DrawLevelShot(rectDef_t *rect) {
    pointer s, info;
    qhandle_t levelshot, detail;

    info = idCGameMain::ConfigString(CS_SERVERINFO);
    s = Info_ValueForKey(info, "mapname");
    levelshot = trap_R_RegisterShaderNoMip(va("levelshots/%s.tga", s));

    if(!levelshot) {
        levelshot = trap_R_RegisterShaderNoMip("gfx/2d/load_screen");
    }

    trap_R_SetColor(nullptr);
    idCGameDrawTools::DrawPic(rect->x, rect->y, rect->w, rect->h, levelshot);

    // blend a detail texture over it
    detail = trap_R_RegisterShader("gfx/misc/detail");
    idCGameDrawTools::DrawPic(rect->x, rect->y, rect->w, rect->h, detail);
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
void idCGameDraw::DrawLevelName(rectDef_t *rect, float32 text_x,
                                float32 text_y, vec4_t color, float32 scale, sint textalign,
                                sint textvalign, sint textStyle) {
    pointer s;

    s = idCGameMain::ConfigString(CS_MESSAGE);

    UI_DrawTextBlock(rect, text_x, text_y, color, scale, textalign, textvalign,
                     textStyle, s);
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
void idCGameDraw::DrawMOTD(rectDef_t *rect, float32 text_x, float32 text_y,
                           vec4_t color, float32 scale, sint textalign, sint textvalign,
                           sint textStyle) {
    pointer s;

    s = idCGameMain::ConfigString(CS_MOTD);

    UI_DrawTextBlock(rect, text_x, text_y, color, scale, textalign, textvalign,
                     textStyle, s);
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
void idCGameDraw::DrawHostname(rectDef_t *rect, float32 text_x,
                               float32 text_y, vec4_t color, float32 scale, sint textalign,
                               sint textvalign, sint textStyle) {
    valueType buffer[ 1024 ];
    pointer info;

    info = idCGameMain::ConfigString(CS_SERVERINFO);

    Q_strncpyz(buffer, Info_ValueForKey(info, "sv_hostname"), 1024);
    Q_CleanStr(buffer);

    UI_DrawTextBlock(rect, text_x, text_y, color, scale, textalign, textvalign,
                     textStyle, buffer);
}

/*
==============
idCGameDraw::DrawDemoPlayback
==============
*/
void idCGameDraw::DrawDemoPlayback(rectDef_t *rect, vec4_t color,
                                   qhandle_t shader) {
    if(!cg_drawDemoState.integer) {
        return;
    }

    trap_R_SetColor(color);
    idCGameDrawTools::DrawPic(rect->x, rect->y, rect->w, rect->h, shader);
    trap_R_SetColor(nullptr);
}

/*
==============
idCGameDraw::DrawDemoRecording
==============
*/
void idCGameDraw::DrawDemoRecording(rectDef_t *rect, vec4_t color,
                                    qhandle_t shader) {
    if(!cg_drawDemoState.integer) {
        return;
    }

    trap_R_SetColor(color);
    idCGameDrawTools::DrawPic(rect->x, rect->y, rect->w, rect->h, shader);
    trap_R_SetColor(nullptr);
}

/*
======================
idCGameDraw::UpdateMediaFraction
======================
*/
void idCGameDraw::UpdateMediaFraction(float32 newFract) {
    cg.mediaFraction = newFract;

    trap_UpdateScreen();
}

/*
====================
idCGameDraw::DrawLoadingScreen

Draw all the status / pacifier stuff during level loading
====================
*/
void idCGameDraw::DrawLoadingScreen(void) {
    Menu_Paint(Menus_FindByName("Loading"), true);
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
float32 idCGameDraw::GetValue(sint ownerDraw) {
    centity_t *cent;
    playerState_t *ps;
    weapon_t weapon;

    cent = &cg_entities[ cg.snap->ps.clientNum ];
    ps = &cg.snap->ps;
    weapon = bggame->GetPlayerWeapon(ps);

    switch(ownerDraw) {
        case CG_PLAYER_AMMO_VALUE:
            if(weapon) {
                return ps->ammo;
            }

            break;

        case CG_PLAYER_CLIPS_VALUE:
            if(weapon) {
                return ps->clips;
            }

            break;

        case CG_PLAYER_HEALTH:
            return ps->stats[ STAT_HEALTH ];
            break;

        default:
            break;
    }

    return -1;
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
pointer idCGameDraw::GetKillerText(void) {
    pointer s = "";

    if(cg.killerName[0]) {
        s = va("Fragged by %s", cg.killerName);
    }

    return s;
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
void idCGameDraw::DrawKiller(rectDef_t *rect, float32 scale, vec4_t color,
                             qhandle_t shader, sint textStyle) {
    // fragged by ... line
    if(cg.killerName[ 0 ]) {
        sint x = rect->x + rect->w / 2;
        UI_Text_Paint(x - UI_Text_Width(GetKillerText(), scale, 0) / 2,
                      rect->y + rect->h, scale, color, GetKillerText(), 0, 0, textStyle);
    }
}

#define SPECTATORS_PIXELS_PER_SECOND 30.0f

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
void idCGameDraw::DrawTeamSpectators(rectDef_t *rect, float32 scale,
                                     sint textvalign, vec4_t color, qhandle_t shader) {
    float y;
    char   *text = cg.spectatorList;
    float  textWidth = UI_Text_Width(text, scale, 0);
    vec4_t region;

    AlignText(rect, text, scale, 0.0f, 0.0f, ALIGN_LEFT, textvalign, nullptr,
              &y);

    if(textWidth > rect->w) {
        // The text is too wide to fit, so scroll it
        int now = trap_Milliseconds();
        int delta = now - cg.spectatorTime;

        region[0] = rect->x;
        region[1] = rect->y;
        region[2] = rect->x + rect->w;
        region[3] = rect->y + rect->h;
        trap_R_SetClipRegion(region);

        UI_Text_Paint(rect->x - cg.spectatorOffset, y, scale, color, text, 0, 0,
                      0);
        UI_Text_Paint(rect->x + textWidth - cg.spectatorOffset, y, scale, color,
                      text, 0, 0, 0);

        trap_R_SetClipRegion(nullptr);

        cg.spectatorOffset += (delta / 1000.0f) * SPECTATORS_PIXELS_PER_SECOND;

        while(cg.spectatorOffset > textWidth) {
            cg.spectatorOffset -= textWidth;
        }

        cg.spectatorTime = now;
    } else {
        UI_Text_Paint(rect->x, y, scale, color, text, 0, 0, 0);
    }
}

/*
==================
DrawTeamLabel
==================
*/
void idCGameDraw::DrawTeamLabel(rectDef_t *rect, team_t team,
                                float32 text_x, float32 text_y, vec4_t color, float32 scale,
                                sint textalign, sint textvalign, sint textStyle) {
    valueType *t;
    valueType stage[ MAX_TOKEN_CHARS ];
    valueType *s;
    float32 tx, ty;

    stage[ 0 ] = '\0';

    switch(team) {
        case TEAM_ALIENS:
            t = "Aliens";

            if(cg.intermissionStarted) {
                Q_vsprintf_s(stage, MAX_TOKEN_CHARS, MAX_TOKEN_CHARS, "(Stage %d)",
                             cgs.alienStage + 1);
            }

            break;

        case TEAM_HUMANS:
            t = "Humans";

            if(cg.intermissionStarted) {
                Q_vsprintf_s(stage, MAX_TOKEN_CHARS, MAX_TOKEN_CHARS, "(Stage %d)",
                             cgs.humanStage + 1);
            }

            break;

        default:
            t = "";
            break;
    }

    switch(textalign) {
        default:
        case ALIGN_LEFT:
            s = va("%s %s", t, stage);
            break;

        case ALIGN_RIGHT:
            s = va("%s %s", stage, t);
            break;
    }

    AlignText(rect, s, scale, 0.0f, 0.0f, textalign, textvalign, &tx, &ty);
    UI_Text_Paint(text_x + tx, text_y + ty, scale, color, s, 0, 0, textStyle);
}

/*
==================
DrawStageReport
==================
*/
void idCGameDraw::DrawStageReport(rectDef_t *rect, float32 text_x,
                                  float32 text_y, vec4_t color, float32 scale, sint textalign,
                                  sint textvalign, sint textStyle) {
    valueType  s[ MAX_TOKEN_CHARS ];
    valueType *reward;
    float32 tx, ty;

    if(cg.intermissionStarted) {
        return;
    }

    if(cg.snap->ps.stats[STAT_TEAM] == TEAM_NONE) {
        return;
    }

    if(cg.snap->ps.stats[ STAT_TEAM ] == TEAM_ALIENS) {
        sint frags = ceil((float32)(cgs.alienNextStageThreshold -
                                    cgs.alienCredits) / ALIEN_CREDITS_PER_FRAG);

        if(frags < 0) {
            frags = 0;
        }

        if(cgs.alienStage < S3) {
            reward = "next stage";
        } else {
            reward = "enemy stagedown";
        }

        if(cgs.alienNextStageThreshold < 0) {
            Q_vsprintf_s(s, MAX_TOKEN_CHARS, MAX_TOKEN_CHARS, "Stage %d",
                         cgs.alienStage + 1);
        } else if(frags == 1) {
            Q_vsprintf_s(s, MAX_TOKEN_CHARS, MAX_TOKEN_CHARS,
                         "Stage %d, 1 frag for %s", cgs.alienStage + 1, reward);
        } else {
            Q_vsprintf_s(s, MAX_TOKEN_CHARS, MAX_TOKEN_CHARS,
                         "Stage %d, %d frags for %s", cgs.alienStage + 1, frags, reward);
        }
    } else if(cg.snap->ps.stats[ STAT_TEAM ] == TEAM_HUMANS) {
        sint credits = cgs.humanNextStageThreshold - cgs.humanCredits;

        if(credits < 0) {
            credits = 0;
        }

        if(cgs.humanStage < S3) {
            reward = "next stage";
        } else {
            reward = "enemy stagedown";
        }

        if(cgs.humanNextStageThreshold < 0) {
            Q_vsprintf_s(s, MAX_TOKEN_CHARS, MAX_TOKEN_CHARS, "Stage %d",
                         cgs.humanStage + 1);
        } else if(credits == 1) {
            Q_vsprintf_s(s, MAX_TOKEN_CHARS, MAX_TOKEN_CHARS,
                         "Stage %d, 1 credit for %s", cgs.humanStage + 1, reward);
        } else {
            Q_vsprintf_s(s, MAX_TOKEN_CHARS, MAX_TOKEN_CHARS,
                         "Stage %d, %d credits for %s", cgs.humanStage + 1, credits, reward);
        }
    }

    AlignText(rect, s, scale, 0.0f, 0.0f, textalign, textvalign, &tx, &ty);

    UI_Text_Paint(text_x + tx, text_y + ty, scale, color, s, 0, 0, textStyle);
}

/*
==================
DrawFPS
==================
*/
#define FPS_FRAMES  20
#define FPS_STRING  "fps"
void idCGameDraw::DrawFPS(rectDef_t *rect, float32 text_x, float32 text_y,
                          float32 scale, vec4_t color, sint textalign, sint textvalign,
                          sint textStyle, bool scalableText) {
    valueType *s;
    float32 tx, ty, w, h, totalWidth;
    sint strLength;
    static sint previousTimes[ FPS_FRAMES ];
    static sint index;
    sint i, total, fps;
    static sint previous;
    sint t, frameTime;

    if(!cg_drawFPS.integer) {
        return;
    }

    // don't use serverTime, because that will be drifting to
    // correct for internet lag changes, timescales, timedemos, etc
    t = trap_Milliseconds();
    frameTime = t - previous;
    previous = t;

    previousTimes[ index % FPS_FRAMES ] = frameTime;
    index++;

    if(index > FPS_FRAMES) {
        // average multiple frames together to smooth changes out a bit
        total = 0;

        for(i = 0; i < FPS_FRAMES; i++) {
            total += previousTimes[i];
        }

        if(!total) {
            total = 1;
        }

        fps = 1000 * FPS_FRAMES / total;

        s = va("%d", fps);
        w = UI_Text_Width("0", scale, 0);
        h = UI_Text_Height("0", scale, 0);
        strLength = idCGameDrawTools::DrawStrlen(s);
        totalWidth = UI_Text_Width(FPS_STRING, scale, 0) + w * strLength;

        AlignText(rect, s, 0.0f, totalWidth, h, textalign, textvalign, &tx, &ty);

        if(scalableText) {
            for(i = 0; i < strLength; i++) {
                valueType c[ 2 ];

                c[ 0 ] = s[ i ];
                c[ 1 ] = '\0';

                UI_Text_Paint(text_x + tx + i * w, text_y + ty, scale, color, c, 0, 0,
                              textStyle);
            }

            UI_Text_Paint(text_x + tx + i * w, text_y + ty, scale, color, FPS_STRING,
                          0, 0, textStyle);
        } else {
            trap_R_SetColor(color);
            DrawField(rect->x, rect->y, 3, rect->w / 3, rect->h, fps);
            trap_R_SetColor(nullptr);
        }
    }
}

/*
==================
idCGameDraw::DrawSpeed
==================
*/
void idCGameDraw::DrawSpeed(rectDef_t *rect, float32 text_x,
                            float32 text_y, float32 scale, vec4_t color, sint textalign,
                            sint textvalign, sint textStyle) {
    valueType *s;
    float32 tx, ty, w, h, totalWidth, speed;
    sint i, strLength;
    playerState_t *ps;
    static float32  speedRecord = 0;
    static float32  previousSpeed = 0;
    static vec4_t previousColor = { 0, 1, 0, 1 };

    if(cg.snap->ps.pm_type == PM_INTERMISSION) {
        return;
    }

    if(!cg_drawSpeed.integer) {
        return;
    }

    if(cg_drawSpeed.integer > 1) {
        speedRecord = 0;
        trap_Cvar_Set("cg_drawSpeed", "1");
    }

    ps = &cg.snap->ps;
    speed = VectorLength(ps->velocity);

    if(speed > speedRecord) {
        speedRecord = speed;
    }

    if(floor(speed) > floor(previousSpeed)) {
        color[0] = 0;
        color[1] = 1;
        color[2] = 0;
        color[3] = 1;
    } else if(floor(speed) < floor(previousSpeed)) {
        color[0] = 1;
        color[1] = 0;
        color[2] = 0;
        color[3] = 1;
    } else {
        color[0] = previousColor[0];
        color[1] = previousColor[1];
        color[2] = previousColor[2];
        color[3] = previousColor[3];
    }

    previousColor[0] = color[0];
    previousColor[1] = color[1];
    previousColor[2] = color[2];
    previousColor[3] = color[3];

    previousSpeed = speed;

    s = va("Speed: %.0f/%.0f", speed, speedRecord);
    w = UI_Text_Width("0", scale, 0);
    h = UI_Text_Height("0", scale, 0);
    strLength = idCGameDrawTools::DrawStrlen(s);
    totalWidth = UI_Text_Width(FPS_STRING, scale, 0) + w * strLength;

    AlignText(rect, s, 0.0f, totalWidth, h, textalign, textvalign, &tx, &ty);

    for(i = 0; i < strLength; i++) {
        valueType c[ 2 ];

        c[ 0 ] = s[ i ];
        c[ 1 ] = '\0';

        UI_Text_Paint(text_x + tx + i * w, text_y + ty, scale, color, c, 0, 0,
                      textStyle);
    }
}

/*
=================
DrawTimerMins
=================
*/
void idCGameDraw::DrawTimerMins(rectDef_t *rect, vec4_t color) {
    sint mins, seconds, msec;

    if(!cg_drawTimer.integer) {
        return;
    }

    msec = cg.time - cgs.levelStartTime;

    seconds = msec / 1000;
    mins = seconds / 60;
    seconds -= mins * 60;

    trap_R_SetColor(color);
    DrawField(rect->x, rect->y, 3, rect->w / 3, rect->h, mins);
    trap_R_SetColor(nullptr);
}

/*
=================
DrawTimerSecs
=================
*/
void idCGameDraw::DrawTimerSecs(rectDef_t *rect, vec4_t color) {
    sint mins, seconds, msec;

    if(!cg_drawTimer.integer) {
        return;
    }

    msec = cg.time - cgs.levelStartTime;

    seconds = msec / 1000;
    mins = seconds / 60;
    seconds -= mins * 60;

    trap_R_SetColor(color);
    DrawFieldPadded(rect->x, rect->y, 2, rect->w / 2, rect->h, seconds);
    trap_R_SetColor(nullptr);
}

/*
=================
DrawTimer
=================
*/
void idCGameDraw::DrawTimer(rectDef_t *rect, float32 text_x,
                            float32 text_y, float32 scale, vec4_t color, sint textalign,
                            sint textvalign, sint textStyle) {
    valueType *s;
    float32 tx, ty, w, h, totalWidth;
    sint i, strLength, mins, seconds, tens, msec;

    if(!cg_drawTimer.integer) {
        return;
    }

    msec = cg.time - cgs.levelStartTime;

    seconds = msec / 1000;
    mins = seconds / 60;
    seconds -= mins * 60;
    tens = seconds / 10;
    seconds -= tens * 10;

    s = va("%d:%d%d", mins, tens, seconds);
    w = UI_Text_Width("0", scale, 0);
    h = UI_Text_Height("0", scale, 0);
    strLength = idCGameDrawTools::DrawStrlen(s);
    totalWidth = w * strLength;

    AlignText(rect, s, 0.0f, totalWidth, h, textalign, textvalign, &tx, &ty);

    for(i = 0; i < strLength; i++) {
        valueType c[ 2 ];

        c[ 0 ] = s[ i ];
        c[ 1 ] = '\0';

        UI_Text_Paint(text_x + tx + i * w, text_y + ty, scale, color, c, 0, 0,
                      textStyle);
    }
}

/*
=================
DrawClock
=================
*/
void idCGameDraw::DrawClock(rectDef_t *rect, float32 text_x,
                            float32 text_y, float32 scale, vec4_t color, sint textalign,
                            sint textvalign, sint textStyle) {
    valueType *s;
    float32 tx, ty, w, h, totalWidth;
    sint i, t, strLength;
    qtime_t qt;

    if(!cg_drawClock.integer) {
        return;
    }

    t = trap_RealTime(&qt);

    if(cg_drawClock.integer == 2) {
        s = va("%02d%s%02d", qt.tm_hour, (qt.tm_sec % 2) ? ":" : " ", qt.tm_min);
    } else {
        valueType *pm = "am";
        sint h = qt.tm_hour;

        if(h == 0) {
            h = 12;
        } else if(h == 12) {
            pm = "pm";
        } else if(h > 12) {
            h -= 12;
            pm = "pm";
        }

        s = va("%d%s%02d%s", h, (qt.tm_sec % 2) ? ":" : " ", qt.tm_min, pm);
    }

    w = UI_Text_Width("0", scale, 0);
    h = UI_Text_Height("0", scale, 0);
    strLength = idCGameDrawTools::DrawStrlen(s);
    totalWidth = w * strLength;

    AlignText(rect, s, 0.0f, totalWidth, h, textalign, textvalign, &tx, &ty);

    for(i = 0; i < strLength; i++) {
        valueType c[ 2 ];

        c[ 0 ] = s[ i ];
        c[ 1 ] = '\0';

        UI_Text_Paint(text_x + tx + i * w, text_y + ty, scale, color, c, 0, 0,
                      textStyle);
    }
}

/*
==================
idCGameDraw::DrawSnapshot
==================
*/
void idCGameDraw::DrawSnapshot(rectDef_t *rect, float32 text_x,
                               float32 text_y, float32 scale, vec4_t color, sint textalign,
                               sint textvalign, sint textStyle) {
    valueType    *s;
    float32   tx, ty;

    if(!cg_drawSnapshot.integer) {
        return;
    }

    s = va("time:%d snap:%d cmd:%d", cg.snap->serverTime, cg.latestSnapshotNum,
           cgs.serverCommandSequence);

    AlignText(rect, s, scale, 0.0f, 0.0f, textalign, textvalign, &tx, &ty);

    UI_Text_Paint(text_x + tx, text_y + ty, scale, color, s, 0, 0, textStyle);
}

/*
===============================================================================
LAGOMETER
===============================================================================
*/

#define LAG_SAMPLES   128

typedef struct {
    sint frameSamples[ LAG_SAMPLES ];
    sint frameCount;
    sint snapshotFlags[ LAG_SAMPLES ];
    sint snapshotSamples[ LAG_SAMPLES ];
    sint snapshotCount;
} lagometer_t;

lagometer_t lagometer;

/*
==============
idCGameDraw::AddLagometerFrameInfo

Adds the current interpolate / extrapolate bar for this frame
==============
*/
void idCGameDraw::AddLagometerFrameInfo(void) {
    sint offset;

    offset = cg.time - cg.latestSnapshotTime;
    lagometer.frameSamples[ lagometer.frameCount & (LAG_SAMPLES - 1) ] =
        offset;
    lagometer.frameCount++;
}

/*
==============
idCGameDraw::AddLagometerSnapshotInfo

Each time a snapshot is received, log its ping time and
the number of snapshots that were dropped before it.

Pass nullptr for a dropped packet.
==============
*/
#define PING_FRAMES 40
void idCGameDraw::AddLagometerSnapshotInfo(snapshot_t *snap) {
    static sint previousPings[ PING_FRAMES ];
    static sint index;
    sint i;

    // dropped packet
    if(!snap) {
        lagometer.snapshotSamples[ lagometer.snapshotCount &
                                                           (LAG_SAMPLES - 1) ] = -1;
        lagometer.snapshotCount++;
        return;
    }

    // add this snapshot's info
    lagometer.snapshotSamples[ lagometer.snapshotCount &
                                                       (LAG_SAMPLES - 1) ] = snap->ping;
    lagometer.snapshotFlags[ lagometer.snapshotCount & (LAG_SAMPLES - 1) ] =
        snap->snapFlags;
    lagometer.snapshotCount++;

    cg.ping = 0;

    if(cg.snap) {
        previousPings[ index++ ] = cg.snap->ping;
        index = index % PING_FRAMES;

        for(i = 0; i < PING_FRAMES; i++) {
            cg.ping += previousPings[ i ];
        }

        cg.ping /= PING_FRAMES;
    }
}

/*
==============
idCGameDraw::DrawDisconnect

Should we draw something differnet for long lag vs no packets?
==============
*/
void idCGameDraw::DrawDisconnect(void) {
    float32 x, y;
    sint cmdNum;
    usercmd_t cmd;
    pointer s;
    sint w;
    vec4_t color = { 1.0f, 1.0f, 1.0f, 1.0f };

    if(cg.serverRespawning) {
        return;
    }

    // draw the phone jack if we are completely past our buffers
    cmdNum = trap_GetCurrentCmdNumber() - CMD_BACKUP + 1;
    trap_GetUserCmd(cmdNum, &cmd);

    // special check for map_restart
    if(cmd.serverTime <= cg.snap->ps.commandTime || cmd.serverTime > cg.time) {
        return;
    }

    // also add text in center of screen
    s = "Connection Interrupted";
    w = UI_Text_Width(s, 0.7f, 0);
    UI_Text_Paint(320 - w / 2, 100, 0.7f, color, s, 0, 0,
                  ITEM_TEXTSTYLE_SHADOWED);

    // blink the icon
    if((cg.time >> 9) & 1) {
        return;
    }

    x = 640 - 48;
    y = 480 - 48;

    idCGameDrawTools::DrawPic(x, y, 48, 48,
                              trap_R_RegisterShader("gfx/2d/net.tga"));
}

/*
==============
idCGameDraw::DrawLagometer
==============
*/
#define MAX_LAGOMETER_PING  900
#define MAX_LAGOMETER_RANGE 300

void idCGameDraw::DrawLagometer(rectDef_t *rect, float32 text_x,
                                float32 text_y, float32 scale, vec4_t textColor) {
    sint a, x, y, i, color;
    float32 ax, ay, aw, ah, mid, range, v, vscale;
    vec4_t  adjustedColor;
    vec4_t  white = { 1.0f, 1.0f, 1.0f, 1.0f };

    if(cg.snap->ps.pm_type == PM_INTERMISSION) {
        return;
    }

    if(!cg_lagometer.integer) {
        return;
    }

    if(cg.demoPlayback) {
        return;
    }

    Vector4Copy(textColor, adjustedColor);
    adjustedColor[ 3 ] = 0.25f;

    trap_R_SetColor(adjustedColor);
    idCGameDrawTools::DrawPic(rect->x, rect->y, rect->w, rect->h,
                              cgs.media.whiteShader);
    trap_R_SetColor(nullptr);

    // draw the graph
    ax = x = rect->x;
    ay = y = rect->y;
    aw = rect->w;
    ah = rect->h;

    trap_R_SetColor(nullptr);

    idCGameDrawTools::AdjustFrom640(&ax, &ay, &aw, &ah);

    color = -1;
    range = ah / 3;
    mid = ay + range;

    vscale = range / MAX_LAGOMETER_RANGE;

    // draw the frame interpoalte / extrapolate graph
    for(a = 0 ; a < aw ; a++) {
        i = (lagometer.frameCount - 1 - a) & (LAG_SAMPLES - 1);
        v = lagometer.frameSamples[ i ];
        v *= vscale;

        if(v > 0) {
            if(color != 1) {
                color = 1;
                trap_R_SetColor(g_color_table[ ColorIndex(COLOR_YELLOW) ]);
            }

            if(v > range) {
                v = range;
            }

            trap_R_DrawStretchPic(ax + aw - a, mid - v, 1, v, 0, 0, 0, 0,
                                  cgs.media.whiteShader);
        } else if(v < 0) {
            if(color != 2) {
                color = 2;
                trap_R_SetColor(g_color_table[ ColorIndex(COLOR_BLUE) ]);
            }

            v = -v;

            if(v > range) {
                v = range;
            }

            trap_R_DrawStretchPic(ax + aw - a, mid, 1, v, 0, 0, 0, 0,
                                  cgs.media.whiteShader);
        }
    }

    // draw the snapshot latency / drop graph
    range = ah / 2;
    vscale = range / MAX_LAGOMETER_PING;

    for(a = 0 ; a < aw ; a++) {
        i = (lagometer.snapshotCount - 1 - a) & (LAG_SAMPLES - 1);
        v = lagometer.snapshotSamples[ i ];

        if(v > 0) {
            if(lagometer.snapshotFlags[ i ] & SNAPFLAG_RATE_DELAYED) {
                if(color != 5) {
                    color = 5;  // YELLOW for rate delay
                    trap_R_SetColor(g_color_table[ ColorIndex(COLOR_YELLOW) ]);
                }
            } else {
                if(color != 3) {
                    color = 3;

                    trap_R_SetColor(g_color_table[ ColorIndex(COLOR_GREEN) ]);
                }
            }

            v = v * vscale;

            if(v > range) {
                v = range;
            }

            trap_R_DrawStretchPic(ax + aw - a, ay + ah - v, 1, v, 0, 0, 0, 0,
                                  cgs.media.whiteShader);
        } else if(v < 0) {
            if(color != 4) {
                color = 4;    // RED for dropped snapshots
                trap_R_SetColor(g_color_table[ ColorIndex(COLOR_RED) ]);
            }

            trap_R_DrawStretchPic(ax + aw - a, ay + ah - range, 1, range, 0, 0, 0, 0,
                                  cgs.media.whiteShader);
        }
    }

    trap_R_SetColor(nullptr);

    if(cg_nopredict.integer || cg_synchronousClients.integer) {
        UI_Text_Paint(ax, ay, 0.5, white, "snc", 0, 0, ITEM_TEXTSTYLE_NORMAL);
    } else {
        valueType *s;

        s = va("%d", cg.ping);
        ax = rect->x + (rect->w / 2.0f) - (UI_Text_Width(s, scale,
                                           0) / 2.0f) + text_x;
        ay = rect->y + (rect->h / 2.0f) + (UI_Text_Height(s, scale,
                                           0) / 2.0f) + text_y;

        Vector4Copy(textColor, adjustedColor);
        adjustedColor[ 3 ] = 0.5f;
        UI_Text_Paint(ax, ay, scale, adjustedColor, s, 0, 0,
                      ITEM_TEXTSTYLE_NORMAL);
    }

    DrawDisconnect();
}

/*
===================
idCGameDraw::DrawConsole
===================
*/
void idCGameDraw::DrawConsole(rectDef_t *rect, float32 text_x,
                              float32 text_y, vec4_t color, float32 scale, sint textalign,
                              sint textvalign, sint textStyle) {
    UI_DrawTextBlock(rect, text_x, text_y, color, scale, textalign, textvalign,
                     textStyle, cg.consoleText);
}

/*
===================
idCGameDraw::DrawTutorial
===================
*/
void idCGameDraw::DrawTutorial(rectDef_t *rect, float32 text_x,
                               float32 text_y, vec4_t color, float32 scale, sint textalign,
                               sint textvalign, sint textStyle) {
    if(!cg_tutorial.integer) {
        return;
    }

    UI_DrawTextBlock(rect, text_x, text_y, color, scale, textalign, textvalign,
                     textStyle, idCGameTutorial::TutorialText());
}

/*
===================
idCGameDraw::DrawWeaponIcon
===================
*/
void idCGameDraw::DrawWeaponIcon(rectDef_t *rect, vec4_t color) {
    centity_t *cent;
    playerState_t *ps;
    weapon_t weapon;

    cent = &cg_entities[ cg.snap->ps.clientNum ];
    ps = &cg.snap->ps;
    weapon = bggame->GetPlayerWeapon(ps);

    // don't display if dead
    if(cg.predictedPlayerState.stats[STAT_HEALTH] <= 0) {
        return;
    }

    if(weapon == 0) {
        return;
    }

    idCGameWeapons::RegisterWeapon(weapon);

    if(ps->clips == 0 && !bggame->Weapon(weapon)->infiniteAmmo) {
        float32 ammoPercent = (float32)ps->ammo / (float32)bggame->Weapon(
                                  weapon)->maxAmmo;

        if(ammoPercent < 0.33f) {
            color[ 0 ] = 1.0f;
            color[ 1 ] = color[ 2 ] = 0.0f;
        }
    }

    if(cg.predictedPlayerState.stats[ STAT_TEAM ] == TEAM_ALIENS &&
            !bggame->AlienCanEvolve((class_t)
                                    cg.predictedPlayerState.stats[ STAT_CLASS ],
                                    ps->persistant[ PERS_CREDIT ], cgs.alienStage)) {
        if(cg.time - cg.lastEvolveAttempt <= NO_CREDITS_TIME) {
            if(((cg.time - cg.lastEvolveAttempt) / 300) % 2) {
                color[3] = 0.0f;
            }
        }
    }

    trap_R_SetColor(color);
    idCGameDrawTools::DrawPic(rect->x, rect->y, rect->w, rect->h,
                              cg_weapons[ weapon ].weaponIcon);
    trap_R_SetColor(nullptr);
}

/*
================================================================================
CROSSHAIR
================================================================================
*/

/*
=================
idCGameDraw::DrawCrosshair
=================
*/
void idCGameDraw::DrawCrosshair(rectDef_t *rect, vec4_t color) {
    float32 w, h, x, y;
    qhandle_t     hShader;
    weaponInfo_t *wi;
    weapon_t weapon;

    weapon = bggame->GetPlayerWeapon(&cg.snap->ps);

    if(cg_drawCrosshair.integer == CROSSHAIR_ALWAYSOFF) {
        return;
    }

    if(cg_drawCrosshair.integer == CROSSHAIR_RANGEDONLY &&
            !bggame->Weapon(weapon)->longRanged) {
        return;
    }

    if((cg.snap->ps.persistant[PERS_SPECSTATE] != SPECTATOR_NOT) ||
            (cg.snap->ps.stats[STAT_STATE] & SS_HOVELING)) {
        return;
    }

    if(cg.renderingThirdPerson) {
        return;
    }

    wi = &cg_weapons[ weapon ];

    w = h = wi->crossHairSize * cg_crosshairSize.value;
    w *= cgDC.aspectScale;

    //FIXME: this still ignores the width/height of the rect, but at least it's
    //neater than cg_crosshairX/cg_crosshairY
    x = rect->x + (rect->w / 2) - (w / 2);
    y = rect->y + (rect->h / 2) - (h / 2);

    hShader = wi->crossHair;

    //aiming at a friendly player/buildable, dim the crosshair
    if(cg.time == cg.crosshairClientTime || cg.crosshairBuildable >= 0) {
        sint i;

        for(i = 0; i < 3; i++) {
            color[i] *= .5f;
        }
    }

    if(hShader != 0) {
        trap_R_SetColor(color);
        idCGameDrawTools::DrawPic(x, y, w, h, hShader);
        trap_R_SetColor(nullptr);
    }
}

/*
=================
idCGameDraw::ScanForCrosshairEntity
=================
*/
void idCGameDraw::ScanForCrosshairEntity(float32 *zChange,
        bool *hitClient) {
    trace_t trace;
    vec3_t start, end;
    sint content;
    team_t team;

    // We haven't hit a client yet
    *hitClient = false;

    VectorCopy(cg.refdef.vieworg, start);
    VectorMA(start, 131072, cg.refdef.viewaxis[ 0 ], end);

    idCGamePredict::Trace(&trace, start, vec3_origin, vec3_origin, end,
                          cg.snap->ps.clientNum, CONTENTS_SOLID | CONTENTS_BODY);

    // if the player is in fog, don't show it
    content = trap_CM_PointContents(trace.endpos, 0);

    if(content & CONTENTS_FOG) {
        return;
    }

    // How far up or down are we looking?
    *zChange = trace.endpos[2] - start[2];

    if(trace.entityNum >= MAX_CLIENTS) {
        entityState_t *s = &cg_entities[trace.entityNum].currentState;

        if(s->eType == ET_BUILDABLE &&
                bggame->Buildable((buildable_t)s->modelindex)->team ==
                cg.snap->ps.stats[STAT_TEAM]) {
            cg.crosshairBuildable = trace.entityNum;
        } else {
            cg.crosshairBuildable = -1;
        }

        return;
    }

    team = cgs.clientinfo[ trace.entityNum ].team;

    if(cg.snap->ps.persistant[ PERS_SPECSTATE ] == SPECTATOR_NOT) {
        //only display team names of those on the same team as this player
        if(team != cg.snap->ps.stats[STAT_TEAM]) {
            return;
        }
    }

    // update the fade timer
    cg.crosshairClientNum = trace.entityNum;
    cg.crosshairClientTime = cg.time;

    if(cg.crosshairClientNum != cg.snap->ps.identifyClient &&
            cg.crosshairClientNum != ENTITYNUM_WORLD) {
        cg.identifyClientRequest = cg.crosshairClientNum;
    }
}

/*
=====================
idCGameDraw::DrawLocation
=====================
*/
void idCGameDraw::DrawLocation(rectDef_t *rect, float32 scale,
                               sint textalign, vec4_t color) {
    pointer location;
    centity_t *locent;
    float32 maxX;
    float32 tx = rect->x, ty = rect->y;
    maxX = rect->x + rect->w;

    locent = idCGamePlayers::GetPlayerLocation();

    if(locent) {
        location = idCGameMain::ConfigString(CS_LOCATIONS +
                                             locent->currentState.generic1);
    } else {
        location = idCGameMain::ConfigString(CS_LOCATIONS);
    }

    if(UI_Text_Width(location, scale, 0) < rect->w) {
        AlignText(rect, location, scale, 0.0f, 0.0f, textalign, VALIGN_CENTER, &tx,
                  &ty);
    }

    UI_Text_Paint_Limit(&maxX, tx, ty, scale, color, location, 0, 0);
    trap_R_SetColor(nullptr);
}

/*
=====================
idCGameDraw::DrawCrosshairNames
=====================
*/
void idCGameDraw::DrawCrosshairNames(rectDef_t *rect, float32 scale,
                                     sint textStyle) {
    float32 *color, w, x, zChange;
    valueType *name;
    bool hitClient = false;

    if(!cg_drawCrosshairNames.integer) {
        return;
    }

    if(cg.renderingThirdPerson) {
        return;
    }

    // scan the known entities to see if the crosshair is sighted on one
    ScanForCrosshairEntity(&zChange, &hitClient);

    // draw the name of the player being looked at
    color = idCGameDrawTools::FadeColor(cg.crosshairClientTime,
                                        CROSSHAIR_CLIENT_TIMEOUT);

    if(!color) {
        trap_R_SetColor(nullptr);
        return;
    }

    name = cgs.clientinfo[ cg.crosshairClientNum ].name;
    w = UI_Text_Width(name, scale, 0);
    x = rect->x + rect->w / 2;
    UI_Text_Paint(x - w / 2, rect->y + rect->h, scale, color, name, 0, 0,
                  textStyle);
    trap_R_SetColor(nullptr);
}

/*
===============
idCGameDraw::DrawSquadMarkers
===============
*/
#define SQUAD_MARKER_W        16.f
#define SQUAD_MARKER_H        8.f
#define SQUAD_MARKER_BORDER   8.f
void idCGameDraw::DrawSquadMarkers(vec4_t color) {
    centity_t *cent;
    vec3_t origin;
    qhandle_t shader;
    float32 x, y, w, h, distance, scale, u1 = 0.f, v1 = 0.f, u2 = 1.f,
                                         v2 = 1.f;
    sint i;
    bool vertical, flip;

    if(cg.snap->ps.persistant[PERS_SPECSTATE] != SPECTATOR_NOT) {
        return;
    }

    trap_R_SetColor(color);

    for(i = 0; i < cg.snap->numEntities; i++) {
        cent = cg_entities + cg.snap->entities[ i ].number;

        if(cent->currentState.eType != ET_PLAYER ||
                cgs.clientinfo[cg.snap->entities[i].number].team !=
                cg.snap->ps.stats[STAT_TEAM] || !cent->pe.squadMarked) {
            continue;
        }

        // Find where on screen the player is
        VectorCopy(cent->lerpOrigin, origin);
        origin[ 2 ] += ((cent->currentState.solid >> 16) & 255) - 30;

        if(!idCGameDrawTools::WorldToScreenWrap(origin, &x, &y)) {
            continue;
        }

        // Scale the size of the marker with distance
        distance = Distance(cent->lerpOrigin, cg.refdef.vieworg);

        if(!distance) {
            continue;
        }

        scale = 200.f / distance;

        if(scale > 1.f) {
            scale = 1.f;
        }

        if(scale < 0.25f) {
            scale = 0.25f;
        }

        // Don't let the marker go off-screen
        vertical = false;
        flip = false;

        if(x < SQUAD_MARKER_BORDER) {
            x = SQUAD_MARKER_BORDER;
            vertical = true;
            flip = false;
        }

        if(x > 640.f - SQUAD_MARKER_BORDER) {
            x = 640.f - SQUAD_MARKER_BORDER;
            vertical = true;
            flip = true;
        }

        if(y < SQUAD_MARKER_BORDER) {
            y = SQUAD_MARKER_BORDER;
            vertical = false;
            flip = true;
        }

        if(y > 480.f - SQUAD_MARKER_BORDER) {
            y = 480.f - SQUAD_MARKER_BORDER;
            vertical = false;
            flip = false;
        }

        // Draw the marker
        if(vertical) {
            shader = cgs.media.squadMarkerV;

            if(flip) {
                u1 = 1.f;
                u2 = 0.f;
            }

            w = SQUAD_MARKER_H * scale;
            h = SQUAD_MARKER_W * scale;
        } else {
            shader = cgs.media.squadMarkerH;

            if(flip) {
                v1 = 1.f;
                v2 = 0.f;
            }

            w = SQUAD_MARKER_W * scale;
            h = SQUAD_MARKER_H * scale;
        }

        idCGameDrawTools::AdjustFrom640(&x, &y, &w, &h);
        trap_R_DrawStretchPic(x - w / 2, y - h / 2, w, h, u1, v1, u2, v2, shader);
    }

    trap_R_SetColor(nullptr);
}

/*
===============
idCGameDraw::OwnerDraw

Draw an owner drawn item
===============
*/
void idCGameDraw::OwnerDraw(float32 x, float32 y, float32 w, float32 h,
                            float32 text_x, float32 text_y, sint ownerDraw, sint ownerDrawFlags,
                            sint align, sint textalign, sint textvalign, float32 special,
                            float32 scale, vec4_t foreColor, vec4_t backColor, qhandle_t shader,
                            sint textStyle) {
    rectDef_t rect;

    if(cg_drawStatus.integer == 0) {
        return;
    }

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    switch(ownerDraw) {
        case CG_PLAYER_CREDITS_VALUE:
            DrawPlayerCreditsValue(&rect, foreColor, true);
            break;

        case CG_PLAYER_CREDITS_VALUE_NOPAD:
            DrawPlayerCreditsValue(&rect, foreColor, false);
            break;

        case CG_PLAYER_STAMINA_1:
        case CG_PLAYER_STAMINA_2:
        case CG_PLAYER_STAMINA_3:
        case CG_PLAYER_STAMINA_4:
            DrawPlayerStamina(ownerDraw, &rect, backColor, foreColor, shader);
            break;

        case CG_PLAYER_STAMINA_BOLT:
            DrawPlayerStaminaBolt(&rect, backColor, foreColor, shader);
            break;

        case CG_PLAYER_AMMO_VALUE:
            DrawPlayerAmmoValue(&rect, foreColor);
            break;

        case CG_PLAYER_AMMO_STACK:
            DrawPlayerAmmoStack(&rect, backColor, foreColor, textalign, textvalign);
            break;

        case CG_PLAYER_CLIPS_VALUE:
            DrawPlayerClipsValue(&rect, foreColor);
            break;

        case CG_PLAYER_CLIPS_STACK:
            DrawPlayerClipsStack(&rect, backColor, foreColor, textalign, textvalign);
            break;

        case CG_PLAYER_BUILD_TIMER:
            DrawPlayerBuildTimer(&rect, foreColor);
            break;

        case CG_PLAYER_HEALTH:
            DrawPlayerHealthValue(&rect, foreColor);
            break;

        case CG_PLAYER_HEALTH_CROSS:
            DrawPlayerHealthCross(&rect, foreColor);
            break;

        case CG_PLAYER_CHARGE_BAR_BG:
            DrawPlayerChargeBarBG(&rect, foreColor, shader);
            break;

        case CG_PLAYER_CHARGE_BAR:
            DrawPlayerChargeBar(&rect, foreColor, shader);
            break;

        case CG_PLAYER_CLIPS_RING:
            DrawPlayerClipsRing(&rect, backColor, foreColor, shader);
            break;

        case CG_PLAYER_BUILD_TIMER_RING:
            DrawPlayerBuildTimerRing(&rect, backColor, foreColor, shader);
            break;

        case CG_PLAYER_WALLCLIMBING:
            DrawPlayerWallclimbing(&rect, backColor, foreColor, shader);
            break;

        case CG_PLAYER_BOOSTED:
            DrawPlayerBoosted(&rect, backColor, foreColor, shader);
            break;

        case CG_PLAYER_BOOST_BOLT:
            DrawPlayerBoosterBolt(&rect, backColor, foreColor, shader);
            break;

        case CG_PLAYER_POISON_BARBS:
            DrawPlayerPoisonBarbs(&rect, foreColor, shader);
            break;

        case CG_PLAYER_ALIEN_SENSE:
            DrawAlienSense(&rect);
            break;

        case CG_PLAYER_HUMAN_SCANNER:
            DrawHumanScanner(&rect, shader, foreColor);
            break;

        case CG_PLAYER_USABLE_BUILDABLE:
            DrawUsableBuildable(&rect, shader, foreColor);
            break;

        case CG_KILLER:
            DrawKiller(&rect, scale, foreColor, shader, textStyle);
            break;

        case CG_PLAYER_SELECT:
            idCGameWeapons::DrawItemSelect(&rect, foreColor);
            break;

        case CG_PLAYER_WEAPONICON:
            DrawWeaponIcon(&rect, foreColor);
            break;

        case CG_PLAYER_ATTACK_FEEDBACK:
            DrawAttackFeedback(&rect);
            break;

        case CG_PLAYER_SELECTTEXT:
            idCGameWeapons::DrawItemSelectText(&rect, scale, textStyle);
            break;

        case CG_SPECTATORS:
            DrawTeamSpectators(&rect, scale, textvalign, foreColor, shader);
            break;

        case CG_PLAYER_LOCATION:
            DrawLocation(&rect, scale, textalign, foreColor);
            break;

        case CG_PLAYER_CROSSHAIRNAMES:
            DrawCrosshairNames(&rect, scale, textStyle);
            break;

        case CG_PLAYER_CROSSHAIR:
            DrawCrosshair(&rect, foreColor);
            break;

        case CG_STAGE_REPORT_TEXT:
            DrawStageReport(&rect, text_x, text_y, foreColor, scale, textalign,
                            textvalign, textStyle);
            break;

        case CG_ALIENS_SCORE_LABEL:
            DrawTeamLabel(&rect, TEAM_ALIENS, text_x, text_y, foreColor, scale,
                          textalign, textvalign, textStyle);
            break;

        case CG_HUMANS_SCORE_LABEL:
            DrawTeamLabel(&rect, TEAM_HUMANS, text_x, text_y, foreColor, scale,
                          textalign, textvalign, textStyle);
            break;

        case CG_SQUAD_MARKERS:
            DrawSquadMarkers(foreColor);
            break;

        //loading screen
        case CG_LOAD_LEVELSHOT:
            DrawLevelShot(&rect);
            break;

        case CG_LOAD_MEDIA:
            DrawMediaProgress(&rect, foreColor, scale, align, textalign, textStyle,
                              special);
            break;

        case CG_LOAD_MEDIA_LABEL:
            DrawMediaProgressLabel(&rect, text_x, text_y, foreColor, scale, textalign,
                                   textvalign);
            break;

        case CG_LOAD_BUILDABLES:
            DrawBuildablesProgress(&rect, foreColor, scale, align, textalign,
                                   textStyle, special);
            break;

        case CG_LOAD_BUILDABLES_LABEL:
            DrawBuildablesProgressLabel(&rect, text_x, text_y, foreColor, scale,
                                        textalign, textvalign);
            break;

        case CG_LOAD_CHARMODEL:
            DrawCharModelProgress(&rect, foreColor, scale, align, textalign, textStyle,
                                  special);
            break;

        case CG_LOAD_CHARMODEL_LABEL:
            DrawCharModelProgressLabel(&rect, text_x, text_y, foreColor, scale,
                                       textalign, textvalign);
            break;

        case CG_LOAD_OVERALL:
            DrawOverallProgress(&rect, foreColor, scale, align, textalign, textStyle,
                                special);
            break;

        case CG_LOAD_LEVELNAME:
            DrawLevelName(&rect, text_x, text_y, foreColor, scale, textalign,
                          textvalign, textStyle);
            break;

        case CG_LOAD_MOTD:
            DrawMOTD(&rect, text_x, text_y, foreColor, scale, textalign, textvalign,
                     textStyle);
            break;

        case CG_LOAD_HOSTNAME:
            DrawHostname(&rect, text_x, text_y, foreColor, scale, textalign,
                         textvalign, textStyle);
            break;

        case CG_FPS:
            DrawFPS(&rect, text_x, text_y, scale, foreColor, textalign, textvalign,
                    textStyle, true);
            break;

        case CG_FPS_FIXED:
            DrawFPS(&rect, text_x, text_y, scale, foreColor, textalign, textvalign,
                    textStyle, false);
            break;

        case CG_TIMER:
            DrawTimer(&rect, text_x, text_y, scale, foreColor, textalign, textvalign,
                      textStyle);
            break;

        case CG_CLOCK:
            DrawClock(&rect, text_x, text_y, scale, foreColor, textalign, textvalign,
                      textStyle);
            break;

        case CG_SPEED:
            DrawSpeed(&rect, text_x, text_y, scale, foreColor, textalign, textvalign,
                      textStyle);
            break;

        case CG_TIMER_MINS:
            DrawTimerMins(&rect, foreColor);
            break;

        case CG_TIMER_SECS:
            DrawTimerSecs(&rect, foreColor);
            break;

        case CG_SNAPSHOT:
            DrawSnapshot(&rect, text_x, text_y, scale, foreColor, textalign,
                         textvalign, textStyle);
            break;

        case CG_LAGOMETER:
            DrawLagometer(&rect, text_x, text_y, scale, foreColor);
            break;

        case CG_DEMO_PLAYBACK:
            DrawDemoPlayback(&rect, foreColor, shader);
            break;

        case CG_DEMO_RECORDING:
            DrawDemoRecording(&rect, foreColor, shader);
            break;

        case CG_CONSOLE:
            DrawConsole(&rect, text_x, text_y, foreColor, scale, textalign, textvalign,
                        textStyle);
            break;

        case CG_TUTORIAL:
            DrawTutorial(&rect, text_x, text_y, foreColor, scale, textalign,
                         textvalign, textStyle);
            break;

        default:
            break;
    }
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
void idCGameLocal::MouseEvent(sint x, sint y) {
    sint n;

    //Dushan - stupid so much
    cgDC.cursorx = cgs.cursorX;
    cgDC.cursory = cgs.cursorY;

    if((cg.predictedPlayerState.pm_type == PM_NORMAL ||
            cg.predictedPlayerState.pm_type == PM_SPECTATOR) &&
            cg.showScores == false) {
        trap_Key_SetCatcher(0);
        return;
    }

    cgs.cursorX += (x * cgDC.aspectScale);

    if(cgs.cursorX < 0) {
        cgs.cursorX = 0;
    } else if(cgs.cursorX > SCREEN_WIDTH) {
        cgs.cursorX = SCREEN_WIDTH;
    }

    cgs.cursorY += y;

    if(cgs.cursorY < 0) {
        cgs.cursorY = 0;
    } else if(cgs.cursorY > SCREEN_HEIGHT) {
        cgs.cursorY = SCREEN_HEIGHT;
    }

    n = Display_CursorType(cgs.cursorX, cgs.cursorY);
    cgs.activeCursor = 0;

    if(n == CURSOR_ARROW) {
        cgs.activeCursor = cgs.media.selectCursor;
    } else if(n == CURSOR_SIZER) {
        cgs.activeCursor = cgs.media.sizeCursor;
    }

    cgDC.cursordx = x;
    cgDC.cursordy = y;
    cgDC.cursorx = cgs.cursorX;
    cgDC.cursory = cgs.cursorY;

    if(cgs.capturedItem) {
        Display_MouseMove(cgs.capturedItem, x, y);
    } else {
        Display_MouseMove(nullptr, cgs.cursorX, cgs.cursorY);
    }
}

/*
==================
idCGameDraw::HideTeamMenus
==================
*/
void idCGameDraw::HideTeamMenu(void) {
    Menus_CloseByName("teamMenu");
    Menus_CloseByName("getMenu");
}

/*
==================
idCGameDraw::ShowTeamMenus
==================
*/
void idCGameDraw::ShowTeamMenu(void) {
    Menus_ActivateByName("teamMenu");
}

/*
==================
idCGameLocal::EventHandling
==================
 type 0 - no event handling
      1 - team menu
      2 - hud editor
*/
void idCGameLocal::EventHandling(sint type, bool fForced) {
    cgs.eventHandling = (bool)type;

    if(type == CGAME_EVENT_NONE) {
        idCGameDraw::HideTeamMenu();
    }
}

/*
===============
idCGameLocal::DrawAttackFeedback
===============
*/
void idCGameLocal::KeyEvent(sint key, bool down) {
    if(!down) {
        return;
    }

    if(cg.predictedPlayerState.pm_type == PM_NORMAL ||
            (cg.predictedPlayerState.pm_type == PM_SPECTATOR &&
             cg.showScores == false)) {
        cgameLocal.EventHandling(CGAME_EVENT_NONE, false);
        trap_Key_SetCatcher(0);
        return;
    }

    Display_HandleKey(key, down, cgs.cursorX, cgs.cursorY);

    if(cgs.capturedItem) {
        cgs.capturedItem = nullptr;
    } else {
        if(key == K_MOUSE2 && down) {
            cgs.capturedItem = Display_CaptureItem(cgs.cursorX, cgs.cursorY);
        }
    }
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
sint idCGameDraw::ClientNumFromName(pointer p) {
    sint i;

    for(i = 0; i < cgs.maxclients; i++) {
        if(cgs.clientinfo[i].infoValid &&
                Q_stricmp(cgs.clientinfo[i].name, p) == 0) {
            return i;
        }
    }

    return -1;
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
void idCGameDraw::RunMenuScript(valueType **args) {
}

/*
================
DrawLighting
================
*/
void idCGameDraw::DrawLighting(void) {
    centity_t *cent;

    cent = &cg_entities[ cg.snap->ps.clientNum ];

    //fade to black if stamina is low
    if((cg.snap->ps.stats[ STAT_STAMINA ] < -800) &&
            (cg.snap->ps.stats[ STAT_TEAM ] == TEAM_HUMANS)) {
        vec4_t black = { 0, 0, 0, 0 };
        black[ 3 ] = 1.0 - ((float32)(cg.snap->ps.stats[ STAT_STAMINA ] + 1000) /
                            200.0f);
        trap_R_SetColor(black);
        idCGameDrawTools::DrawPic(0, 0, 640, 480, cgs.media.whiteShader);
        trap_R_SetColor(nullptr);
    }
}

/*
===============================================================================
CENTER PRINTING
===============================================================================
*/

/*
==============
CenterPrint

Called for important messages that should stay in the center of the screen
for a few moments
==============
*/
void idCGameDraw::CenterPrint(pointer str, sint y, sint charWidth) {
    valueType *s;

    Q_strncpyz(cg.centerPrint, str, sizeof(cg.centerPrint));

    cg.centerPrintTime = cg.time;
    cg.centerPrintY = y;
    cg.centerPrintCharWidth = charWidth;

    // count the number of lines for centering
    cg.centerPrintLines = 1;
    s = cg.centerPrint;

    while(*s) {
        if(*s == '\n') {
            cg.centerPrintLines++;
        }

        s++;
    }
}

/*
===================
DrawCenterString
===================
*/
void idCGameDraw::DrawCenterString(void) {
    valueType *start;
    sint l, x, y, w, h;
    float32 *color;

    if(!cg.centerPrintTime) {
        return;
    }

    color = idCGameDrawTools::FadeColor(cg.centerPrintTime,
                                        1000 * cg_centertime.value);

    if(!color) {
        return;
    }

    trap_R_SetColor(color);

    start = cg.centerPrint;

    y = cg.centerPrintY - cg.centerPrintLines * BIGCHAR_HEIGHT / 2;

    while(1) {
        valueType linebuffer[ 1024 ];

        for(l = 0; l < 50; l++) {
            if(!start[l] || start[l] == '\n') {
                break;
            }

            linebuffer[ l ] = start[ l ];
        }

        linebuffer[ l ] = 0;

        w = UI_Text_Width(linebuffer, 0.5, 0);
        h = UI_Text_Height(linebuffer, 0.5, 0);
        x = (SCREEN_WIDTH - w) / 2;

        UI_Text_Paint(x, y + h, 0.5, color, linebuffer, 0, 0,
                      ITEM_TEXTSTYLE_SHADOWEDMORE);

        y += h + 6;

        while(*start && (*start != '\n')) {
            start++;
        }

        if(!*start) {
            break;
        }

        start++;
    }

    trap_R_SetColor(nullptr);
}

//==============================================================================

/*
=================
idCGameDraw::DrawVote

FIXME: both vote notes are hardcoded, change to ownerdrawn?
=================
*/
void idCGameDraw::DrawVote(void) {
    valueType *s;
    sint sec;
    vec4_t white = { 1.0f, 1.0f, 1.0f, 1.0f };
    valueType yeskey[ 32 ], nokey[ 32 ];

    if(!cgs.voteTime) {
        return;
    }

    // play a talk beep whenever it is modified
    if(cgs.voteModified) {
        cgs.voteModified = false;
        trap_S_StartLocalSound(cgs.media.talkSound, CHAN_LOCAL_SOUND);
    }

    sec = (VOTE_TIME - (cg.time - cgs.voteTime)) / 1000;

    if(sec < 0) {
        sec = 0;
    }

    Q_strncpyz(yeskey, idCGameDrawTools::KeyBinding("vote yes"),
               sizeof(yeskey));
    Q_strncpyz(nokey, idCGameDrawTools::KeyBinding("vote no"), sizeof(nokey));

    s = va("VOTE(%i): \"%s\"  [%s]Yes:%i [%s]No:%i", sec, cgs.voteString,
           yeskey, cgs.voteYes, nokey, cgs.voteNo);

    UI_Text_Paint(8, 340, 0.3f, white, s, 0, 0, ITEM_TEXTSTYLE_NORMAL);
}

/*
=================
idCGameDraw::DrawTeamVote
=================
*/
void idCGameDraw::DrawTeamVote(void) {
    valueType *s;
    sint sec, cs_offset;
    vec4_t white = { 1.0f, 1.0f, 1.0f, 1.0f };
    valueType yeskey[ 32 ], nokey[ 32 ];

    if(cg.predictedPlayerState.stats[STAT_TEAM] == TEAM_HUMANS) {
        cs_offset = 0;
    } else if(cg.predictedPlayerState.stats[STAT_TEAM] == TEAM_ALIENS) {
        cs_offset = 1;
    } else {
        return;
    }

    if(!cgs.teamVoteTime[cs_offset]) {
        return;
    }

    // play a talk beep whenever it is modified
    if(cgs.teamVoteModified[ cs_offset ]) {
        cgs.teamVoteModified[ cs_offset ] = false;
        trap_S_StartLocalSound(cgs.media.talkSound, CHAN_LOCAL_SOUND);
    }

    sec = (VOTE_TIME - (cg.time - cgs.teamVoteTime[ cs_offset ])) / 1000;

    if(sec < 0) {
        sec = 0;
    }

    Q_strncpyz(yeskey, idCGameDrawTools::KeyBinding("teamvote yes"),
               sizeof(yeskey));
    Q_strncpyz(nokey, idCGameDrawTools::KeyBinding("teamvote no"),
               sizeof(nokey));

    s = va("TEAMVOTE(%i): \"%s\"  [%s]Yes:%i   [%s]No:%i", sec,
           cgs.teamVoteString[ cs_offset ], yeskey, cgs.teamVoteYes[cs_offset], nokey,
           cgs.teamVoteNo[ cs_offset ]);

    UI_Text_Paint(8, 360, 0.3f, white, s, 0, 0, ITEM_TEXTSTYLE_NORMAL);
}

/*
===============
idCGameDraw::DrawAttackFeedback
===============
*/
bool idCGameDraw::DrawScoreboard(void) {
    static bool firstTime = true;
    float32 fade, *fadeColor;

    if(menuScoreboard) {
        menuScoreboard->window.flags &= ~WINDOW_FORCED;
    }

    if(cg_paused.integer) {
        cg.deferredPlayerLoading = 0;
        firstTime = true;
        return false;
    }

    if(cg.showScores || cg.predictedPlayerState.pm_type == PM_INTERMISSION) {
        fade = 1.0;
        fadeColor = colorWhite;
    } else {
        cg.deferredPlayerLoading = 0;
        cg.killerName[ 0 ] = 0;
        firstTime = true;
        return false;
    }

    if(menuScoreboard == nullptr) {
        menuScoreboard = Menus_FindByName("teamscore_menu");
    }

    if(menuScoreboard) {
        if(firstTime) {
            idCGameMain::SetScoreSelection(menuScoreboard);
            firstTime = false;
        }

        Menu_Paint(menuScoreboard, true);
    }

    return true;
}

/*
=================
idCGameDraw::DrawIntermission
=================
*/
void idCGameDraw::DrawIntermission(void) {
    if(cg_drawStatus.integer) {
        Menu_Paint(Menus_FindByName("default_hud"), true);
    }

    cg.scoreFadeTime = cg.time;
    cg.scoreBoardShowing = DrawScoreboard();
}


/*
=================
idCGameDraw::DrawFollow
=================
*/
#define FOLLOWING_STRING "Following: "
#define CHASING_STRING "Chasing: "
bool idCGameDraw::DrawFollow(void) {
    float32 w;
    vec4_t color;
    valueType buffer[ MAX_STRING_CHARS ];

    if(cg.snap->ps.clientNum == cg.clientNum) {
        return false;
    }

    color[ 0 ] = 1;
    color[ 1 ] = 1;
    color[ 2 ] = 1;
    color[ 3 ] = 1;

    if(!cg.chaseFollow) {
        strcpy(buffer, FOLLOWING_STRING);
    } else {
        strcpy(buffer, CHASING_STRING);
    }

    strcat(buffer, cgs.clientinfo[ cg.snap->ps.clientNum ].name);

    w = UI_Text_Width(buffer, 0.7f, 0);
    UI_Text_Paint(320 - w / 2, 400, 0.7f, color, buffer, 0, 0,
                  ITEM_TEXTSTYLE_SHADOWED);

    return true;
}

/*
=================
idCGameDraw::DrawQueue
=================
*/
bool idCGameDraw::DrawQueue(void) {
    float32 w;
    vec4_t color;
    sint position, remainder;
    valueType *ordinal, buffer[ MAX_STRING_CHARS ];

    if(!(cg.snap->ps.pm_flags & PMF_QUEUED)) {
        return false;
    }

    color[ 0 ] = 1;
    color[ 1 ] = 1;
    color[ 2 ] = 1;
    color[ 3 ] = 1;

    position = cg.snap->ps.persistant[ PERS_QUEUEPOS ] + 1;

    if(position < 1) {
        return false;
    }

    remainder = position % 10;
    ordinal = "th";

    if(remainder == 1) {
        ordinal = "st";
    } else if(remainder == 2) {
        ordinal = "nd";
    } else if(remainder == 3) {
        ordinal = "rd";
    }

    Q_vsprintf_s(buffer, MAX_STRING_CHARS, MAX_STRING_CHARS,
                 "You are %d%s in the spawn queue", position, ordinal);

    w = UI_Text_Width(buffer, 0.7f, 0);
    UI_Text_Paint(320 - w / 2, 360, 0.7f, color, buffer, 0, 0,
                  ITEM_TEXTSTYLE_SHADOWED);

    if(cg.snap->ps.persistant[PERS_SPAWNS] == 0) {
        Q_vsprintf_s(buffer, MAX_STRING_CHARS, MAX_STRING_CHARS,
                     "There are no spawns remaining");
    } else if(cg.snap->ps.persistant[PERS_SPAWNS] == 1) {
        Q_vsprintf_s(buffer, MAX_STRING_CHARS, MAX_STRING_CHARS,
                     "There is 1 spawn remaining");
    } else {
        Q_vsprintf_s(buffer, MAX_STRING_CHARS, MAX_STRING_CHARS,
                     "There are %d spawns remaining", cg.snap->ps.persistant[PERS_SPAWNS]);
    }

    w = UI_Text_Width(buffer, 0.7f, 0);
    UI_Text_Paint(320 - w / 2, 400, 0.7f, color, buffer, 0, 0,
                  ITEM_TEXTSTYLE_SHADOWED);

    return true;
}

/*
===============
idCGameDraw::DrawBotInfo
===============
*/
void idCGameDraw::DrawBotInfo(void) {
    valueType buffer[ MAX_STRING_CHARS ];
    float32 w;
    vec4_t color = { 1, 1, 1, 1 };
    float32 scale = 0.4f;
    pointer  info;
    valueType key[MAX_INFO_KEY];
    valueType value[MAX_INFO_VALUE];
    sint y, clientnum;

    //Q_vsprintf_s( buffer, MAX_STRING_CHARS, "BotInfo Test.");
    //w = UI_Text_Width( buffer, scale, 0 );
    //Text_Paint( 640 - w , 160, scale, color, buffer, 0, 0, ITEM_TEXTSTYLE_NORMAL );

    // we dont spec? so give info for the crosshairplayer
    if(!cgs.clientinfo[ cg.snap->ps.clientNum ].botSkill) {
        clientnum = cgameLocal.CrosshairPlayer();
    } else {
        // info for the bot we spec
        clientnum = cg.snap->ps.clientNum;
    }

    info = idCGameMain::ConfigString(CS_BOTINFOS + clientnum);

    y = 160;

    while(1) {
        Info_NextPair(&info, key, value);

        if(!key[0]) {
            break;
        }

        if(!value[0]) {
            break;
        }

        Q_vsprintf_s(buffer, MAX_STRING_CHARS, MAX_STRING_CHARS, va("%s: %s", key,
                     value));

        y += UI_Text_Height(buffer, scale, 0) + 5;
        w = UI_Text_Width(buffer, scale, 0);

        UI_Text_Paint(630 - w, y, scale, color, buffer, 0, 0,
                      ITEM_TEXTSTYLE_NORMAL);
    }

}

//==================================================================================

/*
=================
idCGameDraw::Draw2D
=================
*/
#define SPECTATOR_STRING "SPECTATOR"
void idCGameDraw::Draw2D(void) {
    vec4_t color;
    float32 w;
    menuDef_t *menu = nullptr, *defaultMenu;

    color[ 0 ] = color[ 1 ] = color[ 2 ] = color[ 3 ] = 1.0f;

    // if we are taking a levelshot for the menu, don't draw anything
    if(cg.levelShot) {
        return;
    }

    if(cg_draw2D.integer == 0) {
        return;
    }

    if(cg.snap->ps.pm_type == PM_INTERMISSION) {
        DrawIntermission();
        return;
    }

    DrawLighting();

    defaultMenu = Menus_FindByName("default_hud");

    if(cg.snap->ps.persistant[ PERS_SPECSTATE ] != SPECTATOR_NOT) {
        w = UI_Text_Width(SPECTATOR_STRING, 0.7f, 0);
        UI_Text_Paint(320 - w / 2, 440, 0.7f, color, SPECTATOR_STRING, 0, 0,
                      ITEM_TEXTSTYLE_SHADOWED);
    } else {
        menu = Menus_FindByName(bggame->ClassConfig((class_t)
                                cg.predictedPlayerState.stats[STAT_CLASS])->hudName);
    }

    if(menu && !(cg.snap->ps.stats[ STAT_STATE ] & SS_HOVELING) &&
            (cg.snap->ps.stats[ STAT_HEALTH ] > 0)) {
        idCGameBuildable::DrawBuildableStatus();
        idCGamePlayers::DrawTeamStatus();

        if(cg_drawStatus.integer) {
            Menu_Paint(menu, true);
        }
    } else if(cg_drawStatus.integer) {
        Menu_Paint(defaultMenu, true);
    }

    DrawVote();
    DrawTeamVote();
    DrawFollow();
    DrawQueue();
    DrawBotInfo();

    // don't draw center string if scoreboard is up
    cg.scoreBoardShowing = DrawScoreboard();

    if(!cg.scoreBoardShowing) {
        DrawCenterString();
    }
}

/*
===============
idCGameDraw::ScalePainBlendTCs
===============
*/
void idCGameDraw::ScalePainBlendTCs(float32 *s1, float32 *t1, float32 *s2,
                                    float32 *t2) {
    *s1 -= 0.5f;
    *t1 -= 0.5f;
    *s2 -= 0.5f;
    *t2 -= 0.5f;

    *s1 *= cg_painBlendZoom.value;
    *t1 *= cg_painBlendZoom.value;
    *s2 *= cg_painBlendZoom.value;
    *t2 *= cg_painBlendZoom.value;

    *s1 += 0.5f;
    *t1 += 0.5f;
    *s2 += 0.5f;
    *t2 += 0.5f;
}

/*
===============
idCGameDraw::PainBlend
===============
*/
#define PAINBLEND_BORDER 0.15f
void idCGameDraw::PainBlend(void) {
    vec4_t color;
    sint damage;
    float32 damageAsFracOfMax, x, y, w, h, s1, t1, s2, t2;
    qhandle_t shader = cgs.media.viewBloodShader;

    if(cg.snap->ps.persistant[PERS_SPECSTATE] != SPECTATOR_NOT ||
            cg.intermissionStarted) {
        return;
    }

    damage = cg.lastHealth - cg.snap->ps.stats[STAT_HEALTH];

    if(damage < 0) {
        damage = 0;
    }

    damageAsFracOfMax = (float32)damage / cg.snap->ps.stats[STAT_MAX_HEALTH];
    cg.lastHealth = cg.snap->ps.stats[STAT_HEALTH];

    cg.painBlendValue += damageAsFracOfMax * cg_painBlendScale.value;

    if(cg.painBlendValue > 0.0f) {
        cg.painBlendValue -= (cg.frametime / 1000.0f) * cg_painBlendDownRate.value;
    }

    if(cg.painBlendValue > 1.0f) {
        cg.painBlendValue = 1.0f;
    } else if(cg.painBlendValue <= 0.0f) {
        cg.painBlendValue = 0.0f;
        return;
    }

    if(cg.snap->ps.stats[STAT_TEAM] == TEAM_ALIENS) {
        VectorSet(color, 0.43f, 0.8f, 0.37f);
    } else if(cg.snap->ps.stats[STAT_TEAM] == TEAM_HUMANS) {
        VectorSet(color, 0.8f, 0.0f, 0.0f);
    }

    if(cg.painBlendValue > cg.painBlendTarget) {
        cg.painBlendTarget += (cg.frametime / 1000.0f) * cg_painBlendUpRate.value;
    } else if(cg.painBlendValue < cg.painBlendTarget) {
        cg.painBlendTarget = cg.painBlendValue;
    }

    if(cg.painBlendTarget > cg_painBlendMax.value) {
        cg.painBlendTarget = cg_painBlendMax.value;
    }

    color[ 3 ] = cg.painBlendTarget;

    trap_R_SetColor(color);

    //left
    x = 0.0f;
    y = 0.0f;
    w = PAINBLEND_BORDER * 640.0f;
    h = 480.0f;
    idCGameDrawTools::AdjustFrom640(&x, &y, &w, &h);
    s1 = 0.0f;
    t1 = 0.0f;
    s2 = PAINBLEND_BORDER;
    t2 = 1.0f;
    ScalePainBlendTCs(&s1, &t1, &s2, &t2);
    trap_R_DrawStretchPic(x, y, w, h, s1, t1, s2, t2, shader);

    //right
    x = 640.0f - (PAINBLEND_BORDER * 640.0f);
    y = 0.0f;
    w = PAINBLEND_BORDER * 640.0f;
    h = 480.0f;
    idCGameDrawTools::AdjustFrom640(&x, &y, &w, &h);
    s1 = 1.0f - PAINBLEND_BORDER;
    t1 = 0.0f;
    s2 = 1.0f;
    t2 = 1.0f;
    ScalePainBlendTCs(&s1, &t1, &s2, &t2);
    trap_R_DrawStretchPic(x, y, w, h, s1, t1, s2, t2, shader);

    //top
    x = PAINBLEND_BORDER * 640.0f;
    y = 0.0f;
    w = 640.0f - (2 * PAINBLEND_BORDER * 640.0f);
    h = PAINBLEND_BORDER * 480.0f;
    idCGameDrawTools::AdjustFrom640(&x, &y, &w, &h);
    s1 = PAINBLEND_BORDER;
    t1 = 0.0f;
    s2 = 1.0f - PAINBLEND_BORDER;
    t2 = PAINBLEND_BORDER;
    ScalePainBlendTCs(&s1, &t1, &s2, &t2);
    trap_R_DrawStretchPic(x, y, w, h, s1, t1, s2, t2, shader);

    //bottom
    x = PAINBLEND_BORDER * 640.0f;
    y = 480.0f - (PAINBLEND_BORDER * 480.0f);
    w = 640.0f - (2 * PAINBLEND_BORDER * 640.0f);
    h = PAINBLEND_BORDER * 480.0f;
    idCGameDrawTools::AdjustFrom640(&x, &y, &w, &h);
    s1 = PAINBLEND_BORDER;
    t1 = 1.0f - PAINBLEND_BORDER;
    s2 = 1.0f - PAINBLEND_BORDER;
    t2 = 1.0f;
    ScalePainBlendTCs(&s1, &t1, &s2, &t2);
    trap_R_DrawStretchPic(x, y, w, h, s1, t1, s2, t2, shader);

    trap_R_SetColor(nullptr);
}

/*
=====================
idCGameDraw::ResetPainBlend
=====================
*/
void idCGameDraw::ResetPainBlend(void) {
    cg.painBlendValue = 0.0f;
    cg.painBlendTarget = 0.0f;
    cg.lastHealth = cg.snap->ps.stats[ STAT_HEALTH ];
}

/*
=====================
idCGameDraw::DrawActive

Perform all drawing needed to completely fill the screen
=====================
*/
void idCGameDraw::DrawActive(stereoFrame_t stereoView) {
    float32 separation;
    vec3_t baseOrg;

    // optionally draw the info screen instead
    if(!cg.snap) {
        return;
    }

    switch(stereoView) {
        case STEREO_CENTER:
            separation = 0;
            break;

        case STEREO_LEFT:
            separation = -cg_stereoSeparation.value / 2;
            break;

        case STEREO_RIGHT:
            separation = cg_stereoSeparation.value / 2;
            break;

        default:
            separation = 0;
            Error("idCGameLocal::DrawActive: Undefined stereoView");
    }

    // clear around the rendered view if sized down
    idCGameDrawTools::TileClear();

    // offset vieworg appropriately if we're doing stereo separation
    VectorCopy(cg.refdef.vieworg, baseOrg);

    if(separation != 0) {
        VectorMA(cg.refdef.vieworg, -separation, cg.refdef.viewaxis[1],
                 cg.refdef.vieworg);
    }

    // draw 3D view
    trap_R_RenderScene(&cg.refdef);

    // restore original viewpoint if running stereo
    if(separation != 0) {
        VectorCopy(baseOrg, cg.refdef.vieworg);
    }

    // first person blend blobs, done after AnglesToAxis
    if(!cg.renderingThirdPerson) {
        PainBlend();
    }

    // draw status bar and other floating elements
    Draw2D();
}
