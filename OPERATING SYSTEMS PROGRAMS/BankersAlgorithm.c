#include <stdio.h>

int main() {
    // Number of processes and resource types
    int n = 5; // Processes
    int m = 3; // Resources

    // Allocation Matrix: what resources are currently allocated to each process
    int alloc[5][3] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    // Maximum Demand Matrix: maximum resources needed by each process
    int max[5][3] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    // Available Resources
    int avail[3] = {3, 3, 2};

    // Safety Algorithm
    for (int i = 0; i < n; i++) {
        int can_run = 1; // Flag to check if process can run

        for (int j = 0; j < m; j++) {
            if (max[i][j] - alloc[i][j] > avail[j]) {
                can_run = 0; // Not enough resources for this process
                break;
            }
        }

        if (can_run) {
            printf("Process %d can run.\n", i);
            for (int j = 0; j < m; j++) {
                avail[j] += alloc[i][j]; // Reclaim allocated resources
            }
        }
    }

    return 0;
}
