#include <stdio.h>

int main()
{
    float x, y;
    printf("input x: ");
    scanf("%f", &x);

    // x ? (2,12] ? (22,32)
    if ((x > 2 && x <= 12) || (x > 22 && x < 32))
    {
        y = -9 * x * 3 + 5 * x * 2;
        printf("your y(%.2f) = %.2f\n", x, y);
    }
    // x ? (-?, 0]
    else if (x <= 0)
    {
        y = -x * 2 - 12;
        printf("your y(%.2f) = %.2f\n", x, y);
    }
    else
    {
        printf("no solution for x \n");
    }

    return 0;
}