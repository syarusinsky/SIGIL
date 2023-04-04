#include "SurfaceHandler.hpp"

void printGLError()
{
	GLenum err = glGetError();
	if ( err != GL_NO_ERROR )
	{
		if ( err == GL_INVALID_ENUM ) std::cout << "INVALID ENUM" << std::endl;
		if ( err == GL_INVALID_VALUE ) std::cout << "INVALID VALUE" << std::endl;
		if ( err == GL_INVALID_OPERATION ) std::cout << "INVALID OPERATION" << std::endl;
		if ( err == GL_INVALID_FRAMEBUFFER_OPERATION ) std::cout << "INVALID FRAMEBUFFER OPERATION" << std::endl;
		if ( err == GL_OUT_OF_MEMORY ) std::cout << "OUT OF MEMORY" << std::endl;
		if ( err == GL_STACK_UNDERFLOW ) std::cout << "STACK UNDERFLOW" << std::endl;
		if ( err == GL_STACK_OVERFLOW ) std::cout << "STACK OVERFLOW" << std::endl;
	}
}
