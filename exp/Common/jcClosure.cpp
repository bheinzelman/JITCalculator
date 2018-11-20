//  jcClosure.cpp

#include "jcClosure.hpp"

#include <set>
#include <functional>

jcClosure::jcClosure(const std::string &name, std::map<std::string, jcVariablePtr> &scope)
: mName(name),
  mScope(scope)
{
}

std::string jcClosure::name() const
{
    return mName;
}

std::map<std::string, jcVariablePtr> jcClosure::scope() const
{
    return mScope;
}

bool jcClosure::equal(const jcClosure &other) const
{
    auto getKeys = [](std::map<std::string, jcVariablePtr> scope) -> std::set<std::string> {
        std::set<std::string> keys;
        for (auto pair : scope) {
            keys.insert(pair.first);
        }
        return keys;
    };
    return getKeys(scope()) == getKeys(other.scope()) && name() == other.name();
}
