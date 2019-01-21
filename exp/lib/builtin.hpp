//  builtin.hpp

#include "jc.h"
#include "jcVariable.hpp"
#include "Interpreter.hpp"

#include <string>
#include <functional>
#include <map>
#include <vector>
#include <ostream>

namespace lib
{

/**
 Keys that populate the info dict
 */
const std::string kLibParameterNumber = "LibParameterNumber";
const std::string kLibError = "LibError";
const std::string kLibReturnType = "LibReturnType";

/**
 Library functions

 function: print
    - prints the given value

 function: List
    - arg 1, number of elements
    - arg ..., elements in the list
    - returns a new list

 function: head
    - arg 1: a list
    - returns the first element in the list

 function: tail
    - arg 1: a list
    - returns the list with all elements but the first

 function: len
    - arg 1: a list
    - returns the length of the list

 function: concat
    - arg 1: a list
    - arg 2: a list
    - returns list 1 and list 2 concatenated

 */
const std::string kLibPrint = "print";
    
const std::string kLibList = "List";
const std::string kLibHead = "head";
const std::string kLibTail = "tail";
const std::string kLibLen = "len";
const std::string kLibIsEmpty = "isEmpty";
const std::string kLibFilter = "filter";

struct LibState {
    std::ostream &mStdout;
    std::ostream &mStderr;

    LibState();
};

using LibraryFunction = std::function<jcVariablePtr(Interpreter &, LibState)>;

class builtin {
    builtin();
public:
    static builtin Shared();
    /**
     Return function info for given name
     If the function does not exist, the kLibError key will be set
     */
    std::map<std::string, jcVariablePtr> info(const std::string &functionName);

    /**
     Runs the given function with the supplied arguments
     Runtime error will be thrown if function does not exist
     */
    jcVariablePtr execute(const std::string &functionName, Interpreter &interpreter);

    LibState state() const;

private:
    static std::map<std::string, std::map<std::string, jcVariablePtr>> mInfo;
    static std::map<std::string, LibraryFunction> mFunctions;

    LibState mState;
};

}
