//
//  Summary.m
//  FIT
//
//  Created by Steven Newton on Thu Sep 25 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.
//

#import "FITSummary.h"

static NSString *countsKey = @"counts";

@implementation FITSummary

- (void) doTable: (Parse *) table {
   [summary setObject: [counts description] forKey: countsKey];
    NSArray *keys = [summary allKeys];
    NSEnumerator *enumerator = [[keys sortedArrayUsingSelector: @selector(compare:)] objectEnumerator];
    [[table parts] more: [self rows: enumerator]];
}

- (NSString *) gray: (NSString *) string {
    return [[@" <font color=#808080>" stringByAppendingString: string] stringByAppendingString:  @"</font>"];
}
- (Parse *) td: (NSString *) body more: (Parse *) more {
    Parse *td = [[Parse alloc] init];
    [td leader: @"\n"];
    [td tag: @"<td>"];
    [td body: [self gray: body]];
    [td end: @"</td>"];
    [td trailer: @""];
    [td more: more];
    [td parts: nil];
    return [td autorelease];
}

- (Parse *) tr: (Parse *) parts more: (Parse *) more {
    Parse * tr = [[Parse alloc] init];
    [tr leader: @"\n"];
    [tr tag: @"<tr>"];
    [tr more: more];
    [tr end: @"</td>"];
    [tr parts: parts];
    [tr trailer: @""];
    return [tr autorelease];
}

- (Parse *) rows: (NSEnumerator *) enumerator {
    id key = [enumerator nextObject];

    if (key != nil) {

        NSString *value = [[summary objectForKey: key] description];
        Parse *tdd = [self td: value more: nil];

        NSString *label = [key description];
        Parse *tdh = [self td: label more: tdd];

        Parse *tr = [self tr: tdh more: [self rows: enumerator]];
        return tr;
    } else {
        return nil;
    }

}
@end
