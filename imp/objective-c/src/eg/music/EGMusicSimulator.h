//
//  EGMusicSimulator.h
//  FIT
//
//  Created by Steven Newton on Mon Nov 24 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.

#import <Foundation/Foundation.h>


@interface EGMusicSimulator : NSObject {
}

+ (void) play: (int) duration;
+ (void) pause;
+ (void) finish;
+ (void) search;
+ (int) remaining;
+ (void) advance: (int) seconds;
+ (void) fail: (NSString *) message;
+ (void) clearError;
+ (NSString *) status;
@end
