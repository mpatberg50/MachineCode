#include <iostream>
using namespace std;
#include "tokenlist.h"
#include "exprtree.h"
#include "funmap.h"

bool isVariable(Token t)
{
	return !t.isInteger() && !(t.tokenChar() =='=' || t.tokenChar()== '+' || t.tokenChar()=='-' || t.tokenChar()=='/' || t.tokenChar()=='*' || t.tokenChar()=='%' || t.tokenChar()== '(' || t.tokenChar() == ')');
}

bool isOper (Token t)
{
	return t.tokenChar() =='=' || t.tokenChar()== '+' || t.tokenChar()=='-' || t.tokenChar()=='/' || t.tokenChar()=='*' || t.tokenChar()=='%';

}

void createNode (ExprNode *&node, Token t)
{
	if(t.isInteger())
		node = new Value(t.integerValue());
	else if(isVariable(t))
		node = new Variable(t.stringValue());
	else
		node = new Operation (NULL, t.stringValue(),NULL);

}
bool isDMP(Token t)
{
	if(t.isInteger())
		return false;
	if(t.tokenChar()=='/' || t.tokenChar()=='*' || t.tokenChar()=='%')
		return true;
	return false;
}


//sees if there is +,- outside parenthesis
bool asOutsideParenthesis(TokenList list)
{
	ListIterator iter= list.begin();
	
	while(iter!=list.end())
	{
		if(iter.token().stringValue()=="(")
			while(iter.token().stringValue()!=")")
				iter.advance();
		if(iter.token().stringValue()=="+" || iter.token().stringValue()=="-")
			return true;
		iter.advance();
	}
	return false;
}
//sees if there is *,/,% outside parenthesis
bool mpdOutsideParenthesis(TokenList list)
{
	ListIterator iter= list.begin();
	
	while(iter!=list.end())
	{
		if(iter.token().stringValue()=="(")
			while(iter.token().stringValue()!=")")
				iter.advance();
		if(iter.token().stringValue()=="*" || iter.token().stringValue()=="/"|| iter.token().stringValue()=="%")
			return true;
		iter.advance();
	}
	return false;
}
//checks for equal sign outside parenthesis
bool eqOutsideParenthesis(TokenList list)
{
	ListIterator iter= list.begin();
	
	while(iter!=list.end())
	{
		if(iter.token().stringValue()=="(")
			while(iter.token().stringValue()!=")")
				iter.advance();
		if(iter.token().stringValue()=="=")
			return true;
		iter.advance();
	}
	return false;
}
//checks if there is a conditional operation in the statement
bool containsCondOp(TokenList list)
{	
	ListIterator iter= list.begin();
	list.start();

	while(iter!=list.end())
	{
		if(iter.token().stringValue()=="(")
			while(iter.token().stringValue()!=")")
				iter.advance();
		if(!iter.token().isInteger() && (iter.token().stringValue().at(0)=='!' || iter.token().stringValue().at(0)=='<' || iter.token().stringValue().at(0)=='>' || iter.token().stringValue()=="=="))
			return true;
		iter.advance();
	}
	return false;
}




//checks for conditional statements
bool condStatement(TokenList list)
{
	while(!list.atEnd())
	{
		if(list.nextToken().stringValue()=="?")
			return true;
		list.advance();
	}
	return false;
}




// tests to see if function inside of parenthesis
bool isFunction(TokenList l)
{
	bool first =false;
	do
	{
		for(int x= 0 ; x<l.nextToken().stringValue().length(); x++)
		{
			if(l.nextToken().stringValue().at(x)=='(')
				first=true;
			if(l.nextToken().stringValue().at(x)==')' && first ==true)
				return true;
		}
		first=false;
		if(!l.atEnd())	
			l.advance();
	}while(!l.atEnd());

	return false;	
}
TokenList combine(TokenList first, TokenList second)
{
	for(;!second.atEnd();second.advance())
		first.push_back(second.nextToken());

	first.push_back(second.nextToken());

	first.PrintList();
	return first;
}

void empty(TokenList &t)
{
	TokenList x;
	t=x;
}

