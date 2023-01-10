#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

const double EPS = 0.00000001;

typedef struct Matrix {
    unsigned int rows;
    unsigned int cols;
    double* values;
} Matrix;


Matrix memory_allocation(const unsigned int rows, const unsigned int cols) { // ��������� ������
    Matrix mem = { rows,cols,NULL };
    mem.values = (double*)malloc(rows * cols * sizeof(double));
    if (mem.values == NULL) {
        mem.rows = 0;
        mem.cols = 0;
        printf("Error:memory are not allocated\n");
        exit(404);
    }
    return  mem;
}


Matrix free_matrix(Matrix A) { // ������������ ������
    A.rows = 0;
    A.cols = 0;
    free(A.values);
    return A;
}



void output(const char* function_name, Matrix A) { // ���� ������
    printf("\n%s\n", function_name);
    for (unsigned int row = 0; row < A.rows; row++)
    {
        for (unsigned int col = 0; col < A.cols; col++) 
            printf("%lf ", A.values[row * A.cols + col]);
        printf("\n"); 
    }
}


void fill_random(Matrix m) { // �������� ��������� �������� � ������
    for (unsigned int index = 0; index < m.cols * m.rows; index++) {
        m.values[index] = rand()%10 ;
    }
}


Matrix EMPTY() { // ������� ������� � ������ ���������� �����
    Matrix EMPTY = memory_allocation(0, 0);
    EMPTY.values = NULL;
    return EMPTY;
}


Matrix create_zero_matrix(const unsigned int rows, const unsigned int cols) { //�������� ������� �� �����
    Matrix matrix = memory_allocation(rows, cols);
    for (unsigned int idx = 0; idx < rows * cols; idx++) matrix.values[idx] = 0.0;
    return matrix;
}


Matrix transposition(const Matrix A) { // ���������������� �������
    Matrix trans = memory_allocation(A.cols, A.rows);
    for (unsigned int row = 0; row < trans.rows; row++) {
        for (unsigned int col = 0; col < trans.cols; col++) {
            trans.values[row * trans.cols + col] = A.values[col * trans.rows + row];
        }
    }
    return  trans;
}


Matrix minor(const Matrix matrix, const unsigned int size, const unsigned int row, const unsigned int col) {
    Matrix minor = memory_allocation(matrix.rows - 1, matrix.cols - 1);
    unsigned int shiftrow = 0; //�������� ������� ������ � �������
    unsigned int shiftcol; //�������� ������� ������� � �������
    for (unsigned int rows = 0; rows < size - 1; rows++) {
        if (rows == row) shiftrow = 1;  
        shiftcol = 0; 
        for (unsigned int cols = 0; cols < size - 1; cols++) {
            if (cols == col) shiftcol = 1;
            minor.values[rows * (size - 1) + cols] = matrix.values[(rows + shiftrow) * size + (cols + shiftcol)];
        }
    }
    return minor;
}


Matrix create_unit_matrix(const unsigned int size) { // �������� ��������� �������
    Matrix mat = create_zero_matrix(size, size);
    for (unsigned int idx = 0; idx < size * size; idx += size + 1) mat.values[idx] = 1.0;
    return mat;
}


Matrix addition(const Matrix A, const Matrix B) { // �������� ������
    if (A.rows != B.rows || A.cols != B.cols) {
        printf("For Addition Matrix should have same size\n");
        return EMPTY();
    }
    Matrix sum = memory_allocation(A.rows, B.cols);
    for (unsigned int index = 0; index < sum.rows * sum.cols; index++) {
        sum.values[index] = A.values[index] + B.values[index];
    }
    return sum;
}


Matrix subtraction(const Matrix m1, const Matrix m2) { // ��������� ������
    if (m1.rows != m2.rows || m1.cols != m2.cols) {
        printf("For Subtraction Matrix should have same size\n");
        return EMPTY();
    }
    Matrix sub = memory_allocation(m1.rows, m1.cols);
    for (unsigned int index = 0; index < sub.rows * sub.cols; index++) {
        sub.values[index] = m1.values[index] - m2.values[index];
    }
    return sub;
}


Matrix multiplication(const Matrix A, const Matrix B) { // ��������� ������
    if (A.cols != B.rows) {
        printf("For Multiplication First Matrix cols should be equal to Second Matrix rows\n");
        return EMPTY();
    }
    Matrix multiplication = memory_allocation(A.rows, B.cols);
    for (unsigned int row = 0; row < multiplication.rows; row++) {
        for (unsigned int col = 0; col < multiplication.cols; col++) {
            double sum = 0;
            for (unsigned int k = 0; k < A.cols; k++) {
                sum += A.values[row * A.cols + k] * B.values[k * B.cols + col];
            }
            multiplication.values[row * multiplication.cols + col] = sum;
        }
    }
    return multiplication;
}


