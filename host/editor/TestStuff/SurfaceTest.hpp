#ifndef SURFACETEST_HPP
#define SURFACETEST_HPP

#include "Surface.hpp"

#define NUM_THREADS 4

void initStuff();

class SurfaceTest : public Surface<640, 480, CP_FORMAT::RGB_24BIT, NUM_THREADS>
{
	public:
		SurfaceTest();
		~SurfaceTest();

		void loadMesh1 (const std::string& filePath);
		void loadMesh2 (const std::string& filePath);

		void draw(SoftwareGraphics<640, 480, CP_FORMAT::RGB_24BIT, NUM_THREADS>* graphics) override;

	private:
		Mesh m_Mesh1;
		Mesh m_Mesh2;
};

#endif // SURFACETEST_HPP
