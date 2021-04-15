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
// File name:   cgame_players.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description: handle the media and animation for player entities
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <cgame/cgame_precompiled.hpp>

valueType *cg_customSoundNames[ MAX_CUSTOM_SOUNDS ] = {
    "*death1.ogg",
    "*death2.ogg",
    "*death3.ogg",
    "*jump1.ogg",
    "*pain25_1.ogg",
    "*pain50_1.ogg",
    "*pain75_1.ogg",
    "*pain100_1.ogg",
    "*falling1.ogg",
    "*gasp.ogg",
    "*drown.ogg",
    "*fall1.ogg",
    "*taunt.ogg"
};

/*
===============
idCGamePlayers::idCGamePlayers
===============
*/
idCGamePlayers::idCGamePlayers(void) {
}

/*
===============
idCGamePlayers::~idCGamePlayers
===============
*/
idCGamePlayers::~idCGamePlayers(void) {
}

/*
================
idCGamePlayers::CustomSound
================
*/
sfxHandle_t idCGamePlayers::CustomSound(sint clientNum,
                                        pointer soundName) {
    clientInfo_t *ci;
    sint i;

    if(soundName[0] != '*') {
        return trap_S_RegisterSound(soundName);
    }

    if(clientNum < 0 || clientNum >= MAX_CLIENTS) {
        clientNum = 0;
    }

    ci = &cgs.clientinfo[ clientNum ];

    for(i = 0; i < MAX_CUSTOM_SOUNDS && cg_customSoundNames[ i ]; i++) {
        if(!strcmp(soundName, cg_customSoundNames[i])) {
            return ci->sounds[i];
        }
    }

    Error("Unknown custom sound: %s", soundName);
    return 0;
}



/*
=============================================================================

CLIENT INFO

=============================================================================
*/

/*
======================
idCGameLocal::ParseAnimationFile

Read a configuration file containing animation coutns and rates
models/players/visor/animation.cfg, etc
======================
*/
bool idCGamePlayers::ParseAnimationFile(pointer filename,
                                        clientInfo_t *ci) {
    valueType *text_p, *prev;
    sint len, i, skip;
    valueType *token;
    float32 fps;
    valueType text[ 20000 ];
    fileHandle_t f;
    animation_t *animations;

    animations = ci->animations;

    // load the file
    len = trap_FS_FOpenFile(filename, &f, FS_READ);

    if(len <= 0) {
        return false;
    }

    if(len == 0 || len >= sizeof(text) - 1) {
        Printf("File %s is %s\n", filename, len == 0 ? "empty" : "too long");
        trap_FS_FCloseFile(f);
        return false;
    }

    trap_FS_Read(text, len, f);
    text[ len ] = 0;
    trap_FS_FCloseFile(f);

    // parse the text
    text_p = text;
    skip = 0; // quite the compiler warning

    ci->footsteps = FOOTSTEP_NORMAL;
    VectorClear(ci->headOffset);
    ci->gender = GENDER_MALE;
    ci->fixedlegs = false;
    ci->fixedtorso = false;
    ci->nonsegmented = false;

    // read optional parameters
    while(1) {
        prev = text_p;  // so we can unget
        token = COM_Parse(&text_p);

        if(!token) {
            break;
        }

        if(!Q_stricmp(token, "footsteps")) {
            token = COM_Parse(&text_p);

            if(!token) {
                break;
            }

            if(!Q_stricmp(token, "default") || !Q_stricmp(token, "normal")) {
                ci->footsteps = FOOTSTEP_NORMAL;
            } else if(!Q_stricmp(token, "flesh")) {
                ci->footsteps = FOOTSTEP_FLESH;
            } else if(!Q_stricmp(token, "none")) {
                ci->footsteps = FOOTSTEP_NONE;
            } else if(!Q_stricmp(token, "custom")) {
                ci->footsteps = FOOTSTEP_CUSTOM;
            } else {
                Printf("Bad footsteps parm in %s: %s\n", filename, token);
            }

            continue;
        } else if(!Q_stricmp(token, "headoffset")) {
            for(i = 0 ; i < 3 ; i++) {
                token = COM_Parse(&text_p);

                if(!token) {
                    break;
                }

                ci->headOffset[ i ] = atof(token);
            }

            continue;
        } else if(!Q_stricmp(token, "sex")) {
            token = COM_Parse(&text_p);

            if(!token) {
                break;
            }

            if(token[0] == 'f' || token[0] == 'F') {
                ci->gender = GENDER_FEMALE;
            } else if(token[0] == 'n' || token[0] == 'N') {
                ci->gender = GENDER_NEUTER;
            } else {
                ci->gender = GENDER_MALE;
            }

            continue;
        } else if(!Q_stricmp(token, "fixedlegs")) {
            ci->fixedlegs = true;
            continue;
        } else if(!Q_stricmp(token, "fixedtorso")) {
            ci->fixedtorso = true;
            continue;
        } else if(!Q_stricmp(token, "nonsegmented")) {
            ci->nonsegmented = true;
            continue;
        }

        // if it is a number, start parsing animations
        if(token[ 0 ] >= '0' && token[ 0 ] <= '9') {
            text_p = prev;  // unget the token
            break;
        }

        Com_Printf("unknown token '%s' is %s\n", token, filename);
    }

    if(!ci->nonsegmented) {
        // read information for each frame
        for(i = 0; i < MAX_PLAYER_ANIMATIONS; i++) {
            token = COM_Parse(&text_p);

            if(!*token) {
                if(i >= TORSO_GETFLAG && i <= TORSO_NEGATIVE) {
                    animations[ i ].firstFrame = animations[ TORSO_GESTURE ].firstFrame;
                    animations[ i ].frameLerp = animations[ TORSO_GESTURE ].frameLerp;
                    animations[ i ].initialLerp = animations[ TORSO_GESTURE ].initialLerp;
                    animations[ i ].loopFrames = animations[ TORSO_GESTURE ].loopFrames;
                    animations[ i ].numFrames = animations[ TORSO_GESTURE ].numFrames;
                    animations[ i ].reversed = false;
                    animations[ i ].flipflop = false;
                    continue;
                }

                break;
            }

            animations[ i ].firstFrame = atoi(token);

            // leg only frames are adjusted to not count the upper body only frames
            if(i == LEGS_WALKCR) {
                skip = animations[LEGS_WALKCR].firstFrame -
                       animations[TORSO_GESTURE].firstFrame;
            }

            if(i >= LEGS_WALKCR && i < TORSO_GETFLAG) {
                animations[i].firstFrame -= skip;
            }

            token = COM_Parse(&text_p);

            if(!*token) {
                break;
            }

            animations[ i ].numFrames = atoi(token);
            animations[ i ].reversed = false;
            animations[ i ].flipflop = false;

            // if numFrames is negative the animation is reversed
            if(animations[ i ].numFrames < 0) {
                animations[ i ].numFrames = -animations[ i ].numFrames;
                animations[ i ].reversed = true;
            }

            token = COM_Parse(&text_p);

            if(!*token) {
                break;
            }

            animations[ i ].loopFrames = atoi(token);

            token = COM_Parse(&text_p);

            if(!*token) {
                break;
            }

            fps = atof(token);

            if(fps == 0) {
                fps = 1;
            }

            animations[ i ].frameLerp = 1000 / fps;
            animations[ i ].initialLerp = 1000 / fps;
        }

        if(i != MAX_PLAYER_ANIMATIONS) {
            Printf("Error parsing animation file: %s", filename);
            return false;
        }

        // crouch backward animation
        ::memcpy(&animations[ LEGS_BACKCR ], &animations[ LEGS_WALKCR ],
                 sizeof(animation_t));
        animations[ LEGS_BACKCR ].reversed = true;

        // walk backward animation
        ::memcpy(&animations[ LEGS_BACKWALK ], &animations[ LEGS_WALK ],
                 sizeof(animation_t));
        animations[ LEGS_BACKWALK ].reversed = true;

        // flag moving fast
        animations[ FLAG_RUN ].firstFrame = 0;
        animations[ FLAG_RUN ].numFrames = 16;
        animations[ FLAG_RUN ].loopFrames = 16;
        animations[ FLAG_RUN ].frameLerp = 1000 / 15;
        animations[ FLAG_RUN ].initialLerp = 1000 / 15;
        animations[ FLAG_RUN ].reversed = false;

        // flag not moving or moving slowly
        animations[ FLAG_STAND ].firstFrame = 16;
        animations[ FLAG_STAND ].numFrames = 5;
        animations[ FLAG_STAND ].loopFrames = 0;
        animations[ FLAG_STAND ].frameLerp = 1000 / 20;
        animations[ FLAG_STAND ].initialLerp = 1000 / 20;
        animations[ FLAG_STAND ].reversed = false;

        // flag speeding up
        animations[ FLAG_STAND2RUN ].firstFrame = 16;
        animations[ FLAG_STAND2RUN ].numFrames = 5;
        animations[ FLAG_STAND2RUN ].loopFrames = 1;
        animations[ FLAG_STAND2RUN ].frameLerp = 1000 / 15;
        animations[ FLAG_STAND2RUN ].initialLerp = 1000 / 15;
        animations[ FLAG_STAND2RUN ].reversed = true;
    } else {
        // read information for each frame
        for(i = 0; i < MAX_NONSEG_PLAYER_ANIMATIONS; i++) {
            token = COM_Parse(&text_p);

            if(!*token) {
                break;
            }

            animations[ i ].firstFrame = atoi(token);

            token = COM_Parse(&text_p);

            if(!*token) {
                break;
            }

            animations[ i ].numFrames = atoi(token);
            animations[ i ].reversed = false;
            animations[ i ].flipflop = false;

            // if numFrames is negative the animation is reversed
            if(animations[ i ].numFrames < 0) {
                animations[ i ].numFrames = -animations[ i ].numFrames;
                animations[ i ].reversed = true;
            }

            token = COM_Parse(&text_p);

            if(!*token) {
                break;
            }

            animations[ i ].loopFrames = atoi(token);

            token = COM_Parse(&text_p);

            if(!*token) {
                break;
            }

            fps = atof(token);

            if(fps == 0) {
                fps = 1;
            }

            animations[ i ].frameLerp = 1000 / fps;
            animations[ i ].initialLerp = 1000 / fps;
        }

        if(i != MAX_NONSEG_PLAYER_ANIMATIONS) {
            Printf("Error parsing animation file: %s", filename);
            return false;
        }

        // walk backward animation
        memcpy(&animations[ NSPA_WALKBACK ], &animations[ NSPA_WALK ],
               sizeof(animation_t));
        animations[ NSPA_WALKBACK ].reversed = true;
    }

    return true;
}

/*
==========================
idCGamePlayers::RegisterClientSkin
==========================
*/
bool idCGamePlayers::RegisterClientSkin(clientInfo_t *ci,
                                        pointer modelName, pointer skinName) {
    valueType filename[ MAX_QPATH ];

    if(!ci->nonsegmented) {
        Q_vsprintf_s(filename, sizeof(filename), sizeof(filename),
                     "models/players/%s/lower_%s.skin", modelName, skinName);
        ci->legsSkin = trap_R_RegisterSkin(filename);

        if(!ci->legsSkin) {
            Com_Printf("Leg skin load failure: %s\n", filename);
        }

        Q_vsprintf_s(filename, sizeof(filename), sizeof(filename),
                     "models/players/%s/upper_%s.skin", modelName, skinName);
        ci->torsoSkin = trap_R_RegisterSkin(filename);

        if(!ci->torsoSkin) {
            Com_Printf("Torso skin load failure: %s\n", filename);
        }

        Q_vsprintf_s(filename, sizeof(filename), sizeof(filename),
                     "models/players/%s/head_%s.skin", modelName, skinName);
        ci->headSkin = trap_R_RegisterSkin(filename);

        if(!ci->headSkin) {
            Com_Printf("Head skin load failure: %s\n", filename);
        }

        if(!ci->legsSkin || !ci->torsoSkin || !ci->headSkin) {
            return false;
        }
    } else {
        Q_vsprintf_s(filename, sizeof(filename), sizeof(filename),
                     "models/players/%s/nonseg_%s.skin", modelName, skinName);
        ci->nonSegSkin = trap_R_RegisterSkin(filename);

        if(!ci->nonSegSkin) {
            Com_Printf("Non-segmented skin load failure: %s\n", filename);
        }

        if(!ci->nonSegSkin) {
            return false;
        }
    }

    return true;
}

