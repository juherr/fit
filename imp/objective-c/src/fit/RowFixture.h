//
//  RowFixture.h
//  FIT
//
//  Created by Steven Newton on Wed Oct 15 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.
//

#import <Foundation/Foundation.h>
#import "ColumnFixture.h"

@interface RowFixture : ColumnFixture {
    NSMutableArray *missing;
    NSArray *results;
    NSMutableArray *surplus;
}
- (void) match: (NSArray *) expected with: (NSArray *) computed col: (int) col;
- (void) check: (NSMutableArray *) eList against: (NSMutableArray *) cList;
- (NSArray *)list: (Parse *) rows;
- (Parse *) buildRows: (NSArray *)rows;
- (Parse *) buildCells: (NSObject *)row;
- (void) markParse: (Parse *) rows as: (NSString *)message;
- (void) markEnumerator: (NSEnumerator *) rows as: (NSString *)message;
- (NSMutableArray *) query;
- (NSDictionary *) eSort: (NSArray *) list col: (int) i;
- (NSDictionary *) cSort: (NSArray *) list col: (int) i;
- (NSSet *) unionOf: (NSArray *) a and: (NSArray *) b;
@end
