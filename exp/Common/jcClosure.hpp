//  jcClosure.hpp

#pragma once

#include <map>
#include <memory>

#include "jc.h"

/**
 Model to represent a closure. Holds the temporary function name and
 the scope of the closure.
 */
class jcClosure {
public:
    jcClosure(const std::string &funcName, std::map<std::string, jcVariablePtr> &scope);

    std::string name() const;

    std::map<std::string, jcVariablePtr> scope() const;

    bool equal(const jcClosure &other) const;

private:
    std::string mName;
    std::map<std::string, jcVariablePtr> mScope;
};
