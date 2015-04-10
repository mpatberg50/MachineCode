// Variable Tree Header File
// A symbol table for variables will be represented here with 
// a binary tree, associating variable names with integer variables.
// The exterior interface will do nothing but assign to variables
// and look up their values, so the only purpose in having the
// structure definition here is to enable access to the overall tree.

#include <iostream>
#include <string>
using namespace std;

// A node anywhere in tree
class TreeNode
{
    friend class VarTree;
    private:
	string	name;		// variable name
	int	value;		// variable value
	TreeNode *left,		// sub-tree for less than
		 *right;	// sub-tree for greater than
	int key;   //key for the variable


	// Private constructor: only for use by VarTree
	TreeNode( string newName , int val )
	{
	    name.assign( newName );	// get the name
	    value = val;		// and the value
	    left = right = NULL;	// no children
		key=-1;
	}
};

// And a mechanism to store and retrieve entries in the tree.
class VarTree
{
	friend class TreeNode;
    private:
	TreeNode *root;
    public:
        VarTree()
	{
	    root = NULL;	// empty tree
	}
	void assign( string, int );
	int lookup( string );
	int size();
	int recursiveSize(TreeNode*);
	int getKey (string);	//return key for variable
	int RecursiveKey(string , TreeNode*);
	int highKey();		//returns the highest key 
	int recursiveHighKey(int x, TreeNode*);	
	void assignKey( int , string );
	void recursiveAssignKey( TreeNode *&root, string name, int key );
	bool contains (string name);
	bool recContains(string name, TreeNode*);

	bool isNull() const
	{
		return root==NULL;
	}
	
	int varValue() const
	{
		return root->value;
	}
	string varName() const
	{
		return root->name;
	}
	void moveRootRight() 
	{
		root=root->right;
	}
	void moveRootLeft() 
	{
		root= root->left;
	}



	

    private:		// these just help VarTree do its job
	void recursiveAssign( TreeNode *&, string, int );
	void recursiveLookup( TreeNode *&, string, int & );
	
	friend ostream& operator <<( ostream& stream, VarTree t);
};
