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
// File name:   gui_atoms.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description: User interface building blocks and support functions.
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <GUI/gui_precompiled.hpp>

/*
=================
UI_ClampCvar
=================
*/
float32 UI_ClampCvar(float32 min, float32 max, float32 value) {
    if(value < min) {
        return min;
    }

    if(value > max) {
        return max;
    }

    return value;
}

/*
=================
UI_StartDemoLoop
=================
*/
void UI_StartDemoLoop(void) {
    trap_Cmd_ExecuteText(EXEC_APPEND, "d1\n");
}

valueType *UI_Argv(sint arg) {
    static valueType  buffer[MAX_STRING_CHARS];

    trap_Argv(arg, buffer, sizeof(buffer));

    return buffer;
}

valueType *UI_ConcatArgs(sint arg, valueType *buf, sint len) {
    valueType *p;
    sint      c;

    if(len <= 0) {
        return buf;
    }

    p = buf;
    c = trap_Argc();

    for(; arg < c; arg++) {
        valueType *argp = UI_Argv(arg);

        while(*argp && p < &buf[len - 1]) {
            *p++ = *argp++;
        }

        if(p < &buf[len - 2]) {
            *p++ = ' ';
        } else {
            break;
        }
    }

    *p = '\0';

    return buf;
}

valueType *UI_Cvar_VariableString(pointer var_name) {
    static valueType  buffer[MAX_STRING_CHARS];

    trap_Cvar_VariableStringBuffer(var_name, buffer, sizeof(buffer));

    return buffer;
}

static void UI_Cache_f(void) {
    Display_CacheAll();
}

static void UI_Menu_f(void) {
    if(Menu_Count() > 0) {
        trap_Key_SetCatcher(KEYCATCH_UI);
        Menus_ActivateByName(UI_Argv(1));
    }
}

static void UI_CloseMenus_f(void) {
    if(Menu_Count()) {
        Menus_CloseAll(false);

        if(!Menu_Count()) {
            trap_Key_SetCatcher(trap_Key_GetCatcher() & ~KEYCATCH_UI);
            trap_Key_ClearStates();
            trap_Cvar_Set("cl_paused", "0");
        }
    }
}

static void UI_MessageMode_f(void) {
    char *arg = UI_Argv(0);

    if(!chatInfo.say_make_current_line_blank) {
        char buffer[ MAX_CVAR_VALUE_STRING ];

        trap_Cvar_VariableStringBuffer("ui_sayBuffer", buffer, sizeof(buffer));

        if(buffer[0]) {
            chatInfo.historyLine = chatInfo.nextHistoryLine;
            chatInfo.say_history_current = true;
            chatInfo.say_make_current_line_blank = true;
            trap_Cvar_Set("ui_sayBuffer", "");
        }
    }

    trap_Cvar_Set("ui_sayBuffer", "");

    switch(arg[11]) {
        default:
        case '\0':
            // Global
            chatInfo.chat_mode = CHAT_GLOBAL;
            chatInfo.clientNum = -1;
            break;

        case '2':
            // Team
            chatInfo.chat_mode = CHAT_TEAM;
            chatInfo.clientNum = -1;
            break;

        case '3':
            // Crosshair
            chatInfo.chat_mode = CHAT_CROSSHAIR;
            chatInfo.clientNum = trap_CrosshairPlayer();
            break;

        case '4':
            // Area
            chatInfo.chat_mode = CHAT_AREA;
            chatInfo.clientNum = -1;
            break;

        case '5':
            // Admins
            chatInfo.chat_mode = CHAT_ADMINS;
            chatInfo.clientNum = -1;
            break;

        case '6':
            // Clan
            chatInfo.chat_mode = CHAT_CLAN;
            chatInfo.clientNum = -1;
            break;
    }

    trap_Key_SetCatcher(KEYCATCH_UI);
    Menus_CloseByName("say");
    Menus_CloseByName("say_team");
    Menus_CloseByName("say_crosshair");
    Menus_CloseByName("say_area");
    Menus_CloseByName("say_admins");
    Menus_CloseByName("say_clan");

    switch(chatInfo.chat_mode) {
        case CHAT_GLOBAL:
            Menus_ActivateByName("say");
            break;

        case CHAT_TEAM:
            Menus_ActivateByName("say_team");
            break;

        case CHAT_CROSSHAIR:
            Menus_ActivateByName("say_crosshair");
            break;

        case CHAT_AREA:
            Menus_ActivateByName("say_area");
            break;

        case CHAT_ADMINS:
            Menus_ActivateByName("say_admins");
            break;

        case CHAT_CLAN:
            Menus_ActivateByName("say_clan");
            break;

        case NUM_CHAT_MODES:
            chatInfo.chat_mode = CHAT_GLOBAL;
            Menus_ActivateByName("say");
            break;
    }
}

