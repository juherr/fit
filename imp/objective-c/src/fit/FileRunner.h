// Copyright (c) 2002 Cunningham & Cunningham, Inc.
// Released under the terms of the GNU General Public License version 2 or later.

#import <Foundation/Foundation.h>
#import "Parse.h"
#import "Fixture.h"

@interface FileRunner : NSObject {
    Fixture *fixture;
}

- (void) run: (const char **)argv;
@end