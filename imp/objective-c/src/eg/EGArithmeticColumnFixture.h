//
//  EGArithmeticColumnFixture.h
//  FIT
//
//  Created by Steven Newton on Mon Sep 29 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.
//

#import <Foundation/Foundation.h>

#import "ColumnFixture.h"

@interface EGArithmeticColumnFixture : ColumnFixture {
    int _x;
    int _y;
}
- (NSNumber *) plus;
- (NSNumber *) times;
- (NSNumber *) divide;
- (NSNumber *) floating;
- (NSNumber *) x;
- (NSNumber *) y;
- (void) x: (NSNumber *) x;
- (void) y: (NSNumber *) y;
@end
