# Greedy Algorithms / 贪心算法

## Contents / 内容

### Activity Selection Problem / 活动选择问题
- **activity_selection.c** - Selecting maximum non-overlapping activities / 选择最大非重叠活动集

## Greedy Algorithm Properties / 贪心算法特性

1. **Greedy Choice Property** / **贪心选择性质**
   - Local optimal choices lead to global optimum
   - 局部最优选择导致全局最优

2. **Optimal Substructure** / **最优子结构**
   - Optimal solution contains optimal solutions to subproblems
   - 最优解包含子问题的最优解

## Common Applications / 常见应用
- Huffman Coding / 霍夫曼编码
- Dijkstra's Shortest Path / Dijkstra最短路径
- Kruskal's/Prim's MST / Kruskal/Prim最小生成树
- Job Scheduling / 作业调度

## When to Use / 何时使用
Greedy algorithms work when the problem exhibits the greedy choice property. Always verify correctness with proof!
当问题展现贪心选择性质时，贪心算法有效。始终通过证明验证正确性！