#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

int main()
{
    int m = 0, n = 0;

    printf("������ ������� ����� (m) �� �������� (n) (����� ����� ���� ��� �����): \n");
    char buf[128];
    if (!fgets(buf, sizeof(buf), stdin)) {
        fprintf(stderr, "������� �����.\n");
        return 1;
    }

	// ����� �������� ������ �������
    char* p = buf;
    m = (int)strtol(p, &p, 10);
    while (*p && !isdigit((unsigned char)*p) && *p != '-') p++;
    n = (int)strtol(p, NULL, 10);

    if (m <= 0 || n <= 0) {
        fprintf(stderr, "����������� ������ ������� (m � n ����� ���� ������).\n");
        return 1;
    }

    double* matrix = malloc((size_t)m * n * sizeof(double));
    if (!matrix) {
        perror("malloc");
        return 1;
    }

    printf("������ �������� ������� (�������� � ������� ��������� ����������� �� ������������):\n");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (scanf("%lf", &matrix[i * n + j]) != 1) {
                fprintf(stderr, "���������� �������� �������.\n");
                free(matrix);
                return 1;
            }
        }
    }

    printf("���� �������:\n");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%.3lf\t", matrix[i * n + j]);
        }
        printf("\n");
    }

    double x;
    printf("������ ����� X, ��� ������� ������ � ������� ���������:\n");
    if (scanf("%lf", &x) != 1) {
        fprintf(stderr, "���������� �������� X.\n");
        free(matrix);
        return 1;
    }

    const double EPS = 1e-9;
    int found_any = 0;

    // ������ x � ������� ��������� ������ (������� ����� �� ������� ���������)
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
                printf("�������� X �������� � ��������� %d �� ������� (%d, %d)\n", j, mid, j);
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
            printf("�������� X �� �������� � ��������� %d\n", j);
        }
    }

    if (!found_any)
    {
        printf("�������� X �� �������� � ������� ��������� �������.\n");
    }

    free(matrix);
    return 0;
}