//  builtin.cpp

#include "builtin.hpp"

#include <functional>
#include <iostream>

namespace lib {

std::map<std::string, std::map<std::string, jcVariablePtr>> builtin::mInfo =
{
    {
        kLibPrint,
        {
            {kLibParameterNumber, jcVariable::Create(1)},
            {kLibReturnType, jcVariable::Create(jcVariable::TypeInt)}
        }
    },
    {
        kLibList,
        {
            {kLibReturnType, jcVariable::Create(jcVariable::TypeCollection)}
        }
    },
    {
        kLibHead,
        {
            {kLibParameterNumber, jcVariable::Create(1)},
        }
    },
    {
        kLibTail,
        {
            {kLibParameterNumber, jcVariable::Create(1)},
            {kLibReturnType, jcVariable::Create(jcVariable::TypeCollection)}
        }
    },
    {
        kLibLen,
        {
            {kLibParameterNumber, jcVariable::Create(1)},
            {kLibReturnType, jcVariable::Create(jcVariable::TypeInt)}
        }
    },
};

std::map<std::string, LibraryFunction> builtin::mFunctions =
{
    {
        kLibPrint,
        [](std::function<jcVariablePtr()> stackAccess, LibState state) -> jcVariablePtr {
            jcVariablePtr arg = stackAccess();
            state.mStdout << arg->stringRepresentation() << std::endl;
            return jcVariable::Create(0);
        }
    },
    {
        kLibList,
        [](std::function<jcVariablePtr()> stackAccess, LibState state) -> jcVariablePtr {
            int numElements = stackAccess()->asInt();
            std::vector<jcVariablePtr> elements;
            for (int i = 0; i < numElements; i++) {
                elements.push_back(stackAccess());
            }
            jcCollection collection(elements);
            return jcVariable::Create(collection);
        }
    },
    {
        kLibHead,
        [](std::function<jcVariablePtr()> stackAccess, LibState state) -> jcVariablePtr {
            jcVariablePtr arg = stackAccess();
            JC_ASSERT(arg->getType() == jcVariable::TypeCollection);
            JC_ASSERT(arg->asCollection() != nullptr);
            return arg->asCollection()->at(0);
        }
    },
    {
        kLibTail,
        [](std::function<jcVariablePtr()> stackAccess, LibState state) -> jcVariablePtr {
            jcVariablePtr arg = stackAccess();
            JC_ASSERT(arg->getType() == jcVariable::TypeCollection);
            jcCollection *collection = arg->asCollection();
            JC_ASSERT(collection != nullptr);

            std::vector<jcVariablePtr> elementsTail;
            for (int i = 1; i < (int)collection->size(); i++) {
                elementsTail.push_back(collection->at(i));
            }

            return jcVariable::Create(elementsTail);
        }
    },
    {
        kLibLen,
        [](std::function<jcVariablePtr()> stackAccess, LibState state) -> jcVariablePtr {
            jcVariablePtr arg = stackAccess();
            JC_ASSERT(arg->getType() == jcVariable::TypeCollection);
            jcCollection *collection = arg->asCollection();
            JC_ASSERT(collection != nullptr);

            return jcVariable::Create((int)collection->size());
        }
    },
};

LibState::LibState() : mStdout(std::cout), mStderr(std::cerr)
{
}

builtin::builtin() {

}

builtin builtin::Shared() {
    static builtin sharedInstance = builtin();
    return sharedInstance;
}

LibState builtin::state() const
{
    return mState;
}

std::map<std::string, jcVariablePtr> builtin::info(const std::string &functionName)
{
    if (mInfo.count(functionName) > 0) {
        return mInfo[functionName];
    }

    std::map<std::string, jcVariablePtr> errorDict;
    errorDict[kLibError] = jcVariable::Create(functionName + " undefined");
    return errorDict;
}

    jcVariablePtr builtin::execute(const std::string &functionName,  std::function<jcVariablePtr()> stackAccess)
{

    if (mFunctions.count(functionName)) {
        return mFunctions[functionName](stackAccess, state());
    }
    JC_THROW("Library function " + functionName + " undefined");
}

}

