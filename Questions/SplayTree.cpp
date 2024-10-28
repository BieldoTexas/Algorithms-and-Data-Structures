#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Node
{
   struct Node *right,*left;
   int key;
}node;

int findDepth(node* root, int x)
{
   if (root == NULL)
      return -1;

   int depth = 0,flag = 1;
   node* aux = root;
   while(flag && aux!=NULL)
   {
      if(aux->key==x)
         flag = 0;
      else
      {
         depth++;
         if(x<aux->key)
            aux = aux->left;
         else
            aux = aux->right;
      }
   }
   if(!flag)
      return depth;
   else
      return -1;
}

void VisitedNodes(node* root,int key)
{
   int counter = 0,flag = 1;
   node* aux2 = root;
   while(flag)
   {
      if(aux2 == NULL)
         flag = 0;
      else
      {
         counter++;
         if(aux2->key>key)
            aux2 = aux2->left;
         else
            aux2 = aux2->right;
      }
   }
   printf("%d\n",counter);
}
node *rightRotate(node *x)
{
   node *y = x->left;
   node *c = y->right;
   y->right = x;
   x->left = c;
   return y;
}
 
node *leftRotate(node *x)
{
   node *y = x->right;
   node*c = y->left;
   y->left = x;
   x->right = c;
   return y;
} 

node* findFather(node* root,int key)
{
   if(root!=NULL)
   {
      if( ( root->left && key == (root->left->key) ) || (root->right && key == (root->right->key)) )
         return root;

      if(key < root->key)
         return findFather(root->left,key);

      else if(key > root->key)
         return findFather(root->right,key);
   }
   return NULL;
}

node *splay(node* root,int key)
{
   if(root == NULL|| root->key == key)
      return root;

   int depth = findDepth(root,key);
   for(int i = 0;i<depth;i++)
   {
      node* father = findFather(root,key);
      node* grandfather = findFather(root,father->key);

      if(grandfather==NULL)
      {
         if(root->key>key)
            root = rightRotate(root);
         else
            root = leftRotate(root);
      }
      else
      {
         if(father->left && father->left->key == key)
         {
            if(grandfather->key>key)
               grandfather->left = rightRotate(father);
            else
               grandfather->right = rightRotate(father);
         }
         else
         {
            if(grandfather->key>key)
               grandfather->left = leftRotate(father);
            else
               grandfather->right = leftRotate(father);
         }
      }
   }
   return root;
}

node* BST_Search(node* root, int key)
{
   if(root)
   {
      if(key==root->key)
         return root;

      else if(key < root->key)
         return BST_Search(root->left,key);

      else
         return BST_Search(root->right,key);
   }
   else
      return NULL;
}

node* BST_Insert(node* root, int key)
{
   if(root == NULL)
   {
      node* NewNode = (node*) malloc(sizeof(node));
      NewNode->key = key;
      NewNode->left=NewNode->right=NULL;
      return NewNode;
   }
   else
   {
      if(key < root->key)
         root->left=BST_Insert(root->left,key);
      else
         root->right=BST_Insert(root->right,key);

      return root;
   }
}

node* minValueNode(node* Node)
{
   node* current = Node;

   while (current && current->left != NULL)
      current = current->left;

   return current;
}
  
node* deleteNode(node* root, int key)
{
   if(root == NULL)
      return root;

   if(key < root->key)
      root->left = deleteNode(root->left, key);

   else if (key > root->key)
      root->right = deleteNode(root->right, key);

   else 
   {
      if (root->left == NULL)
      {
         node* temp = root->right;
         free(root);
         return temp;
      }
      else if (root->right == NULL)
      {
         node* temp = root->left;
         free(root);
         return temp;
      }
      node* temp = minValueNode(root->right);
      root->key = temp->key;
      root->right = deleteNode(root->right, temp->key);
   }
   
   return root;
}

int main()
{
   short int endFlag=1,flag = 1;
   int key;
   char event[10];
   node*root = NULL;
   node*aux = NULL;
   while(endFlag)
   {
      scanf("%s %u",event,&key);
      
      if(strncmp("INS",event,3) == 0)
      {
         node* aux = BST_Search(root,key);
         if(aux==NULL)
            root = BST_Insert(root,key);

         int depth = findDepth(root,key);
         printf("%d\n",depth);
      
         root = splay(root,key);
      }

      else if(strncmp("FND",event,5) == 0)
      {
         node* aux = BST_Search(root,key);
         int depth = findDepth(root,key);
         if(aux!=NULL)
         {
            printf("%d\n",depth);
            root = splay(root,key);
         }
         else
            VisitedNodes(root,key);
      }
      else if(strncmp("DEL",event,3) == 0)
      {
         node* father = findFather(root,key);
         node* aux = BST_Search(root,key);
         int depth = findDepth(root,key);
         if(aux!=NULL)
         {
            printf("%d\n",depth);
            root = deleteNode(root,key);
            if(father!=NULL)
               root=splay(root,father->key);
         }
         else
            VisitedNodes(root,key);
      }
      else
         endFlag = 0;
   }
}
