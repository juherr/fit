// Copyright (c) 2002 Cunningham & Cunningham, Inc.
// Released under the terms of the GNU General Public License version 2 or later.

#import "FileRunner.h"

@implementation FileRunner
- (void) run: (const char **)argv {

    if (argv[1] == nil || argv[2] == nil) {
        printf("usage: FileRunner input-file output-file\n");
        return;
    }
    NSString *inFileName = [NSString stringWithCString: argv[1]];
    NSFileHandle *inHandle = [NSFileHandle fileHandleForReadingAtPath: inFileName];

    NSFileManager *fm = [NSFileManager defaultManager];

    NSString *outFileName = [NSString stringWithCString: argv[2]];

    [fm createFileAtPath: outFileName contents: nil attributes: nil];

    NSFileHandle *outHandle = [NSFileHandle fileHandleForWritingAtPath: outFileName];

    NSData *data = [inHandle readDataToEndOfFile];

    NSString *content = [[NSString alloc] initWithData: data encoding: NSASCIIStringEncoding];
    Parse *p = [Parse withString: content];
    [content release];
    fixture = [[Fixture alloc] init];
    [fixture->summary setObject: inFileName forKey: @"input file"];
    [fixture->summary setObject: outFileName forKey: @"output file"];
    [fixture doTables: p];
    [fixture release];
    [p print: outHandle];
    [inHandle closeFile];
    [outHandle closeFile];
}
@end

int main(int argc, const char *argv[]) {

    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    FileRunner *runner = [[FileRunner alloc] init];
    [runner run: argv];
    [runner release];
    [pool release];
    return 0;
}
