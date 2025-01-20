#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 50

double* memory_allocate_1D(int size) {
    double *a = (double* )malloc(size * sizeof(double));

    if(a == NULL) {
        printf("Memory allocation failed, not enough memory available.\n");
        exit(1);
    }

    return a;
}

double** memory_allocate_2D(int row, int col) {
    double** mat = (double **)malloc(row * sizeof(double *));

    if(mat == NULL) {
        printf("Memory allocation failed, not enough memory available.\n");
        exit(1);
    }

    for(int i = 0; i < row; i++) {
        mat[i] = (double *)malloc(col * sizeof(double));

        if(mat[i] == NULL) {
            printf("Memory allocation failed, not enough memory available.\n");
            exit(1);
       }
    }

    return mat;
}

void print_to_file(int n, double **A, int format_flag) {
    char filename[MAX_LEN];
    sprintf(filename, "array_%06d_%s.out", n, format_flag == 0 ? "asc" : "bin");

    FILE *file = fopen(filename, format_flag == 0 ? "w" : "wb");
    if (!file) {
        printf("Error opening file");
        exit(1);
    }

    if (format_flag == 0) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                fprintf(file, "%.15f ", A[i][j]);
            }
            fprintf(file, "\n");    
        }
    } else {
        for (int i = 0; i < n; i++) {
            fwrite(A[i], sizeof(double), n, file);
        }
    }

    fclose(file);
}

int main() {
    FILE *input_file = fopen("inputfiles/input.in", "r");
    if (input_file == NULL) {
        printf("Error: Cannot open input file.\n");
        exit(1);

    }

    int n, format_flag;
    fscanf(input_file, "%d", &n);
    fclose(input_file);

    double **A = memory_allocate_2D(n, n);
    for (int i = 0; i < n; i++) {
        A[i] = (double *)malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            A[i][j] = i + j;
        }
    }

    format_flag = 1;
    print_to_file(n, A, format_flag);
    for (int i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);

    return 0;
}
    
