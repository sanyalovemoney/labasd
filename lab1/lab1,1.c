#include <stdio.h>

int main() {
    int x, y;

    printf("Введіть число x: ");
    scanf("%d", &x);

    // 1. Якщо x <= 0
    if (!(x > 0)) {  // заміна "x <= 0" через заперечення
        y = -x * x - 12;
        printf("y = -x^2 - 12 = %d\n", y);
    }
    else {
        // 2. Якщо 0 < x <= 2 → немає відповіді
        if (x <= 2) {
            printf("немає відповіді\n");
        }
        else {
            // 3. Якщо 2 < x <= 12
            if (x > 2) {
                if (x <= 12) {
                    y = -9 * x * 3 + 5 * x * 2;
                    printf("y = -9x*3 + 5x*2 = %d\n", y);
                }
                else {
                    // 4. Якщо 22 < x < 32
                    if (x > 22) {
                        if (x < 32) {
                            y = -9 * x * 3 + 5 * x * 2;
                            printf("y = -9x*3 + 5x*2 = %d\n", y);
                        }
                        else {
                            printf("немає відповіді\n");
                        }
                    }
                    else {
                        printf("немає відповіді\n");
                    }
                }
            }
        }
    }

    return 0;
}
