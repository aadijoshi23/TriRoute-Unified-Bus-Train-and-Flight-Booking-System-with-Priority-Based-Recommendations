#include "search.h"
#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;

static bool sameText(const char* a, const char* b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return false;
        a++; b++;
    }
    return *a == '\0' && *b == '\0';
}

static bool startsWith(const char* city, const char* prefix) {
    while (*prefix) {
        if (!*city || tolower((unsigned char)*city) != tolower((unsigned char)*prefix)) return false;
        city++; prefix++;
    }
    return true;
}

int SearchSystem::findRoutes(const char* from, const char* to, const char* date,
                             const char* mode, int resultIndices[]) const {
    int found = 0;
    for (int i = 0; i < inventory.count(); i++) {
        Transport* trip = inventory.at(i);
        if (sameText(from, trip->getSource()) && sameText(to, trip->getDestination()) &&
            strcmp(date, trip->getDate()) == 0 &&
            (sameText(mode, "all") || sameText(mode, trip->mode()))) {
            resultIndices[found++] = i;
        }
    }
    return found;
}

void SearchSystem::suggestCities(const char* prefix) const {
    char unique[MAX_TRANSPORTS * 2][40];
    int cityCount = 0;
    for (int i = 0; i < inventory.count(); i++) {
        Transport* trip = inventory.at(i);
        const char* candidates[2] = {trip->getSource(), trip->getDestination()};
        for (int j = 0; j < 2; j++) {
            if (!startsWith(candidates[j], prefix)) continue;
            bool duplicate = false;
            for (int k = 0; k < cityCount; k++)
                if (sameText(unique[k], candidates[j])) duplicate = true;
            if (!duplicate) {
                strncpy(unique[cityCount], candidates[j], 39);
                unique[cityCount][39] = '\0';
                cityCount++;
            }
        }
    }
    if (!cityCount) cout << "No matching cities.\n";
    for (int i = 0; i < cityCount; i++) cout << "- " << unique[i] << '\n';
}

bool UndoStack::push(int bookingId) {
    if (top == 299) return false;
    bookingIds[++top] = bookingId;
    return true;
}

int UndoStack::pop() {
    return top < 0 ? -1 : bookingIds[top--];
}
