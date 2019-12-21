#include <iostream>
#include <vector> 
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib> 
#include <string>
#include <cmath>
#include <fstream>
#define MAXSIZE 2000
using namespace std; 
int size=0; //Size of heap
int min=0;
int tot=0;
string s;
string ab="(";
string cd=",";
string ef="),";
int globalcounter=0;
ofstream oput;

struct minHeap 
{ 
	int buildingNum, executed_time, total_time; 
};
struct minHeap heap[MAXSIZE]; //creatng an array of objects, where MAXSIZE is the maximum number of buildings ie 2000
void Heapify(minHeap *p, int i, int n); //Prototype for function Heapify
int parent(int i);
enum COLOUR {RED,BLACK}; 
struct minHeap tempHeap[10];
int tempSize=0;
vector<int> bNum_vector;
vector<int> eTime_vector;
vector<int> tTime_vector;
void printHeap()
{
	for(int i=0;i<size;i++)
	{
		cout<<"\nbnum: "<<heap[i].buildingNum<<" executed time: "<<heap[i].executed_time<<" total time: "<<heap[i].total_time<<" global counter: "<<globalcounter;
	}
	cout<<endl;
}
class RBTNode 
{ 
	public: int bNum, eTime, tTime; 
			COLOUR colour; 
			RBTNode *left, *right, *parent;
			RBTNode (int b, int t);
			RBTNode* returnUncleNode();
			bool isLeftChild();
			bool isRightChild();
			RBTNode* returnSiblingNode();
			bool hasRedChild();		
}; 

class RBTree 
{ 
	public: RBTNode *root; 
			void rotateRR(RBTNode *x);
			void rotateLL(RBTNode *x);
			void swapColours(RBTNode *a, RBTNode *b);
			void swapValues(RBTNode *a, RBTNode *b);
			void fixInsertViolation(RBTNode *x);
			RBTNode *findReplacement(RBTNode *a);
			void fixDeleteViolation(RBTNode *x);
			void performDelete(RBTNode *v);
			void RBTDeleteBuilding(int n);
			RBTNode* findNode(int n);
			void RBTInsertBuilding(int n, int m);
			void inorderTraversal(RBTNode *x, int a); 
			void RBTPrintBuilding(int a);
			void RBTPrintBuilding(int a,int b);
			void findBuildingsInRange(RBTNode* m,int a, int b,int min, int max);
			RBTNode* findMin(RBTNode *root);
			RBTNode* findMax(RBTNode *root); 
			RBTree()
			{ 
				root=NULL; 
			} 
			void RBTNodeUpdate(int a);	
};
RBTree tree;

bool RBTNode::isLeftChild() //Function checks if current node is the left child of it's parent
{
	
	if(this==parent->left) 
		return true;
	else
		return false;
}

bool RBTNode::isRightChild() //Function checks if current node is the left child of it's parent
{
	
	if(this==parent->right) 
		return true;
	else
		return false;
}

bool RBTNode::hasRedChild()//Function returns true when the current node has a red child
{ 
	if(left!=NULL and left->colour==RED)
		return true;
	else if(right!=NULL and right->colour==RED)
		return true;
	else
		return false;
}


RBTNode::RBTNode(int b, int t) //Instantiates a node with Building number, executed time and total time fields
{	 
	parent=left=right=NULL; //Pointers are set to NULL
	this->bNum=b;
	this->eTime=0;
	this->tTime=t;
	colour=RED; //Default color for a node is red
}

RBTNode* RBTNode::returnUncleNode() //Function returns uncle of current node if it exists
{ 
	if ((parent==NULL)or(parent->parent==NULL)) //Checking if parent or grandparent exists
		return NULL; 
	if (parent->isLeftChild()==1) //Checking if parent is left child of Grandparent 
		return parent->parent->right; //Sibling on the opposite side
	else if(parent->isRightChild()==1)
		return parent->parent->left; 
}

RBTNode* RBTNode::returnSiblingNode() //Function returns returnSiblingNode of current node if it exists 
{ 
	if (parent==NULL) //Checking if node is root
		return NULL; 
	if (isLeftChild()==1) //Checking if node is left child of parent
		return parent->right;
	else if (isRightChild()==1) //Checking if node is right child of parent
		return parent->left; 
} 	

