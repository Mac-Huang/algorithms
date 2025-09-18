# Sorting Algorithms / 排序算法

## Contents / 内容

### Implementations / 实现

- **sorting.c** - Collection of comparison-based sorting algorithms / 基于比较的排序算法集合
  - Quick Sort / 快速排序
  - Merge Sort / 归并排序
  - Heap Sort / 堆排序
  - Insertion Sort / 插入排序
  - Selection Sort / 选择排序

- **rhs.c** - Radix/Heap sort implementations / 基数/堆排序实现
  - Radix Sort (linear time for integers) / 基数排序（整数线性时间）
  - Optimized Heap Sort / 优化的堆排序

## Comparison / 比较

| Algorithm / 算法 | Best / 最优 | Average / 平均 | Worst / 最差 | Space / 空间 | Stable / 稳定 |
|-----------------|------------|---------------|-------------|-------------|--------------|
| Quick Sort      | O(n log n) | O(n log n)    | O(n²)       | O(log n)    | No / 否      |
| Merge Sort      | O(n log n) | O(n log n)    | O(n log n)  | O(n)        | Yes / 是     |
| Heap Sort       | O(n log n) | O(n log n)    | O(n log n)  | O(1)        | No / 否      |
| Radix Sort      | O(nk)      | O(nk)         | O(nk)       | O(n+k)      | Yes / 是     |

Where k is the number of digits/characters.
其中k是数字/字符的位数。