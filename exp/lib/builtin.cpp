//  builtin.cpp

#include "builtin.hpp"
#include "jcArray.hpp"
#include "jcClosure.hpp"
#include "jcList.hpp"

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
            {kLibReturnType, jcVariable::Create(jcVariable::TypeArray)}
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
            {kLibReturnType, jcVariable::Create(jcVariable::TypeArray)}
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
    },
    {
        kLibFilter,
        {
            {kLibParameterNumber, jcVariable::Create(2)},
            {kLibReturnType, jcVariable::Create(jcVariable::TypeArray)}
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
//            jcMutableArrayPtr elements = std::make_shared<jcMutableArray>(numElements);
//            for (int i = 0; i < numElements; i++) {
//                elements->push(interpreter.popStack());
//            }
            jcListPtr list = std::make_shared<jcList>();
            for (int i = 0; i < numElements; i++) {
                std::shared_ptr<jcCollection> newCollection = std::shared_ptr<jcCollection>(list->cons(interpreter.popStack()));
                list = std::static_pointer_cast<jcList>(newCollection);
            }
            return jcVariable::Create(list);
        }
    },
    {
        kLibHead,
        [](Interpreter &interpreter, LibState state) -> jcVariablePtr {
            jcVariablePtr arg = interpreter.popStack();
            JC_ASSERT(arg->asCollection() != nullptr);
            return arg->asCollection()->head();
        }
    },
    {
        kLibTail,
        [](Interpreter &interpreter, LibState state) -> jcVariablePtr {
            jcVariablePtr arg = interpreter.popStack();
            JC_ASSERT(arg->asCollection() != nullptr);
            jcCollection* array = arg->asCollection();

            auto tail = std::shared_ptr<jcCollection>(array->tail());

            return jcVariable::CreateFromCollection(tail);
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
    },
    {
        kLibFilter,
        [](Interpreter &interpreter, LibState state) -> jcVariablePtr {
            // TODO this should return the same type that came in..
            jcVariablePtr function = interpreter.popStack();
            jcVariablePtr list = interpreter.popStack();

            JC_ASSERT(list->asCollection() != nullptr);

            jcMutableArrayPtr newArray = std::make_shared<jcMutableArray>();

            list->asCollection()->forEach([&interpreter, newArray, function](jcVariablePtr item) {
                jcVariablePtr result = interpreter.interpret(function, { item });

                JC_ASSERT(result->getType() == jcVariable::TypeInt);

                if (result->asInt()) {
                    newArray->push(item);
                }
            });
            return jcVariable::Create(newArray);
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
    JC_THROW_VM_EXCEPTION("Library function " + functionName + " undefined");
}

}

