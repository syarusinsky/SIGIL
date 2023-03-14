#include "SurfaceTest.hpp"

#include <math.h>

#include "Sprite.hpp"
#include "Engine3D.hpp"
#include "Texture.hpp"

#include <iostream>
#include <fstream>

char         my_font[FONT_FILE_SIZE];
Font*        my_font_ptr = nullptr;

char         test_sprite[SPRITE_FILE_SIZE];
Sprite<CP_FORMAT::RGBA_32BIT>*      test_sprite_ptr = nullptr;

char         test_texture1[TEXTURE_FILE_SIZE];
Texture<CP_FORMAT::RGBA_32BIT>*     test_texture1_ptr = nullptr;

char         test_texture2[TEXTURE_FILE_SIZE];
Texture<CP_FORMAT::RGBA_32BIT>*     test_texture2_ptr = nullptr;

Square my_square = { 0.2f, 0.2f, 0.8f, 0.1f, 0.8f, 0.7f, 0.2f, 0.8f };
Triangle my_triangle = { 0.5f, 0.3f, 0.2f, 0.6f, 0.8f, 0.6f };
float incr = 0.01f;

void initStuff()
{
	unsigned int nSize = FONT_FILE_SIZE;
	std::ifstream fontFile;
	fontFile.open( "../../host/editor/TestStuff/Smoll.sff" );
	fontFile.read( my_font, nSize );
	fontFile.close();

	my_font_ptr = new Font( (uint8_t*)my_font );

	nSize = SPRITE_FILE_SIZE;
	std::ifstream spriteFile;
	spriteFile.open( "../../host/editor/TestStuff/TestImage.sif" );
	spriteFile.read( test_sprite, nSize );
	spriteFile.close();

	test_sprite_ptr = new Sprite<CP_FORMAT::RGBA_32BIT>( (uint8_t*)test_sprite );;

	nSize = TEXTURE_FILE_SIZE;
	std::ifstream textureFile1;
	textureFile1.open( "../../host/editor/TestStuff/eric.sif" );
	textureFile1.read( test_texture1, nSize );
	textureFile1.close();

	nSize = TEXTURE_FILE_SIZE;
	std::ifstream textureFile2;
	textureFile2.open( "../../host/editor/TestStuff/box-texture.sif" );
	textureFile2.read( test_texture2, nSize );
	textureFile2.close();

	test_texture1_ptr = new Texture<CP_FORMAT::RGBA_32BIT>( (uint8_t*)test_texture1 );
	test_texture2_ptr = new Texture<CP_FORMAT::RGBA_32BIT>( (uint8_t*)test_texture2 );
}
