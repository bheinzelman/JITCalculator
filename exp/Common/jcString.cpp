//  jcString.cpp

#include "jcString.hpp"

jcString::jcString(const std::string &value, Context ctx)
: mData(value), mCtx(ctx)
{
}

jcStringPtr jcString::Create(const std::string &value, Context ctx)
{
    return std::make_shared<jcString>(value, ctx);
}

const std::string& jcString::asStdString() const
{
    return mData;
}

bool jcString::equal(const jcString &other) const
{
    return asStdString() == other.asStdString();
}

/**
 jcCollection methods
 */

size_t jcString::size() const
{
    return mData.size();
}

jcVariablePtr jcString::head() const
{
    JC_ASSERT(size() > 0);
    return jcVariable::Create(mData[0]);
}

jcCollection* jcString::tail() const
{
    return slice(1, (int)size());
}

jcCollection* jcString::concat(const jcCollection &other) const
{
    JC_ASSERT(getType() == other.getType());

    const jcString& otherString = dynamic_cast<const jcString&>(other);

    std::string str = std::string(asStdString()) + std::string(otherString.asStdString());

    return new jcString(str, getContext());
}

void jcString::forEach(std::function<void(jcVariablePtr)> callback) const
{
    for (auto it = mData.begin(); it != mData.end(); ++it) {
        callback(jcVariable::Create(*it));
    }
}

jcCollection* jcString::slice(int startIdx, int endIdx) const
{
    JC_ASSERT(startIdx >= 0 && endIdx <= mData.size() && startIdx <= endIdx);
    return new jcString(std::string(mData.begin() + startIdx, mData.begin() + endIdx), getContext());
}

jcVariable::Type jcString::getType() const
{
    return jcVariable::TypeString;
}

