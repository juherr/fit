//
//  EGMusicRealtime.m
//  FIT
//
//  Created by Steven Newton on Mon Oct 20 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.

#import "EGMusicRealtime.h"


@implementation EGMusicRealtime


- (void) pause {
    // just sit and listen
    // advance the simulator the # of seconds specified in the table
    // get the # of seconds from the cell in the 2nd column of this row
    NSString *arg = [[_cells more] text];
    int seconds = [arg intValue];
    [EGMusicSimulator advance: seconds];
}

- (void) await {
    [EGMusicSimulator finish];
}

- (void) fail {
    NSString *message = [[_cells more] text];
    [EGMusicSimulator fail: message];
}
@end
