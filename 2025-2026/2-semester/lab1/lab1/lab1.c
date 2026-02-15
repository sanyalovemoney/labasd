#include <stdio.h>
#include <math.h>

double rec_descent(double x, int i, int n, double current_term, double current_sum) {
    if (i == n) {
        return current_sum;
    }
    double next_term = current_term * i * (x - 1.0) / (x * (i + 1.0));
    return rec_descent(x, i + 1, n, next_term, current_sum + next_term);
}

double sum_descent(double x, int n) {
    double start_term = (x - 1.0) / x;
    return rec_descent(x, 1, n, start_term, start_term);
}

typedef struct {
    double term;
    double sum;
} RecResult;

RecResult rec_return(double x, int i) {
    if (i == 1) {
        double start_term = (x - 1.0) / x;
        RecResult res = { start_term, start_term };
        return res;
    }

    RecResult prev = rec_return(x, i - 1);
    int prev_i = i - 1;

    double current_term = prev.term * prev_i * (x - 1.0) / (x * (prev_i + 1.0));
    double current_sum = prev.sum + current_term;

    RecResult current = { current_term, current_sum };
    return current;
}

double sum_return(double x, int n) {
    return rec_return(x, n).sum;
}

double rec_mixed(double x, int i, int n, double current_term) {
    if (i == n) {
        return current_term;
    }
    double next_term = current_term * i * (x - 1.0) / (x * (i + 1.0));
    return current_term + rec_mixed(x, i + 1, n, next_term);
}

double sum_mixed(double x, int n) {
    double start_term = (x - 1.0) / x;
    return rec_mixed(x, 1, n, start_term);
}

double sum_cyclic(double x, int n) {
    double term = (x - 1.0) / x;
    double sum = term;

    for (int i = 1; i < n; i++) {
        term = term * i * (x - 1.0) / (x * (i + 1.0));
        sum += term;
    }
    return sum;
}

int main() {
    int n_test = 5;
    double x_test = 2.0;

    printf("=== ALGORITHM TESTING (n = %d, x = %.2f) ===\n", n_test, x_test);

    double res_desc = sum_descent(x_test, n_test);
    double res_ret = sum_return(x_test, n_test);
    double res_mix = sum_mixed(x_test, n_test);
    double res_cyc = sum_cyclic(x_test, n_test);
    double res_exact = log(x_test);

    printf("1. Recursion (descent):       %.10f\n", res_desc);
    printf("2. Recursion (return):        %.10f\n", res_ret);
    printf("3. Recursion (mixed):         %.10f\n", res_mix);
    printf("4. Cyclic algorithm:          %.10f\n", res_cyc);
    printf("5. Reference math.h (ln x):   %.10f\n", res_exact);

    printf("\nError: |%.10f - %.10f| = %.10f\n", res_cyc, res_exact, fabs(res_cyc - res_exact));

    return 0;
}