#include "./matrix.h"

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
        throw std::runtime_error("Matrix dimensions do not match for addition.");
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
        throw std::runtime_error("Matrix dimensions do not match for subtraction.");
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
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            transposedElement[j][i] = element[i][j];
        }
    }
    
    return MatrixValue(transposedElement);
}