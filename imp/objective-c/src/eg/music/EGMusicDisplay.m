//
//  EGMusicDisplay.m
//  FIT
//
//  Created by Steven Newton on Wed Oct 15 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.

#import "EGMusicDisplay.h"

@implementation EGMusicDisplay
- (NSArray *) query {
    return [MusicLibrary selected];
}
@end
