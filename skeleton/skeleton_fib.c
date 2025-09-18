#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

int fib(int n) {
    if (n <= 1) return 1;
    return fib(n-1) + fib(n-2);
}

int fib_memo(int n) {
    assert(!"unimplemented");
}

int fib_bottomup(int n) {
    assert(!"unimplemented");
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
