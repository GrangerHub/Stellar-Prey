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
// File name:   gui_shared.hpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GUI_SHARED_H__
#define __GUI_SHARED_H__

#define MAX_MENUNAME 32
#define MAX_ITEMTEXT 64
#define MAX_ITEMACTION 64
#define MAX_MENUDEFFILE 4096
#define MAX_MENUFILE 32768
#define MAX_MENUS 256
#define MAX_MENUITEMS 512
#define MAX_COLOR_RANGES 10
#define MAX_OPEN_MENUS 16

#define WINDOW_MOUSEOVER       0x00000001  // mouse is over it, non exclusive
#define WINDOW_HASFOCUS        0x00000002  // has cursor focus, exclusive
#define WINDOW_VISIBLE         0x00000004  // is visible
#define WINDOW_GREY            0x00000008  // is visible but grey ( non-active )
#define WINDOW_DECORATION      0x00000010  // for decoration only, no mouse, keyboard, etc..
#define WINDOW_FADINGOUT       0x00000020  // fading out, non-active
#define WINDOW_FADINGIN        0x00000040  // fading in
#define WINDOW_MOUSEOVERTEXT   0x00000080  // mouse is over it, non exclusive
#define WINDOW_INTRANSITION    0x00000100  // window is in transition
#define WINDOW_FORECOLORSET    0x00000200  // forecolor was explicitly set ( used to color alpha images or not )
#define WINDOW_HORIZONTAL      0x00000400  // for list boxes and sliders, vertical is default this is set of horizontal
#define WINDOW_LB_LEFTARROW    0x00000800  // mouse is over left/up arrow
#define WINDOW_LB_RIGHTARROW   0x00001000  // mouse is over right/down arrow
#define WINDOW_LB_THUMB        0x00002000  // mouse is over thumb
#define WINDOW_LB_PGUP         0x00004000  // mouse is over page up
#define WINDOW_LB_PGDN         0x00008000  // mouse is over page down
#define WINDOW_ORBITING        0x00010000  // item is in orbit
#define WINDOW_OOB_CLICK       0x00020000  // close on out of bounds click
#define WINDOW_WRAPPED         0x00080000  // wrap text
#define WINDOW_FORCED          0x00100000  // forced open
#define WINDOW_POPUP           0x00200000  // popup
#define WINDOW_BACKCOLORSET    0x00400000  // backcolor was explicitly set
#define WINDOW_TIMEDVISIBLE    0x00800000  // visibility timing ( NOT implemented )
#define WINDOW_DONTCLOSEALL    0x01000000  // don't close during Menus_CloseAll()
#define WINDOW_DRAG            0x08000000  // KTW: Added for draggable windows


// CGAME cursor type bits
#define CURSOR_NONE          0x00000001
#define CURSOR_ARROW        0x00000002
#define CURSOR_SIZER        0x00000004

#ifdef CGAMEDLL
#define STRING_POOL_SIZE 128*1024
#else
#define STRING_POOL_SIZE 384*1024
#endif
#define MAX_STRING_HANDLES 4096

#define MAX_SCRIPT_ARGS 12
#define MAX_EDITFIELD 256
#define ITEM_VALUE_OFFSET 8

#define ART_FX_BASE      "menu/art/fx_base"
#define ART_FX_BLUE      "menu/art/fx_blue"
#define ART_FX_CYAN      "menu/art/fx_cyan"
#define ART_FX_GREEN     "menu/art/fx_grn"
#define ART_FX_RED       "menu/art/fx_red"
#define ART_FX_TEAL      "menu/art/fx_teal"
#define ART_FX_WHITE     "menu/art/fx_white"
#define ART_FX_YELLOW    "menu/art/fx_yel"

#define ASSET_GRADIENTBAR           "gui/assets/gradientbar2.tga"
#define ASSET_SCROLLBAR             "gui/assets/scrollbar.tga"
#define ASSET_SCROLLBAR_ARROWDOWN   "gui/assets/scrollbar_arrow_dwn_a.tga"
#define ASSET_SCROLLBAR_ARROWUP     "gui/assets/scrollbar_arrow_up_a.tga"
#define ASSET_SCROLLBAR_ARROWLEFT   "gui/assets/scrollbar_arrow_left.tga"
#define ASSET_SCROLLBAR_ARROWRIGHT  "gui/assets/scrollbar_arrow_right.tga"
#define ASSET_SCROLL_THUMB          "gui/assets/scrollbar_thumb.tga"
#define ASSET_SLIDER_BAR            "gui/assets/slider2.tga"
#define ASSET_SLIDER_THUMB          "gui/assets/sliderbutt_1.tga"
#define ASSET_CORNERIN              "gui/assets/cornerIn"
#define ASSET_CORNEROUT             "gui/assets/cornerOut"
#define SCROLLBAR_SIZE 16.0f
#define SCROLLBAR_WIDTH (SCROLLBAR_SIZE*DC->aspectScale)
#define SCROLLBAR_HEIGHT SCROLLBAR_SIZE
#define SLIDER_WIDTH (96.0f*DC->aspectScale)
#define SLIDER_HEIGHT 16.0f
#define SLIDER_THUMB_WIDTH (12.0f*DC->aspectScale)
#define SLIDER_THUMB_HEIGHT 20.0f
#define NUM_CROSSHAIRS      10

