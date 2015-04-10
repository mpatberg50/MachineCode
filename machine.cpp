// Machine Implementation File
// This implementation represents a simple machine that can evaluate
// arithmetic expressions.  In addition to the expected arithmetic
// operations, it includes relational operations and branching
// for the conditional operator, and simple support for function calls.
//
// The machine is expected to have a pre-allocated stack space
// for named variables and an arbitrarily long list of temporary
// registers, both represented by integer arrays here (to hold values).
// The machine also has a separate memory area holding all of the
// instructions defined below.

#include <iostream>
#include <sstream>
using namespace std;

#include "machine.h"

string convertIntToString (int x)
{
	string s="";
	char c=' ';
	while(x>=10)
	{
		c=(x%10) + 48;
		s= c +s;
		x=x/10;
	}
	c=(x%10)+ 48;
	s= c +s;

	return s;
}


int compute (int x, int y, string s)
{
	if(s=="+")
		return x+y;
	else if (s=="-")
		return x-y;
	else if (s=="*")
		return x*y;
	else if (s=="/")
		return x/y;
	else if (s=="%")
		return x%y;
}

	
ostream& operator<<( ostream &stream, const Instruction &i )
{
    return stream << i.toString()<<endl;
}

//  Here follow implementations of toString() and execute()
//  for all of the derived classes of the Instruction.

void Print::execute(int registers[],int (&stacks)[100],int &stackPointer, int &programCounter) 
{
	cout<<registers[regNum]<<endl;	//prints out the last and final answer
}

void Computation::execute(int registers[],int (&stacks)[100],int &stackPointer, int &programCounter) 
{
	int temp = compute(registers[firstRegNum],registers[secRegNum],sign);
	registers[regNum]=temp;
}

void ValueInReg::execute(int registers[],int (&stacks)[100],int &stackPointer, int &programCounter) 
{
	registers[regNum]=val;
}

void VariableInReg::execute(int registers[],int (&stacks)[100],int &stackPointer, int &programCounter) 
{

	registers[regNum] = stacks[key]; //this will find the value of the variable in the 
}

void AddVarToStack::execute(int registers[],int (&stacks)[100],int &stackPointer, int &programCounter) 
{
	if(stackPointer>20)
		stackPointer=0;
	stacks[stackPointer] = registers[pos];


	key = stackPointer;
	stackPointer++;
}

string ValueInReg::toString() const
{
	return "T" + convertIntToString(regNum) + "=" + convertIntToString(val); //Ex: T1 = 10
}

string AddVarToStack::toString() const
{
	return "stack[ " + convertIntToString(key) + "] = T" + convertIntToString(pos); //Ex: stack[0] = T1
}

string VariableInReg::toString() const
{
	return "T" + convertIntToString(regNum) + "= stack[" + convertIntToString(key) +"]"; //Ex: T1 = stack[0]
}

string Computation::toString() const
{
	return "T" + convertIntToString(regNum) + "= T" + convertIntToString(firstRegNum) + " " + sign + " T" + convertIntToString(secRegNum); //Ex: T5 = T2 - T4 
}

string Print::toString() const
{
	return "Printed out: T" + convertIntToString(regNum); //Ex: Printed out: T4
}
