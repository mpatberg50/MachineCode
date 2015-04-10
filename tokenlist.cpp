// Token List Implementation file
//
// This tokenizer will scan a character string representing
// an expression, and will return a list of tokens.
// --- only integers and operators are supported (no names)

// The standard C library has some useful functions for us
#include <string.h>
#include <ctype.h>

// And to get the definition of a token:
#include "tokenlist.h"

#include "string.h"

//  output operation
//  Display all of the tokens in the list
ostream& operator<<( ostream &stream, TokenList &t )
{
    ListElement *curr;
    for (curr = t.head; curr != NULL; curr = curr->next)
    {
	stream << " " << curr->token;
    }
    return stream;
}

//returns true if it should be a negetive and false if a subtraction sign
bool isANegative(Token t)
{
	if(!t.isInteger() && (t.tokenChar()=='(' || t.tokenChar()=='?' || t.tokenChar()==':'))
		return true;
	else
		return false;

}

TokenList::TokenList(const char str[])
{
	ListElement* n= new ListElement;
	ListElement* t = n;
	string full="";
	int pos=0;
	head=NULL;
	string cond=" ";
	string temp = "";
	int parCounter =-1;
	
	while(str[pos]!=NULL)
	{
		//test to see if its an integer and if there is already another integer before it
		if(str[pos]>=48 && str[pos] <= 57 && n->token.isInteger())
			n->token=Token(n->token.integerValue()*10+(str[pos]-48));
	
				
		//handles conditional expression operators
		else if((str[pos] == '=' || str[pos] == '<' || str[pos] == '!' || str[pos] == '>') && (full.empty() || full.at(0)=='=' || full.at(0)=='<' || full.at(0)=='>' || full.at(0) =='!'))
		{
			if(full.empty())
				full=str[pos];
			else
				full+=str[pos];
		}
		//creates the string , if its a deffn statement it will put the function with parameters into 1 token
		else if(str[pos]>=65 || (!full.empty() && str[pos]=='('))	//second half of condition is whether it is a function
		{
			if(str[pos]=='(')
				while(str[pos]!=')' || parCounter>0)
				{
					if(str[pos]=='(')
						parCounter++;
					else if(str[pos]==')')
						parCounter--;
					temp=str[pos];
					full+=temp;
					pos++;
				}
			parCounter=-1;
			temp=str[pos];
			full+=temp;
		}

		//puts string into a token
		else if(!full.empty())
		{
			
			n = new ListElement;
			
			if(t!=NULL)
				t->next=n;
			
			n->token=Token(full);
			t=n;
			pos--;			//want it to stay at the same position
			full="";	

			if(head==NULL)
				head=n;
		}

		else if (str[pos]!= ' ')
		{
			if(( head==NULL || isANegative(t->token)) && str[pos]=='-')
			{
				n = new ListElement;
			
				if(t!=NULL)
					t->next=n;
				
				n->token=Token(0);
				t=n;

				if(head==NULL)
					head=n;
			}
			n = new ListElement;

				if(t!=NULL)
					t->next=n;
			
				if(str[pos]>=48 && str[pos]<=57)
					n->token=Token(str[pos]-48);
				else
					n->token=Token(str[pos]);
				t=n;

				if(head==NULL)
					head=n;
			
		}
		pos++;
	}
	
	if(!full.empty())
	{
		if(head!=NULL)
		{
			n = new ListElement;
			t->next=n;
		}
		n->token=Token(full);
		t=n;
	}


	if(head==NULL)
		head=t;
	current=head;
	tail=t;
}


void TokenList::PrintList() 
{
	while(!atEnd())
	{
		cout<<current->token<<" ";
		advance();
	}

	cout<<current->token<<endl;
	start();
}

void TokenList::push_front(Token t)
{
	ListElement* n = new ListElement;
	n->token=t;
	if(head!=NULL)
		n->next=head;
	if(tail==NULL)
	{
		current=n;
		tail=n;
	}
	head=n;
}

void TokenList::push_back(Token t)
{
	ListElement* n =new ListElement;
	n->token=t;
	if(tail!=NULL)
		tail->next=n;
	if(head==NULL)
	{
		current=n;
		head=n;
	}
	tail=n;
}

//  Creates an iterator to refer to the beginning of the list
ListIterator TokenList::begin()
{
    return ListIterator( this, &head );
}

//  Creates an iterator to refer after the end of the list
ListIterator TokenList::end()
{
    return ListIterator( this, &tail->next );
}
