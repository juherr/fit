//
//  AllTests.h
//  FIT
//
//  Created by Steven Newton on Thu Sep 18 2003.
// Copyright (c) 2002 Cunningham & Cunningham, Inc.
// Released under the terms of the GNU General Public License version 2 or later.
//

#import <Foundation/Foundation.h>
#import <ObjcUnit/ObjcUnit.h>

#import "FrameworkTest.h"

@interface AllTests : NSObject
+ (TestSuite *)suite;
@end
