//  jcVariable.hpp

#pragma once

#include <string>
#include <memory>

/**
 * variant type
 */

class jcVariable;
using jcVariablePtr = std::shared_ptr<jcVariable>;

class jcVariable
{
	jcVariable();
public:
    enum Type {
        TypeString,
        TypeInt,
        TypeNone
    };

	static jcVariablePtr Create();
	
	~jcVariable();
	
	std::string asString() const;
	int asInt() const;
	
	void setString(const std::string &str);
	void setInt(const int val);

    inline Type getType() const {
        return mCurrentType;
    }
	
private:
	void willSet();
	
private:
	
	Type mCurrentType;
	
	union {
		char *mStr;
		int mInt;
	} mData;
};
