//
//  AllTests.m
//  FIT
//
//  Created by Steven Newton on Thu Sep 18 2003.
// Copyright (c) 2002 Cunningham & Cunningham, Inc.
// Released under the terms of the GNU General Public License version 2 or later.
//

#import "AllTests.h"


@implementation AllTests

+ (TestSuite *)suite {
    TestSuite *suite = [TestSuite suiteWithName: @"FIT Tests"];

    [suite addTest: [TestSuite suiteWithClass: [FrameworkTest class]]];
    
    return suite;
}
@end
