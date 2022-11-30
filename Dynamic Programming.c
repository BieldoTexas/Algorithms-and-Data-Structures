#include <stdio.h>
#include <stdlib.h>

int main()
{

    int n, m;
    int *V,*C;

    while(scanf("%d %d",&n,&m) != EOF)
    {
        V = (int*) malloc(n*sizeof(int));
        C = (int*) malloc(m*sizeof(int));

        int GreatSolution[m];

        for(int i =0;i<n;i++)
            scanf("%d",&V[i]);

        for(int i =0;i<m;i++)
            scanf("%d",&C[i]);

        int S[m+1][n+1];

        for(int i = 0;i<=n;i++)
            S[0][i] = -9999999; 
        
        for(int i = 0;i<=m;i++)
            S[i][0] = -9999999;

        for(int i=1;i<=m;i++)
        {
            for(int j=1;j<=n;j++)
            {
                if(j>=i && n-(m-i) >= j) // lugares q serao usados
                {
                    if(i==1)
                    {
                        if(C[0] * V[j-1] > S[1][j-1])
                            S[1][j] = C[0] * V[j-1];
                        else
                            S[1][j] = S[1][j-1];
                    }
                    else
                    {
                        if( (C[i-1] * V[j-1]) + S[i-1][j-1] > S[i][j-1])
                            S[i][j] = (C[i-1] * V[j-1]) + S[i-1][j-1];
                        else
                            S[i][j] = S[i][j-1];
                    }
                }
                else // lugares proibidos
                    S[i][j] = -9999999;
            }
        }

        int cur = n;

        for(int i = m;i>0;i--)
        {
            for(int j = cur;j>0;j--)
            {
                if(S[i][j] != S[i][j-1])
                {
                    GreatSolution[i-1] = j-1;
                    cur = j-1;
                    break;
                }
            }
        }

        printf("%d\n",S[m][n]);

        for(int i = 0;i<m;i++)
        {
            if(i==m-1)
                printf("%d",GreatSolution[i]);
            else
                printf("%d ",GreatSolution[i]);
        }
        
        printf("\n\n");
    }
    return 0;
}