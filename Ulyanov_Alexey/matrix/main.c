#include <stdio.h>
#include "malloc.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

const unsigned int max_range = 51;


struct Matrix {
    unsigned int rows;
    unsigned int cols;

    double *data;
    double **value;
};

struct Matrix creation(const unsigned int rows, const unsigned int cols){
    struct Matrix math;
    math.rows = rows;
    math.cols = cols;
    math.data = (double*)malloc(rows * cols * sizeof (double *));
    math.value = (void**)malloc(rows * sizeof(void *));

    for (unsigned int row = 0; row < rows; row++){
        math.value[row] = math.data + row * cols * sizeof(void);

    }
    return math;
}

struct Matrix Empty(){

    struct Matrix rez = creation(0,0);
    return rez;

}

struct Matrix Zero(unsigned int rows, unsigned int cols){

    struct Matrix rez = creation(rows, cols);
    for (int i = 0; i < rows * cols; i++){
        rez.data[i] = 0.0;
    }
    return rez;

}

struct Matrix Unit(unsigned int n){

    struct Matrix rez = Zero(n, n);
    for (int i = 0; i < n; i++){
        rez.value[i][i] = 1.0;
    }
    return rez;

}

struct Matrix transponation(struct Matrix x){
    struct Matrix rez = creation(x.cols, x.rows);
    for (int row = 0; row < rez.rows; row++){
        for (int col = 0; col < rez.cols; col++){
            rez.value[row][col] = x.value[col][row];
        }
    }
    return rez;
}

struct Matrix minor(unsigned int cur_row, unsigned int cur_col, struct Matrix x){
    struct Matrix rez = creation( x.rows - 1, x.cols - 1);
    for (int row = 0; row < cur_row; row++){
        for (int col = 0; col < cur_col; col++){
            rez.value[row][col] = x.value[row][col];
        }
    }
    for (int row = 0; row < cur_row; row++){
        for (int col = cur_col; col < rez.cols; col++){
            rez.value[row][col] = x.value[row][col+1];
        }
    }
    for (int row = cur_row; row < rez.rows; row++){
        for (int col = 0; col < cur_col; col++){
            rez.value[row][col] = x.value[row+1][col];
        }
    }
    for (int row = cur_row; row < rez.rows; row++){
        for (int col = cur_col; col < rez.cols; col++){
            rez.value[row][col] = x.value[row+1][col+1];
        }
    }
    return rez;
}

struct Matrix addition(const struct Matrix x, const struct Matrix y){
    if ((x.cols != y.cols) || (x.rows != y.rows)) return Empty();

    struct Matrix rez = creation(x.rows, x.cols);
    for (int i = 0; i < x.rows * x.cols; i++){
        rez.data[i] = x.data[i] + y.data[i];
    }
    return rez;

}

struct Matrix subtraction(const struct Matrix x, const struct Matrix y){
    if ((x.cols != y.cols) || (x.rows != y.rows)) return Empty();

    struct Matrix rez = creation(x.rows, x.cols);
    for (int i = 0; i < x.rows * x.cols; i++){
        rez.data[i] = x.data[i] - y.data[i];
    }
    return rez;

}

struct Matrix multiplication(const struct Matrix x, const struct Matrix y){
    if (x.cols != y.rows) return Empty();

    struct Matrix rez = creation(x.rows, y.cols);
    for (int row = 0; row < rez.rows; row++){
        for (int col = 0; col < rez.cols; col++){
            rez.value[row][col] = 0.0;
            for (int k = 0; k < x.cols; k++){
                rez.value[row][col] += x.value[row][k] * y.value[k][col];
            }
        }
    }
    return rez;

}

struct Matrix multy_k(struct Matrix x, double k){

    struct Matrix rez = creation(x.rows, x.cols);
    for (int i = 0; i < x.rows * x.cols; i++){
        rez.data[i] = x.data[i] * k;
    }
    return rez;

}

double det(struct Matrix x){
    if (x.cols != x.rows) return *(double*)malloc(1 * sizeof(double *));

    if (x.cols == 1){
        return x.value[0][0];
    } else {
        double rez = 0.0;
        for (int i = 0; i < x.cols; i++) {
            struct Matrix temp = minor(0, i, x);
            rez += pow(-1, i) * x.value[0][i] * det(temp);
            free(temp.value);
        }
        return rez;
    }

}

