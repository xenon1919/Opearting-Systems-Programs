rintf("Process %d can run.\n", i);
            for (int j = 0; j < m; j++) {
                avail[j] += alloc[i][j];