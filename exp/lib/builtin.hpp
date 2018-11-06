//  builtin.hpp

#include "jc.h"
#include "jcVariable.hpp"

#include <string>
#include <map>
#include <vector>

namespace lib
{
namespace builtin
{
    /**
     Keys that populate the info dict
     */
    const std::string kLibParameterNumber = "LibParameterNumber";
    const std::string kLibError = "LibError";
    const std::string kLibReturnType = "LibReturnType";

    /**
     Library functions
     */
    const std::string kLibPrint = "print";

    /**
     Return function info for given name
     If the function does not exist, the kLibError key will be set
     */
    std::map<std::string, jcVariablePtr> info(const std::string &functionName);

    /**
     Runs the given function with the supplied arguments
     Runtime error will be thrown if function does not exist
     */
    jcVariablePtr execute(const std::string &functionName, const std::vector<jcVariablePtr> &args);
}
}
