//
//  EGMusicRealtime.h
//  FIT
//
//  Created by Steven Newton on Mon Oct 20 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.

#import <Foundation/Foundation.h>
#import "FITActionFixture.h"
#import "EGMusicSimulator.h"

@interface EGMusicRealtime : FITActionFixture {
}

- (void) pause;
- (void) await;
- (void) fail;
@end
