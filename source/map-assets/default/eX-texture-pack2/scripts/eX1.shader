// written for ioquake3/renderergl2 by mega, aka odin
// original by evillair
textures/eX/eX_lightpanel_01_lit
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

textures/eX/eX_light_u201_lit
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

textures/eX/eX_lightpanel_01_lit_1000
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

textures/eX/eX_light_u201_lit_1000
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

textures/eX/eX_floor_grate03_d
{
	qer_editorimage textures/eX/eX_floor_grate03_d
	surfaceparm metalsteps
	{
		map textures/eX/eX_floor_grate03_d
	}
	{
		map $lightmap
		blendfunc filter
	}
}

textures/eX/eX_floor_grate03_trans
{
	qer_editorimage textures/eX/eX_floor_grate03_d
	surfaceparm alphashadow
	surfaceparm metalsteps
	cull none
	{
		map textures/eX/eX_floor_grate03_d
		alphafunc ge128
		depthwrite
	}
	{
		map $lightmap
		depthfunc equal
		blendfunc filter
	}
}

textures/eX/eX_floor_grate_03_128_d
{
	qer_editorimage textures/eX/eX_floor_grate_03_128_d
	surfaceparm metalsteps
	{
		map textures/eX/eX_floor_grate_03_128_d
	}
	{
		map $lightmap
		blendfunc filter
	}
}

textures/eX/eX_floor_grate_03_128_trans
{
	qer_editorimage textures/eX/eX_floor_grate_03_128_d
	surfaceparm alphashadow
	surfaceparm metalsteps
	cull none
	{
		map textures/eX/eX_floor_grate_03_128_d
		alphafunc ge128
		depthwrite
	}
	{
		map $lightmap
		depthfunc equal
		blendfunc filter
	}
}

textures/eX/eX_floor_grate_03_d
{
	qer_editorimage textures/eX/eX_floor_grate_03_d
	surfaceparm metalsteps
	{
		map textures/eX/eX_floor_grate_03_d
	}
	{
		map $lightmap
		blendfunc filter
	}
}

textures/eX/eX_floor_grate_03_trans
{
	qer_editorimage textures/eX/eX_floor_grate_03_d
	surfaceparm alphashadow
	surfaceparm metalsteps
	cull none
	{
		map textures/eX/eX_floor_grate_03_d
		alphafunc ge128
		depthwrite
	}
	{
		map $lightmap
		depthfunc equal
		blendfunc filter
	}
}

textures/eX/eX_floor_mtl_grate_01_d
{
	qer_editorimage textures/eX/eX_floor_mtl_grate_01_d
	surfaceparm metalsteps
	{
		map textures/eX/eX_floor_mtl_grate_01_d
	}
	{
		map $lightmap
		blendfunc filter
	}
}

textures/eX/eX_floor_mtl_grate_01_trans
{
	qer_editorimage textures/eX/eX_floor_mtl_grate_01_d
	surfaceparm alphashadow
	surfaceparm metalsteps
	cull none
	{
		map textures/eX/eX_floor_mtl_grate_01_d
		alphafunc ge128
		depthwrite
	}
	{
		map $lightmap
		depthfunc equal
		blendfunc filter
	}
}