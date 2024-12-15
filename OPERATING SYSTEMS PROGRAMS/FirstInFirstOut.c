#include<stdio.h>
#define MAX_FRAME 3
int main() {
    int i,j;
    int pages[]={2,3,2,1,5,2,4,5,3,2};
    int n=sizeof(pages)/sizeof(pages[0]);
    int page_fault=0;
    int frames[MAX_FRAME]={-1};
    for(i=0;i<n;i++) {
        int page=pages[i];
        int page_found=0;
        for(j=0;j<MAX_FRAME;j++) {
            if(frames[i]==page) {
                page_found=1;
                break;
                
            }
        }
        if(!page_found) {
            page_fault++;

        }
    }
    printf("The total number of page faults are %d", page_fault);
    return 0;
}