#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <iostream>

const int MAX_TRANSPORTS = 100;

class Transport {
private:
    char id[20], source[40], destination[40], date[16], departure[8], detail[50];
    int duration, fare, totalSeats, availableSeats;
public:
    Transport(const char* id, const char* source, const char* destination,
              const char* date, const char* departure, int duration, int fare,
              int totalSeats, int availableSeats, const char* detail);
    virtual ~Transport() {}
    virtual const char* mode() const = 0;
    virtual void display() const;
    const char* getId() const { return id; }
    const char* getSource() const { return source; }
    const char* getDestination() const { return destination; }
    const char* getDate() const { return date; }
    const char* getDeparture() const { return departure; }
    const char* getDetail() const { return detail; }
    int getDuration() const { return duration; }
    int getFare() const { return fare; }
    int getTotalSeats() const { return totalSeats; }
    int getAvailableSeats() const { return availableSeats; }
    bool reserveSeats(int quantity);
    void releaseSeats(int quantity);
};

class Bus : public Transport {
public:
    using Transport::Transport;
    const char* mode() const override { return "bus"; }
    void display() const override;
};

class Train : public Transport {
public:
    using Transport::Transport;
    const char* mode() const override { return "train"; }
    void display() const override;
};

class Flight : public Transport {
public:
    using Transport::Transport;
    const char* mode() const override { return "flight"; }
    void display() const override;
};

class TransportInventory {
private:
    Transport* items[MAX_TRANSPORTS];
    int size;
    TransportInventory(const TransportInventory&) = delete;
    TransportInventory& operator=(const TransportInventory&) = delete;
public:
    TransportInventory() : size(0) {}
    ~TransportInventory();
    bool add(Transport* item);
    int count() const { return size; }
    Transport* at(int index) const;
    Transport* findById(const char* id) const;
    void displayAll() const;
};

#endif
