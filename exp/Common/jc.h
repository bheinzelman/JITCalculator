//  jc.h

#pragma once

#include <cassert>
#include <cstdint>
#include <exception>
#include <string>

///////////////////////////////////
// Types
///////////////////////////////////
using fourcc = int32_t;

class jcVariable;
using jcVariablePtr = std::shared_ptr<jcVariable>;

class jcMutableVariable;
using jcMutableVariablePtr = std::shared_ptr<jcMutableVariable>;

class jcCollection;
using jcCollectionPtr = std::shared_ptr<jcCollection>;

class jcMutableCollection;
using jcMutableCollectionPtr = std::shared_ptr<jcMutableCollection>;

class jcClosure;
using jcClosurePtr = std::shared_ptr<jcClosure>;

class jcException : public std::exception {
public:
    jcException(const std::string& message)
        : mMessage(message)
    {
    }

    std::string getMessage() const
    {
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
    do {                          \
        JC_FAIL();                \
        return nullptr;           \
    } while (0);

#define JC_ASSERT(e) \
    do {             \
        assert(e);   \
    } while (0);

#define JC_THROW(m) \
    throw jcException(m)

#define JC_ASSERT_OR_THROW(e, m)  \
    do {                          \
        if (!(e)) {               \
            JC_THROW(m); \
        }                         \
    } while (0);


#define JC_STD_LIBRARY_PATH std::string(LIB_PATH) + "/std.jc"
