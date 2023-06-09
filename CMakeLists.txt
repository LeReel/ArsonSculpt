# CMake entry point
cmake_minimum_required (VERSION 3.26)
project (ArsonS_GL)

find_package(OpenGL REQUIRED)


if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

# Compile external dependencies 
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration/)

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)

include_directories(
	external/glfw-3.3.8/include/
	external/glm-0.9.9.8/
	external/glew-1.13.0/include/
	external/assimp-3.0.1270/include/
	
	src/Scripts/Application/Public/
	
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
	assimp
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

file(GLOB_RECURSE APP_SRC CONFIGURE_DEPENDS "src/Scripts/Application/*.cpp" "src/Scripts/Application/*.h")
file(GLOB_RECURSE IMGUI_SRC CONFIGURE_DEPENDS "src/Scripts/ImGUI/*.cpp" "src/Scripts/ImGUI/*.h")

add_executable(ArsonS_GL
	src/Scripts/main.cpp
	src/Scripts/stb_image.h
	src/Scripts/filesystem.h
	
	${APP_SRC}
	${IMGUI_SRC}

	src/SimpleVertexShader.glsl
	src/SimpleFragmentShader.glsl
)

target_link_libraries(ArsonS_GL
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(ArsonS_GL PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ArsonS_GL/")
create_target_launcher(ArsonS_GL WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/ArsonS_GL/")

SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$")


if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

add_custom_command(
   TARGET ArsonS_GL POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/ArsonS_GL${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/ArsonS_GL/"
)

elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )