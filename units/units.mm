//  units.m

#import <XCTest/XCTest.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Runtime.hpp"
#include "jcVariable.hpp"

@interface units : XCTestCase

@end

@implementation units {
    NSString *_testDirectory;
}

- (void)setUp
{
    _testDirectory = [[[NSProcessInfo processInfo] environment] objectForKey:@"TEST_DIR"];
}

- (NSArray<NSDictionary *> *)inlineExpressions
{
	return @[
			 @{@"value": @2, @"exp": @"1 + 1"},
			 @{@"value": @11, @"exp": @"5 * 2 + 1"},
			 @{@"value": @11, @"exp": @"(5 * 2) + 1"},
			 @{@"value": @2, @"exp": @"2 / 1"},
			 @{@"value": @20, @"exp": @"(5 - 1) * 5"},
			 @{@"value": @99, @"exp": @"(5 - 1) * 5 * 5 - 1"},
             @{@"value": @0, @"exp": @"let add(x123,y) = x123 + y"},
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
             @{@"value": @2, @"exp": @"x(2,1)"},
             @{@"value": @0, @"exp": @"let self(yy) = yy"},
             @{@"value": @1, @"exp": @"self(1)"},
             @{@"value": @1, @"exp": @"head([1,2,3])"},
			 ];
}

static void toStream(NSString *string, std::stringstream& stream) {
    std::string cppStr = std::string(string.UTF8String);
    stream << cppStr;
}

static BOOL testStream(std::istream &stream, Runtime &rt, jcVariablePtr expectedValue) {
    try {
        std::vector<jcVariablePtr> output;
        if (rt.evaluate(stream, output)) {
            if (output.size()) {
                jcVariablePtr value = output.front();
                std::cout << value->stringRepresentation() << std::endl;
                JC_ASSERT(value != nullptr && expectedValue != nullptr);
                return value->equal(*expectedValue);
            }
            return YES;
        }
        return NO;
    } catch (jcException exception) {
        std::cerr << exception.getMessage() << std::endl;
        return NO;
    }
}

- (void)testBasicExpressions {
    Runtime rt;
	for (NSDictionary *d in self.inlineExpressions) {
        jcVariablePtr expected = jcVariable::Create([d[@"value"] intValue]);
		NSString *expression = d[@"exp"];

        std::stringstream stream;
        toStream(expression, stream);

        XCTAssert(testStream(stream, rt, expected));
	}
}

- (void)testListHead {
    Runtime rt;

    NSString *program = @"let list = [1, 2, 3]\
                        head(list())\
    ";

    jcVariablePtr expected = jcVariable::Create(1);
    std::stringstream stream;
    toStream(program, stream);

    XCTAssert(testStream(stream, rt, expected));
}

- (void)testListlen {
    Runtime rt;

    NSString *program = @"let list = [1, 2, 3]\
                          len(list())\
    ";

    jcVariablePtr expected = jcVariable::Create(3);
    std::stringstream stream;
    toStream(program, stream);

    XCTAssert(testStream(stream, rt, expected));
}

- (void)testListConcat {
    Runtime rt;

    NSString *program = @"concat([1,2,3], [4, 5])";

    jcVariablePtr expected = jcVariable::Create(
                                                jcCollection({
                                                    jcVariable::Create(1),
                                                    jcVariable::Create(2),
                                                    jcVariable::Create(3),
                                                    jcVariable::Create(4),
                                                    jcVariable::Create(5)
                                                    })
                                                );
    std::stringstream stream;
    toStream(program, stream);

    XCTAssert(testStream(stream, rt, expected));
}

- (void)testReturnList
{
    Runtime rt;

    NSString *program = @"let returnSelf(x, y, z) = [x, y, z] \
                          returnSelf(1, 2, 3) \
    ";

    jcVariablePtr expected = jcVariable::Create(
                                                jcCollection({
                                                    jcVariable::Create(1),
                                                    jcVariable::Create(2),
                                                    jcVariable::Create(3)
                                                })
                                                );
    std::stringstream stream;
    toStream(program, stream);

    XCTAssert(testStream(stream, rt, expected));
}

- (void)testBasicClosure
{
    Runtime rt;

    NSString *program = @"let callit(fn) = fn()\
                          let returnNum(num) = callit({() = num})\
                          returnNum(4)\
    ";

    jcVariablePtr expected = jcVariable::Create(4);

    std::stringstream stream;
    toStream(program, stream);

    XCTAssert(testStream(stream, rt, expected));
}

- (void)testPassFunction
{
    Runtime rt;

    NSString *program = @"let callit(fn) = fn()\
    let four = 4 \
    callit(four) \
    ";

    jcVariablePtr expected = jcVariable::Create(4);

    std::stringstream stream;
    toStream(program, stream);

    XCTAssert(testStream(stream, rt, expected));
}

- (void)testQuicksort
{
    std::ifstream quickSortFile(std::string([_testDirectory UTF8String]) + "tests/qs.jc");

    Runtime rt;
    std::vector<jcVariablePtr> output;
    XCTAssert(rt.evaluate(quickSortFile, output));
    XCTAssert(output.size() == 0);

    auto intVecToPtr = [](const std::vector<int> &vector) -> jcVariablePtr {
        std::vector<jcVariablePtr> ptrs;
        for (int v : vector) {
            ptrs.push_back(jcVariable::Create(v));
        }

        jcCollection collection(ptrs);

        return jcVariable::Create(collection);
    };

    std::vector<int> valuesToSort;
    while (valuesToSort.size() < 1000) {
        valuesToSort.push_back(arc4random() % 1000);
    }

    std::string program = "qs(" + intVecToPtr(valuesToSort)->stringRepresentation() + ")";

    std::sort(valuesToSort.begin(), valuesToSort.end(), [](int x, int y) {
        return x < y;
    });

    std::stringstream stream;
    stream << program;

    XCTAssert(testStream(stream, rt, intVecToPtr(valuesToSort)));
}

- (void)testInlineClosure
{
    Runtime rt;

    NSString *program = @"{() = 555}()";

    jcVariablePtr expected = jcVariable::Create(555);

    std::stringstream stream;
    toStream(program, stream);

    XCTAssert(testStream(stream, rt, expected));
}

- (void)testFunctionCallString
{
    Runtime rt;

    NSString *program = @"let callit(fn) = fn\
    let four = 4 \
    callit(four)() \
    ";

    jcVariablePtr expected = jcVariable::Create(4);

    std::stringstream stream;
    toStream(program, stream);

    XCTAssert(testStream(stream, rt, expected));
}



@end
