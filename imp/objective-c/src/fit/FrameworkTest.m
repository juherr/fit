//
//  FrameworkTest.m
//  FIT
//
//  Created by Steven Newton on Thu Sep 18 2003.
//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.
//

#import "FrameworkTest.h"

@interface TestFixture : Fixture {
    int sampleInt;
    NSNumber *sampleInteger;
    NSNumber *sampleFloat;
    BOOL sampleBoolean;
    unichar ch;
    NSString* name;
}
- (NSString *) name;
- (void) name: (NSString *) s;
- (int) sampleInt;
- (void) sampleInt: (id) i;
- (NSNumber *) sampleInteger;
- (void) sampleInteger: (NSNumber *) i;
- (NSNumber *) sampleFloat;
- (void) sampleFloat: (NSNumber *) fl;
- (NSNumber *) sampleBoolean;
- (NSNumber *) pi;
- (NSString *) ch;
- (void) ch: (NSString *)s;
@end

@implementation TestFixture
- (NSString *) name {
    return name;
}

- (void) name: (NSString *) s {
    name = s;
}

- (void) ch: (NSString *)s {
    ch = [s characterAtIndex: 0];
}

- (void) sampleInt: (id) i {
    sampleInt = [i intValue];
}
- (int) sampleInt {
    return sampleInt;
}
- (NSNumber *) sampleInteger {
    return sampleInteger;
}
- (void) sampleInteger: (NSNumber *) i {
    sampleInteger = i;
}
- (void) sampleFloat: (NSNumber *) fl {
    sampleFloat = fl;
}
- (NSNumber *) sampleFloat {
    return sampleFloat;
}

- (void) sampleBoolean: (NSNumber *) b {
    sampleBoolean = [b boolValue];
}

- (NSNumber *) sampleBoolean {
    return [NSNumber numberWithBool: sampleBoolean];
}

- (NSNumber *) pi {
    return [NSNumber numberWithFloat: 3.14159862];
}
- (NSString *) ch {
    return [NSString stringWithCharacters: &ch length: 1];
}
@end

@implementation FrameworkTest

- (void) testParsing {
    NSArray *tags = [NSArray arrayWithObject: @"table"];

    Parse *p = [Parse initWithString: @"leader<Table foo=2>body</table>trailer" tags: tags];

    [self assert: [p leader] equals: @"leader"];
    [self assert: [p tag] equals: @"<Table foo=2>"];
    [self assert: [p body] equals: @"body"];
    [self assert: [p trailer] equals: @"trailer"];
}

- (void) testRecursing {
    NSArray *tags = [NSArray arrayWithObjects: @"table", @"tr", @"td", nil];
    Parse *p = [Parse initWithString: @"leader<table><TR><Td>body</tD></TR></table>trailer" tags: tags];
    [self assert: [p body] equals: nil];
    [self assert: [[p parts] body] equals: nil];
    [self assert: [[[p parts] parts] body] equals: @"body"];
}

- (void) testIterating {
    Parse *p = [Parse withString: @"leader<table><tr><td>one</td><td>two</td><td>three</td></tr></table>trailer"];
    [self assert: [[[p parts] parts] body] equals: @"one"];
    [self assert: [[[[p parts] parts] more] body] equals: @"two"];
    [self assert: [[[[[p parts] parts] more] more] body] equals: @"three"];
}

- (void) testIndexing {
    Parse *p = [Parse withString: @"leader<table><tr><td>one</td><td>two</td><td>three</td></tr><tr><td>four</td></tr></table>trailer"];
    [self assert: [[p at: 0 j: 0 k: 0] body] equals: @"one"];
    [self assert: [[p at: 0 j: 0 k: 1] body] equals: @"two"];
    [self assert: [[p at: 0 j: 0 k: 2] body] equals: @"three"];
    [self assert: [[p at: 0 j: 0 k: 3] body] equals: @"three"];
    [self assert: [[p at: 0 j: 0 k: 4] body] equals: @"three"];
    [self assert: [[p at: 0 j: 1 k: 0] body] equals: @"four"];
    [self assert: [[p at: 0 j: 1 k: 1] body] equals: @"four"];
    [self assert: [[p at: 0 j: 2 k: 0] body] equals: @"four"];
    [self assertInt: [p size] equals: 1];
    [self assertInt: [[p parts] size] equals: 2];
    [self assertInt: [[[p parts] parts] size] equals: 3];
    [self assert: [[p leaf] body] equals: @"one"];
    [self assert: [[[[p parts] last] leaf] body] equals: @"four"];
}

- (void) testParseException {
NS_DURING
    Parse *p = [Parse withString: @"leader<table><tr><th>one</th><th>two</th><th>three</th></tr><tr><td>four</td></tr></table>trailer"];
    [self assertFalse: p == nil];
    [self fail: @"expected exception not thrown"];
    NS_VOIDRETURN;
NS_HANDLER
    [self assert: [localException reason] equals: @"Can't find tag: td"];
NS_ENDHANDLER
}

- (void) testText {
    NSArray *tags = [NSArray arrayWithObjects: @"td", nil];
    Parse *p = [Parse initWithString: @"<td>a&lt;b</td>" tags: tags];
    [self assert: [p body] equals: @"a&lt;b"];
    [self assert: [p text] equals: @"a<b"];

    p = [Parse initWithString: @"<td>\ta&gt;b&nbsp;&amp;&nbsp;b>c &&&nbsp;</td>" tags: tags];
    [self assert: [p text] equals: @"a>b & b>c &&"];

    p = [Parse initWithString: @"<td>\ta&gt;b&nbsp;&amp;&nbsp;b>c &&nbsp;</td>" tags: tags];
    [self assert: [p text] equals: @"a>b & b>c &"];

    p = [Parse initWithString: @"<TD><P><FONT FACE=\"Arial\" SIZE=2>GroupTestFixture</FONT></TD>" tags: tags];
    [self assert: [p text] equals: @"GroupTestFixture"];
}

