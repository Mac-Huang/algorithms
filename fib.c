#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

int fib(int n) {
    if (n <= 1) return 1;
    return fib(n-1) + fib(n-2);
}

int fib_memo(int n) {
    if (n <= 1) return 1;

    static int *memo = 0;
    static int memo_size = 0, n_memo = 0;
    if (memo_size <= n) {
        memo_size = 2 * n;
        memo = realloc(memo, memo_size * sizeof(int));
        if (n_memo == 0) {
            n_memo = 2;
            memo[0] = memo[1] = 1;
        }
    }

    if (n < n_memo) return memo[n];

    int result = fib_memo(n-1) + fib_memo(n-2);
    assert(n_memo == n);
    memo[n_memo++] = result;
    return result;
}

int fib_bottomup(int n) {
    int *memo = calloc(n+1, sizeof(int));
    for (int i = 0; i <= n; i++) {
        if (i <= 1) memo[i] = 1;
        else        memo[i] = memo[i-1] + memo[i-2];
    }
    int result = memo[n];
    free(memo);
    return result;
}

int fib_bottomup_goodspace(int n) {
    if (n <= 1) return 0;
    int i_m_1 = 1, i_m_2 = 1;
    for (int i = 2; i < n; i++) {
        int new_i = i_m_1 + i_m_2;
        i_m_2 = i_m_1;
        i_m_1 = new_i;
    }
    return i_m_1 + i_m_2;
}

// https://en.wikipedia.org/wiki/Fibonacci_sequence#Relation_to_the_golden_ratio
// that formula is off-by-one from what we call n; it starts at 1
int fib_o1(int n_) {
    double sqrt5 = pow(5., 0.5),
           phi = (1. + sqrt5) / 2.,
           psi = (1. - sqrt5) / 2.,
           n   = (double)n_ + 1.;
    return (int)((pow(phi, n) - pow(psi, n)) / sqrt5);
}

int main() {
    printf("For small inputs they all work fine ...\n");
    printf("fib(10) = %d\n", fib(10));
    printf("fib_memo(10) = %d\n", fib_memo(10));
    printf("fib_bottomup(10) = %d\n", fib_bottomup(10));
    printf("fib_bottomup_goodspace(10) = %d\n", fib_bottomup_goodspace(10));
    printf("fib_o1(10) = %d\n", fib_o1(10));

    printf("But for large inputs the naive version is much slower!\n");
    printf("fib(43) = %d\n", fib(43));
    printf("fib_memo(43) = %d\n", fib_memo(43));
    printf("fib_bottomup(43) = %d\n", fib_bottomup(43));
    printf("fib_bottomup_goodspace(43) = %d\n", fib_bottomup_goodspace(43));
    printf("fib_o1(43) = %d\n", fib_o1(43));
}
