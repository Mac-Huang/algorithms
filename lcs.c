#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

// *does* work but is slow
char *lcs_recursive(char *s, char *t) {
    if (!*s || !*t) return strdup("");
    if (s[0] == t[0]) {
        char *among_rest = lcs_recursive(s + 1, t + 1);
        char *new = calloc(strlen(among_rest) + 2, sizeof(char));
        new[0] = s[0];
        strcpy(new + 1, among_rest);
        free(among_rest);
        return new;
    }
    char *a = lcs_recursive(s + 1, t), *b = lcs_recursive(s, t + 1);
    if (strlen(a) >= strlen(b)) {
        free(b);
        return a;
    }
    free(a);
    return b;
}

// *does* work and is fast; top-down version.
char *lcs_memo_(char *s, char *t, char *s_start, char *t_start, char ***memo) {
    char *result = memo[s - s_start][t - t_start];
check_result:
    if (result) { memo[s - s_start][t - t_start] = result; return result; }

    if (!*s || !*t) { result = strdup(""); goto check_result; }
    if (s[0] == t[0]) {
        char *among_rest = lcs_memo_(s + 1, t + 1, s_start, t_start, memo);
        result = calloc(strlen(among_rest) + 2, sizeof(char));
        result[0] = s[0];
        strcpy(result + 1, among_rest);
        goto check_result;
    }
    char *a = lcs_memo_(s + 1, t, s_start, t_start, memo),
         *b = lcs_memo_(s, t + 1, s_start, t_start, memo);
    if (strlen(a) >= strlen(b)) {
        result = a;
        goto check_result;
    }
    result = b;
    goto check_result;
}

char *lcs_memo(char *s, char *t) {
    int n_s = strlen(s), n_t = strlen(t);
    // memo[i][j] corresponds to the suffix s[i:] and t[j:]
    char ***memo = calloc(n_s + 1, sizeof(char**));
    for (int i = 0; i <= n_s; i++) memo[i] = calloc(n_t + 1, sizeof(char*));
    // (note: ignoring memory leak of memo)
    return lcs_memo_(s, t, s, t, memo);
}

// does *not* always give the LCS, surprisingly!
char *lcs_greedy(char *s, char *t) {
    int n_s = strlen(s), n_t = strlen(t), n = (n_s < n_t) ? n_s : n_t;
    char **s_pos = calloc(256, sizeof(char *)),
         **t_pos = calloc(256, sizeof(char *));
    for (int i = 0; i < n; i++) {
        if (s_pos[t[i]]) {
            char *among_rest = lcs_greedy(s_pos[t[i]] + 1, t + i + 1);
            char *new = calloc(strlen(among_rest) + 2, sizeof(char));
            new[0] = t[i];
            strcpy(new + 1, among_rest);
            free(among_rest);
            return new;
        } else if (t_pos[s[i]]) {
            char *among_rest = lcs_greedy(s + i + 1, t_pos[s[i]] + 1);
            char *new = calloc(strlen(among_rest) + 2, sizeof(char));
            new[0] = s[i];
            strcpy(new + 1, among_rest);
            free(among_rest);
            return new;
        }
        if (!s_pos[s[i]]) s_pos[s[i]] = s + i;
        if (!t_pos[t[i]]) t_pos[t[i]] = t + i;
    }
    return strdup("");
}

int main() {
    char *s = "alphabetical",
         *t = "alphabetized already";
    printf("LCS_recursive('%s', '%s') = '%s'\n", s, t, lcs_recursive(s, t));
    printf("LCS_memo('%s', '%s') = '%s'\n", s, t, lcs_memo(s, t));
    printf("LCS_greedy('%s', '%s') = '%s'\n", s, t, lcs_greedy(s, t));

    s = "xbax";
    t = "ax";
    printf("LCS_recursive('%s', '%s') = '%s'\n", s, t, lcs_recursive(s, t));
    printf("LCS_memo('%s', '%s') = '%s'\n", s, t, lcs_memo(s, t));
    printf("LCS_greedy('%s', '%s') = '%s'\n", s, t, lcs_greedy(s, t));

    s = "abcabcabcabcabcabcabcabc";
    t = "xyzxyzxyzxyzxyzxyzxyzxyz";
    printf("LCS_memo('%s', '%s') = '%s'\n", s, t, lcs_memo(s, t));
    printf("LCS_greedy('%s', '%s') = '%s'\n", s, t, lcs_greedy(s, t));
    printf("About to run LCS_recursive('%s', '%s'); this may take a very long time!\n", s, t);
    printf("LCS_recursive('%s', '%s') = '%s'\n", s, t, lcs_recursive(s, t));
    return 0;
}
