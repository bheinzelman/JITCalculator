//  jcVariable.hpp

#pragma once

#include <memory>
#include <string>
#include <variant>

#include "jc.h"

/**
 * variant type
 */

class jcVariable {
public:
    enum Type {
        TypeString,
        TypeInt,
        TypeArray,
        TypeClosure,
        TypeNone
    };

    jcVariable();

    static jcVariablePtr Create(std::string value);
    static jcVariablePtr Create(int value);
    static jcVariablePtr Create(const jcArrayPtr &array);
    static jcVariablePtr Create(const jcClosurePtr &closure);

    static jcVariablePtr CreateFromCollection(const jcCollectionPtr &collection);

    ~jcVariable();

    std::string asString() const;
    int asInt() const;
    jcArray* asArrayRaw() const;
    jcClosure* asClosureRaw() const;

    jcCollection *asCollection() const;

    /**
     Use this if you need a shared reference...
     */
    template<typename T>
    std::shared_ptr<T> asSharedPtr() const {
        static_assert(std::is_same<std::shared_ptr<T>, jcArrayPtr>().value ||
                      std::is_same<std::shared_ptr<T>, jcClosurePtr>().value, "Invalid shared ptr type");
        
        return std::get<std::shared_ptr<T>>(mData);
    }

    inline Type getType() const
    {
        return mCurrentType;
    }

    /**
     Returns the string representation,
     if 1 -> "1"
     if "1" -> "1"
     if [1,2,3] -> "[1, 2, 3]"
     */
    std::string stringRepresentation() const;

    bool equal(const jcVariable &other) const;

protected:
    static jcVariablePtr Create();

    void set_String(const std::string& str);
    void set_Int(const int val);
    void set_Array(const jcArrayPtr &array);
    void set_Closure(const jcClosurePtr &closure);

protected:
    Type mCurrentType;

    std::variant<
        std::string,
        int,
        jcArrayPtr,
        jcClosurePtr> mData;
};

class jcMutableVariable : public jcVariable
{
public:
    jcMutableVariable() {}
    static jcMutableVariablePtr Create(jcVariable &other);
    static jcMutableVariablePtr Create();

    void set(const jcVariable &other);
    void setString(const std::string& str);
    void setInt(const int val);
    void setArray(jcArrayPtr array);
    void setClosure(const jcClosurePtr closure);
};

using jcVariableType = jcVariable::Type;
