# 📦 DSA-C Project 2 — Assignment
> **Author:** AMAN KASA &nbsp;|&nbsp; **Language:** C &nbsp;|&nbsp; **Domain:** Data Structures & Algorithms

---

## 🗂️ Project Overview

This project is a collection of **4 real-world DSA problems** implemented in C, covering **Heap-based Priority Queues** and **Graph Algorithms**. Each solution models a practical scenario — from job scheduling to network forensics.

---

## 📁 File Structure

```
project-2_Assignment/
├── 1st-question.c       # Job Scheduling — Max-Heap Priority Queue
├── 2nd-question.c       # Airport Security — Passenger Risk Screening
├── 4th-question.c       # Cybersecurity — BFS Network Exposure Tracking
├── 5th-question.c       # Banking Network — Bellman-Ford Risk Path Audit
└── [Project 2_ Assignment]{DSA-C}(AMAN-KASA).pdf
```

---

## 🔍 Questions Breakdown

### ❶ Job Scheduling — Max-Heap Priority Queue
**File:** `1st-question.c`

Models a **CPU job scheduler** using a Max-Heap. Jobs are identified by a letter ID and an integer priority.

| Operation | Description |
|-----------|-------------|
| 🏗️ Build Heap | Constructs initial Max-Heap from 11 jobs (A–K) |
| ➕ Insert | Inserts an urgent job `(L:100)` and restores heap via `heapifyUp` |
| ❌ Delete Max | Aborts the highest-priority job and restores heap via `heapifyDown` |

**Key Concepts:** `heapifyUp`, `heapifyDown`, Max-Heap

---

### ❷ Airport Security — Passenger Risk Screening
**File:** `2nd-question.c`

Simulates an **airport security priority queue** where passengers are screened in order of their risk score (highest first).

| Operation | Description |
|-----------|-------------|
| 🏗️ Build Heap | Builds Max-Heap from 6 passengers |
| 📋 Extract All | Prints full screening order by extracting from a temp copy |
| ➕ Insert | Adds `UrgentGuest` with risk score `98` |
| ❌ Clear | Removes the highest-risk passenger and restores heap |

**Key Concepts:** Max-Heap, in-place extraction, `memcpy` for non-destructive preview

---

### ❹ Cybersecurity — BFS Network Exposure Tracking
**File:** `4th-question.c`

Models a **10-node workstation network** (A–J) as a weighted adjacency matrix. Given a suspected compromised node, the program performs a **BFS one-hop neighbor scan** to identify directly connected workstations and the highest-risk transaction link.

| Feature | Description |
|---------|-------------|
| 🔗 Graph Model | 10×10 weighted adjacency matrix |
| 🔍 BFS Scan | Finds all direct neighbors of the input node |
| ⚠️ Forensics Report | Reports total exposure vectors & highest-weight link |

**Key Concepts:** Adjacency Matrix, BFS traversal, Graph neighbor detection

---

### ❺ Banking Network — Bellman-Ford Risk Path Audit
**File:** `5th-question.c`

Simulates a **10-branch banking network** where transaction costs are edge weights. Uses the **Bellman-Ford algorithm** to compute the minimum-cost path from branch `A` to all others, and detects negative-weight cycles (fraud indicators).

| Feature | Description |
|---------|-------------|
| 🏦 Graph Model | 15 directed weighted edges across 10 branches (A–J) |
| 📉 Shortest Paths | Computes optimal routing from source `A` |
| 🚨 Cycle Detection | Flags negative-weight cycles as critical alerts |
| 🗺️ Path Backtracking | Prints full optimal route for each destination |

**Key Concepts:** Bellman-Ford, Edge relaxation, Negative cycle detection, Path reconstruction

---

## 🛠️ How to Compile & Run

Each question is a standalone C file. Compile with `gcc`:

```bash
# Question 1
gcc 1st-question.c -o q1 && ./q1

# Question 2
gcc 2nd-question.c -o q2 && ./q2

# Question 4
gcc 4th-question.c -o q4 && ./q4

# Question 5
gcc 5th-question.c -o q5 && ./q5
```

> ⚠️ Question 4 requires user input — enter a workstation marker (A–J) when prompted.

---

## 🧠 Concepts Covered

| Concept | Used In |
|---------|---------|
| Max-Heap | Q1, Q2 |
| heapifyUp / heapifyDown | Q1, Q2 |
| Priority Queue | Q1, Q2 |
| Adjacency Matrix | Q4 |
| BFS Traversal | Q4 |
| Bellman-Ford Algorithm | Q5 |
| Negative Cycle Detection | Q5 |
| Path Reconstruction | Q5 |

---

## 📄 License

This project is submitted as part of a **DSA course assignment**. All implementations are original work by **AMAN KASA**.
