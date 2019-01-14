//  jcUnits.mm

/**
 Tests for JC Data Structures..
 */

#import <XCTest/XCTest.h>

#include <memory>

#include "jc.h"
#include "jcVariable.hpp"
#include "jcString.hpp"

@interface jcUnits : XCTestCase
@end

@implementation jcUnits

- (void)testJcChar {
    jcVariablePtr charObject = jcVariable::Create('A');

    XCTAssert(charObject->getType() == jcVariable::TypeChar);
    XCTAssert(charObject->asChar() == 'A');

    jcMutableVariable other;
    other.setChar('A');

    XCTAssert(charObject->equal(other));

    XCTAssert(charObject->stringRepresentation() == "'A'");
}


- (void)testJcString
{
    std::string theString = "Hello, World!";
    jcStringPtr string = jcString::Create(theString);
    jcString otherString = jcString(theString);

    jcMutableVariable head;
    head.setChar('H');

    jcMutableVariable tail;
    tail.setString(std::string(theString.begin() + 1, theString.end()));

    XCTAssert(string->asStdString() == theString);
    XCTAssert(string->size() == theString.size());
    XCTAssert(string->equal(otherString));
    XCTAssert(string->head()->equal(head));

    jcVariablePtr myTail = jcVariable::CreateFromCollection(std::shared_ptr<jcCollection>(string->tail()));
    XCTAssert(myTail->equal(tail));

    jcVariablePtr myConcat = jcVariable::CreateFromCollection(std::shared_ptr<jcCollection>(string->concat(otherString)));
    XCTAssert(myConcat->equal(*jcVariable::Create(theString + theString)));
}


@end
