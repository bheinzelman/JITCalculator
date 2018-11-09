//  jcCollection.cpp

#include "jcCollection.hpp"
#include "jcVariable.hpp"

jcCollection::jcCollection()
{
}

jcCollection::jcCollection(const std::vector<jcVariablePtr> &vector) : mItems(vector)
{
}

jcCollectionPtr jcCollection::Create()
{
    return std::make_shared<jcCollection>();
}

jcCollectionPtr jcCollection::Create(const std::vector<std::shared_ptr<jcVariable>> &vector)
{
    return std::make_shared<jcCollection>(jcCollection(vector));
}

void jcCollection::push(const jcVariablePtr var)
{
    mItems.push_back(var);
}

jcVariablePtr jcCollection::at(int index) const
{
    JC_ASSERT(index < mItems.size());
    return mItems[index];
}

size_t jcCollection::size() const
{
    return mItems.size();
}

jcCollection jcCollection::concat(const jcCollection &other)
{
    std::vector<jcVariablePtr> newCollection;
    for (int i = 0; i < (int)size(); i++) {
        newCollection.push_back(at(i));
    }
    for (int i = 0; i < other.size(); i++) {
        newCollection.push_back(other.at(i));
    }
    return jcCollection(newCollection);
}