/*
==========================
idCGamePlayers::RegisterClientModelname
==========================
*/
bool idCGamePlayers::RegisterClientModelname(clientInfo_t *ci,
        pointer modelName, pointer skinName) {
    valueType filename[ MAX_QPATH * 2 ];

    // do this first so the nonsegmented property is set
    // load the animations
    Q_vsprintf_s(filename, sizeof(filename), sizeof(filename),
                 "models/players/%s/animation.cfg", modelName);

    if(!ParseAnimationFile(filename, ci)) {
        Com_Printf("Failed to load animation file %s\n", filename);
        return false;
    }

    // load cmodels before models so filecache works

    if(!ci->nonsegmented) {
        Q_vsprintf_s(filename, sizeof(filename), sizeof(filename),
                     "models/players/%s/lower.md3", modelName);
        ci->legsModel = trap_R_RegisterModel(filename);

        if(!ci->legsModel) {
            Com_Printf("Failed to load model file %s\n", filename);
            return false;
        }

        Q_vsprintf_s(filename, sizeof(filename), sizeof(filename),
                     "models/players/%s/upper.md3", modelName);
        ci->torsoModel = trap_R_RegisterModel(filename);

        if(!ci->torsoModel) {
            Com_Printf("Failed to load model file %s\n", filename);
            return false;
        }

        Q_vsprintf_s(filename, sizeof(filename), sizeof(filename),
                     "models/players/%s/head.md3", modelName);
        ci->headModel = trap_R_RegisterModel(filename);

        if(!ci->headModel) {
            Com_Printf("Failed to load model file %s\n", filename);
            return false;
        }
    } else {
        Q_vsprintf_s(filename, sizeof(filename), sizeof(filename),
                     "models/players/%s/nonseg.md3", modelName);
        ci->nonSegModel = trap_R_RegisterModel(filename);

        if(!ci->nonSegModel) {
            Com_Printf("Failed to load model file %s\n", filename);
            return false;
        }
    }

    // if any skins failed to load, return failure
    if(!RegisterClientSkin(ci, modelName, skinName)) {
        Com_Printf("Failed to load skin file: %s : %s\n", modelName, skinName);
        return false;
    }

    return true;
}

/*
====================
idCGamePlayers::ColorFromString
====================
*/
void idCGamePlayers::ColorFromString(pointer v, vec3_t color) {
    sint val;

    VectorClear(color);

    val = atoi(v);

    if(val < 1 || val > 7) {
        VectorSet(color, 1, 1, 1);
        return;
    }

    if(val & 1) {
        color[2] = 1.0f;
    }

    if(val & 2) {
        color[1] = 1.0f;
    }

    if(val & 4) {
        color[0] = 1.0f;
    }
}


/*
===================
idCGameLocal::LoadClientInfo

Load it now, taking the disk hits
===================
*/
void idCGamePlayers::LoadClientInfo(clientInfo_t *ci) {
    pointer  dir, s;
    sint i, clientNum;

    if(!RegisterClientModelname(ci, ci->modelName, ci->skinName)) {
        Error("idCGameLocal::RegisterClientModelname( %s, %s ) failed",
              ci->modelName, ci->skinName);
    }

    // sounds
    dir = ci->modelName;

    for(i = 0; i < MAX_CUSTOM_SOUNDS; i++) {
        s = cg_customSoundNames[ i ];

        if(!s) {
            break;
        }

        // fanny about a bit with sounds that are missing
        if(!idCGameMain::FileExists(va("sound/player/%s/%s", dir, s + 1))) {
            //file doesn't exist

            if(i == 11 || i == 8) {  //fall or falling
                ci->sounds[ i ] = trap_S_RegisterSound("sound/null.ogg");
            } else {
                if(i == 9) {  //gasp
                    s = cg_customSoundNames[7]; //pain100_1
                } else if(i == 10) { //drown
                    s = cg_customSoundNames[0]; //death1
                }

                ci->sounds[ i ] = trap_S_RegisterSound(va("sound/player/%s/%s", dir,
                                                       s + 1));
            }
        } else {
            ci->sounds[ i ] = trap_S_RegisterSound(va("sound/player/%s/%s", dir,
                                                   s + 1));
        }
    }

    if(ci->footsteps == FOOTSTEP_CUSTOM) {
        for(i = 0; i < 4; i++) {
            ci->customFootsteps[ i ] = trap_S_RegisterSound(
                                           va("sound/player/%s/step%d.ogg", dir, i + 1));

            if(!ci->customFootsteps[i]) {
                ci->customFootsteps[i] = trap_S_RegisterSound(
                                             va("sound/player/footsteps/step%d.ogg", i + 1));
            }

            ci->customMetalFootsteps[ i ] = trap_S_RegisterSound(
                                                va("sound/player/%s/clank%d.ogg", dir, i + 1));

            if(!ci->customMetalFootsteps[i]) {
                ci->customMetalFootsteps[i] = trap_S_RegisterSound(
                                                  va("sound/player/footsteps/clank%d.ogg", i + 1));
            }
        }
    }

    // reset any existing players and bodies, because they might be in bad
    // frames for this new model
    clientNum = ci - cgs.clientinfo;

    for(i = 0; i < MAX_GENTITIES; i++) {
        if(cg_entities[i].currentState.clientNum == clientNum &&
                cg_entities[i].currentState.eType == ET_PLAYER) {
            ResetPlayerEntity(&cg_entities[i]);
        }
    }
}

/*
======================
idCGamePlayers::CopyClientInfoModel
======================
*/
void idCGamePlayers::CopyClientInfoModel(clientInfo_t *from,
        clientInfo_t *to) {
    VectorCopy(from->headOffset, to->headOffset);
    to->footsteps = from->footsteps;
    to->gender = from->gender;

    to->legsModel = from->legsModel;
    to->legsSkin = from->legsSkin;
    to->torsoModel = from->torsoModel;
    to->torsoSkin = from->torsoSkin;
    to->headModel = from->headModel;
    to->headSkin = from->headSkin;
    to->nonSegModel = from->nonSegModel;
    to->nonSegSkin = from->nonSegSkin;
    to->nonsegmented = from->nonsegmented;
    to->modelIcon = from->modelIcon;

    ::memcpy(to->animations, from->animations, sizeof(to->animations));
    ::memcpy(to->sounds, from->sounds, sizeof(to->sounds));
    ::memcpy(to->customFootsteps, from->customFootsteps,
             sizeof(to->customFootsteps));
    ::memcpy(to->customMetalFootsteps, from->customMetalFootsteps,
             sizeof(to->customMetalFootsteps));
}


/*
======================
idCGameLocal::GetCorpseNum
======================
*/
sint idCGamePlayers::GetCorpseNum(class_t _class) {
    sint i;
    clientInfo_t *match;
    valueType *modelName, *skinName;

    modelName = bggame->ClassConfig(_class)->modelName;
    skinName = bggame->ClassConfig(_class)->skinName;

    for(i = PCL_NONE + 1; i < PCL_NUM_CLASSES; i++) {
        match = &cgs.corpseinfo[ i ];

        if(!match->infoValid) {
            continue;
        }

        if(!Q_stricmp(modelName, match->modelName) &&
                !Q_stricmp(skinName, match->skinName)) {
            // this clientinfo is identical, so use it's handles
            return i;
        }
    }

    //something has gone horribly wrong
    return -1;
}


/*
======================
idCGamePlayers::ScanForExistingClientInfo
======================
*/
bool idCGamePlayers::ScanForExistingClientInfo(clientInfo_t *ci) {
    sint i;
    clientInfo_t *match;

    for(i = PCL_NONE + 1; i < PCL_NUM_CLASSES; i++) {
        match = &cgs.corpseinfo[ i ];

        if(!match->infoValid) {
            continue;
        }

        if(!Q_stricmp(ci->modelName, match->modelName) &&
                !Q_stricmp(ci->skinName, match->skinName)) {
            // this clientinfo is identical, so use it's handles
            CopyClientInfoModel(match, ci);

            return true;
        }
    }

    // shouldn't happen
    return false;
}


/*
======================
idCGamePlayers::PrecacheClientInfo
======================
*/
void idCGamePlayers::PrecacheClientInfo(class_t _class, valueType *model,
                                        valueType *skin) {
    clientInfo_t *ci, newInfo;

    ci = &cgs.corpseinfo[_class];

    // the old value
    ::memset(&newInfo, 0, sizeof(newInfo));

    // model
    Q_strncpyz(newInfo.modelName, model, sizeof(newInfo.modelName));

    // modelName did not include a skin name
    if(!skin) {
        Q_strncpyz(newInfo.skinName, "default", sizeof(newInfo.skinName));
    } else {
        Q_strncpyz(newInfo.skinName, skin, sizeof(newInfo.skinName));
    }

    newInfo.infoValid = true;

    // actually register the models
    *ci = newInfo;
    LoadClientInfo(ci);
}


/*
=============
idCGamePlayers::TeamJoinMessage

Prints messages when players change teams
=============
*/
void idCGamePlayers::TeamJoinMessage(clientInfo_t *newInfo,
                                     clientInfo_t *ci) {
    sint team, oldteam;
    valueType *playerName;

    if(!ci->infoValid) {
        return;
    }

    // Collect info
    team = newInfo->team;
    oldteam = ci->team;

    // If no change occurred, print nothing
    if(team == oldteam) {
        return;
    }

    playerName = newInfo->name;

    // Print the appropriate message
    if(team == TEAM_NONE) {
        Printf("%s" S_COLOR_WHITE " left the %ss\n", playerName,
               bggame->TeamName((team_t)oldteam));
    } else if(oldteam == TEAM_NONE) {
        Printf("%s" S_COLOR_WHITE " joined the %ss\n", playerName,
               bggame->TeamName((team_t)team));
    } else {
        Printf("%s" S_COLOR_WHITE " left the %ss and joined the %ss\n", playerName,
               bggame->TeamName((team_t)oldteam), bggame->TeamName((team_t)team));
    }
}


