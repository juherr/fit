//
//  ActionFixture.m
//  FIT
//
//  Created by Steven Newton on Tue Oct 14 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.
//

#import "FITActionFixture.h"
#import "Parse.h"
#import "TypeAdapter.h"

static Fixture *actor;

NSString *camel(NSString *s) {
    NSArray *tokens = [s componentsSeparatedByString: @" "];
    int i;
    NSString *result = [tokens objectAtIndex: 0];
    
    for (i = 1; i < [tokens count]; i++) {
        result = [result stringByAppendingString: [[tokens objectAtIndex: i] capitalizedString]];
    }
    return result;
}

@implementation FITActionFixture

- (void) doCells: (Parse *)cells {
    _cells = cells;
    SEL selector = NSSelectorFromString([_cells text]);
    [self performSelector: selector];
}

- (void) start {
    NSString *className = [self camelCase: [[_cells more] text]];
    actor = [[NSClassFromString(className) alloc] init];
}

- (oneway void) release {
    [actor release];
    [super release];
}

- (void) enter {
    NSString *method = [camel([[_cells more] text]) stringByAppendingString: @":"];
    NSString *arg = [[[_cells more] more] text];
    SEL selector = NSSelectorFromString(method);
    [actor performSelector: selector withObject: arg];
}

- (void) press {
    NSString *method = camel([[_cells more] text]);
    SEL selector = NSSelectorFromString(method);
    [actor performSelector: selector];
}

- (void) check {
    NSString *method = [[_cells more] text];
    Parse *p = [[_cells more] more];
    [super check:  p adapter: [TypeAdapter on: actor method: camel(method)]];
}

@end
