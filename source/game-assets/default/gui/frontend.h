
//frontend.menu
#define W         640
#define H         480
#define BUTT_BAR_X  0
#define BUTT_BAR_Y  15
#define BUTT_BAR_W  640
#define BUTT_BAR_H  40
#define BUTT_W      (BUTT_BAR_W/8)
#define BUTT_H      BUTT_BAR_H



#define EDITFIELDLEFT( EDITFIELDLEFT_X, EDITFIELDLEFT_Y, EDITFIELDLEFT_W, EDITFIELDLEFT_H, EDITFIELDLEFT_TEXT, EDITFIELDLEFT_TEXT_SCALE, EDITFIELDLEFT_TEXT_ALIGN_Y, EDITFIELDLEFT_CVAR, EDITFIELDLEFT_MAXCHARS, EDITFIELDLEFT_MAXPAINTCHARS )	\
	itemDef {															\
		name			"efleft"##EDITFIELDLEFT_TEXT					\
      	group			GROUP_NAME										\
		type			ITEM_TYPE_EDITFIELD								\
		text			EDITFIELDLEFT_TEXT								\
		textfont		UI_FONT_COURBD_21								\
		textstyle		ITEM_TEXTSTYLE_SHADOWED							\
		textscale		EDITFIELDLEFT_TEXT_SCALE						\
		textaligny		EDITFIELDLEFT_TEXT_ALIGN_Y						\
		forecolor		.6 .6 .6 1										\
		cvar			EDITFIELDLEFT_CVAR								\
		maxChars		EDITFIELDLEFT_MAXCHARS							\
		maxPaintChars	EDITFIELDLEFT_MAXPAINTCHARS						\
		visible			1												\
    }
