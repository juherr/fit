//
//  MusicLibrary.m
//  FIT
//
//  Created by Steven Newton on Wed Oct 15 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.

#import "MusicLibrary.h"

@implementation Music
+ (Music *) parse: (NSString *) s {
    Music *song = [[Music alloc] init];
    // create a song by splitting the line on tabs
    NSArray *songElements = [s componentsSeparatedByString: @"\t"];
    //Name	Artist	Album	Genre	Size	Time	Track Number	Track Count	Year	Date
    song->name = [songElements objectAtIndex: 0];
    song->artist = [songElements objectAtIndex: 1];
    song->album = [songElements objectAtIndex: 2];
    song->genre = [songElements objectAtIndex: 3];
    song->size = [[songElements objectAtIndex: 4] intValue];
    song->time = [[songElements objectAtIndex: 5] intValue];
    song->trackNumber = [[songElements objectAtIndex: 6] intValue];
    song->trackCount = [[songElements objectAtIndex: 7] intValue];
    song->year = [[songElements objectAtIndex: 8] intValue];
    song->date = [songElements objectAtIndex: 9];
    return [song autorelease];
}
- (NSString *) description {
    return [NSString stringWithFormat: @"%@ by %@, %@, %d", name, artist, genre, date];
}

- (NSString *) title {
    return name;
}

- (NSString *) artist {
    return artist;
}

- (NSString *) album {
    return album;
}

- (NSString *) track {
    return [NSString stringWithFormat: @"%d of %d", trackNumber, trackCount];
}

- (NSNumber *) year {
    return [NSNumber numberWithInt: year];
}

- (NSNumber *)time {
    return [NSNumber numberWithFloat: round (time/0.6) / 100.0];
}

- (NSString *) genre {
    return genre;
}

- (NSNumber *) size {
    return [NSNumber numberWithInt: size];
}

- (NSString *) date {
    return date;
}

- (void) select: (BOOL) select {
    selected = select;
}

@end

@implementation MusicLibrary
static MusicLibrary* _sharedInstance = nil;

+ (id) libraryWithPath: (NSString *) path {
    if (_sharedInstance != nil) {
        return _sharedInstance;
    }
    _sharedInstance = [[[self class] alloc] init];
    NSString *content = [NSString stringWithContentsOfFile: path];
    if (content != nil) {
        NSArray *lines = [content componentsSeparatedByString: @"\r"];
        int songCount = [lines count];
        NSMutableArray *songlist = [NSMutableArray arrayWithCapacity: songCount];
        int i;
        for (i = 1; i < songCount; i++) {
            [songlist addObject: [Music parse: [lines objectAtIndex: i]]];

        }
        _sharedInstance->library = songlist;
    }
    [_sharedInstance autorelease];
    return _sharedInstance;
}

+ (NSArray *) library {
    return _sharedInstance->library;
}

+ (NSArray *) selected {
    return _sharedInstance->selected;
}

+ (NSNumber *) time: (int) trackId {
    return [[_sharedInstance->library objectAtIndex: trackId] time];
}

+ (NSNumber *) totalSongs {
    return [NSNumber numberWithInt: [_sharedInstance->library count]];
}

+ (NSString *) trackName: (int) trackId {
    return [[_sharedInstance->library objectAtIndex: trackId] track];
}

+ (NSString *) trackTitle: (int) trackId {
    return [[_sharedInstance->library objectAtIndex: trackId] title];
}

+ (NSString *) artist: (int) trackId {
    return [[_sharedInstance->library objectAtIndex: trackId] artist];
}

+ (NSString *) album: (int) trackId {
    return [[_sharedInstance->library objectAtIndex: trackId] album];
}

+ (void) findAlbum: (NSString *) album {
    _sharedInstance->selected = [NSMutableArray array];
    NSEnumerator *e = [_sharedInstance->library objectEnumerator];
    Music *m;
    while (m = [e nextObject]) {
        if ([[m album] isEqualToString: album]) {
            [m select: YES];
            [_sharedInstance->selected addObject: m];
        }
    }
}

+ (void) findArtist: (NSString *) artist {
    _sharedInstance->selected = [NSMutableArray array];
    NSEnumerator *e = [_sharedInstance->library objectEnumerator];
    Music *m;
    while (m = [e nextObject]) {
        if ([[m artist] isEqualToString: artist]) {
            [m select: YES];
            [_sharedInstance->selected addObject: m];
        }
    }
}

+ (void) clearSelection {
    _sharedInstance->selected = (NSMutableArray *)_sharedInstance->library;
}

+ (NSNumber *) selectSongCount {
    return [NSNumber numberWithInt: [_sharedInstance->selected count]];
}
@end
