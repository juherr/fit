//
//  Fixture.m
//  FIT
//
//  Created by Steven Newton on Tue Sep 23 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.
//

#import "Fixture.h"
#import "TypeAdapter.h"

@implementation Counts
- (NSString *) description {
    return [NSString stringWithFormat: @" %d right, %d wrong, %d ignores, %d exceptions",
        right, wrong, ignores, exceptions];
}
- (void) tally: (Counts *)source {
}
@end


@interface FITRunTime : NSObject {
    NSCalendarDate *start;
    NSTimeInterval elapsed;
}
@end

@implementation FITRunTime
- (id) init {
    self = [super init];
    start = [NSCalendarDate calendarDate];
    return self;
}

- (long) d: (long) scale {
    long report = elapsed / scale;
    elapsed -= report * scale;
    return report;
}


- (NSString *) description {
    NSCalendarDate *now = [NSCalendarDate calendarDate];
    elapsed = [now timeIntervalSinceDate: start] * 1000;
    return [NSString stringWithFormat: @"%d:%d%d:%d%d.%d", [self d: 60000], [self d: 10000],
        [self d: 1000], [self d: 100], [self d: 10], [self d: 1]];
}
@end


BOOL isNumeric(NSString *s) {
    NSRange r = [s rangeOfCharacterFromSet:[[NSCharacterSet characterSetWithCharactersInString: @"0123456789-."] invertedSet]];
    BOOL isNumeric = (r.location == NSNotFound);
    return (isNumeric);
}

@implementation Fixture

- (id) init {
    self = [super init];

    counts = [[Counts alloc] init];
    summary = [NSMutableDictionary dictionary];
    
    return self;
}

- (oneway void) release {
    [counts release];
    [super release];
}

- (void) doTables: (Parse *) tables {
    FITRunTime *runTime = [[FITRunTime alloc] init];
    NSCalendarDate *start = [NSCalendarDate calendarDate];
    [summary setObject: start forKey: @"run date"];
    [summary setObject: runTime forKey: @"run elapsed time"];
    while (tables != nil) {
        Parse *heading = [tables at:0 j: 0 k:0];
         if (heading != nil) {
             NSString *className = [self camelCase: [heading text]];
             Fixture *fixture = [[NSClassFromString(className) alloc] init];
             // TODO how to release fixture, or to release it at all?
             if (fixture != nil) {
                 fixture->counts = counts;
                 fixture->summary = summary;
                 [fixture doTable: tables];
             }
         }
         tables = [tables more];
     }
    [runTime release];
    
}

- (void) doTable: (Parse *) table {
    [self doRows: [[table parts] more]];
}

- (void) doRows: (Parse *) rows {
    while (rows != nil) {
        Parse *more = [rows more];
        [self doRow: rows];
        rows = more;
    }
}

- (void) doRow: (Parse *) row {
    [self doCells: [row parts]];
}

- (void) doCells: (Parse *) cells {
    int i = 0;
    while (cells != nil) {
        NS_DURING
            [self doCell: cells columnNumber: i];
        NS_HANDLER
            [self exception: cells e: localException];
        NS_ENDHANDLER
        cells = [cells more];
        i++;
    }
}

- (void) doCell: (Parse *) cell columnNumber: (int) columnNumber {
    [self ignore: cell];
}

- (void) check: (Parse *) cell adapter: (id) a {
    NSString *text = [cell text];
    if ([text isEqualToString: @""]) {
        NS_DURING
            [cell addToBody: [self gray: [a get]]];
        NS_HANDLER
            [cell addToBody: [self gray: @"error"]];
        NS_ENDHANDLER
    } else if (a == nil) {
        [self ignore: cell];
    } else if ([text isEqualToString: @"error"]) {
        NS_DURING
            id result = [a invokeMethod];
            [self wrong: cell value: result];
        NS_HANDLER
            [self right: cell];
        NS_ENDHANDLER
    } else {
        NS_DURING
            id result = [a get];
            if([a compare: result to: [a parse: text]]) {
                [self right: cell];
            } else {
                if (result == nil) {
                    [self wrong: cell value: @"nil"];
                } else {
                    [self wrong: cell value: result];
                }
            }
        NS_HANDLER
            [self exception: cell e: localException];
        NS_ENDHANDLER
    }
}
- (NSObject *) parse: (NSString *) s {
    if (isNumeric(s)) {
        return [NSNumber numberWithFloat: [s floatValue]];
    } else {
        return s;
    }
}

- (NSString *) gray: (id) s {
    return [NSString stringWithFormat: @" <font color=#808080>%@</font>", [s description]];
}

- (void) ignore: (Parse *) cell {
    [cell addToTag: @" bgcolor=\"#efefef\""];
    counts->ignores++;
}

- (void) right: (Parse *) cell {
    [cell addToTag: @" bgcolor=\"#cfffcf\""];
    counts->right++;
}

- (void) wrong: (Parse *) cell {
    [cell addToTag: @" bgcolor=\"#ffcfcf\""];
    counts->wrong++;
}

- (void) wrong: (Parse *) cell value: (id) actual {
    [self wrong: cell];
    [cell addToBody: @" <font size=-1 color=#c08080><i>expected</i></font>"];
    [cell addToBody: @"<hr>"];
    [cell addToBody: [self label: actual]];
    [cell addToBody: @" <font size=-1 color=#c08080><i>actual</i></font>"];
}
- (void) exception: (Parse *) cell e: (NSException *) exception {
    [cell addToBody: [NSString stringWithFormat: @"<hr><pre><font size=-2>%@: %@</font></pre>", [exception name], [exception reason]]];
    [cell addToTag: @" bgcolor=\"#ffffcf\""];
    counts->exceptions++;
}

- (NSString *) label: (id) value {
    return [NSString stringWithFormat: @" <font size=-1 color=#c08080><i>%@</i></font> ", value];
}

- (NSString *) camelCase: (NSString *) className {
    NSArray *packages = [className componentsSeparatedByString: @"."];
    if ([packages count] == 1) {
        return className;
    }

    NSString *prefix = [[packages objectAtIndex: 0] uppercaseString];
    NSString *result = prefix;
    int i;
    for (i = 1; i < [packages count] - 1; i++) {
        result = [result stringByAppendingString: [[packages objectAtIndex: i] capitalizedString]];
    }
    result = [result stringByAppendingString: [packages objectAtIndex: i]];
    return result;
}

@end

