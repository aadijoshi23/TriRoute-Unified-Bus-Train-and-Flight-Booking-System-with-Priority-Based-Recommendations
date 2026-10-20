#include "booking.h"
#include <iostream>
#include <cstring>
using namespace std;

static void copyText(char* output, int maxSize, const char* input) {
    strncpy(output, input, maxSize - 1);
    output[maxSize - 1] = '\0';
}

Booking* BookingSystem::find(int bookingId) {
    for (int i = 0; i < size; i++)
        if (records[i].id == bookingId) return &records[i];
    return nullptr;
}

const Booking* BookingSystem::at(int index) const {
    return index >= 0 && index < size ? &records[index] : nullptr;
}

int BookingSystem::book(const char* passenger, const char* transportId, int quantity) {
    Transport* trip = inventory.findById(transportId);
    if (!trip || passenger[0] == '\0' || quantity <= 0 ||
        quantity > trip->getTotalSeats() || size >= 300) return -1;

    bool canBook = trip->getAvailableSeats() >= quantity && waiting.firstFor(transportId) == -1;
    Booking b = {};
    b.id = nextId;
    copyText(b.passenger, sizeof(b.passenger), passenger);
    copyText(b.transportId, sizeof(b.transportId), transportId);
    b.quantity = quantity;
    b.amount = trip->getFare() * quantity;
    copyText(b.status, sizeof(b.status), canBook ? "CONFIRMED" : "WAITING");

    if (canBook) {
        if (!trip->reserveSeats(quantity)) return -1;
    } else if (!waiting.enqueue(b.id, transportId)) {
        return -1;  // Full waiting queue, don't add a partial record.
    }
    records[size++] = b;
    nextId++;
    cout << "Booking " << b.id << " " << b.status << " | Estimated cost: Rs " << b.amount << '\n';
    if (canBook) cout << "Simulated payment: SUCCESS (demo only, no money charged).\n";
    return b.id;
}

void BookingSystem::promote(const char* transportId) {
    Transport* trip = inventory.findById(transportId);
    if (!trip) return;
    while (true) {
        int bookingId = waiting.firstFor(transportId);
        if (bookingId == -1) return;
        Booking* b = find(bookingId);
        if (!b || strcmp(b->status, "WAITING") != 0) {
            waiting.remove(bookingId);
            continue;
        }
        // Strict FIFO: a later waiting passenger cannot overtake the first.
        if (b->quantity > trip->getAvailableSeats()) return;
        if (!trip->reserveSeats(b->quantity)) return;
        waiting.remove(bookingId);
        copyText(b->status, sizeof(b->status), "CONFIRMED");
        cout << "Waitlist promoted: Booking " << bookingId << " is now CONFIRMED.\n";
    }
}

bool BookingSystem::cancel(int bookingId) {
    Booking* b = find(bookingId);
    if (!b || strcmp(b->status, "CANCELED") == 0) return false;
    Transport* trip = inventory.findById(b->transportId);
    if (!trip) return false;
    if (strcmp(b->status, "CONFIRMED") == 0) {
        trip->releaseSeats(b->quantity);
        copyText(b->status, sizeof(b->status), "CANCELED");
        cout << "Booking canceled. Simulated refund: Rs " << b->amount << '\n';
        promote(b->transportId);
    } else {
        waiting.remove(b->id);
        copyText(b->status, sizeof(b->status), "CANCELED");
        cout << "Waitlisted booking canceled (no payment collected).\n";
    }
    return true;
}

bool BookingSystem::addLoaded(const Booking& b) {
    if (size >= 300 || find(b.id) || !inventory.findById(b.transportId)) return false;
    records[size++] = b;
    if (b.id >= nextId) nextId = b.id + 1;
    return true;
}

void BookingSystem::displayBookings() const {
    if (!size) { cout << "No bookings yet.\n"; return; }
    cout << "ID    Passenger         Transport  Qty  Status       Amount (Rs)\n";
    for (int i = 0; i < size; i++) {
        const Booking& b = records[i];
        cout << b.id << "  " << b.passenger << "  " << b.transportId
             << "  " << b.quantity << "    " << b.status << "    " << b.amount << '\n';
    }
}
