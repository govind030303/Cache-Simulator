#include "cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initCache(Cache *cache, int cacheSize, int blockSize, int associativity, const char *writePolicy) {
    cache->numSets = cacheSize / (blockSize * associativity);
    cache->associativity = associativity;
    cache->timeCounter = 0;
    strcpy(cache->writePolicy, writePolicy);
    cache->sets = (CacheBlock **)malloc(cache->numSets * sizeof(CacheBlock *));
    for (int i = 0; i < cache->numSets; i++) {
        cache->sets[i] = (CacheBlock *)malloc(associativity * sizeof(CacheBlock));
        for (int j = 0; j < associativity; j++) {
            cache->sets[i][j].valid = false;
            cache->sets[i][j].dirty = false;
            cache->sets[i][j].frequency = 0;
            cache->sets[i][j].accessTime = 0;
        }
    }
}

bool isHit(Cache *cache, int setIndex, int tag) {
    for (int i = 0; i < cache->associativity; i++) {
        if (cache->sets[setIndex][i].valid && cache->sets[setIndex][i].tag == tag) {
            return true;
        }
    }
    return false;
}

void replaceBlock(Cache *cache, int setIndex, int tag, const char *policy) {
    int replaceIndex = 0;
    if (strcmp(policy, "FIFO") == 0) {
        replaceIndex = 0; 
    } else if (strcmp(policy, "LFU") == 0) {
        int minFreq = cache->sets[setIndex][0].frequency;
        for (int i = 1; i < cache->associativity; i++) {
            if (cache->sets[setIndex][i].frequency < minFreq) {
                replaceIndex = i;
                minFreq = cache->sets[setIndex][i].frequency;
            }
        }
    } else if (strcmp(policy, "LRU") == 0) {
        int oldestTime = cache->sets[setIndex][0].accessTime;
        for (int i = 1; i < cache->associativity; i++) {
            if (cache->sets[setIndex][i].accessTime < oldestTime) {
                replaceIndex = i;
                oldestTime = cache->sets[setIndex][i].accessTime;
            }
        }
    }
    cache->sets[setIndex][replaceIndex].tag = tag;
    cache->sets[setIndex][replaceIndex].valid = true;
}

void accessCache(Cache *l1, Cache *l2, int address, int blockSize, const char *policy, int *l1Hits, int *l1Misses, int *l2Hits, int *l2Misses) {
    int setIndex = (address / blockSize) % l1->numSets;
    int tag = address / (blockSize * l1->numSets);

    printf("Accessing address: %d\n", address);

    if (isHit(l1, setIndex, tag)) {
        (*l1Hits)++;
        printf("L1 Cache Hit!\n");
    } else {
        (*l1Misses)++;
        printf("L1 Cache Miss!\n");
        setIndex = (address / blockSize) % l2->numSets;
        tag = address / (blockSize * l2->numSets);
        if (isHit(l2, setIndex, tag)) {
            (*l2Hits)++;
            printf("L2 Cache Hit!\n");
        } else {
            (*l2Misses)++;
            printf("L2 Cache Miss! Loading from Main Memory...\n");
            replaceBlock(l2, setIndex, tag, policy);
        }
        replaceBlock(l1, (address / blockSize) % l1->numSets, address / (blockSize * l1->numSets), policy);
    }
}

void freeCache(Cache *cache) {
    for (int i = 0; i < cache->numSets; i++) {
        free(cache->sets[i]);
    }
    free(cache->sets);
}
