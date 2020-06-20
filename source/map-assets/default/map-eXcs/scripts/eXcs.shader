textures/eXcs/sky
{
//    skybox "Wasteland" by Ingar
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm sky
// -- direct sunlight
//	q3map_sunExt 1.00 1.00 1.00 160 245 40 2 16
	q3gl2_sun 1.00 1.00 1.00 160 245 40 1 0.75
	skyparms env/eXcs/wasteland - -
}

// no lightmap, always black
textures/eXcs/black
{
	qer_editorimage textures/eXcs/black
	surfaceparm nomarks
	surfaceparm nolightmap
	{
		map $whiteimage
		rgbGen const ( 0 0 0 )
	}
}

textures/eXcs/burst_red_s
{
	entityMergable
	cull none
	{
		map textures/eXcs/sparkle_red
		blendFunc add
		alphaGen	vertex
	}
}

textures/eXcs/burst_blue_s
{
	entityMergable
	cull none
	{
		map textures/eXcs/sparkle_blue
		blendFunc add
		alphaGen	vertex
	}
}

textures/eXcs/force_field_s
{
        qer_editorimage textures/eXcs/force_grid.tga
	surfaceparm trans	
        surfaceparm nomarks	
	surfaceparm nolightmap
	cull none
	{
		map textures/eXcs/force_field.tga
		tcMod Scroll .1 0
		blendFunc add
	}
	{
		map textures/eXcs/force_grid.tga
		tcMod Scroll -.01 0
		blendFunc add
		rgbgen wave sin .2 .2 0 .4
	}
}

textures/eXcs/bulb_red_s
{
	qer_editorimage textures/eXcs/bulb_red.tga
	surfaceparm nomarks
	surfaceparm trans
	surfaceparm nolightmap
	cull disable
	qer_trans 0.5
	{
		map textures/eXcs/bulb_red.tga
		rgbgen vertex
	}
	{
		stage specularmap
		map $whiteimage
	}
}

textures/eXcs/fog_darkness
{
	qer_editorimage textures/eXcs/fog_radiant.jpg
	surfaceparm	trans
	surfaceparm	nonsolid
	surfaceparm	fog
	surfaceparm	nolightmap
	qer_nocarve
	fogparms ( 0 0 0 ) 256
}

// from karith, by jex
textures/eXcs/steam_s
{
	entityMergable
	cull none
	{
		map textures/eXcs/steam.tga
		blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen		entity
		alphaGen	vertex
	}
}

textures/eXcs/eXcs_lightpanel_01_lit
{
	{
		map textures/eX/eX_lightpanel_01_d
	}
	{
		stage normalmap
		map textures/eX/gl2/eX_lightpanel_01_local
	}
	{
		stage specularmap
		map textures/eX/gl2/eX_lightpanel_01_s
		specularreflectance 0.52050
		specularexponent 128
	}
	{
		map $lightmap
		blendfunc filter
	}
	{
		map textures/eXcs/eXcs_lightpanel_01_add
		blendfunc add
	}
}

textures/eXcs/eXcs_lightpanel_01_blue_lit
{
	{
		map textures/eX/eX_lightpanel_01_d
	}
	{
		stage normalmap
		map textures/eX/gl2/eX_lightpanel_01_local
	}
	{
		stage specularmap
		map textures/eX/gl2/eX_lightpanel_01_s
		specularreflectance 0.52050
		specularexponent 128
	}
	{
		map $lightmap
		blendfunc filter
	}
	{
		map textures/eXcs/eXcs_lightpanel_01_blue_add
		blendfunc add
	}
}

textures/eXcs/eXcs_lightpanel_01_red_lit
{
	{
		map textures/eX/eX_lightpanel_01_d
	}
	{
		stage normalmap
		map textures/eX/gl2/eX_lightpanel_01_local
	}
	{
		stage specularmap
		map textures/eX/gl2/eX_lightpanel_01_s
		specularreflectance 0.52050
		specularexponent 128
	}
	{
		map $lightmap
		blendfunc filter
	}
	{
		map textures/eXcs/eXcs_lightpanel_01_red_add
		blendfunc add
	}
}

textures/eXcs/eXcs_light_u201_lit
{
	{
		map textures/eX/eX_light_u201_d
	}
	{
		stage normalmap
		map textures/eX/gl2/eX_light_u201_local
	}
	{
		stage specularmap
		map textures/eX/gl2/eX_light_u201_s
		specularreflectance 0.52050
		specularexponent 128
	}
	{
		map $lightmap
		blendfunc filter
	}
	{
		map textures/eX/eX_light_u201_add
		blendfunc add
	}
}

