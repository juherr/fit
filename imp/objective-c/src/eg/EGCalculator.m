//
//  EGCalculator.m
//  FIT
//
//  Created by Steven Newton on Wed Oct 01 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.
//

#import "EGCalculator.h"
#import "TypeAdapter.h"


BOOL isNumber(id val) {
    return [val isKindOfClass: [NSNumber class]];
}

@implementation EGCalculator
- (NSNumber *) points {
    return [NSNumber numberWithBool: NO];
}
- (NSNumber *) watts {
    return [NSNumber numberWithFloat: .500];
}
- (void) volts: (NSNumber *) volts {
}

- (void) key: (id) key {
    if (isNumber(key)) {
        t = z;
        z = y;
        y = x;
        x = [key floatValue];
    } else {
        if ([key isEqualToString: @"enter"]) {
            t = z;
            z = y;
            y = x;
        } else if ([key isEqualToString: @"/"]) {
            if (x == 0) {
                flash = YES;
            }
            x = y / x;
        } else if ([key isEqualToString: @"+"]) {
            x = y + x;
        } else if ([key isEqualToString: @"*"]) {
            x = y * x;
        } else if ([key isEqualToString: @"sin"]) {
            x = asin(x);
            t = z;
        } else if ([key isEqualToString: @"x^y"]) {
            x = pow(x, y);
        } else if ([key isEqualToString: @"clx"]) {
            flash = NO;
            x = 0;
        } else if ([key isEqualToString: @"clr"]) {
            x = y = z = t = 0;
        } else if ([key isEqualToString: @"chs"]) {
            x = -x;
        }
    }
}

- (NSNumber *) x {
    return [NSNumber numberWithFloat: x];
}

- (NSNumber *) y {
    return [NSNumber numberWithFloat: y];
}

- (NSNumber *) z {
    return [NSNumber numberWithFloat: z];
}

- (NSNumber *) t {
    return [NSNumber numberWithFloat: t];
}

- (NSNumber *) flash {
    return [NSNumber numberWithBool: flash];
}

@end
