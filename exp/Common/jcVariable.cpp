//  jcVariable.cpp

#include "jcVariable.hpp"

jcVariablePtr jcVariable::Create()
{
    return std::make_shared<jcVariable>(jcVariable());
}

jcVariablePtr jcVariable::Create(std::string value)
{
    auto me = jcVariable::Create();
    me->set_String(value);
    return me;
}

jcVariablePtr jcVariable::Create(int value)
{
    auto me = jcVariable::Create();
    me->set_Int(value);
    return me;
}

jcVariablePtr jcVariable::Create(const jcCollection& collection)
{
    auto me = jcVariable::Create();
    me->set_Collection(collection);
    return me;
}

jcVariable::jcVariable()
    : mCurrentType(TypeNone)
{
}

jcVariable::~jcVariable()
{
    willSet();
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

jcCollection* jcVariable::asCollection() const
{
    if (mCurrentType == TypeCollection) {
        return mData.collection;
    }
    return nullptr;
}

void jcVariable::set_String(const std::string& str)
{
    willSet();

    mData.mStr = new char[str.length() + 1];
    strncpy(mData.mStr, str.c_str(), str.length());
    mData.mStr[str.length()] = '\0';
    mCurrentType = TypeString;
}

void jcVariable::set_Int(const int val)
{
    willSet();
    mData.mInt = val;
    mCurrentType = TypeInt;
}

void jcVariable::set_Collection(const jcCollection& collection)
{
    willSet();
    mData.collection = new jcCollection;
    for (int i = 0; i < collection.size(); i++) {
        mData.collection->push(collection.at(i));
    }
    mCurrentType = TypeCollection;
}

void jcVariable::willSet()
{
    if (mCurrentType == TypeString) {
        delete[] mData.mStr;
    }
    if (mCurrentType == TypeCollection) {
        delete mData.collection; 
    }
}
std::string jcVariable::stringRepresentation() const {
    if (getType() == TypeInt) {
        return std::to_string(asInt());
    } else if (getType() == TypeString) {
        return asString();
    } else if (getType() == TypeCollection) {
        std::string rep = "[";
        if (asCollection() != nullptr) {
            for (int i = 0; i < (int)asCollection()->size(); i++) {
                rep += asCollection()->at(i)->stringRepresentation();
                if (i != (int)asCollection()->size() - 1) {
                    rep += ", ";
                }
            }
        }
        return rep + "]";
    }
    return "";
}

bool jcVariable::equal(const jcVariable &other) const
{
    if (getType() != other.getType()) {
        return false;
    }

    switch (getType()) {
        case TypeInt:
            return asInt() == other.asInt();
        case TypeString:
            return asString() == other.asString();
        case TypeCollection: {
            auto collection1 = asCollection();
            auto collection2 = other.asCollection();

            if (collection1 != nullptr && collection2 != nullptr) {
                return collection1->equal(*collection2);
            }
            return collection1 == collection2;
        }
        default:
            return false;
    }
}

/**
 Mutable Variable
 */

jcMutableVariablePtr jcMutableVariable::Create(jcVariable &other)
{
    jcMutableVariablePtr newVar = jcMutableVariable::Create();
    if (other.getType() == TypeInt) {
        newVar->setInt(other.asInt());
    } else if (other.getType() == TypeString) {
        newVar->setString(other.asString());
    } else if (other.getType() == TypeCollection) {
        auto collection = other.asCollection();
        if (collection) {
            newVar->setCollection(*collection);
        }
    }
    return newVar;
}

jcMutableVariablePtr jcMutableVariable::Create()
{
    return std::make_shared<jcMutableVariable>(jcMutableVariable());
}

void jcMutableVariable::setString(const std::string& str)
{
    set_String(str);
}

void jcMutableVariable::setInt(const int val)
{
    set_Int(val);
}

void jcMutableVariable::setCollection(const jcCollection& collection)
{
    set_Collection(collection);
}
