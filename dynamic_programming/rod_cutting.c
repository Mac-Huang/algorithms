#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

// does *not* give the optimal
int best_cut_price_greedy(int rod_length, int *prices) {
    if (rod_length == 0) return 0;

    int best_first_cut = rod_length;
    for (int first_cut = 1; first_cut < rod_length; first_cut++)
        if (prices[first_cut] > prices[best_first_cut])
            best_first_cut = first_cut;

    return prices[best_first_cut]
        + best_cut_price_greedy(rod_length - best_first_cut, prices);
}

// *does* work but is slow
int best_cut_price_recursive(int rod_length, int *prices) {
    int best = prices[rod_length];
    for (int first_cut = 1; first_cut < rod_length; first_cut++) {
        int alt = prices[first_cut] + best_cut_price_recursive(rod_length - first_cut, prices);
        if (alt > best) best = alt;
    }
    return best;
}

// *does* work and is fast; bottom-up to make it easier to implement
int best_cut_price_memoized_bu(int rod_length, int *prices) {
    int *memo = calloc(rod_length + 1, sizeof(*memo));
    for (int l = 0; l <= rod_length; l++) {
        int best = prices[l];
        for (int first_cut = 1; first_cut < l; first_cut++) {
            int alt = prices[first_cut] + memo[l - first_cut];
            if (alt > best) best = alt;
        }
        memo[l] = best;
    }
    return memo[rod_length]; // (note: memory leak ignored)
}

int main() {
    int rod_length = 40,
        prices[] = {0, 6, 5, 24, 3, 4, 6, 10, 10, 15, 26, 6, 5, 24, 3, 4, 6,
                    10, 10, 15, 26, 0, 6, 5, 24, 3, 4, 6, 10, 10, 15, 26, 6, 5,
                    24, 3, 4, 6, 10, 10, 15, 26};
    printf("Greedy: %d\n", best_cut_price_greedy(rod_length, prices));
    printf("Bottom-up version of memoized recursive (should be better than the greedy verision): %d\n", best_cut_price_memoized_bu(rod_length, prices));
    printf("About to run recursive version; will likely take a very long time!\n");
    printf("Recursive: %d\n", best_cut_price_recursive(rod_length, prices));
    return 0;
}
