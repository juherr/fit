//
//  Fixture.h
//  FIT
//
//  Created by Steven Newton on Tue Sep 23 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.
//

#import <Foundation/Foundation.h>

//#import "TypeAdapter.h"
#import "Parse.h"

@interface Counts : NSObject {
@public
    int right;
    int wrong;
    int ignores;
    int exceptions;
}
- (NSString *) description;
- (void) tally: (Counts *)source;
@end

@interface Fixture : NSObject {
@public
    NSMutableDictionary *summary;
    Counts *counts;

}
// Traversal
- (void) doTables: (Parse *) tables;
- (void) doTable: (Parse *) table;
- (void) doRows: (Parse *) rows;
- (void) doRow: (Parse *) row;
- (void) doCells: (Parse *) cells;
- (void) doCell: (Parse *) cell columnNumber: (int) columnNumber;
- (void) check: (Parse *) cell adapter: (id) a;
- (NSObject *) parse: (NSString *) s;
- (NSString *) gray: (id) s;
- (NSString *) label: (id) value;
- (void) ignore: (Parse *) cell;
- (void) right: (Parse *) cell;
- (void) wrong: (Parse *) cell;
- (void) wrong: (Parse *) cell value: (NSString *) actual;
- (void) exception: (Parse *) cell e: (NSException *) exception;
- (NSString *) camelCase: (NSString *) className;
@end
