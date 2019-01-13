//  jcUnits.mm

/**
 Tests for JC Data Structures..
 */

#import <XCTest/XCTest.h>

#include "jc.h"
#include "jcVariable.hpp"

@interface jcUnits : XCTestCase
@end

@implementation jcUnits

-(void)testJcChar {
    jcVariablePtr charObject = jcVariable::Create('A');

    XCTAssert(charObject->getType() == jcVariable::TypeChar);
    XCTAssert(charObject->asChar() == 'A');

    jcMutableVariable other;
    other.setChar('A');

    XCTAssert(charObject->equal(other));

    XCTAssert(charObject->stringRepresentation() == "'A'");
}

@end
