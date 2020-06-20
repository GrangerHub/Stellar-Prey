models\weapons\rifle\lense
{
	{
		map models/weapons/rifle/lense.tga
		blendfunc add
		tcGen environment 
	}
}
models/weapons/prifle/lense
{
	{
		map models/weapons/prifle/lense.tga
		blendfunc add
		tcGen environment 
	}
}
MarkRifleBullet
{
  polygonOffset
  {
    map gfx/marks/mark_rifle.tga
    //map models/weapons/rifle/lense.tga
    blendFunc GL_ZERO GL_ONE_MINUS_SRC_COLOR
    rgbGen exactVertex
  }
}