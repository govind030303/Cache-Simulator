#include "cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int l1CacheSize, l2CacheSize, associativity, blockSize, numAccesses;
    char policy[10], writePolicy[15];
    
    printf("Enter L1 Cache size (in bytes): ");
    scanf("%d", &l1CacheSize);
    printf("Enter L2 Cache size (in bytes): ");
    scanf("%d", &l2CacheSize);
    printf("Enter associativity: ");
    scanf("%d", &associativity);
    printf("Enter Block size (in bytes): ");
    scanf("%d", &blockSize);
    printf("Enter replacement policy (FIFO/LFU/LRU): ");
    scanf("%s", policy);
    printf("Enter write policy (Write-Through/Write-Back): ");
    scanf("%s", writePolicy);
    
    printf("Enter number of memory accesses: ");
    scanf("%d", &numAccesses);
    
    int *accesses = (int *)malloc(numAccesses * sizeof(int));
    printf("Enter memory addresses: ");
    for (int i = 0; i < numAccesses; i++) {
        scanf("%d", &accesses[i]);
    }
    
    Cache l1, l2;
    initCache(&l1, l1CacheSize, blockSize, associativity, writePolicy);
    initCache(&l2, l2CacheSize, blockSize, associativity, writePolicy);
    
    int l1Hits = 0, l1Misses = 0, l2Hits = 0, l2Misses = 0;
    for (int i = 0; i < numAccesses; i++) {
        accessCache(&l1, &l2, accesses[i], blockSize, policy, &l1Hits, &l1Misses, &l2Hits, &l2Misses);
    }
    
    printf("Final Stats - L1 Hits: %d, L1 Misses: %d, L2 Hits: %d, L2 Misses: %d\n", l1Hits, l1Misses, l2Hits, l2Misses);
    
    freeCache(&l1);
    freeCache(&l2);
    free(accesses);
    return 0;
}