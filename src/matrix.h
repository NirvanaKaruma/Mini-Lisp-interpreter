#ifndef MATRIX_H
#define MATRIX_H

#include "./value.h"
#include "./rational.h"
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
    int getrows() const override { return rows; }
    int getcols() const override { return cols; }

    friend MatrixValue operator+(const MatrixValue& m1, const MatrixValue& m2);
    friend MatrixValue operator-(const MatrixValue& m1, const MatrixValue& m2);
    friend MatrixValue operator*(const MatrixValue& m1, const MatrixValue& m2);
    friend MatrixValue operator*(const MatrixValue& m1, double m2);
    friend MatrixValue operator*(double m1, const MatrixValue& m2);
    bool operator==(const MatrixValue& other) const;
    MatrixValue Transpose() const; //求矩阵转置
    friend MatrixValue IdentityMatrix(int n); //定义在builtins.cpp中
    friend MatrixValue Identity(int n); //求单位阵
    friend MatrixValue elementWiseMultiply(const MatrixValue& m1, const MatrixValue& m2); //求矩阵对应位置元素相乘
    double trace() const; //求矩阵迹
    MatrixValue sub(int row, int col); //求矩阵余矩阵
    double det(); //求矩阵行列式
    int rank(); //求矩阵秩
    MatrixValue toUpperTriangularForm() const; //求矩阵上三角矩阵
    MatrixValue inverse(); //求矩阵逆
    
private:
    std::vector<std::vector<double>> element;
    int rows; // 行数
    int cols; // 列数
};

#endif