//  jcCollection.cpp

#include "jcCollection.hpp"
#include "jcVariable.hpp"

jcCollection::jcCollection()
{
}

jcCollection::jcCollection(int size)
{
    mItems.reserve(size);
}

jcCollection::jcCollection(const std::vector<jcVariablePtr> &list) : mItems(list)
{
}

jcCollection::jcCollection(const jcCollection &other)
{
    other.forEach([this](jcVariablePtr item) {
        mItems.push_back(item);
    });
}

void jcCollection::push(const jcVariablePtr var)
{
    mItems.push_back(var);
}

jcVariablePtr jcCollection::head() const
{
    JC_ASSERT(mItems.size());
    return mItems.front();
}

jcCollection* jcCollection::tail() const
{
    return new jcCollection(std::vector<jcVariablePtr>(mItems.begin() + 1, mItems.end()));
}

size_t jcCollection::size() const
{
    return mItems.size();
}

bool jcCollection::isEmpty() const
{
    return mItems.size() == 0;
}

jcCollection* jcCollection::concat(const jcCollection &other) const
{
    jcCollection* newCollection = new jcCollection((int)size() + (int)other.size());

    forEach([&newCollection](jcVariablePtr value) {
        newCollection->push(value);
    });
    other.forEach([&newCollection](jcVariablePtr value) {
        newCollection->push(value);
    });

    return newCollection;
}

void jcCollection::forEach(std::function<void(jcVariablePtr)> callback) const
{
    for (auto item : mItems) {
        callback(item);
    }
}

bool jcCollection::equal(const jcCollection &other) const
{
    if (size() != other.size()) {
        return false;
    }

    auto it1 = mItems.begin();
    auto it2 = other.mItems.begin();

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
