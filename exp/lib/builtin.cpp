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
    {
        kLibConcat,
        {
            {kLibParameterNumber, jcVariable::Create(2)},
            {kLibReturnType, jcVariable::Create(jcVariable::TypeCollection)}
        }
    },
    {
        kLibCons,
        {
            {kLibParameterNumber, jcVariable::Create(2)},
            {kLibReturnType, jcVariable::Create(jcVariable::TypeCollection)}
        }
    },
    {
        kLibIsEmpty,
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
        [](Interpreter &interpreter, LibState state) -> jcVariablePtr {
            jcVariablePtr arg = interpreter.popStack();
            state.mStdout << arg->stringRepresentation() << std::endl;
            return jcVariable::Create(0);
        }
    },
    {
        kLibList,
        [](Interpreter &interpreter, LibState state) -> jcVariablePtr {
            int numElements = interpreter.popStack()->asInt();
            jcMutableCollectionPtr elements = std::make_shared<jcMutableCollection>(numElements);
            for (int i = 0; i < numElements; i++) {
                elements->push(interpreter.popStack());
            }
            return jcVariable::Create(elements);
        }
    },
    {
        kLibHead,
        [](Interpreter &interpreter, LibState state) -> jcVariablePtr {
            jcVariablePtr arg = interpreter.popStack();
            JC_ASSERT(arg->getType() == jcVariable::TypeCollection);
            JC_ASSERT(arg->asCollectionRaw() != nullptr);
            return arg->asCollectionRaw()->head();
        }
    },
    {
        kLibTail,
        [](Interpreter &interpreter, LibState state) -> jcVariablePtr {
            jcVariablePtr arg = interpreter.popStack();
            JC_ASSERT(arg->getType() == jcVariable::TypeCollection);
            jcCollection* collection = arg->asCollectionRaw();
            JC_ASSERT(collection != nullptr);

            jcCollectionPtr tail = std::shared_ptr<jcCollection>(collection->tail());

            return jcVariable::Create(tail);
        }
    },
    {
        kLibLen,
        [](Interpreter &interpreter, LibState state) -> jcVariablePtr {
            jcVariablePtr arg = interpreter.popStack();;
            JC_ASSERT(arg->getType() == jcVariable::TypeCollection);
            jcCollection* collection = arg->asCollectionRaw();
            JC_ASSERT(collection != nullptr);

            return jcVariable::Create((int)collection->size());
        }
    },
    {
        kLibConcat,
        [](Interpreter &interpreter, LibState state) -> jcVariablePtr {
            jcVariablePtr list1 = interpreter.popStack();;
            jcVariablePtr list2 = interpreter.popStack();;

            JC_ASSERT(list1->getType() == jcVariable::TypeCollection);
            JC_ASSERT(list2->getType() == jcVariable::TypeCollection);

            jcCollection* collection1 = list1->asCollectionRaw();
            jcCollection* collection2 = list2->asCollectionRaw();

            JC_ASSERT(collection1 != nullptr && collection2 != nullptr);

            jcCollectionPtr newCollection = std::shared_ptr<jcCollection>(collection1->concat(*collection2));

            return jcVariable::Create(newCollection);
        }
    },
    {
        kLibCons,
        [](Interpreter &interpreter, LibState state) -> jcVariablePtr {
            jcVariablePtr item = interpreter.popStack();;
            jcVariablePtr list = interpreter.popStack();;

            JC_ASSERT(list->getType() == jcVariable::TypeCollection);

            jcCollection* collection = list->asCollectionRaw();
            jcMutableCollectionPtr newCollection = std::make_shared<jcMutableCollection>((int)collection->size() + 1);
            newCollection->push(item);

            collection->forEach([&newCollection](jcVariablePtr item) {
                newCollection->push(item);
            });

            JC_ASSERT(collection != nullptr);

            return jcVariable::Create(newCollection);
        }
    },
    {
        kLibIsEmpty,
        [](Interpreter &interpreter, LibState state) -> jcVariablePtr {
            jcVariablePtr list = interpreter.popStack();

            JC_ASSERT(list->getType() == jcVariable::TypeCollection);

            jcCollection* collection = list->asCollectionRaw();

            JC_ASSERT(collection != nullptr);

            return jcVariable::Create(collection->isEmpty());
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

    jcVariablePtr builtin::execute(const std::string &functionName, Interpreter &interpreter)
{
    if (mFunctions.count(functionName)) {
        return mFunctions[functionName](interpreter, state());
    }
    JC_THROW("Library function " + functionName + " undefined");
}

}

