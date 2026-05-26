#include <stdio.h>

typedef struct {
    unsigned firstbit : 1;
    unsigned secondbit : 3;
    unsigned thirdbit : 4;
} Bitfield;

int main(void) {
    Bitfield a = {0, 0, 0};
    a.firstbit = 1;
    a.secondbit = 5;
    a.thirdbit = 9;

    printf("firstbit = %u\n", a.firstbit);
    printf("secondbit = %u\n", a.secondbit);
    printf("thirdbit = %u\n", a.thirdbit);
    printf("sizeof(Bitfield) = %zu bytes\n", sizeof a);
    return 0;
}
