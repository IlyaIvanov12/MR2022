#include <iostream>
#include <cmath>


#define EPSILON 0.0001


class Matrix {
private:
    unsigned int rows;
    unsigned int cols;
public:
    unsigned int size;
    double **values;
    double *start;

    Matrix(unsigned int input_rows, unsigned int input_cols);
    Matrix(unsigned int input_rows, unsigned int input_cols, double number);  //  number filled matrix
    explicit Matrix(unsigned int row_num);  //  identity matrix
    ~Matrix();
    Matrix(Matrix const &matrix);
    void output() const;
    int filling(double const array[]) const;
    //Matrix operator = (double i);
    Matrix &operator = (Matrix const &matrix);
    Matrix operator + (Matrix matrix) const;
    Matrix operator + (double number) const;
    Matrix operator - (Matrix matrix) const;
    Matrix operator * (Matrix matrix) const;
    Matrix operator * (double number) const;
    Matrix minor_init(int crossed_row, int crossed_col) const;
    double determinant() const;
    Matrix transposition() const;
    static Matrix minor_transformation(Matrix matrix);
    Matrix inversion() const;
    Matrix operator / (Matrix matrix) const;
};


/*unsigned int len(double array[]) {
    printf("%lu\n", std::begin(array));
    printf("%lu\n", std::end(array));
}*/


Matrix::Matrix(unsigned int input_rows, unsigned int input_cols) {
    rows = input_rows;
    cols = input_cols;
    size = rows * cols;
    values = new double *[rows];
    start = new double [size];
    for (int row = 0; row < rows; row++)
        values[row] = start + row * cols;
}


Matrix::Matrix(unsigned int input_rows, unsigned int input_cols, double number) {
    rows = input_rows;
    cols = input_cols;
    size = rows * cols;
    values = new double *[rows];
    start = new double [size];
    for (int row = 0; row < rows; row++)
        values[row] = start + row * cols;
    for (int cell = 0; cell < size; cell++)
        start[cell] = number;
}


Matrix::Matrix(unsigned int row_num) {
    rows = row_num;
    cols = row_num;
    size = rows * cols;
    values = new double *[rows];
    start = new double [size];
    for (int row = 0; row < rows; row++)
        values[row] = start + row * cols;
    for (int row = 0; row < rows; row++)
        for (int col = 0; col < cols; col++)
            values[row][col] = (row == col) ? 1 : 0;
}


Matrix::~Matrix() {
    rows = 0;
    cols = 0;
    size = 0;
    delete values;
    delete start;
}


Matrix::Matrix(Matrix const &matrix) {
    rows = matrix.rows;
    cols = matrix.cols;
    size = rows * cols;
    values = new double *[rows];
    start = new double[size];
    for (int row = 0; row < rows; row++)
        values[row] = start + row * cols;
    for (int cell = 0; cell < size; cell++)
        start[cell] = matrix.start[cell];
}


void Matrix::output() const {

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++)
            std::cout << values[row][col] << "  ";
        std::cout << "\n";
    }
    std::cout << "\n";
}


int Matrix::filling(double const array[]) const {
    for (int cell = 0; cell < size; cell++)
        start[cell] = array[cell];
    return NAN;
}


/*Matrix Matrix::operator = (double i) {

}*/


Matrix &Matrix::operator = (Matrix const &matrix) {
    rows = matrix.rows;
    cols = matrix.cols;
    size = rows * cols;
    values = new double *[rows];
    start = new double[size];
    for (int row = 0; row < rows; row++)
        values[row] = start + row * cols;
    for (int cell = 0; cell < size; cell++)
        start[cell] = matrix.start[cell];
}


Matrix Matrix::operator + (Matrix const matrix) const {
    Matrix sum_matrix(2,2);
    for (int cell = 0; cell < size; cell++)
        sum_matrix.start[cell] = start[cell] + matrix.start[cell];
    return sum_matrix;
}


Matrix Matrix::operator + (double number) const {
    Matrix sum_matrix(2,2);
    for (int cell = 0; cell < size; cell++)
        sum_matrix.start[cell] = start[cell] + number;
    return sum_matrix;
}


Matrix Matrix::operator - (Matrix const matrix) const {
    Matrix sum_matrix(2,2);
    for (int cell = 0; cell < size; cell++)
        sum_matrix.start[cell] = start[cell] - matrix.start[cell];
    return sum_matrix;
}


Matrix Matrix::operator * (Matrix const matrix) const {
    Matrix multiplied_matrix(cols, matrix.rows);
    for(int row = 0; row < multiplied_matrix.rows; row++)
        for(int col = 0; col < multiplied_matrix.cols; col++) {
            multiplied_matrix.values[row][col] = 0;
            for (int k = 0; k < cols; k++)
                multiplied_matrix.values[row][col] += values[row][k] * matrix.values[k][col];
        }
    return multiplied_matrix;
}


Matrix Matrix::operator * (double number) const {
    Matrix operated_matrix(rows, cols);
    for(int cell = 0; cell < size; cell++)
        operated_matrix.start[cell] = start[cell] * number;
    return operated_matrix;
}


Matrix Matrix::minor_init(int crossed_row, int crossed_col) const {
    Matrix minor(rows-1, cols-1);
    int row_link = 0;
    for(int i = 0; i < minor.rows; i++) {
        if(crossed_row == i)
            row_link += 1;
        int col_link = 0;
        for (int j = 0; j < minor.cols; j++) {
            if(j == crossed_col)
                col_link += 1;
            minor.values[i][j] = values[i + row_link][j + col_link];
        }
    }
    return minor;
}


