#include <stdio.h>
#include <time.h>
#include <stdbool.h>

void input(int* grid) {
    int buf;
    printf("Input grid (0 is empty):\n\n");
    for (int r=0; r<9; r++) {
        printf("row %d:\t", r+1);
        scanf("%d", &buf);
        for (int c=8; c>=0; c--) {
            grid[r*9+c] = buf%10;
            buf = buf/10;
        }
    }
    printf("\n");
}

bool valid(int* grid, int r, int c, int sr, int sc, int v) {
    for (int i=0; i<9; i++) {
        if (grid[r*9+i] == v) return false;
        else if (grid[i*9+c] == v) return false;
        else if (grid[(sr+i/3)*9+sc+i%3] == v) return false;
    }
    return true;
}

bool solve(int* grid, int r, int c) {
    if (c == 9) {
        c = 0;
        r++;
        if (r == 9) return true;
    }
    if (grid[r*9+c] != 0) return solve(grid, r, c+1);
    int sr = (r/3)*3;
    int sc = (c/3)*3;
    for (int v=1; v<10; v++) {
        if (valid(grid, r, c, sr, sc, v)) {
            grid[r*9+c] = v;
            if (solve(grid, r, c+1)) return true;
        }
    }
    grid[r*9+c] = 0;
    return false;
}

void output(int* grid, clock_t diff) {
    int msec = 1000 * diff / CLOCKS_PER_SEC;
    for (int r=0; r<81; r+=9) {
        if (r % 27 == 0) printf("+-------+-------+-------+\n");
        for (int c=0; c<9; c++) {
            if (c % 3 == 0) printf("| ");
            printf("%d ", grid[r+c]);
        }
        printf("|\n");
    }
    printf("+-------+-------+-------+\n");
    printf("Time taken: %d.%ds\n", msec/1000, msec%1000);
}

void main() {
    clock_t start, diff;
    int grid[81];
    input(grid);
    start = clock();
    solve(grid, 0, 0);
    diff = clock() - start;
    output(grid, diff);
}