textures/eXcs/eXcs_light_u201_blue_lit
{
	{
		map textures/eX/eX_light_u201_d
	}
	{
		stage normalmap
		map textures/eX/gl2/eX_light_u201_local
	}
	{
		stage specularmap
		map textures/eX/gl2/eX_light_u201_s
		specularreflectance 0.52050
		specularexponent 128
	}
	{
		map $lightmap
		blendfunc filter
	}
	{
		map textures/eXcs/eXcs_light_u201_blue_add
		blendfunc add
	}
}

textures/eXcs/eXcs_light_u201_red_lit
{
	{
		map textures/eX/eX_light_u201_d
	}
	{
		stage normalmap
		map textures/eX/gl2/eX_light_u201_local
	}
	{
		stage specularmap
		map textures/eX/gl2/eX_light_u201_s
		specularreflectance 0.52050
		specularexponent 128
	}
	{
		map $lightmap
		blendfunc filter
	}
	{
		map textures/eXcs/eXcs_light_u201_red_add
		blendfunc add
	}
}

textures/eXcs/eXcs_lightpanel_01_lit_1000
{
	{
		map textures/eX/eX_lightpanel_01_d
	}
	{
		stage normalmap
		map textures/eX/gl2/eX_lightpanel_01_local
	}
	{
		stage specularmap
		map textures/eX/gl2/eX_lightpanel_01_s
		specularreflectance 0.52050
		specularexponent 128
	}
	{
		map $lightmap
		blendfunc filter
	}
	{
		map textures/eXcs/eXcs_lightpanel_01_add
		blendfunc add
	}
}

textures/eXcs/eXcs_lightpanel_01_blue_lit_1000
{
	{
		map textures/eX/eX_lightpanel_01_d
	}
	{
		stage normalmap
		map textures/eX/gl2/eX_lightpanel_01_local
	}
	{
		stage specularmap
		map textures/eX/gl2/eX_lightpanel_01_s
		specularreflectance 0.52050
		specularexponent 128
	}
	{
		map $lightmap
		blendfunc filter
	}
	{
		map textures/eXcs/eXcs_lightpanel_01_blue_add
		blendfunc add
	}
}

textures/eXcs/eXcs_lightpanel_01_red_lit_1000
{
	{
		map textures/eX/eX_lightpanel_01_d
	}
	{
		stage normalmap
		map textures/eX/gl2/eX_lightpanel_01_local
	}
	{
		stage specularmap
		map textures/eX/gl2/eX_lightpanel_01_s
		specularreflectance 0.52050
		specularexponent 128
	}
	{
		map $lightmap
		blendfunc filter
	}
	{
		map textures/eXcs/eXcs_lightpanel_01_red_add
		blendfunc add
	}
}

textures/eXcs/eXcs_light_u201_lit_1000
{
	{
		map textures/eX/eX_light_u201_d
	}
	{
		stage normalmap
		map textures/eX/gl2/eX_light_u201_local
	}
	{
		stage specularmap
		map textures/eX/gl2/eX_light_u201_s
		specularreflectance 0.52050
		specularexponent 128
	}
	{
		map $lightmap
		blendfunc filter
	}
	{
		map textures/eX/eX_light_u201_add
		blendfunc add
	}
}

textures/eXcs/eXcs_light_u201_blue_lit_1000
{
	{
		map textures/eX/eX_light_u201_d
	}
	{
		stage normalmap
		map textures/eX/gl2/eX_light_u201_local
	}
	{
		stage specularmap
		map textures/eX/gl2/eX_light_u201_s
		specularreflectance 0.52050
		specularexponent 128
	}
	{
		map $lightmap
		blendfunc filter
	}
	{
		map textures/eXcs/eXcs_light_u201_blue_add
		blendfunc add
	}
}

textures/eXcs/eXcs_light_u201_red_lit_1000
{
	{
		map textures/eX/eX_light_u201_d
	}
	{
		stage normalmap
		map textures/eX/gl2/eX_light_u201_local
	}
	{
		stage specularmap
		map textures/eX/gl2/eX_light_u201_s
		specularreflectance 0.52050
		specularexponent 128
	}
	{
		map $lightmap
		blendfunc filter
	}
	{
		map textures/eXcs/eXcs_light_u201_red_add
		blendfunc add
	}
}

textures/eXcs/eXcs_floor_grate_03_128_alpha
{
	qer_editorimage	textures/eXcs/eXcs_floor_grate_03_128_d
	surfaceparm alphashadow
	{
		map textures/eXcs/eXcs_floor_grate_03_128_d
		alphafunc GE128
		depthwrite
	}
	{
		stage normalmap
		map textures/eX/gl2/eX_floor_grate_03_128_local
	}
	{
		stage specularmap
		map textures/eX/gl2/eX_floor_grate_03_128_s
		specularreflectance 0.52050
		specularexponent 128
	}
	{
		map $lightmap
		blendfunc filter
		depthfunc equal
	}
}