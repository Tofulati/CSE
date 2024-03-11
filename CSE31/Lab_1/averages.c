#include <stdio.h>

int main() {
    int temp, value, oddSum = 0, evenSum = 0, tempSum = 0, counter = 1, evenCount = 0, oddCount = 0;

    printf("Enter the 1st value: ");
    scanf("%d", &value);

    if (value == 0) {
        printf("\nThere is no average to compute.");
        return 0;
    } else {
        while (value != 0) {
            temp = value;
            while (temp != 0) {
                int test = temp % 10;
                if (test > 0) {
                    tempSum += test;
                } else {
                    tempSum -= test;
                }
                temp = temp / 10;
            }

            if (tempSum % 2 == 0) {
                evenSum += value;
                evenCount++;
            } else {
                oddSum += value;
                oddCount++;
            }

            tempSum = 0;
            counter++;

            if ((counter % 10 == 1) && (counter % 100 != 11)) {
                printf("Enter the %dst value: ", counter);
            } else if ((counter % 10 == 2) && (counter % 100 != 12)) {
                printf("Enter the %dnd value: ", counter);
            } else if ((counter % 10 == 3) && (counter % 100 != 13)) {
                printf("Enter the %drd value: ", counter);
            } else {
                printf("Enter the %dth value: ", counter);
            }

            scanf("%d", &value);
        }

        printf("\n");
        if (evenCount > 0) {
            printf("Average of input values whose digits sum up to an even number: %.2f\n", (float)evenSum / evenCount);
        }

        if (oddCount > 0) {
            printf("Average of input values whose digits sum up to an odd number: %.2f\n", (float)oddSum / oddCount);
        }

        return 0;
    }
}