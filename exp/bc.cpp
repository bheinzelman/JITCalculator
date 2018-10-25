//  bc.cpp

#include "bc.hpp"
#include "jc.h"

#include <string>
#include <algorithm>

namespace bc
{
	
	static std::string opToString(bc::Op op) {
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
			default:
				JC_FAIL();
				break;
		}
		return "";
	}
	
	Instruction::Instruction(bc::Op op, std::vector<jcVariablePtr> operands) : mOp(op), mOperands(operands)
	{
	}

    Instruction::Instruction(bc::Op op) : mOp(op)
    {
    }
	
	int Instruction::numOperands() const
	{
		return (int)mOperands.size();
	}
	
	jcVariablePtr Instruction::getOperand(int idx) const
	{
		JC_ASSERT(idx < mOperands.size());
		return mOperands[idx];
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

		for (int i = 0; i < mOperands.size(); i++) {

            if (mOperands[i]->getType() == jcVariable::TypeInt) {
                int operand = mOperands[i]->asInt();
                output += std::to_string(operand);
            } else {
                output += mOperands[i]->asString();
            }
			if (i < mOperands.size() - 1) {
				output += ", ";
			}
		}
		return output;
	}
	
	Generator::Generator(std::shared_ptr<Node> root) : mRoot(root)
	{
		
	}
	
	std::vector<Instruction> Generator::getInstructions()
	{
		mRoot->accept(this);
		return mOutput;
	}
	
	void Generator::visit(FunctionDecl *function)
	{
        std::string functionName = function->getId();

        Instruction functionLabel = Instruction(bc::Label, {jcVariable::Create(functionName)});
        mOutput.push_back(functionLabel);

        for (std::string param : function->getParameters()) {
            jcVariablePtr paramVar = jcVariable::Create(param);
            Instruction popOp = Instruction(bc::Pop, {paramVar});
            mOutput.push_back(popOp);
        }

        std::string endLabel = functionName + ".end";

        if (function->getGuards().size()) {
            // do guard expressions
            for (int i = 0; i < function->getGuards().size(); i++) {
                std::shared_ptr<Guard> guard = function->getGuards()[i];
                guard->getGuardExpression()->accept(this);

                std::string label = functionName + "." + std::to_string(i);
                Instruction jmpInstruction = Instruction(bc::JmpTrue, {jcVariable::Create(label)});
                mOutput.push_back(jmpInstruction);
            }

            // jump to default if none of guards hit
            std::string lastLabel = functionName + "." + std::to_string(function->getGuards().size());
            mOutput.push_back(Instruction(bc::Jmp, {jcVariable::Create(lastLabel)}));

            // now do the guard bodies

            for (int i = 0; i < function->getGuards().size(); i++) {
                std::shared_ptr<Guard> guard = function->getGuards()[i];
                std::string label = functionName + "." + std::to_string(i);

                mOutput.push_back(Instruction(bc::Label, {jcVariable::Create(label)}));
                guard->getBodyExpression()->accept(this);

                mOutput.push_back(Instruction(bc::Jmp, {jcVariable::Create(endLabel)}));
            }

            mOutput.push_back(Instruction(bc::Label, {jcVariable::Create(lastLabel)}));
        }

        function->getDefaultExpression()->accept(this);

        mOutput.push_back(Instruction(bc::Label, {jcVariable::Create(endLabel)}));

        Instruction returnInstruction = Instruction(bc::Ret, {});
        mOutput.push_back(returnInstruction);
	}

    void Generator::visit(Guard *guard)
    {
        JC_FAIL();
    }

    void Generator::visit(VariableExpression *expression)
    {
        jcVariablePtr varId = jcVariable::Create(expression->getVariableName());
        Instruction pushOp = Instruction(bc::Push, {varId});
        mOutput.push_back(pushOp);
    }
	
	void Generator::visit(BasicExpression *expression)
	{
        jcVariablePtr expressionVal = jcVariable::Create(expression->getValue());
		Instruction pushOp = Instruction(bc::Push, {expressionVal});
		mOutput.push_back(pushOp);
	}

    void Generator::visit(FunctionCallExpression *expression)
    {
        auto arguments = expression->getArguments();
        std::reverse(std::begin(arguments), std::end(arguments));
        for (auto arg : arguments) {
            arg->accept(this);
        }

        jcVariablePtr functionName = jcVariable::Create(expression->getFunctionId());
        Instruction callInstruction = Instruction(bc::Call, {functionName});
        mOutput.push_back(callInstruction);
    }
	
	void Generator::visit(BinaryExpression *expression)
	{
		expression->getLeft()->accept(this);
		expression->getRight()->accept(this);
		
		Token op = expression->getOperator();
		bc::Op bytecodeOp;
		
		switch (op) {
			case Token::Add:
				bytecodeOp = bc::Add;
				break;
			case Token::Multiply:
				bytecodeOp = bc::Multiply;
				break;
			case Token::Subtract:
				bytecodeOp = bc::Subtract;
				break;
			case Token::Divide:
				bytecodeOp = bc::Divide;
				break;
            case Token::Greater_Than:
                bytecodeOp = bc::Greater_Than;
                break;
            case Token::Less_Than:
                bytecodeOp = bc::Less_Than;
                break;
            case Token::Less_Than_Equal:
                bytecodeOp = bc::Less_Than_Equal;
                break;
            case Token::Greater_Than_Equal:
                bytecodeOp = bc::Greater_Than_Equal;
                break;
            case Token::Equals:
                bytecodeOp = bc::Equals;
                break;
			default:
				JC_FAIL();
		}
		
		mOutput.push_back(Instruction(bytecodeOp, {}));
	}

}
