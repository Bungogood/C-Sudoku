#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "examples.h"

typedef struct stack {
    struct stack* next;
    int value;
} stack;

typedef struct list {
    struct list* next;
    int depth;
    int value;
} list;

typedef struct state {
    struct list* pos;
    struct list* rm;
    int length;
} state;

void show_states (state* states) {
    list* cur;
    for (int i=0; i<81; i++) {
        printf("%d: ", i);
        cur = states[i].pos;
        if (cur != NULL) {
            while (cur->next != NULL) {
                printf("%d,", cur->value);
                cur = cur->next;
            }
            printf("%d", cur->value);
        }
        cur = states[i].rm;
        if (cur != NULL) {
            while (cur->next != NULL) {
                printf("(%d:%d),", cur->depth, cur->value);
                cur = cur->next;
            }
            printf("(%d:%d)", cur->depth, cur->value);
        }
        printf("\n");
    }
}

state* create_grid () {
    state* grid = (state*)malloc(81*sizeof(state));
    list* pos;
    for (int i=0; i<81; i++) {
        grid[i].pos = (list*)malloc(9*sizeof(list));
        grid[i].length = 9;
        grid[i].rm = NULL;
        pos = grid[i].pos;
        for (int p=0; p<9; p++) {
            pos[p].value = p+1;
            pos[p].next = &pos[p+1];
        }
        pos[8].value = 9;
        pos[8].next = NULL;
    }
    return grid;
}

state* create_neighbours () {
    state* neighbours = (state*)malloc(81*sizeof(state));
    list* pos;
    int sr, sc, i;
    for (int r=0; r<9; r++) {
        for (int c=0; c<9; c++) {
            i = r*9+c;
            neighbours[i].pos = (list*)malloc(20*sizeof(list));
            neighbours[i].length = 0;
            neighbours[i].rm = NULL;
            pos = neighbours[i].pos;
            for (int p=0; p<19; p++) {
                pos[p].next = &pos[p+1];
            }
            pos[19].next = NULL;
            for (int j=0; j<9; j++) {
                if (j != r) {
                    pos[neighbours[i].length].value = j*9+c;
                    neighbours[i].length++;
                }
                if (j != c) {
                    pos[neighbours[i].length].value = r*9+j;
                    neighbours[i].length++;
                }
            }
            sr = (r/3)*3;
            sc = (c/3)*3;
            for (int rr=sr; rr<sr+3; rr++) {
                for (int cc=sc; cc<sc+3; cc++) {
                    if (r != rr && c != cc) {
                        pos[neighbours[i].length].value = rr*9+cc;
                        neighbours[i].length++;
                    }
                }
            }
        }
    }
    return neighbours;
}

stack* new_stack (stack* next, int value) {
    stack* tmp = (stack*)malloc(sizeof(stack));
    tmp->next = next;
    tmp->value = value;
    return tmp;
}

stack* init_stack (int* sudoku) {
    stack* stack = NULL;
    for (int i=0; i<81; i++) {
        if (sudoku[i] != 0) {
            stack = new_stack(stack, i);
        }
    }
    return stack;
}

int main () {
    state* grid = create_grid();
    show_states(grid);
    state* neighbours = create_neighbours();
    show_states(neighbours);

    int* sudoku = brute;
    //stack* stack = init_stack(sudoku);
    //init_grid(grid, sudoku, stack);
    //init_neighbours(neighbours);
    //state** neighbours = init_neighbours();
    //solve(grid, neighbours, stack, 0);
}
