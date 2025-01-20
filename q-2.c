#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_LEN 256
const double TOLERANCE = 1e-10;
const double MAX_DIFF = 1e-6;

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

int read_input_size(FILE *input_file) {
    int n;
    fscanf(input_file, "%d", &n);
    return n;
}

void read_input_matrix(FILE *input_file, int n, double **mat) {
   for(int i=0; i < n; i++) {
    for(int j = 0; j < n; j++) {
        fscanf(input_file, "%lf,", &mat[i][j]);
    }
   }
}

void read_input_vector(FILE *input_file, int n, double *vec) {
    for(int i = 0 ; i < n; i++) {
        fscanf(input_file, "%lf,", &vec[i]);
    }
}

void multiply_matrix_vector(int n, double **mat, double *vec, double *res) {
    for (int i = 0; i < n; i++) {
        res[i] = 0;
        for (int j = 0; j < n; j++) {
            res[i] += mat[i][j] * vec[j];
        }
    }
}

int check_eigenvector(int n, double *res, double *vec, double *lambda, double *eigenvalue) {

    int all_zero = 1;
    for (int i = 0; i < n; i++) {
        if (fabs(vec[i]) > TOLERANCE) {
            all_zero = 0;
            break;
        }
    }
    if (all_zero) {
        return 0;
    }

    double min_value = INFINITY;
    double max_value = -INFINITY;
    int found_valid_lambda = 0;

    for (int i = 0; i < n; i++) {
        if (fabs(vec[i]) <= TOLERANCE) {
            continue;
        }

        lambda[i] = res[i] / vec[i];

        if (!found_valid_lambda) {
            min_value = max_value = lambda[i];
            found_valid_lambda = 1;
            *eigenvalue = lambda[i];
        } else {
            min_value = fmin(min_value, lambda[i]);
            max_value = fmax(max_value, lambda[i]);
        }
    }

    if (!found_valid_lambda || (max_value - min_value > MAX_DIFF)) {
        return 0;
    }

    return 1;
}

void append_eigenvalue(int n, int vec_num, double eigenvalue) {
    char filename[MAX_LEN];
    sprintf(filename, "inputfiles/vec_%06d_%06d.in", n, vec_num);
    
    FILE* f = fopen(filename, "a");

    fprintf(f, "Eigenvalue: %8e\n", eigenvalue);

    fclose(f);
}

int main() {
    FILE *input_file = fopen("inputfiles/input.in", "r");
    if (input_file == NULL) {
        printf("Error: Cannot open input.in file.\n");
        exit(1);
    }

    int n = read_input_size(input_file);
    fclose(input_file);

    char mat_filename[MAX_LEN];
    sprintf(mat_filename, "inputfiles/mat_%06d.in", n);
    FILE *mat_file = fopen(mat_filename, "r");
    if (mat_file == NULL) {
        printf("Error: Cannot open %s file.\n", mat_filename);
        exit(1);
    }

    double **mat = memory_allocate_2D(n, n);

    read_input_matrix(mat_file, n, mat);
    fclose(mat_file);

    int vec_num = 1;
    char vec_filename[MAX_LEN];

    while (1) {
        sprintf(vec_filename, "inputfiles/vec_%06d_%06d.in", n, vec_num);
        FILE *vec_file = fopen(vec_filename, "r");
        if (vec_file == NULL) {
            break;
        }

        double *vec = memory_allocate_1D(n);
        double *res =  memory_allocate_1D(n);
        double *lambda =  memory_allocate_1D(n);
        double eigenvalue;

        read_input_vector(vec_file, n, vec);
        fclose(vec_file);

        multiply_matrix_vector(n, mat, vec, res);

        if (check_eigenvector(n, res, vec, lambda, &eigenvalue)) {
            append_eigenvalue(n, vec_num, eigenvalue);
            printf("vec_%06d_%06d.in : Yes : %8e\n", n, vec_num, eigenvalue);
        } else {
            printf("vec_%06d_%06d.in : Not an eigenvector\n", n, vec_num);
        }

        free(vec);
        free(res);
        free(lambda);

        vec_num++;
    }

    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);

    return 0;
}
