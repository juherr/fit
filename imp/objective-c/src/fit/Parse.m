//
//  Parse.m
//  FIT
//
//  Created by Steven Newton on Thu Sep 18 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.
//

#import "Parse.h"


@implementation Parse

+ (id) parseWithBody: (NSString *) text tag: (NSString *)tag parts: (Parse *) parts more: (Parse *) more  {
    id newInstance = [[[self class] alloc] init];

    [newInstance leader: @"\n"];
    [newInstance end: [NSString stringWithFormat: @"</%@>", tag]];
    [newInstance trailer: @""];
    [newInstance tag: [NSString stringWithFormat: @"<%@>", tag]];
    [newInstance body: text];
    [newInstance parts: parts];
    [newInstance more: more];
    return [newInstance autorelease];
}

+ (id)initWithString: (NSString *) text tags: (NSArray *) tags {
    return [Parse initWithString: text tags: tags level: 0];
}

+ (id)initWithString: (NSString *) text tags: (NSArray *) tags level: (int) level {
    id newInstance = [[[self class] alloc] init];
    [newInstance text: text];
    [newInstance tags: tags];

    NSString *tag = [tags objectAtIndex: level];
    NSRange startTag = [text rangeOfString: [@"<" stringByAppendingString: tag] options: NSCaseInsensitiveSearch];
    if (startTag.location == NSNotFound) {
        [[NSException exceptionWithName: NSGenericException reason: [NSString stringWithFormat: 	@"Can't find tag: %@", tag] userInfo: nil] raise];
    }

    NSRange nextRange = NSMakeRange(NSMaxRange(startTag), [text length] - startTag.location - startTag.length);
    NSRange endTag = [text rangeOfString: [NSString stringWithString: @">"] options:     NSCaseInsensitiveSearch range: nextRange];
    if (endTag.location == NSNotFound) {
        [[NSException exceptionWithName: NSGenericException reason: [NSString stringWithFormat: 	@"Can't find tag: %@", tag] userInfo: nil] raise];
    }
    nextRange = NSMakeRange(NSMaxRange(endTag), [text length] - endTag.location - endTag.length);
    NSRange startEnd = [text rangeOfString: [@"</" stringByAppendingString: [tags objectAtIndex: level]] options: NSCaseInsensitiveSearch range: nextRange];
    if (startEnd.location == NSNotFound) {
        [[NSException exceptionWithName: NSGenericException reason: [NSString stringWithFormat: 	@"Can't find tag: %@", tag] userInfo: nil] raise];
    }
    nextRange = NSMakeRange(NSMaxRange(startEnd), [text length] - startEnd.location - startEnd.length);
    NSRange endEnd = [text rangeOfString: @">" options: NSCaseInsensitiveSearch range: nextRange];
    if (endEnd.location == NSNotFound) {
        [[NSException exceptionWithName: NSGenericException reason: [NSString stringWithFormat: 	@"Can't find tag: %@", tag] userInfo: nil] raise];
    }
    
    nextRange = NSMakeRange(NSMaxRange(startEnd), [text length] - endEnd.location - endEnd.length);
    NSRange startMore = NSMakeRange(NSNotFound, 0);
    if (nextRange.length > 0) {
        startMore = [text rangeOfString: [@"<" stringByAppendingString: [tags objectAtIndex: level]] options: NSCaseInsensitiveSearch range: nextRange];
    }

    [newInstance leader: [text substringWithRange: NSMakeRange(0, startTag.location)]];
    
    [newInstance tag: [text substringWithRange: NSUnionRange(startTag, endTag)]];

    [newInstance body: [text substringWithRange: NSMakeRange(NSMaxRange(endTag), startEnd.location - NSMaxRange(endTag))]];

    //end
    [newInstance end: [text substringWithRange: NSUnionRange(startEnd, endEnd)]];

    //trailer
    [newInstance trailer: [text substringWithRange: NSMakeRange(NSMaxRange(endEnd), [text length] - endEnd.location - endEnd.length)]];

    if (level + 1 < [tags count]) {
        Parse *parts = [Parse initWithString: [newInstance body] tags: tags level: level + 1];
        [newInstance body: nil];
        [newInstance parts: parts];
    }
    
    if (startMore.location != NSNotFound) {
        NSString *moreString = [text substringFromIndex: endEnd.location + endEnd.length];
        Parse *more = [Parse initWithString: moreString tags: tags level: level];
        [newInstance more: more];
    }

    
    return [newInstance autorelease];
}

+ (id)withString: (NSString *) text {

    NSArray *tags = [NSArray arrayWithObjects: @"table", @"tr", @"td", nil];

    return [Parse initWithString: text tags: tags];

    [tags release];
}
- (void) dealloc {
    [self parts: nil];
    [self more: nil];
    [self tags: nil];
    [self text: nil];
    [super dealloc];
}
- (void) text: (NSString *) text {
    [text retain];
    [_text release];
    _text = text;
}

