#ifndef SURFACETEST_HPP
#define SURFACETEST_HPP

#include "Surface.hpp"

#define NUM_THREADS 4
#define SHADER_PASS_DATA_SIZE 1024

#ifdef SOFTWARE_RENDERING
#define GRAPHICS_API SoftwareGraphics
#else
#define GRAPHICS_API OpenGlGraphics
#endif // SOFTWARE_RENDERING

void initStuff();

class SurfaceTest : public Surface<RENDER_API::SOFTWARE, 640, 480, CP_FORMAT::RGB_24BIT, NUM_THREADS, true, SHADER_PASS_DATA_SIZE>
{
	public:
		SurfaceTest();
		~SurfaceTest();

		void loadMesh1 (const std::string& filePath);
		void loadMesh2 (const std::string& filePath);

		void draw(GRAPHICS_API<640, 480, CP_FORMAT::RGB_24BIT, true, SHADER_PASS_DATA_SIZE>* graphics) override;

	private:
		Mesh m_Mesh1;
		Mesh m_Mesh2;
};

#endif // SURFACETEST_HPP
