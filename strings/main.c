#include <stdio.h>
#include "libstr.h"

#define arraySize(x) (sizeof(x) / sizeof(x[0]))

int main(void) {
    /* Init menu-related data */
    int (*functions[])(char *, char *) = {hamming_dist, b64_distance};
    int numFuncs = arraySize(functions);
    char *descriptions[] = {"hamming distance", "base64 distance"};
    int choice = 0;

    /* Init program data */
    char str1[MAX_STR] = {0}, str2[MAX_STR] = {0};

    /* Print menu */
    while (choice != numFuncs + 1) {
        /* Print menu */
        puts("Choose an option:");

        for (int i = 0; i < numFuncs; i++) {
            printf("\t%d. %s\n", i + 1, descriptions[i]);
        }

        printf("\t%d. exit\n", numFuncs + 1);

        /* Get user input */
        scanf("%d", &choice);

        /* Run function */
        if (1 <= choice && choice <= numFuncs) {
            puts("Enter two strings:");
            scanf("%s %s", str1, str2);
            int res = functions[choice - 1](str1, str2);
            printf("Result: %d\n", res);
        }
    }

    return 0;
}