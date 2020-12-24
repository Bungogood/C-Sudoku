#include <stdlib.h>
#include <stdio.h>

typedef struct {
    struct list* next;
    int value;
} list;

list* create(int len) {
    list* arr = malloc(len*sizeof(list));
    for (int i=0; i<len; i++) {
        arr[i].value = i;
    }
    return arr;
}

int main () {
    list* arr = create(10);
    printf("%d\n", arr[0].value);
}