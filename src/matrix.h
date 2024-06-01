#ifndef MATRIX_H
#define MATRIX_H

#include "./value.h"
#include <vector>

class MatrixValue : public Value {
public:
    MatrixValue();
    ~MatrixValue();
    MatrixValue(int rows, int cols);
    explicit MatrixValue(std::vector<std::vector<double>> element);
    MatrixValue(const MatrixValue& other);
    bool isMatrix() const override { return true; }
    std::string toString() const override;
    std::vector<std::vector<double>> getElement() const { return element; }
    int getrows() override { return rows; }
    int getcols() override { return cols; }

    friend MatrixValue operator+(const MatrixValue& m1, const MatrixValue& m2);
    friend MatrixValue operator-(const MatrixValue& m1, const MatrixValue& m2);
    MatrixValue Transpose() const;
    
private:
    std::vector<std::vector<double>> element;
    int rows; // 行数
    int cols; // 列数
};

#endif