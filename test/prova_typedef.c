#include <stdio.h>

typedef struct {
    int x;
    int y;
} Point;

typedef unsigned int uint;

int main() {
    Point p;
    p.x = 3;
    p.y = 7;

    uint count = 42;

    printf("Point: (%d, %d)\n", p.x, p.y);
    printf("Count: %u\n", count);

    return 0;
}