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
			default:
				JC_FAIL();
				break;
		}
		return "";
	}
	
	Instruction::Instruction(bc::Op op, std::vector<jcVariablePtr> operands) : mOp(op), mOperands(operands)
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
		std::string output = opToString(mOp) + " ";
		for (int i = 0; i < mOperands.size(); i++) {
			int operand = mOperands[i]->asInt();
			output += std::to_string(operand);
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
        for (std::string param : function->getParameters()) {
            jcVariablePtr paramVar = jcVariable::Create();
            paramVar->setString(param);
            Instruction popOp = Instruction(bc::Pop, {paramVar});
            mOutput.push_back(popOp);
        }
        function->getExpression()->accept(this);
	}

    void Generator::visit(VariableExpression *expression)
    {
        jcVariablePtr varId = jcVariable::Create();
        varId->setString(expression->getVariableName());
        Instruction pushOp = Instruction(bc::Push, {varId});
        mOutput.push_back(pushOp);
    }
	
	void Generator::visit(BasicExpression *expression)
	{
        jcVariablePtr expressionVal = jcVariable::Create();
        expressionVal->setInt(expression->getValue());
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
        jcVariablePtr functionName = jcVariable::Create();
        functionName->setString(expression->getFunctionId());
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
			default:
				JC_FAIL();
		}
		
		mOutput.push_back(Instruction(bytecodeOp, {}));
	}

}
