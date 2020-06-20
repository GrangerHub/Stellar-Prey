//UTCSUD-SHADER-FILE (c) Matth ,2016
textures/utcsud/wasser
{
        surfaceparm trans
        surfaceparm nonsolid
        surfaceparm water
        cull disable
        deformVertexes wave 64 sin .25 .25 0 .5
        {
                map textures/utcsud/wasser.jpg
                blendFunc GL_DST_COLOR GL_ONE
                rgbgen identity
                tcmod scale .5 .5
		        tcmod scroll .02 0.1
        }
        {
                map textures/utcsud/wasser.jpg
                blendFunc GL_DST_COLOR GL_ONE
                tcmod scale -.5 -.5
                tcmod scroll .025 .025
        }

}
textures/utcsud/credits
{      
	surfaceparm nobuild
	surfaceparm noimpact	
	surfaceparm nomarks
	surfaceparm slick
	surfaceparm nonsolid
	{
        map textures/utcsud/credits.jpg
	}
	{
        map textures/utcsud/credits_add.jpg
		blendfunc GL_ONE GL_ONE
		rgbGen wave noise 0.75 0.5 0 10
    }
	{
		map $lightmap 
		blendfunc filter
		tcGen lightmap 
		depthFunc equal
	}
}
textures/utcsud/utcsud_03_blend_red
{      
	{
		map $lightmap
		rgbGen identity
	}
    {                       
        map textures/utcsud/utcsud_03r.jpg
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
    }
    { 
        map textures/utcsud/utcsud_03_blend_red.jpg
		blendfunc GL_ONE GL_ONE
	}
}
textures/utcsud/utcsud_03_blend_blue
{      
	{
		map $lightmap
		rgbGen identity
	}
    {                       
        map textures/utcsud/utcsud_03b.jpg
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
    }
    { 
        map textures/utcsud/utcsud_03_blend_blue.jpg
		blendfunc GL_ONE GL_ONE
	}
}
textures/utcsud/utcsud_03
{      
	{
		map $lightmap
		rgbGen identity
	}
    {                       
        map textures/utcsud/utcsud_03.jpg
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
    }
    { 
        map textures/utcsud/utcsud_03_blend.jpg
		blendfunc GL_ONE GL_ONE
	}
}
textures/utcsud/utcsud_20
{      
	{
		map $lightmap
		rgbGen identity
	}
    {                       
        map textures/utcsud/utcsud_20.jpg
		blendFunc GL_DST_COLOR GL_ZERO
		rgbGen identity
    }
    { 
        map textures/utcsud/utcsud_20_blend.jpg
		blendfunc GL_ONE GL_ONE
	}
}
textures/utcsud/sky
{
      surfaceparm noimpact
      surfaceparm nolightmap
      surfaceparm sky
	  nopicmip
	  nomipmaps
      skyparms env/utcsud/ud - -
}

textures/utcsud/vent_01
{  
	surfaceparm nomarks
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm detail
	surfaceparm alphashadow
	{

		map textures/utcsud/vent_01.tga
		depthWrite
		alphaFunc GE128
		
	}
	{
		map $lightmap 
		blendfunc filter
		tcGen lightmap 
		depthFunc equal
	}
}
