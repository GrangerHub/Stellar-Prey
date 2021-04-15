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
// File name:   cgame_view.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CGAME_VIEW_H__
#define __CGAME_VIEW_H__

class idCGameLocal;

class idCGameView : public idCGameLocal {
public:
    idCGameView();
    ~idCGameView();

    static void TestModel_f(void);
    static void TestGun_f(void);
    static void TestModelNextFrame_f(void);
    static void TestModelPrevFrame_f(void);
    static void TestModelNextSkin_f(void);
    static void TestModelPrevSkin_f(void);
    static void AddTestModel(void);
    static void CalcVrect(void);
    static void OffsetThirdPersonView(void);
    static void OffsetShoulderView(void);
    static void OffsetFirstPersonView(void);
    static void StepOffset(void);
    static sint CalcFov(void);
    static void DrawSurfNormal(void);
    static void addSmoothOp(vec3_t rotAxis, float32 rotAngle, float32 timeMod);
    static void smoothWWTransitions(playerState_t *ps, const vec3_t in,
                                    vec3_t out);
    static void smoothWJTransitions(playerState_t *ps, const vec3_t in,
                                    vec3_t out);
    static sint CalcViewValues(void);
    static void AddBufferedSound(sfxHandle_t sfx);
    static void PlayBufferedSounds(void);
};

#endif //!__CGAME_VIEW_H__
