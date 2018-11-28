#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "jc.h"

class jcCollection
{
public:
    jcCollection();
    jcCollection(int size);
    jcCollection(const jcCollection &other);
    jcCollection(const std::vector<jcVariablePtr> &list);

    void push(const jcVariablePtr var);

    size_t size() const;
    bool isEmpty() const;

    jcVariablePtr head() const;
    jcCollection tail();

    jcCollection concat(const jcCollection &other) const;

    void forEach(std::function<void(jcVariablePtr)> callback) const;

    bool equal(const jcCollection &other) const;

private:
    std::vector<jcVariablePtr> mItems;
};
