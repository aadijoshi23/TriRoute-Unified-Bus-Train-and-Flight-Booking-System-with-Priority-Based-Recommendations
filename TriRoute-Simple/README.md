# TriRoute — Simple Console Version (No STL Containers)

A college OOP/DSA prototype demonstrating bus/train/flight search and booking, with six member-owned modules.

## Ownership: two C++ files per person

| Owner | Files | Role |
|---|---|---|
| Neer | `transport.h`, `transport.cpp` | OOP inheritance, virtual dispatch, inventory |
| Aadi | `dsa.h`, `dsa.cpp` | Manual min-heap, open-addressed hash lookup, selection sort |
| Saurav | `menu.h`, `menu.cpp` | Console input, menu, module integration |
| Shubham | `booking.h`, `booking.cpp` | Bookings, seat validation, cancellation, simulated payment |
| Uttkarsh | `storage.h`, `storage.cpp` | CSV persistence and circular FIFO queue |
| Dhrup | `search.h`, `search.cpp` | Route filtering, city suggestions, undo-booking stack |
| Shared | `main.cpp` | Launch and connect modules |

## Build and run (Windows / PowerShell)

Open the **TriRoute-Simple** directory in VS Code, then run:

```powershell
$env:Path = "C:\msys64\ucrt64\bin;" + $env:Path
g++ -std=c++17 -Wall -Wextra main.cpp transport.cpp dsa.cpp menu.cpp booking.cpp storage.cpp search.cpp -o TriRoute.exe
.\TriRoute.exe
```

The working directory must contain `transports.csv`. No CMake, Python, external libraries or HTTP server needed.

## What to demonstrate

- Search `Dehradun` -> `Delhi`, date `2027-01-15`, mode `all`.
- Cheapest: `TRT001`, fastest: `TRF001`, balanced: `TRT002`.
- Book and cancel `TRT001` to watch available seats decrease and increase.
- For FIFO demo, use a **copy of the folder**, lower the seats for one transport in that copy of `transports.csv` to `1,1`, book the last seat, add two waitlist bookings, then cancel the first booking.
- Restart to verify saved seats and booking records.
- Search by ID uses a manually implemented hash table with linear probing.
- Undo is deliberately scoped to **the most recent booking made in this program session**. It cancels that booking; it does not reverse arbitrary past actions.

## Files created at runtime

- `transport_state.csv`: saved seat counts (takes precedence over the seed CSV)
- `bookings.txt`: booking records
- `waitlist.txt`: pending booking IDs in FIFO order

To reset demo data, **after backing up if needed**, delete only these three generated files. Do not delete `transports.csv`.

## Restrictions and scope

- No `vector`, `map`, `unordered_map`, STL `priority_queue`, STL `stack`, STL `queue`, `std::sort`, smart pointers, HTTP, frontend or NLP.
- Uses fixed arrays and `char[]`, with standard C++ I/O and basic C string operations.
- Assumes simple CSV fields (no quoted commas) and bounded array capacities, appropriate for a classroom demo.
- Simulated payments only. This is not a production or real-money booking service.
- File updates are basic, not transactionally atomic; no concurrency handling.
- `transport_state.csv`, `bookings.txt`, and `waitlist.txt` represent a single local snapshot and should remain together.

## Git workflow

First create a backup branch from the existing TriRoute repository. Copy these files into the new branch after testing. Do **not** delete the old implementation before the new one builds.
