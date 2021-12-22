
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <mpi.h>

extern void func0();
extern void func1();
extern uint32_t simpleSieve(uint32_t, uint32_t*);
uint32_t prime[3500];

uint32_t Prime[] = {
 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101,
103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157,163, 167, 173, 179, 181, 191, 193, 197, 199 };
uint32_t isize = sizeof(Prime) / sizeof(uint32_t);
#define NMAX 3401

/* test and set inside an array of 32-bit its representing 32 odds */
#define GETBIT(arr, BIT)   (arr[(BIT) >> 0x3] &   (0x1 << ((BIT)&0x7)))
#define CLRBIT(arr, BIT)   (arr[(BIT) >> 0x3] &= ~(0x1 << (BIT)&0x7))
#define SETBIT(arr, BIT)   (arr[(BIT) >> 0x3] |=  (0x1 << ((BIT)&0x7)))

int main(int argc, char** argv) {
    // Initialize the MPI environment
    int my_rank;
    uint32_t prime[NMAX], psize = 0;
    MPI_Init(NULL, NULL);
    // Get the rank of the process
    
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    if (!my_rank) {
        printf("Start of program\n");
        //printf("%I32u\n",psize);
        //for (int i = 0; i < psize; i++)
        //    printf("%I32u:%I32u\n", i,prime[i]);
    }
    else {
        // Print the message 
        printf("Hello World! My rank is %d ", my_rank);
        printf("%10d\n", my_rank * 1000);
        rangeSieve(my_rank * 1000, my_rank * 1000 + 1000, prime);
        // Finalize the MPI environment.
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}

void func0() {

    printf("Hello world!\n");
 
}

uint32_t rangeSieve(uint32_t low, uint32_t high, uint32_t* prime) {

    uint32_t range = ((high - low) + 2) >> 1;
    uint32_t p, q, p2 = 0, start = 0;
    uint8_t* mark = (uint8_t*) calloc(range, sizeof(uint8_t));
    assert(mark);
    uint32_t isqrt = (uint32_t)sqrt(high);
    for (uint32_t i = 0; i < isize; i++) {
        p = Prime[i];
        if (p > isqrt)
            break;
        q = p + p;
        start = (low / p) * p;
        start += ((start & 1) == 0) ? p : q;
        start -= low;
        for (uint32_t j = start >> 1; j < range; j += p) {
            SETBIT(mark, j);
        }
    }
    return getPrime(low, high, mark, prime);
}

uint32_t getPrime(uint32_t low, uint32_t high, uint8_t* mark, uint32_t* prime) {

    uint32_t psize = 0;
    printf("%I32u %I32u \n", low, high);
    for (uint32_t p = 1; p < (high - low) + 1; p += 2) {
        if (!GETBIT(mark, p >> 1)) {
            prime[psize] = low + p;
            printf("%I32u \n", low + p);
            psize++;
        }
    }
    return psize;
}