void RBTree::RBTInsertBuilding(int n, int m) //Inserts a new node into the Red Black tree
{ 
	RBTNode *newNode=new RBTNode(n,m); //Initialises the node with bNum and tTime
	if (root==NULL) 
	{
  		newNode->colour=BLACK; //First node to be inserted, ie root, therefore black
  		root=newNode;  //Inserting node as root
	} 
	else 
	{ 
	  	RBTNode *t=findNode(n); //Search RBT to see where insertion needs to occur
  		if (t->bNum==n) 
		{ 
			cout<<"\nERROR!Building already exists\n";
    		return; 
  		} 
  		newNode->parent=t; //temp is the node where search ended and it will be the parent of the newNode to be added
  		if(n<t->bNum) //If new node bNum is lesser, add as left child
    		t->left=newNode; 
  		else //If new node bNum is greater, add as right child
    		t->right=newNode; 
  		fixInsertViolation(newNode); //Fix insertion red red violation
	} 
} 

void RBTree::rotateRR(RBTNode *x) //RR Rotation: x is the node where imbalance occurs
{ 
	RBTNode *replaceParent=x->right; //x's right child is the new parent
	if (x==root) 
		root=replaceParent; // If x was the root, new Parent is the new root 	
	if (x->parent!=NULL) 
	{ 
		if (x->isLeftChild()==1) //Check if current node exists on left
			x->parent->left=replaceParent;  //Insert as left child
		else if(x->isRightChild()==1)
			x->parent->right=replaceParent; //Insert as right child  
	} 
	replaceParent->parent=x->parent; 
	x->parent=replaceParent;
	x->right=replaceParent->left; //New Parent's left child will be x's right child
	if (replaceParent->left!=NULL) //Setting x as the parent for new Parent's left child
		replaceParent->left->parent=x; 
	replaceParent->left=x; //x will be new Parent's left child
} 

void RBTree::rotateLL(RBTNode *x) //LL Rotation : x is the node where imbalance occurs
{  
	RBTNode *replaceParent=x->left; //x's left child is the new parent 
	if (x==root) // If x was the root, new Parent is the new root
		root=replaceParent; 
	if (x->parent!=NULL) 
	{ 
		if (x->isLeftChild()==1) //Check if current node exists on left
			x->parent->left=replaceParent;  //Insert as left child
		else if(x->isRightChild()==1)
			x->parent->right=replaceParent; //Insert as right child  
	} 
	replaceParent->parent=x->parent; 
	x->parent=replaceParent;
	x->left=replaceParent->right; //New Parent's right child will be x's left child*************************
	if (replaceParent->right!=NULL) //Setting x as the parent for new Parent's right child
		replaceParent->right->parent=x; 
	replaceParent->right=x; //x will be new Parent's right child
} 

void RBTree::swapColours(RBTNode *a, RBTNode *b) //Given two nodes swaps it's colours
{ 
	COLOUR temp; 
	temp=a->colour; 
	a->colour=b->colour; 
	b->colour=temp; 
} 

void RBTree::swapValues(RBTNode *a, RBTNode *b) //Swaps data values in a node like tTime, eTime and BNum
{ 
	RBTNode *temp=new RBTNode(0,0); 
	temp->bNum=a->bNum;
	temp->tTime=a->tTime; 
	temp->eTime=a->eTime;
	a->bNum=b->bNum;
	a->tTime=b->tTime;
	a->eTime=b->eTime;
	b->bNum=temp->bNum; 
	b->tTime=temp->tTime; 
	b->eTime=temp->eTime;
} 