/*
======================
idCGameLocal::NewClientInfo
======================
*/
void idCGamePlayers::NewClientInfo(sint clientNum) {
    clientInfo_t *ci, newInfo;
    pointer configstring;
    pointer v;
    valueType *slash;

    ci = &cgs.clientinfo[ clientNum ];

    configstring = idCGameMain::ConfigString(clientNum + CS_PLAYERS);

    if(!configstring[ 0 ]) {
        ::memset(ci, 0, sizeof(*ci));
        return;   // player just left
    }

    // the old value
    ::memset(&newInfo, 0, sizeof(newInfo));

    // grab our own ignoreList
    if(clientNum == cg.predictedPlayerState.clientNum) {
        v = Info_ValueForKey(configstring, "ig");
        bggame->ClientListParse(&cgs.ignoreList, v);
    }

    // isolate the player's name
    v = Info_ValueForKey(configstring, "n");
    Q_strncpyz(newInfo.name, v, sizeof(newInfo.name));

    // colors
    v = Info_ValueForKey(configstring, "c1");
    ColorFromString(v, newInfo.color1);

    v = Info_ValueForKey(configstring, "c2");
    ColorFromString(v, newInfo.color2);

    // bot skill
    v = Info_ValueForKey(configstring, "skill");
    newInfo.botSkill = atoi(v);

    // handicap
    v = Info_ValueForKey(configstring, "hc");
    newInfo.handicap = atoi(v);

    // team
    v = Info_ValueForKey(configstring, "t");
    newInfo.team = (team_t)atoi(v);
    TeamJoinMessage(&newInfo, ci);

    // if this is us, execute team-specific config files
    // unfortunately, these get re-executed after a vid_restart, because the
    // cgame can't tell the difference between that and joining a new server
    if(clientNum == cg.clientNum && (!ci->infoValid ||
                                     ci->team != newInfo.team)) {
        valueType config[ MAX_STRING_CHARS ];
        trap_Cvar_VariableStringBuffer(va("cg_%sConfig",
                                          bggame->TeamName(newInfo.team)), config, sizeof(config));

        if(config[0]) {
            trap_SendConsoleCommand(va("exec %s\n", config));
        }
    }

    // model
    v = Info_ValueForKey(configstring, "model");
    Q_strncpyz(newInfo.modelName, v, sizeof(newInfo.modelName));

    slash = strchr(newInfo.modelName, '/');

    if(!slash) {
        // modelName didn not include a skin name
        Q_strncpyz(newInfo.skinName, "default", sizeof(newInfo.skinName));
    } else {
        Q_strncpyz(newInfo.skinName, slash + 1, sizeof(newInfo.skinName));
        // truncate modelName
        *slash = 0;
    }

    // voice
    v = Info_ValueForKey(configstring, "v");
    Q_strncpyz(newInfo.voice, v, sizeof(newInfo.voice));

    // replace whatever was there with the new one
    newInfo.infoValid = true;
    *ci = newInfo;

    // scan for an existing clientinfo that matches this modelname
    // so we can avoid loading checks if possible
    if(!ScanForExistingClientInfo(ci)) {
        LoadClientInfo(ci);
    }
}

/*
=============================================================================
PLAYER ANIMATION
=============================================================================
*/

/*
===============
idCGamePlayers::SetLerpFrameAnimation

may include ANIM_TOGGLEBIT
===============
*/
void idCGamePlayers::SetLerpFrameAnimation(clientInfo_t *ci,
        lerpFrame_t *lf, sint newAnimation) {
    animation_t *anim;

    lf->animationNumber = newAnimation;
    newAnimation &= ~ANIM_TOGGLEBIT;

    if(newAnimation < 0 || newAnimation >= MAX_PLAYER_TOTALANIMATIONS) {
        Error("Bad animation number: %i", newAnimation);
    }

    anim = &ci->animations[ newAnimation ];

    lf->animation = anim;
    lf->animationTime = lf->frameTime + anim->initialLerp;

    if(cg_debugAnim.integer) {
        Printf("Anim: %i\n", newAnimation);
    }
}

/*
===============
idCGamePlayers::RunPlayerLerpFrame

Sets cg.snap, cg.oldFrame, and cg.backlerp
cg.time should be between oldFrameTime and frameTime after exit
===============
*/
void idCGamePlayers::RunPlayerLerpFrame(clientInfo_t *ci, lerpFrame_t *lf,
                                        sint newAnimation, float32 speedScale) {
    sint f, numFrames;
    animation_t *anim;

    // debugging tool to get no animations
    if(cg_animSpeed.integer == 0) {
        lf->oldFrame = lf->frame = lf->backlerp = 0;
        return;
    }

    // see if the animation sequence is switching
    if(newAnimation != lf->animationNumber || !lf->animation) {
        SetLerpFrameAnimation(ci, lf, newAnimation);
    }

    // if we have passed the current frame, move it to
    // oldFrame and calculate a new frame
    if(cg.time >= lf->frameTime) {
        lf->oldFrame = lf->frame;
        lf->oldFrameTime = lf->frameTime;

        // get the next frame based on the animation
        anim = lf->animation;

        if(!anim->frameLerp) {
            return;   // shouldn't happen
        }

        if(cg.time < lf->animationTime) {
            lf->frameTime = lf->animationTime;    // initial lerp
        } else {
            lf->frameTime = lf->oldFrameTime + anim->frameLerp;
        }

        f = (lf->frameTime - lf->animationTime) / anim->frameLerp;
        f *= speedScale;    // adjust for haste, etc
        numFrames = anim->numFrames;

        if(anim->flipflop) {
            numFrames *= 2;
        }

        if(f >= numFrames) {
            f -= numFrames;

            if(anim->loopFrames) {
                f %= anim->loopFrames;
                f += anim->numFrames - anim->loopFrames;
            } else {
                f = numFrames - 1;
                // the animation is stuck at the end, so it
                // can immediately transition to another sequence
                lf->frameTime = cg.time;
            }
        }

        if(anim->reversed) {
            lf->frame = anim->firstFrame + anim->numFrames - 1 - f;
        } else if(anim->flipflop && f >= anim->numFrames) {
            lf->frame = anim->firstFrame + anim->numFrames - 1 - (f % anim->numFrames);
        } else {
            lf->frame = anim->firstFrame + f;
        }

        if(cg.time > lf->frameTime) {
            lf->frameTime = cg.time;

            if(cg_debugAnim.integer) {
                Printf("Clamp lf->frameTime\n");
            }
        }
    }

    if(lf->frameTime > cg.time + 200) {
        lf->frameTime = cg.time;
    }

    if(lf->oldFrameTime > cg.time) {
        lf->oldFrameTime = cg.time;
    }

    // calculate current lerp value
    if(lf->frameTime == lf->oldFrameTime) {
        lf->backlerp = 0;
    } else {
        lf->backlerp = 1.0 - (float32)(cg.time - lf->oldFrameTime) /
                       (lf->frameTime - lf->oldFrameTime);
    }
}


/*
===============
idCGamePlayers::ClearLerpFrame
===============
*/
void idCGamePlayers::ClearLerpFrame(clientInfo_t *ci, lerpFrame_t *lf,
                                    sint animationNumber) {
    lf->frameTime = lf->oldFrameTime = cg.time;
    SetLerpFrameAnimation(ci, lf, animationNumber);
    lf->oldFrame = lf->frame = lf->animation->firstFrame;
}


/*
===============
idCGamePlayers::PlayerAnimation
===============
*/
void idCGamePlayers::PlayerAnimation(centity_t *cent, sint *legsOld,
                                     sint *legs, float32 *legsBackLerp, sint *torsoOld, sint *torso,
                                     float32 *torsoBackLerp) {
    clientInfo_t *ci;
    sint clientNum;
    float32 speedScale = 1.0f;

    clientNum = cent->currentState.clientNum;

    if(cg_noPlayerAnims.integer) {
        *legsOld = *legs = *torsoOld = *torso = 0;
        return;
    }

    ci = &cgs.clientinfo[ clientNum ];

    // do the shuffle turn frames locally
    if(cent->pe.legs.yawing &&
            (cent->currentState.legsAnim & ~ANIM_TOGGLEBIT) == LEGS_IDLE) {
        RunPlayerLerpFrame(ci, &cent->pe.legs, LEGS_TURN, speedScale);
    } else {
        RunPlayerLerpFrame(ci, &cent->pe.legs, cent->currentState.legsAnim,
                           speedScale);
    }

    *legsOld = cent->pe.legs.oldFrame;
    *legs = cent->pe.legs.frame;
    *legsBackLerp = cent->pe.legs.backlerp;

    RunPlayerLerpFrame(ci, &cent->pe.torso, cent->currentState.torsoAnim,
                       speedScale);

    *torsoOld = cent->pe.torso.oldFrame;
    *torso = cent->pe.torso.frame;
    *torsoBackLerp = cent->pe.torso.backlerp;
}

/*
===============
idCGamePlayers::PlayerNonSegAnimation
===============
*/
void idCGamePlayers::PlayerNonSegAnimation(centity_t *cent,
        sint *nonSegOld, sint *nonSeg, float32 *nonSegBackLerp) {
    clientInfo_t *ci;
    sint clientNum;
    float32 speedScale = 1.0f;

    clientNum = cent->currentState.clientNum;

    if(cg_noPlayerAnims.integer) {
        *nonSegOld = *nonSeg = 0;
        return;
    }

    ci = &cgs.clientinfo[ clientNum ];

    // do the shuffle turn frames locally
    if(cent->pe.nonseg.yawing &&
            (cent->currentState.legsAnim & ~ANIM_TOGGLEBIT) == NSPA_STAND) {
        RunPlayerLerpFrame(ci, &cent->pe.nonseg, NSPA_TURN, speedScale);
    } else {
        RunPlayerLerpFrame(ci, &cent->pe.nonseg, cent->currentState.legsAnim,
                           speedScale);
    }

    *nonSegOld = cent->pe.nonseg.oldFrame;
    *nonSeg = cent->pe.nonseg.frame;
    *nonSegBackLerp = cent->pe.nonseg.backlerp;
}

/*
=============================================================================
PLAYER ANGLES
=============================================================================
*/

/*
==================
idCGamePlayers::SwingAngles
==================
*/
void idCGamePlayers::SwingAngles(float32 destination,
                                 float32 swingTolerance, float32 clampTolerance, float32 speed,
                                 float32 *angle, bool *swinging) {
    float32 swing, move, scale;

    if(!*swinging) {
        // see if a swing should be started
        swing = AngleSubtract(*angle, destination);

        if(swing > swingTolerance || swing < -swingTolerance) {
            *swinging = true;
        }
    }

    if(!*swinging) {
        return;
    }

    // modify the speed depending on the delta
    // so it doesn't seem so linear
    swing = AngleSubtract(destination, *angle);
    scale = fabs(swing);

    if(scale < swingTolerance * 0.5) {
        scale = 0.5;
    } else if(scale < swingTolerance) {
        scale = 1.0;
    } else {
        scale = 2.0;
    }

    // swing towards the destination angle
    if(swing >= 0) {
        move = cg.frametime * scale * speed;

        if(move >= swing) {
            move = swing;
            *swinging = false;
        }

        *angle = AngleMod(*angle + move);
    } else if(swing < 0) {
        move = cg.frametime * scale * -speed;

        if(move <= swing) {
            move = swing;
            *swinging = false;
        }

        *angle = AngleMod(*angle + move);
    }

    // clamp to no more than tolerance
    swing = AngleSubtract(destination, *angle);

    if(swing > clampTolerance) {
        *angle = AngleMod(destination - (clampTolerance - 1));
    } else if(swing < -clampTolerance) {
        *angle = AngleMod(destination + (clampTolerance - 1));
    }
}

/*
=================
idCGamePlayers::AddPainTwitch
=================
*/
void idCGamePlayers::AddPainTwitch(centity_t *cent, vec3_t torsoAngles) {
    sint t;
    float32 f;

    t = cg.time - cent->pe.painTime;

    if(t >= PAIN_TWITCH_TIME) {
        return;
    }

    f = 1.0 - (float32)t / PAIN_TWITCH_TIME;

    if(cent->pe.painDirection) {
        torsoAngles[ROLL] += 20 * f;
    } else {
        torsoAngles[ROLL] -= 20 * f;
    }
}