- (void) testUnescape {

    [self assert: [Parse unescape: @"a&lt;b"] equals: @"a<b"];
    [self assert: [Parse unescape: @"a&gt;b&nbsp;&amp;&nbsp;b>c &&"] equals: @"a>b & b>c &&"];
    [self assert: [Parse unescape: @"&amp;amp;&amp;amp;"] equals: @"&amp;&amp;"];
    [self assert: [Parse unescape: @"a&gt;b&nbsp;&amp;&nbsp;b>c &&"] equals: @"a>b & b>c &&"];
}

- (void) testUnformat {
    [self assert: [Parse unformat: @"<font size=+1>a</font>b"] equals: @"ab"];
    [self assert: [Parse unformat: @"a<font size=+1>b</font>"] equals: @"ab"];
    [self assert: [Parse unformat: @"a<b"] equals: @"a<b"];
}

- (void) testRuns {
    [self run: @"arithmetic" right: 37 wrong: 10 ignores: 0 exceptions: 2];
    [self run: @"CalculatorExample" right: 77 wrong: 7 ignores: 0 exceptions: 0];
    [self run: @"MusicExample" right: 95 wrong: 0 ignores: 0 exceptions: 0];
}

- (void) run: (NSString *) file right: (int) right wrong: (int) wrong ignores: (int) ignores exceptions: (int) exceptions {

    NSFileHandle *inHandle = [NSFileHandle fileHandleForReadingAtPath:
        [self directory: @"../Documents/" file: file suffix: @".html"]];

    NSFileManager *fm = [NSFileManager defaultManager];

    NSString *outFileName = [self directory: @"../Reports/" file: file suffix: @".html"];

    [fm createFileAtPath: outFileName contents: nil attributes: nil];

    NSFileHandle *outHandle = [NSFileHandle fileHandleForWritingAtPath: outFileName];

    NSData *data = [inHandle readDataToEndOfFile];

    NSString *content = [[NSString alloc] initWithData: data encoding: NSASCIIStringEncoding];

    Parse *p = [Parse withString: content];
    [content release];

    Fixture *fixture = [[Fixture alloc] init];
    [fixture autorelease];
    [fixture doTables: p];

    [p print: outHandle];
    [inHandle closeFile];
    [outHandle closeFile];

    [self assertInt: fixture->counts->right equals: right message: [file stringByAppendingString: @" right"]];
    [self assertInt: fixture->counts->wrong equals: wrong message: [file stringByAppendingString: @" wrong"]];
    
}

- (NSString *) directory: (NSString *) directory file: (NSString *) file suffix: (NSString *) suffix {
    return [[directory stringByAppendingString: file] stringByAppendingString: suffix];
}

//    protected void run(String file, int right, int wrong, int ignores, int exceptions) throws 
- (void) testTypeAdapter {
    TestFixture *f = [[TestFixture alloc] init];
    // don't know when I'll really be done with it
    [f autorelease];
    TypeAdapter *a = [TypeAdapter on: f field: @"sampleInt"];
    [a set: [a parse: @"123456"]];
    [self assertInt: [f sampleInt] equals: 123456];

    [self assert: [a parse: @"-234567"] equals: [NSNumber numberWithInt: [@"-234567" intValue]]];

    a = [TypeAdapter on: f field: @"sampleInteger"];
    [a set: [a parse: @"54321"]];
    [self assert: [f sampleInteger] equals: [NSNumber numberWithInt: [@"54321" intValue]]];

    a = [TypeAdapter on: f method: @"pi"];
    NSNumber *result = (NSNumber *)[a invokeMethod];
    [self assertFloat: [result floatValue] equals: 3.14159 precision: 0.00001];
    [self assert: [a invokeMethod] equals: [NSNumber numberWithFloat: 3.14159862]];

    a = [TypeAdapter on: f field: @"ch"];
    [a set: [a parse: @"abc"]];
    [self assert: [f ch] equals: @"a"];

    a = [TypeAdapter on: f field: @"name"];
    [a set: [a parse: @"xyzzy"]];
    [self assert: [f name] equals: @"xyzzy"];

    a = [TypeAdapter on: f field: @"sampleFloat"];
    [a set: [a parse: @"6.02e23"]];
    [self assertFloat: [[f sampleFloat] floatValue] equals: 6.02e23 precision: 1e17];

    a = [TypeAdapter on: f field: @"sampleBoolean"];
    [a set: [a parse: @"false"]];
    [self assertFalse: [[f sampleBoolean] boolValue] message: @"boolean failure"];
    
    /*
    a = TypeAdapter.on(f, f.getClass().getField("sampleArray"));
    a.set(a.parse("1,2,3"));
    assertEquals(1, f.sampleArray[0]);
    assertEquals(2, f.sampleArray[1]);
    assertEquals(3, f.sampleArray[2]);
    assertEquals("1, 2, 3", a.toString(f.sampleArray));
    assertTrue(a.equals(new int [] {1,2,3}, f.sampleArray));
    a = TypeAdapter.on(f, f.getClass().getField("sampleDate"));
    Date date = new Date(49,4,26);
    a.set(a.parse(DateFormat.getDateInstance().format(date)));
    assertEquals(date, f.sampleDate);
    a = TypeAdapter.on(f, f.getClass().getField("sampleByte"));
    a.set(a.parse("123"));
    assertEquals(123, f.sampleByte);
    a = TypeAdapter.on(f, f.getClass().getField("sampleShort"));
    a.set(a.parse("12345"));
    assertEquals(12345, f.sampleShort);
*/    
}

@end