void RBTree::fixInsertViolation(RBTNode *x) //Function to fix red violations
{ 
	if (x==root)// If root is red, coloyr it black and return
	{ 
		x->colour=BLACK; 
		return; 
	} 
	RBTNode *parent=x->parent;
	RBTNode *grandparent=parent->parent;
	RBTNode *uncle=x->returnUncleNode(); //Initialise the values for parent, grandparent and returnUncleNode
	if (parent->colour!=BLACK) 
	{ 
		if (uncle!=NULL and uncle->colour==RED) //XYr violation 
		{  
			parent->colour=BLACK; //x's parent will be coloured black
			uncle->colour=BLACK; //x's uncle will be coloured black
			grandparent->colour=RED; //x's grandparent will be coloured red
			fixInsertViolation(grandparent); //Fix all the violations that occur above the grandparent level and up,recursively
		} 
		else //XYb violation:LLb, LRb, RLb, RRb
		{ 
			if (parent->isLeftChild()) //Checks if parent is left child of grandparent
			{ 
				if (x->isLeftChild()) //Checks if x is left child of parent :LLb Rotation
					swapColours(parent, grandparent); 
				else //x is right child of parent :LRb Rotation
				{ 
					rotateRR(parent);  
					swapColours(x, grandparent); 
				}		  
				rotateLL(grandparent); 
			} 
			else //Parent is right child of grandparent
			{ 
					if (x->isLeftChild()) //Checks if x is left child of parent :RLb Rotation
					{ 
						rotateLL(parent); 
						swapColours(x, grandparent); 
					}	 
					else //x is right child of parent :RRb Rotation
						swapColours(parent, grandparent); 	 
					rotateRR(grandparent); 
			} 
		} 
	} 
}	 
	 
RBTNode* RBTree::findReplacement(RBTNode *a) //After removing a node from the RBT, the node, needs to be replaces
{  
	if (a->left!=NULL and a->right!=NULL) //When a has left and right children, 
	{
		RBTNode *temp=a->right; 
		while(temp->left!=NULL) 
			temp=temp->left;
		return temp;	//Returns the leftmost child of the right subtree
	} 
	else if (a->left==NULL and a->right==NULL) //When a does not have left or right children i.e, it's a leaf
		return NULL;  
	else if (a->left!=NULL) //If a has a left child, return left child
		return a->left; 
	else			//Return right child
		return a->right; 
} 

void RBTree::fixDeleteViolation(RBTNode *x) //Corrects violations that occur when deletion is made
{ 
	if(x==root)  
		return; //If only one node exists, return
	RBTNode *sibling=x->returnSiblingNode(), *parent=x->parent; 
	if(sibling==NULL) 
		fixDeleteViolation(parent); //If sibling doesn't exist, push 1 level up
	else
	{ 	if(sibling->colour==RED and sibling->isLeftChild())
		{
			parent->colour=RED; //Set parent to red and sibling to black
			sibling->colour=BLACK;
			rotateLL(parent);
			fixDeleteViolation(x);
		}
		else if (sibling->colour==RED and sibling->isRightChild())
		{
			parent->colour=RED; //Set parent to red and sibling to black
			sibling->colour=BLACK;
			rotateRR(parent); 
			fixDeleteViolation(x);
		}
		else if(sibling->colour==BLACK and sibling->hasRedChild()==1 and sibling->left!=NULL and sibling->left->colour==RED and sibling->isLeftChild())
		{
			sibling->left->colour=sibling->colour; 
			sibling->colour=parent->colour; 
			rotateLL(parent);
			parent->colour=BLACK;
		}
		else if(sibling->colour==BLACK and sibling->hasRedChild()==1 and sibling->left!=NULL and sibling->left->colour==RED and sibling->isRightChild())
		{
			sibling->left->colour = parent->colour; 
			rotateLL(sibling); 
			rotateRR(parent);
			parent->colour=BLACK;
		}
		else if(sibling->colour==BLACK and sibling->hasRedChild()==1 and sibling->right!=NULL and sibling->right->colour==RED and sibling->isLeftChild())
		{
			sibling->right->colour=parent->colour; 
			rotateRR(sibling); 
			rotateLL(parent);
			parent->colour=BLACK;
		}
		else if(sibling->colour==BLACK and sibling->hasRedChild()==1 and sibling->right!=NULL and sibling->right->colour==RED and sibling->isRightChild())
		{
			sibling->right->colour=sibling->colour; 
			sibling->colour=parent->colour; 
			rotateRR(parent);
			parent->colour=BLACK;
		}
		else //If both children are black 
		{ 
			sibling->colour=RED; 
			if (parent->colour==BLACK) 
				fixDeleteViolation(parent); 
			else
				parent->colour=BLACK; 
		}
		
	} 
} 
	