/*
===============
idCGamePlayers::PlayerAngles

Handles seperate torso motion
legs pivot based on direction of movement
head always looks exactly at cent->lerpAngles
if motion < 20 degrees, show in head only
if < 45 degrees, also show in torso
===============
*/
void idCGamePlayers::PlayerAngles(centity_t *cent, vec3_t srcAngles,
                                  vec3_t legs[ 3 ], vec3_t torso[ 3 ], vec3_t head[ 3 ]) {
    vec3_t legsAngles, torsoAngles, headAngles;
    float32 dest, speed;
    static sint movementOffsets[ 8 ] = { 0, 22, 45, -22, 0, 22, -45, -22 };
    vec3_t velocity;
    sint dir, clientNum;
    clientInfo_t *ci;

    VectorCopy(srcAngles, headAngles);
    headAngles[ YAW ] = AngleMod(headAngles[ YAW ]);
    VectorClear(legsAngles);
    VectorClear(torsoAngles);

    // --------- yaw -------------

    // allow yaw to drift a bit
    if((cent->currentState.legsAnim & ~ANIM_TOGGLEBIT) != LEGS_IDLE ||
            (cent->currentState.torsoAnim & ~ANIM_TOGGLEBIT) != TORSO_STAND) {
        // if not standing still, always point all in the same direction
        cent->pe.torso.yawing = true;  // always center
        cent->pe.torso.pitching = true;  // always center
        cent->pe.legs.yawing = true; // always center
    }

    // adjust legs for movement dir
    if(cent->currentState.eFlags & EF_DEAD) {
        // don't let dead bodies twitch
        dir = 0;
    } else {
        // did use angles2.. now uses time2.. looks a bit funny but time2 isn't used othwise
        dir = cent->currentState.time2;

        if(dir < 0 || dir > 7) {
            Error("Bad player movement angle");
        }
    }

    legsAngles[ YAW ] = headAngles[ YAW ] + movementOffsets[ dir ];
    torsoAngles[ YAW ] = headAngles[ YAW ] + 0.25 * movementOffsets[ dir ];

    // torso
    if(cent->currentState.eFlags & EF_DEAD) {
        SwingAngles(torsoAngles[ YAW ], 0, 0, cg_swingSpeed.value,
                    &cent->pe.torso.yawAngle, &cent->pe.torso.yawing);
        SwingAngles(legsAngles[ YAW ], 0, 0, cg_swingSpeed.value,
                    &cent->pe.legs.yawAngle, &cent->pe.legs.yawing);
    } else {
        SwingAngles(torsoAngles[ YAW ], 25, 90, cg_swingSpeed.value,
                    &cent->pe.torso.yawAngle, &cent->pe.torso.yawing);
        SwingAngles(legsAngles[ YAW ], 40, 90, cg_swingSpeed.value,
                    &cent->pe.legs.yawAngle, &cent->pe.legs.yawing);
    }

    torsoAngles[ YAW ] = cent->pe.torso.yawAngle;
    legsAngles[ YAW ] = cent->pe.legs.yawAngle;

    // --------- pitch -------------

    // only show a fraction of the pitch angle in the torso
    if(headAngles[PITCH] > 180) {
        dest = (-360 + headAngles[PITCH]) * 0.75f;
    } else {
        dest = headAngles[PITCH] * 0.75f;
    }

    SwingAngles(dest, 15, 30, 0.1f, &cent->pe.torso.pitchAngle,
                &cent->pe.torso.pitching);
    torsoAngles[ PITCH ] = cent->pe.torso.pitchAngle;

    //
    clientNum = cent->currentState.clientNum;

    if(clientNum >= 0 && clientNum < MAX_CLIENTS) {
        ci = &cgs.clientinfo[ clientNum ];

        if(ci->fixedtorso) {
            torsoAngles[PITCH] = 0.0f;
        }
    }

    // --------- roll -------------

    // lean towards the direction of travel
    VectorCopy(cent->currentState.pos.trDelta, velocity);
    speed = VectorNormalize(velocity);

    if(speed) {
        vec3_t axis[ 3 ];
        float32 side;

        speed *= 0.05f;

        AnglesToAxis(legsAngles, axis);
        side = speed * DotProduct(velocity, axis[ 1 ]);
        legsAngles[ ROLL ] -= side;

        side = speed * DotProduct(velocity, axis[ 0 ]);
        legsAngles[ PITCH ] += side;
    }

    //
    clientNum = cent->currentState.clientNum;

    if(clientNum >= 0 && clientNum < MAX_CLIENTS) {
        ci = &cgs.clientinfo[ clientNum ];

        if(ci->fixedlegs) {
            legsAngles[ YAW ] = torsoAngles[ YAW ];
            legsAngles[ PITCH ] = 0.0f;
            legsAngles[ ROLL ] = 0.0f;
        }
    }

    // pain twitch
    AddPainTwitch(cent, torsoAngles);

    // pull the angles back out of the hierarchial chain
    AnglesSubtract(headAngles, torsoAngles, headAngles);
    AnglesSubtract(torsoAngles, legsAngles, torsoAngles);
    AnglesToAxis(legsAngles, legs);
    AnglesToAxis(torsoAngles, torso);
    AnglesToAxis(headAngles, head);
}

#define MODEL_WWSMOOTHTIME 200

/*
===============
idCGamePlayers::PlayerWWSmoothing

Smooth the angles of transitioning wall walkers
===============
*/
void idCGamePlayers::PlayerWWSmoothing(centity_t *cent, vec3_t in[3],
                                       vec3_t out[3]) {
    entityState_t *es = &cent->currentState;
    sint i;
    vec3_t surfNormal, rotAxis, temp;
    vec3_t refNormal = { 0.0f, 0.0f,  1.0f };
    vec3_t ceilingNormal = { 0.0f, 0.0f, -1.0f };
    float32 stLocal, sFraction, rotAngle;
    vec3_t inAxis[3], lastAxis[3], outAxis[3];

    //set surfNormal
    if(!(es->eFlags & EF_WALLCLIMB)) {
        VectorCopy(refNormal, surfNormal);
    } else if(!(es->eFlags & EF_WALLCLIMBCEILING)) {
        VectorCopy(es->angles2, surfNormal);
    } else {
        VectorCopy(ceilingNormal, surfNormal);
    }

    AxisCopy(in, inAxis);

    if(!VectorCompare(surfNormal, cent->pe.lastNormal)) {
        //if we moving from the ceiling to the floor special case
        //( x product of colinear vectors is undefined)
        if(VectorCompare(ceilingNormal, cent->pe.lastNormal) &&
                VectorCompare(refNormal, surfNormal)) {
            VectorCopy(in[ 1 ], rotAxis);
            rotAngle = 180.0f;
        } else {
            AxisCopy(cent->pe.lastAxis, lastAxis);
            rotAngle = DotProduct(inAxis[ 0 ], lastAxis[ 0 ]) + DotProduct(inAxis[ 1 ],
                       lastAxis[ 1 ]) + DotProduct(inAxis[ 2 ], lastAxis[ 2 ]);

            rotAngle = RAD2DEG(acos((rotAngle - 1.0f) / 2.0f));

            CrossProduct(lastAxis[ 0 ], inAxis[ 0 ], temp);
            VectorCopy(temp, rotAxis);
            CrossProduct(lastAxis[ 1 ], inAxis[ 1 ], temp);
            VectorAdd(rotAxis, temp, rotAxis);
            CrossProduct(lastAxis[ 2 ], inAxis[ 2 ], temp);
            VectorAdd(rotAxis, temp, rotAxis);

            VectorNormalize(rotAxis);
        }

        //iterate through smooth array
        for(i = 0; i < MAXSMOOTHS; i++) {
            //found an unused index in the smooth array
            if(cent->pe.sList[ i ].time + MODEL_WWSMOOTHTIME < cg.time) {
                //copy to array and stop
                VectorCopy(rotAxis, cent->pe.sList[ i ].rotAxis);
                cent->pe.sList[ i ].rotAngle = rotAngle;
                cent->pe.sList[ i ].time = cg.time;
                break;
            }
        }
    }

    //iterate through ops
    for(i = MAXSMOOTHS - 1; i >= 0; i--) {
        //if this op has time remaining, perform it
        if(cg.time < cent->pe.sList[ i ].time + MODEL_WWSMOOTHTIME) {
            stLocal = 1.0f - (((cent->pe.sList[ i ].time + MODEL_WWSMOOTHTIME) -
                               cg.time) / MODEL_WWSMOOTHTIME);
            sFraction = -(cos(stLocal * M_PI) + 1.0f) / 2.0f;

            RotatePointAroundVector(outAxis[ 0 ], cent->pe.sList[ i ].rotAxis,
                                    inAxis[ 0 ], sFraction * cent->pe.sList[ i ].rotAngle);
            RotatePointAroundVector(outAxis[ 1 ], cent->pe.sList[ i ].rotAxis,
                                    inAxis[ 1 ], sFraction * cent->pe.sList[ i ].rotAngle);
            RotatePointAroundVector(outAxis[ 2 ], cent->pe.sList[ i ].rotAxis,
                                    inAxis[ 2 ], sFraction * cent->pe.sList[ i ].rotAngle);

            AxisCopy(outAxis, inAxis);
        }
    }

    //outAxis has been copied to inAxis
    AxisCopy(inAxis, out);
}

/*
===============
idCGamePlayers::PlayerNonSegAngles

Resolve angles for non-segmented models
===============
*/
void idCGamePlayers::PlayerNonSegAngles(centity_t *cent, vec3_t srcAngles,
                                        vec3_t nonSegAxis[ 3 ]) {
    vec3_t localAngles, velocity, surfNormal, ceilingNormal = { 0.0f, 0.0f, -1.0f };
    float32 speed;
    sint dir;
    entityState_t *es = &cent->currentState;

    VectorCopy(srcAngles, localAngles);
    localAngles[ YAW ] = AngleMod(localAngles[ YAW ]);
    localAngles[ PITCH ] = 0.0f;
    localAngles[ ROLL ] = 0.0f;

    //set surfNormal
    if(!(es->eFlags & EF_WALLCLIMBCEILING)) {
        VectorCopy(es->angles2, surfNormal);
    } else {
        VectorCopy(ceilingNormal, surfNormal);
    }

    //make sure that WW transitions don't cause the swing stuff to go nuts
    if(!VectorCompare(surfNormal, cent->pe.lastNormal)) {
        //stop idCGameLocal::SwingAngles having an eppy
        cent->pe.nonseg.yawAngle = localAngles[ YAW ];
        cent->pe.nonseg.yawing = false;
    }

    // --------- yaw -------------

    // allow yaw to drift a bit
    if((cent->currentState.legsAnim & ~ANIM_TOGGLEBIT) != NSPA_STAND) {
        // if not standing still, always point all in the same direction
        cent->pe.nonseg.yawing = true; // always center
    }

    // adjust legs for movement dir
    if(cent->currentState.eFlags & EF_DEAD) {
        // don't let dead bodies twitch
        dir = 0;
    } else {
        // did use angles2.. now uses time2.. looks a bit funny but time2 isn't used othwise
        dir = cent->currentState.time2;

        if(dir < 0 || dir > 7) {
            Error("Bad player movement angle");
        }
    }

    // torso
    if(cent->currentState.eFlags & EF_DEAD) {
        SwingAngles(localAngles[ YAW ], 0, 0, cg_swingSpeed.value,
                    &cent->pe.nonseg.yawAngle, &cent->pe.nonseg.yawing);
    } else {
        SwingAngles(localAngles[ YAW ], 40, 90, cg_swingSpeed.value,
                    &cent->pe.nonseg.yawAngle, &cent->pe.nonseg.yawing);
    }

    localAngles[ YAW ] = cent->pe.nonseg.yawAngle;

    // --------- pitch -------------

    //NO PITCH!


    // --------- roll -------------


    // lean towards the direction of travel
    VectorCopy(cent->currentState.pos.trDelta, velocity);
    speed = VectorNormalize(velocity);

    if(speed) {
        vec3_t  axis[ 3 ];
        float32   side;

        //much less than with the regular model system
        speed *= 0.01f;

        AnglesToAxis(localAngles, axis);
        side = speed * DotProduct(velocity, axis[ 1 ]);
        localAngles[ ROLL ] -= side;

        side = speed * DotProduct(velocity, axis[ 0 ]);
        localAngles[ PITCH ] += side;
    }

    //FIXME: PAIN[123] animations?
    // pain twitch
    //idCGameLocal::AddPainTwitch( cent, torsoAngles );

    AnglesToAxis(localAngles, nonSegAxis);
}

