#include "transport.h"
#include <cstring>
#include <iomanip>
using namespace std;

static void safeCopy(char* dest, int size, const char* src) {
    strncpy(dest, src, size - 1);
    dest[size - 1] = '\0';
}

Transport::Transport(const char* itemId, const char* from, const char* to,
                     const char* tripDate, const char* depart, int minutes,
                     int price, int seats, int available, const char* attribute)
    : duration(minutes), fare(price), totalSeats(seats), availableSeats(available) {
    safeCopy(id, sizeof(id), itemId);
    safeCopy(source, sizeof(source), from);
    safeCopy(destination, sizeof(destination), to);
    safeCopy(date, sizeof(date), tripDate);
    safeCopy(departure, sizeof(departure), depart);
    safeCopy(detail, sizeof(detail), attribute);
}

bool Transport::reserveSeats(int quantity) {
    if (quantity <= 0 || quantity > availableSeats) return false;
    availableSeats -= quantity;
    return true;
}

void Transport::releaseSeats(int quantity) {
    if (quantity > 0 && quantity <= totalSeats - availableSeats)
        availableSeats += quantity;
}

void Transport::display() const {
    cout << left << setw(9) << id << setw(8) << mode()
         << setw(13) << source << setw(13) << destination
         << setw(13) << date << setw(8) << departure
         << setw(9) << duration << setw(9) << fare
         << availableSeats << "/" << totalSeats << "  " << detail << '\n';
}

void Bus::display() const { Transport::display(); }
void Train::display() const { Transport::display(); }
void Flight::display() const { Transport::display(); }

TransportInventory::~TransportInventory() {
    for (int i = 0; i < size; i++) delete items[i];
}

bool TransportInventory::add(Transport* item) {
    if (item == nullptr || size >= MAX_TRANSPORTS || findById(item->getId())) return false;
    items[size++] = item;
    return true;
}

Transport* TransportInventory::at(int index) const {
    return index >= 0 && index < size ? items[index] : nullptr;
}

Transport* TransportInventory::findById(const char* id) const {
    for (int i = 0; i < size; i++)
        if (strcmp(items[i]->getId(), id) == 0) return items[i];
    return nullptr;
}

void TransportInventory::displayAll() const {
    cout << "ID       Mode    From         To           Date         Time    Minutes  Rs       Seats   Details\n";
    for (int i = 0; i < size; i++) items[i]->display();
}
