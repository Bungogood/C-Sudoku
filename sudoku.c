#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "examples.h"

typedef struct list {
    struct list* next;
    int value;
} list;

typedef struct state {
    struct state* next;
    struct list* pos;
    int length;
    int depth;
} state;

void show_list(list* cur) {
    if (cur != NULL) {
        while (cur->next != NULL) {
            printf("%d, ", cur->value);
            cur = cur->next;
        }
        printf("%d", cur->value);
    }
}

void show_states(state** states) {
    state* cur;
    for (int i=0; i<81; i++) {
        printf("%d: ", i);
        cur = states[i];
        while (cur != NULL) {
            printf("%d:(", cur->depth);
            show_list(cur->pos);
            printf("), ");
            cur = cur->next;
        }
        printf("\n");
    }
}

void show_sudoku(state** grid) {
    for (int r=0; r<81; r+=9) {
        if (r % 27 == 0) printf("+-------+-------+-------+\n");
        for (int c=0; c<9; c++) {
            if (c % 3 == 0) printf("| ");
            if (grid[r+c]->length == 1) printf("%d ", grid[r+c]->pos->value);
            else printf("  ");
        }
        printf("|\n");
    }
    printf("+-------+-------+-------+\n");
}

list* new_list (list* next, int value) {
    list* tmp = (list*)malloc(sizeof(list));
    tmp->next = next;
    tmp->value = value;
    return tmp;
}

state* new_state (state* next, list* pos, int length, int depth) {
    state* tmp = (state*)malloc(sizeof(state));
    tmp->next = next;
    tmp->pos = pos;
    tmp->length = length;
    tmp->depth = depth;
    return tmp;
}

void free_list (list* l) {
    free(l);
}

void free_state (state* s) {
    list* nxt;
    list* cur = s->pos;
    while (cur != NULL) {
        nxt = cur->next;
        free_list(cur);
        cur = nxt;
    }
    free(s);
}

list* init_stack (int* sudoku) {
    list* stack = NULL;
    for (int i=0; i<81; i++) {
        if (sudoku[i] != 0) {
            stack = new_list(stack, i);
        }
    }
    return stack;
}

state** init_grid (int* sudoku, list* stack) {
    state** grid = (state**)malloc(81*sizeof(state*));
    list* pos;
    while (stack != NULL) {
        pos = new_list(NULL, sudoku[stack->value]);
        grid[stack->value] = new_state(NULL, pos, 1, 0);
        stack = stack->next;
    }
    for (int i=0; i<81; i++) {
        pos = NULL;
        if (grid[i] == NULL) {
            for (int v=1; v<10; v++) {
                pos = new_list(pos, v);
            }
            grid[i] = new_state(NULL, pos, 9, 0);
        }
    }
    return grid;
}

state** init_neighbours () {
    state** neighbours = (state**)malloc(81*sizeof(state*));
    int sr, sc, i;
    for (i=0; i<81; i++) {
        neighbours[i] = new_state(NULL, NULL, 20, 0);
    }
    for (int r=0; r<9; r++) {
        for (int c=0; c<9; c++) {
            i = r*9+c;
            for (int j=0; j<9; j++) {
                if (j != r) neighbours[j*9+c]->pos = new_list(neighbours[j*9+c]->pos, i);
                if (j != c) neighbours[r*9+j]->pos = new_list(neighbours[r*9+j]->pos, i);
            }
            sr = (r/3)*3;
            sc = (c/3)*3;
            for (int rr=sr; rr<sr+3; rr++) {
                for (int cc=sc; cc<sc+3; cc++) {
                    if (r != rr && c != cc) neighbours[rr*9+cc]->pos = new_list(neighbours[rr*9+cc]->pos, i);
                }
            }
        }
    }
    return neighbours;
}

bool remove_deep (state** states, int index, int value, int depth) {
    list* cur = states[index]->pos;
    list* pos = NULL;
    bool found = false;
    while (cur != NULL) {
        if (cur->value == value) found = true;
        else pos = new_list(pos, cur->value);
        cur = cur->next;
    }
    if (found) {
        states[index] = new_state(states[index], pos, states[index]->length-1, depth);
    }
    return found;
}

bool remove_level (state** states, int index, int value) { //empty states
    list* cur = states[index]->pos;
    list* rm;
    if (cur->value == value) {
        states[index]->pos = cur->next;
        states[index]->length--;
        free_list(cur);
        return true;
    } 
    while (cur->next != NULL) {
        if (cur->next->value == value) {
            rm = cur->next;
            cur->next = rm->next;
            free_list(rm);
            states[index]->length--;
            return true;
        } 
        cur = cur->next;
    }
    return false;
}

bool remove_list (state** states, int index, int value, int depth) {
    if (states[index]->depth < depth) return remove_deep(states, index, value, depth);
    else return remove_level(states, index, value);
}

bool check (state** grid, state** neighbours, list* stack, int depth) {
    int nindex;
    int index = stack->value;
    int value = grid[index]->pos->value;
    list* cur = neighbours[index]->pos;
    while (cur != NULL) {
        nindex = cur->value;
        remove_list(neighbours, nindex, index, depth);
        if (remove_list(grid, nindex, value, depth)) {
            if (grid[nindex]->length == 0) return false;
            else if (grid[nindex]->length == 1) {
                stack->next = new_list(stack->next, nindex);
            }
        }
        cur = cur->next;
    }
    return true;
}

int shortest (state** grid) {
    int min = -1;
    for (int i=0; i<81; i++) {
        if (grid[i]->length == 2) {
            min = i;
            break;
        } else if (grid[i]->length > 1) {
            if (min == -1) {
                min = i;
            } else if (grid[i]->length < grid[min]->length) {
                min = i;
            }
        }
    }
    return min;
}

void undo (state** states, int depth) {
    state* tmp;
    for (int i=0; i<81; i++) {
        if (states[i]->depth == depth) {
            tmp = states[i];
            states[i] = states[i]->next;
            free_state(tmp);
        }
    }
}

void solve (state** grid, state** neighbours, list* stack, int depth);

void recursive (state** grid, state** neighbours, int min, int depth) {
    list* cur = grid[min]->pos;
    list* stack;
    while (cur != NULL) {
        stack = new_list(NULL, min);
        grid[min] = new_state(grid[min], new_list(NULL, cur->value), 1, depth);
        solve(grid, neighbours, stack, depth);
        undo(neighbours, depth);
        undo(grid, depth);
        cur = cur->next;
    }
}

void solve (state** grid, state** neighbours, list* stack, int depth) {
    list* tmp;
    while (stack != NULL) {
        if(!check(grid, neighbours, stack, depth)) return;
        tmp = stack;
        stack = stack->next;
        free_list(tmp);
    }
    int min = shortest(grid);
    if (min == -1) show_sudoku(grid);
    else recursive(grid, neighbours, min, depth+1);
}

int main () {
    int* sudoku = brute;
    list* stack = init_stack(sudoku);
    state** grid = init_grid(sudoku, stack);
    state** neighbours = init_neighbours();
    solve(grid, neighbours, stack, 0);
}
