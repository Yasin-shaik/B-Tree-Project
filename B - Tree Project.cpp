#include<iostream>
#include<string>
#include<bits/stdc++.h>
using namespace std;
#define order 5
const int minimum_entries=((order+1)/2)-1;
typedef struct
{
    string Reg_Number;
    string name;
    char gender[6];
    char Branch[15];
    char section;
}Student;
struct node;
typedef struct
{
    Student *dataPtr;
    struct node *rightPtr;
}Entry;
typedef struct node
{
    struct node *first_ptr;
    int num_entries;
    Entry entry[order-1];
}Node;
class B_Tree;
class SearchEntry
{
    public:
    Student *Search(B_Tree,string ,Node *);
    Student *BTree_Search(B_Tree,string);
    int Search_node(B_Tree,Node *,string);
    friend void perform_operations(int,B_Tree &);
};
class InsertionEntry:protected SearchEntry
{
    public:
    void Insert(Node *,int,Entry *);//Function used to insert entries
    int Insert(B_Tree &,Node *,Student *,Entry *);//polymorphism
    void BTree_Insert(B_Tree &,Student *);
    void splitNode(Node *,int,int,Entry *);
    friend void perform_operations(int,B_Tree &);
};
class Rebuild
{
    protected:
    void Combine_Nodes(Node *,int,Node *,Node *);
    void Borrow_Left(Node *,int,Node *,Node *);
    void Borrow_Right(Node *,int,Node *,Node *);
    int Reflow(Node *,int);
};
class DeletionEntry:protected Rebuild,protected SearchEntry
{
    public:
    int Delete(Node *,int);//Function used to delete entry
    int Delete(Node *,int,Node *);//Function used to delete mid one
    int Delete(B_Tree &,Node *,string,int *);//Polymorphism
    int BTree_Delete(B_Tree &,string);
    friend void perform_operations(int,B_Tree &);
};
class B_Tree:protected InsertionEntry,protected DeletionEntry
{
    public:
    int count;
    B_Tree()
    {
        B_tree_count++;
        count=0;
    }
    Node *root;
    protected:
    static int B_tree_count;
    friend ostream& operator <<(ostream&,const B_Tree tree);
    friend class SearchEntry;
    friend class InsertionEntry;
    friend class DeletionEntry;
    friend void perform_operations(int,B_Tree &);
    public:
    ~B_Tree()
    {
        B_tree_count--;
    }
};
int B_Tree::B_tree_count=0;
void print(Student *node)
{
    cout<<"Details of the student:"<<endl;
    cout<<"Name:"<<node->name<<endl;
    cout<<"Register No:"<<node->Reg_Number<<endl;
    cout<<"Gender:"<<node->gender<<endl;
    cout<<"Branch:"<<node->Branch<<endl;
    cout<<"Section:"<<node->section<<endl;
}
void display(Node *Root)
{
    if(Root==NULL)
    {
        return;
    }
    if(Root->first_ptr==NULL)
    {
        for(int i=0;i<Root->num_entries;i++)
        {
            print(Root->entry[i].dataPtr);
        }
        return;
    }
    else
    {
        display(Root->first_ptr);
        for(int i=0;i<Root->num_entries;i++)
        {
            print(Root->entry[i].dataPtr);
            display(Root->entry[i].rightPtr);
        }
        return;
    }
}
ostream& operator <<(ostream &out,const B_Tree tree)
{
    display(tree.root);
    out<<"\n";
    return out;
}
int compare(string p,string q)
{
    if(p<q)
    {
        return -1;
    }
    else if(p==q)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
Student * SearchEntry::Search(B_Tree tree,string target,Node *root)
{
    int entryNo;
    if(root==NULL)
    {
        return NULL;
    }
    if(compare(target,root->entry[0].dataPtr->Reg_Number)<0)
    {
        return Search(tree,target,root->first_ptr);
    }
    entryNo=root->num_entries-1;
    while(entryNo>=0&&compare(target,root->entry[entryNo].dataPtr->Reg_Number)<0)
    {
        entryNo--;
    }
    if(compare(target,root->entry[entryNo].dataPtr->Reg_Number)==0)
    {
        return root->entry[entryNo].dataPtr;
    }
    return Search(tree,target,root->entry[entryNo].rightPtr);
}
Student * SearchEntry::BTree_Search(B_Tree tree,string target)
{
    if(tree.root!=NULL)
    {
        return Search(tree,target,tree.root);
    }
    else
    {
        return NULL;
    }
}
int SearchEntry::Search_node(B_Tree tree,Node *root,string target)
{
    if(compare(target,root->entry[0].dataPtr->Reg_Number)<0)
    {
        return 0;
    }
    int entryindex=root->num_entries-1;
    while(entryindex>=0&&compare(target,root->entry[entryindex].dataPtr->Reg_Number)<0)
    {
        entryindex--;
    }
    return entryindex;
}
void InsertionEntry::Insert(Node *root,int EntryIndex,Entry *upEntry)
{
    root->entry[EntryIndex]=*upEntry;
}
void InsertionEntry::splitNode(Node *node,int entryIndex,int compResult,Entry *upEntry)
{
    int fromIndex;
    int toIndex;
    Node *rightPtr;
    rightPtr=new(nothrow) Node;
    if(!rightPtr)
    {
        return;
    }
    rightPtr->first_ptr=NULL;
    for(int i=0;i<order-1;i++)
    {
        rightPtr->entry[i].dataPtr=NULL;
        rightPtr->entry[i].rightPtr=NULL;
    }
    if(entryIndex<minimum_entries)
    {
        fromIndex=minimum_entries;
    }
    else
    {
        fromIndex=minimum_entries+1;
    }
    toIndex=0;
    rightPtr->num_entries=node->num_entries-fromIndex;
    while(fromIndex<node->num_entries)
    {
        rightPtr->entry[toIndex++]=node->entry[fromIndex++];
    }
    node->num_entries=node->num_entries-rightPtr->num_entries;
    if(entryIndex<minimum_entries)
    {
        if(compResult<0)
        {
            for(int i=node->num_entries-1;i>=entryIndex;i--)
            {
                node->entry[i+1]=node->entry[i];
            }
            Insert(node,entryIndex,upEntry);
        }
        else
        {
            for(int i=node->num_entries-1;i>=entryIndex+1;i--)
            {
                node->entry[i+1]=node->entry[i];
            }
            Insert(node,entryIndex+1,upEntry);
        }
    }
    else
    {
        int p;
        compResult=compare((*upEntry).dataPtr->Reg_Number,rightPtr->entry[0].dataPtr->Reg_Number);
        if(compResult<0)
        {
            for(int i=rightPtr->num_entries;i>0;i--)
            {
                rightPtr->entry[i]=rightPtr->entry[i-1];
            }
            Insert(rightPtr,0,upEntry);
        }
        else
        {
            p=0;
            while(p<rightPtr->num_entries&&compare((*upEntry).dataPtr->Reg_Number,rightPtr->entry[p].dataPtr->Reg_Number)>0)
            {
                p++;
            }
            for(int i=rightPtr->num_entries;i>p;i--)
            {
                rightPtr->entry[i]=rightPtr->entry[i-1];
            }
            Insert(rightPtr,p,upEntry);
        }
        (node->num_entries)--;
        (rightPtr->num_entries)++;
    }
    (*upEntry).dataPtr=node->entry[minimum_entries].dataPtr;
    (*upEntry).rightPtr=rightPtr;
    rightPtr->first_ptr=node->entry[minimum_entries].rightPtr;
    return;
}
int InsertionEntry::Insert(B_Tree &tree,Node *root,Student *dataPtr,Entry *upEntry)
{
    int istaller;
    int compresult;
    int entryIndex;
    Node *sub_Tree;
    if(root==NULL)
    {
        (*upEntry).dataPtr=dataPtr;
        (*upEntry).rightPtr=NULL;
        return 1;
    }
    entryIndex=Search_node(tree,root,dataPtr->Reg_Number);
    compresult=compare(dataPtr->Reg_Number,root->entry[entryIndex].dataPtr->Reg_Number);
    if(entryIndex<=0&&compresult<0)
    {
        sub_Tree=root->first_ptr;
    }
    else
    {
        sub_Tree=root->entry[entryIndex].rightPtr;
    }
    istaller=Insert(tree,sub_Tree,dataPtr,upEntry);
    if(istaller)
    {
        if(root->num_entries>=order-1)
        {
            splitNode(root,entryIndex,compresult,upEntry);
            istaller=1;
        }
        else
        {
            if(compresult>=0)
            {
                for(int i=root->num_entries-1;i>=entryIndex+1;i--)
                {
                    root->entry[i+1]=root->entry[i];
                }
                Insert(root,entryIndex+1,upEntry);
            }
            else
            {
                for(int i=root->num_entries-1;i>=entryIndex;i--)
                {
                    root->entry[i+1]=root->entry[i];
                }
                Insert(root,entryIndex,upEntry);
            }
            (root->num_entries)++;
            istaller=0;
        }
    }
    return istaller;
}
void InsertionEntry:: BTree_Insert(B_Tree &tree,Student *dataPtr)
{
    Node *new_ptr;
    Entry UpEntry;
    int istaller;
    if(tree.root==NULL)
    {
        new_ptr=(Node *)malloc(sizeof(Node));
        new_ptr->first_ptr=NULL;
        new_ptr->num_entries=1;
        new_ptr->entry[0].dataPtr=dataPtr;
        new_ptr->entry[0].rightPtr=NULL;
        tree.root=new_ptr;
        tree.count++;
        for(int i=1;i<order-1;i++)
        {
            new_ptr->entry[i].dataPtr=NULL;
            new_ptr->entry[i].rightPtr=NULL;
        }
        return;
    }
    else
    {
        istaller=Insert(tree,tree.root,dataPtr,&UpEntry);
        if(istaller)
        {
            Node *new_node=(Node *)malloc(sizeof(Node));
            if(new_node)
            {
                new_node->entry[0]=UpEntry;
                new_node->num_entries=1;
                new_node->first_ptr=tree.root;
                tree.root=new_node;
            }
        }
        (tree.count)++;
        return;
    }
}
int DeletionEntry::Delete(Node *root,int EntryIndex)
{
    for(int i=EntryIndex;i<root->num_entries;i++)
    {
        root->entry[i]=root->entry[i+1];
    }
    (root->num_entries)--;
    if(root->num_entries<minimum_entries)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
void Rebuild::Combine_Nodes(Node *root,int EntryIndex,Node *leftPtr,Node *rightPtr)
{
    int fromIndex,toIndex;
    toIndex=leftPtr->num_entries;
    leftPtr->entry[toIndex].dataPtr=root->entry[EntryIndex].dataPtr;
    leftPtr->entry[toIndex].rightPtr=rightPtr->first_ptr;
    (leftPtr->num_entries)++;
    //Move all Data from right Subtree to left subtree
    fromIndex=0;
    while(fromIndex<rightPtr->num_entries)
    {
        leftPtr->entry[toIndex++]=rightPtr->entry[fromIndex++];
    }
    leftPtr->num_entries+=rightPtr->num_entries;
    delete rightPtr;
    rightPtr=NULL;
    for(int i=EntryIndex;i<root->num_entries-1;i++)
    {
        root->entry[i]=root->entry[i+1];
    }
    (root->num_entries)--;
    return;
}
void Rebuild::Borrow_Left(Node *root,int EntryIndex,Node *leftPtr,Node *rightPtr)
{
    int toIndex;
    toIndex=0;
    for(int i=rightPtr->num_entries-1;i>=0;i--)
    {
        rightPtr->entry[i+1]=rightPtr->entry[i];
    }
    rightPtr->entry[0].dataPtr=root->entry[EntryIndex].dataPtr;
    rightPtr->entry[0].rightPtr=rightPtr->first_ptr;
    rightPtr->first_ptr=leftPtr->entry[leftPtr->num_entries-1].rightPtr;
    (rightPtr->num_entries)++;
    root->entry[EntryIndex].dataPtr=leftPtr->entry[leftPtr->num_entries-1].dataPtr;
    (leftPtr->num_entries)--;
    return;
}
void Rebuild::Borrow_Right(Node *root,int EntryIndex,Node *leftPtr,Node *rightPtr)
{
    int toIndex;
    toIndex=leftPtr->num_entries;
    leftPtr->entry[toIndex].dataPtr=root->entry[EntryIndex].dataPtr;
    leftPtr->entry[toIndex].rightPtr=rightPtr->first_ptr;
    (leftPtr->num_entries)++;
    root->entry[EntryIndex].dataPtr=rightPtr->entry[0].dataPtr;
    rightPtr->first_ptr=rightPtr->entry[0].rightPtr;
    for(int i=0;i<rightPtr->num_entries-1;i++)
    {
        rightPtr->entry[i]=rightPtr->entry[i+1];
    }
    (rightPtr->num_entries)--;
    return;
}
int Rebuild::Reflow(Node *root,int EntryIndex)
{
    Node *leftPtr;
    Node *rightPtr;
    int underflow;
    if(EntryIndex==0)
    {
        leftPtr=root->first_ptr;
    }
    else
    {
        leftPtr=root->entry[EntryIndex-1].rightPtr;
    }
    rightPtr=root->entry[EntryIndex].rightPtr;
    //Try to borrow from right first
    if(rightPtr->num_entries>minimum_entries)
    {
        Borrow_Right(root,EntryIndex,leftPtr,rightPtr);
        underflow=0;
    }
    else//Not possible from right next try left otherwise merge both left and right child's of the root
    {
        if(leftPtr->num_entries>minimum_entries)
        {
            Borrow_Left(root,EntryIndex,leftPtr,rightPtr);
            underflow=0;
        }
        else//Unable to borrow from left child also so combine both left and right child's of the root node
        {
            Combine_Nodes(root,EntryIndex,leftPtr,rightPtr);
            if(root->num_entries<minimum_entries)
            {
                underflow=1;
            }
            else
            {
                underflow=0;
            }
        }
    }
    return underflow;
}
int DeletionEntry::Delete(Node *root,int EntryIndex,Node *subTree)
{
    int deleteIndex;
    int rightIndex;
    int underflow;
    if(subTree->first_ptr==NULL)//Check for leaf for direct deletion
    {
        deleteIndex=subTree->num_entries-1;
        root->entry[EntryIndex].dataPtr=subTree->entry[deleteIndex].dataPtr;
        subTree->num_entries--;
        if(subTree->num_entries<minimum_entries)
        {
            underflow=1;
        }
        else
        {
            underflow=0;
        }
    }
    else//Till no leaf node found traverse throught rightPtr of all nodes to reach the predecessor
    {
        rightIndex=subTree->num_entries-1;
        underflow=Delete(root,EntryIndex,subTree->entry[rightIndex].rightPtr);
        if(underflow)
        {
            underflow=Reflow(root,EntryIndex);
        }
    }
    return underflow;
}
int DeletionEntry:: Delete(B_Tree &tree,Node *root,string key,int *success)
{
    Node *leftPtr;
    Node *subTree;
    int entryIndex;
    int underflow;
    if(root==NULL)
    {
        (*success)=0;
        return 0;
    }
    entryIndex=Search_node(tree,root,key);
    if(compare(key,root->entry[entryIndex].dataPtr->Reg_Number)==0)
    {
        (*success)=1;
        if(root->entry[entryIndex].rightPtr==NULL)//Deleting leaf node of the B-Tree
        {
            underflow=Delete(root,entryIndex);
        }
        else//Deleting Internal node of the B-Tree
        {
            if(entryIndex>0)
            {
                leftPtr=root->entry[entryIndex-1].rightPtr;
            }
            else
            {
                leftPtr=root->first_ptr;
            }
            underflow=Delete(root,entryIndex,leftPtr);
            if(underflow)
            {
                underflow=Reflow(root,entryIndex);
            }
        }
    }
    else
    {
        if(compare(key,root->entry[0].dataPtr->Reg_Number)<0)
        {
            subTree=root->first_ptr;
        }
        else
        {
            subTree=root->entry[entryIndex].rightPtr;
        }
        underflow=Delete(tree,subTree,key,success);
        if(underflow)
        {
            underflow=Reflow(root,entryIndex);
        }
    }
    return underflow;
}
int DeletionEntry::BTree_Delete(B_Tree &tree,string key)
{
    int success;
    Node *delete_Ptr;
    if(tree.root==NULL)
    {
        return 0;
    }
    Delete(tree,tree.root,key,&success);
    if(success)
    {
        (tree.count)--;
        if(tree.root->num_entries==0)
        {
            delete_Ptr=tree.root;
            tree.root=tree.root->first_ptr;
            free(delete_Ptr);
            delete_Ptr=NULL;
        }
    }
    return success;
}
Student *create(void)
{
    Student *node;
    node=new(nothrow) Student;
    if(!node)
    {
        return NULL;
    }
    cout<<"\nEnter the name of the student:";
    cin>>node->name;
    cout<<"Enter the Register number of the student:";
    cin>>node->Reg_Number;
    cout<<"Enter the gender of  the student:";
    cin>>node->gender;
    cout<<"Enter the branch of the student:";
    cin>>node->Branch;
    cout<<"Enter the section of the Student:";
    cin>>node->section;
    return node;
}
void perform_operations(int choice,B_Tree &tree)
{
    Student *node;
    int flag;
    string Reg_No;
    switch(choice)
        {
            case 1:node=create();
                   tree.BTree_Insert(tree,node);
                   cout<<"Successfully inserted"<<endl;
            break;
            case 2:
                 cout<<"Enter the Register number to search for the Student:";
                 cin>>Reg_No;
                 node=tree.InsertionEntry::BTree_Search(tree,Reg_No);
                 if(node==NULL)
                 {
                    cout<<"Not found"<<endl;
                 }
                 else
                 {
                    print(node);
                 }
                 break;
            case 3:cout<<"Enter the Register number of the student to delete:";
                   cin>>Reg_No;
                   flag=tree.BTree_Delete(tree,Reg_No);
                   if(flag==1)
                   {
                        cout<<"Successfully Deleted"<<endl;
                   }
                   else
                   {
                    cout<<"Sorry!! Unable to find the Student with given Register number"<<endl;
                   }
            break;
            case 4:cout<<"Details of all Students are as follows:"<<endl;
                   cout<<tree;
            break;
            case 5:cout<<"Total number of people are:"<<tree.count<<endl;
            break;
            case 6:exit(0);
            break;
            default:cout<<"please enter the valid input"<<endl;
        }
}
int main()
{
    int choice;
    B_Tree tree;
    tree.root=NULL;
    while(1)
    {
        cout<<"Enter 1 to insert a Student"<<endl;
        cout<<"Enter 2 to search a Student"<<endl;
        cout<<"Enter 3 to to delete a Student"<<endl;
        cout<<"Enter 4 to display all Student"<<endl;
        cout<<"Enter 5 to know total number of Students"<<endl;
        cout<<"Enter 6 to exit"<<endl;
        cout<<"Enter your choice:";
        cin>>choice;
        perform_operations(choice,tree);
    }
}