struct Matrix reverse(struct Matrix x){
    double deter = det(x);
    if ((x.cols != x.rows) || (deter == 0.0)) return Empty();

    struct Matrix rez = creation(x.rows, x.cols);
    for (int row = 0; row < rez.rows; row++){
        for (int col = 0; col < rez.cols; col++){
            struct Matrix temp = minor(row, col, x);
            rez.value[row][col] = pow(-1, row + col) * det(temp);
            free(temp.value);
        }
    }
    return multy_k(transponation(rez), 1/deter);

}

struct Matrix pow_mat(struct Matrix x, unsigned int n){
    if (x.rows != x.cols) return Empty();

    if (n == 0) return Unit(x.rows);
    struct Matrix rez = creation(x.rows, x.cols);
    rez.data = x.data;
    rez.value = x.value;
    for (int i = 1; i < n; i++){
        rez = multiplication(x, rez);
    }
    return rez;

}

struct Matrix exponent(struct Matrix x, const unsigned int kol_slog){
    if (x.cols != x.rows) return Empty();

    struct Matrix rez = Zero(x.rows, x.cols);
    double a_n = 1;
    for (int i = 0; i < kol_slog; i++){
        rez = addition(rez, multy_k(pow_mat(x, i), a_n));
        a_n /= i + 1;
    }
    return rez;

}


void output(struct Matrix x){

    for (int row = 0; row < x.rows; row++){
        for (int col = 0; col < x.cols; col++){
            printf("%.2lf ", x.value[row][col]);
        }
        printf("\n");
    }
    printf("\n");

}


void test_of_add(){

    struct Matrix first = creation(2, 3);
    struct Matrix second = creation(2,3);

    double arr_first[2][3] = {{1, 2, 3}, {4,5, 6}};
    double arr_second[2][3] = {{6, 5, 4}, {3, 2,1}};

    for (int row = 0; row < first.rows; row++){
        for (int col = 0; col < first.cols; col++) {
            first.value[row][col] = arr_first[row][col];
        }
    }
    for (int row = 0; row < second.rows; row++){
        for (int col = 0; col < second.cols; col++) {
            second.value[row][col] = arr_second[row][col];
        }
    }

    struct Matrix rez_add = addition(first, second);

    int flag = (addition(Zero(2,2), first).rows == Empty().rows);
    for (int row = 0; row < rez_add.rows; row++){
        for (int col = 0; col < rez_add.cols; col++) {
            flag *= (rez_add.value[row][col] == 7.0);
        }
    }

    if (flag){
        printf("Test of addition was successful\n");
    } else {
        printf("Test of addition was faild\n");
    }
}

void test_of_sub(){
    struct Matrix first = creation(2, 3);
    struct Matrix second = creation(2,3);

    double arr_first[2][3] = {{1, 2, 3}, {4,5, 6}};
    double arr_second[2][3] = {{1, 2, 3}, {4,5, 6}};

    for (int row = 0; row < first.rows; row++){
        for (int col = 0; col < first.cols; col++) {
            first.value[row][col] = arr_first[row][col];
        }
    }
    for (int row = 0; row < second.rows; row++) {
        for (int col = 0; col < second.cols; col++) {
            second.value[row][col] = arr_second[row][col];
        }
    }

    struct Matrix rez_sub = subtraction(first, second);

    int flag = (subtraction(Zero(2,2), Unit(3)).rows == Empty().rows);
    for (int row = 0; row < rez_sub.rows; row++){
        for (int col = 0; col < rez_sub.cols; col++) {
            flag *= (rez_sub.value[row][col] == 0.0);
        }
    }

    if (flag){
        printf("Test of subtraction was successful\n");
    } else {
        printf("Test of subtraction was faild\n");
    }
}

