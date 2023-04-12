#ifndef SURFACEHANDLER_HPP
#define SURFACEHANDLER_HPP

/**************************************************************************
 * A utility class to handle the differences between rendering apis.
 * Currently it renders to an OpenGL framebuffer, even when using software
 * rendering, but I may want to change this later.
**************************************************************************/

#include "SurfaceTest.hpp"

void printGLError();

template <RENDER_API api, unsigned int width, unsigned int height, CP_FORMAT format, unsigned int numThreads, bool include3D,
	unsigned int shaderPassDataSize>
class SurfaceHandler
{
	public:
		SurfaceHandler() :
			m_Surface(),
			m_FBTex( 0 ),
			m_FBTexLocation( 0 ),
			m_FBProgram( 0 )
		{
			initStuff();
			m_Surface.setOnlyFont();
			m_Surface.setMeshVShader( vShader );
			m_Surface.setMeshFShader( fShader );
			m_Surface.loadMesh1( "../../host/editor/TestStuff/eric.obj" );
			m_Surface.loadMesh2( "../../host/editor/TestStuff/cube.obj" );

			while( m_Surface.render() ) {}

			if constexpr ( api == RENDER_API::SOFTWARE )
			{
				m_FBTex = createAndLoadTexture( m_Surface.advanceFrameBuffer().getPixels().data(), width, height );
			}
			else if constexpr ( api == RENDER_API::OPENGL )
			{
				m_FBTex = m_Surface.advanceFrameBuffer().getTexture();
			}

			const char* vertexShaderCStr =
				"#version 120\n"
				"varying vec2 TexCoord;\n"
				"\n"
				"void main()\n"
				"{\n"
				"   gl_Position = gl_Vertex;\n"
				"   TexCoord = gl_MultiTexCoord0.xy;\n"
				"}";
			const char* fragmentShaderCStr =
				"#version 330\n"
				"layout (location = 0) out vec4 FragColor;\n"
				"\n"
				"varying vec2 TexCoord;\n"
				"uniform sampler2D FBTex;\n"
				"\n"
				"void main()\n"
				"{\n"
				"   FragColor = texture(FBTex, TexCoord);\n"
				"}";
			GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
			GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
			glShaderSource( vertexShader, 1, &vertexShaderCStr, NULL );
			glShaderSource( fragmentShader, 1, &fragmentShaderCStr, NULL );
			glCompileShader( vertexShader );
			int success;
			glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );
			if( ! success )
			{
				char infoLog[512];
				glGetShaderInfoLog( vertexShader, 512, NULL, infoLog );
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			};
			glCompileShader( fragmentShader );
			glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success );
			if( ! success )
			{
				char infoLog[512];
				glGetShaderInfoLog( fragmentShader, 512, NULL, infoLog );
				std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			};
			m_FBProgram = glCreateProgram();
			glAttachShader( m_FBProgram, vertexShader );
			glAttachShader( m_FBProgram, fragmentShader );
			glLinkProgram( m_FBProgram );
			glGetProgramiv( m_FBProgram, GL_LINK_STATUS, &success );
			if( ! success )
			{
				char infoLog[512];
				glGetProgramInfoLog( m_FBProgram, 512, NULL, infoLog );
				std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			}
			glDeleteShader( vertexShader );
			glDeleteShader( fragmentShader );

			m_FBTexLocation = glGetUniformLocation( m_FBProgram, "FBTex" );
		}

		void render (int display_w, int display_h)
		{
			while ( m_Surface.render() ) {}

			if constexpr ( api == RENDER_API::SOFTWARE )
			{
				glBindTexture( GL_TEXTURE_2D, m_FBTex );
				glTexSubImage2D(
					GL_TEXTURE_2D,
					0,
					0,
					0,
					width,
					height,
					GL_RGB,
					GL_UNSIGNED_BYTE,
					m_Surface.advanceFrameBuffer().getPixels().data()
				);
			}
			else if constexpr ( api == RENDER_API::OPENGL )
			{
				// since m_Surface.render may change the viewport as it draws to the texture
				glViewport( 0, 0, display_w, display_h );

				m_FBTex = m_Surface.advanceFrameBuffer().getTexture();
				glBindTexture( GL_TEXTURE_2D, m_FBTex );
			}

			glUseProgram( m_FBProgram );
			glUniform1i( m_FBTexLocation, 0 );

			constexpr float aspectRatio = static_cast<float>( width ) / static_cast<float>( height );
			glActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D, m_FBTex );
			glEnable( GL_TEXTURE_2D );
			glBegin( GL_QUADS );
			if ( display_w > display_h )
			{
				float w = display_h * aspectRatio;
				float pos_w = w / display_w;
				if constexpr ( api == RENDER_API::OPENGL )
				{
					glTexCoord2i( 0, 0 );
				}
				else if constexpr ( api == RENDER_API::SOFTWARE )
				{
					glTexCoord2i( 0, 1 );
				}
				glVertex2f( -pos_w, -1.0f );
				if constexpr ( api == RENDER_API::OPENGL )
				{
					glTexCoord2i( 1, 0 );
				}
				else if constexpr ( api == RENDER_API::SOFTWARE )
				{
					glTexCoord2i( 1, 1 );
				}
				glVertex2f( pos_w, -1.0f );
				if constexpr ( api == RENDER_API::OPENGL )
				{
					glTexCoord2i( 1, 1 );
				}
				else if constexpr ( api == RENDER_API::SOFTWARE )
				{
					glTexCoord2i( 1, 0 );
				}
				glVertex2f( pos_w, 1.0f );
				if constexpr ( api == RENDER_API::OPENGL )
				{
					glTexCoord2i( 0, 1 );
				}
				else if constexpr ( api == RENDER_API::SOFTWARE )
				{
					glTexCoord2i( 0, 0 );
				}
				glVertex2f( -pos_w, 1.0f );
			}
			else if( display_h > display_w )
			{
				float h = display_w / aspectRatio;
				float pos_h = h / display_h;
				if constexpr ( api == RENDER_API::OPENGL )
				{
					glTexCoord2i( 0, 0 );
				}
				else if constexpr ( api == RENDER_API::SOFTWARE )
				{
					glTexCoord2i( 0, 1 );
				}
				glVertex2f( -1.0f, -pos_h );
				if constexpr ( api == RENDER_API::OPENGL )
				{
					glTexCoord2i( 1, 0 );
				}
				else if constexpr ( api == RENDER_API::SOFTWARE )
				{
					glTexCoord2i( 1, 1 );
				}
				glVertex2f( 1.0f, -pos_h );
				if constexpr ( api == RENDER_API::OPENGL )
				{
					glTexCoord2i( 1, 1 );
				}
				else if constexpr ( api == RENDER_API::SOFTWARE )
				{
					glTexCoord2i( 1, 0 );
				}
				glVertex2f( 1.0f, pos_h );
				if constexpr ( api == RENDER_API::OPENGL )
				{
					glTexCoord2i( 0, 1 );
				}
				else if constexpr ( api == RENDER_API::SOFTWARE )
				{
					glTexCoord2i( 0, 0 );
				}
				glVertex2f( -1.0f, pos_h );
			}
			else // display_w == display_h
			{
				if constexpr ( api == RENDER_API::OPENGL )
				{
					glTexCoord2i( 0, 0 );
				}
				else if constexpr ( api == RENDER_API::SOFTWARE )
				{
					glTexCoord2i( 0, 1 );
				}
				glVertex2f( -1.0f, -1.0f );
				if constexpr ( api == RENDER_API::OPENGL )
				{
					glTexCoord2i( 1, 0 );
				}
				else if constexpr ( api == RENDER_API::SOFTWARE )
				{
					glTexCoord2i( 1, 1 );
				}
				glVertex2f( 1.0f, -1.0f );
				if constexpr ( api == RENDER_API::OPENGL )
				{
					glTexCoord2i( 1, 1 );
				}
				else if constexpr ( api == RENDER_API::SOFTWARE )
				{
					glTexCoord2i( 1, 0 );
				}
				glVertex2f( 1.0f, 1.0f );
				if constexpr ( api == RENDER_API::OPENGL )
				{
					glTexCoord2i( 0, 1 );
				}
				else if constexpr ( api == RENDER_API::SOFTWARE )
				{
					glTexCoord2i( 0, 0 );
				}
				glVertex2f( -1.0f, 1.0f );
			}
			glEnd();
			glDisable( GL_TEXTURE_2D );
			glActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D, 0 );
			glUseProgram( 0 );
			glFlush();
		}

		static void vShader (TriShaderData<CP_FORMAT::RGBA_32BIT, shaderPassDataSize>& vShaderData)
		{
		}

		static void fShader (Color& colorOut, TriShaderData<CP_FORMAT::RGBA_32BIT, shaderPassDataSize>& fShaderData, float v1Cur, float v2Cur,
				float v3Cur, float texCoordX, float texCoordY, float lightAmnt)
		{
			colorOut = fShaderData.textures[0]->getColor( texCoordX, texCoordY ) * lightAmnt;
		}


	private:
		SurfaceTest<api, width, height, format, numThreads, include3D, shaderPassDataSize> m_Surface;
		GLuint m_FBTex;
		GLint m_FBTexLocation;
		GLuint m_FBProgram;

	GLuint createAndLoadTexture (uint8_t* data, unsigned int data_width, unsigned int data_height)
	{
		GLuint texture;
		glGenTextures( 1, &texture );
		glBindTexture( GL_TEXTURE_2D, texture );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, data_width, data_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
		glBindTexture( GL_TEXTURE_2D, 0 );

		return texture;
	}
};

#endif // SURFACEHANDLER_HPP
