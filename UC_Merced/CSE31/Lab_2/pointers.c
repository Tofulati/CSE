#include <stdio.h>

int main() {
    int x, y, *px, *py;
    px = &x;
    py = &y;

    int arr[10];
    int *pa = arr;

    printf("x = %d, y = %d, arr[0] = %d\n", x, y, arr[0]);

    printf("x = %p, y = %p\n", &x, &y);

    printf("px value = %d, px address = %p, py value = %d, py address = %p\n", *px, px, *py, py);

    for (int i = 0; i < 10; i++) {
        printf("%d", *(pa));
        if (i < 9) {
            printf(", ");
        }
        pa++;
    }

    printf("\n");

    printf("%p\n", &arr[0]);

    printf("%p\n", &arr);

    return 0;
}