#include <stdio.h>

void printBits(int num) {
    unsigned int value = (unsigned int) num;
    unsigned int mask = 1u << (sizeof(int) * 8 - 1);
    for (int i = 0; i < sizeof(int) * 8; i++) {
        if (value & mask) {
            printf("1");
        } else {
            printf("0");
        }
        mask >>= 1;
    }
    printf("\n");
}



int countOnebits(int num) {
    unsigned int value = (unsigned int) num;
    int count = 0;
    while (value) {
        count += value & 1;
        value >>= 1;
    }
    return count;
}

int main() {
    
    int num;

    printf("Enter a number to print the bits in an integer:\n");
    scanf("%d", &num);

    printBits(num);
    int result = countOnebits(num);

    printf("Number of 1 bits in %d is: %d\n", num, result);

    return 0;
}