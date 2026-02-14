#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <stdio.h>
#include <math.h>

double sum_on_descent(int current, int n, double x, double f_current) {
    if (current > n) {
        return 0.0;
    }

    double sum_rest = sum_on_descent(current + 1, n, x,
        f_current * current * (x - 1.0) / (current * x + x));
    return f_current + sum_rest;
}

double calculate_sum_method1(int n, double x) {
    if (n < 1 || x <= 0.5) {
        return 0.0;
    }

    double f1 = (x - 1.0) / x;
    return sum_on_descent(1, n, x, f1);
}

typedef struct {
    double sum;
    double f_next;
} Result;

Result sum_on_return(int current, int n, double x, double f_current) {
    Result res;

    if (current > n) {
        res.sum = 0.0;
        res.f_next = f_current;
        return res;
    }

    double f_next = f_current * current * (x - 1.0) / (current * x + x);
    Result rest = sum_on_return(current + 1, n, x, f_next);

    res.sum = f_current + rest.sum;
    res.f_next = rest.f_next;

    return res;
}

double calculate_sum_method2(int n, double x) {
    if (n < 1 || x <= 0.5) {
        return 0.0;
    }

    double f1 = (x - 1.0) / x;
    Result result = sum_on_return(1, n, x, f1);

    return result.sum;
}

double sum_mixed(int current, int n, double x, double f_current) {
    if (current > n) {
        return 0.0;
    }

    double f_next = f_current * current * (x - 1.0) / (current * x + x);
    double sum_rest = sum_mixed(current + 1, n, x, f_next);
    return f_current + sum_rest;
}

double calculate_sum_method3(int n, double x) {
    if (n < 1 || x <= 0.5) {
        return 0.0;
    }

    double f1 = (x - 1.0) / x;
    return sum_mixed(1, n, x, f1);
}

int main() {
    int n, method;
    double x;

    printf("Enter n (positive integer): ");
    if (scanf("%d", &n) != 1 || n < 1) {
        printf("Error: n must be a positive integer!\n");
        return 1;
    }

    printf("Enter x (x > 0.5): ");
    if (scanf("%lf", &x) != 1 || x <= 0.5) {
        printf("Error: x must be greater than 0.5!\n");
        return 1;
    }

    printf("Choose method (1-Descent, 2-Return, 3-Mixed, 4-Compare all): ");
    if (scanf("%d", &method) != 1 || method < 1 || method > 4) {
        printf("Error: invalid method!\n");
        return 1;
    }

    printf("\n");

    if (method == 4) {
        double result1 = calculate_sum_method1(n, x);
        double result2 = calculate_sum_method2(n, x);
        double result3 = calculate_sum_method3(n, x);
        double ln_x = log(x);

        printf("Method 1: %.10lf  (error: %.2e)\n", result1, result1 - ln_x);
        printf("Method 2: %.10lf  (error: %.2e)\n", result2, result2 - ln_x);
        printf("Method 3: %.10lf  (error: %.2e)\n", result3, result3 - ln_x);
        printf("ln(%.2lf): %.10lf\n", x, ln_x);

        if (result1 == result2 && result2 == result3) {
            printf("\n✓ All methods give identical results\n");
        }
    }
    else {
        double result;
        const char* method_name;

        switch (method) {
        case 1:
            result = calculate_sum_method1(n, x);
            method_name = "Method 1 (Descent)";
            break;
        case 2:
            result = calculate_sum_method2(n, x);
            method_name = "Method 2 (Return)";
            break;
        case 3:
            result = calculate_sum_method3(n, x);
            method_name = "Method 3 (Mixed)";
            break;
        default:
            return 1;
        }

        printf("%s\n", method_name);
        printf("Sum of first %d terms: %.10lf\n", n, result);
        printf("ln(%.2lf) = %.10lf\n", x, log(x));
        printf("Error: %.2e\n", result - log(x));
    }

    return 0;
}
