//  utils.h

#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "jc.h"

/**
 Utility functions for unit tests
 */

namespace TestUtils {

template<typename T>
std::shared_ptr<jcList> buildList(std::vector<T> listVals)
{
    std::reverse(std::begin(listVals), std::end(listVals));
    jcListPtr list = std::make_shared<jcList>();
    for (T i : listVals) {
        list = std::shared_ptr<jcList>(list->cons(jcVariable::Create(i)));
    }
    return list;
}

template<typename T>
jcVariablePtr buildListVariable(std::vector<T> listVals)
{
    return jcVariable::CreateFromCollection(buildList(listVals));
}

}