void RBTree::performDelete(RBTNode *a) //Deletes a node from the RBT
{	 
   	RBTNode *b=findReplacement(a); //Node to replace v int the RBT 
   	RBTNode *parent=a->parent; 
    if (b==NULL) //a is a leaf, BSTreplace returns NULL when RBTNode is a leaf
	{ 
   		if (a==root) //Check if a is a root, remove it and set root to NULL 
       		root=NULL; 
		else 
		{ 
       		if ((b==NULL or b->colour==BLACK) and (a->colour==BLACK)==1) //Both a and b are black
       			fixDeleteViolation(a); //Fixes double black violation
			else //One of the nodes, either a or b is red
			{ 
       			if (a->returnSiblingNode()!= NULL) //If a has a sibling, change it's colour to red
           			a->returnSiblingNode()->colour=RED; 
       		} 
       		if (a->isLeftChild()) 
       			parent->left = NULL; //If a is left child, change it's parent's left pointer to NULL 
			else if (a->isRightChild()) 
       			parent->right = NULL; //Change parent's right pointer to NULL
    	} 
      	delete a; //Delete a
      	return; 
    } 
  	if (a->left==NULL or a->right==NULL) //a has only one child
	{  
    	if (a==root) //if a is the root, assign the value of the successor
		{ 
       		a->bNum=b->bNum;
			a->tTime=b->tTime;
			a->eTime=b->eTime; 
       		a->left=b->right=NULL; 
       		delete b; //Value of successor has been as assigned to the root, delete successor
    	} 
		else 
		{ 
       		if (a->isLeftChild()) //Check if a is the left child
       			parent->left=b; //Make b the left child of v's parent
			else if (a->isRightChild())//a is the right child
      			parent->right=b; //Make b the right child of b's parent
       		delete a; 
       		b->parent=parent; //assign parent of deleted node a to be the parent of b
       		if ((b==NULL or b->colour==BLACK) and (a->colour==BLACK)==1) 
       			fixDeleteViolation(b); //Fix Black black violation
			else  
       			b->colour=BLACK; 	//If a or b is red, colour u black 		 
    	} 
    	return; 
    } 
    swapValues(b,a); //a has two children, swap the value of a and its successor b and recurse
    performDelete(b);  
} 

void RBTree::RBTDeleteBuilding(int n) //Deletes node by value of key
{ 
	if (root==NULL) //Tree is empty   
		return; 
	RBTNode *v=findNode(n); 
	if (v->bNum!=n) 
	{ 
		cout<<"NO BUILDING TO DELETE WITH BUILDING NUMBER"<<n<<endl; 
		return; 
	} 
	performDelete(v); 
} 

RBTNode* RBTree::findNode(int n) //Function returns the node where the next insertion/delettion will occur
{ 
	RBTNode *t=root; 
	while (t!=NULL) 
	{ 
  		if (n==t->bNum) //If key is equal to a key that already exists in the tree, break 
    		break;
    	if(n>t->bNum)
    	{
    		if (t->right==NULL) //Check if right subtree exists
      			break; 
    		else
      			t=t->right;
		}
		else //If key is lesser,go down the left subtree
		{ 
    		if (t->left==NULL)  //Check if left subtree exists
      			break; 
    		else
      			t=t->left; 
  		}  
	} 
	return t; //Returns where you falloff, this is where insertion/deletion occurs
}		 

void RBTree::RBTPrintBuilding(int a) //Prints Building triplet based on Building Number
{
	RBTNode *s=findNode(a);
	if(s->bNum==a)
		oput<<"("<<s->bNum<<","<<s->eTime<<","<<s->tTime<<")"<<endl; //The output is in the file
	else
		oput<<"(0,0,0)"<<endl;
}

