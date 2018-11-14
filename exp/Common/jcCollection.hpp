#pragma once

#include <vector>
#include <memory>

#include "jc.h"

class jcCollection
{
public:
    jcCollection();
    jcCollection(const std::vector<jcVariablePtr> &vector);

    static jcCollectionPtr Create();
    static jcCollectionPtr Create(const std::vector<jcVariablePtr> &vector);

    void push(const jcVariablePtr var);
    jcVariablePtr at(int index) const;
    size_t size() const;

    jcCollection concat(const jcCollection &other) const;

    bool equal(const jcCollection &other) const;

private:
    std::vector<jcVariablePtr> mItems;

};
