//  jc.h

#pragma once

#include <cassert>
#include <cstdint>
#include <exception>
#include <string>

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

#define JC_THROW_PARSE_EXCEPTION(m, l) \
    throw jcException::ParseError(m, l)

#define JC_ASSERT_OR_THROW_PARSE(e, m, l)   \
    do {                                    \
        if (!(e)) {                         \
            JC_THROW_PARSE_EXCEPTION(m, l); \
        }                                   \
    } while (0);

#define JC_THROW_VM_EXCEPTION(m) \
    throw jcException::VmError(m)

#define JC_ASSERT_OR_THROW_VM(e, m)   \
    do {                                    \
        if (!(e)) {                         \
            JC_THROW_VM_EXCEPTION(m); \
        }                                   \
    } while (0);


#define JC_STD_LIBRARY_PATH std::string(LIB_PATH) + "/std.jc"

#define JC_CLASS(classname) \
    class classname; \
    using classname##Ptr = std::shared_ptr<classname>;

///////////////////////////////////
// Types
///////////////////////////////////
using fourcc = int32_t;

/// Define ptr types

JC_CLASS(jcVariable);
JC_CLASS(jcMutableVariable);
JC_CLASS(jcArray);
JC_CLASS(jcMutableArray);
JC_CLASS(jcClosure);
JC_CLASS(jcString);
JC_CLASS(jcList);
JC_CLASS(jcCollection);

class jcException : public std::exception {
public:
    enum class Domain {
        Parse,
        Codegen,
        Vm
    };

    static jcException ParseError(const std::string &message, int64_t line)
    {
        return jcException(message, Domain::Parse, line);
    }

    static jcException CodegenError(const std::string &message)
    {
        return jcException(message, Domain::Codegen, -1);
    }

    static jcException VmError(const std::string &message) {
        return jcException(message, Domain::Vm, -1);
    }

    std::string getMessage() const
    {
        return mMessage;
    }

    int64_t getLine() const
    {
        return mLine;
    }

    Domain getDomain() const
    {
        return mDomain;
    }

private:
    jcException(const std::string& message, Domain exceptionDomain, int64_t lineNumber)
    : mMessage(message), mLine(lineNumber), mDomain(exceptionDomain)
    {
    }

    Domain mDomain;
    int64_t mLine;
    std::string mMessage;
};
