//
//  TypeAdapter.m
//  FIT
//
//  Created by Steven Newton on Mon Sep 22 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.
//

#import "TypeAdapter.h"


@implementation TypeAdapter
+ (TypeAdapter *) on: (Fixture *) f field: (NSString *) fieldName {
    TypeAdapter *newInstance;
    // if NSSelectorFromString(fieldName) yields a message that returns
    // a type '@', then create create self,
    // NSString or NSNumber
    // otherwise create a PrimitiveTypeAdapter
    newInstance = [[self class] on: f];
    newInstance->field = fieldName;
    return newInstance; 
}

+ (TypeAdapter *) on: (Fixture *) f method: (NSString *) methodName {
    TypeAdapter *newInstance;
    newInstance = [[self class] on: f];
    newInstance->method = methodName;
    return newInstance;
}

+ (TypeAdapter *) on: (Fixture *) f {
    TypeAdapter *newInstance = [[[self class] alloc] init];
    newInstance->fixture = f;
    return [newInstance autorelease];
}

+ (BOOL) returnsObject: (Fixture *) f withSelector: (SEL) aSelector {
    NSMethodSignature *sig = [f methodSignatureForSelector: aSelector];
    const char *returnType = [sig methodReturnType];
    if (returnType == nil) {
        return false;
    }
    return returnType[0] == '@';
}

+ (BOOL) takesObject: (Fixture *) f forSelector: (SEL) aSelector {
    NSMethodSignature *sig = [f methodSignatureForSelector: aSelector];
    if ([sig numberOfArguments] < 3) {
        return false;
    }
    const char *paramType = [sig getArgumentTypeAtIndex: 2];
    return paramType[0] == '@';
}

- (void) fixture: (Fixture *) f {
    fixture = f;
}
- (void) fieldName: (NSString *) fieldName {
    field = fieldName;
}

- (void) target: (id) object {
    target = object;
}

- (void) set: (id) i {
    SEL selector = NSSelectorFromString([field stringByAppendingString: @":"]);
    [fixture performSelector: selector withObject: i];
}

- (id) get {
    SEL selector;
    if (field != nil) {
        selector = NSSelectorFromString(field);
    } else {
        selector = NSSelectorFromString(method);
    }
    if (target != nil) {
        return [target performSelector: selector];
    } else {
        return [fixture performSelector: selector];
    }
}

- (NSObject *) parse: (NSString *) s {
    NSString *trimmed = [s stringByTrimmingCharactersInSet: [NSCharacterSet whitespaceCharacterSet]];
    if ([trimmed isEqualToString: @"false"]) {
        return [NSNumber numberWithBool: NO];
    } else if ([trimmed isEqualToString: @"true"]) {
        return [NSNumber numberWithBool: YES];
    } else {
        return [fixture parse: s];
    }
}

- (id) invokeMethod {
    SEL selector = NSSelectorFromString(method);
    return [fixture performSelector: selector];
}

- (BOOL) compare: (NSObject *) aValue to: (NSObject *) bValue {
    if (aValue == nil) {
        return (bValue == nil);
    }
    return [aValue isEqual: bValue];
}

@end
