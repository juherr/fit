//
//  Parse.h
//  FIT
//
//  Created by Steven Newton on Thu Sep 18 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.
//

#import <Foundation/Foundation.h>


@interface Parse : NSObject {
    NSString *_text;
    NSString *_body;
    NSString *_leader;
    NSString *_end;
    NSString *_tag;
    NSString *_trailer;
    NSArray *_tags;
    Parse *_parts;
    Parse *_more;
}

+ (id) initWithString: (NSString *) text tags: (NSArray *) tags level: (int) level;
+ (id) initWithString: (NSString *) text tags: (NSArray *) tags;
+ (id) parseWithBody: (NSString *) text tag: (NSString *)tag parts: (Parse *) parts more: (Parse *) more;
+ (id) withString: (NSString *) text;
- (void) text: (NSString *) text;
- (void) tag: (NSString *) tag;
- (void) tags: (NSArray *) tags;
- (void) body: (NSString *) body;
- (void) leader: (NSString *) leader;
- (void) parts: (Parse *) parts;
- (Parse *) parts;
- (void) more: (Parse *) more;
- (Parse *) more;
- (NSString *)leader;
- (NSString *)tag;
- (NSString *)body;
- (NSString *)trailer;
- (void)trailer: (NSString *) s;
- (NSString *)text;
- (Parse *)at: (int)i j: (int) j k: (int)k;
- (Parse *) at: (int) i;
- (int) size;
- (Parse *) leaf;
- (Parse *) last;
+ (NSString *) unescape: (NSString *) s;
+ (NSString *) unformat: (NSString *) s;
- (void) print: (NSFileHandle *) fh;
- (void) end: (NSString *)s;
- (NSString *) end;
- (void) addToTag: (NSString *) text;
- (void) addToBody: (NSString *) text;
- (void) write: (NSFileHandle *) fh string: (NSString *)string;
@end
