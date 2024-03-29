#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need GLFW (http://www.glfw.org):
# Linux:
#   apt-get install libglfw-dev
# Mac OS X:
#   brew install glfw
# MSYS2:
#   pacman -S --noconfirm --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw
#

#CXX = g++
#CXX = clang++

EXE = sigil_editor
IMGUI_DIR = ../../lib/imgui
SIGL_DIR = ../../lib/SIGL
SOURCES = main.cpp
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
SOURCES += $(wildcard $(SIGL_DIR)/src/*.cpp)
SOURCES += ./TestStuff/SurfaceTest.cpp
SOURCES += ./TestStuff/SurfaceHandler.cpp
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL
INCLUDES = -I$(IMGUI_DIR)
INCLUDES += -I$(IMGUI_DIR)/backends
INCLUDES += -I$(SIGL_DIR)/include
INCLUDES += -I./TestStuff

CXXFLAGS = $(INCLUDES)
CXXFLAGS += -Ofast
CXXFLAGS += -g
CXXFLAGS += -funroll-loops
CXXFLAGS += -fomit-frame-pointer
CXXFLAGS += -finline
CXXFLAGS += -fopenmp
CXXFLAGS += -fno-exceptions
CXXFLAGS += -march=x86-64
CXXFLAGS += -mtune=generic
CXXFLAGS += -mno-vzeroupper
CXXFLAGS += --param max-inline-insns-single=1000 # lets go crazy with the inlining
LIBS =

##---------------------------------------------------------------------
## OPENGL ES
##---------------------------------------------------------------------

## This assumes a GL ES library available in the system, e.g. libGLESv2.so
# CXXFLAGS += -DIMGUI_IMPL_OPENGL_ES2
# LINUX_GL_LIBS = -lGLESv2

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glew glfw3`

	CXXFLAGS += `pkg-config --cflags glew glfw3`
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LIBS += -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib
	#LIBS += -lglfw3
	LIBS += -lglfw

	CXXFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(OS), Windows_NT)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lglew32 -lglfw3 -lgdi32 -lopengl32 -limm32

	CXXFLAGS += `pkg-config --cflags glew glfw3`
	CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:%.cpp %.hpp 
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $@ $<

%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $@ $<

%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $@ $<

%.o:$(SIGL_DIR)/src/%.cpp $(SIGL_DIR)/include/%.hpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $@ $<

%.o:./TestStuff/%.cpp ./TestStuff/%.hpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -MMD -MP -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS) $(addsuffix .d, $(basename $(notdir $(SOURCES))))
