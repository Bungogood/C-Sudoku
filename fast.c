#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct list {
    struct list* next;
    char value;
    int depth;
} list;

typedef struct state {
    struct list values[20];
    struct list* pos;
    struct list* rm;
} state;

void show_grid (state* grid) {
    list* cur;
    for (int i=0; i<81; i++) {
        printf("%d: ", i);
        cur = grid[i].pos;
        if (cur != NULL) {
            while (cur->next != NULL) {
                printf("%c,", cur->value);
                cur = cur->next;
            }
            printf("%c", cur->value);
        }
        cur = grid[i].rm;
        if (cur != NULL) {
            while (cur->next != NULL) {
                printf("(%d:%c),", cur->depth, cur->value);
                cur = cur->next;
            }
            printf("(%d:%c)", cur->depth, cur->value);
        }
        printf("\n");
    }
}
void show_sudoku(state* grid) {
    for (int r=0; r<81; r+=9) {
        if (r % 27 == 0) printf("+-------+-------+-------+\n");
        for (int c=0; c<9; c++) {
            if (c % 3 == 0) printf("| ");
            if (grid[r+c].pos != NULL && grid[r+c].pos->next == NULL) printf("%c ", grid[r+c].pos->value);
            else printf("  ");
        }
        printf("|\n");
    }
    printf("+-------+-------+-------+\n");
}

void create_grid (state* grid) {
    list* value;
    for (int i=0; i<81; i++) {
        grid[i].pos = &grid[i].values[0];
        grid[i].rm = NULL;
        value = grid[i].values;
        for (int p=0; p<8; p++) {
            value[p].value = '1'+p;
            value[p].next = &value[p+1];
        }
        value[8].value = '9';
        value[8].next = NULL;
    }
}
void create_neighbours(state* neighbours) {
    list* value;
    int sr, sc, i, len;
    for (int r=0; r<9; r++) {
        for (int c=0; c<9; c++) {
            i = r*9+c;
            neighbours[i].pos = &neighbours[i].values[0];
            neighbours[i].rm = NULL;
            value = neighbours[i].values;
            len = 0;
            for (int p=0; p<19; p++) {
                value[p].next = &value[p+1];
            }
            value[19].next = NULL;
            for (int j=0; j<9; j++) {
                if (j != r) {
                    value[len].value = j*9+c;
                    len++;
                }
                if (j != c) {
                    value[len].value = r*9+j;
                    len++;
                }
            }
            sr = (r/3)*3;
            sc = (c/3)*3;
            for (int rr=sr; rr<sr+3; rr++) {
                for (int cc=sc; cc<sc+3; cc++) {
                    if (r != rr && c != cc) {
                        value[len].value = rr*9+cc;
                        len++;
                    }
                }
            }
        }
    }
}

int init_grid (state* grid, int* stack, char* sudoku) {
    int ptr = 0;
    list* cur;
    list* tmp;
    for (int i=0; i<81; i++) {
        if (sudoku[i] != '0') {
            stack[ptr++] = i;
            tmp = NULL;
            if (grid[i].pos != NULL && grid[i].pos->value == sudoku[i]) {
                tmp = grid[i].pos;
                grid[i].pos = grid[i].pos->next;
            }
            if (grid[i].pos != NULL) {
                cur = grid[i].pos;
                if (tmp == NULL) {
                    while (cur->next != NULL) {
                        if (cur->next->value == sudoku[i]) {
                            tmp = cur->next;
                            cur->next = cur->next->next;
                            break;
                        }
                        cur = cur->next;
                    }
                }
                while (cur->next != NULL) {
                    cur = cur->next;
                }
                cur->next = grid[i].rm;
                grid[i].rm = grid[i].pos;
            } else {
                if (grid[i].rm->value == sudoku[i]) {
                    tmp = grid[i].rm;
                    grid[i].rm = grid[i].rm->next;
                } else {
                    cur = grid[i].rm;
                }
            }
            if (tmp == NULL) {
                while (cur->next != NULL) {
                    if (cur->next->value == sudoku[i]) {
                        tmp = cur->next;
                        cur->next = cur->next->next;
                        break;
                    }
                    cur = cur->next;
                }
            }
            grid[i].pos = tmp;
            grid[i].pos->next = NULL;
            grid[i].rm->depth = 0;
        } else {
            if (grid[i].rm != NULL) {
                cur = grid[i].rm;
                while (cur->next != NULL) {
                    cur = cur->next;
                }
                cur->next = grid[i].pos;
                grid[i].pos = grid[i].rm;
                grid[i].rm = NULL;
            }
        }
    }
    return ptr;
}
void init_neighbours (state* neighbours) {
    list* cur;
    for (int i=0; i<81; i++) {
        if (neighbours[i].rm != NULL) {
            cur = neighbours[i].rm;
            while (cur->next != NULL) {
                cur = cur->next;
            }
            cur->next = neighbours[i].pos;
            neighbours[i].pos = neighbours[i].rm;
            neighbours[i].rm = NULL;
        }
    }
}
void solution (state* grid, char* sudoku) {
    for (int i=0; i<81; i++) {
        sudoku[i] = grid[i].pos->value;
    }
}

