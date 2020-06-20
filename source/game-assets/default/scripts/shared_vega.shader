// -----------------------------------------------------------------
// ENVIRONMENT MAP

// skybox "Vega" by Ingar
textures/shared_vega/sky
{
	qer_editorimage textures/shared_vega_src/sky_p
      
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm sky
      
	// sunlight position red green blue intensity degrees elevation
	//q3map_sun   .9 .9 .9 75 90 25
	q3gl2_sun   .9 .9 .9 75 90 25 1 0.75
	// sunlight intensity
	q3map_surfacelight 150

	skyparms env/shared_vega_src/sky - -
}


// ------------------------------------------------------------
// TEXTURE PACK SHADERS

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// base

// base blue metal
textures/shared_vega/base01
{
	qer_editorimage textures/shared_vega_src/base01_p
	
	{
		stage diffuseMap	
		map textures/shared_vega_src/base01_d
	}
	{
		stage normalParallaxMap	
		map textures/shared_vega_src/base01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap	
		map textures/shared_vega_src/base01_s
		specularreflectance 0.52050
		specularexponent 128

	}
}

// base blue metal (nonsolid)
textures/shared_vega/base01_nonsolid
{
	qer_editorimage textures/shared_vega_src/base01_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/base01_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/base01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/base01_s
		specularreflectance 0.52050
		specularexponent 128
	}
	
	surfaceparm nonsolid
	surfaceparm nomarks
	cull none
}

// base black metal
textures/shared_vega/base01a
{
	qer_editorimage textures/shared_vega_src/base01a_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/base01a_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/base01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/base01_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

// base black metal (nonsolid)
textures/shared_vega/base01a_nonsolid
{
	qer_editorimage textures/shared_vega_src/base01a_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/base01a_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/base01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/base01_s
		specularreflectance 0.52050
		specularexponent 128
	}
	
	surfaceparm nonsolid
	surfaceparm nomarks
	cull none
}

