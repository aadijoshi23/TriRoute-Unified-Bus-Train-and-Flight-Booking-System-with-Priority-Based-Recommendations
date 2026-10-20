
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