# DSA-C Projects — Week 3 Formative Assignment

> **Language:** C &nbsp;|&nbsp; **Topic:** Data Structures & Algorithm Analysis &nbsp;|&nbsp; **Total Marks:** 17

---

## Project Structure

```
DSA-C_projects/
├── 1st-question/
│   ├── question-1.c          # Quick Sort on student records
│   ├── students.txt          # Input file
│   └── sorted_students.txt   # Output file (auto-generated)
├── 2nd-question/
│   └── question-2.c          # Doubly linked list bus route navigator
├── 3rd-question/
│   └── question-3.c          # Binary tree with hierarchical queries
├── 4th-question/
│   ├── question-4.c          # BST indexed by student last name
│   └── raw_students.txt      # Input file
└── README.md
```

---

## How to Compile & Run

Each question compiles independently with GCC:

```bash
# Question 1
cd 1st-question && gcc -o question-1 question-1.c && ./question-1

# Question 2
cd 2nd-question && gcc -o question-2 question-2.c && ./question-2

# Question 3
cd 3rd-question && gcc -o question-3 question-3.c && ./question-3

# Question 4
cd 4th-question && gcc -o question-4 question-4.c && ./question-4
```

---

---

# Question 1 — Dynamic Sorting System [3 pts]

## Overview

Reads student records from `students.txt`, sorts them in ascending order of grade using **Quick Sort**, and writes the result to `sorted_students.txt`. Ties in grade are broken lexicographically by first name.

## Input Format (`students.txt`)

```
Firstname Lastname StudentID Grade
John Doe S101 85.5
Jane Smith S102 92.0
```

## Output Format (`sorted_students.txt`)

```
Alice White S104 78.00
Bob Brown S103 85.50
John Doe S101 85.50
Jane Smith S102 92.00
```

## Key Design Decisions

- Records are loaded into a dynamically allocated array that doubles in capacity via `realloc` when full — handles arbitrarily large datasets.
- The `compareStudents` comparator sorts by grade ascending; on equal grades it falls back to `strcmp` on first name.
- The last element of each subarray is chosen as the pivot (Lomuto partition scheme).

---

## Algorithm Analysis — Quick Sort

Let **T(n)** define the time to sort **n** student records. The partition step visits every element in the subarray exactly once → **Θ(n)** work per level.

### Best Case

Occurs when the pivot consistently splits the array into two equal halves of size n/2.

**Recurrence:**
```
T(n) = 2T(n/2) + Θ(n)
```

**Resolution via Master Theorem** (a=2, b=2, f(n)=Θ(n)):
```
log_b(a) = log_2(2) = 1
f(n) = Θ(n^1)  →  Case 2 applies
```
```
T(n) ∈ O(n log n)
```

### Average Case

Occurs when pivot selections produce reasonably balanced partitions across an arbitrary unsorted dataset. A representative unbalanced split such as 1/10 and 9/10 still yields logarithmic depth.

**Recurrence:**
```
T(n) = T(n/10) + T(9n/10) + Θ(n)
```

Summing work across all recursion levels (each level totals Θ(n), depth ≈ log n):
```
T(n) ∈ O(n log n)
```

### Worst Case

Occurs when the pivot is always the minimum or maximum element — e.g., input already sorted in ascending or descending order. Each partition removes only one element.

**Recurrence:**
```
T(n) = T(n−1) + Θ(n)
```

**Expansion:**
```
T(n) = Θ(n) + Θ(n−1) + ... + Θ(1)
     = Σ(i=1 to n) Θ(i)
     = n(n+1)/2
```
```
T(n) ∈ O(n²)
```

---

## Critical Evaluation — Quick Sort vs. Insertion Sort

### Dataset Scalability

| Algorithm | Time Complexity | Operations for n=100,000 |
|-----------|----------------|--------------------------|
| Insertion Sort | O(n²) | ~10,000,000,000 |
| Quick Sort (avg) | O(n log n) | ~1,700,000 |

