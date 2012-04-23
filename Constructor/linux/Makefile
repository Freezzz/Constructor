CC = gcc
CXX = g++
TARGET = constructor
CCFLAGS = -Wall -Wextra -Wno-unused-parameter -g
CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -g
VISIBILITY = 
COCOS2DX_HOME = ../../..
SOURCE_PATH = ../Classes/

INCLUDES = -I${COCOS2DX_HOME}/cocos2dx/platform/third_party/linux \
	-I${COCOS2DX_HOME}/cocos2dx/include \
	-I${COCOS2DX_HOME}/cocos2dx \
	-I${COCOS2DX_HOME}/Box2D \
	-I${COCOS2DX_HOME}/ \
	-I${COCOS2DX_HOME}/cocos2dx/platform \
	-I${COCOS2DX_HOME}/CocosDenshion/include \
	-I../Classes/ -I../Classes/Actors -I../Classes/Controlls -I../Classes/Players -I../Classes/Weapons

DEFINES = -DLINUX

OBJECTS = $(shell find ${SOURCE_PATH} -iname '*.cpp' | sed -rs 's/\.cpp/.o/g' ) main.o

STATICLIBS = ${COCOS2DX_HOME}/lib/linux/Debug/libbox2d.a
SHAREDLIBS = -L${COCOS2DX_HOME}/lib/linux/Debug -lcocos2d -lGL -lrt -lz -Wl,-rpath,${COCOS2DX_HOME}/lib/linux/Debug/

#$(shell ../../build-linux.sh $<)
####### Build rules
$(TARGET): $(OBJECTS)
	echo $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(OBJECTS) -o $(TARGET) $(SHAREDLIBS) $(STATICLIBS)
	
####### Compile
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@

%.o: %.c
	$(CC) $(CCFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@
		
clean: 
	rm -f $(OBJECTS) $(TARGET) core