static void UI_Me_f(void) {
    char buf[MAX_SAY_TEXT - 4];

    UI_ConcatArgs(1, buf, sizeof(buf));

    trap_Cmd_ExecuteText(EXEC_APPEND, va("say \"/me %s\"\n", buf));
}

static void UI_Prompt_f(void) {
    static valueType buffer[ MAX_SAY_TEXT ];
    itemDef_t *item;

    if(trap_Argc() < 3) {
        Com_Printf("prompt <callback> [prompt]: Opens the chatbox, store the text in ui_sayBuffer and then vstr callback\n");
        return;
    }

    trap_Argv(1, uiInfo.chatPromptCallback, sizeof(uiInfo.chatPromptCallback));
    trap_Argv(2, buffer, sizeof(buffer));
    trap_Cvar_Set("ui_sayBuffer", "");
    uiInfo.chatTargetClientNum = -1;
    uiInfo.chatTeam = false;
    uiInfo.chatAdmins = false;
    uiInfo.chatClan = false;
    uiInfo.chatPrompt = true;
    trap_Key_SetCatcher(KEYCATCH_UI);
    Menus_CloseByName("say");
    Menus_CloseByName("say_team");
    Menus_CloseByName("say_crosshair");
    Menus_CloseByName("say_attacker");
    Menus_CloseByName("say_admins");
    Menus_CloseByName("say_prompt");
    Menus_CloseByName("say_clan");
    item = Menu_FindItemByName(Menus_ActivateByName("say_prompt"),
                               "say_field");

    if(item) {
        trap_Argv(2, buffer, sizeof(buffer));
        item->text = buffer;
    }
}

struct {
    valueType *cmd;
    void (*function)(void);
} commands[ ] = {
    { "ui_load", UI_Load },
    { "ui_report", UI_Report },
    { "ui_cache", UI_Cache_f },
    {"me", UI_Me_f},
    { "messagemode", UI_MessageMode_f },
    { "messagemode2", UI_MessageMode_f },
    { "messagemode3", UI_MessageMode_f },
    { "messagemode4", UI_MessageMode_f },
    { "messagemode5", UI_MessageMode_f },
    { "messagemode6", UI_MessageMode_f },
    { "prompt", UI_Prompt_f },
    { "menu", UI_Menu_f },
    { "closemenus", UI_CloseMenus_f }
};

/*
=================
idUserInterfaceManagerLocal::ConsoleCommand
=================
*/
bool idUserInterfaceManagerLocal::ConsoleCommand(sint realTime) {
    valueType  *cmd;
    sint   i;

    uiInfo.uiDC.frameTime = realTime - uiInfo.uiDC.realTime;
    uiInfo.uiDC.realTime = realTime;

    cmd = UI_Argv(0);

    for(i = 0; i < sizeof(commands) / sizeof(commands[ 0 ]); i++) {
        if(Q_stricmp(commands[ i ].cmd, cmd) == 0) {
            commands[ i ].function();
            return true;
        }
    }

    return false;
}

void UI_DrawNamedPic(float32 x, float32 y, float32 width, float32 height,
                     pointer picname) {
    qhandle_t  hShader;

    hShader = trap_R_RegisterShaderNoMip(picname);
    UI_AdjustFrom640(&x, &y, &width, &height);
    trap_R_DrawStretchPic(x, y, width, height, 0, 0, 1, 1, hShader);
}

void UI_DrawHandlePic(float32 x, float32 y, float32 w, float32 h,
                      qhandle_t hShader) {
    float32  s0;
    float32  s1;
    float32  t0;
    float32  t1;

    if(w < 0) {
        // flip about vertical
        w  = -w;
        s0 = 1;
        s1 = 0;
    } else {
        s0 = 0;
        s1 = 1;
    }

    if(h < 0) {
        // flip about horizontal
        h  = -h;
        t0 = 1;
        t1 = 0;
    } else {
        t0 = 0;
        t1 = 1;
    }

    UI_AdjustFrom640(&x, &y, &w, &h);
    trap_R_DrawStretchPic(x, y, w, h, s0, t0, s1, t1, hShader);
}

void UI_SetColor(const float32 *rgba) {
    trap_R_SetColor(rgba);
}
