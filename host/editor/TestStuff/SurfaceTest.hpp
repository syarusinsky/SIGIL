#ifndef SURFACETEST_HPP
#define SURFACETEST_HPP

#include "Surface.hpp"
#include "ObjFileLoader.hpp"

// PS1 resolution: 640, 480 ARMor8 resolution: 128, 64
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_COLOR_FORMAT CP_FORMAT::RGB_24BIT

#define FONT_FILE_SIZE 779

#define SPRITE_FILE_SIZE 88365

#define TEXTURE_FILE_SIZE 262153

class Font;

extern Font* 				my_font_ptr;

extern Sprite<CP_FORMAT::RGBA_32BIT>* 	test_sprite_ptr;

extern Texture<CP_FORMAT::RGBA_32BIT>* 	test_texture1_ptr;

extern Texture<CP_FORMAT::RGBA_32BIT>*  test_texture2_ptr;

void initStuff();

template <RENDER_API api, unsigned int width, unsigned int height, CP_FORMAT format, unsigned int numThreads, bool include3D,
	unsigned int shaderPassDataSize>
class SurfaceTest : public Surface<api, width, height, format, numThreads, include3D, shaderPassDataSize>
{
	public:
		SurfaceTest();
		~SurfaceTest();

		void setOnlyFont()
		{
			this->setFont( my_font_ptr );
		}

		void loadMesh1 (const std::string& filePath);
		void loadMesh2 (const std::string& filePath);

		void setMeshVShader (void (*shader) (TriShaderData<CP_FORMAT::RGBA_32BIT, shaderPassDataSize>& vShaderData))
		{
			m_VShader = shader;
		}

		void setMeshFShader (void (*shader) (Color& colorOut, TriShaderData<CP_FORMAT::RGBA_32BIT, shaderPassDataSize>& fShaderData, float v1Cur,
					float v2Cur, float v3Cur, float texCoordX, float texCoordY, float lightAmnt))
		{
			m_FShader = shader;
		}

		void draw(Graphics<width, height, format, api, include3D, shaderPassDataSize>* graphics) override;

	private:
		Mesh m_Mesh1;
		Mesh m_Mesh2;

		void (*m_VShader) (TriShaderData<CP_FORMAT::RGBA_32BIT, shaderPassDataSize>& vShaderData);
		void (*m_FShader) (Color& colorOut, TriShaderData<CP_FORMAT::RGBA_32BIT, shaderPassDataSize>& fShaderData, float v1Cur, float v2Cur,
				float v3Cur, float texCoordX, float texCoordY, float lightAmnt);
};

typedef struct square
{
	float topLeftX;
	float topLeftY;
	float topRightX;
	float topRightY;
	float bottomRightX;
	float bottomRightY;
	float bottomLeftX;
	float bottomLeftY;
} Square;

typedef struct triangle
{
	float x1;
	float y1;
	float x2;
	float y2;
	float x3;
	float y3;
} Triangle;

extern Square my_square;
extern Triangle my_triangle;
extern float incr;

template <RENDER_API api, unsigned int width, unsigned int height, CP_FORMAT format, unsigned int numThreads, bool include3D,
	unsigned int shaderPassDataSize>
SurfaceTest<api, width, height, format, numThreads, include3D, shaderPassDataSize>::SurfaceTest() :
	Surface<api, width, height, format, numThreads, include3D, shaderPassDataSize>()
{
}

template <RENDER_API api, unsigned int width, unsigned int height, CP_FORMAT format, unsigned int numThreads, bool include3D,
	unsigned int shaderPassDataSize>
SurfaceTest<api, width, height, format, numThreads, include3D, shaderPassDataSize>::~SurfaceTest()
{
}

template <RENDER_API api, unsigned int width, unsigned int height, CP_FORMAT format, unsigned int numThreads, bool include3D,
	unsigned int shaderPassDataSize>
void SurfaceTest<api, width, height, format, numThreads, include3D, shaderPassDataSize>::loadMesh1 (const std::string& filePath)
{
	ObjFileLoader loader;
	if ( ! loader.createMeshFromFile(m_Mesh1, filePath) )
	{
		std::cout << "Failed to load obj file" << std::endl;
	}
}

