#include "storage.h"
#include "booking.h"
#include <fstream>
#include <cstring>
#include <cstdlib>
using namespace std;

bool WaitlistQueue::enqueue(int bookingId, const char* transportId) {
    if (used == 200) return false;
    queue[rear].bookingId = bookingId;
    strncpy(queue[rear].transportId, transportId, 19);
    queue[rear].transportId[19] = '\0';
    rear = (rear + 1) % 200;
    used++;
    return true;
}

WaitlistEntry WaitlistQueue::dequeue() {
    WaitlistEntry entry = queue[front];
    front = (front + 1) % 200;
    used--;
    return entry;
}

WaitlistEntry WaitlistQueue::at(int position) const {
    WaitlistEntry empty = {-1, ""};
    if (position < 0 || position >= used) return empty;
    return queue[(front + position) % 200];
}

int WaitlistQueue::firstFor(const char* transportId) const {
    for (int i = 0; i < used; i++) {
        WaitlistEntry e = at(i);
        if (strcmp(e.transportId, transportId) == 0) return e.bookingId;
    }
    return -1;
}

bool WaitlistQueue::remove(int bookingId) {
    bool found = false;
    int originalCount = used;
    for (int i = 0; i < originalCount; i++) {
        WaitlistEntry e = dequeue();
        if (e.bookingId == bookingId) found = true;
        else enqueue(e.bookingId, e.transportId);
    }
    return found;
}

// Read one CSV record with 11 fields. Simple format (no quoted commas).
static bool addTransportLine(char* line, TransportInventory& inventory) {
    char* values[11];
    int fields = 0;
    char* part = strtok(line, ",\r\n");
    while (part && fields < 11) { values[fields++] = part; part = strtok(nullptr, ",\r\n"); }
    if (fields != 11 || strcmp(values[0], "id") == 0) return false;
    int duration = atoi(values[6]), fare = atoi(values[7]);
    int seats = atoi(values[8]), available = atoi(values[9]);
    if (duration <= 0 || fare < 0 || seats <= 0 || available < 0 || available > seats) return false;
    Transport* t = nullptr;
    if (strcmp(values[1], "bus") == 0)
        t = new Bus(values[0], values[2], values[3], values[4], values[5], duration, fare, seats, available, values[10]);
    else if (strcmp(values[1], "train") == 0)
        t = new Train(values[0], values[2], values[3], values[4], values[5], duration, fare, seats, available, values[10]);
    else if (strcmp(values[1], "flight") == 0)
        t = new Flight(values[0], values[2], values[3], values[4], values[5], duration, fare, seats, available, values[10]);
    if (!t) return false;
    if (!inventory.add(t)) { delete t; return false; }
    return true;
}

bool StorageManager::load(TransportInventory& inventory, BookingSystem& bookings, WaitlistQueue& waitlist) {
    ifstream state("transport_state.csv");
    bool savedState = state.is_open();
    ifstream seed;
    if (!savedState) seed.open("transports.csv");
    istream& file = savedState ? static_cast<istream&>(state) : static_cast<istream&>(seed);
    if (!file.good()) { cout << "Cannot open transports.csv in this folder.\n"; return false; }
    char line[400];
    file.getline(line, 400);  // CSV header
    while (file.getline(line, 400)) addTransportLine(line, inventory);
    if (inventory.count() == 0) return false;

    ifstream booked("bookings.txt");
    while (booked.getline(line, 400)) {
        Booking b = {};
        char* part = strtok(line, ",\r\n");
        if (!part) continue;
        b.id = atoi(part);
        part = strtok(nullptr, ",\r\n"); if (!part) continue;
        strncpy(b.passenger, part, sizeof(b.passenger) - 1);
        part = strtok(nullptr, ",\r\n"); if (!part) continue;
        strncpy(b.transportId, part, sizeof(b.transportId) - 1);
        part = strtok(nullptr, ",\r\n"); if (!part) continue;
        b.quantity = atoi(part);
        part = strtok(nullptr, ",\r\n"); if (!part) continue;
        strncpy(b.status, part, sizeof(b.status) - 1);
        part = strtok(nullptr, ",\r\n"); if (!part) continue;
        b.amount = atoi(part);
        if (b.id > 0 && b.quantity > 0 &&
            (strcmp(b.status, "CONFIRMED") == 0 || strcmp(b.status, "WAITING") == 0 || strcmp(b.status, "CANCELED") == 0))
            bookings.addLoaded(b);
    }
    ifstream waitingFile("waitlist.txt");
    while (waitingFile.getline(line, 400)) {
        int id = atoi(line);
        Booking* b = bookings.find(id);
        if (b && strcmp(b->status, "WAITING") == 0) waitlist.enqueue(id, b->transportId);
    }
    cout << "Loaded " << inventory.count() << " transport services.\n";
    return true;
}

bool StorageManager::save(const TransportInventory& inventory, const BookingSystem& bookings,
                          const WaitlistQueue& waitlist) {
    ofstream state("transport_state.csv");
    ofstream booked("bookings.txt");
    ofstream waitingFile("waitlist.txt");
    if (!state || !booked || !waitingFile) return false;
    state << "id,mode,source,destination,date,departure_time,duration_minutes,fare_rupees,total_seats,available_seats,attribute\n";
    for (int i = 0; i < inventory.count(); i++) {
        Transport* t = inventory.at(i);
        state << t->getId() << ',' << t->mode() << ',' << t->getSource() << ','
              << t->getDestination() << ',' << t->getDate() << ',' << t->getDeparture()
              << ',' << t->getDuration() << ',' << t->getFare() << ',' << t->getTotalSeats()
              << ',' << t->getAvailableSeats() << ',' << t->getDetail() << '\n';
    }
    for (int i = 0; i < bookings.count(); i++) {
        const Booking* b = bookings.at(i);
        booked << b->id << ',' << b->passenger << ',' << b->transportId << ','
               << b->quantity << ',' << b->status << ',' << b->amount << '\n';
    }
    for (int i = 0; i < waitlist.count(); i++) waitingFile << waitlist.at(i).bookingId << '\n';
    state.close(); booked.close(); waitingFile.close();
    return !state.fail() && !booked.fail() && !waitingFile.fail();
}
