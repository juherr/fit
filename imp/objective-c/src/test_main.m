//  Copyright (c) 2002 Cunningham & Cunningham, Inc.
//  Released under the terms of the GNU General Public License version 2 or later.

#import <ObjcUnit/ObjcUnit.h>
#import "AllTests.h"

int main(int argc, const char *argv[]) {
    TestRunnerMain([AllTests class]);
    return 0;
}