textures/shared_vega/base02
{
	qer_editorimage textures/shared_vega_src/base02_p
	
	{
		stage diffuseMap	
		map textures/shared_vega_src/base02_d
	}
	{
		stage normalParallaxMap	
		map textures/shared_vega_src/base02_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap	
		map textures/shared_vega_src/base02_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/base02_nonsolid
{
	qer_editorimage textures/shared_vega_src/base02_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/base02_d
	}
	{
		stage normalParallaxMap	
		map textures/shared_vega_src/base02_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/base02_s
		specularreflectance 0.52050
		specularexponent 128
	}
	
	surfaceparm nonsolid
	surfaceparm nomarks
	cull none
}

textures/shared_vega/base03
{
	qer_editorimage textures/shared_vega_src/base03_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/base03_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/base03_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/base03_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/base03_nonsolid
{
	qer_editorimage textures/shared_vega_src/base03_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/base03_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/base03_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/base03_s
		specularreflectance 0.52050
		specularexponent 128
	}
	
	surfaceparm nonsolid
	surfaceparm nomarks
	cull none
}

textures/shared_vega/base03a
{
	qer_editorimage textures/shared_vega_src/base03a_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/base03a_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/base03_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/base03_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/base03a_nonsolid
{
	qer_editorimage textures/shared_vega_src/base03a_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/base03a_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/base03_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/base03_s
		specularreflectance 0.52050
		specularexponent 128
	}
	
	surfaceparm nonsolid
	surfaceparm nomarks
	cull none
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// concrete

textures/shared_vega/concrete01
{
	qer_editorimage textures/shared_vega_src/concrete01_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/concrete01_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/concrete01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/concrete01_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// asphalt

textures/shared_vega/asphalt01
{
	qer_editorimage textures/shared_vega_src/asphalt01_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/asphalt01_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/asphalt01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/asphalt01_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// panels

textures/shared_vega/panel01
{
	qer_editorimage textures/shared_vega_src/panel01_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/panel01_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/panel01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/panel01_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/panel01_nocull
{
	qer_editorimage textures/shared_vega_src/panel01_p
	
	{
		stage diffuseMap 
		map textures/shared_vega_src/panel01_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/panel01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/panel01_s
		specularreflectance 0.52050
		specularexponent 128
	}
	
	cull none
}

textures/shared_vega/panel01a
{
	qer_editorimage textures/shared_vega_src/panel01a_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/panel01a_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/panel01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/panel01_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/panel02
{
	qer_editorimage textures/shared_vega_src/panel02_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/panel02_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/panel02_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/panel02_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/panel02a
{
	qer_editorimage textures/shared_vega_src/panel02a_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/panel02a_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/panel02_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/panel02_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/panel03
{
	qer_editorimage textures/shared_vega_src/panel03_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/panel03_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/panel03_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/panel03_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/panel03a
{
	qer_editorimage textures/shared_vega_src/panel03a_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/panel03a_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/panel03_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/panel03_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/panel04
{
	qer_editorimage textures/shared_vega_src/panel04_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/panel04_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/panel04_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/panel04_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/panel04a
{
	qer_editorimage textures/shared_vega_src/panel04a_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/panel04a_d
	}
	{
		stage normalParallaxMap
		map  textures/shared_vega_src/panel04_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/panel04_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

// panel with lights side)
textures/shared_vega/panel05_300
{
	qer_editorimage textures/shared_vega_src/panel05_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/panel05_d
	}
	{
		stage normalParallaxMap	
		map textures/shared_vega_src/panel05_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/panel05_s
		specularreflectance 0.52050
		specularexponent 128
	}
	//glowmap         textures/shared_vega_src/panel05_a
	
	q3map_surfacelight 300
}

textures/shared_vega/panel05_500
{
	qer_editorimage textures/shared_vega_src/panel05_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/panel05_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/panel05_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/panel05_s
		specularreflectance 0.52050
		specularexponent 128
	}
	//glowmap         textures/shared_vega_src/panel05_a
	
	q3map_surfacelight 500
}

// panel with lights (centered)
textures/shared_vega/panel06_300
{
	qer_editorimage textures/shared_vega_src/panel06_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/panel06_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/panel06_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/panel06_s
		specularreflectance 0.52050
		specularexponent 128
	}
	//glowmap         textures/shared_vega_src/panel06_a
	
	q3map_surfacelight 300
}

textures/shared_vega/panel06_500
{
	qer_editorimage textures/shared_vega_src/panel06_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/panel06_d
	}
	{
		stage normalParallaxMap	
		map textures/shared_vega_src/panel06_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/panel06_s
		specularreflectance 0.52050
		specularexponent 128
	}
	//glowmap         textures/shared_vega_src/panel06_a
	
	q3map_surfacelight 500
}

textures/shared_vega/panel06broken
{
	qer_editorimage textures/shared_vega_src/panel06broken_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/panel06broken_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/panel06_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/panel06_s
		specularreflectance 0.52050
		specularexponent 128
	}
	
	q3map_surfacelight 100
	
	// -- lightmap style does not work
	// q3map_lightstyle 1	
	// -- add the following to worldspawn
	// _style1rgbGen wave sawtooth 0.0 1.0 0 0.5
	
	{
		map textures/shared_vega_src/panel06_a
		blend add
		rgbGen wave sawtooth 0.0 1.0 0 0.5
	}
}

textures/shared_vega/panel07
{
	qer_editorimage textures/shared_vega_src/panel07_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/panel07_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/panel07_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/panel07_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/panel07a
{
	qer_editorimage textures/shared_vega_src/panel07a_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/panel07a_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/panel07_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/panel07_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/panel07b
{
	qer_editorimage textures/shared_vega_src/panel07b_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/panel07b_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/panel07_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/panel07_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/panel07c
{
	qer_editorimage textures/shared_vega_src/panel07c_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/panel07c_d
	}
	{	
		stage normalParallaxMap
		map textures/shared_vega_src/panel07_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/panel07_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

// beige panels
textures/shared_vega/panel08
{
	qer_editorimage textures/shared_vega_src/panel08_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/panel08_d
	}
	{
		stage normalParallaxMap	
		map textures/shared_vega_src/panel08_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/panel08_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

// beige panels with light strips
textures/shared_vega/panel08a_300
{
	qer_editorimage textures/shared_vega_src/panel08a_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/panel08a_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/panel08_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/panel08_s
		specularreflectance 0.52050
		specularexponent 128
	}
	//glowmap         textures/shared_vega_src/panel08a_a
	
	q3map_surfacelight 300
	q3map_lightRGB 1.0 1.0 0.93
}

// metal panel with round blue lights
textures/shared_vega/light01_300
{
	qer_editorimage textures/shared_vega_src/light01_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/light01_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/light01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/light01_s
		specularreflectance 0.52050
		specularexponent 128
	}
	//glowmap         textures/shared_vega_src/light01_a
	
	q3map_surfacelight 300
	// blue #73C0D7
	q3map_lightRGB 0.6078 0.7529 0.8431
}

// metal panel (dark) with round blue lights
textures/shared_vega/light01a_300
{
	qer_editorimage textures/shared_vega_src/light01a_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/light01a_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/light01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/light01_s
		specularreflectance 0.52050
		specularexponent 128
	}
	//glowmap         textures/shared_vega_src/light01_a
	
	q3map_surfacelight 300
	// blue #73C0D7
	q3map_lightRGB 0.6078 0.7529 0.8431
}

// metal panel with round brown lights
textures/shared_vega/light02_300
{
	qer_editorimage textures/shared_vega_src/light02_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/light02_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/light02_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/light02_s
		specularreflectance 0.52050
		specularexponent 128
	}
	//glowmap         textures/shared_vega_src/light02_a
	
	q3map_surfacelight 300
	// brown #BEB39A
	q3map_lightRGB 0.745098 0.701961 0.603922
}

// metal panel with round brown lights (no light)
textures/shared_vega/light02_off
{
	qer_editorimage textures/shared_vega_src/light02_off_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/light02_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/light02_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/light02_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

// metal panel (dark) with round brown lights
textures/shared_vega/light02a_300
{
	qer_editorimage textures/shared_vega_src/light02a_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/light02a_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/light02_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/light02_s
		specularreflectance 0.52050
		specularexponent 128
	}
	//glowmap         textures/shared_vega_src/light02_a
	
	q3map_surfacelight 300
	// brown #BEB39A
	q3map_lightRGB 0.745098 0.701961 0.603922
}

// metal panel (dark) with round brown lights (no light)
textures/shared_vega/light02a_off
{
	qer_editorimage textures/shared_vega_src/light02a_off_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/light02a_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/light02_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/light02_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

// small round blue light (blue metal background)
textures/shared_vega/light03_1500
{
	qer_editorimage textures/shared_vega_src/light03_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/light03_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/light03_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/light03_s
		specularreflectance 0.52050
		specularexponent 128
	}
	//glowmap         textures/shared_vega_src/light03_a
	
	q3map_surfacelight 1500
	// blue #73C0D7
	q3map_lightRGB 0.6078 0.7529 0.8431
}

// small round blue light (dark metal background)
textures/shared_vega/light03a_1500
{
	qer_editorimage textures/shared_vega_src/light03a_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/light03a_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/light03_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/light03_s
		specularreflectance 0.52050
		specularexponent 128
	}
	//glowmap         textures/shared_vega_src/light03_a
	
	q3map_surfacelight 1500
	// blue #73C0D7
	q3map_lightRGB 0.6078 0.7529 0.8431
}

// small round brown light (blue metal background)
textures/shared_vega/light04_1500
{
	qer_editorimage textures/shared_vega_src/light04_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/light04_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/light04_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/light04_s
		specularreflectance 0.52050
		specularexponent 128
	}
	//glowmap         textures/shared_vega_src/light04_a
	
	q3map_surfacelight 1500
	// brown #BEB39A
	q3map_lightRGB 0.745098 0.701961 0.603922
}

// small round brown light (dark metal background)
textures/shared_vega/light04a_1500
{
	qer_editorimage textures/shared_vega_src/light04a_p
	
	{
		stage diffuseMap	
		map textures/shared_vega_src/light04a_d
	}
	{
		stage normalParallaxMap	
		map textures/shared_vega_src/light04_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/light04_s
		specularreflectance 0.52050
		specularexponent 128
	}
	//glowmap         textures/shared_vega_src/light04_a
	
	q3map_surfacelight 1500
	// brown #BEB39A
	q3map_lightRGB 0.745098 0.701961 0.603922
}

// small square light
textures/shared_vega/squarelight01_off
{
	qer_editorimage textures/shared_vega_src/squarelight01_off_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/squarelight01_off_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/squarelight01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/squarelight01_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/squarelight01_white_1500
{
	qer_editorimage textures/shared_vega_src/squarelight01_white_p
	
	q3map_surfacelight 1500
	
	{
		stage diffuseMap
		map textures/shared_vega_src/squarelight01_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/squarelight01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/squarelight01_s
		specularreflectance 0.52050
		specularexponent 128
	}
	{
		blend add
		map textures/shared_vega_src/squarelight01_a
	}
}

textures/shared_vega/squarelight01_blue_1500
{
	qer_editorimage textures/shared_vega_src/squarelight01_blue_p
	
	// blue #73C0D7
	q3map_lightRGB 0.6078 0.7529 0.8431
	q3map_surfacelight 1500
	
	{
		stage diffuseMap
		map textures/shared_vega_src/squarelight01_d
	}
	{
		stage normalParallaxMap	
		map textures/shared_vega_src/squarelight01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap	
		map textures/shared_vega_src/squarelight01_s	
		specularreflectance 0.52050
		specularexponent 128
	}
	{
		blend add
		map textures/shared_vega_src/squarelight01_a
	}
}

textures/shared_vega/squarelight01_red_1500
{
	qer_editorimage textures/shared_vega_src/squarelight01_red_p
	
	// red
	q3map_lightRGB 1.0 0.0 0.0
	q3map_surfacelight 1500

	{
		stage diffuseMap
		map textures/shared_vega_src/squarelight01_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/squarelight01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/squarelight01_s	
		specularreflectance 0.52050
		specularexponent 128
	}
	{
		blend add
		map textures/shared_vega_src/squarelight01_a
	}
}

textures/shared_vega/squarelight01_green_1500
{
	qer_editorimage textures/shared_vega_src/squarelight01_green_p
	
	// green
	q3map_lightRGB 0.0 1.0 0.0
	q3map_surfacelight 1500

	{
		stage diffuseMap
		map textures/shared_vega_src/squarelight01_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/squarelight01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/squarelight01_s	
		specularreflectance 0.52050
		specularexponent 128
	}
	{
		blend add
		map textures/shared_vega_src/squarelight01_a
	}
}

textures/shared_vega/squarelight01_yellow_1500
{
	qer_editorimage textures/shared_vega_src/squarelight01_yellow_p
	
	// yellow
	q3map_lightRGB 1.0 1.0 0.0
	q3map_surfacelight 1500

	{
		stage diffuseMap
		map textures/shared_vega_src/squarelight01_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/squarelight01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/squarelight01_s	
		specularreflectance 0.52050
		specularexponent 128
	}
	{
		blend add
		map textures/shared_vega_src/squarelight01_a
	}
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// dirt

textures/shared_vega/dirt01
{
	qer_editorimage textures/shared_vega_src/dirt01_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/dirt01_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/dirt01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/dirt01_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// glass

textures/shared_vega/glass01
{
	qer_editorimage textures/shared_vega_src/glass01_p
	qer_trans .7
	
	cull none
	
	{
		map textures/shared_vega_src/glass01_env
		blendFunc gl_dst_color gl_one
		tcGen environment
	}
	{
		map textures/shared_vega_src/glass01_b
		blend filter
	}
	{
		map $lightmap
		blendFunc gl_dst_color gl_one
	}
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// rust

textures/shared_vega/rust01
{
	qer_editorimage textures/shared_vega_src/rust01_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/rust01_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/rust01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/rust01_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// tiles

textures/shared_vega/tile01
{
	qer_editorimage textures/shared_vega_src/tile01_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/tile01_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/tile01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/tile01_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/tile01a
{
	qer_editorimage textures/shared_vega_src/tile01a_p
	
	{
		stage diffuseMap 
		map textures/shared_vega_src/tile01a_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/tile01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/tile01_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/tile02
{
	qer_editorimage textures/shared_vega_src/tile02_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/tile02_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/tile02_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/tile02_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/tile02a
{
	qer_editorimage textures/shared_vega_src/tile02a_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/tile02a_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/tile02_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/tile02_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/tile03
{
	qer_editorimage textures/shared_vega_src/tile03_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/tile03_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/tile03_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/tile03_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// trims

textures/shared_vega/trim01
{
	qer_editorimage textures/shared_vega_src/trim01_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/trim01_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/trim01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/trim01_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/trim01a
{
	qer_editorimage textures/shared_vega_src/trim01a_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/trim01a_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/trim01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/trim01_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/trim02
{
	qer_editorimage textures/shared_vega_src/trim02_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/trim02_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/trim02_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/trim02_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/trim02a
{
	qer_editorimage textures/shared_vega_src/trim02a_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/trim02a_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/trim02_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/trim02_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/trim03
{
	qer_editorimage textures/shared_vega_src/trim03_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/trim03_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/trim03_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/trim03_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/trim03a
{
	qer_editorimage textures/shared_vega_src/trim03a_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/trim03a_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/trim03_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/trim03a_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/trim03b
{
	qer_editorimage textures/shared_vega_src/trim03b_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/trim03b_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/trim03_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/trim03_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/trim03b_white_1000
{
	qer_editorimage textures/shared_vega_src/trim03b_white_p
	
	{
		stage normalParallaxMap
		map textures/shared_vega_src/trim03_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage diffuseMap
		map textures/shared_vega_src/trim03b_light_d
	}
	{
		stage specularMap
		map textures/shared_vega_src/trim03b_light_s
		specularreflectance 0.52050
		specularexponent 128
	}
	//glowmap textures/shared_vega_src/trim03b_white_a
	
	q3map_surfacelight 1000
}

textures/shared_vega/trim03b_green_1000
{
	qer_editorimage textures/shared_vega_src/trim03b_green_p
	
	{
		stage normalParallaxMap
		map textures/shared_vega_src/trim03_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage diffuseMap
		map textures/shared_vega_src/trim03b_light_d	
	}
	{
		stage specularMap
		map textures/shared_vega_src/trim03b_light_s
		specularreflectance 0.52050
		specularexponent 128
	}
	//glowmap         textures/shared_vega_src/trim03b_green_a
		
    // green #b3ffb3
    q3map_lightRGB 0.7020 1.0 0.7020
        
	q3map_surfacelight 1000
}

textures/shared_vega/trim04
{
	qer_editorimage textures/shared_vega_src/trim04_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/trim04_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/trim04_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/trim04_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/trim05
{
	qer_editorimage textures/shared_vega_src/trim05_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/trim05_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/trim05_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/trim05_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/trim06
{
	qer_editorimage textures/shared_vega_src/trim06_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/trim06_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/trim06_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/trim06_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/trim07
{
	qer_editorimage textures/shared_vega_src/trim07_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/trim07_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/trim07_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/trim07_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

// elevator light strip
textures/shared_vega/trim08
{
	qer_editorimage textures/shared_vega_src/trim08_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/trim08_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/trim08_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/trim08_s
		specularreflectance 0.52050
		specularexponent 128
	}
	
	q3map_surfacelight 500
	// yellow #ffff80
	q3map_lightRGB  1.0 1.0 0.5020
	
	{
		map textures/shared_vega_src/trim08_1_a
		blend add
		// rgbGen wave <func> <base> <amp> <phase> <freq>
		rgbGen wave square .25 0.75 0 1
	}
	{
		map textures/shared_vega_src/trim08_2_a
		blend add
		// rgbGen wave <func> <base> <amp> <phase> <freq>
		rgbGen wave square .25 0.75 .5 1
	}
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// grates

// grate01
textures/shared_vega/grate01
{
	qer_editorimage textures/shared_vega_src/grate01_p
	qer_trans .7
	
	surfaceparm nomarks
	surfaceparm metalsteps
	
	// polygonOffset fixes Z-fighting with objects on top of the grate
	polygonOffset
	cull none
	
	{
		map textures/shared_vega_src/grate01_b
		alphafunc GE128
		blend blend
		depthwrite
	}
	{
		map $lightmap
		depthfunc equal
		blend filter
		rgbGen identity
	}
}

// grate01 (nonsolid)
textures/shared_vega/grate01_nonsolid
{
	qer_editorimage textures/shared_vega_src/grate01_p
	qer_trans .7
	
	surfaceparm nomarks
	surfaceparm nonsolid
	surfaceparm metalsteps
	
	cull none
	
	{
		map textures/shared_vega_src/grate01_b
		alphafunc GE128
		blend blend
		depthwrite
	}
	{
		map $lightmap
		depthfunc equal
		blend filter
		rgbGen identity
	}
}

// grate02
textures/shared_vega/grate02
{
	qer_editorimage textures/shared_vega_src/grate02_p
	qer_trans .7
	
	surfaceparm nomarks
	surfaceparm metalsteps
	
	// polygonOffset fixes Z-fighting with objects on top of the grate
	polygonOffset
	cull none
	
	{
		map textures/shared_vega_src/grate02_b
		alphafunc GE128
		blend blend
		depthwrite
	}
	{
		map $lightmap
		depthfunc equal
		blend filter
		rgbGen identity
	}
}

// grate02 (nonsolid)
textures/shared_vega/grate02_nonsolid
{
	qer_editorimage textures/shared_vega_src/grate02_p
	qer_trans .7
	
	surfaceparm nomarks
	surfaceparm nonsolid
	surfaceparm metalsteps
	
	cull none
	
	{
		map textures/shared_vega_src/grate02_b
		alphafunc GE128
		blend blend
		depthwrite
	}
	{
		map $lightmap
		depthfunc equal
		blend filter
		rgbGen identity
	}
}


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
//floors

textures/shared_vega/floor01
{
	qer_editorimage textures/shared_vega_src/floor01_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/floor01_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/floor01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/floor01_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/floor02
{
	qer_editorimage textures/shared_vega_src/floor02_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/floor02_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/floor02_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/floor02_s
		specularreflectance 0.52050
		specularexponent 128
	}
	
	surfaceparm metalsteps
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// pipes

textures/shared_vega/pipe01
{
	qer_editorimage textures/shared_vega_src/pipe01_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/pipe01_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/pipe01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/pipe01_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/pipe01a
{
	qer_editorimage textures/shared_vega_src/pipe01a_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/pipe01_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/pipe01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/pipe01_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// wood

textures/shared_vega/wood01
{
	qer_editorimage textures/shared_vega_src/wood01_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/wood01_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/wood01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/wood01_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// controls

textures/shared_vega/controls01
{
	qer_editorimage textures/shared_vega_src/controls01_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/controls01_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/controls01_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/controls01_s
		specularreflectance 0.52050
		specularexponent 128
	}
	
	{
		map textures/shared_vega_src/controls01_a
		blend add
		rgbGen wave square 0.0 1.0 0 1
	}
}

textures/shared_vega/controls02
{
	qer_editorimage textures/shared_vega_src/controls02_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/controls02_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/controls02_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/controls02_s
		specularreflectance 0.52050
		specularexponent 128
	}
	//glowmap		textures/shared_vega_src/controls02_a
	
	q3map_surfacelight 50
	q3map_lightRGB 1.0 1.0 0.93
}

textures/shared_vega/controls03
{
	qer_editorimage textures/shared_vega_src/controls03_p
	
	{
		stage diffuseMap
		map textures/shared_vega_src/controls03_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/controls03_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/controls03_s
		specularreflectance 0.52050
		specularexponent 128
	}
	
	q3map_surfacelight 50
	q3map_lightRGB 1.0 0.0 0.0
	
	{
		map textures/shared_vega_src/controls03_a
		blend add
		rgbGen wave square 0.0 1.0 0 1
	}
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// posters

// radioactive warning sign
textures/shared_vega/poster_radioactive
{
	qer_editorimage textures/shared_vega_src/poster_radioactive_p

	surfaceparm nomarks
	surfaceparm nonsolid	
	polygonOffset
	sort decal
	
        
	{
		// this diffuse map does not have an alpha channel
		stage diffuseMap
		map textures/shared_vega_src/poster_radioactive_d
	}
}

// kharnov premium poster
textures/shared_vega/poster_vodka
{
	qer_editorimage textures/shared_vega_src/poster_vodka_p

	surfaceparm nomarks
	surfaceparm nonsolid	
	polygonOffset
	sort decal
	
        
	{
		// this diffuse map does not have an alpha channel
		stage diffuseMap
		map textures/shared_vega_src/poster_vodka_p

	}
}

// kyp name tag
textures/shared_vega/poster_kyp
{
	qer_editorimage textures/shared_vega_src/poster_kyp_p

	surfaceparm nomarks
	surfaceparm nonsolid
	polygonOffset
	sort decal
	
        
	{
		// this diffuse map does not have an alpha channel
		stage diffuseMap
		map textures/shared_vega_src/poster_kyp_d
	}
}

// easter eggs drawing by Josky=KCT=
textures/shared_vega/poster_eastereggs
{
	qer_editorimage textures/shared_vega_src/poster_eastereggs_p

	surfaceparm nomarks
	surfaceparm nonsolid	
	polygonOffset
	sort decal
	
        
	{
		// this diffuse map does not have an alpha channel
		stage diffuseMap
		map textures/shared_vega_src/poster_eastereggs_d
	}
}


// high voltage warning sign
textures/shared_vega/poster_voltage
{
	qer_editorimage textures/shared_vega_src/poster_voltage_p

	surfaceparm nomarks
	surfaceparm nonsolid
	polygonOffset
	sort decal
	
        
	{
		// this diffuse map does not have an alpha channel
		stage diffuseMap
		map textures/shared_vega_src/poster_voltage_d
	}
}


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// safebox

textures/shared_vega/safebox41
{
	qer_editorimage textures/shared_vega_src/safebox41_p

	{
		stage diffuseMap
		map textures/shared_vega_src/safebox41_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/safebox_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/safebox41_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/safebox42
{
	qer_editorimage textures/shared_vega_src/safebox42_p

	{
		stage diffuseMap
		map textures/shared_vega_src/safebox42_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/safebox_small_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/safebox42_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

textures/shared_vega/safebox46
{
    qer_editorimage textures/shared_vega_src/safebox46_p
    
	{
		stage diffuseMap
		map textures/shared_vega_src/safebox46_d
	}
	{
		stage normalParallaxMap
		map textures/shared_vega_src/safebox_small_n
		normalscale 1.1 1.1 0.5
	}
	{
		stage specularMap
		map textures/shared_vega_src/safebox46_s
		specularreflectance 0.52050
		specularexponent 128
	}
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// water surface

textures/shared_vega/water
{
	qer_editorimage textures/shared_vega_src/water_p
        qer_trans .5
  
        surfaceparm nonsolid
        surfaceparm water
        cull none
        
        q3map_globaltexture
        
        deformVertexes wave 64 sin .25 .25 0 .5
        {
                map textures/shared_vega_src/water_b
                blendFunc GL_dst_color GL_one
                rgbgen identity
                tcmod scale .5 .5
		tcmod scroll .01 .01
        }
}
    
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// plasma

// plasma in glass pipe
textures/shared_vega/plasma_pipe
{
	qer_editorimage textures/shared_vega_src/plasma_pipe_p
	qer_trans .7
	
	surfaceparm nonsolid
	surfaceparm noimpact
	surfaceparm nomarks        
	cull none
	
	q3map_surfacelight 150	
	// radioactive green #00f200
	q3map_lightRGB 0.0 0.95 0.0
	
        {
            map textures/shared_vega_src/plasma_pipe_b
            tcmod scroll 0.0 0.5
			blend blend
        }
        {
            map textures/shared_vega_src/glass01_b
			blend filter
        }

}

// plasma whirlpool
textures/shared_vega/plasma_whirl
{
	qer_editorimage textures/shared_vega_src/plasma_whirl_p
        
	q3map_surfacelight 150
	// radioactive green #00f200
	q3map_lightRGB 0.0 0.95 0.0
	q3map_globaltexture

	surfaceparm nonsolid
	surfaceparm noimpact
	surfaceparm nomarks        
	
        cull none

        // deformVertexes wave <div> <func> <base> <amplitude> <phase> <freq>
        deformVertexes wave 64 sin .25 .25 0 .5
        
        {
            map textures/shared_vega_src/plasma_whirl_b
			//tcmod scale .5 .5
			//tcmod turb 1.0 0.5 0.0 0.1
			tcmod rotate 20
			blend blend
        }
}

// plasma content, has the slime surfaceparm, but doesn't draw anything
// base in a shader from the Tremulous map slimepunk by Taiyo.uk
textures/shared_vega/plasma_nodraw
{
	qer_editorimage textures/shared_vega_src/plasma_nodraw_p
	qer_trans 0.7
	
	surfaceparm noimpact
	surfaceparm slime
	surfaceparm nodraw
	surfaceparm nolightmap
	surfaceparm nonsolid
	surfaceparm trans
	surfaceparm nomarks
}
