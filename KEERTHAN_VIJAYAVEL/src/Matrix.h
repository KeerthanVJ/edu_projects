// Matrix.h

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cstdint>
#include "Vector.h"

class Matrix {

protected:
    Vector<Vector<uint8_t>> data;

private:
    int rows_count;
    int cols_count;

public:
    // Default constructor
    Matrix();

    // Constructor with rows and columns
    Matrix(int row, int column);

    // Copy constructor
    Matrix(const Matrix& src);

    // Assignment operator
    Matrix& operator=(const Matrix& src);

    // Destructor
    ~Matrix();

    // Input stream operator
    friend std::istream& operator>>(std::istream& is, Matrix& m);

    // Output stream operator
    friend std::ostream& operator<<(std::ostream& os, const Matrix& m);

    // Arithmetic operators
    Matrix operator+(const Matrix& rhs) const;
    Matrix operator-(const Matrix& rhs) const;
    Matrix operator*(const Matrix& rhs) const;

    // Subscript operator
    Vector<uint8_t>& operator[](int index);
    const Vector<uint8_t>& operator[](int index) const;

    // Number of rows
    int getRows() const;

    // Number of columns
    int getCols() const;

    // Transpose function (in-place)
    void transpose();
};

#endif // MATRIX_H
