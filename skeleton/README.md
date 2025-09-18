# Skeleton Files / 骨架文件

## Purpose / 目的

These skeleton files are templates for learning and practicing algorithm implementations. They provide the structure with key functions left for you to implement.

这些骨架文件是用于学习和练习算法实现的模板。它们提供了结构，关键函数留给您实现。

## Contents / 内容

| File / 文件 | Algorithm / 算法 | Key Functions to Implement / 要实现的关键函数 |
|------------|-----------------|-------------------------------------------|
| `avl_skeleton.c` | AVL Tree / AVL树 | rotate, insert, rebalance |
| `23tree_skeleton.c` | 2-3 Tree / 2-3树 | split, insert, search |
| `splay_skeleton.c` | Splay Tree / 伸展树 | splay, insert, find |
| `unionfind_skeleton.c` | Union-Find / 并查集 | find, union, path compression |
| `skeleton_bfs_dfs_topo.c` | Graph Traversal / 图遍历 | BFS, DFS, topological sort |
| `skeleton_shortest_paths.c` | Shortest Paths / 最短路径 | Dijkstra, Bellman-Ford |
| `skeleton_min_span_tree.c` | MST Algorithms / MST算法 | Kruskal, Prim |
| `skeleton_fib.c` | Fibonacci DP / 斐波那契DP | memoization, tabulation |
| `skeleton_activity_selection.c` | Activity Selection / 活动选择 | greedy selection |

## How to Use / 如何使用

1. Copy the skeleton file to your workspace / 将骨架文件复制到您的工作区
2. Implement the missing functions / 实现缺失的函数
3. Test your implementation / 测试您的实现
4. Compare with the complete version / 与完整版本比较

```bash
# Example / 例子
cp skeleton/avl_skeleton.c my_avl.c
# Implement the functions / 实现函数
gcc -o my_avl my_avl.c
./my_avl
```

## Tips / 提示
- Start with simple test cases / 从简单的测试用例开始
- Draw diagrams for complex operations / 为复杂操作绘制图表
- Use debugger or print statements / 使用调试器或打印语句
- Verify edge cases / 验证边界情况