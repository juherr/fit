//
//  EGMusicDisplay.h
//  FIT
//
//  Created by Steven Newton on Wed Oct 15 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.

#import <Foundation/Foundation.h>
#import "RowFixture.h"
#import "MusicLibrary.h"

@interface EGMusicDisplay : RowFixture {

}
- (NSArray *) query;
@end
