//  jcList.cpp

#include "jcList.hpp"

jcList::jcList(const jcList& other)
{
    if (other.isEmpty()) {
        mList = std::make_shared<list>(nullptr, nullptr);
        return;
    }
    std::shared_ptr<list> tmp = nullptr;
    other.forEach([this, &other, &tmp](jcVariablePtr item) {
        if (mList == nullptr) {
            mList = std::make_shared<list>(item, nullptr);
            tmp = mList;
        } else {
            tmp->tail = std::make_shared<list>(item, nullptr);
            tmp = tmp->tail;
        }
    });
    mList->endNode = tmp;
    mSize = (int)other.size();
}

jcListPtr jcList::buildList(const std::vector<jcVariablePtr> &items)
{
    jcListPtr newList = std::make_shared<jcList>();
    std::vector<jcVariablePtr> cpy = std::vector<jcVariablePtr>(items);
    std::reverse(std::begin(cpy), std::end(cpy));
    for (auto item : cpy) {
        auto newCollection = std::shared_ptr<jcCollection>(newList->cons(item));
        newList = std::static_pointer_cast<jcList>(newCollection);
    }
    return newList;
}

jcList* jcList::cons(const jcVariablePtr &value) const
{
    jcList *newList = new jcList;
    newList->mList = std::make_shared<list>(value, mList);
    newList->mSize = (int)(size() + 1);
    if (mList == nullptr) {
        newList->mList->endNode = newList->mList;
    }
    return newList;
}

bool jcList::equal(const jcList &other) const
{
    // this could be removed but for now it makes things simpler.
    if (size() != other.size()) return false;

    std::shared_ptr<list> tmp = mList;
    std::shared_ptr<list> end = mList->endNode;
    std::shared_ptr<list> otherTmp = other.mList;

    while (tmp != end->tail) {
        if (tmp->item->equal(*otherTmp->item) == false) {
            return false;
        }
        tmp = tmp->tail;
        otherTmp = otherTmp->tail;
    }
    return true;
}

bool jcList::isEmpty() const
{
    return mList == nullptr;
}

size_t jcList::size() const
{
    return mSize;
}

jcVariablePtr jcList::head() const
{
    JC_ASSERT(mList != nullptr);
    return mList->item;;
}

jcCollection* jcList::concat(const jcCollection &other) const
{
    JC_ASSERT(getType() == other.getType());
    const jcList& otherList = dynamic_cast<const jcList&>(other);

    jcList myCopy = jcList(*this);
    jcList otherCopy = jcList(otherList);

    if (other.isEmpty()) {
        return new jcList(myCopy.mList, (int)myCopy.size());
    } else if (isEmpty()) {
        return new jcList(otherCopy.mList, (int)otherCopy.size());
    }

    myCopy.mList->endNode->tail = otherCopy.mList;
    myCopy.mList->endNode = otherCopy.mList->endNode;

    return new jcList(myCopy.mList, (int)(myCopy.size() + otherCopy.size()));
}

void jcList::forEach(std::function<void(jcVariablePtr&)> callback) const
{
    if (mList == nullptr) return;
    JC_ASSERT(mList->endNode != nullptr);

    std::shared_ptr<list> tmp = mList;
    std::shared_ptr<list> end = mList->endNode;
    while (tmp != end->tail) {
        callback(tmp->item);
        tmp = tmp->tail;
    }
}

jcCollection* jcList::slice(int startIdx, int endIdx) const
{
    if (mList == nullptr) return nullptr;
    JC_ASSERT(mList->endNode != nullptr);
    JC_ASSERT(startIdx >= 0 && endIdx >= startIdx && endIdx <= size());

    if (startIdx == endIdx) return new jcList;

    std::shared_ptr<list> startNode = nullptr;
    std::shared_ptr<list> endNode = nullptr;

    if (endIdx == size()) endNode = mList->endNode;

    int idx = 0;

    std::shared_ptr<list> tmp = mList;
    while (tmp != mList->endNode->tail) {
        if (idx == startIdx) {
            startNode = tmp;
        }
        if (idx == endIdx - 1) {
            endNode = tmp;
        }

        if (startNode && endNode) break;

        tmp = tmp->tail;
        idx++;
    }

    JC_ASSERT(startNode && endNode);

    std::shared_ptr<list> newList = std::make_shared<list>(startNode->item, startNode->tail);
    newList->endNode = endNode;

    return new jcList(newList, endIdx - startIdx);
}

jcVariable::Type jcList::getType() const
{
    return jcVariable::TypeList;
}
