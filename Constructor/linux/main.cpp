#include "main.h"

#include "AppDelegate.h"

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	
	// create the application instance
	AppDelegate app;

	return cocos2d::CCApplication::sharedApplication().run();
}
