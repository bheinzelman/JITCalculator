//  units.m

#import <XCTest/XCTest.h>

#include <string>

#include "Calculator.hpp"

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
			 ];
}

- (void)test1 {
	for (NSDictionary *d in self.expressions) {
		int expected = [d[@"value"] intValue];
		NSString *expression = d[@"exp"];
		
		std::string exp = std::string(expression.UTF8String);
		
		Calculator c(exp);
		XCTAssert(c.calculate() == expected);
	}
}

@end
