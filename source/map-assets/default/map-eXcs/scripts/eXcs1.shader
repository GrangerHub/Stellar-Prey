textures/eXcs/sky
{
//    skybox "Wasteland" by Ingar
      qer_editorimage env/eXcs/wasteland
      surfaceparm noimpact
      surfaceparm nolightmap
      surfaceparm sky
// -- surface sky light
      q3map_skylight 225 3
      q3map_lightimage env/eXcs/wasteland
// -- direct sunlight
//    q3map_sunExt <red> <green> <blue> <intensity> <degrees> <elevation> <deviance> <samples>
//      q3map_sunExt 1.00 0.949 0.977 125 245 40 2 16
//      q3map_sunExt 1.00 1.00 1.00 160 245 40 2 16
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
        qer_editorimage textures/eXcs/force_grid
	surfaceparm trans	
        surfaceparm nomarks	
	surfaceparm nolightmap
	cull none
	{
		map textures/eXcs/force_field
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
	qer_editorimage textures/eXcs/bulb_red
	q3map_surfacelight 1000
	surfaceparm nomarks
	surfaceparm trans
	surfaceparm nolightmap
	cull disable
	qer_trans 0.5
	{
		map textures/eXcs/bulb_red
		rgbgen vertex
	}
}

textures/eXcs/fog_darkness
{
	qer_editorimage textures/eXcs/fog_radiant
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
		rgbGen		vertex
		alphaGen	vertex
	}
}

textures/eXcs/eXcs_lightpanel_01_lit
{
	qer_editorimage	textures/eX/eX_lightpanel_01_d
	{
		map textures/eX/eX_lightpanel_01_d
	}
	{
		map $lightmap
		blendfunc filter
	}
	{
		map textures/eX/eX_lightpanel_01_add
		blendfunc add
	}
}

textures/eXcs/eXcs_lightpanel_01_blue_lit
{
	qer_editorimage	textures/eX/eX_lightpanel_01_d
	{
		map textures/eX/eX_lightpanel_01_d
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
	qer_editorimage	textures/eX/eX_lightpanel_01_d
	{
		map textures/eX/eX_lightpanel_01_d
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
	qer_editorimage	textures/eX/eX_light_u201_d
	{
		map textures/eX/eX_light_u201_d
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
	qer_editorimage	textures/eX/eX_light_u201_d
	{
		map textures/eX/eX_light_u201_d
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
	qer_editorimage	textures/eX/eX_light_u201_d
	{
		map textures/eX/eX_light_u201_d
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
	qer_editorimage	textures/eX/eX_lightpanel_01_d
	q3map_surfacelight 1000
	q3map_lightimage textures/eX/eX_lightpanel_01_add
	{
		map textures/eX/eX_lightpanel_01_d
	}
	{
		map $lightmap
		blendfunc filter
	}
	{
		map textures/eX/eX_lightpanel_01_add
		blendfunc add
	}
}

textures/eXcs/eXcs_lightpanel_01_blue_lit_1000
{
	qer_editorimage	textures/eX/eX_lightpanel_01_d
	q3map_surfacelight 1000
	q3map_lightimage textures/eXcs/eXcs_lightpanel_01_blue_add
	{
		map textures/eX/eX_lightpanel_01_d
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
	qer_editorimage	textures/eX/eX_lightpanel_01_d
	q3map_surfacelight 1000
	q3map_lightimage textures/eXcs/eXcs_lightpanel_01_red_add
	{
		map textures/eX/eX_lightpanel_01_d
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
	qer_editorimage	textures/eX/eX_light_u201_d
	q3map_surfacelight 1000
	q3map_lightimage textures/eX/eX_light_u201_add
	{
		map textures/eX/eX_light_u201_d
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
	qer_editorimage	textures/eX/eX_light_u201_d
	q3map_surfacelight 1000
	q3map_lightimage textures/eXcs/eXcs_light_u201_blue_add
	{
		map textures/eX/eX_light_u201_d
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
	qer_editorimage	textures/eX/eX_light_u201_d
	q3map_surfacelight 1000
	q3map_lightimage textures/eXcs/eXcs_light_u201_red_add
	{
		map textures/eX/eX_light_u201_d
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
		map $lightmap
		blendfunc filter
		depthfunc equal
	}
}