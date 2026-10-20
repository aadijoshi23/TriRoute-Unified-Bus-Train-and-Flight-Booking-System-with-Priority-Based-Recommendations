#include "dsa.h"
#include <cstring>
using namespace std;

//hash function
static int hashId(const char* id) {
    unsigned int hash = 0;
    for (int i = 0; id[i] != '\0'; i++) hash = (hash * 31 + (unsigned char)id[i]) % 211;
    return (int)hash;
}

//constructor
RankingEngine::RankingEngine() : heapSize(0), inventory(nullptr) {
    for (int i = 0; i < 211; i++) hashTable[i] = -1;
}

//linear probing
void RankingEngine::buildIndex(const TransportInventory& data) {
    inventory = &data;
    for (int i = 0; i < 211; i++) hashTable[i] = -1;
    for (int i = 0; i < data.count(); i++) {
        int slot = hashId(data.at(i)->getId());
        while (hashTable[slot] != -1) slot = (slot + 1) % 211;
        hashTable[slot] = i;
    }
}

//lookup function
Transport* RankingEngine::lookup(const char* id) const {
    if (!inventory) return nullptr;
    int slot = hashId(id);
    for (int step = 0; step < 211 && hashTable[slot] != -1; step++) {
        Transport* item = inventory->at(hashTable[slot]);
        if (strcmp(item->getId(), id) == 0) return item;
        slot = (slot + 1) % 211;
    }
    return nullptr;
}

//MIN-HEAP
bool RankingEngine::better(const HeapNode& a, const HeapNode& b) const {
    if (a.score != b.score) return a.score < b.score;
    return strcmp(inventory->at(a.inventoryIndex)->getId(),
                  inventory->at(b.inventoryIndex)->getId()) < 0;
}

//heapify up
void RankingEngine::push(const HeapNode& node) {
    int i = heapSize++;
    heap[i] = node;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (!better(heap[i], heap[parent])) break;
        HeapNode temp = heap[parent]; heap[parent] = heap[i]; heap[i] = temp;
        i = parent;
    }
}

//heapify down
HeapNode RankingEngine::pop() {
    HeapNode answer = heap[0];
    heap[0] = heap[--heapSize];
    int i = 0;
    while (true) {
        int left = 2 * i + 1, right = left + 1, smallest = i;
        if (left < heapSize && better(heap[left], heap[smallest])) smallest = left;
        if (right < heapSize && better(heap[right], heap[smallest])) smallest = right;
        if (smallest == i) break;
        HeapNode temp = heap[i]; heap[i] = heap[smallest]; heap[smallest] = temp;
        i = smallest;
    }
    return answer;
}

//rank function
int RankingEngine::rank(const TransportInventory& data, const int matches[],
                        int count, int priority, int orderedIndices[]) {
    if (count <= 0) return 0;
    if (count > MAX_TRANSPORTS) count = MAX_TRANSPORTS;
    int lowFare = data.at(matches[0])->getFare(), highFare = lowFare;
    int lowTime = data.at(matches[0])->getDuration(), highTime = lowTime;
    for (int i = 1; i < count; i++) {
        Transport* t = data.at(matches[i]);
        if (t->getFare() < lowFare) lowFare = t->getFare();
        if (t->getFare() > highFare) highFare = t->getFare();
        if (t->getDuration() < lowTime) lowTime = t->getDuration();
        if (t->getDuration() > highTime) highTime = t->getDuration();
    }
    if (priority == 4) {  // Manual selection sort by departure time
        for (int i = 0; i < count; i++) orderedIndices[i] = matches[i];
        for (int i = 0; i < count; i++) {
            int best = i;
            for (int j = i + 1; j < count; j++) {
                Transport* a = data.at(orderedIndices[j]);
                Transport* b = data.at(orderedIndices[best]);
                int cmp = strcmp(a->getDeparture(), b->getDeparture());
                if (cmp < 0 || (cmp == 0 && strcmp(a->getId(), b->getId()) < 0)) best = j;
            }
            int tmp = orderedIndices[i]; orderedIndices[i] = orderedIndices[best]; orderedIndices[best] = tmp;
        }
        return count;
    }
    heapSize = 0;
    for (int i = 0; i < count; i++) {
        Transport* t = data.at(matches[i]);
        double score = 0;
        if (priority == 1) score = t->getFare();
        else if (priority == 2) score = t->getDuration();
        else {
            double pricePart = highFare == lowFare ? 0.0 : (double)(t->getFare() - lowFare) / (highFare - lowFare);
            double timePart = highTime == lowTime ? 0.0 : (double)(t->getDuration() - lowTime) / (highTime - lowTime);
            score = (pricePart + timePart) / 2.0;
        }
        HeapNode node = {score, matches[i]};
        push(node);
    }
    for (int i = 0; i < count; i++) orderedIndices[i] = pop().inventoryIndex;
    return count;
}
