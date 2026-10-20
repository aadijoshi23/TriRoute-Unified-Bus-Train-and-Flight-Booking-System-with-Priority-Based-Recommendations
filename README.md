<p align="center">
  <img src="docs/logogehu.png" alt="Graphic Era Hill University Logo" width="260"/>
</p>

# TriRoute: Unified Bus, Train & Flight Booking System

### Priority-Based Object-Oriented Programming (OOP) & Data Structures and Algorithms (DSA) Implementation

**Course Project: Project-Based Learning (PBL) | Department of Computer Science & Engineering**
**Graphic Era Hill University — Dehradun • Bhimtal • Haldwani**

---

[![Language: C++17](https://img.shields.io/badge/Language-C%2B%2B17-00599C?logo=c%2B%2B)](https://en.cppreference.com/w/cpp/17)
[![Compiler: GCC / MinGW](https://img.shields.io/badge/Compiler-GCC%20%2F%20MinGW-brightgreen?logo=gnu)](https://gcc.gnu.org/)
[![Interface: Console Menu](https://img.shields.io/badge/Interface-Console%20Menu-informational)](#-modular-codebase-organization)
[![Paradigm: OOP + DSA](https://img.shields.io/badge/Paradigm-OOP%20%2B%20DSA-purple)](#-data-structures--algorithmic-design)
[![Project Progress](https://img.shields.io/badge/Progress-Phase%202%20In%20Progress-yellow)](#-project-progress--member-task-matrix)

> **Note:** badges above reflect a console-based build inferred from this repo's file list (`menu.h/cpp`, no API or frontend files present). If an API + browser frontend is still planned separately, update the "Interface" badge and the Architecture section below.

---

## 📌 Repository Description

> **TriRoute** is a unified travel booking system written in **C++17** covering Bus, Train, and Flight search in one program. A user selects a transport mode, enters source, destination, date and a priority — **Cheapest**, **Fastest**, or **Balanced** — and the system scores matching options and ranks them using the structure suited to the job: a **priority queue (min-heap)** for top-K ranking, a **hash map** for instant lookups, **sorting** for chronological views, and a **FIFO queue** for fair waitlisting. The design is built around a clean **Transport inheritance hierarchy** (`Bus`, `Train`, `Flight`) with CSV-backed persistence.

---

## 👥 Student & Team Information

### **Team Name: Runtime Error**

| Member Photo | Full Name & University Credentials | Project Role & Assigned Subsystems |
|---|---|---|
| <img src="docs/aadi_joshi.jpeg" width="80"/> | **Aadi Joshi** *(Team Lead)*<br>• **University Roll No:** `2519251`<br>• **Student ID:** `25101210278`<br>• **Email:** rj8578785@gmail.com | **Core OOP Domain Model** (`transport.h` / `transport.cpp`)<br>• Abstract `Transport` base class and the `Bus` / `Train` / `Flight` hierarchy.<br>• Encapsulated seat and pricing data behind controlled booking methods.<br>• Polymorphic fare calculation and detail display. |
| <img src="docs/neer_joshi.jpeg" width="80"/> | **Neer Joshi**<br>• **University Roll No:** `2519431`<br>• **Student ID:** `25101210277`<br>• **Email:** neer23102007@gmail.com | **DSA Ranking Engine** (`dsa.h` / `dsa.cpp`)<br>• Priority-queue (min-heap) ranking for Cheapest / Fastest / Balanced.<br>• Hash-map based ID/route lookup, chronological sorting.<br>• Leads the planned AI/NLP natural-language search module. |
| <img src="docs/saurav_baluni.jpeg" width="80"/> | **Saurav Baluni**<br>• **University Roll No:** `2519500`<br>• **Student ID:** `25101210324`<br>• **Email:** sanubaluni0@gmail.com | **Search & Lookup** (`search.h` / `search.cpp`)<br>• Route/date/mode filtering feeding the ranking engine. |
| <img src="docs/shubham_bangwal.jpeg" width="80"/> | **Shubham Jagdishprasad Bangwal**<br>• **University Roll No:** `2519521`<br>• **Student ID:** `25101210646`<br>• **Email:** shubhambangwal2007@gmail.com | **Booking System** (`booking.h` / `booking.cpp`)<br>• `User`, `Booking`, `Payment` handling and seat allocation.<br>• Booking validation and controlled seat-count updates. |
| <img src="docs/uttkarsh_kala.jpeg" width="80"/> | **Uttkarsh Kala**<br>• **University Roll No:** `2519555`<br>• **Student ID:** `25101281399`<br>• **Email:** kalauttkarsh.208@gmail.com | **Persistence** (`storage.h` / `storage.cpp`)<br>• CSV-based load/save for transport, user, and booking records (`transports.csv`).<br>• FIFO waitlist management and promotion on cancellation. |
| <img src="docs/dhrup_singh.jpeg" width="80"/> | **Dhrup Singh**<br>• **University Roll No:** `2519348`<br>• **Student ID:** `25101210356`<br>• **Email:** dhrupsingh777@gmail.com | **Menu / User Interface** (`menu.h` / `menu.cpp`)<br>• Console menu driving search, booking, and cancellation flows.<br>• City autocomplete and undo *(planned)*. |

---

## 🎯 Motivation & Problem Statement

### The Real-World Travel Booking Problem

- **Fragmented search**: Bus, train, and flight options live on separate platforms, forcing manual comparison.
- **Price-only sorting**: Most booking tools default to a plain price- or time-sorted list, ignoring that travellers weigh cost and duration differently.
- **Unfair waitlisting**: Without a disciplined first-come-first-served structure, waitlist promotion can be inconsistent.
- **No single source of truth**: Inventory, bookings, and cancellations need to stay consistent across every subsequent search.

### Why Existing Solutions Fall Short

1. **Manual comparison across apps** — cheap, but entirely dependent on the user cross-checking price and duration by hand.
2. **Commercial OTA platforms** — feature-rich, but closed-source, siloed by transport mode, and not built to demonstrate OOP/DSA design.

### The TriRoute Solution

TriRoute converts a travel priority into a weighted score and uses a min-heap to retrieve only the best-ranked options — the same principle real search engines use to avoid sorting an entire inventory for a handful of top results.

---

## 🏗️ System Architecture

> **Architecture note:** this section describes a console/menu-driven build, inferred from `menu.h/cpp` and the absence of API or frontend files in the repo. If a browser frontend and REST API (`GET /search`, `POST /book`, `POST /cancel`) are still planned, add an `api/` module and a `frontend/` folder, and update this section accordingly.

### Component Breakdown

1. **Console Menu (`menu.h` / `menu.cpp`)** — presents the Bus/Train/Flight choice, collects source, destination, date and priority, and displays ranked results.
2. **OOP Domain Core (`transport.h` / `transport.cpp`)** — abstract `Transport` class with `Bus`, `Train`, `Flight` derived classes; encapsulation and polymorphic fare/display logic.
3. **DSA Ranking Engine (`dsa.h` / `dsa.cpp`)** — min-heap ranking, hash-map lookup, sorting.
4. **Search & Lookup (`search.h` / `search.cpp`)** — filters transport records by route, date, and mode before scoring.
5. **Booking System (`booking.h` / `booking.cpp`)** — `User`, `Booking`, `Payment`, seat validation and allocation.
6. **Persistence (`storage.h` / `storage.cpp`)** — CSV read/write (`transports.csv`), FIFO waitlist handling.

---

## ⚡ Data Structures & Algorithmic Design

| Data Structure | Primary Operation | Why It's Used |
|---|---|---|
| **Priority Queue (Min-Heap)** | Retrieve top-K ranked options | Faster than sorting the whole list when only the best few results matter |
| **Hash Map** | ID / route lookup | Near-constant-time lookup instead of scanning every record |
| **Sorting** | Chronological schedule view | Presents options in departure-time order when ranking isn't requested |
| **Queue (FIFO)** | Waitlist handling | First-come-first-served seat promotion when a seat frees up |

---

## 🧩 Modular Codebase Organization

---

## 🛠️ Build and Run

### Prerequisites

- **Compiler**: GCC / MinGW with C++17 support (`g++` version 6.3 or higher).
- **Environment**: PowerShell, Command Prompt, or any terminal.

### Compile


### Run
*(Confirm the exact compile line against your actual `main.cpp` includes — if any file doesn't compile standalone yet, say so and I'll adjust.)*

---

## 🧪 Testing & Validation Status

| # | Test Scenario | Target Module | Status | Notes |
|---|---|---|---|---|
| 1 | Transport polymorphism | `transport.cpp` | *Needs confirmation* | Bus/Train/Flight should invoke correct overridden fare/display logic |
| 2 | Route/date/mode filtering | `search.cpp` | *Needs confirmation* | Should return only matching records |
| 3 | Priority ranking | `dsa.cpp` | *Needs confirmation* | Cheapest/Fastest/Balanced order should match manual calculation |
| 4 | Booking validation | `booking.cpp` | *Needs confirmation* | Valid bookings reduce seats once; invalid quantities rejected |
| 5 | CSV load/save | `storage.cpp` | *Needs confirmation* | Data should persist correctly across restarts |
| 6 | FIFO waitlist | `storage.cpp` | *Needs confirmation* | First waitlisted user promoted first |

*(Replace "Needs confirmation" with Pass/Fail once each module is actually run — I can't verify this without seeing the code or your test results.)*

---

## 📊 Project Progress & Member Task Matrix

> Status below reflects **file presence only**, not verified functionality. Update per-module once confirmed.

| Team Member | File(s) Present | Status |
|---|---|---|
| Aadi Joshi | `transport.h/.cpp` | *Confirm: working / partial / stub* |
| Neer Joshi | `dsa.h/.cpp` | *Confirm: working / partial / stub* |
| Saurav Baluni | `search.h/.cpp` | *Confirm: working / partial / stub* |
| Shubham Bangwal | `booking.h/.cpp` | *Confirm: working / partial / stub* |
| Uttkarsh Kala | `storage.h/.cpp` | *Confirm: working / partial / stub* |
| Dhrup Singh | `menu.h/.cpp` | *Confirm: working / partial / stub* |
| — | AI/NLP module | **Pending** — no files present in repo |
| — | REST API / frontend | **Pending or descoped** — confirm which |

---

**Department of Computer Science & Engineering**
**Graphic Era Hill University, Dehradun**
*TriRoute — Unified Bus, Train & Flight Booking System*