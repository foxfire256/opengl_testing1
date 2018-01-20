cmake_minimum_required(VERSION 3.0.0)
# cmake version 3.0.0 is needed for block comments, 2.8.11 was the previous
# min version
project(ot1)

# http://www.cmake.org/Wiki/CMake_Useful_Variables

set(CMAKE_INCLUDE_CURRENT_DIR ON)


#include_directories("../common-cpp")


# this hasn't been tested for Visual studio as of 2018.01.20
# this assumes you are running cmake from a build directory in the source directory
# cmake .. -G "Visual Studio 14 2015 Win64"
# cmake .. -G "Visual Studio 15 2017 Win64"
if(MSVC)
	ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
	
	SET(CMAKE_C_FLAGS "/MP ${CMAKE_C_FLAGS}")
	SET(CMAKE_CXX_FLAGS "/MP ${CMAKE_CXX_FLAGS}")
	
	find_package(OpenGL REQUIRED)
	#find_library(OPENGL_LIBRARY NAMES OpenGL32 opengl32)
	
	find_package(Qt5Core)
	find_package(Qt5Widgets)
	find_package(Qt5OpenGL)
	
	set(LIBS ${LIBS} kernel32.lib)
	set(SDL2_LIBS
		optimized "C:\\vclibs\\vc2015libs\\lib\\SDL2main.lib"
		debug "C:\\vclibs\\vc2015libs\\lib\\debug\\SDL2main.lib")
	set(SDL2_LIBS ${SDL2_LIBS}
		optimized "C:\\vclibs\\vc2015libs\\lib\\SDL2.lib"
		debug "C:\\vclibs\\vc2015libs\\lib\\debug\\SDL2.lib")
	set(GUI_LIBS ${GUI_LIBS} ${SDL2_LIBS}
		optimized "C:\\vclibs\\vc2015libs\\lib\\SDL2_ttf.lib"
		debug "C:\\vclibs\\vc2015libs\\lib\\debug\\SDL2_ttf.lib")
	set(GUI_LIBS ${GUI_LIBS}
		optimized "C:\\vclibs\\vc2015libs\\lib\\SDL2_image.lib"
		debug "C:\\vclibs\\vc2015libs\\lib\\debug\\SDL2_image.lib")
	set(GUI_LIBS ${GUI_LIBS}
		optimized "C:\\vclibs\\vc2015libs\\lib\\glew32.lib"
		debug "C:\\vclibs\\vc2015libs\\lib\\debug\\glew32d.lib")
	set(GUI_LIBS ${GUI_LIBS} ${OPENGL_LIBRARY})
	
	#set(PNG_LIBRARIES
	#	optimized "C:\\vclibs\\vc2015libs\\lib\\libpng16.lib"
	#	debug "C:\\vclibs\\vc2015libs\\lib\\debug\\libpng16d.lib")
	
	set(BOOST_LIBS ${BOOST_LIBS}
		optimized "C:\\vclibs\\vc2015libs\\lib\\libboost_program_options-vc140-mt-1_63.lib"
		debug "C:\\vclibs\\vc2015libs\\lib\\debug\\libboost_program_options-vc140-mt-gd-1_63.lib")
	set(BOOST_LIBS ${BOOST_LIBS}
		optimized "C:\\vclibs\\vc2015libs\\lib\\libboost_system-vc140-mt-1_63.lib"
		debug "C:\\vclibs\\vc2015libs\\lib\\debug\\libboost_system-vc140-mt-gd-1_63.lib")
	set(BOOST_LIBS ${BOOST_LIBS}
		optimized "C:\\vclibs\\vc2015libs\\lib\\libboost_filesystem-vc140-mt-1_63.lib"
		debug "C:\\vclibs\\vc2015libs\\lib\\debug\\libboost_filesystem-vc140-mt-gd-1_63.lib")
	
	include_directories("C:\\vclibs\\vc2015libs\\include")
endif(MSVC)