typedef struct {
    pointer command;
    pointer args[MAX_SCRIPT_ARGS];
}
scriptDef_t;


typedef struct {
    float32 x;    // horiz position
    float32 y;    // vert position
    float32 w;    // width
    float32 h;    // height;
}
rectDef_t;

typedef rectDef_t rectangle;

// FIXME: do something to separate text vs window stuff

typedef struct {
    rectangle rect;                 // client coord rectangle
    sint aspectBias;                 // direction in which to aspect compensate
    rectangle rectClient;           // screen coord rectangle
    pointer name;               //
    pointer group;              // if it belongs to a group
    pointer cinematicName;      // cinematic name
    sint cinematic;                  // cinematic handle
    sint style;                      //
    sint border;                     //
    sint ownerDraw;                  // ownerDraw style
    sint ownerDrawFlags;             // show flags for ownerdraw items
    float32 borderSize;               //
    sint flags;                      // visible, focus, mouseover, cursor
    rectangle rectEffects;          // for various effects
    rectangle rectEffects2;         // for various effects
    sint offsetTime;                 // time based value for various effects
    sint nextTime;                   // time next effect should cycle
    vec4_t foreColor;               // text color
    vec4_t backColor;               // border color
    vec4_t borderColor;             // border color
    vec4_t outlineColor;            // border color
    qhandle_t background;           // background asset
}
windowDef_t;

typedef windowDef_t WinDow;

typedef struct {
    vec4_t  color;
    float32    low;
    float32    high;
}
colorRangeDef_t;

// FIXME: combine flags into bitfields to save space
// FIXME: consolidate all of the common stuff in one structure for menus and items
// THINKABOUTME: is there any compelling reason not to have items contain items
// and do away with a menu per say.. major issue is not being able to dynamically allocate
// and destroy stuff.. Another point to consider is adding an alloc free call for vm's and have
// the engine just allocate the pool for it based on a cvar
// many of the vars are re-used for different item types, as such they are not always named appropriately
// the benefits of c++ in DOOM will greatly help crap like this
// FIXME: need to put a type ptr that points to specific type info per type
//
#define MAX_LB_COLUMNS 16

typedef struct columnInfo_s {
    sint pos;
    sint width;
    sint align;
}
columnInfo_t;

typedef struct listBoxDef_s {
    sint startPos;
    sint endPos;
    sint drawPadding;
    sint cursorPos;
    float32 elementWidth;
    float32 elementHeight;
    sint elementStyle;
    sint numColumns;
    columnInfo_t columnInfo[MAX_LB_COLUMNS];
    pointer doubleClick;
    bool notselectable;
    bool noscrollbar;
}
listBoxDef_t;

typedef struct comboBoxDef_s {
    sint cursorPos;
}
comboBoxDef_t;

typedef struct editFieldDef_s {
    float32 minVal;                  //  edit field limits
    float32 maxVal;                  //
    float32 defVal;                  //
    float32 range;                   //
    sint maxChars;                  // for edit fields
    sint maxPaintChars;             // for edit fields
    sint maxFieldWidth;             // for edit fields
    sint paintOffset;               //
}
editFieldDef_t;

#define MAX_MULTI_CVARS 32

typedef struct multiDef_s {
    pointer cvarList[MAX_MULTI_CVARS];
    pointer cvarStr[MAX_MULTI_CVARS];
    float32 cvarValue[MAX_MULTI_CVARS];
    sint count;
    bool strDef;
}
multiDef_t;

typedef struct modelDef_s {
    sint angle;
    vec3_t origin;
    float32 fov_x;
    float32 fov_y;
    sint rotationSpeed;

    vec3_t axis;

    sint   animated;
    sint   startframe;
    sint   numframes;
    sint   fps;

    sint   frame;
    sint   oldframe;
    float32 backlerp;
    sint   frameTime;
}
modelDef_t;

