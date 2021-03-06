//  builtin.hpp

#include "jc.h"
#include "jcVariable.hpp"
#include "Interpreter.hpp"

#include <string>
#include <functional>
#include <unordered_map>
#include <map>
#include <vector>
#include <ostream>

namespace lib
{

/**
 Keys that populate the info dict
 */
const std::string kLibParameterNumber = "LibParameterNumber";
const std::string kLibReturnType = "LibReturnType";

/**
 Library functions

 function: print
    - prints the given value

 function: List
    - arg 1, number of elements
    - arg ..., elements in the list
    - returns a new list

 function: len
    - arg 1: a list
    - returns the length of the list

 */
const std::string kLibPrint = "print";
const std::string kLibLen = "len";
const std::string kLibIsEmpty = "isEmpty";

struct LibState {
    std::ostream &mStdout;
    std::ostream &mStderr;

    LibState();
};

using LibraryFunction = std::function<jcVariablePtr(Interpreter&, const LibState&)>;

class builtin {
    builtin();
public:
    static builtin Shared();
    /**
     Return function info for given name
     If the function does not exist, the kLibError key will be set
     */
    std::map<std::string, jcVariablePtr>* info(const std::string &functionName);

    /**
     Runs the given function with the supplied arguments
     Runtime error will be thrown if function does not exist
     */
    jcVariablePtr execute(const std::string &functionName, Interpreter &interpreter);

    const LibState& state() const;

private:
    static std::unordered_map<std::string, std::map<std::string, jcVariablePtr>> mInfo;
    static std::unordered_map<std::string, LibraryFunction> mFunctions;

    LibState mState;
};

}