template <RENDER_API api, unsigned int width, unsigned int height, CP_FORMAT format, unsigned int numThreads, bool include3D,
	unsigned int shaderPassDataSize>
void SurfaceTest<api, width, height, format, numThreads, include3D, shaderPassDataSize>::loadMesh2 (const std::string& filePath)
{
	ObjFileLoader loader;
	if ( ! loader.createMeshFromFile(m_Mesh2, filePath) )
	{
		std::cout << "Failed to load obj file" << std::endl;
	}
}

template <RENDER_API api, unsigned int width, unsigned int height, CP_FORMAT format, unsigned int numThreads, bool include3D,
	unsigned int shaderPassDataSize>
void SurfaceTest<api, width, height, format, numThreads, include3D, shaderPassDataSize>::draw (Graphics<width, height, format, api, include3D,
		shaderPassDataSize>* graphics)
{
	graphics->setColor( 0.0f, 0.0f, 0.0f );
	graphics->fill();

	static float blue = 0.1f;
	graphics->setColor( 0.73f, 0.56f, blue );

	static bool flip = false;
	if (!flip)
	{
		my_square.topLeftX     += incr;
		my_square.bottomLeftX  += incr;
		my_square.topRightX    -= incr;
		my_square.bottomRightX -= incr;
		blue                   += incr;

		if (my_square.topLeftX >= 0.8f)
		{
			my_square.topLeftX     = 0.8f;
			my_square.bottomLeftX  = 0.8f;
			my_square.topRightX    = 0.2f;
			my_square.bottomRightX = 0.2f;
			flip = !flip;
		}
	}
	else
	{
		my_square.topLeftX     -= incr;
		my_square.bottomLeftX  -= incr;
		my_square.topRightX    += incr;
		my_square.bottomRightX += incr;
		blue                   -= incr;

		if (my_square.topLeftX <= 0.2f)
		{
			my_square.topLeftX     = 0.2f;
			my_square.bottomLeftX  = 0.2f;
			my_square.topRightX    = 0.8f;
			my_square.bottomRightX = 0.8f;
			flip = !flip;
		}
	}

	graphics->drawQuad(
			my_square.topLeftX,     my_square.topLeftY,
			my_square.topRightX,    my_square.topRightY,
			my_square.bottomRightX, my_square.bottomRightY,
			my_square.bottomLeftX,  my_square.bottomLeftY   );

	graphics->setColor( 1.0f, 0.1f, blue );

	graphics->drawTriangle( my_triangle.x1, my_triangle.y1, my_triangle.x2, my_triangle.y2, my_triangle.x3, my_triangle.y3 );

	graphics->drawTriangle ( 0.1f, 0.4f, 0.0f, 0.2f, 0.0f, 0.6f );

	graphics->setColor( 0.2f, 0.9f, blue );

	graphics->drawBoxFilled( 0.7f, 0.2f, 0.9f, 0.5f );

	graphics->setColor( 0.5f, 0.5f, 0.5f );

	static float lineXEnd = -2.0f;
	static float lineIncr = 0.1f;
	graphics->drawLine( 0.5f, 1.0f, lineXEnd, -2.0f );
	lineXEnd += lineIncr;
	if (lineXEnd <= -2.0f)
	{
		lineIncr = 0.1f;
	}
	else if (lineXEnd >= 3.0f)
	{
		lineIncr = -0.1f;
	}

	graphics->setColor( 0.1f, 0.8f, 0.5f );

	static float trianglePixelX = 0.0f;
	static float trianglePixelY = 0.0f;
	static float trianglePixelXIncr = 0.005f;
	static float trianglePixelYIncr = 0.002f;

	graphics->drawTriangleFilled( trianglePixelX, trianglePixelY, 0.7f, 0.7f, 0.3f, 0.3f );

	trianglePixelX += trianglePixelXIncr;
	if (trianglePixelX >= 1.0f)
	{
		trianglePixelX = 1.0f;
		trianglePixelXIncr = -0.005f;
	}
	else if (trianglePixelX <= 0.0f)
	{
		trianglePixelX = 0.0f;
		trianglePixelXIncr = 0.005f;
	}

	trianglePixelY += trianglePixelYIncr;
	if (trianglePixelY >= 1.0f)
	{
		trianglePixelY = 1.0f;
		trianglePixelYIncr = -0.002f;
	}
	else if (trianglePixelY <= 0.0f)
	{
		trianglePixelY = 0.0f;
		trianglePixelYIncr = 0.002f;
	}

	graphics->setColor( 1.0f, 0.7f, 0.7f );

	graphics->drawQuadFilled( trianglePixelX, trianglePixelY, 0.6f, 0.4f, 0.6f, 0.6f, 0.5f, 0.6f );

	graphics->setColor( 0.3f, 1.0f, 0.3f );

	static float boxPixelXStart = -0.05f;
	static float boxPixelYStart = -0.05f;
	static float boxPixelIncr = 0.01f;

	graphics->drawBoxFilled( boxPixelXStart, boxPixelYStart, boxPixelXStart + 0.1f, boxPixelYStart + 0.1f );

	boxPixelXStart += boxPixelIncr;
	boxPixelYStart += boxPixelIncr;

	if (boxPixelXStart >= 2.0f)
	{
		boxPixelIncr = -0.01f;
	}
	else if (boxPixelXStart <= -1.0f)
	{
		boxPixelIncr = 0.01f;
	}

	static float originX = 0.5f;
	static float originY = 0.5f;
	static float originXIncr = -0.05f;
	static float originYIncr = -0.02f;
	graphics->setColor( 1.0f, 1.0f, 1.0f );
	graphics->drawCircleFilled( originX, originY, 0.2f );

	originX += originXIncr;
	originY += originYIncr;

	if (originX < -0.5f)
	{
		originX = -0.5f;
		originXIncr = 0.05f;
	}
	else if (originX > 1.5f)
	{
		originX = 1.5f;
		originXIncr = -0.05f;
	}

	if (originY < -0.5f)
	{
		originY = -0.5f;
		originYIncr = 0.02f;
	}
	else if (originY > 1.5f)
	{
		originY = 1.5f;
		originYIncr = -0.02f;
	}

	static float textScale = 10.0f;
	static float textScaleIncr = -0.1f;
	static float textX = 0.5f;
	static float textY = 0.5f;
	static float textXMov = -0.01f;
	static float textYMov = 0.005f;
	graphics->setColor( 0.5f, 1.0f, 1.0f );
	graphics->drawText( textX, textY, "siike", textScale );
	textScale += textScaleIncr;
	textX += textXMov;
	textY += textYMov;
	if ( textScale > 10.0f || textScale < 0.5f ) textScaleIncr = textScaleIncr * -1.0f;
	if ( textX > 1.2f || textX < -0.5f) textXMov = textXMov * -1.0f;
	if ( textY < -0.2f || textY > 1.0f ) textYMov = textYMov * -1.0f;

	/*
	// tests all characters
	graphics->setColor( 0.5f, 1.0f, 1.0f );
	graphics->drawText( 0.0f, 0.0f, "abcdefghijklmnopqrstuvwxyz", 3.0f );
	graphics->drawText( 0.0f, 0.1f, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 3.0f );
	graphics->drawText( 0.0f, 0.2f, "0123456789`~!@#$%^&*()-_=+", 3.0f );
	graphics->drawText( 0.0f, 0.3f, "[{]}\\|;:'\",<.>/?", 3.0f );
	*/

	static float spriteScale = 1.5f;
	static float spriteScaleIncr = -0.01f;
	static float spriteX = 0.2f;
	static float spriteY = 0.2f;
	static float spriteXMov = 0.005f;
	static float spriteYMov = -0.01f;
	static int   spriteRotDegrees = 0;
	static int   spriteRotIncr = 1;
	test_sprite_ptr->setRotationAngle( spriteRotDegrees );
	test_sprite_ptr->setScaleFactor( spriteScale );
	graphics->drawSprite( spriteX, spriteY, *test_sprite_ptr );
	spriteScale += spriteScaleIncr;
	spriteX += spriteXMov;
	spriteY += spriteYMov;
	spriteRotDegrees += spriteRotIncr;
	if ( spriteScale > 1.5f || spriteScale < 0.2f ) spriteScaleIncr = spriteScaleIncr * -1.0f;
	if ( spriteX > 0.9f || spriteX < -0.1f ) spriteXMov = spriteXMov * -1.0f;
	if ( spriteY > 0.7f || spriteY < -0.2f ) spriteYMov = spriteYMov * -1.0f;

	// TODO remove this once the 3D engine has a proper mesh rendering algorithm
	// draw cube
	std::array<Texture<CP_FORMAT::RGBA_32BIT>*, 5> texArray1 = { test_texture1_ptr };
	std::array<Texture<CP_FORMAT::RGBA_32BIT>*, 5> texArray2 = { test_texture2_ptr };
	float aspectRatio = static_cast<float>(this->getWidth()) / static_cast<float>(this->getHeight());
	Camera3D camera( 0.01f, 10.0f, 60.0f, aspectRatio );
	Mesh model1 = m_Mesh1;
	Mesh model2 = m_Mesh2;
	model1.transformMat = generateIdentityMatrix();
	model2.transformMat = generateIdentityMatrix();
	static float xTranslate = 0.0f;
	static float xTranslateIncr = 0.01f;
	static float xRotation = 0.0f;
	static float xRotationIncr = 1.0f;
	TriShaderData<CP_FORMAT::RGBA_32BIT, shaderPassDataSize> shaderData1{ texArray1, camera, Color(), nullptr, m_VShader, m_FShader };
	TriShaderData<CP_FORMAT::RGBA_32BIT, shaderPassDataSize> shaderData2{ texArray2, camera, Color(), nullptr, m_VShader, m_FShader };
	model1.rotate( 180.0f, xRotation, 0.0f );
	model2.rotate( xRotation, xRotation * 0.5f, 0.0f );
	model1.translate( 0.0f, 0.0f, 2.5f );
	model2.translate( 0.0f, 0.0f, 8.0f );
	model1.translate( 0.0f, xTranslate * 0.5f, 0.0f );
	model2.translate( xTranslate * 2.0f, 0.0f, 0.0f );
	for ( Face face : model1.faces )
	{
		// TODO transforms should be applied in a pipeline
		face.vertices[0].vec = face.vertices[0].vec * model1.transformMat;
		face.vertices[1].vec = face.vertices[1].vec * model1.transformMat;
		face.vertices[2].vec = face.vertices[2].vec * model1.transformMat;
		face.vertices[0].normal = face.vertices[0].normal * model1.transformMat;
		face.vertices[1].normal = face.vertices[1].normal * model1.transformMat;
		face.vertices[2].normal = face.vertices[2].normal * model1.transformMat;

		graphics->drawTriangleShaded( face, shaderData1 );
	}

	for ( Face face : model2.faces )
	{
		// TODO transforms should be applied in a pipeline
		face.vertices[0].vec = face.vertices[0].vec * model2.transformMat;
		face.vertices[1].vec = face.vertices[1].vec * model2.transformMat;
		face.vertices[2].vec = face.vertices[2].vec * model2.transformMat;
		face.vertices[0].normal = face.vertices[0].normal * model2.transformMat;
		face.vertices[1].normal = face.vertices[1].normal * model2.transformMat;
		face.vertices[2].normal = face.vertices[2].normal * model2.transformMat;

		graphics->drawTriangleShaded( face, shaderData2 );
	}

	xTranslate += xTranslateIncr;
	if ( xTranslate >= 3.0f )
	{
		xTranslateIncr *= -1.0f;
	}
	else if ( xTranslate <= -3.0f )
	{
		xTranslateIncr *= -1.0f;
	}
	xRotation += xRotationIncr;
	if ( xRotation >= 360.0f || xRotation <= 0.0f )
	{
		xRotationIncr = -xRotationIncr;
	}

	// graphics->drawDepthBuffer( camera );
}
#endif // SURFACETEST_HPP
