#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    int v,w;
    struct Node *next;
}node;

typedef struct list
{
    node* head,*tail;
}List;

typedef struct HeapNode
{
    int v;
    int dist;
}heapNode;

typedef struct heap
{
    int capacity;
    heapNode* array;
    int heap_size; 
}Heap;

node* makeNode(int value)
{
    node* newNode = (node*) malloc(sizeof(node));
    newNode->v = value;
    newNode->next = NULL;
    return newNode;
}

Heap* create_Heap(int capacity)
{
    Heap* heap = malloc(sizeof(Heap));
    heap->array = (heapNode*) malloc(capacity*sizeof(heapNode));
    heap->heap_size = 0;
    heap->capacity = capacity;
    return heap;
}

void bubble_up(Heap** heap,int index)
{
    int i = index;
    heapNode tmp;
    while(i>0 && (*heap)->array[i].dist < (*heap)->array[(i-1)/2].dist )
    {
        tmp = (*heap)->array[i];
        (*heap)->array[i] = (*heap)->array[(i-1)/2];
        (*heap)->array[(i-1)/2]= tmp;
        i = (i-1)/2;
    }
}

void bubble_down(Heap** heap, int index)
{
    int l = (2*index)+1 , r = (2*index) + 2, m = index;

    if(l<(*heap)->heap_size && (*heap)->array[l].dist < (*heap)->array[m].dist)
        m = l;

    if(r<(*heap)->heap_size && (*heap)->array[r].dist < (*heap)->array[m].dist)
        m = r;
    
    if(m!=index)
    {
        heapNode tmp = (*heap)->array[m];
        (*heap)->array[m] = (*heap)->array[index];
        (*heap)->array[index] = tmp;
        bubble_down(heap,m);
    }
}

void heap_insert(Heap** heap,int v,int dist)
{
    if((*heap)->capacity == (*heap)->heap_size)
        (*heap)->array = (heapNode*) realloc((*heap)->array,2*(*heap)->capacity*sizeof(heapNode));
    
    (*heap)->array[(*heap)->heap_size].v = v;
    (*heap)->array[(*heap)->heap_size].dist = dist;
    (*heap)->heap_size++;

    bubble_up(heap,(*heap)->heap_size-1);
}

void heap_extract(Heap** heap, int* u,int* d)
{
    heapNode r = (*heap)->array[0];

    *u = r.v;
    *d = r.dist;

    (*heap)->array[0] = (*heap)->array[(*heap)->heap_size-1];
    (*heap)->heap_size--;

    bubble_down(heap,0);
}

void heap_update(Heap** heap,int v,int dv)
{   
    int i;
    for(i =0; i<(*heap)->heap_size; i++)
        if((*heap)->array[i].v == v)
            break;

    (*heap)->array[i].dist = dv;

    bubble_up(heap,i);
}

void Dijkstra(List* graph, int S,int** D,int V)
{
    Heap* heap = create_Heap(V);
    heap_insert(&heap,S,0);
    short int flag = 0;

    for(int i = 0;i<V;i++)
    {
        if(i!=S)
        {
            (*D)[i] = 9999999;
            heap_insert(&heap,i,9999999);
        }
    }
    
    node* e = NULL;
    int u,d,v,w;
    (*D)[S] = 0;

    for(int i = 0;i<V && !flag;i++)
    {
        heap_extract(&heap,&u,&d);
        e = graph[u].head->next;

        while(e!=NULL && !flag)
        {
            v = e->v;
            w = e->w;

            if((*D)[u] + w < (*D)[v])
            {
                (*D)[v] = (*D)[u] + w;
                heap_update(&heap,v,(*D)[v]);
            }
            e = e->next;
        }
    }
}

int main()
{
    int V,E,N,M,w,v1,v2,X,Y,D;

    scanf("%d %d %d %d",&V,&E,&N,&M);

    List lists[V];
    int stores[N],Dresult[N],motoboys[M],smallest,smallestIndex,currOrder=0,index,Z,waitingOrders = 0,waitingOrders2 = 0;

    int** Dy = (int**) malloc(N*sizeof(int));

    for(int i =0;i<V;i++)
        Dy[i] = (int*) malloc(V*sizeof(int));
   
    int* orders = NULL;

    for(int i=0;i<V;i++)
    {
        lists[i].head = makeNode(i);
        lists[i].tail = lists[i].head;
    }

    for(int i = 0;i<E;i++)
    {   
        scanf("%d %d %d",&v1,&v2,&w);
        node* x = makeNode(v1);
        node* y = makeNode(v2);

        lists[v1].tail->next = y;
        lists[v1].tail = y;
        y->w = w;
        
        lists[v2].tail->next = x;
        lists[v2].tail = x;
        x->w = w;
    }

    for(int i = 0;i<N;i++)
        scanf("%d",&stores[i]);
    
    for(int i=0;i<N;i++)
        Dijkstra(lists,stores[i],&Dy[i],V);

    for(int i = 0;i<M;i++)
        motoboys[i] = stores[0];
    
    char event[3];
    short int endflag = 1;

    while(endflag)
    {
        scanf("%s",event);
        if(strncmp(event,"NEW",3) == 0)
        {   
            scanf("%d %d",&index,&Z);
            waitingOrders++;
            waitingOrders2++;
            orders = (int*) realloc(orders,(waitingOrders2)*sizeof(int));
            orders[index] = Z;
            printf("+%d %d\n",index,waitingOrders);
        }
        else if(strncmp(event,"DLV",3) == 0)
        {
            while(orders[currOrder]==-1)
                currOrder++;
            
            scanf("%d",&D);
            X = motoboys[D];

            for(int i = 0;i<N;i++)
                Dresult[i] = Dy[i][X] + Dy[i][orders[currOrder]];
            
            smallest = 9999999;
            smallestIndex = -1;

            for(int i = 0;i<N;i++)
            {
                if(Dresult[i]<smallest)
                {
                    smallestIndex = i;
                    smallest = Dresult[i];
                }
                
                if(Dresult[i]==smallest)
                    if(stores[i]<stores[smallestIndex])
                        smallestIndex = i;
                
            }

            Y = stores[smallestIndex];
            printf("%d %d %d %d %d\n",currOrder,X,Y,orders[currOrder],smallest);
            motoboys[D] = orders[currOrder];
            orders[currOrder] = -1;
            currOrder++;
            waitingOrders--;
        }
        else if(strncmp(event,"CEL",3) == 0)
        {
            scanf("%d",&index);
            orders[index] = -1;
            waitingOrders--;
            printf("-%d %d\n",index,waitingOrders);
        }
        else
            endflag = 0;
    }
return 0;
}

