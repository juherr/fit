//
//  ColumnFixture.h
//  FIT
//
//  Created by Steven Newton on Mon Sep 29 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.

#import <Foundation/Foundation.h>

#import "Fixture.h"
#import "TypeAdapter.h"

@interface ColumnFixture : Fixture {
    BOOL hasExecuted;
    NSMutableArray *columnBindings;
}

- (void) check: (Parse *) cell adapter: (TypeAdapter *) a;
- (void) bind: (Parse *)heads;
- (TypeAdapter *) bindField: (NSString *) name;
- (TypeAdapter *) bindMethod: (NSString *) name;
- (void) reset;
- (void) execute;
@end
