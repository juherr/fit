//
//  EGCalculator.h
//  FIT
//
//  Created by Steven Newton on Wed Oct 01 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.
//

#import <Foundation/Foundation.h>

#import "ColumnFixture.h"

@interface EGCalculator : ColumnFixture {
@private
    BOOL flash;
    float x;
    float y;
    float z;
    float t;
}
- (NSNumber *) points;
- (NSNumber *) watts;
- (void) volts: (NSNumber *) volts;
- (void) key: (id) key;
- (NSNumber *) x;
- (NSNumber *) y;
- (NSNumber *) z;
- (NSNumber *) t;
- (NSNumber *) flash;
@end
