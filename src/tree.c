#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../include/tree.h"

typedef struct pair{
    Node * parent;
    Node * child;
}Pair;

static Node * MakeNode(const Item * pi);
static bool ToLeft(const Item * i1,const Item * i2);
static bool ToRight(const Item * i1,const Item * i2);
static void AddNode(Node * new_node, Node * root);
static void InOrder(const Node * root,void (*pfun)(Item item));
static Pair SeekItem(const Item * pi,const Tree * ptree);
static void DeleteNode(Node **ptr);
static void DeleteAllNodes(Node * ptr);

void InitializeTree(Tree * ptree)
{
    ptree->root = NULL;
    ptree->size = 0;
}

bool TreeIsEmpty(const Tree * ptree)
{
    if(ptree->root == NULL)
        return true;
    else
        return false;
}

bool TreeIsFull(const Tree * ptree)
{
    if(ptree->size == MAXITEMS)
        return true;
    else 
        return false;
}

int TreeItemCount(const Tree * ptree)
{
    return ptree->size;
}

bool AddItem(const Item * pi,Tree * ptree)
{
    Node * new_node;
    if(TreeIsFull(ptree))       
    {
        fprintf(stderr,"Tree is full\n");
        return false;
    }
    //判断是否有添加重复的Item项目
    if(SeekItem(pi,ptree).child != NULL)    //第一次运行到这里返回为空。
    {
        fprintf(stderr,"Attempted to add duplicate item\n");
        return false;
    }
    printf("end of seekitem-----\n");
    new_node = MakeNode(pi);    //生成一个包含该项目的节点。
    if(new_node == NULL)
    {
        fprintf(stderr,"Couldn't create node\n");
        return false;
    }
    ptree->size++;
    
    if(ptree->root == NULL)
        ptree->root = new_node;
    else
        AddNode(new_node,ptree->root);
    return true;
}

bool InTree(const Item * pi,const Tree * ptree)
{
    return (SeekItem(pi,ptree).child == NULL) ? false:true;
}

bool DeleteItem(const Item * pi,Tree * ptree)
{
    Pair look;
    look = SeekItem(pi,ptree);
    if(look.child == NULL)
        return false;
    
    if(look.parent == NULL)
        DeleteNode(&ptree->root);
    else if(look.parent->left == look.child)
        DeleteNode(&look.parent->left);
    else
        DeleteNode(&look.parent->right);
    ptree->size--;

    return true;
}

void Traverse(const Tree * ptree, void (*pfun)(Item item))
{
    if(ptree != NULL)
        InOrder(ptree->root,pfun);
}

void DeleteAll(Tree * ptree)
{
    if(ptree != NULL)
        DeleteAllNodes(ptree->root);
    ptree->root = NULL;
    ptree->size = 0;
}

static void InOrder(const Node * root,void (*pfun)(Item item))
{
    //从树的最左边的节点到最右边的节点，依次显示节点所包含的信息。
    if(root != NULL)
    {
        //一直遍历到最左边的节点，然后从最左边的节点开始显示节点包含的项目信息。
        InOrder(root->left,pfun);
        (*pfun)(root->item);
        InOrder(root->right,pfun);
    }
}
static void DeleteAllNodes(Node * root)
{
    Node * pright;

    if(root != NULL)
    {
        pright = root->right;
        DeleteAllNodes(root->left);
        free(root);
        DeleteAllNodes(pright);
    }
}
static void AddNode(Node * new_node,Node * root)
{
    //比较添加的字符，从而决定树要往左边生长。
    if(ToLeft(&new_node->item,&root->item))
    {
        printf("new_node-->%p,root->left-->%p\n",new_node,root->left);
        printf("&new_node->item-->%p,&root->item-->%p\n",&new_node->item,&root->item);
        if(root->left == NULL)
        {
            root->left = new_node;
        }
        else    
        {
            printf("******\n");
            //注意，由于上面if语句得出往左边生长，所以将树的做节点作为传进去作为值来使用。
            AddNode(new_node,root->left);
            
        }

    }
    else if(ToRight(&new_node->item,&root->item))
    {
        if(root->right == NULL)
            root->right = new_node;
        else
            AddNode(new_node,root->right);
    }
    else
    {
        fprintf(stderr,"location error in AddNode()\n");
        exit(1);
    }
}

static bool ToLeft(const Item * i1,const Item * i2)
{
    int comp1;
    if((comp1 = strcmp(i1->petname,i2->petname)) < 0)
        return true;

    else if(comp1 == 0 && strcmp(i1->petkind,i2->petkind) < 0)
        return true;
    else
        return false;
}

static bool ToRight(const Item * i1, const Item * i2)
{
    int comp1;
    if((comp1 = strcmp(i1->petname, i2->petname)) > 0)
        return true;
    else if(comp1 == 0 && strcmp(i1->petkind,i2->petkind) > 0)
        return true;
    else 
        return false;
}

static Node * MakeNode(const Item * pi)
{
    Node * new_node;

    new_node = (Node *)malloc(sizeof(Node));
    if(new_node != NULL)
    {
        new_node->item = *pi;
        new_node->left = NULL;
        new_node->right = NULL;
    }
    return new_node;
}
static Pair SeekItem(const Item * pi,const Tree * ptree)
{
    Pair look;
    look.parent = NULL;
    look.child = ptree->root;
    if(look.child == NULL)      //第一次运行时候子节点为空，设置为根节点。
        return look;
    while(look.child != NULL)
    {
        if(ToLeft(pi,&(look.child->item)))
        {
            look.parent = look.child;
            look.child = look.child->left;
        }
        else if(ToRight(pi, &(look.child->item)))
        {
            look.parent = look.child;
            look.child = look.child->right;
        }
        else
            break;
    }
    return look;
}

static void DeleteNode(Node **ptr)
{
    Node * temp;
    puts((*ptr)->item.petname);
    if((*ptr)->left == NULL)
    {
        temp = * ptr;
        *ptr = (*ptr)->right;
        free(temp);
    }
    else if((*ptr)->right == NULL)
    {
        temp = *ptr;
        *ptr = (*ptr)->left;
        free(temp);
    }
    else
    {
        for(temp = (*ptr)->left;temp->right != NULL;
                    temp = temp->right)
            continue;
        temp->right = (*ptr)->right;
        temp = *ptr;
        *ptr = (*ptr)->left;
        free(temp);
    }
}


