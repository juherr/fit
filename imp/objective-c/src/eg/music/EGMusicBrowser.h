//
//  EGMusicBrowser.h
//  FIT
//
//  Created by Steven Newton on Tue Oct 14 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.
//

#import <Foundation/Foundation.h>
#import "Fixture.h"

#import "MusicLibrary.h"
#import "EGMusicSimulator.h"

@interface EGMusicBrowser : Fixture {
    int selectedSong;
    NSString *status;
}
- (NSNumber *) totalSongs;
- (NSString *) title;
- (NSString *) artist;
- (NSString *) album;
- (NSNumber *) year;
- (NSNumber *) time;
- (NSString *) track;
- (void) library: (NSString *) path;
- (void) select: (NSNumber *) i;
- (NSString *) playing;
- (NSString *) status;
- (NSString *) message;
- (NSNumber *) selectedSongs;
- (NSNumber *) remaining;
- (void) play;
- (void) pause;
- (void) ok;
- (void) sameAlbum;
- (void) sameArtist;
- (void) showAll;
@end
