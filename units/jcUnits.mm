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

std::shared_ptr<jcList> buildList(std::vector<int> listVals)
{
    std::reverse(std::begin(listVals), std::end(listVals));
    jcListPtr list = std::make_shared<jcList>();
    for (int i : listVals) {
        list = std::shared_ptr<jcList>(list->cons(jcVariable::Create(i)));
    }
    return list;
}

- (void)testJcList
{
    std::vector<int> listVals = {1, 2, 3, 4};
    std::vector<int> otherVals = {1, 2, 3};
    std::vector<int> concatVals = {1, 2, 3, 4, 1, 2, 3, 4};
    {
        // Test size

        // List with stuff.
        jcListPtr list = buildList(listVals);
        XCTAssert(list->size() == listVals.size());

        jcList emptyList;
        XCTAssert(emptyList.size() == 0);
        XCTAssert(emptyList.isEmpty() == true);
    }

    {
        // Test concat
        jcListPtr list1 = buildList(listVals);
        jcListPtr list2 = buildList(listVals);

        std::unique_ptr<jcCollection> concated = std::unique_ptr<jcCollection>(list1->concat(*list2));
        XCTAssert(concated->size() == concatVals.size());
        int idx = 0;
        concated->forEach([self, &idx, &concated, &concatVals](jcVariablePtr item) {
            XCTAssert(item->asInt() == concatVals[idx++]);
        });
    }

    {
        // Test slice..

        jcListPtr bigList = buildList(concatVals);
        std::unique_ptr<jcCollection> slice = std::unique_ptr<jcCollection>(bigList->slice(2, 5));
        std::vector<int> vectorSlice = std::vector<int>(concatVals.begin() + 2, concatVals.begin() + 5);

        int idx = 0;
        slice->forEach([self, &slice, &vectorSlice, &idx](jcVariablePtr item) {
            std::cout << item->asInt() << std::endl;
            XCTAssert(item->asInt() == vectorSlice[idx++]);
        });
    }

    {
        // equal
        jcListPtr list1 = buildList(listVals);
        jcListPtr list2 = buildList(listVals);
        XCTAssert(list1->equal(*list2));

        jcListPtr list3 = buildList(otherVals);
        XCTAssert(list1->equal(*list3) == false);

    }
}


@end
