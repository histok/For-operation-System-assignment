#include <stdio.h>
#include <stdbool.h>

void fifoPage(int pages[], int n, int f) {
    int frame[f];
    for (int i = 0; i < f; i++)
        frame[i] = -1;

    int hit = 0;
    int pointer = 0;
    for (int i = 0; i < n; i++) {
        bool allocated = false;
        for (int j = 0; j < f; j++)
            if (frame[j] == pages[i]) {
                hit++;
                allocated = true;
                break;
            }

        if (!allocated) {
            frame[pointer] = pages[i];
            pointer = (pointer + 1) % f;
        }

        printf("\n");
        for (int k = 0; k < f; k++)
            printf("%d ", frame[k]);
    }
    printf("\n\n the number of shorting of edge: %d", n - hit);
    printf("\nthe percent of shorting of edge: %f\n", (n - hit) / (double)n);
}

int main() {
    int pages[] = { 7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3 };
    int n = sizeof(pages) / sizeof(pages[0]);
    int f = 4;

    fifoPage(pages, n, f);

    return 0;
}

