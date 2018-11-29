//  jcVariable.hpp

#pragma once

#include <memory>
#include <string>

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

    /**
     WARNING, this transfers  the collection to the jcMutableVariable
     and sets the given collection to NULL
     */
    static jcVariablePtr Create(jcCollection **collection);
    
    static jcVariablePtr Create(const jcClosure& closure);

    ~jcVariable();

    std::string asString() const;
    int asInt() const;
    jcCollection* asCollection() const;
    jcClosure *asClosure() const;

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

    void willSet();

    void set_String(const std::string& str);
    void set_Int(const int val);
    void set_Collection(jcCollection** collection);
    void set_Closure(const jcClosure& closure);

protected:
    Type mCurrentType;

    union {
        char* mStr;
        int mInt;
        jcCollection* collection;
        jcClosure* closure;
    } mData;
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
    void setCollection(jcCollection** collection);
    void setClosure(const jcClosure& closure);
};