if(MINGW OR MSYS)
	set(CMAKE_PREFIX_PATH "c:/msys64/mingw64")
	set(CMAKE_INCLUDE_PATH "c:/msys64/mingw64/include")
	set(CMAKE_LIBRARY_PATH "c:/msys64/mingw64/lib")
	
	find_package(OpenGL REQUIRED)
	find_library(OPENGL_LIBRARY NAMES GL)
	#find_package(Freetype REQUIRED)
	#include_directories(${FREETYPE_INCLUDE_DIRS})
	#find_package(PNG REQUIRED)
	#include_directories(${PNG_INCLUDE_DIR})
	find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
	include_directories(${Boost_INCLUDE_DIRS})
	find_package(GLEW REQUIRED)
	include_directories(${GLEW_INCLUDE_DIRS})

	INCLUDE(FindPkgConfig)

	PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
	include_directories(${SDL2_INCLUDE_DIR})

	set(BOOST_LIBS ${Boost_LIBRARIES})
	set(LIBS ${LIBS} ${Boost_LIBRARIES})
	set(SDL_LIBS
		${SDL2_LIBRARIES}
		${GLEW_LIBRARIES}
		${OPENGL_LIBRARIES}
		)

	find_package(Qt5Core)
	find_package(Qt5Widgets)
	find_package(Qt5OpenGL)
	
	set(QT_LIBS
		Qt5::Core Qt5::Widgets Qt5::OpenGL
		${GLEW_LIBRARIES}
		${OPENGL_LIBRARIES}
		)
	
	include_directories("c:/msys64/mingw64/include/eigen3")
endif(MINGW OR MSYS)

if(APPLE)
	set(CMAKE_PREFIX_PATH "/usr/local")
	set(CMAKE_INCLUDE_PATH "/usr/local/include")
	set(CMAKE_LIBRARY_PATH "/usr/local/lib")
	
	find_package(OpenGL REQUIRED)
	#find_library(OPENGL_LIBRARY NAMES OpenGL)
	#find_package(Freetype REQUIRED)
	#include_directories(${FREETYPE_INCLUDE_DIRS})
	#find_package(PNG REQUIRED)
	#include_directories(${PNG_INCLUDE_DIR})
	find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
	include_directories(${Boost_INCLUDE_DIRS})
	find_package(GLEW REQUIRED)
	include_directories(${GLEW_INCLUDE_DIRS})
	find_package(OpenAL REQUIRED)
	include_directories(${OPENAL_INCLUDE_DIRS})

	INCLUDE(FindPkgConfig)

	PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
	include_directories(${SDL2_INCLUDE_DIR})

	set(BOOST_LIBS ${Boost_LIBRARIES})
	set(LIBS ${LIBS} ${Boost_LIBRARIES})
	set(SDL_LIBS
		${SDL2_LIBRARIES}
		${GLEW_LIBRARIES}
		${OPENGL_LIBRARIES}
		)

	# If there are problems with qt5 on Apple run 'which qmake' and make sure
	# it is the correct one. Usually you have to run 'brew link qt5 --force'.
	# Anaconda causes problems too since it has qt4 and other libs.
	set(CMAKE_PREFIX_PATH "/usr/local/opt/qt")
	set(CMAKE_INCLUDE_PATH "/usr/local/opt/qt/include")
	set(CMAKE_LIBRARY_PATH "/usr/local/opt/qt/lib")
	find_package(Qt5Core)
	find_package(Qt5Widgets)
	find_package(Qt5OpenGL)
	
	set(QT_LIBS
		Qt5::Core Qt5::Widgets Qt5::OpenGL
		${GLEW_LIBRARIES}
		${OPENGL_LIBRARIES}
		)
	
	include_directories("/usr/local/opt/qt/include")
	
	set(CMAKE_PREFIX_PATH "/usr/local")
	set(CMAKE_INCLUDE_PATH "/usr/local/include")
	set(CMAKE_LIBRARY_PATH "/usr/local/lib")
	
	include_directories("/usr/local/include")
	include_directories("/usr/local/include/eigen3")
endif(APPLE)

# this probably only works on openSUSE Tumbleweed
if(UNIX AND NOT APPLE)
	set(CMAKE_PREFIX_PATH "/usr")
	set(CMAKE_INCLUDE_PATH "/usr/include")
	set(CMAKE_LIBRARY_PATH "/usr/lib")

	find_package(OpenGL REQUIRED)
	find_library(OPENGL_LIBRARY NAMES GL)
	#find_package(Freetype REQUIRED)
	#include_directories(${FREETYPE_INCLUDE_DIRS})
	#find_package(PNG REQUIRED)
	#include_directories(${PNG_INCLUDE_DIR})
	find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
	include_directories(${Boost_INCLUDE_DIRS})
	find_package(GLEW REQUIRED)
	include_directories(${GLEW_INCLUDE_DIRS})

	INCLUDE(FindPkgConfig)

	PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
	include_directories(${SDL2_INCLUDE_DIR})

	set(BOOST_LIBS ${Boost_LIBRARIES})
	set(LIBS ${LIBS} ${Boost_LIBRARIES})
	set(SDL_LIBS
		${SDL2_LIBRARIES}
		${GLEW_LIBRARIES}
		${OPENGL_LIBRARIES}
		)

	find_package(Qt5Core)
	find_package(Qt5Widgets)
	find_package(Qt5OpenGL)
	
	set(QT_LIBS
		Qt5::Core Qt5::Widgets Qt5::OpenGL
		${GLEW_LIBRARIES}
		${OPENGL_LIBRARIES}
		)
 
	include_directories("/usr/include")
	include_directories("/usr/include/eigen3")

