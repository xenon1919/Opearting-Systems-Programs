#include<stdio.h>
#define MAX_FRAMES 3
int main() {
    int i,j;
    int pages[]={2,3,2,1,5,2,4,5,3,2};
    int page_faults=0;

    int n=sizeof(pages)/sizeof(pages[0]);
    int frames[MAX_FRAMES]={-1};
    int used_time[MAX_FRAMES]={0};
    for(i=0;i<n;i++) {
        int page=pages[i];
        int page_found=0;

        int lru_index=0;
        for(j=0;j<MAX_FRAMES;j++) {
            if(frames[j]==page) {
                page_found=1;
                used_time[j]=i;
                break;
            }
            else if(used_time[j]<used_time[lru_index]) {
                lru_index=j;
            }
        }
        if(!page_found) {
            frames[lru_index]=page;
            used_time[lru_index]=i;
            page_faults++;
        }
    }
    printf("Page faults: %d",page_faults);
    return 0;
}