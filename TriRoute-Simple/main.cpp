#include "transport.h"
#include "dsa.h"
#include "menu.h"
#include "booking.h"
#include "storage.h"
#include "search.h"
#include <iostream>
using namespace std;

int main() {
    TransportInventory inventory;
    WaitlistQueue waitlist;
    BookingSystem bookings(inventory, waitlist);
    SearchSystem search(inventory);
    RankingEngine ranking;
    UndoStack history;
    StorageManager storage;

    if (!storage.load(inventory, bookings, waitlist)) {
        cout << "Startup failed: could not load transport data.\n";
        return 1;
    }
    ranking.buildIndex(inventory);
    ConsoleMenu menu(inventory, ranking, bookings, waitlist, search, history, storage);
    menu.run();
    return 0;
}
