gui/assets/background
{
	nomipmaps
	nopicmip
	
	{
		map gui/assets/images/white.tga
		rgbGen wave triangle -8 9 0.63 0.137
		alphaGen wave noise -3 4 0 2.37
		tcMod scale 0.5 0.5
		tcMod scroll 0.0025 -0.002
		tcMod stretch sin 3 0.02 6 .2
	}
	{
		map gui/assets/images/owback2.tga
		blendfunc blend
		rgbGen wave sin .2 .2 0.5 0.1
        tcmod rotate 25
        tcMod stretch sin .5 0.1 0.1 .5
	}
    {
		map gui/assets/images/owback_circuit.tga
		blendfunc blend
		tcMod turb 0 0.009 0 0.01
		rgbGen wave noise 0.95 0.05 0 45
		tcmod scroll 3 3
	}
	{
		animMap 71 gui/assets/shiny/frame_0out.tga gui/assets/shiny/frame_1out.tga gui/assets/shiny/frame_2out.tga gui/assets/shiny/frame_3out.tga gui/assets/shiny/frame_4out.tga gui/assets/shiny/frame_5out.tga gui/assets/shiny/frame_6out.tga gui/assets/shiny/frame_7out.tga gui/assets/shiny/frame_8out.tga gui/assets/shiny/frame_9out.tga gui/assets/shiny/frame_10out.tga gui/assets/shiny/frame_11out.tga gui/assets/shiny/frame_12out.tga gui/assets/shiny/frame_13out.tga gui/assets/shiny/frame_14out.tga gui/assets/shiny/frame_15out.tga gui/assets/shiny/frame_16out.tga gui/assets/shiny/frame_17out.tga gui/assets/shiny/frame_18out.tga gui/assets/shiny/frame_19out.tga gui/assets/shiny/frame_20out.tga gui/assets/shiny/frame_21out.tga gui/assets/shiny/frame_22out.tga gui/assets/shiny/frame_23out.tga gui/assets/shiny/frame_24out.tga gui/assets/shiny/frame_25out.tga gui/assets/shiny/frame_26out.tga gui/assets/shiny/frame_27out.tga gui/assets/shiny/frame_28out.tga gui/assets/shiny/frame_29out.tga gui/assets/shiny/frame_30out.tga gui/assets/shiny/frame_31out.tga gui/assets/shiny/frame_32out.tga gui/assets/shiny/frame_33out.tga gui/assets/shiny/frame_34out.tga gui/assets/shiny/frame_35out.tga gui/assets/shiny/frame_36out.tga gui/assets/shiny/frame_37out.tga gui/assets/shiny/frame_38out.tga gui/assets/shiny/frame_39out.tga gui/assets/shiny/frame_40out.tga gui/assets/shiny/frame_41out.tga gui/assets/shiny/frame_42out.tga gui/assets/shiny/frame_43out.tga gui/assets/shiny/frame_44out.tga gui/assets/shiny/frame_45out.tga gui/assets/shiny/frame_46out.tga gui/assets/shiny/frame_47out.tga gui/assets/shiny/frame_48out.tga gui/assets/shiny/frame_49out.tga gui/assets/shiny/frame_50out.tga gui/assets/shiny/frame_51out.tga gui/assets/shiny/frame_52out.tga gui/assets/shiny/frame_53out.tga gui/assets/shiny/frame_54out.tga gui/assets/shiny/frame_55out.tga gui/assets/shiny/frame_56out.tga gui/assets/shiny/frame_57out.tga gui/assets/shiny/frame_58out.tga gui/assets/shiny/frame_59out.tga gui/assets/shiny/frame_60out.tga gui/assets/shiny/frame_61out.tga gui/assets/shiny/frame_62out.tga gui/assets/shiny/frame_63out.tga gui/assets/shiny/frame_64out.tga gui/assets/shiny/frame_65out.tga gui/assets/shiny/frame_66out.tga gui/assets/shiny/frame_67out.tga gui/assets/shiny/frame_68out.tga gui/assets/shiny/frame_69out.tga gui/assets/shiny/frame_70out.tga 
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen identity
	}
	{
		clampmap gui/assets/images/owback_logo.tga
		blendFunc blend
	}
	{
		clampmap gui/assets/images/owback_logoedge.tga
		blendFunc blend
		tcmod rotate -15
	}
	{
		clampmap gui/assets/images/owback_logoedge.tga
		blendFunc blend
		tcmod rotate 3
	}
}

