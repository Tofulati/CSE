#include <stdio.h>

int main() {
    int count, typo;

    //Ask for repetition count
    printf("Enter the repetition count for the punishment phrase: ");
    scanf("%d", &count);

    while (count <= 0) {
        printf("You entered an invalid value for the repetition count! Please re-enter: ");
        scanf("%d", &count);
    }

    printf("\n");

    //Ask for typo line
    printf("Enter the line where you want to insert the typo: ");
    scanf("%d", &typo);

    while (typo <= 0 || typo > count) {
        printf("You entered an invalid value for the typo placement! Please re-enter: ");
        scanf("%d", &typo);
    }

    printf("\n");

    //Prints out lines
    for (int i = 1; i <= count; i++) {
        if (i == typo) {
            printf("Cading wiht is C avesone!\n");
        } else {
            printf("Coding with C is awesome!\n");
        }
    }

    return 0;
}