ExprNode* createETree (TokenList list, bool first, FunctionDef func, VarTree &v)
{
	TokenList left, right;
	if(first)
	{
		if(!list.atEnd())
			list.advance();
		//handles assignments

		if(list.nextToken().stringValue()=="=")
		{
			list.start();
			left.push_back(list.nextToken());
			list.advance();
			while(!list.atEnd())
			{
				list.advance();
				right.push_back(list.nextToken());
			}
			return new Operation (createETree(left,false,func,v),"=",createETree(right,false,func,v));
		}
		else
		{

			list.start();	
			return createETree(list,false,func,v);
		}
	}
	else
	{
		list.start();

		if(condStatement(list))
		{
			TokenList cond,first,second;
			for(;list.nextToken().stringValue()!="?";list.advance())
			{
				cond.push_back(list.nextToken());			
			}
			list.advance();
			for(;list.nextToken().stringValue()!=":";list.advance())
			{
				first.push_back(list.nextToken());
			}
			list.advance();
			for(;!list.atEnd();list.advance())
			{
				second.push_back(list.nextToken());
			}
			second.push_back(list.nextToken());

			return new Conditional(createETree(cond,false,func,v),createETree(first,false,func,v),createETree(second,false,func,v));
		}

		else if(asOutsideParenthesis(list))
		{
			list.start();
			int parCounter=0;
			while(list.nextToken().stringValue()!="+" &&list.nextToken().stringValue()!="-")
			{
				//gets out of parenthetic expressions
				if(list.nextToken().stringValue()=="(")
				{
					parCounter++;
					while(parCounter!=0)
					{
						left.push_back(list.nextToken());
						list.advance();
						if(list.nextToken().stringValue()=="(")
							parCounter++;
						else if(list.nextToken().stringValue()==")")
							parCounter--;
					}
				}
				left.push_back(list.nextToken());
				list.advance();
			}
			string s = list.nextToken().stringValue();

			while(!list.atEnd())
			{					
				list.advance();
				right.push_back(list.nextToken());

			}
			
			TokenList mod;
			while(asOutsideParenthesis(right))
			{	
				empty(mod);
				left.push_back(Token(s));	//puts operation to the back
				while(right.nextToken().stringValue()!="+" &&right.nextToken().stringValue()!="-")
				{
					//gets out of parenthetic expressions
					if(right.nextToken().stringValue()=="(")
					{
						parCounter++;
						while(parCounter!=0)
						{
							left.push_back(right.nextToken());
							right.advance();
							if(right.nextToken().stringValue()=="(")
								parCounter++;
							else if(right.nextToken().stringValue()==")")
								parCounter--;
						}
					}
				left.push_back(right.nextToken());
				right.advance();
				}
				s = right.nextToken().stringValue();
				
				while(!right.atEnd())
				{				
					right.advance();
					mod.push_back(right.nextToken());
				}		
				right=mod;
			}
			return new Operation(createETree(left,false,func,v),s,createETree(right,false,func,v));
			
		}
	
		//locates if there is a '*', '/', or '%' outside the parenthesis
		else if(mpdOutsideParenthesis(list))
		{
			int parCounter=0;
			while(list.nextToken().stringValue()!="*" &&list.nextToken().stringValue()!="/" &&list.nextToken().stringValue()!="%")
			{
				if(list.nextToken().stringValue()=="(")
				{
					parCounter++;
					while(parCounter!=0)
					{
						left.push_back(list.nextToken());
						list.advance();
						if(list.nextToken().stringValue()=="(")
							parCounter++;
						else if(list.nextToken().stringValue()==")")
							parCounter--;
					}
				}
				left.push_back(list.nextToken());
				list.advance();
			}
			string s = list.nextToken().stringValue();
			
			while(!list.atEnd())
			{				list.advance();
				right.push_back(list.nextToken());

			}


			TokenList mod;
			while(mpdOutsideParenthesis(right))
			{	
				empty(mod);
				left.push_back(Token(s));	//puts operation to the back
				while(right.nextToken().stringValue()!="*" &&right.nextToken().stringValue()!="/" && right.nextToken().stringValue()!="%")
				{
					//gets out of parenthetic expressions
					if(right.nextToken().stringValue()=="(")
					{
						parCounter++;
						while(parCounter!=0)
						{
							left.push_back(right.nextToken());
							right.advance();
							if(right.nextToken().stringValue()=="(")
								parCounter++;
							else if(right.nextToken().stringValue()==")")
								parCounter--;
						}
					}
				left.push_back(right.nextToken());
				right.advance();
				}
				s = right.nextToken().stringValue();
				
				while(!right.atEnd())
				{				
					right.advance();
					mod.push_back(right.nextToken());
				}		
				right=mod;
			}
			return new Operation(createETree(left,false,func,v),s,createETree(right,false,func,v));
		}

		//locates if there is an '=' outside of parenthesis
		else if(eqOutsideParenthesis(list))
		{
			int parCounter=0;
			while(list.nextToken().stringValue()!="=")
			{
				if(list.nextToken().stringValue()=="(")
				{
					parCounter++;
					while(parCounter!=0)
					{
						left.push_back(list.nextToken());
						list.advance();
						if(list.nextToken().stringValue()=="(")
							parCounter++;
						else if(list.nextToken().stringValue()==")")
							parCounter--;
					}
				}
				left.push_back(list.nextToken());
				list.advance();
			}
			while(!list.atEnd())
			{				
				list.advance();
				right.push_back(list.nextToken());

			}
			return new Operation(createETree(left,false,func,v),"=",createETree(right,false,func,v));
		}
			
		else if(containsCondOp(list))
		{
			int parCounter=0;
			list.start();
			while(list.nextToken().isInteger() || (list.nextToken().stringValue()!="=="  && list.nextToken().stringValue().at(0)!='<' &&list.nextToken().stringValue().at(0)!='>'&&list.nextToken().stringValue()!="!="))
			{
				if(list.nextToken().stringValue()=="(")
				{
					parCounter++;
					while(parCounter!=0)
					{
						left.push_back(list.nextToken());
						list.advance();
						if(list.nextToken().stringValue()=="(")
							parCounter++;
						else if(list.nextToken().stringValue()==")")
							parCounter--;
					}
				}
				left.push_back(list.nextToken());
				list.advance();
			}
			string s = list.nextToken().stringValue();
			while(!list.atEnd())
			{				
				list.advance();
				right.push_back(list.nextToken());
			}
		
			return new Operation(createETree(left,false,func,v),s,createETree(right,false,func,v));

		}
		//the only three possiblities remaining would be a stand alone variable, number, or parenthetic expression
		else if(list.nextToken().isInteger())
			return new Value(list.nextToken().integerValue());
		else if(isFunction(list))
		{
			int param[10];
			int x=0;
			string s="";
			for(x=0; list.nextToken().stringValue().at(x)!='('; x++)
				s+=list.nextToken().stringValue().at(x);

			
			int parNum=0; //dictates the place in the array
			x++;	//gets past the first parenthesis into the parameters
			int tempPar=0;
			string paramExpression=""; //gets the string that is the actual parameter
			int count=0;
			char parExpression[30];

			while(list.nextToken().stringValue().at(x)!=')' )
			{
				for(x; x<list.nextToken().stringValue().length()-1 && list.nextToken().stringValue().at(x)!=',';x++)
				{	
					parExpression[count]=list.nextToken().stringValue().at(x);
					count++;
				}

				parExpression[count]=NULL;	//creates the NULL that will allow you to create the token list ending
				TokenList paramList(parExpression);
				param[parNum]=createETree(paramList,false,func,v)->evaluate(v);

				parNum++;
			}
			return new Function(func[s],param);
		}
		else if(list.nextToken().stringValue()!="(")
		{
			if(!v.contains(list.nextToken().stringValue()))
				v.assign(list.nextToken().stringValue(),0);

			return new Variable(list.nextToken().stringValue());
		}
		//precondition: will be only a parenthetic expression
		//this will take out the parentheis for the expression and re do them
		else
		{
			list.advance();
			while(!list.atEnd())
			{
				left.push_back(list.nextToken());
				list.advance();
			}
			return createETree(left,false,func,v);
		}
	}
	
}

