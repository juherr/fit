//
//  ColumnFixture.m
//  FIT
//
//  Created by Steven Newton on Mon Sep 29 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.
//

#import "ColumnFixture.h"


@implementation ColumnFixture

- (void) doRows: (Parse *)rows {
    [self bind: [rows parts]];
    [super doRows: [rows more]];
}

- (void) doRow: (Parse *)row {
    hasExecuted = NO;
NS_DURING
    [self reset];
    [super doRow: row];
    if (!hasExecuted) {
        [self execute];
    }
NS_HANDLER
    [self exception: [row leaf] e: localException];
NS_ENDHANDLER
}

- (void) doCell: (Parse *) cell columnNumber: (int) columnNumber {
    TypeAdapter *a = [columnBindings objectAtIndex: columnNumber];
    NS_DURING
        NSString *text = [cell text];
        if ([text isEqualToString: @""]) {
            [self check: cell adapter: a];
        } else if (a == nil) {
            [self ignore: cell];
            // if this is a field column, set it
        } else if (a->field != nil) {
            [a set: [a parse: text]];
            // else this is a method column, call the method
        } else if (a->method != nil) {
            [self check: cell adapter: a];
        }
    NS_HANDLER
        [self exception: cell e: localException];
    NS_ENDHANDLER
}

- (void) check: (Parse *) cell adapter: (TypeAdapter *) a {
    [super check: cell adapter: a];
}

- (void) bind: (Parse *)heads {
    int i;
    columnBindings = [NSMutableArray arrayWithCapacity: [heads size]];
    NSString *suffix = @"()";
    for (i = 0; heads != nil; i++, heads = [heads more]) {
        NSString *name = [heads text];
        if (name == nil) {
            return;
        } else if ([name isEqualToString: @""]) {
            [columnBindings insertObject: [NSNull null] atIndex: i];
        } else if ([name hasSuffix: suffix]) {
            name = [name substringToIndex: [name length] - [suffix length]];
            [columnBindings insertObject: [self bindMethod: name] atIndex: i];
        } else {
            [columnBindings insertObject: [self bindField: name] atIndex: i];
        }
            
    }
}

- (TypeAdapter *) bindField: (NSString *) name {
    return [TypeAdapter on: self field: name];
}

- (TypeAdapter *) bindMethod: (NSString *) name {
    return [TypeAdapter on: self method: name];
}

- (void) reset {
    // about to process first cell of row
}

- (void) execute {
    // about to process first method call of row
}

@end
