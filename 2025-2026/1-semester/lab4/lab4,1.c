#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

int main()
{
    int m = 0, n = 0;

    printf("Введіть кількість рядків (m) та стовпців (n) (можна через кому або пробіл): \n");
    char buf[128];
    if (!fgets(buf, sizeof(buf), stdin)) {
        fprintf(stderr, "Помилка вводу.\n");
        return 1;
    }

	// Розбір введених розмірів матриці
    char* p = buf;
    m = (int)strtol(p, &p, 10);
    while (*p && !isdigit((unsigned char)*p) && *p != '-') p++;
    n = (int)strtol(p, NULL, 10);

    if (m <= 0 || n <= 0) {
        fprintf(stderr, "Неприпустимі розміри матриці (m і n мають бути додатні).\n");
        return 1;
    }

    double* matrix = malloc((size_t)m * n * sizeof(double));
    if (!matrix) {
        perror("malloc");
        return 1;
    }

    printf("Введіть елементи матриці (елементи в кожному стовпчику впорядковані за незменшенням):\n");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (scanf("%lf", &matrix[i * n + j]) != 1) {
                fprintf(stderr, "Некоректне значення матриці.\n");
                free(matrix);
                return 1;
            }
        }
    }

    printf("Ваша матриця:\n");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%.3lf\t", matrix[i * n + j]);
        }
        printf("\n");
    }

    double x;
    printf("Введіть число X, яке потрібно знайти в кожному стовпчику:\n");
    if (scanf("%lf", &x) != 1) {
        fprintf(stderr, "Некоректне значення X.\n");
        free(matrix);
        return 1;
    }

    const double EPS = 1e-9;
    int found_any = 0;

    // Шукаємо x у кожному стовпчику окремо (бінарний пошук по кожному стовпчику)
    for (int j = 0; j < n; j++)
    {
        int L = 0, R = m - 1;
        int found = 0;

        while (L <= R)
        {
            int mid = L + (R - L) / 2;
            double val = matrix[mid * n + j];

            if (fabs(val - x) <= EPS)
            {
                printf("Значення X знайдено у стовпчику %d на позиції (%d, %d)\n", j, mid, j);
                found = 1;
                found_any = 1;
                break;
            }
            else if (val < x)
            {
                L = mid + 1;
            }
            else
            {
                R = mid - 1;
            }
        }

        if (!found)
        {
            printf("Значення X не знайдено у стовпчику %d\n", j);
        }
    }

    if (!found_any)
    {
        printf("Значення X не знайдено в жодному стовпчику матриці.\n");
    }

    free(matrix);
    return 0;
}