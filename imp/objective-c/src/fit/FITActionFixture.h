//
//  ActionFixture.h
//  FIT
//
//  Created by Steven Newton on Tue Oct 14 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.
//

#import <Foundation/Foundation.h>
#import "Fixture.h"

@interface FITActionFixture : Fixture {
    Parse *_cells;
}
- (void) start;
- (void) enter;
@end
