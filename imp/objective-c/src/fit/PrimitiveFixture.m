//
//  PrimitiveFixture.m
//  FIT
//
//  Created by Steven Newton on Thu Sep 25 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.
//

#import "PrimitiveFixture.h"

int intValue(Parse *cell) {
    NSString *text = [cell text];
    if ([text rangeOfString: @"."].location != NSNotFound) {
        [[NSException exceptionWithName: @"NSInvalidArgumentException" reason: @"Not an integer" userInfo: nil] raise];
    }
    return [text intValue];
}

@implementation PrimitiveFixture
- (void) check: (Parse *) cell value: (int) value {
    if (intValue(cell) == value) {
        [super right: cell];
    } else {
        [super wrong: cell value: [[NSNumber numberWithInt: value] stringValue]];
    }
}

@end