#define CVAR_ENABLE    0x00000001
#define CVAR_DISABLE  0x00000002
#define CVAR_SHOW      0x00000004
#define CVAR_HIDE      0x00000008

typedef struct itemDef_s {
    WinDow window;                 // common positional, border, style, layout info
    rectangle textRect;            // rectangle the text ( if any ) consumes
    sint type;                      // text, button, radiobutton, checkbox, textfield, listbox, combo
    sint alignment;                 // left center right
    sint textalignment;             // ( optional ) alignment for text within rect based on text width
    sint textvalignment;            // ( optional ) alignment for text within rect based on text width
    float32 textalignx;              // ( optional ) text alignment x coord
    float32 textaligny;              // ( optional ) text alignment x coord
    float32 textscale;               // scale percentage from 72pts
    sint textStyle;                 // ( optional ) style, normal and shadowed are it for now
    pointer text;              // display text
    void *parent;                  // menu owner
    qhandle_t asset;               // handle to asset
    pointer mouseEnterText;    // mouse enter script
    pointer mouseExitText;     // mouse exit script
    pointer mouseEnter;        // mouse enter script
    pointer mouseExit;         // mouse exit script
    pointer action;            // select script
    pointer onFocus;           // select script
    pointer leaveFocus;        // select script
    pointer onTextEntry;       // called when text entered
    pointer cvar;              // associated cvar
    pointer cvarTest;          // associated cvar for enable actions
    pointer enableCvar;         // enable, disable, show, or hide based on value, this can contain a list
    sint cvarFlags;                 //  what type of action to take on cvarenables
    sfxHandle_t focusSound;
    sint numColors;                 // number of color ranges
    colorRangeDef_t colorRanges[MAX_COLOR_RANGES];
    float32 special;                 // float32 used for feeder id's etc.. diff per type
    sint modifier;                 // sint used for feeder id's etc.. diff per type
    sint cursorPos;                 // cursor position in characters
    void *typeData;                // type specific data ptr's
}
itemDef_t;

typedef struct {
    WinDow window;
    pointer  font;                // font
    bool fullScreen;              // covers entire screen
    sint itemCount;                    // number of items;
    sint fontIndex;                    //
    sint cursorItem;                    // which item as the cursor
    sint fadeCycle;                    //
    float32 fadeClamp;                  //
    float32 fadeAmount;                  //
    pointer onOpen;                // run when the menu is first opened
    pointer onClose;              // run when the menu is closed
    pointer onESC;                // run when the menu is closed
    pointer soundName;            // background loop sound for menu
    pointer listenCvar;           // Cvar to be executed as an uiscript

    vec4_t focusColor;                // focus color for items
    vec4_t disableColor;              // focus color for items
    itemDef_t *items[MAX_MENUITEMS];  // items this menu contains
}
menuDef_t;

typedef struct {
    pointer fontStr;
    pointer cursorStr;
    pointer gradientStr;
    fontInfo_t textFont;
    fontInfo_t smallFont;
    fontInfo_t bigFont;
    qhandle_t cursor;
    qhandle_t gradientBar;
    qhandle_t scrollBarArrowUp;
    qhandle_t scrollBarArrowDown;
    qhandle_t scrollBarArrowLeft;
    qhandle_t scrollBarArrowRight;
    qhandle_t scrollBar;
    qhandle_t scrollBarThumb;
    qhandle_t buttonMiddle;
    qhandle_t buttonInside;
    qhandle_t solidBox;
    qhandle_t sliderBar;
    qhandle_t sliderThumb;
    qhandle_t cornerOut;
    qhandle_t cornerIn;
    sfxHandle_t menuEnterSound;
    sfxHandle_t menuExitSound;
    sfxHandle_t menuBuzzSound;
    sfxHandle_t itemFocusSound;
    float32 fadeClamp;
    sint fadeCycle;
    float32 fadeAmount;
    float32 shadowX;
    float32 shadowY;
    vec4_t shadowColor;
    float32 shadowFadeClamp;
    bool fontRegistered;
    valueType emoticons[ MAX_EMOTICONS ][ MAX_EMOTICON_NAME_LEN ];
    qhandle_t emoticonShaders[ MAX_EMOTICONS ];
    sint emoticonWidths[ MAX_EMOTICONS ];
    sint emoticonCount;
}
cachedAssets_t;

typedef struct {
    pointer name;
    void (*handler)(itemDef_t *item, valueType **args);
}
commandDef_t;

