#ifndef SURFACETEST_HPP
#define SURFACETEST_HPP

#include "Surface.hpp"

#define NUM_THREADS 1
#define SHADER_PASS_DATA_SIZE 1024

void initStuff();

class SurfaceTest : public Surface<RENDER_API::SOFTWARE, 640, 480, CP_FORMAT::RGB_24BIT, NUM_THREADS, false, SHADER_PASS_DATA_SIZE>
{
	public:
		SurfaceTest();
		~SurfaceTest();

		void loadMesh1 (const std::string& filePath);
		void loadMesh2 (const std::string& filePath);

		void draw(Graphics<640, 480, CP_FORMAT::RGB_24BIT, RENDER_API::SOFTWARE, false, SHADER_PASS_DATA_SIZE>* graphics) override;

	private:
		Mesh m_Mesh1;
		Mesh m_Mesh2;
};

#endif // SURFACETEST_HPP
