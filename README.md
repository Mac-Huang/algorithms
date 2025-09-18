# CS161 Algorithms Tutorial

> A comprehensive collection of algorithm implementations from Stanford CS161
>
> [中文版](README_zh.md)

## Author

**Mac Huang** - Algorithm implementations and educational materials for Stanford CS161 course.

---

## Table of Contents

- [Introduction](#introduction)
- [Data Structures](#data-structures)
- [Graph Algorithms](#graph-algorithms)
- [Dynamic Programming](#dynamic-programming)
- [Sorting Algorithms](#sorting-algorithms)
- [Hash Tables](#hash-tables)
- [Greedy Algorithms](#greedy-algorithms)
- [Getting Started](#getting-started)
- [Contributing](#contributing)

---

## Introduction

This repository contains implementations of fundamental algorithms and data structures covered in Stanford's CS161 course. Each implementation includes both complete code and skeleton templates for learning purposes.

### Features

- 📚 **Comprehensive Coverage** - Covers major topics in algorithms and data structures
- 🎯 **Educational Focus** - Includes skeleton files for practice
- 💻 **Multiple Languages** - Implementations in C and Python
- 🔗 **Learning Resources** - Links to online tutorials and visualizations

---

## Data Structures

### Trees

| File | Description | Learning Resources |
|------|-------------|-------------------|
| `avl.c` | AVL Tree implementation | [BS Tree Viz](https://xuming.ai/demos/bs-tree.html) • [VisuAlgo](https://visualgo.net/en/bst) • [GeeksforGeeks](https://www.geeksforgeeks.org/avl-tree-set-1-insertion/) |
| `optimized_avl.c` | Optimized AVL Tree | [CS161 Lecture Notes](http://web.stanford.edu/class/cs161/schedule.html) |
| `23trees.c` | 2-3 Tree implementation | [B+ Tree Viz](https://xuming.ai/demos/bplus-tree.html) • [2-3 Tree Visualization](https://www.cs.usfca.edu/~galles/visualization/BTree.html) |
| `splay.c` | Splay Tree implementation | [Splay Tree Visualization](https://www.cs.usfca.edu/~galles/visualization/SplayTree.html) |
| `bst_insert.c` | Binary Search Tree insertion | [BS Tree Viz](https://xuming.ai/demos/bs-tree.html) • [BST Visualization](https://www.cs.usfca.edu/~galles/visualization/BST.html) |
| `decision_trees.py` | Decision Trees implementation | [Visual Introduction](http://www.r2d3.us/visual-intro-to-machine-learning-part-1/) |

**Key Concepts:**
- Self-balancing trees
- Tree rotations
- Search, insertion, deletion operations
- Time complexity: O(log n) for balanced trees

### Union-Find

| File | Description | Learning Resources |
|------|-------------|-------------------|
| `unionfind.c` | Union-Find (Disjoint Set) | [VisuAlgo - UFDS](https://visualgo.net/en/ufds) • [Princeton Slides](https://www.cs.princeton.edu/~rs/AlgsDS07/01UnionFind.pdf) |

**Key Concepts:**
- Path compression
- Union by rank
- Applications in Kruskal's algorithm

---

## Graph Algorithms

### Traversal & Topological Sort

| File | Description | Learning Resources |
|------|-------------|-------------------|
| `bfs_dfs_topo.c` | BFS, DFS, and Topological Sort | [VisuAlgo - Graph Traversal](https://visualgo.net/en/dfsbfs) • [Topological Sort](https://www.geeksforgeeks.org/topological-sorting/) |

**Key Concepts:**
- Breadth-First Search (BFS)
- Depth-First Search (DFS)
- Topological ordering of DAGs

### Shortest Paths

| File | Description | Learning Resources |
|------|-------------|-------------------|
| `shortest_paths.c` | Dijkstra's and Bellman-Ford | [Dijkstra Visualization](https://www.cs.usfca.edu/~galles/visualization/Dijkstra.html) • [Bellman-Ford](https://visualgo.net/en/sssp) |

**Key Concepts:**
- Single-source shortest paths
- Negative weight edges handling
- Time complexity analysis

### Minimum Spanning Tree

| File | Description | Learning Resources |
|------|-------------|-------------------|
| `min_span_tree.c` | Kruskal's and Prim's algorithms | [MST Visualization](https://visualgo.net/en/mst) • [Princeton MST](https://algs4.cs.princeton.edu/43mst/) |

**Key Concepts:**
- Greedy approach
- Edge selection strategies
- Forest connectivity

### Network Flow

| File | Description | Learning Resources |
|------|-------------|-------------------|
| `max_flow.c` | Maximum Flow algorithms | [Max Flow Visualization](https://www.cs.usfca.edu/~galles/visualization/MaxFlow.html) • [MIT OCW](https://ocw.mit.edu/courses/6-046j-design-and-analysis-of-algorithms-spring-2015/resources/lecture-13-network-flows/) |

**Key Concepts:**
- Ford-Fulkerson method
- Residual graphs
- Min-cut max-flow theorem

---

## Dynamic Programming

| File | Description | Learning Resources |
|------|-------------|-------------------|
| `fib.c` | Fibonacci with memoization | [DP Introduction](https://www.geeksforgeeks.org/dynamic-programming/) |
| `knapsack.c` | 0/1 Knapsack problem | [Knapsack Visualization](https://algorithm-visualizer.org/dynamic-programming/knapsack-problem) |
| `lcs.c` | Longest Common Subsequence | [LCS Visualization](https://www.cs.usfca.edu/~galles/visualization/DPLCS.html) |
| `rod_cutting.c` | Rod Cutting problem | [MIT OCW - DP](https://ocw.mit.edu/courses/6-006-introduction-to-algorithms-fall-2011/resources/lecture-19-dynamic-programming-i-fibonacci-shortest-paths/) |

**Key Concepts:**
- Optimal substructure
- Overlapping subproblems
- Memoization vs Tabulation
- State transition equations

---

## Sorting Algorithms

| File | Description | Learning Resources |
|------|-------------|-------------------|
| `sorting.c` | Various sorting algorithms | [Sorting Viz](https://xuming.ai/demos/sorting.html) • [Toptal Viz](https://www.toptal.com/developers/sorting-algorithms) • [VisuAlgo](https://visualgo.net/en/sorting) |
| `rhs.c` | Radix/Heap sort implementations | [Radix Sort](https://www.cs.usfca.edu/~galles/visualization/RadixSort.html) • [Heap Sort](https://www.cs.usfca.edu/~galles/visualization/HeapSort.html) |

**Key Concepts:**
- Comparison-based sorting
- Linear-time sorting
- In-place vs stable sorting
- Time and space complexity

### Complexity Comparison

| Algorithm | Best | Average | Worst | Space | Stable |
|-----------|------|---------|-------|-------|--------|
| Quick Sort | O(n log n) | O(n log n) | O(n²) | O(log n) | No |
| Merge Sort | O(n log n) | O(n log n) | O(n log n) | O(n) | Yes |
| Heap Sort | O(n log n) | O(n log n) | O(n log n) | O(1) | No |
| Radix Sort | O(nk) | O(nk) | O(nk) | O(n+k) | Yes |

---

## Hash Tables

| File | Description | Learning Resources |
|------|-------------|-------------------|
| `linear_probing.c` | Linear probing hash table | [Hashing Viz](https://xuming.ai/demos/hashing.html) • [Hash Table Visualization](https://www.cs.usfca.edu/~galles/visualization/OpenHash.html) |
| `separate_chaining.c` | Separate chaining hash table | [Hashing Viz](https://xuming.ai/demos/hashing.html) • [Chaining Visualization](https://www.cs.usfca.edu/~galles/visualization/ClosedHash.html) |
| `hash_check.c` | Hash function testing | [Hashing Viz](https://xuming.ai/demos/hashing.html) • [Hash Functions](https://www.geeksforgeeks.org/hash-functions-and-list-types-of-hash-functions/) |

**Key Concepts:**
- Collision resolution strategies
- Load factor analysis
- Hash function design
- Amortized analysis

---

## Greedy Algorithms

| File | Description | Learning Resources |
|------|-------------|-------------------|
| `activity_selection.c` | Activity Selection problem | [Greedy Algorithms](https://www.geeksforgeeks.org/activity-selection-problem-greedy-algo-1/) • [MIT OCW](https://ocw.mit.edu/courses/6-046j-design-and-analysis-of-algorithms-spring-2015/resources/lecture-1-course-overview-interval-scheduling/) |

**Key Concepts:**
- Greedy choice property
- Proof of correctness
- Exchange argument

---

## Getting Started

### Prerequisites

- **C Compiler** (GCC or Clang)
- **Python 3.x** for Python implementations
- **Make** (optional) for building

### Compilation

For C files:

```bash
# Compile a single file
gcc -o program_name source_file.c

# Example
gcc -o avl avl.c
./avl

# With optimization
gcc -O2 -o program_name source_file.c
```

For Python files:

```bash
python filename.py
# Example
python decision_trees.py
```

### Using Skeleton Files

The `skeleton/` directory contains template files for practice:

1. Copy the skeleton file
2. Implement the missing functions
3. Compare with the complete implementation

```bash
# Example
cp skeleton/avl_skeleton.c my_avl.c
# Edit my_avl.c to complete the implementation
gcc -o my_avl my_avl.c
./my_avl
```

---

## File Organization

```
algorithms/
├── README.md                    # This file
├── README_zh.md                # Chinese version
├── data_structures/            # Data structure implementations
│   ├── trees/                 # Tree structures
│   ├── hash_tables/           # Hash tables
│   └── union_find/            # Union-Find
├── graph_algorithms/          # Graph algorithms
│   ├── traversal/            # BFS, DFS, Topological sort
│   ├── shortest_paths/       # Shortest path algorithms
│   ├── mst/                  # Minimum spanning tree
│   └── flow/                 # Network flow
├── dynamic_programming/       # DP problems
├── sorting/                  # Sorting algorithms
├── greedy/                   # Greedy algorithms
└── skeleton/                 # Template files for practice
```

---

## Additional Learning Resources

### Online Courses
- [Stanford CS161 (Official)](http://web.stanford.edu/class/cs161/)
- [MIT 6.006 Introduction to Algorithms](https://ocw.mit.edu/courses/6-006-introduction-to-algorithms-fall-2011/)
- [Coursera - Algorithms Specialization](https://www.coursera.org/specializations/algorithms)

### Textbooks
- **Introduction to Algorithms** (CLRS)
- **Algorithm Design** by Kleinberg & Tardos
- **The Algorithm Design Manual** by Skiena

### Practice Platforms
- [LeetCode](https://leetcode.com/)
- [HackerRank](https://www.hackerrank.com/domains/algorithms)
- [CodeForces](https://codeforces.com/)

---

## Contributing

Contributions are welcome! Please feel free to submit pull requests with:

- Bug fixes
- New algorithm implementations
- Documentation improvements
- Test cases

---

## License

This project is for educational purposes. Please refer to Stanford CS161 course policies for usage guidelines.

---

## Acknowledgments

- Stanford University CS161 course instructors
- **Matthew Sotoudeh** for CS161 reference materials ([https://masot.net/](https://masot.net/))

---

## Contact

For questions or suggestions about this repository, please open an issue on GitHub.