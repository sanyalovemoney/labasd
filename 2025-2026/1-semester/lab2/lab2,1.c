
#include <stdio.h>
#include <math.h>

int main()
{
    int n, i, j, ct;
    double S, P, num;

    S = 0;
    ct = 0;
    ct += 2; 

    printf("Введіть n: ");
    scanf("%d", &n);

    if (n > 0)
    {
        for (i = 1; i <= n; i++)
        {
            
            P = 1;
            ct++; 

            for (j = 1; j <= i + 1; j++)
            {
                P *= j;
                ct += 2; 
            }

           
            num = pow(2, i) + 1;
            ct += 2; 
            num = num * num;
            ct++; 

            S += num / P;
            ct += 2;
            ct += 2; 
        }

        printf("Результат = %.7f\n", S);
        ct++; 
    }
    else
    {
        printf("Немає значення для n\n");
    }

    printf("Кількість операцій = %d\n", ct);
    return 0;
}