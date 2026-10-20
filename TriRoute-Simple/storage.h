#ifndef STORAGE_H
#define STORAGE_H

#include "transport.h"

struct WaitlistEntry {
    int bookingId;
    char transportId[20];
};

// Uttkarsh: fixed-array circular FIFO queue.
class WaitlistQueue {
private:
    WaitlistEntry queue[200];
    int front, rear, used;
    WaitlistEntry dequeue();
public:
    WaitlistQueue() : front(0), rear(0), used(0) {}
    bool enqueue(int bookingId, const char* transportId);
    int firstFor(const char* transportId) const;
    bool remove(int bookingId);
    int count() const { return used; }
    WaitlistEntry at(int position) const;
};

class BookingSystem;
class StorageManager {
public:
    // On first launch loads transports.csv; later prefers saved state.
    bool load(TransportInventory& inventory, BookingSystem& bookings, WaitlistQueue& waitlist);
    bool save(const TransportInventory& inventory, const BookingSystem& bookings,
              const WaitlistQueue& waitlist);
};

#endif
