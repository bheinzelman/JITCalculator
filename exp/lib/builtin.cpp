//  builtin.cpp

#include "builtin.hpp"
#include "jcArray.hpp"
#include "jcClosure.hpp"
#include "jcList.hpp"

#include <functional>
#include <iostream>

namespace lib {

std::unordered_map<std::string, std::map<std::string, jcVariablePtr>> builtin::mInfo =
{
    {
        kLibPrint,
        {
            {kLibParameterNumber, jcVariable::Create(1)},
            {kLibReturnType, jcVariable::Create(jcVariable::TypeInt)}
        }
    },
    {
        kLibLen,
        {
            {kLibParameterNumber, jcVariable::Create(1)},
            {kLibReturnType, jcVariable::Create(jcVariable::TypeInt)}
        }
    },
    {
        kLibIsEmpty,
        {
            {kLibParameterNumber, jcVariable::Create(1)},
            {kLibReturnType, jcVariable::Create(jcVariable::TypeInt)}
        }
    }
};

std::unordered_map<std::string, LibraryFunction> builtin::mFunctions =
{
    {
        kLibPrint,
        [](Interpreter &interpreter, LibState state) -> jcVariablePtr {
            jcVariablePtr arg = interpreter.popStack();
            state.mStdout << arg->stringRepresentation() << std::endl;
            return jcVariable::Create(0);
        }
    },
    {
        kLibLen,
        [](Interpreter &interpreter, LibState state) -> jcVariablePtr {
            jcVariablePtr arg = interpreter.popStack();
            JC_ASSERT(arg->asCollection() != nullptr);
            jcCollection* collection = arg->asCollection();

            return jcVariable::Create((int)collection->size());
        }
    },
    {
        kLibIsEmpty,
        [](Interpreter &interpreter, LibState state) -> jcVariablePtr {
            jcVariablePtr list = interpreter.popStack();

             JC_ASSERT(list->asCollection() != nullptr);

            jcCollection* array = list->asCollection();

            return jcVariable::Create(array->isEmpty());
        }
    }
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

const LibState& builtin::state() const
{
    return mState;
}

std::map<std::string, jcVariablePtr>* builtin::info(const std::string &functionName)
{
    if (mInfo.count(functionName) > 0) {
        return &mInfo[functionName];
    }

    return nullptr;
}

jcVariablePtr builtin::execute(const std::string &functionName, Interpreter &interpreter)
{
    if (mFunctions.count(functionName)) {
        return mFunctions[functionName](interpreter, state());
    }
    JC_THROW_VM_EXCEPTION("Library function " + functionName + " undefined");
}

}

