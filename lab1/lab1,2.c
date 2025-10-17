
#include <stdio.h>
int main() {
    int x, y;
    printf("Введіть x: ");
    scanf("%d", &x);
	// x ? (22,32) ? (-?,0]
    if (x > 22 && x < 32) {
        y = -9 * x * x * x + 5 * x * x;
        printf("Результат: y = %d\n", y);
    }
	// x ? (-?,0]
    else if (x <= 0) {
        y = -x * x - 12;
        printf("Результат: y = %d\n", y);
    }
    else {
        printf("немає відповіді\n");
    }
    return 0;
}