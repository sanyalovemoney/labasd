#include <stdio.h>
#include <math.h>

int main()
{
    int n, i, ct;
    double S, P, num;

    S = 0;
    P = 1; // Початкове значення для факторіалу
    ct = 0;
    ct += 3; 
    
    printf("Введіть n: ");
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Невірний ввід. Потрібне ціле число.\n");
        return 1;
    }

    if (n > 0)
    {
        for (i = 1; i <= n; i++)
        {
            // Динамічне програмування: використовуємо попереднє значення P
            P *= (i + 1); 
            ct += 2; 
            
            num = pow(2, i) + 1;
            ct += 2; 
            num = num * num; 
            ct++; 
            
            S += num / P;
            ct += 2; 
            ct++; 
        }

        printf("Результат = %.7f\n", S);
        ct++; // вивід результату
    }
    else
    {
        printf("Немає значення для n\n");
    }

    printf("Кількість операцій = %d\n", ct);
    return 0;
}