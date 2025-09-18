# CS161 算法教程

> 斯坦福CS161算法课程的综合实现集合
>
> [English Version](README.md)

## 作者

**Mac Huang** - 斯坦福CS161课程的算法实现和教育材料

---

## 目录

- [简介](#简介)
- [数据结构](#数据结构)
- [图算法](#图算法)
- [动态规划](#动态规划)
- [排序算法](#排序算法)
- [哈希表](#哈希表)
- [贪心算法](#贪心算法)
- [快速开始](#快速开始)
- [贡献](#贡献)

---

## 简介

本仓库包含斯坦福CS161课程中涵盖的基础算法和数据结构的实现。每个实现都包括完整代码和用于学习目的的骨架模板。

### 特性

- 📚 **全面覆盖** - 涵盖算法和数据结构的主要主题
- 🎯 **教育导向** - 包含供练习的骨架文件
- 💻 **多语言支持** - 使用C和Python实现
- 🔗 **学习资源** - 在线教程和可视化链接

---

## 数据结构

### 树

| 文件 | 描述 | 学习资源 |
|------|------|----------|
| `avl.c` | AVL树实现 | [BS树可视化](https://xuming.ai/demos/bs-tree.html) • [VisuAlgo - AVL树](https://visualgo.net/zh/bst) • [极客教程](https://www.geeksforgeeks.org/avl-tree-set-1-insertion/) |
| `optimized_avl.c` | 优化的AVL树 | [CS161课程笔记](https://web.stanford.edu/class/cs161/) |
| `23trees.c` | 2-3树实现 | [B+树可视化](https://xuming.ai/demos/bplus-tree.html) • [2-3树可视化](https://www.cs.usfca.edu/~galles/visualization/BTree.html) |
| `splay.c` | 伸展树实现 | [伸展树可视化](https://www.cs.usfca.edu/~galles/visualization/SplayTree.html) |
| `bst_insert.c` | 二叉搜索树插入 | [BS树可视化](https://xuming.ai/demos/bs-tree.html) • [BST可视化](https://www.cs.usfca.edu/~galles/visualization/BST.html) |
| `decision_trees.py` | 决策树实现 | [可视化介绍](http://www.r2d3.us/visual-intro-to-machine-learning-part-1/) |

**核心概念：**
- 自平衡树
- 树旋转
- 搜索、插入、删除操作
- 时间复杂度：平衡树为O(log n)

### 并查集

| 文件 | 描述 | 学习资源 |
|------|------|----------|
| `unionfind.c` | 并查集（不相交集） | [VisuAlgo - 并查集](https://visualgo.net/zh/ufds) • [普林斯顿幻灯片](https://www.cs.princeton.edu/~rs/AlgsDS07/01UnionFind.pdf) |

**核心概念：**
- 路径压缩
- 按秩合并
- 在Kruskal算法中的应用

---

## 图算法

### 遍历与拓扑排序

| 文件 | 描述 | 学习资源 |
|------|------|----------|
| `bfs_dfs_topo.c` | 广度优先、深度优先搜索和拓扑排序 | [VisuAlgo - 图遍历](https://visualgo.net/zh/dfsbfs) • [拓扑排序](https://www.geeksforgeeks.org/topological-sorting/) |

**核心概念：**
- 广度优先搜索 (BFS)
- 深度优先搜索 (DFS)
- 有向无环图的拓扑排序

### 最短路径

| 文件 | 描述 | 学习资源 |
|------|------|----------|
| `shortest_paths.c` | Dijkstra和Bellman-Ford算法 | [Dijkstra可视化](https://www.cs.usfca.edu/~galles/visualization/Dijkstra.html) • [Bellman-Ford](https://visualgo.net/zh/sssp) |

**核心概念：**
- 单源最短路径
- 负权边处理
- 时间复杂度分析

### 最小生成树

| 文件 | 描述 | 学习资源 |
|------|------|----------|
| `min_span_tree.c` | Kruskal和Prim算法 | [MST可视化](https://visualgo.net/zh/mst) • [普林斯顿MST](https://algs4.cs.princeton.edu/43mst/) |

**核心概念：**
- 贪心方法
- 边选择策略
- 森林连通性

### 网络流

| 文件 | 描述 | 学习资源 |
|------|------|----------|
| `max_flow.c` | 最大流算法 | [最大流可视化](https://www.cs.usfca.edu/~galles/visualization/MaxFlow.html) • [MIT公开课](https://ocw.mit.edu/courses/6-046j-design-and-analysis-of-algorithms-spring-2015/resources/lecture-13-network-flows/) |

**核心概念：**
- Ford-Fulkerson方法
- 残余图
- 最小割最大流定理

---

## 动态规划

| 文件 | 描述 | 学习资源 |
|------|------|----------|
| `fib.c` | 带记忆化的斐波那契 | [动态规划介绍](https://www.geeksforgeeks.org/dynamic-programming/) |
| `knapsack.c` | 0/1背包问题 | [背包问题可视化](https://algorithm-visualizer.org/dynamic-programming/knapsack-problem) |
| `lcs.c` | 最长公共子序列 | [LCS可视化](https://www.cs.usfca.edu/~galles/visualization/DPLCS.html) |
| `rod_cutting.c` | 钢条切割问题 | [MIT公开课 - DP](https://ocw.mit.edu/courses/6-006-introduction-to-algorithms-fall-2011/resources/lecture-19-dynamic-programming-i-fibonacci-shortest-paths/) |

**核心概念：**
- 最优子结构
- 重叠子问题
- 记忆化vs制表法
- 状态转移方程

---

## 排序算法

| 文件 | 描述 | 学习资源 |
|------|------|----------|
| `sorting.c` | 各种排序算法 | [排序可视化](https://xuming.ai/demos/sorting.html) • [Toptal可视化](https://www.toptal.com/developers/sorting-algorithms) • [VisuAlgo](https://visualgo.net/zh/sorting) |
| `rhs.c` | 基数/堆排序实现 | [基数排序](https://www.cs.usfca.edu/~galles/visualization/RadixSort.html) • [堆排序](https://www.cs.usfca.edu/~galles/visualization/HeapSort.html) |

**核心概念：**
- 基于比较的排序
- 线性时间排序
- 原地排序vs稳定排序
- 时间和空间复杂度

### 复杂度比较

| 算法 | 最优 | 平均 | 最差 | 空间 | 稳定 |
|------|------|------|------|------|------|
| 快速排序 | O(n log n) | O(n log n) | O(n²) | O(log n) | 否 |
| 归并排序 | O(n log n) | O(n log n) | O(n log n) | O(n) | 是 |
| 堆排序 | O(n log n) | O(n log n) | O(n log n) | O(1) | 否 |
| 基数排序 | O(nk) | O(nk) | O(nk) | O(n+k) | 是 |

---

## 哈希表

| 文件 | 描述 | 学习资源 |
|------|------|----------|
| `linear_probing.c` | 线性探测哈希表 | [哈希可视化](https://xuming.ai/demos/hashing.html) • [哈希表可视化](https://www.cs.usfca.edu/~galles/visualization/OpenHash.html) |
| `separate_chaining.c` | 链地址法哈希表 | [哈希可视化](https://xuming.ai/demos/hashing.html) • [链地址法可视化](https://www.cs.usfca.edu/~galles/visualization/ClosedHash.html) |
| `hash_check.c` | 哈希函数测试 | [哈希可视化](https://xuming.ai/demos/hashing.html) • [哈希函数](https://www.geeksforgeeks.org/hash-functions-and-list-types-of-hash-functions/) |

**核心概念：**
- 冲突解决策略
- 负载因子分析
- 哈希函数设计
- 摊还分析

---

## 贪心算法

| 文件 | 描述 | 学习资源 |
|------|------|----------|
| `activity_selection.c` | 活动选择问题 | [贪心算法](https://www.geeksforgeeks.org/activity-selection-problem-greedy-algo-1/) • [MIT公开课](https://ocw.mit.edu/courses/6-046j-design-and-analysis-of-algorithms-spring-2015/resources/lecture-1-course-overview-interval-scheduling/) |

**核心概念：**
- 贪心选择性质
- 正确性证明
- 交换论证

---

## 快速开始

### 前置条件

- **C编译器**（GCC或Clang）
- **Python 3.x** 用于Python实现
- **Make**（可选）用于构建

### 编译

对于C文件：

```bash
# 编译单个文件
gcc -o 程序名 源文件.c

# 例子
gcc -o avl avl.c
./avl

# 带优化
gcc -O2 -o 程序名 源文件.c
```

对于Python文件：

```bash
python 文件名.py
# 例子
python decision_trees.py
```

### 使用骨架文件

`skeleton/`目录包含用于练习的模板文件：

1. 复制骨架文件
2. 实现缺失的函数
3. 与完整实现进行比较

```bash
# 例子
cp skeleton/avl_skeleton.c my_avl.c
# 编辑my_avl.c完成实现
gcc -o my_avl my_avl.c
./my_avl
```

---

## 文件组织

```
algorithms/
├── README.md                    # 英文版
├── README_zh.md                # 本文件
├── data_structures/            # 数据结构实现
│   ├── trees/                 # 树结构
│   ├── hash_tables/           # 哈希表
│   └── union_find/            # 并查集
├── graph_algorithms/          # 图算法
│   ├── traversal/            # BFS, DFS, 拓扑排序
│   ├── shortest_paths/       # 最短路径算法
│   ├── mst/                  # 最小生成树
│   └── flow/                 # 网络流
├── dynamic_programming/       # 动态规划
├── sorting/                  # 排序算法
├── greedy/                   # 贪心算法
└── skeleton/                 # 练习模板
```

---

## 额外学习资源

### 在线课程
- [斯坦福CS161（官方）](http://web.stanford.edu/class/cs161/)
- [MIT 6.006 算法导论](https://ocw.mit.edu/courses/6-006-introduction-to-algorithms-fall-2011/)
- [Coursera - 算法专项课程](https://www.coursera.org/specializations/algorithms)
- [中国大学MOOC - 数据结构](https://www.icourse163.org/course/ZJU-93001)

### 教材推荐
- **算法导论** (CLRS)
- **算法设计** - Kleinberg & Tardos
- **算法设计手册** - Skiena
- **算法（第4版）** - Sedgewick（有中文版）

### 练习平台
- [LeetCode中文站](https://leetcode-cn.com/)
- [牛客网](https://www.nowcoder.com/)
- [洛谷](https://www.luogu.com.cn/)
- [HackerRank](https://www.hackerrank.com/domains/algorithms)
- [CodeForces](https://codeforces.com/)

### 中文学习资源
- [OI Wiki](https://oi-wiki.org/)
- [算法可视化](https://algorithm-visualizer.org/)
- [力扣算法图解](https://leetcode-cn.com/leetbook/)

---

## 贡献

欢迎贡献！请随时提交包含以下内容的拉取请求：

- 错误修复
- 新算法实现
- 文档改进
- 测试用例

---

## 许可证

该项目用于教育目的。请参考斯坦福CS161课程政策以了解使用指南。

---

## 联系方式

对于关于本仓库的问题或建议，请在GitHub上开启issue。