void RBTree::findBuildingsInRange(RBTNode *root, int a, int b,int min, int max)
{
	
    if (root==NULL) //Check if root is null
	{

    }
  	else
	{
        if(a<=min and b>=max)//Check if all nodes needto be printed
        {
            findBuildingsInRange(root->left,a,b,min,max);
            bNum_vector.push_back(root->bNum);
            eTime_vector.push_back(root->eTime);
            tTime_vector.push_back(root->tTime);
            findBuildingsInRange(root->right,a,b,min,max);
        }
        else if(a>min and b>=max)//If a>min, then printing does not start at the leaf, middle of left subtree and all of right subtree
        {
            if(root->bNum<a)//Check if left subtreee needsto be included
            {
                findBuildingsInRange(root->right,a,b,min,max);
            }
            else if(root->bNum==a)//Check if it starts at root, and only right subtree is included
            {
				bNum_vector.push_back(root->bNum);
            	eTime_vector.push_back(root->eTime);
                tTime_vector.push_back(root->tTime);
                findBuildingsInRange(root->right,a,b,min,max);
            }
            else if(root->bNum>a and root->bNum<b)//check if left subtree is includedd and only left subtree
            {
                findBuildingsInRange(root->left,a,b,min,max);
                bNum_vector.push_back(root->bNum);
                eTime_vector.push_back(root->eTime);
                tTime_vector.push_back(root->eTime);
                findBuildingsInRange(root->right,a,b,min,max);
            }
            
            else if(root->bNum==b)//Check if it starts at root, and only right subtree is included
            {
                bNum_vector.push_back(root->bNum);
                eTime_vector.push_back(root->eTime);
                tTime_vector.push_back(root->tTime);
                findBuildingsInRange(root->left,a,b,min,max);
            }
        }
        else if(a<=min and b<=max)//Left subtree is fully incuded and most of right subtree
        {
     		if(root->bNum>b)//Most of left subtree
            {
                findBuildingsInRange(root->left,a,b,min,max);
            }
            else if(a<root->bNum and root->bNum<b)//Part of left and part of right subtree
            {

                findBuildingsInRange(root->left,a,b,min,max);
				bNum_vector.push_back(root->bNum);
                eTime_vector.push_back(root->eTime);
                tTime_vector.push_back(root->tTime);
                findBuildingsInRange(root->right,a,b,min,max);
            }
            else if(root->bNum==b)//Full left subtree

            {
				bNum_vector.push_back(root->bNum);
                eTime_vector.push_back(root->eTime);
                tTime_vector.push_back(root->tTime);
                findBuildingsInRange(root->left,a,b,min,max);
            }
            else if(root->bNum==a)//Only right subtree
            {
                bNum_vector.push_back(root->bNum);
                eTime_vector.push_back(root->eTime);
                tTime_vector.push_back(root->tTime);
                findBuildingsInRange(root->right,a,b,min,max);
            }
        }
        else if(a>min and b<max)
        {
            if(a<root->bNum and root->bNum<b)
            {
                findBuildingsInRange(root->left,a,b,min,max);
                bNum_vector.push_back(root->bNum);
                eTime_vector.push_back(root->eTime);
                tTime_vector.push_back(root->tTime);
                findBuildingsInRange(root->right,a,b,min,max);
            }
            else if(root->bNum>b) //Print most of left subtree
            {
				findBuildingsInRange(root->left,a,b,min,max);
            }
            else if(root->bNum<a)//Print most of right subtree
            {
                findBuildingsInRange(root->right,a,b,min,max);
            }
         	else if(root->bNum==a)//Start from the root, print full right subtree
			{
                bNum_vector.push_back(root->bNum);
                eTime_vector.push_back(root->eTime);
                tTime_vector.push_back(root->tTime);
                findBuildingsInRange(root->right,a,b,min,max);
            }
            else if(root->bNum==b)//Start fro the root, print full left subtree
            {
                bNum_vector.push_back(root->bNum);
                eTime_vector.push_back(root->eTime);
                tTime_vector.push_back(root->tTime);
                findBuildingsInRange(root->left,a,b,min,max);
            }
        }
    }
}

