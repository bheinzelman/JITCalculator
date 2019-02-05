//  jcList.hpp

#pragma once

#include "jc.h"
#include "jcCollection.h"

#include <string>
#include <memory>
#include <vector>


class jcList : public jcCollection
{
private:
    struct list;

    jcList(const std::shared_ptr<list>& list, int size)
        : mList(list), mSize(size)
    {
    }
public:
    jcList()
    {
        mList = nullptr;
        mSize = 0;
    }

    // for unit tests only
//    static jcListPtr buildList(const std::vector<jcVariablePtr> &items);

    jcList(const jcList& other);

    jcList* cons(const jcVariablePtr &value) const;

    bool equal(const jcList &other) const;

    /**
     jcCollection methods
     */
    size_t size() const override;
    bool isEmpty() const override;
    jcCollection* concat(const jcCollection &other) const override;
    void forEach(std::function<void(jcVariablePtr&)> callback) const override;
    jcCollection* slice(int startIdx, int endIdx) const override;
    jcVariablePtr at(int index) const override;
    jcVariable::Type getType() const override;
private:

    struct list {
        list(const jcVariablePtr &item, const std::shared_ptr<list> &tail)
            : item(item), tail(tail)
        {
            endNode = tail ? tail->endNode : nullptr;
        }

        jcVariablePtr item;
        std::shared_ptr<list> tail;
        std::shared_ptr<list> endNode;
    };

    std::shared_ptr<list> mList;
    int mSize=0;
};
