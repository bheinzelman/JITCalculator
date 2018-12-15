//  jcVariable.cpp

#include "jcVariable.hpp"

jcVariablePtr jcVariable::Create()
{
    return std::make_shared<jcVariable>();
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

jcVariablePtr jcVariable::Create(const jcCollectionPtr &collection)
{
    auto me = jcVariable::Create();
    me->set_Collection(collection);
    return me;
}

jcVariablePtr jcVariable::Create(const jcClosurePtr &closure)
{
    auto me = jcVariable::Create();
    me->set_Closure(closure);
    return me;
}

jcVariable::jcVariable()
    : mCurrentType(TypeNone)
{
}

jcVariable::~jcVariable()
{
}

std::string jcVariable::asString() const
{
    if (mCurrentType == TypeString) {
        return std::get<std::string>(mData);
    }
    return std::string();
}

int jcVariable::asInt() const
{
    if (mCurrentType == TypeInt) {
        return std::get<int>(mData);
    }
    return 0;
}

jcClosure* jcVariable::asClosureRaw() const
{
    if (mCurrentType == TypeClosure) {
        return std::get<jcClosurePtr>(mData).get();
    }
    return nullptr;
}

jcCollection* jcVariable::asCollectionRaw() const
{
    if (mCurrentType == TypeCollection) {
        return std::get<std::shared_ptr<jcCollection>>(mData).get();
    }
    return nullptr;
}

void jcVariable::set_String(const std::string& str)
{
    mData = str;
    mCurrentType = TypeString;
}

void jcVariable::set_Int(const int val)
{
    mData = val;
    mCurrentType = TypeInt;
}

void jcVariable::set_Closure(const jcClosurePtr &closure)
{
    mData = closure;
    mCurrentType = TypeClosure;
}

void jcVariable::set_Collection(const jcCollectionPtr &collection)
{
    mData = collection;
    mCurrentType = TypeCollection;
}

std::string jcVariable::stringRepresentation() const {
    if (getType() == TypeInt) {
        return std::to_string(asInt());
    } else if (getType() == TypeString) {
        return asString();
    } else if (getType() == TypeCollection) {
        std::string rep = "[";
        if (asCollectionRaw() != nullptr) {
            int i = (int)asCollectionRaw()->size();
            asCollectionRaw()->forEach([&rep, &i](jcVariablePtr element) {
                rep += element->stringRepresentation();
                if (--i != 0) {
                    rep += ", "; 
                }
            });
        }
        return rep + "]";
    }
    return "";
}

template<typename T>
static bool pointerEqual(T* one, T* two) {
    if (one != nullptr && two != nullptr) {
        return one->equal(*two);
    }
    return one == two;
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
        case TypeCollection:
            return pointerEqual(asCollectionRaw(), other.asCollectionRaw());
        case TypeClosure:
            return pointerEqual(asClosureRaw(), other.asClosureRaw());
        default:
            return false;
    }
}

/**
 Mutable Variable
 */

void jcMutableVariable::set(const jcVariable &other)
{
    if (other.getType() == TypeInt) {
        setInt(other.asInt());
    } else if (other.getType() == TypeString) {
        setString(other.asString());
    } else if (other.getType() == TypeCollection) {
        setCollection(other.asSharedPtr<jcCollection>());
    } else if (other.getType() == TypeClosure) {
        setClosure(other.asSharedPtr<jcClosure>());
    }
}

jcMutableVariablePtr jcMutableVariable::Create(jcVariable &other)
{
    jcMutableVariablePtr newVar = jcMutableVariable::Create();
    newVar->set(other);
    return newVar;
}

jcMutableVariablePtr jcMutableVariable::Create()
{
    return std::make_shared<jcMutableVariable>();
}

void jcMutableVariable::setString(const std::string& str)
{
    set_String(str);
}

void jcMutableVariable::setInt(const int val)
{
    set_Int(val);
}

void jcMutableVariable::setCollection(std::shared_ptr<jcCollection> collection)
{
    set_Collection(collection);
}

void jcMutableVariable::setClosure(const jcClosurePtr closure)
{
    set_Closure(closure);
}
