//  builtin.cpp

#include "builtin.hpp"

#include <functional>

namespace lib {

namespace builtin {

using BuiltinFunction = std::function<jcVariablePtr(std::vector<jcVariablePtr>)>;

static std::map<std::string, std::map<std::string, jcVariablePtr>> _info =
{
    { kLibPrint,
        {
            {kLibParameterNumber, jcVariable::Create(1)},
            {kLibReturnType, jcVariable::Create("Int")}
        }
    }
};

static std::map<std::string, BuiltinFunction> _functions =
{
    { kLibPrint,
        [](const std::vector<jcVariablePtr> &args) -> jcVariablePtr {
            jcVariablePtr arg = args.front();
            if (arg->getType() == jcVariable::TypeInt) {
                printf("%d\n", arg->asInt());
            } else if (arg->getType() == jcVariable::TypeString) {
                printf("%s\n", arg->asString().c_str());
            }
            return jcVariable::Create(0);
        }
    }
};

std::map<std::string, jcVariablePtr> info(const std::string &functionName)
{
    if (_info.count(functionName) > 0) {
        return _info[functionName];
    }

    std::map<std::string, jcVariablePtr> errorDict;
    errorDict[kLibError] = jcVariable::Create(functionName + " undefined");
    return errorDict;
}

jcVariablePtr execute(const std::string &functionName, const std::vector<jcVariablePtr> &args)
{
    if (_info.count(functionName)) {
        auto info = _info[functionName];
        JC_ASSERT_OR_THROW(info[kLibParameterNumber]->asInt() == 1, "Invalid args");

        if (_functions.count(functionName)) {
            return _functions[functionName](args);
        }
    }

    JC_THROW("Library function " + functionName + " undefined");
}

}
}