RBTNode* RBTree::findMin(RBTNode *root)//Finds minimum node in a tree
{
	while (root->left!=NULL)
    {
		root=root->left;
	}
	return root;	
}

RBTNode* RBTree::findMax(RBTNode *root)//Finds maximum node in a tree
{
	while (root->right!=NULL)
    {
		root=root->right;
	}
	return root;	
}
//int ctr=0;
void RBTree::RBTPrintBuilding(int a,int b) //Prints Building triplet based on Building Number range
{
	//oput<<endl;
	std::stringstream s1;
	if(size==0)
		oput<<"(0,0,0)"<<endl;
	RBTNode* min=findMin(root);
	RBTNode* max=findMax(root);
	if(b<min->bNum or a>max->bNum)
    {
    	
    }
    else
    {
        findBuildingsInRange(root,a,b,min->bNum,max->bNum);
        int psize=bNum_vector.size();
        for(int i=0;i<psize;i++)
        {
			oput<<"(";
            oput<<bNum_vector.at(i)<<",";
            oput<<eTime_vector.at(i)<<",";
            oput<<tTime_vector.at(i);
            oput<<")";
            if(i<(psize-1))
                oput<<",";
        }
        oput<<endl;
    }
    eTime_vector.clear();
    tTime_vector.clear();
    bNum_vector.clear();
	
}

void RBTree::RBTNodeUpdate(int a)//Updates the execeuted time in the RBTNode whenever it is updated in the heap
{
	//int a=x->bNum;
	RBTNode *x=findNode(a);
	int b=x->eTime;
	x->eTime=b+1;	
}

bool cont=true;
void Insert(int bnum, int ttime) 
{ 
    if (size>MAXSIZE) //Checking to see if number of buildings can be more than 2000
    { 
        cout <<"Error! Size exceeded\n"; 
        return; 
    } 
    if(cont==true)
    {
		size++;
    	int i=size-1; 
		//If size is not greater than Maxsize, add a node into the heap at the end.
    	
    	heap[i].buildingNum=bnum; //Initialization
		heap[i].total_time=ttime;
  		heap[i].executed_time=0;
    	
		while (i!=0 and heap[parent(i)].executed_time>=heap[i].executed_time) // Fix the min heap property if it is violated 
    	{ 
			if((heap[parent(i)].executed_time==heap[i].executed_time) and (heap[parent(i)].buildingNum>heap[i].buildingNum))
    		{
    			swap(heap[i], heap[parent(i)]); 
       			i =parent(i);
			}
			else if((heap[parent(i)].executed_time==heap[i].executed_time) and (heap[parent(i)].buildingNum<heap[i].buildingNum))
    		{
       			i=parent(i);
			}
			else if(heap[parent(i)].executed_time>heap[i].executed_time)
			{
				swap(heap[i], heap[parent(i)]); 
       			i=parent(i);
			}
    	}
	}
	else
	{
		tempHeap[tempSize].buildingNum=bnum;
		tempHeap[tempSize].total_time=ttime;
  		tempHeap[tempSize].executed_time=0;
  		tempSize++; 
	}
}

int parent(int i)
{
	return floor((i-1)/2);
}

void work()
{

	if(size<=0)
		return;
	heap[0].executed_time=heap[0].executed_time+1;
	tree.RBTNodeUpdate(heap[0].buildingNum);
	if(heap[0].executed_time==heap[0].total_time)
	{ 
		if (size==1)  
        	tot=globalcounter; // When size=1 last building is being completed, store this in tot. This gives the total time to build the city
		oput<<"("<<heap[0].buildingNum<<","<<globalcounter<<")"<<endl;
		tree.RBTDeleteBuilding(heap[0].buildingNum); 
    	heap[0]=heap[size-1]; 
    	size--;
    	Heapify(heap,size,0);
    	cont=true;
    	if(tempSize>0)
		{
			for(int i=0;i<tempSize;i++)
			{
				Insert(tempHeap[i].buildingNum, tempHeap[i].total_time);
				tempSize--;
			}
			tempSize=0;
		}
	}
	else if(heap[0].executed_time%5==0)
	{
		Heapify(heap, size,0);
		cont=true;
		if(tempSize>0)
		{
			for(int i=0;i<tempSize;i++)
			{
				Insert(tempHeap[i].buildingNum, tempHeap[i].total_time);
				tempSize--;
			}
			tempSize=0;
		}
	}
	else
		cont=false;	
}

