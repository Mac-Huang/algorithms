import sys
from collections import defaultdict
import copy

class UnionFind:
    def __init__(self, join_by_weight):
        self.parent_of = dict()
        self.weight_of = defaultdict(lambda: 1)
        self.join_by_weight = join_by_weight
        self.cost = 0

    def depth(self):
        def _depth(node):
            if node not in self.parent_of:
                return 1
            return 1 + _depth(self.parent_of[node])
        return max(map(_depth, range(1, 6)))

    def copy(self): return copy.deepcopy(self)

    def find_root(self, A):
        self.cost += 1
        if A not in self.parent_of:
            return A
        self.parent_of[A] = self.find_root(self.parent_of[A])
        return self.parent_of[A]

    def union(self, A, B):
        A = self.find_root(A)
        B = self.find_root(B)
        if A == B:
            return
        if self.join_by_weight and self.weight_of[B] < self.weight_of[A]:
            A, B = B, A
        self.parent_of[A] = B
        self.weight_of[B] += self.weight_of[A]
        return B

    def same_set(self, A, B):
        return self.find_root(A) == self.find_root(B)

    def children_of(self, A):
        return [x for x in self.parent_of if self.parent_of[x] == A]

    def print_forest(self):
        roots = [x for x in range(1, 6) if x not in self.parent_of]
        def print_tree(x, depth=0):
            print(("| " * depth) + str(x))
            for child in self.children_of(x):
                print_tree(child, depth + 1)
        for root in roots:
            print_tree(root)

descr = sys.argv[1]
version_A = UnionFind(True)
version_B = UnionFind(False)

deepest_A, deepest_B = version_A.copy(), version_B.copy()

assert len(descr) % 2 == 0
for A, B in zip(map(int, descr[::2]), map(int, descr[1::2])):
    print(f"Union({A}, {B})")
    version_A.union(A, B)
    version_B.union(A, B)
    version_B.print_forest()
    if version_A.depth() >= deepest_A.depth():
        deepest_A = version_A.copy()
    if version_B.depth() >= deepest_B.depth():
        deepest_B = version_B.copy()

print("DEPEST:")
print("Version B:")
deepest_B.print_forest()
print("Version A:")
deepest_A.print_forest()

print("---------------------------------------------")
version_B.cost = 0
version_B.same_set(1, 5)
COST = version_B.cost
print("Same-Set Cost (max is 7):", COST)

print("---------------------------------------------")
version_A.same_set(1, 5)

version_B.print_forest()
version_A.print_forest()
