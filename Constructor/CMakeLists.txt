
cmake_minimum_required( VERSION 2.6 )
project( constructor )

set( PROJECT_DIR "${CMAKE_SOURCE_DIR}" )
set( SOURCE_DIR ${PROJECT_DIR}/Classes )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_DIR}/cmake )

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE Debug CACHE STRING "Chose build type: None Debug Release RelWithDebInfo MinSizeRel" FORCE )
endif( NOT CMAKE_BUILD_TYPE )

find_package( Cocos2dx REQUIRED )
find_package( OpenGL REQUIRED )

IF( "${CMAKE_SYSTEM}" MATCHES "Linux" )
	file( GLOB_RECURSE SRCS ${SOURCE_DIR}/*.cpp ${PROJECT_DIR}/linux/*.cpp )
	file( GLOB_RECURSE HDRS ${SOURCE_DIR}/*.h ${PROJECT_DIR}/linux/*.h )
ELSE()
	file( GLOB_RECURSE SRCS "${SOURCE_DIR}/*.cpp" )
	file( GLOB_RECURSE HDRS "${SOURCE_DIR}/*.h" )
ENDIF()
set( CODE_FILES ${SRCS} ${HDRS} )

include_directories( ${COCOS2DX_INCLUDE_DIRS} ${SOURCE_DIR} )
link_directories( ${COCOS2DX_LIB_DIR} )
add_executable( constructor ${CODE_FILES} )
target_link_libraries( constructor ${COCOS2DX_LIBRARIES} ${OPENGL_LIBRARIES} )