Insertion Sort becomes completely impractical at institutional scale.

### File-Based Input

Flat files stream records sequentially into memory arrays. Insertion Sort continuously shifts elements rightward during each insertion, producing large numbers of memory write operations proportional to the current array size. Quick Sort processes records **in-place** using boundary indices and partition-based swapping, significantly reducing memory movement overhead.

### Cache Locality & Performance

Quick Sort's partition step accesses memory sequentially through adjacent array indices, aligning efficiently with modern CPU cache prefetching (L1/L2 cache). Insertion Sort's repeated shifting on large arrays increases cache misses and memory write pressure.

**Conclusion:** Quick Sort delivers O(n log n) average performance, in-place operation, and superior cache behavior — making it the correct choice for large file-based academic datasets.

---

---

# Question 2 — Bus Route Navigation System [4 pts]

## Overview

Models a bus route as a **doubly linked list** where each node is a bus stop. Supports forward traversal (home → campus), backward traversal (campus → home), and dynamic stop insertion — all via a menu-driven interface.

## Features

| Feature | Detail |
|---------|--------|
| Data structure | Doubly linked list |
| Traversal | Forward and backward with 3-second delays |
| Insertion | O(1) append at end via tail pointer |
| Cross-platform delay | `sleep()` on Linux/macOS, `Sleep()` on Windows |
| Memory | All nodes freed on exit — no leaks |

## Sample Interaction

```
Initialize Bus Route Database:
How many base entries would you like to create? 3
Enter name for stop 1: Home
Enter name for stop 2: City Center
Enter name for stop 3: Campus

=== Navigation Controller Menu ===
1. Move Forward (Home -> Campus)
2. Move Backward (Campus -> Home)
3. Add New Bus Stop at End of Route
4. Exit Navigation System
Select option: 1

--- Commencing Route Traversal ---
Arriving at Stop #1: [Home]
Arriving at Stop #2: [City Center]
Arriving at Stop #3: [Campus]
--- Destination Reached. Traversal terminated. ---
```

---

## Algorithm Analysis — Appending a New Bus Stop

### Time Complexity: O(1) — Constant Time

### Justification

In a naive linked list, appending requires traversing from the head to the last node — visiting every node — giving **O(n)** time.

This implementation maintains a dedicated `tail` pointer that always holds the exact memory address of the last node. Insertion therefore requires only three pointer assignments regardless of list size:

```c
(*tail)->next = newNode;   // link old tail forward to new node
newNode->prev  = *tail;    // link new node backward to old tail
*tail          = newNode;  // advance tail pointer
```

These three operations execute in constant time because no traversal is needed. Whether the route has 5 stops or 50,000 stops, the cost is identical.

```
T(n) ∈ O(1)
```

---

---

# Question 3 — Hierarchical Binary Tree System [5 pts]

## Overview

Constructs a **complete binary tree** from an array of 68 distinct integer component identifiers (1000–1067) using **level-order (BFS) insertion**. Supports querying any node for its parent, sibling, and grandchildren.

## Insertion Rule

Nodes are inserted level by level, left to right, using a queue-assisted BFS traversal. This guarantees the tree remains as complete and balanced as possible at all times, preventing degeneration into a linked-list structure.

```
Level 0:                    1000
Level 1:            1001            1002
Level 2:        1003    1004    1005    1006
Level 3:      1007 1008 ...
```

## Supported Operations

| Operation | Description |
|-----------|-------------|
| Root node | Always the first inserted element (1000) |
| Leaf nodes | All nodes with no children |
| Parent | Direct ancestor of a queried node |
| Sibling | The other child of the same parent |
| Grandchildren | Children of the queried node's children |

## Sample Output

```
Root Identifier Node: 1000
Identified Leaf Nodes: 1063 1064 1065 ... 1034 1035 ...

Enter Target ID to query relational metadata: 1005
Parent Node: 1002
Sibling Nodes: 1006
Grandchildren Nodes: 1023 1024 1025 1026
```

