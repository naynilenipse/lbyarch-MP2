#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

extern void imgCvtGrayIntToDouble(int width, int height, int* input, double output[]);

int main() {
    int width, height;

    scanf("%d %d", &height, &width);

    int total = width * height;

    int* input = malloc(total * sizeof(int));
    double* output = malloc(total * sizeof(double));

    if (input == NULL || output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < total; i++) {
        scanf("%d", &input[i]);
    }

    //debugging
    //printf("width = %d\n", width);
    //printf("height = %d\n", height);

    imgCvtGrayIntToDouble(width, height, input, output);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%.2f ", output[i * width + j]);
        }
        printf("\n");
    }

    free(input);
    free(output);

    return 0;
}