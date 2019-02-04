//  jcArray.cpp

#include "jcArray.hpp"
#include "jcVariable.hpp"

jcArray::jcArray()
{
}

jcArray::jcArray(int size)
{
    mItems = std::make_shared<std::vector<jcVariablePtr>>();
    mItems->reserve(size);
}

jcArray::jcArray(const std::vector<jcVariablePtr> &list)
{
    mItems = std::make_shared<std::vector<jcVariablePtr>>(list.begin(), list.end());
    mStartIndex = 0;
    mEndIndex = (int)list.size();
}

jcArray::jcArray(const jcArray &other)
{
    mItems = std::make_shared<std::vector<jcVariablePtr>>();
    mItems->reserve(other.size());
    other.forEach([this](jcVariablePtr item) {
        mItems->push_back(item);
    });
}

jcArray::jcArray(const std::shared_ptr<std::vector<jcVariablePtr>> &items, int startIdx, int endIdx)
    : mItems(items), mStartIndex(startIdx), mEndIndex(endIdx)
{
}

jcVariablePtr jcArray::head() const
{
    JC_ASSERT(size());
    return *startIt();
}

jcCollection* jcArray::tail() const
{
    return slice(1, (int)size());
}

jcCollection* jcArray::slice(int startIdx, int endIdx) const
{
    JC_ASSERT(startIdx >= 0 && endIdx <= mItems->size() && startIdx <= endIdx);
    return new jcArray(mItems, startIdx, endIdx);
}

size_t jcArray::size() const
{
    return mEndIndex - mStartIndex;
}

jcCollection* jcArray::concat(const jcCollection &other) const
{
    JC_ASSERT(getType() == other.getType());
    
    jcMutableArray* newArray = new jcMutableArray((int)size() + (int)other.size());

    forEach([&newArray](jcVariablePtr value) {
        newArray->push(value);
    });
    other.forEach([&newArray](jcVariablePtr value) {
        newArray->push(value);
    });

    return newArray;
}

void jcArray::forEach(std::function<void(jcVariablePtr&)> callback) const
{
    for (int i = mStartIndex; i < mEndIndex; i++) {
        callback((*mItems)[i]);
    }
}

bool jcArray::equal(const jcArray &other) const
{
    if (size() != other.size()) {
        return false;
    }

    auto it1 = startIt();
    auto it2 = other.startIt();

    for (int i = 0; i < (int)size(); i++) {
        auto elem1 = *it1;
        auto elem2 = *it2;

        if (elem1 == nullptr && elem2 == nullptr) {
            return true;
        }
        
        if (elem1 == nullptr ^ elem2 == nullptr) {
            return false;
        }

        if (elem1->equal(*elem2) == false) {
            return false;
        }
        ++it1;
        ++it2;
    }
    return true;
}

std::vector<jcVariablePtr>::iterator jcArray::startIt() const
{
    return mItems->begin() + mStartIndex;
}

std::vector<jcVariablePtr>::iterator jcArray::endIt() const
{
    return mItems->begin() + mEndIndex;
}

void jcArray::pushBack(const jcVariablePtr &element)
{
    mItems->push_back(element);
    mEndIndex++;
}

/// jcMutableArray

jcMutableArray::jcMutableArray() : jcArray(0)
{
}

jcMutableArray::jcMutableArray(int size) : jcArray(size)
{
}

jcMutableArray::jcMutableArray(const jcArray &other) : jcArray(other)
{
}

jcMutableArray::jcMutableArray(const std::vector<jcVariablePtr> &list) : jcArray(list)
{
}

void jcMutableArray::push(const jcVariablePtr &var)
{
    pushBack(var);
}