//adds function to the map
void addFunction (FunctionDef &funs, TokenList list, VarTree v)
{
	//gets past the initial "deffn"
	list.advance();

	FunDef newFunction;

	string temp="", function=list.nextToken().stringValue();

	int x=0;
	//this will get the function
	for(x = 0; function.at(x) != '('; x++)
		temp+=function.at(x);
	x++;
	newFunction.name=temp;
	int parNum=0;
	temp="";

	//adds parameters to function
	while(function.at(x)!=')')
	{
		if(function.at(x)==',')
			temp="";
		else
			temp+=function.at(x);

		if(function.at(x+1)==')' || function.at(x+1)==',')
		{
			newFunction.parameter[parNum]=temp;
			parNum++;
		}
		x++;
	}
	list.advance();

	TokenList exp;

	while(!list.atEnd())
	{
		list.advance();
		exp.push_back(list.nextToken());
	}

	//creates an expression tree from the definition side
	newFunction.functionBody=createETree(exp,false,funs,v);

	funs[newFunction.name]=newFunction;
}


void compile( const char expr[], VarTree &vars, FunctionDef &funs,Instruction *prog[], int &pBegin, int &pEnd )
{
  static int result = 0;		// the return value
  Token prevValue(result);		// and as a token, if needed

   ExprNode *head;
	TokenList list(expr);

	/*
	if(list.nextToken().stringValue()=="deffn")
	{
		addFunction(funs,list,vars);
		return 0;
	}*/
	
   if (list.nextToken().stringValue()=="+" || list.nextToken().stringValue()=="-" || list.nextToken().stringValue()=="/" || list.nextToken().stringValue()=="%" || list.nextToken().stringValue()=="*" )
   {
        list.push_front( prevValue );
    }

	head=createETree(list,true,funs,vars);
	int instr=pEnd;
	static int reg = 0;

	list.PrintList();
	head->translateTree(prog,reg,instr,vars);

	prog[instr] = new Print(reg-1);	//final register number

	instr++;
	pBegin=0;
	pEnd=instr;

}