void test_of_multy(){
    struct Matrix first = creation(2, 3);
    struct Matrix second = creation(3,1);

    double arr_first[2][3] = {{1, 5, 6}, {3,9, 4}};
    double arr_second[3][1] = {{1}, {0}, {1}};

    for (int row = 0; row < first.rows; row++){
        for (int col = 0; col < first.cols; col++) {
            first.value[row][col] = arr_first[row][col];
        }
    }
    for (int row = 0; row < second.rows; row++) {
        for (int col = 0; col < second.cols; col++) {
            second.value[row][col] = arr_second[row][col];
        }
    }

    struct Matrix rez_mul = multiplication(first, second);

    int flag = (multiplication(Zero(2,3), Unit(2)).rows == Empty().rows);
    for (int row = 0; row < rez_mul.rows; row++){
        for (int col = 0; col < rez_mul.cols; col++) {
            flag *= (rez_mul.value[row][col] == 7.0);
        }
    }

    if (flag){
        printf("Test of multiplication was successful\n");
    } else {
        printf("Test of multiplication was faild\n");
    }
}

void test_of_reverse(){
    unsigned int n = 3;
    struct Matrix obj = creation(n, n);
    for (int row = 0; row < obj.rows; row++){
        for (int col = 0; col < obj.cols; col++){
            obj.value[row][col] = rand() % max_range;
        }
    }

    struct Matrix rez = multiplication(obj, reverse(obj));   //должно соответствовать единичной матрице
    struct Matrix unit_3 = Unit(n);

    int flag = (rez.rows != Empty().rows);

    for (int row = 0; row < rez.rows; row++){
        for (int col = 0; col < rez.cols; col++){
            flag *= (round(rez.value[row][col]) == unit_3.value[row][col]);
        }
    }

    if (flag){
        printf("Test of reverse matrix was successful\n");
    } else {
        printf("Test of reverse matrix was faild\n");
    }
}

void test_of_exponent(){
    struct Matrix base = creation(3, 3);
    for (int row = 0; row < base.rows; row++){
        for (int col = 0; col < base.cols; col++){
            base.value[row][col] = rand() % max_range;
        }
    }

    struct Matrix option_1 = exponent(base, 3);
    struct Matrix option_2 = addition(Unit(3), addition(base, multy_k(multiplication(base, base), 0.5)));

    int flag = (option_1.rows == option_2.rows);
    for (int row = 0; row < option_1.rows; row++){
        for (int col = 0; col < option_1.cols; col++){
            flag *= (option_1.value[row][col] == option_2.value[row][col]);
        }
    }

    if (flag){
        printf("Test of exponent was successful\n");
    } else {
        printf("Test of exponent was faild\n");
    }
}

void block_tests(){
    test_of_add();
    test_of_sub();
    test_of_multy();
    test_of_reverse();
    test_of_exponent();
    printf("\n");
}

int main() {
    //srand(time(NULL));

    struct Matrix A = creation(5, 5);
    struct Matrix B = creation(5, 5);

    for (int i = 0; i < A.rows; i++){
        for (int j = 0; j < A.cols; j++){
            A.value[i][j] = rand() % max_range;
        }
    }
    for (int i = 0; i < B.rows; i++){
        for (int j = 0; j < B.cols; j++){
            B.value[i][j] = rand() % max_range;
        }
    }

    block_tests();

    printf("This is empty Matrix\n");
    output(Empty());
    printf("This is Zero matrix\n");
    output(Zero(5,5));
    printf("This is Unit matrix\n");
    output(Unit(5));

    printf("This is matrix A\n");
    output(A);
    printf("This is matrix B\n");
    output(B);

    printf("This is transporation of matrix A\n");
    output(transponation(A));

    printf("This is matrix A + B\n");
    output(addition(A,B));
    printf("This is matrix A - B\n");
    output(subtraction(A,B));
    printf("This is matrix A * B\n");
    output(multiplication(A, B));
    printf("This is matrix A * 0.33\n");
    output(multy_k(A, 0.33));

    printf("This is det of matrix A\n");
    printf("%.2lf\n\n", det(A));

    printf("This is reverse of matrix A\n");
    output(reverse(A));
    printf("This is matrix A in degree 3\n");
    output(pow_mat(A, 3));

    printf("This is exp of Matrix A\n");
    output(exponent(A, 3));

    return 0;
}