bool remove_list (state* states, int index, char value, int depth) {
    list* rm;
    if (states[index].pos->value == value) {
        rm = states[index].pos;
        states[index].pos = states[index].pos->next;
        rm->depth = depth;
        rm->next = states[index].rm;
        states[index].rm = rm;
        return true;
    }
    list* cur = states[index].pos;
    while (cur->next != NULL) {
        if (cur->next->value == value) {
            rm = cur->next;
            cur->next = cur->next->next;
            rm->depth = depth;
            rm->next = states[index].rm;
            states[index].rm = rm;
            return true;
        } 
        cur = cur->next;
    }
    return false;
}
int shortest (state* grid) {
    int i, len;
    int min = -1;
    int shortest = -1;
    list* cur;
    for (i=0; i<81; i++) {
        cur = grid[i].pos;
        len = 0;
        while (cur != NULL) {
            if (++len == shortest) break;
            cur = cur->next;
        }
        if (len == 2) {
            return i;
        } else if (len > 1 && (len < shortest || min == -1)) {
            min = i;
            shortest = len;
        }
    }
    return min;
}
void undo (state* states, int depth) {
    // not finished
    list* cur;
    list* mv;
    for (int i=0; i<81; i++) {
        while (states[i].rm != NULL && states[i].rm->depth == depth) {
            mv = states[i].rm;
            states[i].rm = states[i].rm->next;
            if (states[i].pos == NULL || mv->value < states[i].pos->value) {
                mv->next = states[i].pos;
                states[i].pos = mv;
            } else {
                cur = states[i].pos;
                while (cur->next != NULL && mv->value > cur->next->value) {
                    cur = cur->next;
                }
                mv->next = cur->next;
                cur->next = mv;
            }
        }
    }
}

//remove list should never allow the taking of a single
void show(list* cur) {
    while (cur != NULL) {
        printf("%c", cur->value);
        cur = cur->next;
    }
    printf("\n");
}

bool solve (state* grid, state* neighbours, int* stack, int ptr, int depth) {
    int index, neighbour;
    char value;
    list* cur;
    while (ptr != 0) {
        index = stack[--ptr];
        value = grid[index].pos->value;
        cur = neighbours[index].pos;
        while (cur != NULL) {
            neighbour = cur->value;
            remove_list(neighbours, neighbour, index, depth);
            if (remove_list(grid, neighbour, value, depth)) {
                if (grid[neighbour].pos == NULL) return false;
                else if (grid[neighbour].pos->next == NULL) stack[ptr++] = neighbour;
            }
            cur = cur->next;
        }
    }
    int min = shortest(grid);
    if (min == -1) {
        //show_sudoku(grid);
        return true;
    } else {
        list* head = grid[min].pos;
        list* tail = grid[min].rm;
        cur = grid[min].pos;
        while (cur->next != NULL) {
            cur->depth = depth;
            cur = cur->next;
        }
        cur->depth = depth;
        cur->next = grid[min].rm;
        grid[min].rm = grid[min].pos->next;
        grid[min].pos->next = NULL;
        do {
            stack[0] = min;
            if (solve(grid, neighbours, stack, 1, depth+1)) return true;
            undo(grid, depth+1);
            undo(neighbours, depth+1);
            grid[min].pos->next = tail;
            cur->next = grid[min].pos;
            cur = grid[min].pos;
            grid[min].pos = grid[min].rm;
            grid[min].rm = grid[min].rm->next;
            grid[min].pos->next = NULL;
        } while (grid[min].pos != head);
        return false;
    };
}

int main (int argc, char* argv[]) {
    state grid[81];
    state neighbours[81];
    char sudoku[82];
    int stack[81];
    int ptr, length;

    FILE *rptr = fopen(argv[1], "r");
    FILE *wptr = fopen(argv[2], "w");

    if (rptr == NULL) {
        printf("Error! opening file\n");
        exit(1);
    }
    fscanf(rptr, "%d\n", &length);
    fprintf(wptr,"%d\n", length);
    printf("length: %d\n", length);

    create_grid (grid);
    create_neighbours (neighbours);
    for (int i=0; i<length; i++) {
        fscanf(rptr, "%[^\n]\n", sudoku);
        fprintf(wptr,"%s,", sudoku);
        ptr = init_grid (grid, stack, sudoku);
        init_neighbours (neighbours);
        solve (grid, neighbours, stack, ptr, 0);
        solution (grid, sudoku);
        //show_sudoku(grid);
        fprintf(wptr,"%s\n", sudoku);
    }
    
    fclose(rptr);
    fclose(wptr);
}