/*
===============
idCGamePlayers::PlayerUpgrade
===============
*/
void idCGamePlayers::PlayerUpgrades(centity_t *cent, refEntity_t *torso) {
    sint held, active;
    refEntity_t jetpack, battpack, flash;
    entityState_t *es = &cent->currentState;

    held = es->modelindex;
    active = es->modelindex2;

    if(held & (1 << UP_JETPACK)) {
        ::memset(&jetpack, 0, sizeof(jetpack));
        VectorCopy(torso->lightingOrigin, jetpack.lightingOrigin);
        jetpack.shadowPlane = torso->shadowPlane;
        jetpack.renderfx = torso->renderfx;

        jetpack.hModel = cgs.media.jetpackModel;

        //identity matrix
        AxisCopy(axisDefault, jetpack.axis);

        //FIXME: change to tag_back when it exists
        idCGameEnts::PositionRotatedEntityOnTag(&jetpack, torso, torso->hModel,
                                                "tag_head");

        trap_R_AddRefEntityToScene(&jetpack);

        if(active & (1 << UP_JETPACK)) {
            if(es->pos.trDelta[ 2 ] > 10.0f) {
                if(cent->jetPackState != JPS_ASCENDING) {
                    if(idCGameParticles::IsParticleSystemValid(&cent->jetPackPS)) {
                        idCGameParticles::DestroyParticleSystem(&cent->jetPackPS);
                    }

                    cent->jetPackPS = idCGameParticles::SpawnNewParticleSystem(
                                          cgs.media.jetPackAscendPS);
                    cent->jetPackState = JPS_ASCENDING;
                }

                trap_S_AddLoopingSound(cent->currentState.number, cent->lerpOrigin,
                                       vec3_origin, cgs.media.jetpackAscendSound);
            } else if(es->pos.trDelta[ 2 ] < -10.0f) {
                if(cent->jetPackState != JPS_DESCENDING) {
                    if(idCGameParticles::IsParticleSystemValid(&cent->jetPackPS)) {
                        idCGameParticles::DestroyParticleSystem(&cent->jetPackPS);
                    }

                    cent->jetPackPS = idCGameParticles::SpawnNewParticleSystem(
                                          cgs.media.jetPackDescendPS);
                    cent->jetPackState = JPS_DESCENDING;
                }

                trap_S_AddLoopingSound(cent->currentState.number, cent->lerpOrigin,
                                       vec3_origin, cgs.media.jetpackDescendSound);
            } else {
                if(cent->jetPackState != JPS_HOVERING) {
                    if(idCGameParticles::IsParticleSystemValid(&cent->jetPackPS)) {
                        idCGameParticles::DestroyParticleSystem(&cent->jetPackPS);
                    }

                    cent->jetPackPS = idCGameParticles::SpawnNewParticleSystem(
                                          cgs.media.jetPackHoverPS);
                    cent->jetPackState = JPS_HOVERING;
                }

                trap_S_AddLoopingSound(cent->currentState.number, cent->lerpOrigin,
                                       vec3_origin, cgs.media.jetpackIdleSound);
            }

            ::memset(&flash, 0, sizeof(flash));
            VectorCopy(torso->lightingOrigin, flash.lightingOrigin);
            flash.shadowPlane = torso->shadowPlane;
            flash.renderfx = torso->renderfx;

            flash.hModel = cgs.media.jetpackFlashModel;

            if(!flash.hModel) {
                return;
            }

            AxisCopy(axisDefault, flash.axis);

            idCGameEnts::PositionRotatedEntityOnTag(&flash, &jetpack, jetpack.hModel,
                                                    "tag_flash");
            trap_R_AddRefEntityToScene(&flash);

            if(idCGameParticles::IsParticleSystemValid(&cent->jetPackPS)) {
                idCGameAttachment::SetAttachmentTag(&cent->jetPackPS->attachment, jetpack,
                                                    jetpack.hModel, "tag_flash");
                idCGameAttachment::SetAttachmentCent(&cent->jetPackPS->attachment, cent);
                idCGameAttachment::AttachToTag(&cent->jetPackPS->attachment);
            }
        } else if(idCGameParticles::IsParticleSystemValid(&cent->jetPackPS)) {
            idCGameParticles::DestroyParticleSystem(&cent->jetPackPS);
            cent->jetPackState = JPS_OFF;
        }
    } else if(idCGameParticles::IsParticleSystemValid(&cent->jetPackPS)) {
        idCGameParticles::DestroyParticleSystem(&cent->jetPackPS);
        cent->jetPackState = JPS_OFF;
    }

    if(held & (1 << UP_BATTPACK)) {
        ::memset(&battpack, 0, sizeof(battpack));
        VectorCopy(torso->lightingOrigin, battpack.lightingOrigin);
        battpack.shadowPlane = torso->shadowPlane;
        battpack.renderfx = torso->renderfx;

        battpack.hModel = cgs.media.battpackModel;

        //identity matrix
        AxisCopy(axisDefault, battpack.axis);

        //FIXME: change to tag_back when it exists
        idCGameEnts::PositionRotatedEntityOnTag(&battpack, torso, torso->hModel,
                                                "tag_head");

        trap_R_AddRefEntityToScene(&battpack);
    }

    if(es->eFlags & EF_BLOBLOCKED) {
        vec3_t temp, origin, up = { 0.0f, 0.0f, 1.0f };
        trace_t tr;
        float32 size;

        VectorCopy(es->pos.trBase, temp);
        temp[ 2 ] -= 4096.0f;

        idCGamePredict::Trace(&tr, es->pos.trBase, nullptr, nullptr, temp,
                              es->number, MASK_SOLID);
        VectorCopy(tr.endpos, origin);

        size = 32.0f;

        if(size > 0.0f) {
            idCGameMarks::ImpactMark(cgs.media.creepShader, origin, up, 0.0f, 1.0f,
                                     1.0f, 1.0f, 1.0f, false, size, true);
        }
    }
}

/*
===============
idCGamePlayers::PlayerFloatSprite

Float a sprite over the player's head
===============
*/
void idCGamePlayers::PlayerFloatSprite(centity_t *cent, qhandle_t shader) {
    sint rf;
    refEntity_t ent;

    if(cent->currentState.number == cg.snap->ps.clientNum &&
            !cg.renderingThirdPerson) {
        rf = RF_THIRD_PERSON;   // only show in mirrors
    } else {
        rf = 0;
    }

    ::memset(&ent, 0, sizeof(ent));
    VectorCopy(cent->lerpOrigin, ent.origin);
    ent.origin[ 2 ] += 48;
    ent.reType = RT_SPRITE;
    ent.customShader = shader;
    ent.radius = 10;
    ent.renderfx = rf;
    ent.shaderRGBA[ 0 ] = 255;
    ent.shaderRGBA[ 1 ] = 255;
    ent.shaderRGBA[ 2 ] = 255;
    ent.shaderRGBA[ 3 ] = 255;
    trap_R_AddRefEntityToScene(&ent);
}

/*
===============
idCGamePlayers::PlayerSprites

Float sprites over the player's head
===============
*/
void idCGamePlayers::PlayerSprites(centity_t *cent) {
    if(cent->currentState.eFlags & EF_CONNECTION) {
        PlayerFloatSprite(cent, cgs.media.connectionShader);
        return;
    }
}

/*
===============
idCGamePlayers::PlayerShadow

Returns the Z component of the surface being shadowed
should it return a full plane instead of a Z?
===============
*/
#define SHADOW_DISTANCE   128
bool idCGamePlayers::PlayerShadow(centity_t *cent, float32 *shadowPlane,
                                  class_t _class) {
    vec3_t end, mins, maxs, surfNormal = { 0.0f, 0.0f, 1.0f };
    trace_t trace;
    float32 alpha;
    entityState_t *es = &cent->currentState;

    bggame->ClassBoundingBox(_class, mins, maxs, nullptr, nullptr, nullptr);
    mins[2] = 0.0f;
    maxs[2] = 2.0f;

    if(es->eFlags & EF_WALLCLIMB) {
        if(es->eFlags & EF_WALLCLIMBCEILING) {
            VectorSet(surfNormal, 0.0f, 0.0f, -1.0f);
        } else {
            VectorCopy(es->angles2, surfNormal);
        }
    }

    *shadowPlane = 0;

    if(cg_shadows.integer == 0) {
        return false;
    }

    // send a trace down from the player to the ground
    VectorCopy(cent->lerpOrigin, end);
    VectorMA(cent->lerpOrigin, -SHADOW_DISTANCE, surfNormal, end);

    trap_CM_BoxTrace(&trace, cent->lerpOrigin, end, mins, maxs, 0,
                     MASK_PLAYERSOLID);

    // no shadow if too high
    if(trace.fraction == 1.0 || trace.startsolid || trace.allsolid) {
        return false;
    }

    // FIXME: stencil shadows will be broken for walls.
    // Unfortunately there isn't much that can be
    // done since Q3 references only the Z coord
    // of the shadowPlane
    if(surfNormal[2] < 0.0f) {
        *shadowPlane = trace.endpos[2] - 1.0f;
    } else {
        *shadowPlane = trace.endpos[2] + 1.0f;
    }

    if(cg_shadows.integer != 1) {  // no mark for stencil or projection shadows
        return true;
    }

    // fade the shadow out with height
    alpha = 1.0 - trace.fraction;

    // add the mark as a temporary, so it goes directly to the renderer
    // without taking a spot in the cg_marks array
    idCGameMarks::ImpactMark(cgs.media.shadowMarkShader, trace.endpos,
                             trace.plane.normal, cent->pe.legs.yawAngle, 0.0f, 0.0f, 0.0f, alpha,
                             false, 24.0f * bggame->ClassConfig(_class)->shadowScale, true);

    return true;
}

/*
===============
idCGamePlayers::PlayerSplash

Draw a mark at the water surface
===============
*/
void idCGamePlayers::PlayerSplash(centity_t *cent, class_t _class) {
    vec3_t start, end, mins, maxs;
    trace_t trace;
    sint contents;

    if(!cg_shadows.integer) {
        return;
    }

    bggame->ClassBoundingBox(_class, mins, maxs, nullptr, nullptr, nullptr);

    VectorCopy(cent->lerpOrigin, end);
    end[ 2 ] += mins[ 2 ];

    // if the feet aren't in liquid, don't make a mark
    // this won't handle moving water brushes, but they wouldn't draw right anyway...
    contents = trap_CM_PointContents(end, 0);

    if(!(contents & (CONTENTS_WATER | CONTENTS_SLIME | CONTENTS_LAVA))) {
        return;
    }

    VectorCopy(cent->lerpOrigin, start);
    start[ 2 ] += 32;

    // if the head isn't out of liquid, don't make a mark
    contents = trap_CM_PointContents(start, 0);

    if(contents & (CONTENTS_SOLID | CONTENTS_WATER | CONTENTS_SLIME |
                   CONTENTS_LAVA)) {
        return;
    }

    // trace down to find the surface
    trap_CM_BoxTrace(&trace, start, end, nullptr, nullptr, 0,
                     (CONTENTS_WATER | CONTENTS_SLIME | CONTENTS_LAVA));

    if(trace.fraction == 1.0f) {
        return;
    }

    idCGameMarks::ImpactMark(cgs.media.wakeMarkShader, trace.endpos,
                             trace.plane.normal, cent->pe.legs.yawAngle, 1.0f, 1.0f, 1.0f, 1.0f,
                             false, 32.0f * bggame->ClassConfig(_class)->shadowScale, true);
}