gui/assets/theme
{
	{
		animMap 71 gui/assets/shiny/frame_0out.tga gui/assets/shiny/frame_1out.tga gui/assets/shiny/frame_2out.tga gui/assets/shiny/frame_3out.tga gui/assets/shiny/frame_4out.tga gui/assets/shiny/frame_5out.tga gui/assets/shiny/frame_6out.tga gui/assets/shiny/frame_7out.tga gui/assets/shiny/frame_8out.tga gui/assets/shiny/frame_9out.tga gui/assets/shiny/frame_10out.tga gui/assets/shiny/frame_11out.tga gui/assets/shiny/frame_12out.tga gui/assets/shiny/frame_13out.tga gui/assets/shiny/frame_14out.tga gui/assets/shiny/frame_15out.tga gui/assets/shiny/frame_16out.tga gui/assets/shiny/frame_17out.tga gui/assets/shiny/frame_18out.tga gui/assets/shiny/frame_19out.tga gui/assets/shiny/frame_20out.tga gui/assets/shiny/frame_21out.tga gui/assets/shiny/frame_22out.tga gui/assets/shiny/frame_23out.tga gui/assets/shiny/frame_24out.tga gui/assets/shiny/frame_25out.tga gui/assets/shiny/frame_26out.tga gui/assets/shiny/frame_27out.tga gui/assets/shiny/frame_28out.tga gui/assets/shiny/frame_29out.tga gui/assets/shiny/frame_30out.tga gui/assets/shiny/frame_31out.tga gui/assets/shiny/frame_32out.tga gui/assets/shiny/frame_33out.tga gui/assets/shiny/frame_34out.tga gui/assets/shiny/frame_35out.tga gui/assets/shiny/frame_36out.tga gui/assets/shiny/frame_37out.tga gui/assets/shiny/frame_38out.tga gui/assets/shiny/frame_39out.tga gui/assets/shiny/frame_40out.tga gui/assets/shiny/frame_41out.tga gui/assets/shiny/frame_42out.tga gui/assets/shiny/frame_43out.tga gui/assets/shiny/frame_44out.tga gui/assets/shiny/frame_45out.tga gui/assets/shiny/frame_46out.tga gui/assets/shiny/frame_47out.tga gui/assets/shiny/frame_48out.tga gui/assets/shiny/frame_49out.tga gui/assets/shiny/frame_50out.tga gui/assets/shiny/frame_51out.tga gui/assets/shiny/frame_52out.tga gui/assets/shiny/frame_53out.tga gui/assets/shiny/frame_54out.tga gui/assets/shiny/frame_55out.tga gui/assets/shiny/frame_56out.tga gui/assets/shiny/frame_57out.tga gui/assets/shiny/frame_58out.tga gui/assets/shiny/frame_59out.tga gui/assets/shiny/frame_60out.tga gui/assets/shiny/frame_61out.tga gui/assets/shiny/frame_62out.tga gui/assets/shiny/frame_63out.tga gui/assets/shiny/frame_64out.tga gui/assets/shiny/frame_65out.tga gui/assets/shiny/frame_66out.tga gui/assets/shiny/frame_67out.tga gui/assets/shiny/frame_68out.tga gui/assets/shiny/frame_69out.tga gui/assets/shiny/frame_70out.tga 
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
        rgbGen identity
	}
}
