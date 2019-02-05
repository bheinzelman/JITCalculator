#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "jc.h"
#include "jcCollection.h"

class jcArray : public jcCollection
{
public:
    jcArray();
    jcArray(int size);
    jcArray(const jcArray &other);
    jcArray(const std::vector<jcVariablePtr> &list);

    size_t size() const override;

    /**
     You own this sucker, be careful
     */
    jcCollection* concat(const jcCollection &other) const override;

    void forEach(std::function<void(jcVariablePtr&)> callback) const override;

    bool equal(const jcArray &other) const;

    jcCollection* slice(int startIdx, int endIdx) const override;

    jcVariablePtr at(int index) const override;

    jcVariable::Type getType() const override
    {
        return jcVariable::TypeArray;
    }

    /// Protected Methods
protected:
    void pushBack(const jcVariablePtr &element);

    /// Private Methods
private:
    jcArray(const std::shared_ptr<std::vector<jcVariablePtr>> &items,
            int startIdx,
            int endIdx);

    std::vector<jcVariablePtr>::iterator startIt() const;
    std::vector<jcVariablePtr>::iterator endIt() const;

    /// Private Properties
private:
    std::shared_ptr<std::vector<jcVariablePtr>> mItems;
    int mStartIndex=0;
    int mEndIndex=0;
};

class jcMutableArray : public jcArray {
public:
    jcMutableArray();
    jcMutableArray(int size);
    jcMutableArray(const jcArray &other);
    jcMutableArray(const std::vector<jcVariablePtr> &list);

    void push(const jcVariablePtr &var);
};
