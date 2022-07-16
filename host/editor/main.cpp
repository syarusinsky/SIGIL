#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <iostream>

void updateTexture()
{
	
}

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
	// Setup window
	glfwSetErrorCallback( glfw_error_callback );
	if ( !glfwInit() )
	{
		return 1;
	}

	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 2 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
	glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_ES_API );
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ); 	// 3.2+ only
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE ); 			// Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
	// glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ); 	// 3.2+ only
	// glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE ); 			// 3.0+ only
#endif

	// Create window with graphics context
	GLFWwindow* window = glfwCreateWindow( 1280, 720, "SIGIL Editor", NULL, NULL );
	if ( window == NULL )
	{
		return 1;
	}
	glfwMakeContextCurrent( window );
	glfwSwapInterval( 1 ); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 	// Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; 	// Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL( window, true );
	ImGui_ImplOpenGL3_Init( glsl_version );

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );

	// MY TEXTURE LOADING CODE ----------------------------------------------------------
	GLuint texture;
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	// TODO need to get this from the Surface class
	constexpr unsigned int data_width = 640;
	constexpr unsigned int data_height = 480;
	uint8_t data[data_width * data_height * 3];
	for ( unsigned int y = 0; y < data_height; y++ )
	{
		for ( unsigned int x = 0; x < data_width; x++ )
		{
			data[(((y * data_width) + x) * 3) + 0] = 255.0f * static_cast<float>( static_cast<float>(y) / data_height );
			data[(((y * data_width) + x) * 3) + 1] = 255.0f * static_cast<float>( static_cast<float>(y) / data_height );
			data[(((y * data_width) + x) * 3) + 2] = 255.0f * static_cast<float>( static_cast<float>(y) / data_height );
		}
	}
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, data_width, data_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
	glBindTexture( GL_TEXTURE_2D, 0 );
	//create test checker image
	// unsigned char texDat[64];
	// for (int i = 0; i < 64; ++i)
	//     texDat[i] = ((i + (i / 8)) % 2) * 128 + 127;

	// //upload to GPU texture
	// GLuint texture;
	// glGenTextures(1, &texture);
	// glBindTexture(GL_TEXTURE_2D, texture);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 8, 8, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, texDat);
	// glBindTexture(GL_TEXTURE_2D, 0);
	// ----------------------------------------------------------------------------------

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite
		// your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application,
		// or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()!
		// You can browse its code to learn more about Dear ImGui!).
		if ( show_demo_window )
		{
			ImGui::ShowDemoWindow(&show_demo_window);
		}

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin( "Hello, world!" ); // Create a window called "Hello, world!" and append into it.

			ImGui::Text( "This is some useful text." ); // Display some text (you can use a format strings too)
			ImGui::Checkbox( "Demo Window", &show_demo_window ); // Edit bools storing our window open/close state
			ImGui::Checkbox( "Another Window", &show_another_window );

			ImGui::SliderFloat( "float", &f, 0.0f, 1.0f ); // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3( "clear color", (float*)&clear_color ); // Edit 3 floats representing a color

			if ( ImGui::Button("Button") ) // Buttons return true when clicked (most widgets return true when edited/activated)
			{
				counter++;
			}
			ImGui::SameLine();
			ImGui::Text( "counter = %d", counter );

			ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate );
			ImGui::End();
		}

		// 3. Show another simple window.
		if ( show_another_window )
		{
			// Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Begin( "Another Window", &show_another_window );
			ImGui::Text( "Hello from another window!" );
			if ( ImGui::Button("Close Me") )
			{
				show_another_window = false;
			}
			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize( window, &display_w, &display_h );
		glViewport( 0, 0, display_w, display_h );
		glClearColor( clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w );
		glClear( GL_COLOR_BUFFER_BIT );
		// MY RENDER CODE ------------------------------------------
		// update texture
		for ( unsigned int byte = 0; byte < data_width * data_height * 3; byte++ )
		{
			data[byte]++;
		}
		glBindTexture( GL_TEXTURE_2D, texture );
		glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			0,
			0,
			data_width,
			data_height,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			data
		);
		glBindTexture( GL_TEXTURE_2D, 0);
		// TODO need to get this from the Surface class
		constexpr float aspectRatio = 640.0f / 480.0f;
		glBindTexture( GL_TEXTURE_2D, texture );
		glEnable( GL_TEXTURE_2D );
		glBegin( GL_QUADS );
		if ( display_w > display_h )
		{
			float width = display_h * aspectRatio;
			float pos_w = width / display_w;
			glTexCoord2i( 0, 1 );
			glVertex2f( -pos_w, -1.0f );
			glTexCoord2i( 1, 1 );
			glVertex2f( pos_w, -1.0f );
			glTexCoord2i( 1, 0 );
			glVertex2f( pos_w, 1.0f );
			glTexCoord2i( 0, 0 );
			glVertex2f( -pos_w, 1.0f );
		}
		else if( display_h > display_w )
		{
			float height = display_w / aspectRatio;
			float pos_h = height / display_h;
			glTexCoord2i( 0, 1 );
			glVertex2f( -1.0f, -pos_h );
			glTexCoord2i( 1, 1 );
			glVertex2f( 1.0f, -pos_h );
			glTexCoord2i( 1, 0 );
			glVertex2f( 1.0f, pos_h );
			glTexCoord2i( 0, 0 );
			glVertex2f( -1.0f, pos_h );
		}
		else // display_w == display_h
		{
			glTexCoord2i( 0, 1 );
			glVertex2f( -1.0f, -1.0f );
			glTexCoord2i( 1, 1 );
			glVertex2f( 1.0f, -1.0f );
			glTexCoord2i( 1, 0 );
			glVertex2f( 1.0f, 1.0f );
			glTexCoord2i( 0, 0 );
			glVertex2f( -1.0f, 1.0f );
		}
		glEnd();
		glDisable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, 0 );
		glFlush();
		// glClear(GL_COLOR_BUFFER_BIT);
		// glBindTexture(GL_TEXTURE_2D, texture);
		// glEnable(GL_TEXTURE_2D);
		// glBegin(GL_QUADS);
		// glTexCoord2i(0, 0); glVertex2f(-1.0f, -1.0f);
		// glTexCoord2i(0, 1); glVertex2f(-1.0f, 1.0f);
		// glTexCoord2i(1, 1); glVertex2f(1.0f, 1.0f);
		// glTexCoord2i(1, 0); glVertex2f(1.0f, -1.0f);
		// glEnd();
		// glDisable(GL_TEXTURE_2D);
		// glBindTexture(GL_TEXTURE_2D, 0);
		// glFlush();
		// ---------------------------------------------------------
		ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

		glfwSwapBuffers( window );
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow( window );
	glfwTerminate();

	return 0;
}
