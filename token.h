// Token Header file
// This is for the beginning of an object-oriented string tokenizer
// to be used for arithmetic expressions.    The tokenizer is an
// object that returns a linked list of object tokens, which are these.

// Since lots of files want to know what a token is, there is the
// danger of redeclaration, which these next couple lines will neutralize.
// (There is an #endif at the bottom as well)
#ifndef TOKEN
#define TOKEN

// Provides some helpful functionality for understanding tokens
#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
using namespace std;
 
// Here is a definition of the token itself:
class Token {
    //  All the data members are private to keep them protected.
    private:
	bool    isInt;		// to identify the token type later
	bool	isString;
	int	value;		// value for an integer token
//	char	oper;		// character for an operator token
	string operVar;


    //  All of the methods here are public (which is not always the case)
    //  First, a couple to initialize a new token, either operator or integer
    public:
	Token(string c)		// and some constructors to
	{			//      initialize these tokens.
	    operVar = c;     
	    isInt = false;
		isString= true;
	    value = 0;		// initialize unused value
	}

	Token (char c)
	{
		operVar = c;
		isInt = false;
		isString=true;
		value=0;
	}


	Token(int i)
	{
	    value = i;
	    isInt = true;
		isString=false;
	    operVar = "";		// initialize unused value
	}

	Token()			// default constructor
	{
	    value = 0;
	    operVar = "";
	    isInt = false;
		isString=false;
	}
	//  Here are several accessor methods used to describe
	//  the token, making visible the hidden private members.
	//  No changes are permitted to this object -- only queries.
	//
	//  Placing these within the class definition allows for
	//  inline expansion at the function call point, to save
	//  the need for call and return.  In-line is only suitable
	//  for very short functions (like these and constructors).
	//
	//  The keyword 'const' at the end of these function headings
	//  promises that calling the method will not change the
	//  object.  The method only returns information.

	bool isNull() const
	{
	    return !isInt && !isString;
	}

	bool isInteger() const
	{
	    return isInt;
	}

	int integerValue() const
	{
	    return value;
	}
	
	char tokenChar() const
	{
	    return operVar.at(0);  //return the first char of string
	}

	string stringValue() const
	{
		return operVar;
	}

	//   And some functions will be postponed to an
	//   implementation file.

	friend ostream& operator <<( ostream& stream, Token &t);
};

// End of the conditional compilation
#endif