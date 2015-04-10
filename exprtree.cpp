// Expression Tree Implementation File
// These functions manage an expression tree that describes
// arithmetic expressions using integer variables and values.
// Some functions construct a tree from a character string that
// represents the expression, and then the tree can be traversed
// and evaluated.
#include <iostream>
#include <sstream>
using namespace std;
#include "exprtree.h"
#include "tokenlist.h"

// Outputting any tree node will simply output its string version
ostream& operator<<( ostream &stream, const ExprNode &e )
{
    return stream << e.toString();
}

// A Value is just an integer value -- easy to evaluate
// Unfortunately, the string class does not have a constructor for it
string Value::toString() const
{
    ostringstream convert;	// output string stream
    convert << value;		// 'output' into the stream
    return convert.str();	// and extract its string equivalent
}

int Value::evaluate( VarTree &v ) const
{
    return value;
}

//  A variable is just an alphabetic string -- easy to display
//  TO evaluate, would need to look it up in the data structure
string Variable::toString() const
{
    return name;
}

int Variable::evaluate( VarTree &v ) const
{
    return v.lookup( name );
}

string Operation::toString() const
{
	return oper;
}

int Operation::evaluate(VarTree &v) const
{
	if(toString()=="+")
		return left->evaluate(v) + right->evaluate(v);
	else if(toString()=="-")
		return left->evaluate(v) - right->evaluate(v);
	else if(toString()=="/")
		return left->evaluate(v) / right->evaluate(v);
	else if (toString()=="%")
		return left->evaluate(v) % right->evaluate(v);
	else if (toString()=="*")
		return left->evaluate(v) * right->evaluate(v);
	else if (toString()=="=")
	{
		v.assign(left->toString(), right->evaluate(v));
		return right->evaluate(v);
	}
	else if(toString()=="==")
	{
		if(left->evaluate(v)==right->evaluate(v))
			return 1;
		return 0;
	}
	else if(toString()=="!=")
	{
		if(right->evaluate(v)==left->evaluate(v))
			return 0;
		return 1;
	}
	else if (toString()=="<")
	{
		if(left->evaluate(v)<right->evaluate(v))
			return 1;
		return 0;
	}	
	else if (toString()==">")
	{
		if(left->evaluate(v)>right->evaluate(v))
			return 1;
		return 0;
	}
	else if (toString()=="<=")
	{
		if(left->evaluate(v)<=right->evaluate(v))
			return 1;
		return 0;
	}
	else if (toString()==">=")
	{
		if(left->evaluate(v)>=right->evaluate(v))
			return 1;
		return 0;
	}
}

string Conditional::toString() const
{
	return test->toString();
}

int Conditional::evaluate(VarTree &v) const
{
	if(test->evaluate(v)==1) //1 = true
		return trueCase->evaluate(v);
	else if (test->evaluate(v)==0) //0 = false
		return falseCase->evaluate(v);
}

string Function::toString() const
{
	return func.name;
}

int Function::evaluate(VarTree &v) const
{
	return func.functionBody->evaluate(*func.locals);
}

//these will convert the expr tree into instructions

//regCounter = register counter = where you are in the register
//instrCounter = instruction counter = where you are in the instruction array

void Value::translateTree(Instruction *prog[], int &regCounter, int &instrCounter, VarTree &v) 
{
//  places instruction      creates value that will be placed into register
	prog[instrCounter] = new ValueInReg(value,regCounter);
	regCounter++;
	instrCounter++;

}



void Variable::translateTree(Instruction *prog[],int &regCounter, int &instrCounter, VarTree &v) 
{
	
	if(v.getKey(name)!=-1)
	{
		//add variable to the register
		prog[instrCounter] = new VariableInReg(v.getKey(name),regCounter);
		instrCounter++;
		
		regCounter++;
	}
	else
	{
		//need to create instruction to add variable into stack

		prog[instrCounter] = new AddVarToStack(regCounter-1,v.highKey()+1);	//this will be the opposite of the equal sign and also the last completed register
	
		//give variable a key in the tree
		int tempKey = v.highKey();
		key=tempKey+1;
		v.assignKey(key,name);


		instrCounter++;

		//add variable to the register

		prog[instrCounter] = new VariableInReg(key,regCounter);
		instrCounter++;
		regCounter++;
		//make sure to give both of the instructions a key
	}

}

void Operation::translateTree(Instruction *prog[],int &regCounter, int &instrCounter, VarTree &v) 
{
	int l,r;
	if(oper=="=")
	{
		//do the right side first
		right->translateTree(prog,regCounter,instrCounter,v);
		//give this number to the variable
		
		//have the variables assign its value based on the answer to the right side
		left->translateTree(prog,regCounter,instrCounter,v);
	}
	//if it is not an assignment statement
	else
	{

		left->translateTree(prog,regCounter,instrCounter,v);
		l = regCounter-1;	//gets where the first argument will be from (the place on the register)
		right->translateTree(prog,regCounter,instrCounter,v);
		r = regCounter-1; //gets where the second argument will be from (the place on the register)
		prog[instrCounter] = new Computation(l,r,oper,regCounter);
		regCounter++;
		instrCounter++;
	}



}

void Conditional::translateTree(Instruction *prog[], int &regCounter, int &instrCounter,VarTree &v) 
{}

void ExprNode::translateTree(Instruction *prog[], int &regCounter, int &instrCounter,VarTree &v)
{}

void Function::translateTree(Instruction *prog[], int &regCounter, int &instrCounter,VarTree &v)
{
	func.functionBody->translateTree(prog,regCounter,instrCounter,v);
}
