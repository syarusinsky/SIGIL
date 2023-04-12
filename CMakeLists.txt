cmake_minimum_required( VERSION 3.9.0 )

# PROJECT NAME AND BUILD TYPE (RELEASE OR DEBUG)
set( TARGET_NAME sigil_editor )
project( ${TARGET_NAME} )

# C/C++ FLAGS AND STANDARDS
set( CMAKE_CXX_STANDARD 17 )
set( CXX_AND_C_FLAGS
"-Wall \
-Wextra \
-Ofast \
-g \
-funroll-loops \
-fomit-frame-pointer \
-finline \
-fopenmp \
-fno-exceptions \
-march=x86-64 \
-mtune=generic \
-mno-vzeroupper \
--param max-inline-insns-single=1000 \
-MMD \
-MP"
)
set( COMMON_CXX_FLAGS ${CXX_AND_C_FLAGS} )
set( COMMON_C_FLAGS ${CXX_AND_C_FLAGS} )

set( CMAKE_VERBOSE_MAKEFILE true )

# DEFINITIONS
# add_definitions( -DSOME_DEF=1 )


# TODO TEMPORARY TO MAKE SURE THINGS WORK, MOVE THIS TO SUBDIRECTORIES
# IN THE FUTURE
set( SOURCE_FILES
	${CMAKE_CURRENT_LIST_DIR}/host/editor/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/host/editor/TestStuff/SurfaceTest.cpp
	${CMAKE_CURRENT_LIST_DIR}/host/editor/TestStuff/SurfaceHandler.cpp
)

set( HEADER_FILES
	${CMAKE_CURRENT_LIST_DIR}/host/editor/TestStuff/SurfaceTest.hpp
	${CMAKE_CURRENT_LIST_DIR}/host/editor/TestStuff/SurfaceHandler.hpp
)

set( INCLUDE_DIRS
	${CMAKE_CURRENT_LIST_DIR}/host/editor
	${CMAKE_CURRENT_LIST_DIR}/host/editor/TestStuff
)


# PLATFORM DEPENDENT
set( OS_DEPENDENT_COMPILE_OPTIONS "" )
set( OS_DEPENDENT_LINK_OPTIONS "" )
if ( UNIX AND NOT APPLE )
	set( LINUX TRUE )
endif ( UNIX AND NOT APPLE )

if ( WIN32 )
	message( "---WINDOWS DETECTED---" )

	find_package( PkgConfig REQUIRED )
	pkg_check_modules( GLEW REQUIRED glew )
	pkg_check_modules( GLFW3 REQUIRED glfw3 )

	find_package( OpenGL REQUIRED )

	set( INCLUDE_DIRS
		${INCLUDE_DIRS}

		${GLEW_INCLUDE_DIRS}
		${GLFW3_INCLUDE_DIRS}
		${OPENGL_INCLUDE_DIRS}
	)

	set( LIBRARIES_TO_LINK
		${LIBRARIES_TO_LINK}

		${GLEW_LIBRARIES}
		${GLFW3_LIBRARIES}
		${OPENGL_LIBRARIES}
	)

	set( OS_DEPENDENT_LINK_OPTIONS
"\
-lglew32 \
-lglfw3 \
-lgdi32 \
-lopengl32 \
-limm32 \
"
	)
	set( OS_DEPENDENT_COMPILE_OPTIONS
		""
	)
endif ( WIN32 )

if ( LINUX )
	message( "---LINUX DETECTED---" )
	find_package( PkgConfig REQUIRED )
	pkg_check_modules( GLEW REQUIRED glew )
	pkg_check_modules( GLFW3 REQUIRED glfw3 )

	set( INCLUDE_DIRS
		${INCLUDE_DIRS}

		${GLEW_INCLUDE_DIRS}
		${GLFW3_INCLUDE_DIRS}
	)

	set( LIBRARIES_TO_LINK
		${LIBRARIES_TO_LINK}

		${GLEW_LIBRARIES}
		${GLFW3_LIBRARIES}
	)

	set( OS_DEPENDENT_LINK_OPTIONS
		"-IGL"
	)
	set( OS_DEPENDENT_COMPILE_OPTIONS
		""
	)
endif ( LINUX )

if ( APPLE )
	message( "---APPLE DETECTED---" )
	set( OS_DEPENDENT_LINK_OPTIONS
"\
-framework OpenGL \
-framework Cocoa \
-framework IOKit \
-framework CoreVideo \
-L/usr/local/lib \
-L/opt/local/lib \
-L/opt/homebrew/lib \
-lglfw \
"
	)
	set( OS_DEPENDENT_COMPILE_OPTIONS
"\
-I/usr/local/include \
-I/opt/local/include \
-I/opt/homebrew/include \
"
	)
endif ( APPLE )


# SUBDIRECTORIES
add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/lib )


# SOURCE FILES
set( SOURCE_FILES
	${SOURCE_FILES}

	${LIB_SOURCE_FILES}
)

# HEADER FILES
set( HEADER_FILES
	${HEADER_FILES}

	${LIB_HEADER_FILES}
)

# INCLUDE DIRECTORIES
set( INCLUDE_DIRS
	${INCLUDE_DIRS}

	${LIB_INCLUDE_DIRS}
)

# LIBRARIES TO LINK
set( LIBRARIES_TO_LINK
	${LIBRARIES_TO_LINK}

	${LIB_LIBRARIES}
)


# INCLUDING DIRECTORIES
include_directories( ${INCLUDE_DIRS} )


# ADDING LIBRARIES
add_library( ${SIGL_LIB_NAME} STATIC ${SIGL_SOURCE_FILES} )
add_library( ${IMGUI_LIB_NAME} STATIC ${IMGUI_SOURCE_FILES} )


# CREATE AND LINK EXECUTABLE
add_executable( ${TARGET_NAME}
	${SOURCE_FILES}
	${HEADER_FILES}
)

target_link_libraries( ${TARGET_NAME}
	${LIBRARIES_TO_LINK}
)

separate_arguments( COMMON_C_FLAGS NATIVE_COMMAND ${COMMON_C_FLAGS} )
separate_arguments( COMMON_CXX_FLAGS NATIVE_COMMAND ${COMMON_CXX_FLAGS} )
separate_arguments( OS_DEPENDENT_LINK_OPTIONS NATIVE_COMMAND ${OS_DEPENDENT_LINK_OPTIONS} )
separate_arguments( OS_DEPENDENT_COMPILE_OPTIONS NATIVE_COMMAND ${OS_DEPENDENT_COMPILE_OPTIONS} )
target_compile_options( ${TARGET_NAME}
	PUBLIC
	$<$<COMPILE_LANGUAGE:C>:$<1: ${COMMON_C_FLAGS}>>
	$<$<COMPILE_LANGUAGE:CXX>:$<1: ${COMMON_CXX_FLAGS}>>
	${OS_DEPENDENT_COMPILE_OPTIONS}
)

target_link_options( ${TARGET_NAME}
	PUBLIC
	${OS_DEPENDENT_LINK_OPTIONS}
)

set_property( TARGET
	${TARGET_NAME}
	PROPERTY RUNTIME_OUTPUT_DIRECTORY
	${CMAKE_BINARY_DIR}/bin
)


