#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Node
{
    int value;
    struct Node *next;
}node;

typedef struct list
{
    node* head,*tail;
}List;

node* makeNode(int value)
{
    node* newNode = (node*) malloc(sizeof(node));
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}

int Find(int a,int* parent)
{
    if(parent[a] != a)
        parent[a] = Find(parent[a],parent);

    return parent[a];
}

void Union(int a, int b,int** parent)
{
    (*parent)[Find(b,*parent)] = Find(a,*parent);
}

void Shortest_path(List* graph,int S,int V,int dest,int *parent)
{
    int D[V];
    int visitedNodes[V];

    for(int i=0;i<V;i++)
    {
        D[i] = -1;
        visitedNodes[i] = 0;
    }
    
    D[S] = 0;

    node* head = (node*) malloc(sizeof(node));
    head->next = NULL;
    node* tail;
    tail = head;

    node* aux = makeNode(S);

    tail->next = aux;
    tail = aux;

    short flag = 0;

    while(head!=tail&& !flag)
    {
        int x = head->next->value;
        node* tmp = head->next;
        head->next = head->next->next;
        free(tmp);

        if(head->next == NULL)
            tail = head;
        
        int Class = Find(x,parent);
        tmp = graph[Class].head->next;
        visitedNodes[Class] = 1;

        int v,Class2;

        while(tmp!=NULL && !flag)
        {   
            Class2 = Find(tmp->value,parent);

            if(Class2==dest)
                flag = 1;

            if(D[Class2] == -1 && visitedNodes[Class2] != 1)
            {
                D[Class2] = D[Class] + 1;
                node* tmp2 = makeNode(Class2);
                tail->next = tmp2;
                tail = tmp2;
            }
            tmp = tmp->next;
        }
    }
    printf("%d\n",D[dest]);
}

void degree(List *graph,int A,int V,int v,int** parent)
{
    int degree = 0,visitedNodes[v],Class;

    for(int i = 0;i<v;i++)
        visitedNodes[i] = 0;

    node* aux = graph[A].head->next;

    while(aux!=NULL)
    {
        Class = Find(aux->value,*parent);

        if(visitedNodes[Class]==0 && Class != A)
        {
            visitedNodes[Class] = 1;
            degree++;
        }
        aux = aux->next;
    }
    printf("%d %d\n",V,degree);
}

void EdgeContraction(List* graph,int* V,int A,int B,int** parent,int saveV)
{   
    if(A!=B)
    {
        graph[A].tail->next = graph[B].head->next;
        graph[A].tail = graph[B].tail;

        Union(A,B,parent);

        (*V)--;
    }
    degree(graph,A,*V,saveV,parent);
}

int main()
{
    int V,E,v1,v2,saveV;
    scanf("%d %d",&V,&E);
    saveV = V;
    List lists[V];
    int* parent = (int*) malloc(V*sizeof(int));

    for(int i = 0;i<V;i++)
        parent[i] = i;

    for(int i=0;i<V;i++)
    {
        lists[i].head = makeNode(i);
        lists[i].head->value = i;
        lists[i].tail = lists[i].head;
    }
    
    for(int i = 0;i<E;i++)
    {   
        scanf("%d %d",&v1,&v2);
        node* x = makeNode(v1);
        node* y = makeNode(v2);

        lists[v1].tail->next = y;
        lists[v1].tail = y;
        
        lists[v2].tail->next = x;
        lists[v2].tail = x;
    }

    int n,x,y;
    char event[3];
    scanf("%d",&n);

    for(int i = 0;i<n;i++)
    {
        scanf("%s %d %d",event,&x,&y);

        if(strncmp("DIS",event,3) == 0)
            Shortest_path(lists,Find(x,parent),saveV,Find(y,parent),parent);
        
        else if(strncmp("CTR",event,3)==0)
            EdgeContraction(lists,&V,Find(x,parent),Find(y,parent),&parent,saveV);
    }

    return 0;
}
