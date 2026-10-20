#ifndef MENU_H
#define MENU_H

#include "transport.h"
#include "dsa.h"
#include "booking.h"
#include "storage.h"
#include "search.h"

// Saurav: simple user interface that joins the five other modules.
class ConsoleMenu {
private:
    TransportInventory& inventory;
    RankingEngine& ranking;
    BookingSystem& bookings;
    WaitlistQueue& waitlist;
    SearchSystem& search;
    UndoStack& undo;
    StorageManager& storage;
    void searchRoutes();
    void bookTicket();
    void cancelTicket();
    void showWaitlist() const;
    void undoBooking();
    void save();
public:
    ConsoleMenu(TransportInventory& data, RankingEngine& ranker, BookingSystem& booker,
                WaitlistQueue& queue, SearchSystem& finder, UndoStack& history,
                StorageManager& files)
        : inventory(data), ranking(ranker), bookings(booker), waitlist(queue),
          search(finder), undo(history), storage(files) {}
    void run();
};

#endif
