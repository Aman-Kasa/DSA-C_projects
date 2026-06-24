# DSA-C Project 2

> Author: AMAN KASA  |  Language: C  |  Domain: Data Structures & Algorithms

## Overview

This repository contains five C programs built for a data structures and algorithms assignment. The solutions focus on practical queue, heap, and graph problems, with each program demonstrating a different algorithmic idea in a small, readable implementation.

The project highlights:

- Max-heap operations for priority-driven workflows
- Graph modeling with adjacency matrices and edge lists
- Kruskal's Minimum Spanning Tree algorithm
- Bellman-Ford shortest path analysis
- Breadth-first neighborhood-style inspection of a network

## Project Structure

```text
project-2_Assignment/
├── 1st-question/
│   ├── 1st-question
│   └── 1st-question.c
├── 2nd-question/
│   ├── 2nd-question
│   └── 2nd-question.c
├── 3rd-question/
│   ├── 3rd-question
│   └── 3rd-question.c
├── 4th-question/
│   ├── 4th-question
│   └── 4th-question.c
├── 5th-question/
│   ├── 5th-question
│   └── 5th-question.c
└── PDF-report/
```

## Question Summary

| Question | Topic | What it demonstrates |
| --- | --- | --- |
| 1 | Max Heap / Priority Queue | Builds a heap, inserts an urgent job, then removes the top-priority job |
| 2 | Max Heap / Screening Queue | Orders passengers by risk, inserts a new high-risk passenger, then removes it |
| 3 | Kruskal's Algorithm | Builds a minimum-cost fiber network using Union-Find and edge sorting |
| 4 | BFS-style Network Exposure | Inspects direct neighbors of a compromised workstation and reports the strongest link |
| 5 | Bellman-Ford Algorithm | Finds shortest paths between branches and checks for negative-weight cycles |

## How To Run

Compile any question with `gcc` and run the generated executable.

```bash
gcc 1st-question/1st-question.c -o 1st-question/1st-question
./1st-question/1st-question
```

Repeat the same pattern for the remaining files by replacing the folder and source name.

Example:

```bash
gcc 3rd-question/3rd-question.c -o 3rd-question/3rd-question
./3rd-question/3rd-question
```

## Output Highlights

- Question 1 prints the heap before and after inserting and removing an urgent job.
- Question 2 shows the screening queue ordered by risk, then updates the heap after insertion and deletion.
- Question 3 lists accepted and rejected edges while constructing the MST.
- Question 4 prompts for a workstation label and reports the directly connected nodes.
- Question 5 prints minimum-cost routes from the source branch and flags negative cycles when present.

## Notes

- The compiled binaries are stored next to their source files.
- The `PDF-report/` folder can be used for the written submission or supporting documentation.
- All programs are standalone and can be compiled independently.

## Technologies Used

- C programming language
- Standard C library
- Heap data structures
- Graph algorithms
- Union-Find / Disjoint Set

## Author

AMAN KASA
