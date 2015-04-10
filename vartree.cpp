// Variable Tree Implementation File
// This is a simple binary search tree associating variables
// with integer values.
#include <iostream>
#include <string>
using namespace std;

#include "vartree.h"

//  recursiveAssign
//  A recursive tree-traversal function to assign to a variable.
//  Parameters:
//  	node	(modified TreeNode ptr)	tree to insert into
//  	name	(input string)		name of variable
//  	value	(input integer)		value to assign
//  NOTE: node May be a null pointer, indicating an empty tree
void VarTree::recursiveAssign( TreeNode *&root, string name, int value )
{
	if(root==NULL)
		root= new TreeNode(name,value);
	else if(root->name==name)
		root = new TreeNode(name,value);
	else if(name > root->name)   //if true name would be alphabetically before root->name
	{
		recursiveAssign(root->left,name,value);
	}
	else if (name < root->name)  //if true, name would be alphabetically after root->name
	{
		recursiveAssign(root->right,name,value);
	}
}

//  assign
//  Assigns a value to a variable.
//  If the variable does not yet exist, it is created.
//  Parameters:
//  	name	(input string)		name of variable
//  	value	(input integer)		value to assign
void VarTree::assign( string name, int value )
{
    recursiveAssign( root, name, value );

}

//  recursiveLookup
//  A recursive tree-traversal function to search for a variable.
//  If the variable does not exist, it is created with a value of 0.
//  Parameters:
//  	node	(modified TreeNode ptr)	tree to insert into
//  	name	(input string)		name of variable
//  	value	(output integer)	value of variable
//  NOTE: node May be a null pointer, indicating an empty tree
void VarTree::recursiveLookup( TreeNode *&root, string name, int &value )
{
	if(root->name == name)
		value=root->value;
	else if(root==NULL)
		value = 0;
	else if(name > root->name)   //if true name would be alphabetically before root->name
	{
		recursiveLookup(root->left,name,value);
	}
	else if (name < root->name)  //if true, name would be alphabetically after root->name
	{
		recursiveLookup(root->right,name,value);
	}


}

//  lookup
//  Searches for a variable to get its value
//  If the variable does not yet exist, it is created.
//  Parameters:
//  	name	(input char array)	name of variable
//  Returns:				value of variable
int VarTree::lookup( string name )
{
    int value;
    recursiveLookup( root, name, value );
    return value;
}

int VarTree::RecursiveKey(string name, TreeNode *root)
{
	if(root==NULL)
	{
		return -1;
	}
	if (root->name==name)
	{
		return root->key;
	}
	else if(name > root->name)   //if true name would be alphabetically before root->name
	{
		return RecursiveKey(name,root->left);
	}
	else if (name < root->name)  //if true, name would be alphabetically after root->name
	{
		return RecursiveKey(name,root->right);
	}

}


int VarTree::getKey(string name)
{
	return RecursiveKey(name,root);
}

int VarTree::size()
{
	return	recursiveSize(root);
}


int VarTree::recursiveSize(TreeNode *root)
{
	if(root==NULL)
		return 0;
	else
		return recursiveSize(root->right) + recursiveSize(root->left) + 1;
}

int VarTree::highKey()
{
	int x= -1;
	return recursiveHighKey(x, root);
}

int VarTree::recursiveHighKey(int high, TreeNode *node)
{
	int left=0;
	int right=0;
	
	if(node==NULL)
		return high;
	else if (high<node->key)
		high=node->key;

	left = recursiveHighKey(high,node->left);
	right= recursiveHighKey(high,node->right);
	
	if(left>high)
		high=left;
	if(right>high)
		high=right;
	return high;
}

void VarTree::assignKey(int key, string name)
{
	
	recursiveAssignKey(root,name, key );
}

void VarTree::recursiveAssignKey( TreeNode *&root, string name, int key )
{
	if(root==NULL)
		root->key=key;
	else if(root->name==name)
		root->key=key;
	else if(name > root->name)   //if true name would be alphabetically before root->name
	{
		recursiveAssignKey(root->left,name,key);
	}
	else if (name < root->name)  //if true, name would be alphabetically after root->name
	{
		recursiveAssignKey(root->right,name,key);
	}
}

bool VarTree::contains(string name)
{
	return recContains(name,root);
}

bool VarTree::recContains(string name, TreeNode *root)
{
	if(root==NULL)
		return false;
	else if (root->name==name)
	{
		return true;
	}
	else if(name > root->name)   //if true name would be alphabetically before root->name
	{
		return recContains(name,root->left);
	}
	else if (name < root->name)  //if true, name would be alphabetically after root->name
	{
		return recContains(name,root->right);
	}

}
ostream& operator<<( ostream &stream, VarTree t)
{
	VarTree left=t;
	VarTree right=t;

	if (t.isNull())
	{
		return stream<<"";
	}
	else
	{
		left.moveRootLeft();
		right.moveRootRight();
		return stream<<t.varName()<<": "<<t.varValue()<<endl<<left<<right;
	}
	
}
