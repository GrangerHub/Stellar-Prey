// Tremulous 1.1.0
// common shaders for the radiant map editor

textures/common/areaportal
{
	qer_trans 0.50
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm structural
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm areaportal
}

textures/common/caulk
{
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nomarks
}

textures/common/clip
{
	qer_trans 0.40
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	surfaceparm playerclip
}

textures/common/clusterportal
{
	qer_trans 0.50
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm clusterportal
}

textures/common/cushion
{
	qer_nocarve
	qer_trans 0.50
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm nodamage
}

textures/common/donotenter
{
	qer_trans 0.50
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm donotenter
}

textures/common/full_clip // use weapclip instead
{
	qer_trans 0.40
	surfaceparm nodraw
	surfaceparm playerclip
}

textures/common/hint
{
	qer_nocarve
	qer_trans 0.30
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm structural
	surfaceparm trans
	surfaceparm noimpact
	surfaceparm hint
}

textures/common/hintskip
{
	qer_nocarve
	qer_trans 0.30
	surfaceparm nodraw
	surfaceparm nonsolid
	surfaceparm skip
	surfaceparm structural
	surfaceparm trans
}

textures/common/invisible // solid, transparent polygons, casts shadows
{
	qer_editorimage textures/common/qer_invisible.tga
	surfaceparm nolightmap
	{
		map textures/common/invisible.tga
		alphaFunc GE128
		depthWrite
		rgbGen vertex
	}
}

textures/common/mirror
{
	qer_editorimage textures/common/qer_mirror.tga
	surfaceparm nolightmap
	portal
	{
		map textures/common/invisible.tga
		blendfunc GL_ONE GL_ONE_MINUS_SRC_ALPHA
		depthWrite
	}
//	{
//		map textures/common/mirror.tga
//		blendFunc GL_ZERO GL_ONE_MINUS_SRC_COLOR
//	}
}

textures/common/missileclip
{
	qer_trans 0.40
	surfaceparm nodamage
	surfaceparm nomarks
	surfaceparm nodraw
	surfaceparm playerclip
	surfaceparm trans
}

textures/common/nodraw
{
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
}

textures/common/nodrawnonsolid
{
	surfaceparm nonsolid
	surfaceparm nolightmap
	surfaceparm nodraw
}

textures/common/nodrop
{
	qer_nocarve
	qer_trans 0.5
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm nodrop
}

textures/common/noimpact
{
	qer_editorimage textures/common/qer_noimpact.tga
	surfaceparm noimpact
}

textures/common/nolightmap
{
	qer_editorimage textures/common/qer_nolightmap.tga
	surfaceparm nolightmap
}

textures/common/origin
{
	qer_nocarve
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm origin
}

textures/common/portal
{
	qer_editorimage textures/common/qer_portal.tga
	surfaceparm nolightmap
	portal
	{
		map textures/common/invisible.tga
		blendfunc GL_ONE GL_ONE_MINUS_SRC_ALPHA
		depthWrite
	}
}

textures/common/slick
{
	qer_trans 0.50
	surfaceparm nodraw
	surfaceparm nolightmap
		 surfaceparm noimpact	
		 	surfaceparm nomarks
	surfaceparm trans
	surfaceparm slick
		surfaceparm nobuild
}

textures/common/terrain
{
	q3map_terrain
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nomarks
}

textures/common/terrain2
{
	q3map_terrain
	qer_editorimage textures/common/terrain.tga
	surfaceparm dust
	surfaceparm nodraw
	surfaceparm nomarks
	surfaceparm nolightmap
}

textures/common/trigger
{
	qer_trans 0.50
	qer_nocarve
	surfaceparm nodraw
}

textures/common/weapclip
{
	qer_trans 0.40
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm trans
	surfaceparm nomarks
}

textures/common/metalclip
{
	qer_trans 0.40
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	surfaceparm playerclip
	surfaceparm metalsteps
}

textures/common/botclip
{
	qer_trans 0.40
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
	surfaceparm noimpact
	surfaceparm botclip
}

textures/common/lightgrid
{
	qer_trans 0.50
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm detail
	surfaceparm nomarks
	surfaceparm trans
	surfaceparm lightgrid
}

textures/common/ladder
{
        qer_trans 0.50
        surfaceparm ladder
        surfaceparm nodraw
        surfaceparm nolightmap
        surfaceparm nonsolid
        surfaceparm trans
        surfaceparm nomarks
        surfaceparm noimpact
        surfaceparm playerclip
}
