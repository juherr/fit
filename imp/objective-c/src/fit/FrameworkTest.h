//
//  FrameworkTest.h
//  FIT
//
//  Created by Steven Newton on Thu Sep 18 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.
//

#import <Foundation/Foundation.h>
#import <ObjcUnit/ObjcUnit.h>

#import "Parse.h"
#import "TypeAdapter.h"

@interface FrameworkTest : TestCase {

}

- (void) run: (NSString *) file right: (int) right wrong: (int) wrong ignores: (int) ignores exceptions: (int) exceptions;
- (NSString *) directory: (NSString *) directory file: (NSString *) file suffix: (NSString *) suffix;
@end
