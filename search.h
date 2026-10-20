#ifndef SEARCH_H
#define SEARCH_H

#include "transport.h"

// Dhrup: manual route search, city suggestions and array-based undo stack.
class SearchSystem {
private:
    const TransportInventory& inventory;
public:
    SearchSystem(const TransportInventory& data) : inventory(data) {}
    int findRoutes(const char* source, const char* destination, const char* date,
                   const char* mode, int resultIndices[]) const;
    void suggestCities(const char* prefix) const;
};

class UndoStack {
private:
    int bookingIds[300];
    int top;
public:
    UndoStack() : top(-1) {}
    bool push(int bookingId);
    int pop();  // -1 when nothing is left
};

#endif