void Heapify(minHeap *heap, int n, int i)//Heapify function heapifies the  entire heap
{
	int small=i;
	int l=2*i+1; //Left Sibling of i
	int r=2*i+2; //Right Sibling of i
	if((r<n) and (heap[r].executed_time==heap[l].executed_time) and (heap[r].executed_time==heap[i].executed_time))
	{
		if(heap[r].buildingNum<heap[l].buildingNum and heap[r].buildingNum<heap[i].buildingNum)
			small=r;
		else if(heap[l].buildingNum<heap[r].buildingNum and heap[l].buildingNum<heap[i].buildingNum)
			small=l;
		else 
			small=i;
	}
	else if(r<n and heap[r].executed_time==heap[i].executed_time)
	{
		if(heap[r].buildingNum<heap[i].buildingNum)
			small=r;
		else
			small=i;
	}
	else if(l<n and heap[l].executed_time==heap[i].executed_time)
	{
		if(heap[l].buildingNum<heap[i].buildingNum)
			small=l;
		else
			small=i;
	}
	if(r<n and heap[r].executed_time<heap[small].executed_time and heap[l].executed_time<heap[small].executed_time and heap[l].executed_time==heap[r].executed_time)
	{
		if(heap[r].buildingNum<heap[l].buildingNum)
			small=r;
		else
			small=l;
	}
	else if(l<n and heap[l].executed_time<heap[small].executed_time)
		small=l;
	else if(r<n and heap[r].executed_time<heap[small].executed_time)
		small=r;
	if(small!=i)
	{
		swap(heap[i],heap[small]);
		Heapify(heap,n,small);
	}
		
}

void swap(minHeap a, minHeap b)
{
	struct minHeap temp;
	temp=a;
	a=b;
	b=temp;	
} 

int main (int argc, char** argv)
{
	
   	oput.open("output_file.txt",ios::out|ios::trunc); //If file exists, truncate it's contents.
	int counter=0,param1=0,param2=0,pos=0;
	int flag=0;
  	string fname,gcounterstr,param;
  	ifstream myfile(argv[1]);
  	bool b=0;
  	if (myfile.is_open())
 	{
    	while (!std::getline(myfile, gcounterstr,':').eof())//Get value of counter
    	{
			istringstream(gcounterstr)>>counter;//Change to integer
			getline(myfile,fname,'('); //Get unction name
			getline(myfile,param,')'); //Get parameter string
    		while(globalcounter<counter)
			{
				globalcounter=globalcounter+1;
				work();
			}
			if(fname.compare("Insert")==0||fname.compare(" Insert")==0)//Check if function is Insert
			{	
				pos=param.find(',');
				istringstream(param.substr(0,pos))>>param1; //Change first parameter to integer
				istringstream(param.substr(pos+1,param.length()-1))>>param2; //Change second parameter to integer
				Insert(param1,param2);
				tree.RBTInsertBuilding(param1,param2);	
			}
			else if(fname.compare(" PrintBuilding")==0||fname.compare("PrintBuilding")==0)//Check if function is Print Building
			{
				pos=param.find(',');// Find delimiter
				if (pos==std::string::npos)
				{
					istringstream(param)>>param1; //Change to integer
					tree.RBTPrintBuilding(param1);
				}
				else
				{
					istringstream(param.substr(0,pos))>>param1;//Change first parameter to integer
					istringstream(param.substr(pos+1,param.length()-1))>>param2; //Change second parameter to integer
					tree.RBTPrintBuilding(param1,param2);
				}
			}
		}
		while(size>0)
  		{
  			globalcounter++;
  			work();
		}
    	myfile.close();
  	}
  	else cout <<"Unable to open file";
    oput.close();
	return 0;
}
