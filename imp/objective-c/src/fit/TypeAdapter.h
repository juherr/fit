//
//  TypeAdapter.h
//  FIT
//
//  Created by Steven Newton on Mon Sep 22 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.
//

#import <Foundation/Foundation.h>

#import "Fixture.h"

@interface TypeAdapter : NSObject {
    Fixture *fixture;
    NSObject *target;
@public
    NSString* field;
    NSString* method;
}
+ (TypeAdapter *) on: (Fixture *) f field: (NSString *) fieldName;
+ (TypeAdapter *) on: (Fixture *) f method: (NSString *) methodName;
+ (TypeAdapter *) on: (Fixture *) f;
+ (BOOL) returnsObject: (Fixture *) f withSelector: (SEL) aSelector;
+ (BOOL) takesObject: (Fixture *) f forSelector: (SEL) aSelector;
- (void) fixture: (Fixture *) f;
- (void) set: (NSObject *) i;
- (NSObject *) get;
- (NSObject *) parse: (NSString *)s;
- (NSObject *) invokeMethod;
- (BOOL) compare: (NSObject *) aValue to: (NSObject *) bValue;
- (void) target: (id) object;
@end
