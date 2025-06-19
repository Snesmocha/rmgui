#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <rmgui/dynarray.h>

void benchmark_c_vector_insert(size_t insert_count) {
    vector vec = initialize_vec(sizeof(int));
    int value = 42;

    clock_t start = clock();

    for (size_t i = 0; i < insert_count; ++i) {
        size_t mid = vec.size / 2;
        insert_vector(&vec, &value, mid, 1);
    }

    clock_t end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC;

    printf("C vector insert:   %f seconds\n", duration);

    free_vector(&vec);
}

int main(void) {
    const size_t count = 100000;

    printf("Benchmarking C vector middle insertions (%zu elements):\n", count);

    benchmark_c_vector_insert(count);

    return 0;
}