/*
=================
idCGameLocal::LightVerts
=================
*/
sint idCGamePlayers::LightVerts(vec3_t normal, sint numVerts,
                                polyVert_t *verts) {
    sint i, j;
    float32 incoming;
    vec3_t ambientLight, lightDir, directedLight;

    trap_R_LightForPoint(verts[ 0 ].xyz, ambientLight, directedLight,
                         lightDir);

    for(i = 0; i < numVerts; i++) {
        incoming = DotProduct(normal, lightDir);

        if(incoming <= 0) {
            verts[ i ].modulate[ 0 ] = ambientLight[ 0 ];
            verts[ i ].modulate[ 1 ] = ambientLight[ 1 ];
            verts[ i ].modulate[ 2 ] = ambientLight[ 2 ];
            verts[ i ].modulate[ 3 ] = 255;
            continue;
        }

        j = (ambientLight[ 0 ] + incoming * directedLight[ 0 ]);

        if(j > 255) {
            j = 255;
        }

        verts[ i ].modulate[ 0 ] = j;

        j = (ambientLight[ 1 ] + incoming * directedLight[ 1 ]);

        if(j > 255) {
            j = 255;
        }

        verts[ i ].modulate[ 1 ] = j;

        j = (ambientLight[ 2 ] + incoming * directedLight[ 2 ]);

        if(j > 255) {
            j = 255;
        }

        verts[ i ].modulate[ 2 ] = j;

        verts[ i ].modulate[ 3 ] = 255;
    }

    return true;
}

/*
=================
idCGamePlayers::LightFromDirection
=================
*/
sint idCGamePlayers::LightFromDirection(vec3_t point, vec3_t direction) {
    sint j;
    float32 incoming;
    vec3_t ambientLight, lightDir, directedLight, result;

    trap_R_LightForPoint(point, ambientLight, directedLight, lightDir);

    incoming = DotProduct(direction, lightDir);

    if(incoming <= 0) {
        result[ 0 ] = ambientLight[ 0 ];
        result[ 1 ] = ambientLight[ 1 ];
        result[ 2 ] = ambientLight[ 2 ];
        return (sint)((float32)(result[ 0 ] + result[ 1 ] + result[ 2 ]) / 3.0f);
    }

    j = (ambientLight[ 0 ] + incoming * directedLight[ 0 ]);

    if(j > 255) {
        j = 255;
    }

    result[ 0 ] = j;

    j = (ambientLight[ 1 ] + incoming * directedLight[ 1 ]);

    if(j > 255) {
        j = 255;
    }

    result[ 1 ] = j;

    j = (ambientLight[ 2 ] + incoming * directedLight[ 2 ]);

    if(j > 255) {
        j = 255;
    }

    result[ 2 ] = j;

    return (sint)((float32)(result[ 0 ] + result[ 1 ] + result[ 2 ]) / 3.0f);
}

/*
=================
idCGamePlayers::AmbientLight
=================
*/
sint idCGamePlayers::AmbientLight(vec3_t point) {
    vec3_t ambientLight, lightDir, directedLight, result;

    trap_R_LightForPoint(point, ambientLight, directedLight, lightDir);

    result[ 0 ] = ambientLight[ 0 ];
    result[ 1 ] = ambientLight[ 1 ];
    result[ 2 ] = ambientLight[ 2 ];
    return (sint)((float32)(result[ 0 ] + result[ 1 ] + result[ 2 ]) / 3.0f);
}

/*
===============
idCGamePlayers::Player
===============
*/
#define TRACE_DEPTH 32.0f
void idCGamePlayers::Player(centity_t *cent) {
    clientInfo_t *ci;

    // NOTE: legs is used for nonsegmented models
    //       this helps reduce code to be changed
    refEntity_t legs, torso, head;
    entityState_t *es = &cent->currentState;
    sint clientNum, renderfx, held = es->modelindex;
    bool shadow = false;
    float32 shadowPlane = 0.0f, scale;
    class_t _class = (class_t)((es->misc >> 8) & 0xFF);
    vec3_t tempAxis[ 3 ], tempAxis2[ 3 ], angles, surfNormal = { 0.0f, 0.0f, 1.0f };

    // the client number is stored in clientNum.  It can't be derived
    // from the entity number, because a single client may have
    // multiple corpses on the level using the same clientinfo
    clientNum = es->clientNum;

    if(clientNum < 0 || clientNum >= MAX_CLIENTS) {
        Error("Bad clientNum on player entity");
    }

    ci = &cgs.clientinfo[ clientNum ];

    // it is possible to see corpses from disconnected players that may
    // not have valid clientinfo
    if(!ci->infoValid) {
        return;
    }

    //don't draw
    if(es->eFlags & EF_NODRAW) {
        return;
    }

    // get the player model information
    renderfx = 0;

    if(es->number == cg.snap->ps.clientNum) {
        if(!cg.renderingThirdPerson) {
            renderfx = RF_THIRD_PERSON;     // only draw in mirrors
        } else if(cg_cameraMode.integer) {
            return;
        }
    }

    if(cg_drawBBOX.integer) {
        vec3_t  mins, maxs;

        bggame->ClassBoundingBox(_class, mins, maxs, nullptr, nullptr, nullptr);
        idCGameEnts::DrawBoundingBox(cent->lerpOrigin, mins, maxs);
    }

    ::memset(&legs, 0, sizeof(legs));
    ::memset(&torso, 0, sizeof(torso));
    ::memset(&head, 0, sizeof(head));

    VectorCopy(cent->lerpAngles, angles);
    AnglesToAxis(cent->lerpAngles, tempAxis);

    //rotate lerpAngles to floor
    if(es->eFlags & EF_WALLCLIMB &&
            bggame->RotateAxis(es->angles2, tempAxis, tempAxis2, true,
                               es->eFlags & EF_WALLCLIMBCEILING)) {
        AxisToAngles(tempAxis2, angles);
    } else {
        VectorCopy(cent->lerpAngles, angles);
    }

    //normalise the pitch
    if(angles[PITCH] < -180.0f) {
        angles[PITCH] += 360.0f;
    }

    // get the rotation information
    if(!ci->nonsegmented) {
        PlayerAngles(cent, angles, legs.axis, torso.axis, head.axis);
    } else {
        PlayerNonSegAngles(cent, angles, legs.axis);
    }

    AxisCopy(legs.axis, tempAxis);

    //rotate the legs axis to back to the wall
    if(es->eFlags & EF_WALLCLIMB &&
            bggame->RotateAxis(es->angles2, legs.axis, tempAxis, false,
                               es->eFlags & EF_WALLCLIMBCEILING)) {
        AxisCopy(tempAxis, legs.axis);
    }

    //smooth out WW transitions so the model doesn't hop around
    PlayerWWSmoothing(cent, legs.axis, legs.axis);

    AxisCopy(tempAxis, cent->pe.lastAxis);

    // get the animation state (after rotation, to allow feet shuffle)
    if(!ci->nonsegmented) {
        PlayerAnimation(cent, &legs.oldframe, &legs.frame, &legs.backlerp,
                        &torso.oldframe, &torso.frame, &torso.backlerp);
    } else {
        PlayerNonSegAnimation(cent, &legs.oldframe, &legs.frame, &legs.backlerp);
    }

    // add the talk baloon or disconnect icon
    PlayerSprites(cent);

    // add the shadow
    if((es->number == cg.snap->ps.clientNum && cg.renderingThirdPerson) ||
            es->number != cg.snap->ps.clientNum) {
        shadow = PlayerShadow(cent, &shadowPlane, _class);
    }

    // add a water splash if partially in and out of water
    PlayerSplash(cent, _class);

    if(cg_shadows.integer == 3 && shadow) {
        renderfx |= RF_SHADOW_PLANE;
    }

    renderfx |= RF_LIGHTING_ORIGIN;     // use the same origin for all

    // add the legs
    if(!ci->nonsegmented) {
        legs.hModel = ci->legsModel;

        if(held & (1 << UP_LIGHTARMOUR)) {
            legs.customSkin = cgs.media.larmourLegsSkin;
        } else {
            legs.customSkin = ci->legsSkin;
        }
    } else {
        legs.hModel = ci->nonSegModel;
        legs.customSkin = ci->nonSegSkin;
    }

    VectorCopy(cent->lerpOrigin, legs.origin);

    VectorCopy(cent->lerpOrigin, legs.lightingOrigin);
    legs.shadowPlane = shadowPlane;
    legs.renderfx = renderfx;
    VectorCopy(legs.origin,
               legs.oldorigin);   // don't positionally lerp at all

    //move the origin closer into the wall with a CapTrace
    if(es->eFlags & EF_WALLCLIMB && !(es->eFlags & EF_DEAD) &&
            !(cg.intermissionStarted)) {
        vec3_t start, end, mins, maxs;
        trace_t tr;

        if(es->eFlags & EF_WALLCLIMBCEILING) {
            VectorSet(surfNormal, 0.0f, 0.0f, -1.0f);
        } else {
            VectorCopy(es->angles2, surfNormal);
        }

        bggame->ClassBoundingBox(_class, mins, maxs, nullptr, nullptr, nullptr);

        VectorMA(legs.origin, -TRACE_DEPTH, surfNormal, end);
        VectorMA(legs.origin, 1.0f, surfNormal, start);
        idCGamePredict::CapTrace(&tr, start, mins, maxs, end, es->number,
                                 MASK_PLAYERSOLID);

        //if the trace misses completely then just use legs.origin
        //apparently capsule traces are "smaller" than box traces
        if(tr.fraction != 1.0f) {
            VectorMA(legs.origin, tr.fraction * -TRACE_DEPTH, surfNormal, legs.origin);
        }

        VectorCopy(legs.origin, legs.lightingOrigin);
        VectorCopy(legs.origin,
                   legs.oldorigin);   // don't positionally lerp at all
    }

    //rescale the model
    scale = bggame->ClassConfig(_class)->modelScale;

    if(scale != 1.0f) {
        VectorScale(legs.axis[ 0 ], scale, legs.axis[ 0 ]);
        VectorScale(legs.axis[ 1 ], scale, legs.axis[ 1 ]);
        VectorScale(legs.axis[ 2 ], scale, legs.axis[ 2 ]);

        legs.nonNormalizedAxes = true;
    }

    //offset on the Z axis if required
    VectorMA(legs.origin, bggame->ClassConfig(_class)->zOffset, surfNormal,
             legs.origin);
    VectorCopy(legs.origin, legs.lightingOrigin);
    VectorCopy(legs.origin,
               legs.oldorigin);   // don't positionally lerp at all

    trap_R_AddRefEntityToScene(&legs);

    // if the model failed, allow the default nullmodel to be displayed
    if(!legs.hModel) {
        return;
    }

    if(!ci->nonsegmented) {
        // add the torso
        torso.hModel = ci->torsoModel;

        if(held & (1 << UP_LIGHTARMOUR)) {
            torso.customSkin = cgs.media.larmourTorsoSkin;
        } else {
            torso.customSkin = ci->torsoSkin;
        }

        if(!torso.hModel) {
            return;
        }

        VectorCopy(cent->lerpOrigin, torso.lightingOrigin);

        idCGameEnts::PositionRotatedEntityOnTag(&torso, &legs, ci->legsModel,
                                                "tag_torso");

        torso.shadowPlane = shadowPlane;
        torso.renderfx = renderfx;

        trap_R_AddRefEntityToScene(&torso);

        // add the head
        head.hModel = ci->headModel;

        if(held & (1 << UP_HELMET)) {
            head.customSkin = cgs.media.larmourHeadSkin;
        } else {
            head.customSkin = ci->headSkin;
        }

        if(!head.hModel) {
            return;
        }

        VectorCopy(cent->lerpOrigin, head.lightingOrigin);

        idCGameEnts::PositionRotatedEntityOnTag(&head, &torso, ci->torsoModel,
                                                "tag_head");

        head.shadowPlane = shadowPlane;
        head.renderfx = renderfx;

        trap_R_AddRefEntityToScene(&head);

        // if this player has been hit with poison cloud, add an effect PS
        if((es->eFlags & EF_POISONCLOUDED) &&
                (es->number != cg.snap->ps.clientNum || cg.renderingThirdPerson)) {
            if(!idCGameParticles::IsParticleSystemValid(&cent->poisonCloudedPS)) {
                cent->poisonCloudedPS = idCGameParticles::SpawnNewParticleSystem(
                                            cgs.media.poisonCloudedPS);
            }

            idCGameAttachment::SetAttachmentTag(&cent->poisonCloudedPS->attachment,
                                                head, head.hModel, "tag_head");
            idCGameAttachment::SetAttachmentCent(&cent->poisonCloudedPS->attachment,
                                                 cent);
            idCGameAttachment::AttachToTag(&cent->poisonCloudedPS->attachment);
        } else if(idCGameParticles::IsParticleSystemValid(
                      &cent->poisonCloudedPS)) {
            idCGameParticles::DestroyParticleSystem(&cent->poisonCloudedPS);
        }
    }

    // add the gun / barrel / flash
    if(es->weapon != WP_NONE) {
        if(!ci->nonsegmented) {
            idCGameWeapons::AddPlayerWeapon(&torso, nullptr, cent);
        } else {
            idCGameWeapons::AddPlayerWeapon(&legs, nullptr, cent);
        }
    }

    PlayerUpgrades(cent, &torso);

    //sanity check that particle systems are stopped when dead
    if(es->eFlags & EF_DEAD) {
        if(idCGameParticles::IsParticleSystemValid(&cent->muzzlePS)) {
            idCGameParticles::DestroyParticleSystem(&cent->muzzlePS);
        }

        if(idCGameParticles::IsParticleSystemValid(&cent->jetPackPS)) {
            idCGameParticles::DestroyParticleSystem(&cent->jetPackPS);
        }
    }

    VectorCopy(surfNormal, cent->pe.lastNormal);
}

