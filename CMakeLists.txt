cmake_minimum_required( VERSION 2.6 )

set( CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter" )
set( CMAKE_CXX_FLAGS_DEBUG "-g -pg -DDEBUG" )
set( CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" )
set( CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -fno-inline -g -pg -DNDEBUG" )
set( CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG" )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake )

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE Debug CACHE STRING "Chose build type: None Debug Release RelWithDebInfo MinSizeRel" FORCE )
endif( NOT CMAKE_BUILD_TYPE )


option( BUILD_CONSTRUCTOR "Build constructor" ON )
option( BUILD_LEVELEDITOR "Build level editor" ON )

if( BUILD_CONSTRUCTOR )
	add_subdirectory( Constructor )
endif()
if( BUILD_LEVELEDITOR )
	add_subdirectory( leveleditor )
endif()

