//  units.m

#import <XCTest/XCTest.h>

#include <string>

#include "Runtime.hpp"

@interface units : XCTestCase

@end

@implementation units

- (NSArray<NSDictionary *> *)expressions
{
	return @[
			 @{@"value": @2, @"exp": @"1 + 1"},
			 @{@"value": @11, @"exp": @"5 * 2 + 1"},
			 @{@"value": @11, @"exp": @"(5 * 2) + 1"},
			 @{@"value": @2, @"exp": @"2 / 1"},
			 @{@"value": @20, @"exp": @"(5 - 1) * 5"},
			 @{@"value": @99, @"exp": @"(5 - 1) * 5 * 5 - 1"},
             @{@"value": @0, @"exp": @"let add(x,y) = x + y"},
             @{@"value": @4, @"exp": @"add(add(1,1), add(1,1))"},
			 ];
}

- (void)test1 {
    Runtime rt;
	for (NSDictionary *d in self.expressions) {
		int expected = [d[@"value"] intValue];
		NSString *expression = d[@"exp"];
		
		std::string exp = std::string(expression.UTF8String);

        int value;
        try {
            if (rt.evaluate(exp, &value)) {
                XCTAssert(value == expected);
            }
        } catch (...) {
            XCTFail();
        }
	}
}

@end
