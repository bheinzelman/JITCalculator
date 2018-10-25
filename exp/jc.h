//  jc.h

#pragma once

#include <cassert>
#include <exception>
#include <string>
#include <cstdint>

///////////////////////////////////
// Types
///////////////////////////////////
using fourcc = int32_t;


class jcException : public std::exception
{
public:
	jcException(const std::string &message) : mMessage(message)
	{
	}
	
	std::string getMessage() const {
		return mMessage;
	}
private:
	std::string mMessage;
};


///////////////////////////////////
// Macros
///////////////////////////////////
#define JC_VERSION_STRING "0.0.0"

#define JC_FAIL() assert(0);

#define JC_FAIL_AND_RETURN_NULL() \
	do {\
		JC_FAIL(); \
		return nullptr;\
	} while (0);

#define JC_ASSERT(e) \
	do {\
		assert(e); \
} while (0);

#define JC_THROW(m) \
	throw jcException(m)\

#define JC_ASSERT_OR_THROW(e, m) \
	do {\
		if (!(e)) {\
			throw jcException(m);\
		}\
	} while (0);
