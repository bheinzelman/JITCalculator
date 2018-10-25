//  units.m

#import <XCTest/XCTest.h>

#include <string>
#include <iostream>
#include <sstream>

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
             @{@"value": @0, @"exp": @"2 > 3"},
             @{@"value": @1, @"exp": @"2 < 3"},
             @{@"value": @0, @"exp": @"2 >= 3"},
             @{@"value": @1, @"exp": @"2 >= 1"},
             @{@"value": @1, @"exp": @"2 <= 3"},
             @{@"value": @1, @"exp": @"2 <= 2"},
             @{@"value": @1, @"exp": @"2 == 2"},
             @{@"value": @1, @"exp": @"(1 == 2) == 0"},
             @{@"value": @0, @"exp": @"let x(a,b) | a > b = a | else = b"},
             @{@"value": @2, @"exp": @"x(2,1)"}
			 ];
}

- (void)test1 {
    Runtime rt;
	for (NSDictionary *d in self.expressions) {
		int expected = [d[@"value"] intValue];
		NSString *expression = d[@"exp"];
		
		std::string exp = std::string(expression.UTF8String);
        std::stringstream stream;
        stream << exp;

        try {
            std::vector<int> output;
            if (rt.evaluate(stream, output)) {
                if (output.size()) {
                    int value = output.front();
                    XCTAssert(value == expected);
                    assert(output.front() == expected);
                }
            }
        } catch (jcException exception) {
            std::cerr << exception.getMessage() << std::endl;
            XCTFail();
        }
	}
}

@end
