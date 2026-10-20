#include "menu.h"
#include <iostream>
#include <cstring>
#include <limits>
using namespace std;

static bool getNumber(const char* message, int& number) {
    cout << message;
    if (!(cin >> number)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Please enter a valid number.\n";
        return false;
    }
    cin.ignore(10000, '\n');
    return true;
}

void ConsoleMenu::save() {
    if (!storage.save(inventory, bookings, waitlist))
        cout << "Warning: could not save all files.\n";
    else cout << "Data saved.\n";
}

void ConsoleMenu::searchRoutes() {
    char from[40], to[40], date[16], mode[16];
    cout << "Source city: "; cin.getline(from, 40);
    cout << "Destination city: "; cin.getline(to, 40);
    cout << "Date (YYYY-MM-DD): "; cin.getline(date, 16);
    cout << "Mode (all/bus/train/flight): "; cin.getline(mode, 16);
    int matches[MAX_TRANSPORTS], ordered[MAX_TRANSPORTS];
    int count = search.findRoutes(from, to, date, mode, matches);
    if (!count) { cout << "No matching journeys found.\n"; return; }
    int priority;
    if (!getNumber("Rank by: 1=Cheapest 2=Fastest 3=Balanced 4=Departure time: ", priority)) return;
    if (priority < 1 || priority > 4) { cout << "Invalid ranking choice.\n"; return; }
    ranking.rank(inventory, matches, count, priority, ordered);
    cout << "\n" << count << " matching journeys (best first):\n";
    cout << "ID       Mode    From         To           Date         Time    Minutes  Rs       Seats   Details\n";
    for (int i = 0; i < count; i++) inventory.at(ordered[i])->display();
}

void ConsoleMenu::bookTicket() {
    char name[50], id[20];
    int qty;
    cout << "Passenger name: "; cin.getline(name, 50);
    cout << "Transport ID (e.g. TRT001): "; cin.getline(id, 20);
    if (!ranking.lookup(id)) { cout << "Unknown transport ID.\n"; return; }
    if (!getNumber("Number of seats: ", qty)) return;
    int bookingId = bookings.book(name, id, qty);
    if (bookingId == -1) cout << "Booking rejected. Check the details/seat limit.\n";
    else { undo.push(bookingId); save(); }
}

void ConsoleMenu::cancelTicket() {
    int id;
    if (!getNumber("Booking ID to cancel: ", id)) return;
    if (bookings.cancel(id)) save();
    else cout << "Booking not found or already canceled.\n";
}

void ConsoleMenu::showWaitlist() const {
    if (!waitlist.count()) { cout << "Waitlist is empty.\n"; return; }
    for (int i = 0; i < waitlist.count(); i++) {
        WaitlistEntry e = waitlist.at(i);
        cout << (i + 1) << ". Booking " << e.bookingId
             << " for " << e.transportId << '\n';
    }
}

void ConsoleMenu::undoBooking() {
    // Limited undo: cancel the most recently created active booking this session.
    int id;
    while ((id = undo.pop()) != -1) {
        Booking* b = bookings.find(id);
        if (b && strcmp(b->status, "CANCELED") != 0) {
            cout << "Undo last booking " << id << ":\n";
            if (bookings.cancel(id)) save();
            return;
        }
    }
    cout << "Nothing to undo in this session.\n";
}

void ConsoleMenu::run() {
    while (true) {
        cout << "\n=====================================\n"
             << "  TRIROUTE - BUS | TRAIN | FLIGHT\n"
             << "=====================================\n"
             << "1. Display all transports\n"
             << "2. Search and rank journeys\n"
             << "3. Find a transport by ID (hash table)\n"
             << "4. Book a ticket\n"
             << "5. Cancel a ticket\n"
             << "6. View bookings\n"
             << "7. View FIFO waitlist\n"
             << "8. Suggest cities\n"
             << "9. Undo last booking\n"
             << "10. Save\n"
             << "0. Exit\n";
        int choice;
        if (!getNumber("Enter choice: ", choice)) continue;
        if (choice == 0) { save(); cout << "Goodbye!\n"; break; }
        if (choice == 1) inventory.displayAll();
        else if (choice == 2) searchRoutes();
        else if (choice == 3) {
            char id[20]; cout << "Transport ID: "; cin.getline(id, 20);
            Transport* t = ranking.lookup(id);
            if (t) t->display(); else cout << "Not found.\n";
        }
        else if (choice == 4) bookTicket();
        else if (choice == 5) cancelTicket();
        else if (choice == 6) bookings.displayBookings();
        else if (choice == 7) showWaitlist();
        else if (choice == 8) {
            char prefix[40]; cout << "City starts with: "; cin.getline(prefix, 40);
            search.suggestCities(prefix);
        }
        else if (choice == 9) undoBooking();
        else if (choice == 10) save();
        else cout << "Invalid choice.\n";
    }
}