Invalid node IDs are handled gracefully:
```
Enter Target ID to query relational metadata: 9999
Error: Target ID 9999 does not exist in the system architecture.
```

---

## Algorithm Analysis — Level-Order Insertion

### Time Complexity: O(n) — Linear Time

### Justification

When inserting a new node into a complete binary tree containing **n** existing nodes, the algorithm performs a BFS traversal using a queue to find the first available child slot.

At each step it checks:
1. Is the current node's **left child** empty? → insert here.
2. Is the current node's **right child** empty? → insert here.
3. Otherwise → enqueue both children and continue.

In the **worst case**, the first available slot is the last position on the deepest level. The BFS may need to visit all **n** existing nodes before finding it.

```
Worst-case traversal: n nodes visited
T(n) ∈ O(n)
```

> Note: For a perfectly complete binary tree, the insertion slot is always at depth ⌊log₂n⌋, so the average case is closer to **O(log n)**. However, the worst-case bound is **O(n)**.

---

---

# Question 4 — BST Student Record Index [5 pts]

## Overview

Reads student records from `raw_students.txt` and organizes them into a **Binary Search Tree (BST)** keyed by last name. Supports fast last-name lookup that returns all matching student records.

## Input Format (`raw_students.txt`)

```
Firstname|Lastname|Grade
John|Doe|88.5
Jane|Doe|92.0
Mark|Zuckerberg|75.0
```

## Duplicate Last Name Handling

When two students share the same last name, a new `StudentDataNode` is prepended to a linked list stored at that BST node. All records under the same last name are retrieved together during search.

```
BST Node "Doe"
    └── dataHead → [Jane, 92.0] → [John, 88.5] → NULL
```

## Sample Interaction

```
Enter Student Last Name to search: Doe

Matches found for 'Doe':
- Jane | Grade: 92.00
- John | Grade: 88.50
```

```
Enter Student Last Name to search: Nobody
No records found for: 'Nobody'.
```

---

## Algorithm Analysis — BST Search

The search time is directly determined by the **height h** of the tree.

| Case | Complexity | Structural Context |
|------|-----------|-------------------|
| Best | O(1) | Target matches the root node on the first comparison |
| Average | O(log n) | Tree is reasonably balanced; height ≈ log₂n; search space halves at each step |
| Worst | O(n) | Records inserted in sorted alphabetical order; BST degenerates into a linear chain |

---

## Conceptual Understanding

### BST vs. General Binary Tree

| Property | General Binary Tree | Binary Search Tree |
|----------|--------------------|--------------------|
| Node ordering | None | Left subtree < root < right subtree |
| Search efficiency | O(n) — must scan all nodes | O(log n) average |
| Use case | Hierarchical structure | Efficient keyed lookup |

A BST enforces a strict ordering invariant at every node, enabling the search algorithm to eliminate half the remaining candidates at each comparison step.

### Linear File Search vs. BST Search

| Approach | Time Complexity | Mechanism |
|----------|----------------|-----------|
| Linear file scan | O(n) | Reads line-by-line from disk until match found |
| BST search | O(log n) average | Traverses in-memory tree, halving search space each step |

**Concrete example:** A balanced BST containing 65,536 records has height:
```
log₂(65536) = 16
```
A lookup requires at most **16 comparisons** vs. up to **65,536 comparisons** with linear scanning.

### Engineering Justification

For a growing university system, repeated linear file scans introduce major I/O bottlenecks as enrollment scales. By parsing all records into an in-memory BST at startup, every subsequent search operates in O(log n) time with no disk access. This makes the BST architecture significantly more scalable, efficient, and appropriate for institutional-scale academic data management.

---

## Memory Management

All four programs use `malloc`/`realloc`/`free` exclusively. No memory leaks — verified with AddressSanitizer (`-fsanitize=address`).

---
