//UTCSUD-MATERIAL-FILE (c) Matth ,2016
textures/utcsud/sky
{
      surfaceparm noimpact
      surfaceparm nolightmap
      surfaceparm sky
	  nopicmip
	  nomipmaps
	  q3gl2_sun 1.00 1.00 1.00 400 0 35 1 0.85
      skyparms env/utcsud/ud - -
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
textures/utcsud/utcsud_03
{
	{
		map textures/utcsud/utcsud_03
	}
	{
		stage normalmap
		map textures/utcsud/utcsud_03_n
	}
	{
		stage specularmap
		map textures/utcsud/utcsud_03_s
		specularreflectance 0.6
		specularexponent 128
	}
	{
		map $lightmap
		blendfunc filter
	}
	{
		map textures/utcsud/utcsud_03_blend
		blendfunc add
	}
}

textures/utcsud/utcsud_03_blend_red
{
	{
		map textures/utcsud/utcsud_03
	}
	{
		stage normalmap
		map textures/utcsud/utcsud_03_n
	}
	{
		stage specularmap
		map textures/utcsud/utcsud_03_s
		specularreflectance 0.6
		specularexponent 128
	}
	{
		map $lightmap
		blendfunc filter
	}
	{
		map textures/utcsud/utcsud_03_blend_red
		blendfunc add
	}
}

textures/utcsud/utcsud_03_blend_blue
{
	{
		map textures/utcsud/utcsud_03
	}
	{
		stage normalmap
		map textures/utcsud/utcsud_03_n
	}
	{
		stage specularmap
		map textures/utcsud/utcsud_03_s
		specularreflectance 0.6
		specularexponent 128
	}
	{
		map $lightmap
		blendfunc filter
	}
	{
		map textures/utcsud/utcsud_03_blend_blue
		blendfunc add
	}
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
