
#include <stdio.h>
int main() {
    int x, y;
    printf("Input x: ");
    scanf("%d", &x);
	// x ? (22,32) ? (-?,0]
    if (x > 22 && x < 32) {
        y = -9 * x * x * x + 5 * x * x;
        printf("Result: y = %d\n", y);
    }
	// x ? (-?,0]
    else if (x <= 0) {
        y = -x * x - 12;
        printf("Result: y = %d\n", y);
    }
    else {
        printf("No solution for this x\n");
    }
    return 0;
}