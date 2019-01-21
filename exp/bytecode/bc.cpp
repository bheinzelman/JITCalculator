//  bc.cpp

#include "bc.hpp"
#include "jc.h"
#include "builtin.hpp"

#include <algorithm>
#include <string>

namespace bc {

static std::string opToString(bc::Op op)
{
    switch (op) {
    case bc::Add:
        return "ADD";
    case bc::Subtract:
        return "SUB";
    case bc::Multiply:
        return "MUL";
    case bc::Divide:
        return "DIV";
    case bc::Push:
        return "PUSH";
    case bc::Pop:
        return "POP";
    case bc::Ret:
        return "RET";
    case bc::Exit:
        return "EXIT";
    case bc::Call:
        return "CALL";
    case bc::Less_Than:
        return "LESS_THAN";
    case bc::Greater_Than:
        return "GREATER_THAN";
    case bc::Less_Than_Equal:
        return "LESS_THAN_EQUAL";
    case bc::Greater_Than_Equal:
        return "GREATER_THAN_EQUAL";
    case bc::Equals:
        return "EQUALS";
    case bc::JmpTrue:
        return "JMP_TRUE";
    case bc::Jmp:
        return "Jmp";
    case bc::Label:
        return "Label";
    case bc::PushC:
        return "PushC";
    default:
        JC_FAIL();
        break;
    }
    return "";
}

Instruction::Instruction(bc::Op op, const jcVariablePtr &operands)
    : mOp(op)
    , mOperand(operands)
{
}

Instruction::Instruction(bc::Op op)
    : mOp(op)
{
}

int Instruction::numOperands() const
{
    return mOperand != nullptr ? 1 : 0;
}

jcVariablePtr Instruction::getOperand() const
{
    return mOperand;
}

bc::Op Instruction::getOp() const
{
    return mOp;
}

std::string Instruction::toString() const
{
    std::string output = "";
    if (mOp != bc::Label) {
        output += opToString(mOp) + " ";
    }

    if (mOperand) {
        if (mOperand->getType() == jcVariable::TypeInt) {
            int operand = mOperand->asInt();
            output += std::to_string(operand);
        } else {
            output += mOperand->asString();
        }
    }
    return output;
}

Generator::Generator()
{
}

std::vector<Instruction> Generator::getInstructions(std::shared_ptr<Node> root)
{
    mOutput.clear();
    mClosures.clear();
    mScope.clear();
    root->accept(this);
    return mOutput;
}

std::vector<Instruction> Generator::getClosureInstructions()
{
    mOutput.clear();
    generateClosures();
    return mOutput;
}

void Generator::generateClosures()
{
    for (int i = 0; i < mClosures.size(); i++) {
        Closure *closure = mClosures[i].first;
        std::set<std::string> scope = mClosures[i].second;

        std::string label = closureLabel(i);
        mCurrentFunctionLabel = label;
        mOutput.push_back(Instruction(bc::Label, jcVariable::Create(label)));

        // push the current scope into the closure
        for (auto it = scope.rbegin(); it != scope.rend(); ++it) {
            std::string var = *it;
            jcVariablePtr paramVar = jcVariable::Create(var);
            Instruction popOp = Instruction(bc::Pop, paramVar);
            mOutput.push_back(popOp);
        }

        closure->getBody()->accept(this);
    }
}

std::string Generator::closureLabel(int idx) const
{
    return std::string("c.") + std::to_string(idx);
}

/**
 Used for making non-closure labels
 */
std::string Generator::labelMaker()
{
    static int numLabels = 0;
    return std::string(".") + std::to_string(numLabels++);
}

void Generator::visit(FunctionBody* functionBody)
{
    JC_ASSERT(mCurrentFunctionLabel.size() > 0);

    std::string functionName = mCurrentFunctionLabel;

    for (std::string param : functionBody->getParameters()) {
        jcVariablePtr paramVar = jcVariable::Create(param);
        Instruction popOp = Instruction(bc::Pop, paramVar);
        mOutput.push_back(popOp);
        mScope.insert(param);
    }

    std::string endLabel = functionName + ".end";

    if (functionBody->getGuards().size()) {
        std::vector<std::string> labels;

        // do guard expressions
        for (int i = 0; i < functionBody->getGuards().size(); i++) {
            std::shared_ptr<Guard> guard = functionBody->getGuards()[i];
            guard->getGuardExpression()->accept(this);

            std::string label = labelMaker();
            labels.push_back(label);

            Instruction jmpInstruction = Instruction(bc::JmpTrue, jcVariable::Create(label));
            mOutput.push_back(jmpInstruction);
        }

        // jump to default if none of guards hit
        std::string lastLabel = functionName + "." + std::to_string(functionBody->getGuards().size());
        mOutput.push_back(Instruction(bc::Jmp, jcVariable::Create(lastLabel)));

        // now do the guard bodies

        for (int i = 0; i < functionBody->getGuards().size(); i++) {
            std::shared_ptr<Guard> guard = functionBody->getGuards()[i];
            std::string label = labels[i];

            mOutput.push_back(Instruction(bc::Label, jcVariable::Create(label)));
            guard->getBodyExpression()->accept(this);

            mOutput.push_back(Instruction(bc::Jmp, jcVariable::Create(endLabel)));
        }

        mOutput.push_back(Instruction(bc::Label, jcVariable::Create(lastLabel)));
    }

    functionBody->getDefaultExpression()->accept(this);

    if (functionBody->getGuards().size()) {
        mOutput.push_back(Instruction(bc::Label, jcVariable::Create(endLabel)));
    }

    Instruction returnInstruction = Instruction(bc::Ret);
    mOutput.push_back(returnInstruction);
}

void Generator::visit(Closure* closure)
{
    std::string closureName = closureLabel(mNumClosures++);

    mOutput.push_back(Instruction(bc::PushC, jcVariable::Create(closureName)));


    // NEED TO PUSH WHAT IS IN SCOPE SO WE KNOW WHAT WE HAVE IN SCOPE..., NEED TO ADD A PUSHC INSTRUCTION THAT CAN PUSH
    // A CLOSURE OBJECT. THE CLOSURE OBJECT WILL HOLD A COPY OF THE ITEMS IN SCOPE. WHEN IT IS CALLED IT WILL PUSH THOSE
    // ITEMS ONTO THE STACK... AHHHHH THIS IS WHAT NEEDS TO HAPPEN FOR THIS TO WORK>>>
    mClosures.push_back(std::make_pair(closure, std::set<std::string>(mScope)));
}

void Generator::visit(NegateExpression* expression)
{
    expression->getExpression()->accept(this);
    Instruction negate = Instruction(bc::Neg);
    mOutput.push_back(negate);
}

void Generator::visit(TernaryExpresssion* expression)
{
    expression->getConditionalExpression()->accept(this);

    auto trueLabel = labelMaker();
    auto endLabel = labelMaker();

    mOutput.push_back(Instruction(bc::JmpTrue, jcVariable::Create(trueLabel)));

    // false
    expression->getFalseExpression()->accept(this);
    mOutput.push_back(Instruction(bc::Jmp, jcVariable::Create(endLabel)));

    // true
    mOutput.push_back(Instruction(bc::Label, jcVariable::Create(trueLabel)));
    expression->getTrueExpression()->accept(this);

    mOutput.push_back(Instruction(bc::Label, jcVariable::Create(endLabel)));
}

void Generator::visit(NotExpression* expression)
{
    expression->getExpression()->accept(this);
    Instruction notInstruction = Instruction(bc::Not);
    mOutput.push_back(notInstruction);
}

void Generator::visit(FunctionDecl* function)
{
    // new function new scope..
    mScope.clear();

    std::string functionName = function->getId();

    Instruction functionLabel = Instruction(bc::Label, jcVariable::Create(functionName));
    mOutput.push_back(functionLabel);

    mCurrentFunctionLabel = functionName;

    auto functionBody = function->getFunctionBody();

    functionBody->accept(this);
}

void Generator::visit(Guard* guard)
{
    JC_FAIL();
}

void Generator::visit(VariableExpression* expression)
{
    jcVariablePtr varId = jcVariable::Create(expression->getVariableName());
    Instruction pushOp = Instruction(bc::Push, varId);
    mOutput.push_back(pushOp);
}

void Generator::visit(BasicExpression* expression)
{
    jcVariablePtr expressionVal = jcVariable::Create(expression->getValue());
    Instruction pushOp = Instruction(bc::Push, expressionVal);
    mOutput.push_back(pushOp);
}

void Generator::visit(ListExpression* list)
{
    auto elements = list->getElements();
    for (auto element : elements) {
        element->accept(this);
    }
    Instruction pushOp = Instruction(bc::Push, jcVariable::Create((int)elements.size()));
    mOutput.push_back(pushOp);

    mOutput.push_back(Instruction(bc::Push, jcVariable::Create(lib::kLibList)));
    mOutput.push_back(Instruction(bc::Call));
}

void Generator::visit(FunctionCallExpression* expression)
{
    auto arguments = expression->getArguments();
    std::reverse(std::begin(arguments), std::end(arguments));
    for (auto arg : arguments) {
        arg->accept(this);
    }

    expression->getCallee()->accept(this);
    mOutput.push_back(Instruction(bc::Call));
}

void Generator::visit(BinaryExpression* expression)
{
    expression->getLeft()->accept(this);
    expression->getRight()->accept(this);

    TokenType op = expression->getOperator();
    bc::Op bytecodeOp;

    switch (op) {
    case TokenType::Add:
        bytecodeOp = bc::Add;
        break;
    case TokenType::Multiply:
        bytecodeOp = bc::Multiply;
        break;
    case TokenType::Subtract:
        bytecodeOp = bc::Subtract;
        break;
    case TokenType::Divide:
        bytecodeOp = bc::Divide;
        break;
    case TokenType::Greater_Than:
        bytecodeOp = bc::Greater_Than;
        break;
    case TokenType::Less_Than:
        bytecodeOp = bc::Less_Than;
        break;
    case TokenType::Less_Than_Equal:
        bytecodeOp = bc::Less_Than_Equal;
        break;
    case TokenType::Greater_Than_Equal:
        bytecodeOp = bc::Greater_Than_Equal;
        break;
    case TokenType::Equals:
        bytecodeOp = bc::Equals;
        break;
    case TokenType::Cons:
        bytecodeOp = bc::Cons;
        break;
    case TokenType::Concat:
        bytecodeOp = bc::Concat;
        break;
    default:
        JC_FAIL();
    }

    mOutput.push_back(Instruction(bytecodeOp));
}

}
