//  units.m

#import <XCTest/XCTest.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

#include "Runtime.hpp"
#include "jcVariable.hpp"
#include "jcUtils.hpp"
#include "jcArray.hpp"
#include "jcList.hpp"

#include "utils.h"


class AnswerExpression {
public:
    jcVariablePtr answer;
    std::string expression;

    AnswerExpression(jcVariablePtr answer,
                     std::string expression) : answer(answer), expression(expression)
    {
    }
};

@interface units : XCTestCase

@end

@implementation units {
    NSString *_testDirectory;
}

- (void)setUp
{
    _testDirectory = [[[NSProcessInfo processInfo] environment] objectForKey:@"TEST_DIR"];
}

static std::vector<AnswerExpression> getBasicExpressions()
{
    return {
        AnswerExpression(jcVariable::Create(2), "1 + 1"),
        AnswerExpression(jcVariable::Create(11), "5 * 2 + 1"),
        AnswerExpression(jcVariable::Create(11), "(5 * 2) + 1"),
        AnswerExpression(jcVariable::Create(2), "2 / 1"),
        AnswerExpression(jcVariable::Create(20), "(5 - 1) * 5"),
        AnswerExpression(jcVariable::Create(99), "(5 - 1) * 5 * 5 - 1"),
        AnswerExpression(jcVariable::Create(0), "let add(x123,y) = x123 + y"),
        AnswerExpression(jcVariable::Create(4), "add(add(1,1), add(1,1))"),
        AnswerExpression(jcVariable::Create(0), "2 > 3"),
        AnswerExpression(jcVariable::Create(1), "2 < 3"),
        AnswerExpression(jcVariable::Create(0), "2 >= 3"),
        AnswerExpression(jcVariable::Create(1), "2 >= 1"),
        AnswerExpression(jcVariable::Create(1), "2 <= 3"),
        AnswerExpression(jcVariable::Create(1), "2 <= 2"),
        AnswerExpression(jcVariable::Create(1), "2 == 2"),
        AnswerExpression(jcVariable::Create(0), "!(2 == 2)"),
        AnswerExpression(jcVariable::Create(100), "-(10 * -10)"),
        AnswerExpression(jcVariable::Create(1), "(1 == 2) == 0"),
        AnswerExpression(jcVariable::Create(0), "let x(a,b) | a > b = a | else = b"),
        AnswerExpression(jcVariable::Create(2), "x(2,1)"),
        AnswerExpression(jcVariable::Create(0), "let self(yy) = yy"),
        AnswerExpression(jcVariable::Create(1), "self(1)"),
        AnswerExpression(jcVariable::Create(1), "head([1,2,3])"),
        AnswerExpression(jcVariable::Create(900), "2 > 3 ? (100 > 99 ? 1000 : 101) : 900")

    };
}

