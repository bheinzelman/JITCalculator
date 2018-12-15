//  jcVariable.hpp

#pragma once

#include <memory>
#include <string>
#include <variant>

#include "jc.h"
#include "jcCollection.hpp"
#include "jcClosure.hpp"

/**
 * variant type
 */

class jcVariable {
public:
    enum Type {
        TypeString,
        TypeInt,
        TypeCollection,
        TypeClosure,
        TypeNone
    };

    jcVariable();

    static jcVariablePtr Create(std::string value);
    static jcVariablePtr Create(int value);

    static jcVariablePtr Create(const jcCollectionPtr &collection);
    
    static jcVariablePtr Create(const jcClosurePtr &closure);

    ~jcVariable();

    std::string asString() const;
    int asInt() const;
    jcCollection* asCollectionRaw() const;
    jcClosure* asClosureRaw() const;

    /**
     Use this if you need a shared reference...
     */
    template<typename T>
    std::shared_ptr<T> asSharedPtr() const {
        static_assert(std::is_same<std::shared_ptr<T>, jcCollectionPtr>().value ||
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
    void set_Collection(const jcCollectionPtr &collection);
    void set_Closure(const jcClosurePtr &closure);

protected:
    Type mCurrentType;

    std::variant<
        std::string,
        int,
        jcCollectionPtr,
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

    /**
     WARNING, this transfers  the collection to the jcMutableVariable
     and sets the given collection to NULL
     */
    void setCollection(jcCollectionPtr collection);
    void setClosure(const jcClosurePtr closure);
};
