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
	static jcVariablePtr Create();
	
	~jcVariable();
	
	std::string asString() const;
	int asInt() const;
	
	void setString(const std::string &str);
	void setInt(const int val);
	
private:
	void willSet();
	
private:
	enum type {
		TypeString,
		TypeInt,
		TypeNone
	};
	
	type mCurrentType;
	
	union {
		char *mStr;
		int mInt;
	} mData;
};
