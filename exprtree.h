//  Expression Tree Header File
//  Describes the elements of an expression tree, using
//  derived classes to represent polymorphism.
//  All objects in this structure are immutable --
//  once constructed, they are never changed.
//  They only be displayed or evaluated.
#include <iostream>
using namespace std;
#include "vartree.h"
#include "funmap.h"
#include "machine.h"
class ExprNode
{
	private:
	int	registerNumber;

    public:

	void setRegNum(int r)
	{
		registerNumber=r;
	}
	int getRegNum()
	{
		return registerNumber;
	}

	virtual void translateTree(Instruction *prog[], int &regCounter, int &instrCounter, VarTree &v) ;
    friend ostream& operator<<( ostream&, const ExprNode & );
    virtual string toString() const = 0;	// facilitates << operator
    virtual int evaluate( VarTree &v ) const = 0;  // evaluate this node
};

class Value: public ExprNode
{
    private:
	int value;
    public:
	string toString() const;	// facilitates << operator
	int evaluate( VarTree &v ) const;
	void translateTree(Instruction *prog[], int &regCounter, int &instrCounter,VarTree &v) ;
	Value(int v)
	{
	    value = v;
	}
};

class Variable: public ExprNode
{
    private:
	string name;
	int key;
    public:
	string toString() const ;	// facilitates << operator
	int evaluate( VarTree &v ) const;
	void translateTree(Instruction *prog[], int &regCounter, int &instrCounter,VarTree &v);
	Variable(string var)
	{
	    name = var;
	}
};

class Operation: public ExprNode
{
    private:
	string oper;
	ExprNode *left, *right;	 // operands
    public:
	string toString() const;	// facilitates << operator
	int evaluate( VarTree &v ) const;
	void translateTree(Instruction *prog[], int &regCounter, int &instrCounter,VarTree &v) ;
	Operation( ExprNode *l, string o, ExprNode *r )
	{
	    left = l;
	    right = r;
	    oper = o;
	}
};

class Conditional: public ExprNode
{
    private:
	ExprNode *test, *trueCase, *falseCase;
    public:
	string toString() const;	// facilitates << operator
	int evaluate(VarTree &v) const;
	void translateTree(Instruction *prog[], int &regCounter, int &instrCounte,VarTree &vr) ;
	Conditional( ExprNode *b, ExprNode *t, ExprNode *f)
	{
	    test = b;
	    trueCase = t;
	    falseCase = f;
	}
};

class Function: public ExprNode
{
	private:
	FunDef func;

	public:
	string toString() const;
	int evaluate(VarTree &v) const;
	void translateTree(Instruction *prog[], int &regCounter, int &instrCounter,VarTree &v) ;
	Function(FunDef f, int par[10])
	{
		string s ="";
		func=f;
		func.locals= new VarTree();

		for(int x=0; x<10 && par[x]!=NULL; x++)
		{
			func.locals->assign(f.parameter[x],par[x]);
		}
	}
};

