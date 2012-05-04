cmake_minimum_required( VERSION 2.6 )
project( leveleditor )

find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
find_package(Box2D REQUIRED)

include( UseQt4 )
set( CMAKE_AUTOMOC TRUE )

file( GLOB_RECURSE SRCS *.cpp )
file( GLOB_RECURSE HDRS *.h )
file( GLOB UIS "*.ui" )
file( GLOB RCCS "*.qrc" )
qt4_wrap_ui( UI_SRC ${UIS})
QT4_ADD_RESOURCES( RCC_SRC ${RCCS} )
set( CODE_FILES ${SRCS} ${HDRS} ${UIS} ${UI_SRC} ${RCCS} ${RCC_SRC} )

include_directories( ${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_SOURCE_DIR} ${BOX2D_INCLUDE_DIR} )
add_executable( leveleditor ${CODE_FILES} )
target_link_libraries( leveleditor ${QT_LIBRARIES} ${BOX2D_LIBRARIES} )