- (void) tags: (NSArray *) tags {
    [tags retain];
    [_tags release];
    _tags = tags;
}

- (void) body: (NSString *) body {
    [body retain];
    [_body release];
    _body = body;
}

- (void) leader: (NSString *) leader {
    _leader = leader;
}

- (NSString *) leader {
    return _leader;
}

- (void) tag: (NSString * ) tag {
    _tag = tag;
}

- (NSString *) tag {
    return _tag;
}    

- (NSString *) body {
    return _body;
}

- (void) end: (NSString *)s {
    _end = s;
}

- (NSString *) end {
    return _end;
}

- (NSString *) trailer {
    return _trailer;
}

- (void) trailer: (NSString *) s{
    _trailer = s;
}

- (void) parts: (Parse *) parts {
    [parts retain];
    [_parts release];
    _parts = parts;
}

- (Parse *) parts {
    return _parts;
}

- (Parse *) more {
    return _more;
}

- (void) more: (Parse *) more {
    [more retain];
    [_more release];
    _more = more;
}

- (Parse *) at: (int) i {
    if (i == 0 || _more == nil) {
        return self;
    } else {
        return [_more at: i-1];
    }
}

- (Parse *) at: (int)i j: (int)j {
    return [[[self at: i] parts] at: j];
}

- (Parse *) at: (int)i j: (int)j k: (int)k {
    return [[[self at: i j: j] parts] at: k];
}

- (int) size {
    return _more == nil ? 1 : [_more size]+1;
}

- (Parse *) leaf {
    return _parts == nil ? self : [_parts leaf];
}

- (Parse *) last {
    return _more == nil ? self : [_more last];
}

- (NSString *) text {
    NSString *result = [[self class] unescape: [[self class] unformat: _body]];    
    return result;
}

+ (NSString *) unformat: (NSString *) s{
    NSString *result = @"";
    NSScanner *scanner = [NSScanner scannerWithString: s];
    NSString *chunk = @"";
    while (![scanner isAtEnd]) {
        [scanner scanUpToString: @"<" intoString: &chunk];
        if (chunk != nil) {
            result = [result stringByAppendingString: chunk];
        }
        NSString *tagBody = @"";
        NSString *rightCaret = @"";
        [scanner scanUpToString: @">" intoString: &tagBody];
        [scanner scanString: @">" intoString: &rightCaret];
        if (rightCaret == nil || [rightCaret length] == 0) {
            result = [result stringByAppendingString: tagBody];
        }
    }
    return result;
}

+ (NSString *) unescape: (NSString *) s {
    if ([s isEqualToString: @""]) {
        return s;
    }
    NSMutableString *result = [NSMutableString stringWithString: s];

    [result replaceOccurrencesOfString: @"&lt;" withString: @"<" options: NSLiteralSearch range: NSMakeRange(0, [result length])];
    [result replaceOccurrencesOfString: @"&gt;" withString: @">" options: NSLiteralSearch range: NSMakeRange(0, [result length])];
    [result replaceOccurrencesOfString: @"&nbsp;" withString: @" " options: NSLiteralSearch range: NSMakeRange(0, [result length])];
    [result replaceOccurrencesOfString: @"&amp;" withString: @"&" options: NSLiteralSearch range: NSMakeRange(0, [result length])];
    [result replaceOccurrencesOfString: @" " withString: @"" options: NSBackwardsSearch range: NSMakeRange([result length] - 1 , 1)];

    return result;
}

- (NSString *) description {
    return _text;
}

- (void) print: (NSFileHandle *) fh {
    [self write: fh string: _leader];
    [self write: fh string: _tag];
    if (_parts != nil) {
        [_parts print: fh];
    } else {
        [self write: fh string: _body];
    }
    [self write: fh string: _end];
    if (_more != nil) {
        [_more print: fh];
    } else {
        [self write: fh string: _trailer];
    }
}

- (void) addToTag: (NSString *) text {
    int last = [_tag length] - 1;
    _tag = [[[_tag substringToIndex: last] stringByAppendingString: text]
	stringByAppendingString: @">"];
}

- (void) addToBody: (NSString *) text {
    if (text != nil) {
        _body = [_body stringByAppendingString: text];
    }
}

- (void) write: (NSFileHandle *)fh string: (id)s {
    if ([s isKindOfClass: [NSNumber class]]) {
        [fh writeData: [[s descriptionWithLocale: nil] dataUsingEncoding: NSASCIIStringEncoding]];
    } else {
        [fh writeData: [s dataUsingEncoding: NSASCIIStringEncoding]];
    }
}
@end
