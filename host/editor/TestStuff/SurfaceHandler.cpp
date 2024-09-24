#include "SurfaceHandler.hpp"

void printGLError()
{
	GLenum err = glGetError();
	if ( err != GL_NO_ERROR )
	{
		if ( err == GL_INVALID_ENUM ) SLOG::log( LogLevels::ERROR, LogMethodsE::ERROR, "OpenGL error : invalid enum" );
		if ( err == GL_INVALID_VALUE ) SLOG::log( LogLevels::ERROR, LogMethodsE::ERROR, "OpenGL error : invalid value" );
		if ( err == GL_INVALID_OPERATION ) SLOG::log( LogLevels::ERROR, LogMethodsE::ERROR, "OpenGL error : invalid operation" );
		if ( err == GL_INVALID_FRAMEBUFFER_OPERATION ) SLOG::log( LogLevels::ERROR, LogMethodsE::ERROR, "OpenGL error : invalid framebuffer operation" );
		if ( err == GL_OUT_OF_MEMORY ) SLOG::log( LogLevels::ERROR, LogMethodsE::ERROR, "OpenGL error : out of memory" );
		if ( err == GL_STACK_UNDERFLOW ) SLOG::log( LogLevels::ERROR, LogMethodsE::ERROR, "OpenGL error : stack underflow" );
		if ( err == GL_STACK_OVERFLOW ) SLOG::log( LogLevels::ERROR, LogMethodsE::ERROR, "OpenGL error : stack overflow" );
	}
}
