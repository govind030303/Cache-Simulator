#ifndef CACHE_H
#define CACHE_H

#include <stdbool.h>

typedef struct {
    int tag;
    int frequency; // For LFU
    int accessTime; // For LRU
    bool valid;
    bool dirty; // For Write-Back
} CacheBlock;

typedef struct {
    CacheBlock **sets;
    int numSets;
    int associativity;
    int timeCounter;
    char writePolicy[15]; // Write-Through or Write-Back
} Cache;

void initCache(Cache *cache, int cacheSize, int blockSize, int associativity, const char *writePolicy);
bool isHit(Cache *cache, int setIndex, int tag);
void replaceBlock(Cache *cache, int setIndex, int tag, const char *policy);
void accessCache(Cache *l1, Cache *l2, int address, int blockSize, const char *policy, int *l1Hits, int *l1Misses, int *l2Hits, int *l2Misses);
void freeCache(Cache *cache);

#endif