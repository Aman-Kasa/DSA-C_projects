# 🚨 Emergency Dispatch Incident Tracker

A terminal-based incident management system for city emergency response centers. Built in **C** using a **doubly linked list**, it records and navigates incidents from ambulance teams, police units, and fire stations in real time.

---

## 📋 Overview

Incidents arrive continuously and are stored in chronological order. Operators can navigate through the incident history, enable live monitoring, and manage the incident log — all from a simple command interface.

---

## 🏗️ Data Structure

| Choice | Why |
|---|---|
| Doubly Linked List | Enables O(1) forward/backward navigation between incidents |
| Fixed capacity (25) | Bounded memory — oldest incident is auto-discarded on overflow |

```
HEAD ←→ [Incident 1] ←→ [Incident 2] ←→ ... ←→ [Incident 25] ← TAIL
                              ↑
                           current
```

---

## ⚙️ Functional Requirements

| Requirement | Status |
|---|---|
| Incidents stored in chronological order | ✅ |
| Active pointer starts at oldest incident | ✅ |
| Max 25 incidents (oldest auto-discarded) | ✅ |
| Forward / backward navigation | ✅ |
| Live incident monitoring toggle | ✅ |
| Delete all incidents | ✅ |
| Save session and quit | ✅ |

---

## 🕹️ Operator Commands

| Key | Action |
|---|---|
| `n` | Add a new incident manually |
| `f` | View next (newer) incident |
| `b` | View previous (older) incident |
| `l` | Enable live incident monitoring |
| `s` | Stop live monitoring |
| `d` | Delete all incidents |
| `q` | Save session and quit |

---

## 🚀 Build & Run

```bash
gcc -o emergency_tracker 1st-question.c
./emergency_tracker
```

---

## 💡 Example Session

```
--- Emergency Tracker (3/25) | Live: OFF ---
Viewing: Car accident on Main St
Commands: [n]ew, [f]wd, [b]ack, [l]ive, [s]top, [d]elete, [q]uit: f

--- Emergency Tracker (3/25) | Live: OFF ---
Viewing: Structure fire at 5th Ave
```

---

## 📁 Project Structure

```
question-1/
├── 1st-question.c   # Source code
├── 1st-question     # Compiled binary
└── README.md        # Documentation
```

---

## 🧠 Key Implementation Notes

- **Memory safety** — `deleteAll()` frees every node; called on exit to prevent leaks
- **Overflow handling** — when capacity hits 25, `head` is freed before the new node is appended
- **Live monitoring** — simulated per loop iteration; in production this would run on a separate thread with non-blocking I/O