typedef struct {
    qhandle_t (*registerShaderNoMip)(pointer p);
    void (*setColor)(const vec4_t v);
    void (*drawHandlePic)(float32 x, float32 y, float32 w, float32 h,
                          qhandle_t asset);
    void (*drawStretchPic)(float32 x, float32 y, float32 w, float32 h,
                           float32 s1, float32 t1, float32 s2, float32 t2, qhandle_t hShader);
    qhandle_t (*registerModel)(pointer p);
    void (*modelBounds)(qhandle_t model, vec3_t min, vec3_t max);
    void (*fillRect)(float32 x, float32 y, float32 w, float32 h,
                     const vec4_t color);
    void (*drawRect)(float32 x, float32 y, float32 w, float32 h, float32 size,
                     const vec4_t color);
    void (*drawRoundedRect)(float32 x, float32 y, float32 w, float32 h,
                            float32 size, const vec4_t color);
    void (*fillRoundedRect)(float32 x, float32 y, float32 w, float32 h,
                            float32 size, const vec4_t color);
    void (*drawSides)(float32 x, float32 y, float32 w, float32 h,
                      float32 size);
    void (*drawTopBottom)(float32 x, float32 y, float32 w, float32 h,
                          float32 size);
    void (*clearScene)(void);
    void (*addRefEntityToScene)(const refEntity_t *re);
    void (*renderScene)(const refdef_t *fd);
    void (*registerFont)(pointer pFontname, sint pointSize, fontInfo_t *font);
    void (*ownerDrawItem)(float32 x, float32 y, float32 w, float32 h,
                          float32 text_x,
                          float32 text_y, sint ownerDraw, sint ownerDrawFlags,
                          sint align, sint textalign, sint textvalign,
                          float32 special, float32 scale, vec4_t foreColor,
                          vec4_t backColor, qhandle_t shader, sint textStyle);
    float32(*getValue)(sint ownerDraw);
    bool (*ownerDrawVisible)(sint flags);
    void (*runScript)(valueType **p);
    void (*getCVarString)(pointer cvar, valueType *buffer, uint64 bufsize);
    float32(*getCVarValue)(pointer cvar);
    void (*setCVar)(pointer cvar, pointer value);
    void (*drawTextWithCursor)(float32 x, float32 y, float32 scale,
                               vec4_t color, pointer text, sint cursorPos, valueType cursor, sint limit,
                               sint style);
    void (*setOverstrikeMode)(bool b);
    bool (*getOverstrikeMode)(void);
    void (*startLocalSound)(sfxHandle_t sfx, sint channelNum);
    bool (*ownerDrawHandleKey)(sint ownerDraw, sint flags, float32 *special,
                               sint key);
    sint(*feederCount)(float32 feederID);
    pointer(*feederItemText)(float32 feederID, sint index, sint column,
                             qhandle_t *handle);
    qhandle_t (*feederItemImage)(float32 feederID, sint index);
    void (*feederSelection)(float32 feederID, sint index);
    sint(*feederInitialise)(float32 feederID);
    void (*keynumToStringBuf)(sint keynum, valueType *buf, sint buflen);
    void (*getBindingBuf)(sint keynum, valueType *buf, sint buflen);
    void (*setBinding)(sint keynum, pointer binding);
    void (*executeText)(sint exec_when, pointer text);
    void (*Error)(sint level, pointer error, ...);
    void (*Print)(pointer msg, ...);
    void (*Pause)(bool b);
    sint(*ownerDrawWidth)(sint ownerDraw, float32 scale);
    pointer(*ownerDrawText)(sint ownerDraw);
    sfxHandle_t (*registerSound)(pointer name);
    void (*startBackgroundTrack)(pointer intro, pointer loop);
    void (*stopBackgroundTrack)(void);
    sint(*playCinematic)(pointer name, float32 x, float32 y, float32 w,
                         float32 h);
    void (*stopCinematic)(sint handle);
    void (*drawCinematic)(sint handle, float32 x, float32 y, float32 w,
                          float32 h);
    void (*runCinematicFrame)(sint handle);
    sint(*getFileList)(pointer path, pointer extension, valueType *listbuf,
                       sint bufsize);
    valueType *(*translateString)(pointer string);
    void (*checkAutoUpdate)();
    void (*getAutoUpdate)();

    float32 yscale;
    float32 xscale;
    float32 aspectScale;
    sint realTime;
    sint frameTime;
    float32 cursorx;
    float32 cursory;
    float32 cursordx;
    float32 cursordy;
    bool debug;

    cachedAssets_t Assets;

    vidconfig_t glconfig;
    qhandle_t  whiteShader;
    qhandle_t gradientImage;
    qhandle_t cursor;
    float32 FPS;

    bool  hudloading;
}
displayContextDef_t;

