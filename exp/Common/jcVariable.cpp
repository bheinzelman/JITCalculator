//  jcVariable.cpp

#include "jcVariable.hpp"
#include "jcArray.hpp"
#include "jcClosure.hpp"
#include "jcString.hpp"
#include "jcList.hpp"

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

jcVariablePtr jcVariable::Create(char value)
{
    auto me = jcVariable::Create();
    me->set_Char(value);
    return me;
}

jcVariablePtr jcVariable::Create(const jcArrayPtr &array)
{
    auto me = jcVariable::Create();
    me->set_Array(array);
    return me;
}

jcVariablePtr jcVariable::Create(const jcClosurePtr &closure)
{
    auto me = jcVariable::Create();
    me->set_Closure(closure);
    return me;
}

jcVariablePtr jcVariable::Create(const jcStringPtr &value)
{
    auto me = jcVariable::Create();
    me->set_jcString(value);
    return me;
}

jcVariablePtr jcVariable::Create(const jcListPtr &value)
{
    auto me = jcVariable::Create();
    me->set_List(value);
    return me;
}

jcVariable::jcVariable()
    : mCurrentType(TypeNone)
{
}

jcVariablePtr jcVariable::CreateFromCollection(const jcCollectionPtr &collection)
{
    JC_ASSERT(collection);
    switch (collection->getType()) {
        case jcVariable::TypeArray:
            return jcVariable::Create(std::static_pointer_cast<jcArray>(collection));
        case jcVariable::TypeString:
            return jcVariable::Create(std::static_pointer_cast<jcString>(collection));
        case jcVariable::TypeList:
            return jcVariable::Create(std::static_pointer_cast<jcList>(collection));
        default:
            return nullptr;
    }
}

jcVariable::~jcVariable()
{
}

std::string jcVariable::asString() const
{
    if (mCurrentType == TypeString) {
        return std::get<jcStringPtr>(mData)->asStdString();
    }
    return std::string();
}

char jcVariable::asChar() const
{
    if (mCurrentType == TypeChar) {
        return std::get<char>(mData);
    }
    return 0;
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

jcArray* jcVariable::asArrayRaw() const
{
    if (mCurrentType == TypeArray) {
        return std::get<std::shared_ptr<jcArray>>(mData).get();
    }
    return nullptr;
}

jcString* jcVariable::asJcStringRaw() const
{
    if (mCurrentType == TypeString) {
        return std::get<jcStringPtr>(mData).get();
    }
    return nullptr;
}

jcList* jcVariable::asListRaw() const {
    if (mCurrentType == TypeList) {
        return std::get<jcListPtr>(mData).get();
    }
    return nullptr;
}

jcCollection* jcVariable::asCollection() const
{
    switch (getType()) {
        case TypeArray:
            return asArrayRaw();
        case TypeList:
            return asListRaw();
        default:
            return nullptr;
    }
}

void jcVariable::set_String(const std::string& str)
{
    mData = jcString::Create(str);
    mCurrentType = TypeString;
}

void jcVariable::set_Int(const int val)
{
    mData = val;
    mCurrentType = TypeInt;
}

void jcVariable::set_Char(const char val)
{
    mData = val;
    mCurrentType = TypeChar;
}

void jcVariable::set_Closure(const jcClosurePtr &closure)
{
    mData = closure;
    mCurrentType = TypeClosure;
}

void jcVariable::set_Array(const jcArrayPtr &array)
{
    mData = array;
    mCurrentType = TypeArray;
}

void jcVariable::set_jcString(const jcStringPtr &string)
{
    mData = string;
    mCurrentType = TypeString;
}

void jcVariable::set_List(const jcListPtr &list) {
    mData = list;
    mCurrentType = TypeList;
}

std::string jcVariable::stringRepresentation() const {
    if (getType() == TypeInt) {
        return std::to_string(asInt());
    } else if (getType() == TypeString) {
        return asString();
    } else if (getType() == TypeChar) {
        return "'" + std::string(1, asChar()) + "'";
    } else if (asCollection()) {
        std::string rep = "[";

        int i = (int)asCollection()->size();
        asCollection()->forEach([&rep, &i](jcVariablePtr element) {
            rep += element->stringRepresentation();
            if (--i != 0) {
                rep += ", ";
            }
        });

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
        case TypeChar:
            return asChar() == other.asChar();
        case TypeString:
            return asString() == other.asString();
        case TypeArray:
            return pointerEqual(asArrayRaw(), other.asArrayRaw());
        case TypeClosure:
            return pointerEqual(asClosureRaw(), other.asClosureRaw());
        case TypeList:
            return pointerEqual(asListRaw(), other.asListRaw());
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
    } else if (other.getType() == TypeArray) {
        setArray(other.asSharedPtr<jcArray>());
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

void jcMutableVariable::setArray(std::shared_ptr<jcArray> array)
{
    set_Array(array);
}

void jcMutableVariable::setClosure(const jcClosurePtr closure)
{
    set_Closure(closure);
}

void jcMutableVariable::setChar(const char val)
{
    set_Char(val);
}