double Matrix::determinant() const{
    if(rows != cols) {
        return NAN;
    }
    double determinant = 0;
    if(rows == 1) {
        determinant = values[0][0];
        return determinant;
    }
    int k = 1;
    for(int col = 0; col < cols; col++) {
        Matrix minor = minor_init(0, col);
        determinant += k * values[0][col] * minor.determinant();
        k = -k;
    }
    return determinant;
}


Matrix Matrix::transposition() const {
    unsigned int new_rows = cols;
    unsigned int new_cols = rows;
    Matrix transposed_matrix(new_rows, new_cols);
    for(int row = 0; row < transposed_matrix.rows; row++)
        for(int col = 0; col < transposed_matrix.cols; col++)
            transposed_matrix.values[row][col] = values[col][row];
    return transposed_matrix;
}


Matrix Matrix::minor_transformation(Matrix matrix) { //////////////////////////////////////////////////////////////////////
    Matrix inverse_added_matrix(matrix.rows, matrix.cols);
    if(matrix.rows == 1) {
        inverse_added_matrix = matrix;
        return inverse_added_matrix;
    }
    for(int row = 0; row < matrix.rows; row++) {
        for(int col = 0; col < matrix.cols; col++) {
            Matrix minor = matrix.minor_init(row, col);
            int k = ((row + col) % 2 == 0) ? 1 : -1;
            inverse_added_matrix.values[row][col] = k * minor.determinant();
        }
    }
    return inverse_added_matrix;
}


Matrix Matrix::inversion() const {////////////////////////////////////////////////////////////////////////////////////////////////////
    double determinant = this->determinant();

    double inv_det = 1 / determinant;
    Matrix transformed_matrix = minor_transformation(this->transposition());
    Matrix inverse_matrix = transformed_matrix * inv_det;
    return inverse_matrix;
}


Matrix Matrix::operator / (Matrix matrix) const {///////////////////////////////////////////////////////////////////////////////////
    Matrix inverse_matrix = matrix.inversion();
    Matrix inverse_multiplied_matrix(cols, inverse_matrix.rows);
    for(int row = 0; row < inverse_multiplied_matrix.rows; row++)
        for(int col = 0; col < inverse_multiplied_matrix.cols; col++) {
            inverse_multiplied_matrix.values[row][col] = 0;
            for (int k = 0; k < cols; k++)
                inverse_multiplied_matrix.values[row][col] += values[row][k] * inverse_matrix.values[k][col];
        }
    return inverse_multiplied_matrix;
}


void calculation_check(double true_array[], Matrix matrix, std::string text) {
    int error_flag = 0;
    std::cout << text << " test:\n";
    for (int cell = 0; cell < matrix.size; cell++) {
        int cell_flag = 0;
        if (std::abs(true_array[cell] - matrix.start[cell]) > EPSILON) {
            cell_flag = 1;
            error_flag += 1;
        }
        std::cout << "true: " << true_array[cell] << " res: " << matrix.start[cell] << " | " << cell_flag << "\n";
    }
    std::cout << "Errors: " << error_flag << "\n";
    std::cout << "\n";
}


void test() {
    Matrix res_matrix(2, 2, NAN);

    Matrix matrix1(2,2);
    double array1[] = {2, 8, 1, 3};
    matrix1.filling(array1);

    Matrix matrix2(2,2);
    double array2[] = {4, 9, 21, 13};
    matrix2.filling(array2);

    //  = overload test
    res_matrix = matrix1;
    double eq_true_array[] = {2, 8, 1, 3};
    calculation_check(eq_true_array, res_matrix, "= overload");

    //  addition test (matrix)
    res_matrix = matrix1 + matrix2;
    double sum_true_array[] = {6, 17, 22, 16};
    calculation_check(sum_true_array, res_matrix, "addition (matrix)");

    //  addition test (number)
    res_matrix = matrix1 + 2;
    double snum_true_array[] = {4, 10, 3, 5};
    calculation_check(snum_true_array, res_matrix, "addition (number)");

    //  subtraction test
    res_matrix = matrix1 - matrix2;
    double sub_true_array[] = {-2, -1, -20, -10};
    calculation_check(sub_true_array, res_matrix, "subtraction");

    //  multiplication test
    res_matrix = matrix1 * matrix2;
    double multi_true_array[] = {176, 122, 67, 48};
    calculation_check(multi_true_array, res_matrix, "multiplication (matrix)");

    //  multiplication test (number)
    res_matrix = matrix1 * 2;
    double mnum_true_array[] = {4, 16, 2, 6};
    calculation_check(mnum_true_array, res_matrix, "multiplication (number)");

    //  determinant test
    double determinant_true_array[] = {-2};

    double determinant = matrix1.determinant();
    Matrix det_matrix(1, 1, determinant);
    calculation_check(determinant_true_array, det_matrix, "determinant");

    //  transposition test
    double transp_true_array[] = {2, 1, 8, 3};
    res_matrix = matrix1.transposition();
    calculation_check(transp_true_array, res_matrix, "transposition");

    //  inversion test
    double inv_true_array[] = {-1.5, 4, 0.5, -1};
    res_matrix = matrix1.inversion();
    calculation_check(inv_true_array, res_matrix, "inversion");

    //  inverse multiplication test
    double inv_multi_true_array[] = {1.0365, -0.1022, 0.3650, -0.0219};
    res_matrix = matrix1 / matrix2;
    calculation_check(inv_multi_true_array, res_matrix, "inverse multiplication");

}


int main() {
    test();
    return 0;
}