/*
===============
idCGamePlayers::Corpse
===============
*/
void idCGamePlayers::Corpse(centity_t *cent) {
    clientInfo_t  *ci;
    refEntity_t legs, torso, head;
    entityState_t *es = &cent->currentState;
    sint corpseNum, renderfx;
    bool shadow = false;
    float32 shadowPlane, scale;
    vec3_t origin, liveZ, deadZ;

    corpseNum = GetCorpseNum((class_t)es->clientNum);

    if(corpseNum < 0 || corpseNum >= MAX_CLIENTS) {
        Error("Bad corpseNum on corpse entity: %d", corpseNum);
    }

    ci = &cgs.corpseinfo[ corpseNum ];

    // it is possible to see corpses from disconnected players that may
    // not have valid clientinfo
    if(!ci->infoValid) {
        return;
    }

    ::memset(&legs, 0, sizeof(legs));
    ::memset(&torso, 0, sizeof(torso));
    ::memset(&head, 0, sizeof(head));

    VectorCopy(cent->lerpOrigin, origin);
    bggame->ClassBoundingBox((class_t)es->clientNum, liveZ, nullptr, nullptr,
                             deadZ, nullptr);
    origin[ 2 ] -= (liveZ[ 2 ] - deadZ[ 2 ]);

    VectorCopy(es->angles, cent->lerpAngles);

    // get the rotation information
    if(!ci->nonsegmented) {
        PlayerAngles(cent, cent->lerpAngles, legs.axis, torso.axis, head.axis);
    } else {
        PlayerNonSegAngles(cent, cent->lerpAngles, legs.axis);
    }

    //set the correct frame (should always be dead)
    if(cg_noPlayerAnims.integer) {
        legs.oldframe = legs.frame = torso.oldframe = torso.frame = 0;
    } else if(!ci->nonsegmented) {
        ::memset(&cent->pe.legs, 0, sizeof(lerpFrame_t));

        RunPlayerLerpFrame(ci, &cent->pe.legs, es->legsAnim, 1);
        legs.oldframe = cent->pe.legs.oldFrame;
        legs.frame = cent->pe.legs.frame;
        legs.backlerp = cent->pe.legs.backlerp;

        ::memset(&cent->pe.torso, 0, sizeof(lerpFrame_t));

        RunPlayerLerpFrame(ci, &cent->pe.torso, es->torsoAnim, 1);
        torso.oldframe = cent->pe.torso.oldFrame;
        torso.frame = cent->pe.torso.frame;
        torso.backlerp = cent->pe.torso.backlerp;
    } else {
        ::memset(&cent->pe.nonseg, 0, sizeof(lerpFrame_t));

        RunPlayerLerpFrame(ci, &cent->pe.nonseg, es->legsAnim, 1);
        legs.oldframe = cent->pe.nonseg.oldFrame;
        legs.frame = cent->pe.nonseg.frame;
        legs.backlerp = cent->pe.nonseg.backlerp;
    }

    // add the shadow
    shadow = PlayerShadow(cent, &shadowPlane, (class_t)es->clientNum);

    // get the player model information
    renderfx = 0;

    if(cg_shadows.integer == 3 && shadow) {
        renderfx |= RF_SHADOW_PLANE;
    }

    renderfx |= RF_LIGHTING_ORIGIN;     // use the same origin for all

    // add the legs
    if(!ci->nonsegmented) {
        legs.hModel = ci->legsModel;
        legs.customSkin = ci->legsSkin;
    } else {
        legs.hModel = ci->nonSegModel;
        legs.customSkin = ci->nonSegSkin;
    }

    VectorCopy(origin, legs.origin);

    VectorCopy(origin, legs.lightingOrigin);
    legs.shadowPlane = shadowPlane;
    legs.renderfx = renderfx;
    legs.origin[ 2 ] += bggame->ClassConfig((class_t)es->clientNum)->zOffset;
    VectorCopy(legs.origin,
               legs.oldorigin);   // don't positionally lerp at all

    //rescale the model
    scale = bggame->ClassConfig((class_t)es->clientNum)->modelScale;

    if(scale != 1.0f) {
        VectorScale(legs.axis[ 0 ], scale, legs.axis[ 0 ]);
        VectorScale(legs.axis[ 1 ], scale, legs.axis[ 1 ]);
        VectorScale(legs.axis[ 2 ], scale, legs.axis[ 2 ]);

        legs.nonNormalizedAxes = true;
    }

    trap_R_AddRefEntityToScene(&legs);

    // if the model failed, allow the default nullmodel to be displayed
    if(!legs.hModel) {
        return;
    }

    if(!ci->nonsegmented) {
        // add the torso
        torso.hModel = ci->torsoModel;

        if(!torso.hModel) {
            return;
        }

        torso.customSkin = ci->torsoSkin;

        VectorCopy(origin, torso.lightingOrigin);

        idCGameEnts::PositionRotatedEntityOnTag(&torso, &legs, ci->legsModel,
                                                "tag_torso");

        torso.shadowPlane = shadowPlane;
        torso.renderfx = renderfx;

        trap_R_AddRefEntityToScene(&torso);

        // add the head
        head.hModel = ci->headModel;

        if(!head.hModel) {
            return;
        }

        head.customSkin = ci->headSkin;

        VectorCopy(origin, head.lightingOrigin);

        idCGameEnts::PositionRotatedEntityOnTag(&head, &torso, ci->torsoModel,
                                                "tag_head");

        head.shadowPlane = shadowPlane;
        head.renderfx = renderfx;

        trap_R_AddRefEntityToScene(&head);
    }
}

/*
===============
idCGameLocal::GetTag
===============
*/
bool idCGameLocal::GetTag(sint clientNum, valueType *tagname,
                          orientation_t *_or) {
    clientInfo_t *ci;
    centity_t *cent;
    refEntity_t *refent;
    vec3_t tempAxis[3];
    vec3_t org;
    sint i;

    ci = &cgs.clientinfo[ clientNum ];

    if(cg.snap && clientNum == cg.snap->ps.clientNum &&
            cg.renderingThirdPerson) {
        cent = &cg.predictedPlayerEntity;
    } else {
        cent = &cg_entities[ci->clientNum];

        if(!cent->currentValid) {
            return false;      // not currently in PVS
        }
    }

    refent = &cent->pe.bodyRefEnt;

    VectorCopy(refent->origin, org);

    for(i = 0 ; i < 3 ; i++) {
        VectorMA(org, _or->origin[i], refent->axis[i], org);
    }

    VectorCopy(org, _or->origin);

    // rotate with entity
    AxisMultiply(refent->axis, _or->axis, tempAxis);
    ::memcpy(_or->axis, tempAxis, sizeof(vec3_t) * 3);

    return true;
}

/*
===============
idCGamePlayers::ResetPlayerEntity

A player just came into view or teleported, so reset all animation info
===============
*/
void idCGamePlayers::ResetPlayerEntity(centity_t *cent) {
    cent->errorTime = -99999;   // guarantee no error decay added
    cent->extrapolated = false;

    ClearLerpFrame(&cgs.clientinfo[ cent->currentState.clientNum ],
                   &cent->pe.legs, cent->currentState.legsAnim);
    ClearLerpFrame(&cgs.clientinfo[ cent->currentState.clientNum ],
                   &cent->pe.torso, cent->currentState.torsoAnim);
    ClearLerpFrame(&cgs.clientinfo[ cent->currentState.clientNum ],
                   &cent->pe.nonseg, cent->currentState.legsAnim);

    bggame->EvaluateTrajectory(&cent->currentState.pos, cg.time,
                               cent->lerpOrigin);
    bggame->EvaluateTrajectory(&cent->currentState.apos, cg.time,
                               cent->lerpAngles);

    VectorCopy(cent->lerpOrigin, cent->rawOrigin);
    VectorCopy(cent->lerpAngles, cent->rawAngles);

    ::memset(&cent->pe.legs, 0, sizeof(cent->pe.legs));

    cent->pe.legs.yawAngle = cent->rawAngles[ YAW ];
    cent->pe.legs.yawing = false;
    cent->pe.legs.pitchAngle = 0;
    cent->pe.legs.pitching = false;

    ::memset(&cent->pe.torso, 0, sizeof(cent->pe.legs));

    cent->pe.torso.yawAngle = cent->rawAngles[ YAW ];
    cent->pe.torso.yawing = false;
    cent->pe.torso.pitchAngle = cent->rawAngles[ PITCH ];
    cent->pe.torso.pitching = false;

    ::memset(&cent->pe.nonseg, 0, sizeof(cent->pe.nonseg));

    cent->pe.nonseg.yawAngle = cent->rawAngles[ YAW ];
    cent->pe.nonseg.yawing = false;
    cent->pe.nonseg.pitchAngle = cent->rawAngles[ PITCH ];
    cent->pe.nonseg.pitching = false;

    if(cg_debugPosition.integer) {
        Printf("%i ResetPlayerEntity yaw=%i\n", cent->currentState.number,
               cent->pe.torso.yawAngle);
    }
}

/*
==================
idCGamePlayers::PlayerDisconnect

Player disconnecting
==================
*/
void idCGamePlayers::PlayerDisconnect(vec3_t org) {
    particleSystem_t  *ps;

    trap_S_StartSound(org, ENTITYNUM_WORLD, CHAN_AUTO,
                      cgs.media.disconnectSound);

    ps = idCGameParticles::SpawnNewParticleSystem(cgs.media.disconnectPS);

    if(idCGameParticles::IsParticleSystemValid(&ps)) {
        idCGameAttachment::SetAttachmentPoint(&ps->attachment, org);
        idCGameAttachment::AttachToPoint(&ps->attachment);
    }
}

