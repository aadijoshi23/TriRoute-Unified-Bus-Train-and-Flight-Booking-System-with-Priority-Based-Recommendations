#ifndef DSA_H
#define DSA_H

#include "transport.h"


struct HeapNode {
    double score;
    int inventoryIndex;
};

class RankingEngine {
private:
    HeapNode heap[MAX_TRANSPORTS];
    int heapSize;
    int hashTable[211];
    const TransportInventory* inventory;
    bool better(const HeapNode& a, const HeapNode& b) const;
    void push(const HeapNode& node);
    HeapNode pop();
public:
    RankingEngine();
    void buildIndex(const TransportInventory& data);
    Transport* lookup(const char* id) const;
    // priority: 1=cheapest, 2=fastest, 3=balanced, 4=departure time
    int rank(const TransportInventory& data, const int matches[], int count,
             int priority, int orderedIndices[]);
};

#endif
