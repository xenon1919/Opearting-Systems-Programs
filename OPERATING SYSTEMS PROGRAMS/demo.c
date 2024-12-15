#include<stdio.h>
#include<stdlib.h>
int main()
{
    int RQ[100],i,n,totalheadmoment=0,initial;
    printf("enter no of requests\n");
    scanf("%d",&n);
    printf("enter the request sequence\n")
    for(i=0;i<n;i++)
    scanf("%d",&RQ[i])
    printf("enter initial head position\n");
    scanf("%d",&initial);
    for(i=0;i<n;i++)
    
        totalheadmoment=totalheadmoment+abs(RQ[i]-initial);
        initial=RQ[i]}
        printf("total head moments is %d",totalheadmoment);
        return 0;
    }
