//
//  RowFixture.m
//  FIT
//
//  Created by Steven Newton on Wed Oct 15 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.
//

#import "RowFixture.h"
@implementation RowFixture

- (void) doRows: (Parse *) rows {
    missing = [NSMutableArray array];
    surplus = [NSMutableArray array];
NS_DURING
    [self bind: [rows parts]];
    results = [self query];
    [self match: [self list: [rows more]] with: results col: 0];
    Parse *last = [rows last];
    [last more: [self buildRows: surplus]];
    [self markParse: [last more] as: @"surplus"];
    [self markEnumerator: [missing objectEnumerator] as: @"missing"];
NS_HANDLER
    [self exception: [rows leaf] e: localException];
NS_ENDHANDLER
}

- (NSMutableArray *) query {
    return [self doesNotRecognizeSelector: _cmd];
}

- (void) match: (NSArray *) expected with: (NSArray *) computed col: (int) col {
    if (col >= [columnBindings count]) {
        [self check: [NSMutableArray arrayWithArray: expected]
            against: [NSMutableArray arrayWithArray: computed]];
    } else if ([columnBindings objectAtIndex: col] == nil) {
        [self match: expected with: computed col: col+1];
    } else {
        // do some fancy stuff
        NSDictionary *eMap = [self eSort: expected col: col];
        NSDictionary *cMap = [self cSort: computed col: col];
        NSSet *keys = [self unionOf: [eMap allKeys] and: [cMap allKeys]];
        NSEnumerator *i = [keys objectEnumerator];
        id key;
        while (key = [i nextObject]) {
            NSMutableArray *eList = [eMap objectForKey: key];
            NSMutableArray *cList = [cMap objectForKey: key];
            if (eList == nil) {
                [surplus addObjectsFromArray: cList];
            } else if (cList == nil) {
                [missing addObjectsFromArray: eList];
            } else if ([eList count] == 1 && [cList count] == 1) {
                [self check: eList against: cList];
            } else {
                [self match: eList with: cList col: col + 1];
            }
        }
    }
}

void bin(NSMutableDictionary *dict, id key, id row) {
    id obj;
    if (obj = [dict objectForKey: key]) {
        [obj addObject: row];
    } else {
        NSMutableArray *list = [NSMutableArray arrayWithObject: row];
        [dict setObject: list forKey: key];
    }
}

- (NSDictionary *) eSort: (NSArray *) list col: (int) i {
    NSMutableDictionary *result = [NSMutableDictionary dictionaryWithCapacity: [list count]];
    TypeAdapter *a = [columnBindings objectAtIndex: i];
    NSEnumerator *e = [list objectEnumerator];
    Parse *row;
    while (row = [e nextObject]) {
        Parse *cell = [[row parts] at: i];
        id key = [a parse: [cell text]];
        bin(result, key, row);
    }
    return result;
}


- (NSDictionary *) cSort: (NSArray *) list col: (int) i {
    NSMutableDictionary *result = [NSMutableDictionary dictionaryWithCapacity: [list count]];
    TypeAdapter *a = [columnBindings objectAtIndex: i];
    NSEnumerator *e = [list objectEnumerator];
    id row;
    while (row = [e nextObject]) {
        [a target: row];
        id key = [a get];
        bin(result, key, row);
    }
    return result;
}

- (NSSet *) unionOf: (NSArray *) a and: (NSArray *) b {
    NSMutableSet *aSet = [NSMutableSet setWithArray: a];
    NSSet *bSet = [NSSet setWithArray: b];
    [aSet unionSet: bSet];
    return aSet;
}

- (void) check: (NSMutableArray *) eList against: (NSMutableArray *) cList {
    if ([eList count] == 0) {
        [surplus addObjectsFromArray: cList];
        return;
    }
    if ([cList count] == 0) {
        [surplus addObjectsFromArray: eList];
        return;
    }
    Parse *row = [eList objectAtIndex: 0];
    [eList removeObjectAtIndex: 0];
    Parse *cell = [row parts];
    id obj = [cList objectAtIndex: 0];
    [cList removeObjectAtIndex: 0];
    int i;
    for (i = 0; i < [columnBindings count] && (cell != nil); i++) {
        TypeAdapter *a = [columnBindings objectAtIndex: i];
        if (a != nil) {
            [a target: obj];
        }
        [self check: cell adapter: a];
        cell = [cell more];
    }
    [self check: eList against: cList];
}

- (NSArray *)list: (Parse *) rows {
    NSMutableArray *result = [NSMutableArray array];
    while (rows != nil) {
        [result addObject: rows];
        rows = [rows more];
    }
    return result;
}

- (Parse *) buildRows: (NSArray *)rows {
    Parse *root = [Parse parseWithBody: nil tag: nil parts: nil more: nil];
    Parse *next = root;
    int i;
    for (i = 0; i < [rows count]; i++) {
        [next more: [Parse parseWithBody: nil tag: @"tr" parts: [self buildCells: [rows objectAtIndex: i]] more: nil]];
        next = [next more];
    }
   return [root more];
}

- (Parse *) buildCells: (NSObject *)row {
    if (row == nil) {
        Parse *empty = [Parse parseWithBody: nil tag: @"td" parts: nil more: nil];
        [empty addToTag: [NSString stringWithFormat: @" colspan=%d", [columnBindings count]]];
        return empty;
    }
    Parse *root = [Parse parseWithBody: nil tag: nil parts: nil more: nil];
    Parse *next = root;
    int i;
    for (i = 0; i < [columnBindings count]; i++) {
        [next more: [Parse parseWithBody: @"&nbsp;" tag: @"td" parts: nil more: nil]];
        next = [next more];
        TypeAdapter *a = [columnBindings objectAtIndex: i];
        if (a == nil) {
            [self ignore: next];
        } else {
            [a target: row];
            [next body: [[a get] description]];
        }
    }
    return [root more];
}
    
- (void) markParse: (Parse *) rows as: (NSString *)message {
    while (rows != nil) {
        [self wrong: [rows parts]];
        [[rows parts] addToBody: [self label: message]];
        rows = [rows more];
    }
}

- (void) markEnumerator: (NSEnumerator *) rows as: (NSString *)message {
    Parse *row;
    while (row = [rows nextObject]) {
        [self wrong: [row parts]];
        [[row parts] addToBody: [self label: message]];        
    }
}

@end