/*
=================
idCGamePlayers::Bleed

This is the spurt of blood when a character gets hit
=================
*/
void idCGamePlayers::Bleed(vec3_t origin, vec3_t normal, sint entityNum) {
    team_t team = cgs.clientinfo[ entityNum ].team;
    qhandle_t bleedPS;
    particleSystem_t *ps;

    if(!cg_blood.integer) {
        return;
    }

    if(team == TEAM_ALIENS) {
        bleedPS = cgs.media.alienBleedPS;
    } else if(team == TEAM_HUMANS) {
        bleedPS = cgs.media.humanBleedPS;
    } else {
        return;
    }

    ps = idCGameParticles::SpawnNewParticleSystem(bleedPS);

    if(idCGameParticles::IsParticleSystemValid(&ps)) {
        idCGameAttachment::SetAttachmentPoint(&ps->attachment, origin);
        idCGameAttachment::SetAttachmentCent(&ps->attachment,
                                             &cg_entities[ entityNum ]);
        idCGameAttachment::AttachToPoint(&ps->attachment);

        idCGameParticles::SetParticleSystemNormal(ps, normal);
    }
}

/*
==================
idCGameLocal::TeamStatusDisplay
==================
*/
#define STATUS_FADE_TIME      200
#define STATUS_MAX_VIEW_DIST  900.0f
#define STATUS_PEEK_DIST      20
void idCGamePlayers::TeamStatusDisplay(centity_t *cent) {
    entityState_t *es = &cent->currentState;
    vec3_t origin;
    class_t pclass = (class_t)((es->misc >> 8) & 0xFF);
    sint health = cgs.clientinfo[ es->clientNum ].health;
    //sint credits = cgs.clientinfo[ es->clientNum ].armor; currently unused
    float32 healthScale = bggame->Class(pclass)->health, x, y;
    vec4_t color;
    trace_t tr;
    float32 d;
    buildStat_t *bs;
    sint i, j, entNum;
    vec3_t trOrigin, right;
    bool visible = false;
    vec3_t mins, maxs;
    entityState_t *hit;
    float32 transparency = 0.5f;

    if((cent->currentState.misc & 0x00FF) == TEAM_ALIENS) {
        bs = &cgs.alienBuildStat;
    } else {
        bs = &cgs.humanBuildStat;
    }

    if(!bs->loaded) {
        return;
    }

    d = Distance(cent->lerpOrigin, cg.refdef.vieworg);

    if(d > STATUS_MAX_VIEW_DIST) {
        return;
    }

    Vector4Copy(bs->foreColor, color);

    // trace for top
    if((es->legsAnim & ~ANIM_TOGGLEBIT) == LEGS_WALKCR ||
            (es->legsAnim & ~ANIM_TOGGLEBIT) == LEGS_IDLECR) {
        bggame->ClassBoundingBox(pclass, mins, nullptr, maxs, nullptr, nullptr);
    } else {
        bggame->ClassBoundingBox(pclass, mins, maxs, nullptr, nullptr, nullptr);
    }

    VectorCopy(cent->lerpOrigin, origin);

    // top point
    origin[ 2 ] += mins[ 2 ];
    origin[ 2 ] += (fabs(mins[ 2 ]) + fabs(maxs[ 2 ]));
    //origin[ 2 ] *= 1.1f;

    entNum = cg.predictedPlayerState.clientNum;

    // if first try fails, step left, step right
    for(j = 0; j < 3; j++) {
        VectorCopy(cg.refdef.vieworg, trOrigin);

        switch(j) {
            case 1:
                // step right
                AngleVectors(cg.refdefViewAngles, nullptr, right, nullptr);
                VectorMA(trOrigin, STATUS_PEEK_DIST, right, trOrigin);
                break;

            case 2:
                // step left
                AngleVectors(cg.refdefViewAngles, nullptr, right, nullptr);
                VectorMA(trOrigin, -STATUS_PEEK_DIST, right, trOrigin);
                break;

            default:
                break;
        }

        // look through up to 3 players and/or transparent buildables
        for(i = 0; i < 3; i++) {
            idCGamePredict::Trace(&tr, trOrigin, nullptr, nullptr, origin, entNum,
                                  MASK_SHOT);

            if(tr.entityNum == cent->currentState.number) {
                visible = true;
                break;
            }

            if(tr.entityNum == ENTITYNUM_WORLD) {
                break;
            }

            hit  = &cg_entities[ tr.entityNum ].currentState;

            if(tr.entityNum < MAX_CLIENTS || (hit->eType == ET_BUILDABLE &&
                                              (!(es->eFlags & EF_B_SPAWNED) ||
                                               bggame->Buildable((buildable_t)hit->modelindex)->transparentTest)))

            {
                entNum = tr.entityNum;
                VectorCopy(tr.endpos, trOrigin);
            } else {
                break;
            }
        }
    }

    // hack to make the kit obscure view
    if(cg_drawGun.integer && visible &&
            cg.predictedPlayerState.stats[ STAT_TEAM ] == TEAM_HUMANS &&
            idCGameDrawTools::WorldToScreen(origin, &x, &y)) {
        if(x > 450 && y > 260) {
            visible = false;
        }
    }

    healthScale = (float32)health / healthScale;

    if(health > 0 && healthScale < 0.01f) {
        healthScale = 0.01f;
    } else if(healthScale < 0.0f) {
        healthScale = 0.0f;
    } else if(healthScale > 1.0f) {
        healthScale = 1.0f;
    }

    if(cg_hideHealthyTeamStatus.integer && healthScale == 1.0f) {
        visible = false;
    }

    if(!visible && cent->buildableStatus.visible) {
        cent->buildableStatus.visible   = false;
        cent->buildableStatus.lastTime  = cg.time;
    } else if(visible && !cent->buildableStatus.visible) {
        cent->buildableStatus.visible   = true;
        cent->buildableStatus.lastTime  = cg.time;
    }

    color[ 3 ] = transparency;

    // Fade up
    if(cent->buildableStatus.visible) {
        if(cent->buildableStatus.lastTime + STATUS_FADE_TIME > cg.time) {
            color[3] = transparency * ((float32)(cg.time -
                                                 cent->buildableStatus.lastTime) / STATUS_FADE_TIME);
        }
    }

    // Fade down
    if(!cent->buildableStatus.visible) {
        if(cent->buildableStatus.lastTime + STATUS_FADE_TIME > cg.time) {
            color[3] = transparency * (1.0f - (float32)(cg.time -
                                       cent->buildableStatus.lastTime) / STATUS_FADE_TIME);
        } else {
            return;
        }
    }

    if(idCGameDrawTools::WorldToScreen(origin, &x, &y)) {
        float32 picH = bs->frameHeight * 0.85;
        float32 picW = bs->frameWidth * 0.3;
        float32 picX = x;
        float32 picY = y;
        float32 scale;
        float32 subH, subY;
        vec4_t frameColor;

        // this is fudged to get the width/height in the cfg to be more realistic
        scale = (picH / d) * 3;

        picH *= scale;
        picW *= scale;
        picX -= (picW * 0.5f);
        picY -= (picH * 0.5f);

        // sub-elements such as icons and number
        subH = picH - (picH * bs->verticalMargin);
        subY = picY + (picH * 0.5f) - (subH * 0.5f);

        /*if( bs->frameShader )
        {
          Vector4Copy( bs->backColor, frameColor );
          frameColor[ 3 ] = color[ 3 ];
          trap_R_SetColor( frameColor );
          DrawPic( picX, picY, picW, picH, bs->frameShader );
          trap_R_SetColor( nullptr );
        }*/

        if(health > 0) {
            float32 hX, hY, hW, hH;
            vec4_t healthColor;

            hX = picX + (bs->healthPadding * scale);
            hY = picY + (bs->healthPadding * scale);
            hH = picH - (bs->healthPadding * 2.0f * scale);
            hW = picW * healthScale - (bs->healthPadding * 2.0f * scale);

            if(healthScale == 1.0f) {
                Vector4Copy(bs->healthLowColor, healthColor);
            } else if(healthScale >= 0.75f) {
                Vector4Copy(bs->healthGuardedColor, healthColor);
            } else if(healthScale >= 0.50f) {
                Vector4Copy(bs->healthElevatedColor, healthColor);
            } else if(healthScale >= 0.25f) {
                Vector4Copy(bs->healthHighColor, healthColor);
            } else {
                Vector4Copy(bs->healthSevereColor, healthColor);
            }

            healthColor[ 3 ] = color[ 3 ];
            trap_R_SetColor(healthColor);

            idCGameDrawTools::DrawPic(hX, hY, hW, hH, cgs.media.whiteShader);
            trap_R_SetColor(nullptr);
        }

        if(bs->overlayShader) {
            float32 oW = bs->overlayWidth * 0.3f;
            float32 oH = bs->overlayHeight * 0.85f;
            float32 oX = x;
            float32 oY = y;

            oH *= scale;
            oW *= scale;
            oX -= (oW * 0.5f);
            oY -= (oH * 0.5f);

            trap_R_SetColor(frameColor);
            idCGameDrawTools::DrawPic(oX, oY, oW, oH, bs->overlayShader);
            trap_R_SetColor(nullptr);
        }

        trap_R_SetColor(color);

        {
            float32 nX;
            sint healthMax;
            sint healthPoints;

            healthMax = bggame->Class(pclass)->health;
            healthPoints = (sint)(healthScale * healthMax);

            if(health > 0 && healthPoints < 1) {
                healthPoints = 1;
            }

            nX = picX + (picW * 0.5f) - 2.0f - ((subH * 4) * 0.5f);

            if(healthPoints > 999) {
                nX -= 0.0f;
            } else if(healthPoints > 99) {
                nX -= subH * 0.5f;
            } else if(healthPoints > 9) {
                nX -= subH * 1.0f;
            } else {
                nX -= subH * 1.5f;
            }

            idCGameDraw::DrawField(nX, subY, 4, subH, subH, healthPoints);
        }
        trap_R_SetColor(nullptr);
    }
}

/*
==================
idCGamePlayers::SortDistance
==================
*/
sint idCGamePlayers::SortDistance2(const void *a, const void *b) {
    centity_t *aent, *bent;
    float32 adist, bdist;

    aent = &cg_entities[ *(sint *)a ];
    bent = &cg_entities[ *(sint *)b ];
    adist = Distance(cg.refdef.vieworg, aent->lerpOrigin);
    bdist = Distance(cg.refdef.vieworg, bent->lerpOrigin);

    if(adist > bdist) {
        return -1;
    } else if(adist < bdist) {
        return 1;
    } else {
        return 0;
    }
}

/*
==================
idCGamePlayers::DrawTeamStatus
==================
*/
void idCGamePlayers::DrawTeamStatus(void) {
    sint i, buildableList[ MAX_ENTITIES_IN_SNAPSHOT ], buildables = 0;
    centity_t *cent;
    entityState_t *es;

    if(cg_drawTeamStatus.integer) {
        for(i = 0; i < cg.snap->numEntities; i++) {
            cent = &cg_entities[ cg.snap->entities[ i ].number ];
            es = &cent->currentState;

            if(es->eType == ET_PLAYER &&
                    (cent->currentState.misc & 0x00FF) ==
                    cg.predictedPlayerState.stats[STAT_TEAM]) {
                buildableList[buildables++] = cg.snap->entities[i].number;
            }
        }

        qsort(buildableList, buildables, sizeof(sint), SortDistance2);

        for(i = 0; i < buildables; i++) {
            TeamStatusDisplay(&cg_entities[buildableList[i]]);
        }
    }
}

/*
==================
idCGamePlayers::GetPlayerLocation
==================
*/
centity_t *idCGamePlayers::GetPlayerLocation(void) {
    centity_t *eloc, *best;
    float32 bestlen, len;
    vec3_t origin;

    best = nullptr;
    bestlen = 3.0f * 8192.0f * 8192.0f;

    VectorCopy(cg.predictedPlayerState.origin, origin);

    for(eloc = cg.locationHead; eloc; eloc = eloc->nextLocation) {
        len = DistanceSquared(origin, eloc->lerpOrigin);

        if(len > bestlen) {
            continue;
        }

        if(!trap_R_inPVS(origin, eloc->lerpOrigin)) {
            continue;
        }

        bestlen = len;
        best = eloc;
    }

    return best;
}
