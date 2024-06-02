#include "./matrix.h"
#include "./error.h"

MatrixValue::MatrixValue() : rows(0) , cols(0) {}

MatrixValue::MatrixValue(int rows, int cols) : rows(rows), cols(cols) {
    element.resize(rows);
    for (int i = 0; i < rows; i++) {
        element[i].resize(cols, 0.0);
    }
}

MatrixValue::MatrixValue(std::vector<std::vector<double>> element) : element(element) {
    this->rows = element.size();
    this->cols = element[0].size();
}

MatrixValue::MatrixValue(const MatrixValue& other)
    : rows(other.rows), cols(other.cols), element(other.rows, std::vector<double>(other.cols))
{
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            element[i][j] = other.element[i][j];
        }
    }
}

MatrixValue::~MatrixValue() {
    element.clear();
}

std::string MatrixValue::toString() const {
    std::string str = "[\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            str += std::to_string(element[i][j]) + " ";
        }
        str += "\n";
    }
    str += "]";
    return str;
}

MatrixValue operator+(const MatrixValue& m1, const MatrixValue& m2) {
    if (m1.rows != m2.rows || m1.cols != m2.cols) {
        throw MathError("Matrix dimensions do not match for addition.");
    }

    MatrixValue result(m1.rows, m1.cols);
    for (int i = 0; i < m1.rows; ++i) {
        for (int j = 0; j < m1.cols; ++j) {
            result.element[i][j] = m1.element[i][j] + m2.element[i][j];
        }
    }
    return result;
}

MatrixValue operator-(const MatrixValue& m1, const MatrixValue& m2) {
    if (m1.rows != m2.rows || m1.cols != m2.cols) {
        throw MathError("Matrix dimensions do not match for subtraction.");
    }

    MatrixValue result(m1.rows, m1.cols);
    for (int i = 0; i < m1.rows; ++i) {
        for (int j = 0; j < m1.cols; ++j) {
            result.element[i][j] = m1.element[i][j] - m2.element[i][j];
        }
    }
    return result;
}

MatrixValue MatrixValue::Transpose() const {
    std::vector<std::vector<double>> transposedElement(cols, std::vector<double>(rows));
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            transposedElement[j][i] = element[i][j];
        }
    }
    
    return MatrixValue(transposedElement);
}

MatrixValue operator*(const MatrixValue& m1, const MatrixValue& m2) {
    if (m1.cols != m2.rows) {
        throw MathError("Matrix dimensions do not match for multiplication.");
    }
    MatrixValue result(m1.rows, m2.cols);
    for(int i = 0; i < m1.rows; i++){
        for(int j = 0; j < m2.cols; j++){
            double sum = 0;
            for(int k = 0; k < m1.cols; k++){
                sum += m1.element[i][k] * m2.element[k][j];
            }
            result.element[i][j] = sum;
        }
    }
    return result;
}

MatrixValue operator*(const MatrixValue& m1, double m2){
    MatrixValue result(m1.rows, m1.cols);
    for(int i = 0; i < m1.rows; i++){
        for(int j = 0; j < m1.cols; j++){
            result.element[i][j] = m1.element[i][j] * m2;
        }
    }
    return result;
}

MatrixValue operator*(double m1, const MatrixValue& m2){
    return m2 * m1;
}

bool MatrixValue::operator==(const MatrixValue& other) const {
    if (rows != other.rows || cols != other.cols) {
        return false;
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (std::abs(element[i][j] - other.element[i][j]) > 1e-9) {
                return false;
            }
        }
    }

    return true;
}

MatrixValue Identity(int n){
    if (n <= 0) {
        throw MathError("Matrix dimensions must be positive.");
    }

    MatrixValue result(n, n);
    for (int i = 0; i < n; i++) {
        result.element[i][i] = 1;
    }
    return result;
}

MatrixValue elementWiseMultiply(const MatrixValue& m1, const MatrixValue& m2){
    if (m1.rows != m2.rows || m1.cols != m2.cols) {
        throw MathError("Matrices must have the same dimensions for element-wise multiplication.");
    }
    std::vector<std::vector<double>> result(m1.rows, std::vector<double>(m1.cols, 0.0));
    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            result[i][j] = m1.element[i][j] * m2.element[i][j];
        }
    }

    return MatrixValue(result);
}

double MatrixValue::trace() const {
    if (rows != cols) {
        throw MathError("Matrix must be square for trace.");
    }

    double sum = 0.0;
    for (int i = 0; i < rows; i++) {
        sum += element[i][i];
    }
    return sum;
}

MatrixValue MatrixValue::sub(int row, int col){
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw MathError("Invalid row or column index.");
    }
    int subRows = rows - 1;
    int subCols = cols - 1;

    MatrixValue subMatrix(subRows, subCols);

    int subRow = -1;
    int subCol = -1;
    for(int i = 0; i < rows; i++){
        if(i != row){
            subRow++;
            for(int j = 0; j < cols; j++){
                if(j != col){
                    subCol++;
                    subMatrix.element[subRow][subCol] = element[i][j];
                    subCol--;
                }
            }
            subRow--;
        }
    }
    return subMatrix;
}

double MatrixValue::det() {
    if (rows != cols) {
        throw MathError("Matrix must be square for determinant.");
    }
    if(rows == 1 && cols == 1) return element[0][0];
    else {
        double value = 0;
        for(int j = 0; j < cols; j++){
            value += pow(-1,j+2) * element[0][j] * (*this).sub(0,j).det();
        }
        return value;
    }
}

int MatrixValue::rank(){
    int rankValue = std::min(rows, cols);
    MatrixValue augmentedMatrix(*this); // 创建一个增广矩阵，用于进行高斯消元
    for (int i = 0; i < rankValue; i++) {
        if (std::abs(augmentedMatrix.element[i][i]) < 1e-8) { // 检查主元是否接近于零
            rankValue = i;
            break;
        }
        // 将当前行作为主元行，消除下方所有行的对应列元素
        for (int j = i + 1; j < augmentedMatrix.rows; j++) {
            double factor = augmentedMatrix.element[j][i] / augmentedMatrix.element[i][i];
            for (int k = i; k < augmentedMatrix.cols; k++) {
                augmentedMatrix.element[j][k] -= factor * augmentedMatrix.element[i][k];
            }
        }
    }
    return rankValue;
}

MatrixValue MatrixValue::toUpperTriangularForm() const {
    MatrixValue upper(*this);
    for (int i = 0; i < std::min(rows, cols) - 1; i++) {
        if (std::abs(upper.element[i][i]) < 1e-8) {
            throw MathError("Matrix is singular and cannot be transformed to an upper triangular matrix.");
        }
        
        for (int j = i + 1; j < rows; j++) {
            double factor = upper.element[j][i] / upper.element[i][i];
            for (int k = i; k < cols; k++) {
                upper.element[j][k] -= factor * upper.element[i][k];
            }
        }
    }

    return upper;
}

MatrixValue MatrixValue::inverse() {
    if (rows != cols) {
        throw MathError("Matrix must be square for inverse.");
    }
    int det = this->det();
    if (det == 0) {
        throw MathError("Matrix is singular and cannot be inverted.");
    }
    MatrixValue inverse(rows, cols);
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            inverse.element[i][j] = pow(-1, i+j+2) * (*this).sub(j,i).det() / det;
        }
    }
    return inverse;
}