static BOOL testStream(std::istream &stream, Runtime &rt, jcVariablePtr expectedValue) {
    try {
        std::vector<jcVariablePtr> output;
        bool result = false;

//        double timeElapsed = jc::measureElapsedTime([&result, &rt, &stream, &output]() {
            result = rt.evaluateREPL(stream, output);
//        });

//        std::cout << "Execution time: " << timeElapsed << std::endl;

        if (result) {
            if (output.size()) {
                jcVariablePtr value = output.front();
//                std::cout << value->stringRepresentation() << std::endl;
//                std::cout << expectedValue->stringRepresentation() << std::endl;

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
    for (AnswerExpression object : getBasicExpressions()) {
        jcVariablePtr expected = object.answer;
        std::string expression = object.expression;

        std::stringstream stream;
        stream << expression;

        XCTAssert(testStream(stream, rt, expected));
	}
}

- (void)testListHead {
    Runtime rt;

    std::string program = "let list = [1, 2, 3]\
                        head(list())\
    ";

    jcVariablePtr expected = jcVariable::Create(1);
    std::stringstream stream;
    stream << program;

    XCTAssert(testStream(stream, rt, expected));
}

- (void)testListlen {
    Runtime rt;

    std::string program = "let list = [1, 2, 3]\
                          len(list())\
    ";

    jcVariablePtr expected = jcVariable::Create(3);
    std::stringstream stream;
    stream << program;

    XCTAssert(testStream(stream, rt, expected));
}

- (void)testListConcat {
    Runtime rt;

    std::string program = "[1,2,3] ++ [4, 5]";
    std::vector<int> elems = {1, 2, 3, 4, 5};

    jcListPtr list = TestUtils::buildList(elems);

    jcVariablePtr expected = jcVariable::Create(list);
    std::stringstream stream;
    stream << program;

    XCTAssert(testStream(stream, rt, expected));
}

- (void)testReturnList
{
    Runtime rt;

    std::string program = "let returnSelf(x, y, z) = [x, y, z] \
                          returnSelf(1, 2, 3) \
    ";
    std::vector<int> elems = { 1, 2, 3 };
    jcListPtr collection = TestUtils::buildList(elems);

    jcVariablePtr expected = jcVariable::Create(collection);
    std::stringstream stream;
    stream << program;

    XCTAssert(testStream(stream, rt, expected));
}

- (void)testBasicClosure
{
    Runtime rt;

    std::string program = "let callit(fn) = fn()\
                          let returnNum(num) = callit({() = num})\
                          returnNum(4)\
    ";

    jcVariablePtr expected = jcVariable::Create(4);

    std::stringstream stream;
    stream << program;

    XCTAssert(testStream(stream, rt, expected));
}

- (void)testPassFunction
{
    Runtime rt;

    std::string program = "let callit(fn) = fn()\
    let four = 4 \
    callit(four) \
    ";

    jcVariablePtr expected = jcVariable::Create(4);

    std::stringstream stream;
    stream << program;

    XCTAssert(testStream(stream, rt, expected));
}

- (void)testIndex
{
    Runtime rt;

    std::string program = "let array() = [1,2,3]\
    array()[0] + array()[1] + array()[2]\
    ";

    jcVariablePtr expected = jcVariable::Create(6);

    std::stringstream stream;
    stream << program;

    XCTAssert(testStream(stream, rt, expected));
}

- (void)testQuicksort
{
    std::ifstream quickSortFile(std::string([_testDirectory UTF8String]) + "tests/qs.jc");

    Runtime rt;
    std::vector<jcVariablePtr> output;
    XCTAssert(rt.evaluateREPL(quickSortFile, output));
    XCTAssert(output.size() == 0);

    const int NUM_ITEMS = 10000;

    std::vector<int> valuesToSort;
    while (valuesToSort.size() < NUM_ITEMS) {
        valuesToSort.push_back(arc4random() % NUM_ITEMS);
    }

    std::string program = "qs(" + TestUtils::buildListVariable(valuesToSort)->stringRepresentation() + ")";

    std::sort(valuesToSort.begin(), valuesToSort.end(), [](int x, int y) {
        return x < y;
    });

    std::stringstream stream;
    stream << program;

    XCTAssert(testStream(stream, rt, TestUtils::buildListVariable(valuesToSort)));
}

- (void)testInlineClosure
{
    Runtime rt;

    std::string program = "{() = 555}()";

    jcVariablePtr expected = jcVariable::Create(555);

    std::stringstream stream;
    stream << program;

    XCTAssert(testStream(stream, rt, expected));
}

- (void)testFunctionCallString
{
    Runtime rt;

    std::string program = "let callit(fn) = fn\
    let four = 4 \
    callit(four)() \
    ";

    jcVariablePtr expected = jcVariable::Create(4);

    std::stringstream stream;
    stream << program;

    XCTAssert(testStream(stream, rt, expected));
}

- (void)testSlice1
{
    Runtime rt;

    std::string program = "[1,2,3][1:]";

    jcVariablePtr expected = TestUtils::buildListVariable<int>({2, 3});

    std::stringstream stream;
    stream << program;

    XCTAssert(testStream(stream, rt, expected));
}

- (void)testSlice2
{
    Runtime rt;

    std::string program = "[1,2,3][:2]";

    jcVariablePtr expected = TestUtils::buildListVariable<int>({1, 2});

    std::stringstream stream;
    stream << program;

    XCTAssert(testStream(stream, rt, expected));
}

- (void)testSlice3
{
    Runtime rt;

    std::string program = "[1,2,3][:]";

    jcVariablePtr expected = TestUtils::buildListVariable<int>({1, 2, 3});

    std::stringstream stream;
    stream << program;

    XCTAssert(testStream(stream, rt, expected));
}

- (void)testComment
{
    Runtime rt;

    std::string program = "let callit(fn) = fn\
    # this be a comment, dont mind me \
    let four = 4 \
    callit(four)() \
    ";

    jcVariablePtr expected = jcVariable::Create(4);

    std::stringstream stream;
    stream << program;

    XCTAssert(testStream(stream, rt, expected));
}

@end
