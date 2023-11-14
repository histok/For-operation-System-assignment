#include <stdio.h>
#include <stdbool.h>

int findLRU(int time[], int f) {
    int min = time[0];
    int res = 0;
    for (int i = 1; i < f; i++) {
        if (time[i] < min) {
            min = time[i];
            res = i;
        }
    }
    return res;
}

void lruPage(int pages[], int n, int f) {
    int frame[f];
    for (int i = 0; i < f; i++)
        frame[i] = -1;

    int time[f];
    for (int i = 0; i < f; i++)
        time[i] = 0;

    int hit = 0;
    for (int i = 0; i < n; i++) {
        bool allocated = false;
        for (int j = 0; j < f; j++)
            if (frame[j] == pages[i]) {
                hit++;
                time[j] = i + 1;
                time[j] = i + 1;
                allocated = true;
                break;
            }

        if (!allocated) {
            int lru = findLRU(time, f);
            frame[lru] = pages[i];
            time[lru] = i + 1;
        }

        printf("\n");
        for (int k = 0; k < f; k++)
            printf("%d ", frame[k]);
    }
    printf("\n\nthe number of shorting of edge: %d", n - hit);
    printf("\nthe percent of shorting of edge: %f\n", (n - hit) / (double)n);
}

int main() {
    int pages[] = { 7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3 };
    int n = sizeof(pages) / sizeof(pages[0]);
    int f = 4;

    lruPage(pages, n, f);

    return 0;
}


