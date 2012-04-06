//
//  ConstructorAppController.h
//  Constructor
//
//  Created by Ivan Litsvinenka on 4/6/12.
//  Copyright The Epic Bean 2012. All rights reserved.
//

@class RootViewController;

@interface AppController : NSObject <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate> {
    UIWindow *window;
    RootViewController	*viewController;
}

@end

