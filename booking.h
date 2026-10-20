#ifndef BOOKING_H
#define BOOKING_H

#include "transport.h"
#include "storage.h"

struct Booking {
    int id;
    char passenger[50];
    char transportId[20];
    int quantity;
    char status[16];  // CONFIRMED, WAITING, CANCELED
    int amount;
};

// Shubham: ticket booking, cancellation, seat allocation and dummy payment.
class BookingSystem {
private:
    Booking records[300];
    int size, nextId;
    TransportInventory& inventory;
    WaitlistQueue& waiting;
    void promote(const char* transportId);
public:
    BookingSystem(TransportInventory& data, WaitlistQueue& queue)
        : size(0), nextId(1001), inventory(data), waiting(queue) {}
    int book(const char* passenger, const char* transportId, int quantity);
    bool cancel(int bookingId);
    Booking* find(int bookingId);
    const Booking* at(int index) const;
    int count() const { return size; }
    bool addLoaded(const Booking& booking);
    void displayBookings() const;
};

#endif