Matrix multy_k(const Matrix matrix, const double k) { // ��������� ������� �� �����
    Matrix multy = memory_allocation(matrix.rows, matrix.cols);
    for (unsigned int index = 0; index < matrix.rows * matrix.cols; index++) {
        multy.values[index] = matrix.values[index] * k;
    }
    return multy;
}


Matrix division_k(const Matrix matrix, const double k) { // ������� ������� �� �����
    Matrix division = memory_allocation(matrix.rows, matrix.cols);
    for (unsigned int index = 0; index < matrix.rows * matrix.cols; index++) {
        division.values[index] = matrix.values[index] / k;
    }
    return division;
}


double determinant(const Matrix matrix, const unsigned int size) { // ������������ � ����������� �������
    if (matrix.rows != matrix.cols) {
        printf("Matrix should be nxn\n");
        return 0;
    }
    double det = 0;
    int k = 1;
    if (size == 0) return 0;
    if (size == 1) return matrix.values[0];
    if (size == 2) return (matrix.values[0] * matrix.values[3] - matrix.values[2] * matrix.values[1]);
    for (unsigned int idx = 0; idx < size; idx++) {
        Matrix temp = minor(matrix, size, 0, idx);
        det += k * matrix.values[idx] * determinant(temp, size - 1);
        k = -k;
        free_matrix(temp);
    }
    return det;
}


Matrix reverse_matrix(const Matrix A, const unsigned int size) { // ������� ���������� �������� �������
    double det = determinant(A, size);
    if (A.rows != A.cols) {
        printf("Matrix should have size nxn\n");
        return EMPTY();
    }
    if (fabs(det) < EPS) {
        printf("Matrix is degenerative, determinate is not determined\n");
        return EMPTY();
    }
    Matrix reverse = memory_allocation(size, size);
    Matrix temp;
    int k = 1;
    for (unsigned int i = 0; i < reverse.rows; i++) {
        for (unsigned int j = 0; j < reverse.cols; j++) {
            temp = minor(A, size, i, j);
            reverse.values[i * size + j] = k * determinant(temp, size - 1);
            k = -k;
        }
        free_matrix(temp);
    }
    Matrix temp_transpose = transposition(reverse);
    free_matrix(reverse);
    Matrix temp_division = division_k(temp_transpose, det);
    free_matrix(temp_transpose);
    reverse = temp_division;
    return reverse;
}


Matrix exponent_matrix(const Matrix exponent) { // ������� ���������� ���������� �������
    if (exponent.rows != exponent.cols) {
        printf("Matrix should have size nxn\n");
        return EMPTY();
    }
    unsigned int n = 30; // ���������� ������ ����
    Matrix ex = create_unit_matrix(exponent.rows);
    Matrix temp = create_unit_matrix(exponent.rows);
    Matrix temp_multiply;
    Matrix temp_add;
    double fact = 1.0;
    for (unsigned int i = 1; i < n; i++) {
        fact *= i;
        temp_multiply = multiplication(temp, exponent);
        free_matrix(temp);
        temp = temp_multiply;
        temp_add = addition(ex, division_k(temp, fact));
        free_matrix(ex);
        ex = temp_add;
    }
    free_matrix(temp_multiply);
    return ex;
}


int main()
{
    
    Matrix A;
    A = memory_allocation(3, 3);
    fill_random(A);
    output("First Matrix", A);
    Matrix B;
    B = memory_allocation(3, 3);
    fill_random(B);
    output("Second Matrix", B);
    Matrix add = addition(A, B);
    output("Addiction", add);
    Matrix sub = subtraction(A, B);
    output("Subtraction", sub);
    Matrix multy = multiplication(A, B);
    output("Multiplication", multy);
    printf("det %lf\n", determinant(A, A.rows));
    Matrix reverse = reverse_matrix(A, A.rows);
    output("Reverse Matrix", reverse);
    Matrix exp = exponent_matrix(A);
    output("Exponent Matrix", exp);

    free_matrix(A);
    free_matrix(B);
    free_matrix(add);
    free_matrix(sub);
    free_matrix(multy);
    free_matrix(exp);
    return 0;
}