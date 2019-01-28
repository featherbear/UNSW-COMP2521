#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "Item.h"
#include "quicksort.h"

static const int MODE_QUIET = 8; // 0b1000

static int checkArguments(int argc, char **argv);

static int checkArguments(int argc, char **argv) {
    int result = 0;
    bool status = true;

    if (argc == 1) status = false;
    else {
        bool modeFlagSelected = false;

        for (size_t i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-pn") == 0) (modeFlagSelected = true) && (result |= MODE_NAIVE);
            else if (strcmp(argv[i], "-pm") == 0) (modeFlagSelected = true) && (result |= MODE_MEDIAN);
            else if (strcmp(argv[i], "-pr") == 0) (modeFlagSelected = true) && (result |= MODE_RANDOM);
            else if (strcmp(argv[i], "-q") == 0) result |= MODE_QUIET;
            else {
                status = false;
                break;
            }
        }

        if (!modeFlagSelected) status = false;
    }

    if (status) return result;
    return 0;
}

int main(int argc, char **argv) {
    int mode = checkArguments(argc, argv);
    if (!mode) {
        printf("Usage: %s -p<n/m/r> [-q]\n", argv[0]);
        return -1;
    }

    // Get size
    fprintf(stdout, (!(mode & MODE_QUIET)) ? "Enter data size: " : "");
    size_t n_items;
    scanf("%zu", &n_items);

    // Scan in items
    Item *items = malloc(n_items * sizeof(Item));
    memset(items, 0, n_items);
    if (!(mode & MODE_QUIET)) puts("Enter data:");
    for (size_t i = 0; i < n_items; i++) scanf("%d", &items[i]);

    // Do the sort here
    if (mode & MODE_NAIVE) quicksort_naive(items, n_items);
    else if (mode & MODE_MEDIAN) quicksort_median(items, n_items);
    else if (mode & MODE_RANDOM) quicksort_random(items, n_items);

    // Print if quiet flag is not present
    if (!(mode & MODE_QUIET)) {
        if (mode & MODE_NAIVE) puts("Sorted with naive pivot:");
        if (mode & MODE_MEDIAN) puts("Sorted with median of three pivot:");
        if (mode & MODE_RANDOM) puts("Sorted with randomised pivot:");
        printArrayInline(items, n_items);
        puts("");
    }
}
