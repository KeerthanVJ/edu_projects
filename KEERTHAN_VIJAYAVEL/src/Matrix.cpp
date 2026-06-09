// Matrix.cpp
//Keerthan Vijayavel

#include "Matrix.h"
#include <stdexcept>
#include <algorithm>

// Default constructor
Matrix::Matrix() : rows_count(0), cols_count(0) {}

// Constructor for rows and columns
Matrix::Matrix(int ro, int co) : rows_count(ro), cols_count(co) {
    data = Vector<Vector<uint8_t>>(ro);
    for (int i = 0; i < ro; ++i) {
        data[i] = Vector<uint8_t>(co);
        for (int j = 0; j < co; ++j)
            data[i][j] = 0;
    }
}


// Assignment operator
Matrix& Matrix::operator=(const Matrix& other) {
    if (this == &other) return *this;
    
    rows_count = other.rows_count;
    cols_count = other.cols_count;
    data = Vector<Vector<uint8_t>>(rows_count);
    
    for (int i = 0; i < rows_count; ++i) {
        data[i] = Vector<uint8_t>(cols_count);
        for (int j = 0; j < cols_count; ++j)
            data[i][j] = other.data[i][j];
    }
    
    return *this;
}

// Copy constructor
Matrix::Matrix(const Matrix& other) : rows_count(other.rows_count), cols_count(other.cols_count) {
    data = Vector<Vector<uint8_t>>(rows_count);
    for (int i = 0; i < rows_count; ++i) {
        data[i] = Vector<uint8_t>(cols_count);
        for (int j = 0; j < cols_count; ++j)
            data[i][j] = other.data[i][j];
    }
}

// Destructor
Matrix::~Matrix() {}

// rows count
int Matrix::getRows() const { return rows_count; }

//columns count
int Matrix::getCols() const { return cols_count; }

// Input stream operator
std::istream& operator>>(std::istream& is, Matrix& m) {
    int r, c;
    is >> r >> c;
    
    m = Matrix(r, c);
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            int v;
            is >> v;
            m[i][j] = static_cast<uint8_t>(v);
        }
    }
    
    return is;
}

// Output stream operator
std::ostream& operator<<(std::ostream& os, const Matrix& m) {
    os << m.getRows() << " " << m.getCols() << "\n";
    
    int rows = m.getRows();
    int cols = m.getCols();
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            os << (int)m[i][j] << " ";
        os << "\n";
    }
    
    return os;
}

// Addition
Matrix Matrix::operator+(const Matrix& ope) const {
    if (rows_count != ope.rows_count || cols_count != ope.cols_count)
        throw std::invalid_argument("Dimensions do not match for +");
    
    Matrix res(rows_count, cols_count);
    for (int i = 0; i < rows_count; i++) {
        for (int j = 0; j < cols_count; j++) {
            res[i][j] = std::min(255, (int)data[i][j] + (int)ope.data[i][j]);
        }
    }
    
    return res;
}
//subtraction
Matrix Matrix::operator-(const Matrix& ope) const {
    if (rows_count != ope.rows_count || cols_count != ope.cols_count)
        throw std::invalid_argument("Dimensions do not match for -");
    
    Matrix res(rows_count, cols_count);
    for (int i = 0; i < rows_count; i++) {
        for (int j = 0; j < cols_count; j++) {
            int d = (int)data[i][j] - (int)ope.data[i][j];
            res[i][j] = d < 0 ? 0 : d;
        }
    }
    
    return res;
}
//multiplication
Matrix Matrix::operator*(const Matrix& ope) const {
    if (rows_count != ope.rows_count || cols_count != ope.cols_count)
        throw std::invalid_argument("Dimensions do not match for *");
    
    Matrix res(rows_count, cols_count);
    
    for (int i = 0; i < rows_count; i++) {
        for (int j = 0; j < cols_count; j++) {
            int answer = ((int)data[i][j] * (int)ope.data[i][j]) / 255;
            res[i][j] = answer > 255 ? 255 : answer;
        }
    }
    
    return res;
}

// Subscript operator
Vector<uint8_t>& Matrix::operator[](int index) {
    if (index < 0 || index >= rows_count)
        throw std::out_of_range("Index for the row is out of range.");
    return data[index];
}

const Vector<uint8_t>& Matrix::operator[](int index) const {
    if (index < 0 || index >= rows_count)
        throw std::out_of_range("Index for the row is out of range");
    return data[index];
}

// Transpose extra credit
void Matrix::transpose() {
    Matrix t(cols_count, rows_count);
    
    for (int i = 0; i < rows_count; i++)
        for (int j = 0; j < cols_count; j++)
            t[j][i] = data[i][j];
    
    *this = t;
}
