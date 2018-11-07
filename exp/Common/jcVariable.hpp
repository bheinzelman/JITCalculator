//  jcVariable.hpp

#pragma once

#include <memory>
#include <string>

#include "jc.h"
#include "jcCollection.hpp"

/**
 * variant type
 */

class jcVariable {

protected:
    jcVariable();

public:
    enum Type {
        TypeString,
        TypeInt,
        TypeCollection,
        TypeNone
    };

    static jcVariablePtr Create(std::string value);
    static jcVariablePtr Create(int value);
    static jcVariablePtr Create(const jcCollection& collection);

    ~jcVariable();

    std::string asString() const;
    int asInt() const;
    jcCollection* asCollection() const;

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
    std::string stringRepresentation();

protected:
    static jcVariablePtr Create();

    void willSet();

    void set_String(const std::string& str);
    void set_Int(const int val);
    void set_Collection(const jcCollection& collection);

protected:
    Type mCurrentType;

    union {
        char* mStr;
        int mInt;
        jcCollection* collection;
    } mData;
};

class jcMutableVariable : public jcVariable
{
    jcMutableVariable() {}

public:
    static jcMutableVariablePtr Create(jcVariable &other);
    static jcMutableVariablePtr Create();

    void setString(const std::string& str);
    void setInt(const int val);
    void setCollection(const jcCollection& collection);
};
