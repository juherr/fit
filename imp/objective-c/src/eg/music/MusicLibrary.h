//
//  MusicLibrary.h
//  FIT
//
//  Created by Steven Newton on Wed Oct 15 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.

#import <Foundation/Foundation.h>

@interface Music : NSObject {
    NSString *name;
    NSString *artist;
    NSString *album;
    NSString *genre;
    int size;
    int time;
    int trackNumber;
    int trackCount;
    int year;
    NSString *date;
    BOOL selected;
}
+ (Music *) parse: (NSString *) s;
- (NSString *) title;
- (NSString *) artist;
- (NSString *) album;
- (NSString *) track;
- (NSNumber *) year;
- (NSString *) genre;
- (NSNumber *) time;
- (NSNumber *) size;
- (NSString *) date;
- (void) select: (BOOL) select;
@end

@interface MusicLibrary : NSObject {
    NSArray *library;
    NSMutableArray *selected;
}
+ (id) libraryWithPath: (NSString *) path;
+ (NSArray *) library;
+ (NSArray *) selected;
+ (NSNumber *) totalSongs;
+ (NSString *) trackName: (int) trackId;
+ (NSString *) trackTitle: (int) trackId;
+ (NSString *) artist: (int) trackId;
+ (NSString *) album: (int) trackId;
+ (NSNumber *) time: (int) trackId;
+ (void) findAlbum: (NSString *) album;
+ (void) findArtist: (NSString *) artist;
+ (void) clearSelection;
+ (NSNumber *) selectSongCount;
@end
