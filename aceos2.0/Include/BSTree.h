#include <malloc.h>
#include <string.h>
#define KEYSIZE 10
struct BinarySearchTree
{
    char szKey[KEYSIZE];
    void * ptrData;
    struct BinarySearchTree * Left;
    struct BinarySearchTree * Right;
};
inline int BST_Count(struct BinarySearchTree * BSTList)
{
    if ( BSTList == NULL )
        return 0;
    else
        return 1+CountBST(BSTList->Left)+CountBST(BSTList->Right);
 
}
/* Creates a new Node and assigns its value */
inline struct BinarySearchTree * CreateNewNode(char * szKey, void * ptrData)
{
    int Len;
    struct BinarySearchTree *NewNode = (struct BinarySearchTree *) malloc(sizeof(struct BinarySearchTree));
    if ( NewNode==NULL ) 
            return NULL;
    NewNode->Left=NewNode->Right = NULL;
    Len = strlen(szKey);            //get the size of the key
    if ( Len >= KEYSIZE )           // and check whether the length is within the limit
        return NULL;               //KEYSIZE TOO LONG
    strncpy(NewNode->szKey, szKey, Len<KEYSIZE?Len:KEYSIZE-1);
    for(;Len<KEYSIZE;Len++)
        NewNode->szKey[Len]=0;  //filling the remaining characters with 0
    NewNode->ptrData = ptrData;
    return NewNode;
}
/* This function will returns 0 success 
        -1 on memory allocation error
        -2 unexpected error
*/
inline int BST_Add(struct BinarySearchTree **Root, char * szKey, void * ptrData)
{
    struct BinarySearchTree *BT, *NewNode;
    BT = *Root;
    if ( *Root != NULL )
    {
        if ( strcmp(BT->szKey,szKey) )
            return 1; //duplication
        if ( strcmp(BT->szKey,szKey)< 0 )
        {
            if ( BT->Left != NULL )
                return AddToBST(&BT->Left, szKey, ptrData);
            BT->Left=CreateNewNode(szKey, ptrData);
            if ( BT->Left == NULL )
                return -1;      //new node creation error
            return 0;           //sucess
        }
        if ( strcmp(BT->szKey,szKey)> 0 )
        {
            if ( BT->Right != NULL )
                return AddToBST(&BT->Right, szKey, ptrData);
            BT->Right=CreateNewNode(szKey, ptrData);
            if ( BT->Right == NULL )
                return -1;      //new node creation error
            return 0;           //sucess
        }
    }
    else
    {
        *Root=CreateNewNode(szKey, ptrData);
        if ( *Root == NULL )
            return -1;  //new node creation error
        return 0;       //succes
    }
    return -2; //unknown error
}
/* This function will delete the given node from the BST 
*/
inline int BST_Delete(struct BinarySearchTree **Root, char * szKey)
{
}
inline void * BST_GetData(struct BinarySearchTree *Root, char * szKey)
{       
    struct BinarySearchTree * List=Root;
    if ( List == NULL )
        return NULL;
    if ( strcmp(List->szKey,szKey) == 0 )
        return List->ptrData;
    if ( strcmp(List->szKey,szKey) < 0 )
        return BST_GetData( List->Right, szKey);
    if ( strcmp(List->szKey,szKey) > 0 )
        return BST_GetData( List->Left, szKey);
    return NULL;
}

/*This function Enumerates all the nodes in the BST by calling CallBack Function for each node*/
inline int BST_Enumerate(struct BinarySearchTree *Root, int (*CallBack)(void * Data)  )
{
    static int Count=0;
    if ( Root != NULL )
    {
        Count++;
        if ( CallBack(Root) )
            return Count;
        BST_Enumerate(Root->Left,CallBack);
        BST_Enumerate(Root->Right,CallBack);
    }
    return 0;
}