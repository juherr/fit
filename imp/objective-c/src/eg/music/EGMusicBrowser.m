//
//  EGMusicBrowser.m
//  FIT
//
//  Created by Steven Newton on Tue Oct 14 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.

#import "EGMusicBrowser.h"


@implementation EGMusicBrowser
- (NSNumber *) totalSongs {
    return [MusicLibrary totalSongs];
}

- (NSString *) title {
    return [MusicLibrary trackTitle: selectedSong];
}

- (NSString *) artist {
    return [MusicLibrary artist: selectedSong];
}

- (NSString *) album {
    return [MusicLibrary album: selectedSong];
}

- (NSNumber *) year {
    return [NSNumber numberWithInt: 1974];
}

- (NSNumber *) time {
    return [MusicLibrary time: selectedSong];
}

- (NSString *) track {
    return [MusicLibrary trackName: selectedSong];
}

- (void) library: (NSString *) path {
    [MusicLibrary libraryWithPath: path];
}

- (void) select: (NSNumber *) i {
    selectedSong = [i intValue] - 1;
}

- (NSString *) playing {
    return [self title];
}

- (NSString *) status {
    return [EGMusicSimulator status];
}

- (NSString *) message {
    return @"load jamed";
}

- (NSNumber *) selectedSongs {
    return [MusicLibrary selectSongCount];
}

- (NSNumber *) remaining {
    return [NSNumber numberWithFloat: trunc([EGMusicSimulator remaining] / .6) / 100.0];
}

- (void) play {
    double duration = [[self time] floatValue] * 60.0;
    [EGMusicSimulator play: duration];
}

- (void) pause {
    [EGMusicSimulator pause];
}

- (void) ok {
    [EGMusicSimulator clearError];
}

- (void) sameAlbum {
    [MusicLibrary findAlbum: [self album]];
    [EGMusicSimulator search];
}

- (void) sameArtist {
    [MusicLibrary findArtist: [self artist]];
}

- (void) showAll {
    [MusicLibrary clearSelection];
}

@end