endif(UNIX AND NOT APPLE)


if(NOT MSVC)
	# --------------------------------------------------------------------------
	# WARNING: all warnings are disabled for GCC
	# --------------------------------------------------------------------------
	
	# better optimizations
	# -O3 still IEEE754 compliant
	# -Ofast not IEEE754 compliant, faster than -O3
	# -ffast-math not needed with -Ofast
	# -ftree-vectorize not sure how well this does
	# -mavx
	# -mavx2
	set(CMAKE_C_FLAGS_RELEASE "-Ofast")
	set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -w")
	
	# force some better debugging
	# this should be gcc
	if(NOT APPLE)
		set(CMAKE_C_FLAGS_DEBUG "-ggdb3 -Og")
		set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -Og -w")
	endif(NOT APPLE)
	# and this should be clang
	if(APPLE)
		set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")
		set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
	endif(APPLE)

	# enable C11/C++14 for gcc and clang
	set(EXTRA_CXX_FLAGS "-std=c++14 -Wno-deprecated-declarations -pipe")
	set(EXTRA_C_FLAGS "-std=c11 -Wno-deprecated-declarations -pipe")
	
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS}")
	
	# pipe keeps compilation in memory
	set(CMAKE_LD_FLAGS "-pipe")
endif(NOT MSVC)

set(MAIN_SOURCE_QT
	main_qt.cpp
)
set(MAIN_SOURCE_SDL
	main_sdl.cpp
)
set(SOURCE_SDL
	sdl/sdl_window.hpp
	sdl/sdl_window.cpp
	)
set(SOURCE_QT
	qt/qt_window.hpp
	qt/qt_window.cpp
	qt/opengl_window.hpp
	qt/opengl_window.cpp
	)
set(SOURCE_GFX
	gfx/gfx.hpp
	gfx/gfx.cpp
	gfx/gfx_factory.hpp
	gfx/gfx_factory.cpp
	gfx/gfx_opengl.hpp
	gfx/gfx_opengl.cpp
	)

# static libraries don't work so well on Apple for some reason
if(APPLE)
	set(LIBRARY_TYPE SHARED)
else(APPLE)
	set(LIBRARY_TYPE STATIC)
endif(APPLE)

add_executable(${PROJECT_NAME}_sdl ${MAIN_SOURCE_SDL} ${SOURCE_SDL} ${SOURCE_GFX})
target_link_libraries(${PROJECT_NAME}_sdl ${LIBS} ${SDL_LIBS})
# this is needed to make headers work
#target_include_directories(${PROJECT_NAME}_sdl PUBLIC sdl)

# Instruct CMake to run Qt moc automatically when needed.
set(CMAKE_AUTOMOC ON)
add_executable(${PROJECT_NAME}_qt ${MAIN_SOURCE_QT} ${SOURCE_QT} ${SOURCE_GFX})
target_link_libraries(${PROJECT_NAME}_qt ${LIBS} ${QT_LIBS})
#target_include_directories(${PROJECT_NAME}_qt PUBLIC qt)
set(CMAKE_AUTOMOC OFF)


MESSAGE( STATUS "MINGW: " ${MINGW} )
MESSAGE( STATUS "MSYS: " ${MSYS} )
MESSAGE( STATUS "MSVC: " ${MSVC} )
MESSAGE( STATUS "APPLE: " ${APPLE} )
MESSAGE( STATUS "INCLUDE_DIRECTORIES: " ${INCLUDE_DIRECTORIES} )
MESSAGE( STATUS "CMAKE_C_FLAGS: " ${CMAKE_C_FLAGS} )
MESSAGE( STATUS "CMAKE_C_FLAGS_DEBUG: " ${CMAKE_C_FLAGS_DEBUG} )
MESSAGE( STATUS "CMAKE_C_FLAGS_RELEASE: " ${CMAKE_C_FLAGS_RELEASE} )
MESSAGE( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )
MESSAGE( STATUS "CMAKE_CXX_FLAGS_DEBUG: " ${CMAKE_CXX_FLAGS_DEBUG} )
MESSAGE( STATUS "CMAKE_CXX_FLAGS_RELEASE: " ${CMAKE_CXX_FLAGS_RELEASE} )
get_target_property(QtCore_location Qt5::Core LOCATION)
MESSAGE( STATUS "QtCore_location: " ${QtCore_location})
get_target_property(QtWdigets_location Qt5::Widgets LOCATION)
MESSAGE( STATUS "QtWdigets_location: " ${QtWdigets_location})
