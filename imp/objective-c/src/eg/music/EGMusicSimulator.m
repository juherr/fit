//
//  EGMusicSimulator.m
//  FIT
//
//  Created by Steven Newton on Mon Nov 24 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.

#import "EGMusicSimulator.h"


@implementation EGMusicSimulator

static int timer;
static int startTime;
static int remaining;
static NSString *status = @"ready";
static NSString *message;

+ (void) play: (int) duration {
    if (status == @"pause") {
        status = @"playing";
    } else {
        status = @"loading";
        remaining = duration;
    }
    startTime = timer;
}

+ (void) pause {
    [self advance: 2];
    status = @"pause";
}

+ (void) finish {
    status = @"ready";
}

+ (void) clearError {
    status = @"ready";
}

+ (int) remaining {
    return remaining;
}

+ (void) search {
    status = @"searching";
}

+ (void) advance: (int) seconds {
    timer = timer + seconds;
    if (status == @"playing") {
        remaining = remaining - seconds;
    }
    if (timer - startTime > 2 && status == @"loading") {
        status = @"playing";
    }
}

+ (void) fail: (NSString *) failMessage {
    message = failMessage;
}

+ (NSString *) message {
    return message;
}

+ (NSString *) status {
    return status;
}

@end
