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

    size_t size() const;
    bool isEmpty() const;

    jcVariablePtr head() const;

    /**
     You own this sucker, be careful
     */
    jcCollection* tail() const;
    jcCollection* concat(const jcCollection &other) const;

    void forEach(std::function<void(jcVariablePtr)> callback) const;

    bool equal(const jcCollection &other) const;

protected:
    std::vector<jcVariablePtr> mItems;
    
};

class jcMutableCollection : public jcCollection {
public:
    jcMutableCollection();
    jcMutableCollection(int size);
    jcMutableCollection(const jcCollection &other);
    jcMutableCollection(const std::vector<jcVariablePtr> &list);

    void push(const jcVariablePtr var);
};