pointer String_Alloc(pointer p);
void String_Init(void);
void String_Report(void);
void Init_Display(displayContextDef_t *dc);
void Display_ExpandMacros(valueType *buff);
void Menu_Init(menuDef_t *menu);
void Item_Init(itemDef_t *item);
void Menu_PostParse(menuDef_t *menu);
menuDef_t *Menu_GetFocused(void);
void Menu_HandleKey(menuDef_t *menu, sint key, bool down);
void Menu_HandleMouseMove(menuDef_t *menu, float32 x, float32 y);
void Menu_ScrollFeeder(menuDef_t *menu, sint feeder, bool down);
bool Float_Parse(valueType **p, float32 *f);
bool Color_Parse(valueType **p, vec4_t *c);
bool Int_Parse(valueType **p, sint *i);
bool Rect_Parse(valueType **p, rectDef_t *r);
bool String_Parse(valueType **p, pointer *out);
bool Script_Parse(valueType **p, pointer *out);
bool PC_Float_Parse(sint handle, float32 *f);
bool PC_Color_Parse(sint handle, vec4_t *c);
bool PC_Int_Parse(sint handle, sint *i);
bool PC_Rect_Parse(sint handle, rectDef_t *r);
bool PC_String_Parse(sint handle, pointer *out);
bool PC_Script_Parse(sint handle, pointer *out);
sint Menu_Count(void);
void Menu_New(sint handle);
void Menu_PaintAll(void);
void Menu_Reset(void);
bool Menus_AnyFullScreenVisible(void);
menuDef_t *Menus_ActivateByName(pointer p);
void Menus_Activate(menuDef_t *menu);
menuDef_t *Menu_Get(sint handle);

displayContextDef_t *Display_GetContext(void);
void *Display_CaptureItem(sint x, sint y);
bool Display_MouseMove(void *p, float32 x, float32 y);
sint Display_CursorType(sint x, sint y);
bool Display_KeyBindPending(void);
menuDef_t *Menus_FindByName(pointer p);
itemDef_t *Menu_FindItemByName(menuDef_t *menu, pointer p);
void Menus_CloseByName(pointer p);
void Display_HandleKey(sint key, bool down, sint x, sint y);
void LerpColor(vec4_t a, vec4_t b, vec4_t c, float32 t);
void Menus_CloseAll(bool force);
void Menu_Paint(menuDef_t *menu, bool forcePaint);
void Menu_SetFeederSelection(menuDef_t *menu, sint feeder, sint index,
                             pointer name);
void Display_CacheAll(void);

typedef void (CaptureFunc)(void *p);

void UI_InstallCaptureFunc(CaptureFunc *f, void *data, sint timeout);
void UI_RemoveCaptureFunc(void);

void *UI_Alloc(sint size);
void UI_InitMemory(void);
bool UI_OutOfMemory(void);

void Controls_GetConfig(void);
void Controls_SetConfig(bool restart);
void Controls_SetDefaults(void);

//for cg_draw.c
void Item_Text_Wrapped_Paint(itemDef_t *item);
void UI_DrawTextBlock(rectDef_t *rect, float32 text_x, float32 text_y,
                      vec4_t color,
                      float32 scale, sint textalign, sint textvalign,
                      sint textStyle, pointer text);
void UI_Text_Paint(float32 x, float32 y, float32 scale, vec4_t color,
                   pointer text, float32 adjust, sint limit, sint style);
void UI_Text_Paint_Limit(float32 *maxX, float32 x, float32 y,
                         float32 scale,
                         vec4_t color, pointer text, float32 adjust, sint limit);
float32 UI_Text_Width(pointer text, float32 scale, sint limit);
float32 UI_Text_Height(pointer text, float32 scale, sint limit);
float32 UI_Text_EmWidth(float32 scale);
float32 UI_Text_EmHeight(float32 scale);

sint      trap_PC_AddGlobalDefine(valueType *define);
sint      trap_PC_LoadSource(pointer filename);
sint      trap_PC_FreeSource(sint handle);
sint      trap_PC_ReadToken(sint handle, pc_token_t *pc_token);
sint      trap_PC_SourceFileAndLine(sint handle, valueType *filename,
                                    sint *line);

void    BindingFromName(pointer cvar);
extern valueType g_nameBind1[ 32 ];
extern valueType g_nameBind2[ 32 ];

#endif //!__GUI_SHARED_H__
