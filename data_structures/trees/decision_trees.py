def dtree_for(algo, n, get_depth=False):
    """Make a decision tree for @algo. @algo must return the sorted array."""
    path2node = dict()
    path = tuple()
    path_idx = 0
    seen = set()
    worklist = [tuple()]
    def comp(which, lhs, rhs):
        nonlocal path_idx
        if path_idx >= len(path):
            path2node[path] = (which, lhs, rhs)
            worklist.append(path + (False,))
            worklist.append(path + (True,))
            raise StopIteration
        path_idx += 1
        return path[path_idx - 1]

    class Element:
        def __init__(self, i): self.i = i
        def __lt__(lhs, rhs): return comp( "<", lhs, rhs)
        def __le__(lhs, rhs): return comp("<=", lhs, rhs)
        def __eq__(lhs, rhs): return comp( "=", lhs, rhs)
        def __gt__(lhs, rhs): return comp( ">", lhs, rhs)
        def __ge__(lhs, rhs): return comp(">=", lhs, rhs)

    while worklist:
        path = worklist.pop()
        if path in seen: continue
        path_idx = 0
        seen.add(path)
        sym_input = [Element(i) for i in range(n)]
        try:                    path2node[path] = algo(sym_input)
        except StopIteration:   continue

    def print_tree(path=tuple(), depth=0):
        print("| " * depth, end="")
        if path and path[-1]:       print("Y: ", end="")
        elif path and not path[-1]: print("N: ", end="")

        if isinstance(path2node[path], list):
            print("Return:", [e.i for e in path2node[path]], f"({depth})")
        else:
            comp, i, j = path2node[path]
            print(f"Is A[{i.i}] {comp} A[{j.i}]?")
            print_tree(path + (True,),  depth + 1)
            print_tree(path + (False,), depth + 1)

    if not get_depth:
        print_tree()
    else:
        longest = max(path2node.keys(), key=len)
        print(len(longest))

def insertion_sort(A):
    for i in range(len(A)):
        while i > 0 and A[i] < A[i-1]:
            A[i], A[i-1] = A[i-1], A[i]
            i = i - 1
    return A

def selection_sort(A):
    for i in range(len(A)):
        m = i
        for j in range(i+1, len(A)):
            if A[m] > A[j]:
                m = j
        A[i], A[m] = A[m], A[i]
    return A

def merge_sort(A):
    if len(A) <= 1: return A
    mid = len(A) // 2
    left, right = merge_sort(A[:mid]), merge_sort(A[mid:])
    new = []
    while left and right:
        if left[0] <= right[0]: new.append(left.pop(0))
        else:                   new.append(right.pop(0))
    while left:     new.append(left.pop(0))
    while right:    new.append(right.pop(0))
    return new

# Change this to try different input list lengths
N = 3
# Change this to True if you just want to know the depth of the tree
just_depth = False

# print("Insertion sort:")
# dtree_for(insertion_sort, N, just_depth)
# print("Selection sort:")
# dtree_for(selection_sort, N, just_depth)
print("Merge sort:")
dtree_for(merge_sort, N, just_depth)
# print("Python sort:")
# dtree_for(sorted, N, just_depth)
