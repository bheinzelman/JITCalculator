//  jcUnits.mm

/**
 Tests for JC Data Structures..
 */

#import <XCTest/XCTest.h>

#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>

#include "jc.h"
#include "jcVariable.hpp"
#include "jcString.hpp"
#include "jcList.hpp"

#include "utils.h"

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
    jcStringPtr string = jcString::Create(theString, jcString::StringContextValue);
    jcString otherString = jcString(theString, jcString::StringContextValue);

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

- (void)testJcList
{
    std::vector<int> listVals = {1, 2, 3, 4};
    std::vector<int> otherVals = {1, 2, 3};
    std::vector<int> concatVals = {1, 2, 3, 4, 1, 2, 3, 4};
    {
        // Test size

        // List with stuff.
        jcListPtr list = TestUtils::buildList(listVals);
        XCTAssert(list->size() == listVals.size());

        jcList emptyList;
        XCTAssert(emptyList.size() == 0);
        XCTAssert(emptyList.isEmpty() == true);
    }

    {
        // Test concat
        jcListPtr list1 = TestUtils::buildList(listVals);
        jcListPtr list2 = TestUtils::buildList(listVals);

        std::unique_ptr<jcCollection> concated = std::unique_ptr<jcCollection>(list1->concat(*list2));
        XCTAssert(concated->size() == concatVals.size());
        int idx = 0;
        concated->forEach([self, &idx, &concated, &concatVals](jcVariablePtr item) {
            XCTAssert(item->asInt() == concatVals[idx++]);
        });
    }

    {
        // Test slice..

        jcListPtr bigList = TestUtils::buildList(concatVals);
        std::unique_ptr<jcCollection> slice = std::unique_ptr<jcCollection>(bigList->slice(2, 5));
        XCTAssert(slice->size() == 3);
        std::vector<int> vectorSlice = std::vector<int>(concatVals.begin() + 2, concatVals.begin() + 5);

        int idx = 0;
        slice->forEach([self, &slice, &vectorSlice, &idx](jcVariablePtr item) {
            XCTAssert(item->asInt() == vectorSlice[idx++]);
        });

        std::unique_ptr<jcCollection> slice2 = std::unique_ptr<jcCollection>(slice->slice(0, 2));
        std::vector<int> vectorSlice2 = std::vector<int>(vectorSlice.begin(), vectorSlice.begin() + 2);

        idx = 0;
        XCTAssert(slice2->size() == 2);
        slice2->forEach([self, &slice, &vectorSlice2, &idx](jcVariablePtr item) {
            XCTAssert(item->asInt() == vectorSlice2[idx++]);
        });
    }

    {
        // equal
        jcListPtr list1 = TestUtils::buildList(listVals);
        jcListPtr list2 = TestUtils::buildList(listVals);
        XCTAssert(list1->equal(*list2));

        jcListPtr list3 = TestUtils::buildList(otherVals);
        XCTAssert(list1->equal(*list3) == false);
    }

    {
        // Test cons
        jcListPtr list1 = TestUtils::buildList(listVals);
        std::unique_ptr<jcList> newList = std::unique_ptr<jcList>(list1->cons(jcVariable::Create(100)));
        XCTAssert(newList->size() == list1->size() + 1);
    }
}


@end
