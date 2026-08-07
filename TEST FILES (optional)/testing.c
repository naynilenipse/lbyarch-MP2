// benchmark.c

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

extern void imgCvtGrayIntToDouble(int width, int height, int* input, double output[]);

#define TRIALS 30
#define EPS 1e-9

static void fillRandom(int* input, int total) {
    for (int i = 0; i < total; i++) {
        input[i] = rand() % 256;
    }
}

// Runs the asm function trials times over the same input, timing only the call itself.
// Returns the average time in milliseconds.
typedef void (*ConvertFn)(int, int, int*, double*);

static double timeFunction(ConvertFn fn, int width, int height, int* input, double* output) {
    LARGE_INTEGER freq, t0, t1;
    QueryPerformanceFrequency(&freq);

    double totalMs = 0.0;
    for (int t = 0; t < TRIALS; t++) {
        QueryPerformanceCounter(&t0);
        fn(width, height, input, output);
        QueryPerformanceCounter(&t1);
        totalMs += (double)(t1.QuadPart - t0.QuadPart) * 1000.0 / (double)freq.QuadPart;
    }
    return totalMs / TRIALS;
}

static void correctnessCheck(int width, int height) {
    int total = width * height;
    int* input = (int*)malloc(total * sizeof(int));
    double* outAsm = (double*)malloc(total * sizeof(double));

    if (input == NULL || outAsm == NULL) {
        fprintf(stderr, "Allocation failed for correctness check (%dx%d)\n", width, height);
        free(input);
        free(outAsm);
        return;
    }

    srand(42);
    fillRandom(input, total);

    imgCvtGrayIntToDouble(width, height, input, outAsm);

    int mismatchesExpected = 0;

    for (int i = 0; i < total; i++) {
        double expected = (double)input[i] / 255.0;
        if (fabs(outAsm[i] - expected) > EPS) {
            mismatchesExpected++;
        }
    }

    printf("Correctness check (%dx%d, %d pixels)\n", width, height, total);
    printf("Sample values (input -> asm output | expected):\n");
    int shown = total < 10 ? total : 10;
    for (int i = 0; i < shown; i++) {
        double expected = (double)input[i] / 255.0;
        printf("  in=%3d -> asm=%.6f | expected=%.6f\n",
            input[i], outAsm[i], expected);
    }

    if (mismatchesExpected == 0) {
        printf("PASS: all %d asm outputs match expected int/255.0\n", total);
    }
    else {
        printf("FAIL: %d/%d asm outputs did not match expected value\n", mismatchesExpected, total);
    }

    printf("\n");

    free(input);
    free(outAsm);
}

static void benchmarkSize(int width, int height) {
    int total = width * height;
    int* input = (int*)malloc(total * sizeof(int));
    double* output = (double*)malloc(total * sizeof(double));

    if (input == NULL || output == NULL) {
        fprintf(stderr, "Allocation failed for size %dx%d\n", width, height);
        free(input);
        free(output);
        return;
    }

    srand(1); // fixed seed so each size gets reproducible input data
    fillRandom(input, total);
    double asmMs = timeFunction(imgCvtGrayIntToDouble, width, height, input, output);

    printf("Size %5d x %-5d (%8d pixels): asm avg = %10.6f ms\n",
        width, height, total, asmMs);

    free(input);
    free(output);
}

int main(void) {
    correctnessCheck(10, 10);

    printf("Timing (%d runs averaged per size)\n", TRIALS);
    benchmarkSize(10, 10);
    benchmarkSize(100, 100);
    benchmarkSize(1000, 1000);

    return 0;
}