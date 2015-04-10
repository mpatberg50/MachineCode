// A simple RISC machine
// This structure represents a simple machine that can evaluate
// arithmetic expressions.  In addition to the expected arithmetic
// operations, it includes relational operations and branching
// for the conditional operator, and simple support for function calls.
//
// The machine is expected to have a pre-allocated stack space
// for named variables and an arbitrarily long list of temporary
// registers, both represented by integer arrays here (to hold values).
// The machine also has a separate memory area holding all of the
// instructions defined below.
//
// Most of these instructions are also immutable -- once constructed,
// they cannot be changed -- only displayed and executed.

#include <iostream>
using namespace std;

class Instruction
{
   protected:
	int valueTemp;		// register computed or tested
				// additional fields defined later
	Instruction( int temp )
	{
	    valueTemp = temp;
	}
   public:
	friend ostream& operator<<( ostream&, const Instruction & );
	virtual string toString() const = 0; // facilitates << operator
	virtual void execute( int [], int (&stacks)[100], int &, int &  )  = 0;
};

// here follow all the derived classes defining additional
// memory variables as necessary, and declaring the two functions
// above that will actually be implemented in "machine.cpp"

// one simple example is given here:
class Print: public Instruction
{
private:
	int regNum;	
   public:

	string toString() const;
	void execute( int [], int (&stacks)[100], int &, int & ) ;
	Print( int r) : Instruction(0) {regNum=r;}
};

class AddToRegister:public Instruction
{
	protected:
	int regNum;	//this is the number where the final  number will be placed in the register
	public:
		AddToRegister (int r ) :Instruction(0){regNum=r;}
	virtual string toString() const = 0;
	virtual void execute(int [], int (&stacks)[100], int &, int &) = 0;
};

class Computation: public AddToRegister
{
private:
	int firstRegNum, secRegNum;
	string sign;

public:
	Computation(int first,int second,string s,int r) : AddToRegister (r)
	{
		firstRegNum = first;
		secRegNum = second;
		sign = s;
	}
	string toString() const;
	void execute(int [],int (&stacks)[100],int &, int &) ;
};

class ValueInReg : public AddToRegister
{
private:
	int val;	//the actual value that is in the tree
public:
	ValueInReg(int v, int r) : AddToRegister (r)
	{
		val=v;
	}
	string toString() const;
	void execute(int [],int (&stacks)[100],int &, int &) ;
};

class VariableInReg: public AddToRegister
{
private:
	int valRegNum;  //indicates where the value of the variable can be found in the register, is not where thet variable will be placed
	int key;
public:
	VariableInReg(int v, int r) : AddToRegister (r) //v represents where 
	{
		key=v;
		valRegNum=v;
	}
	string toString() const;
	void execute(int [],int (&stacks)[100],int &, int &) ;
};



class AddVarToStack: public Instruction
{
private:
	int pos;	//this will represent the position that will hold the position the value can be found in the register
	int key;	
public:
	AddVarToStack(int p, int k) : Instruction (0)
	{
		key=k;
		pos=p;
	}
	string toString() const;
	void execute(int [],int (&stacks)[100],int &, int &) ;

};


/*

class Compute : public Instruction
{
private:
	string sign;
	int first, second, result;

    public:
	void execute( int [], int [], int &, int & ) const;
	string toString() const;
	Compute (int r, int argA, int argB, string s) : Instruction (1){
	sign=s;
	first= argA;
	second = argB;
	result=r;
	}

};




// also, as was pointed out in the assignment description,
// all of the arithmetic instructions are very simple and can
// be made children of a more general class (not defined here yet)
// All that is declared here is a new override to the execute method,
// and a constructor that simply calls the constructor for Compute,
// passing along all three relevant registers and the addition symbol.




class Add: public Compute
{
   public:
	void execute( int [], int [], int &, int & ) const;
	Add( int result, int argA, int argB ) : Compute(result, argA, argB, "+" ) { }
	};

class Divide: public Compute
	{
   public:
	void execute( int [], int [], int &, int & ) const;
	Divide( int result, int argA, int argB ) : Compute(result, argA, argB, "/" ) { }
	};


class Multiply: public Compute
	{
   public:
	void execute( int [], int [], int &, int & ) const;
	Multiply( int result, int argA, int argB ) : Compute(result, argA, argB, "*" ) { }
	};

class Subtract: public Compute

	{
   public:
	void execute( int [], int [], int &, int & ) const;
	Subtract( int result, int argA, int argB ) : Compute(result, argA, argB, "-" ) { }
	};
	*/