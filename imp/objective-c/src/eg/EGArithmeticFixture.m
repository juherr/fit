//
//  ArithmeticFixture.m
//  FIT
//
//  Created by Steven Newton on Thu Sep 25 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.
//

#import "EGArithmeticFixture.h"


static int divide(int a, int b) {
    if (b == 0) {
        [[NSException exceptionWithName: @"NSInvalidArgumentException" reason: @"division by zero" userInfo: nil] raise];
    }
    return a/b;
}

@implementation EGArithmeticFixture {
}

- (void) doRows: (Parse *) rows {
    [super doRows: [rows more]];
}

- (void) doCell: (Parse *) cell columnNumber: (int) columnNumber {
    switch (columnNumber) {
        case 0: x = [[cell text] intValue] ; break;
        case 1: y = [[cell text] intValue]; break;
        case 2: [self check: cell value: x+y]; break;
        case 3: [self check: cell value: x-y]; break;
        case 4: [self check: cell value: x*y]; break;
        case 5: [self check: cell value: divide(x,y)]; break;
        default: [self ignore: cell]; break;
    }
}

@end
