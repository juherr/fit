//
//  PrimitiveFixture.h
//  FIT
//
//  Created by Steven Newton on Thu Sep 25 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.
//

#import <Foundation/Foundation.h>

#import "Fixture.h"

@interface PrimitiveFixture : Fixture {

}
- (void) check: (Parse *) cell value: (int) value;
@end
