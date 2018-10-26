//  jcVariable.cpp

#include "jcVariable.hpp"

jcVariablePtr jcVariable::Create()
{
    return std::make_shared<jcVariable>(jcVariable());
}

jcVariablePtr jcVariable::Create(std::string value)
{
    auto me = jcVariable::Create();
    me->setString(value);
    return me;
}

jcVariablePtr jcVariable::Create(int value)
{
    auto me = jcVariable::Create();
    me->setInt(value);
    return me;
}

jcVariable::jcVariable()
    : mCurrentType(TypeNone)
{
}

jcVariable::~jcVariable()
{
    if (mCurrentType == TypeString) {
        delete[] mData.mStr;
    }
}

std::string jcVariable::asString() const
{
    if (mCurrentType == TypeString) {
        return std::string(mData.mStr);
    }
    return std::string();
}

int jcVariable::asInt() const
{
    if (mCurrentType == TypeInt) {
        return mData.mInt;
    }
    return 0;
}

void jcVariable::setString(const std::string& str)
{
    willSet();

    mData.mStr = new char[str.length() + 1];
    strncpy(mData.mStr, str.c_str(), str.length());
    mData.mStr[str.length()] = '\0';
    mCurrentType = TypeString;
}

void jcVariable::setInt(const int val)
{
    willSet();
    mData.mInt = val;
    mCurrentType = TypeInt;
}

void jcVariable::willSet()
{
    if (mCurrentType == TypeString) {
        delete[] mData.mStr;
    }
}
