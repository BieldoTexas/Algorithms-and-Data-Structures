#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct client
{
    unsigned ip;
    int timestamp;
}Client;

void init_hash(Client*** hash,int sizeHash,int** sizeList)
{
    (*hash) = (Client**) calloc(sizeHash,sizeof(Client*));

    for(int i = 0;i<sizeHash;i++)
        (*hash)[i] = (Client*) calloc(1,sizeof(Client));

    (*sizeList) = (int*) calloc(sizeHash,sizeof(int));
}

void freeHash(Client*** hash,int sizeHash,int** sizeList)
{
    Client** tmp = *hash;

    for(int i = 0;i<sizeHash;i++)
        free(tmp[i]);

    free(tmp);

    free(*sizeList);
}

void hashing (Client*** hash,int* lineSize,int indexHash,int key,unsigned ip,short int* flag)
{
    (*hash)[indexHash] = (Client*) realloc((*hash)[indexHash],(1+(*lineSize))*sizeof(Client));
    (*hash)[indexHash][*lineSize].ip = ip;
    (*hash)[indexHash][*lineSize].timestamp = key;
    (*lineSize)++;

    if(!(*flag))
        printf("%d %d\n",indexHash,*lineSize);
}

void sorting(Client** hashLine,int lineSize)
{
    if(lineSize>1)
    {
        for(int u = 0;u<lineSize-1;u++)
        {
            for(int h = 0;h<lineSize-u-1;h++)
            {
                if((*hashLine)[h].timestamp>(*hashLine)[h+1].timestamp)
                {
                    Client temp = (*hashLine)[h];
                    (*hashLine)[h]=(*hashLine)[h+1];
                    (*hashLine)[h+1]=temp;
                }
            }
        }
    }
}

void rehashing (Client*** hash,Client*** newHash,int* sizeHash,int** lineSize,short int* flag,int**newLineSize)
{
    int k = *sizeHash;
    *sizeHash = (*sizeHash)*2 + 1;

    for(int i = 0;i<k;i++)
    {
        if((*lineSize)[i]>0)
        {
            for(int y = 0;y<(*lineSize)[i];y++)
            {
                int index = (*hash)[i][y].timestamp%(*sizeHash);
                hashing(newHash,&((*newLineSize)[index]),index,(*hash)[i][y].timestamp,(*hash)[i][y].ip,flag);
            }
        }
    }

    for(int i = 0;i<(*sizeHash);i++)
        sorting(&(*newHash)[i],(*newLineSize)[i]);
    
    freeHash(hash,k,lineSize);
    *hash = *newHash;

    *lineSize = *newLineSize;
    (*flag) = 0;
}

void binarySearch(Client* list, int begin, int end, int ts)
{
    int i = (begin + end) / 2;
    short flag = 0;

    if (begin > end)
    {
        printf("-1 -1\n");
        flag = 1;
    }

    if (list[i].timestamp == ts&&!flag)
    {
        printf("%u %d\n",list[i].ip,i);
        flag = 1;
    }

    if(!flag)
    {
        if (list[i].timestamp < ts)
            return binarySearch(list, i + 1, end, ts);
        else 
            return binarySearch(list, begin, i - 1, ts);
    }  
}    

int main()
{
    int M,key,index,ip,nClient = 0;
    short int endFlag = 1,flag = 0,flag2 = 0;
    float Lmax;
    char event[25];
    Client **table = NULL;
    int* sizeList = NULL;

    scanf("%d %f",&M,&Lmax);
    init_hash(&table,M,&sizeList);

    while(endFlag) 
    {
        scanf("%s",event);
        if(strncmp("NEW",event,3) == 0)
        {   
            if(((float)nClient/M) > Lmax)
            {
                int* newSizeList = NULL;
                Client** newHash = NULL;
                flag2 = 1;

                init_hash(&newHash,(2*M) +1,&newSizeList);
                rehashing(&table,&newHash,&M,&sizeList,&flag2,&newSizeList);
            }

            scanf("%d %u",&key,&ip);
            index = key%M;
            nClient++;

            hashing(&table,&sizeList[index],index,key,ip,&flag2);
        }
        
        else if(strncmp("QRY",event,3) == 0)
        { 
            scanf("%d",&key);
            index = key%M;
            flag = 0;

            if(sizeList[index]>0)        
                binarySearch(table[index],0,sizeList[index]-1,key);
            else
                printf("-1 -1\n"); 
        }
        
        else if(strncmp("END",event,3) == 0 )
        {
            endFlag = 0;
            freeHash(&table,M,&sizeList);
        }
    }
    return 0;
}