//
//  EGArithmeticColumnFixture.m
//  FIT
//
//  Created by Steven Newton on Mon Sep 29 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.

#import "EGArithmeticColumnFixture.h"


@implementation EGArithmeticColumnFixture
- (NSNumber *) plus {
    return [NSNumber numberWithInt: (_x + _y)];
}

- (NSNumber *) times {
    return [NSNumber numberWithInt: _x * _y];
}

- (NSNumber *) divide {
    if (_y == 0) {
        [[NSException exceptionWithName: @"NSInvalidArgumentException" reason: @"division by zero" userInfo: nil] raise];
    }
    return [NSNumber numberWithInt: _x / _y];
}

- (NSNumber *) floating {
    return [NSNumber numberWithFloat: (float)_x / (float)_y];
}

- (void) x: (NSNumber *) x {
    _x = [x intValue];
}

- (NSNumber *) x {
    return [NSNumber numberWithInt: _x];
}

- (void) y: (NSNumber *) y {
    _y = [y intValue];
}

- (NSNumber *) y {
    return [NSNumber numberWithInt: _y];
}

- (NSObject *) parse: (NSString *) s {
    // check for floats...
    NSRange decimal = [s rangeOfString: @"."];
    if (decimal.location != NSNotFound) {
        return [NSNumber numberWithFloat: [s floatValue]];
    } else {
        return [NSNumber numberWithInt: [s intValue]];
    }
}
@end
