#include "Matrix.h"
#include <iostream>
#include "MatrixException.h"

using namespace std;


Matrix::Matrix(int rows, int cols) : rows_(rows), col_(cols) {
    std::cout << "ctor" << std::endl;
    size = rows * cols;
    data = new float[size];
    for (size_t i = 0; i < this->size; ++i) {
        data[i] = 0;
    }
}


Matrix::Matrix(const Matrix &m) :rows_(m.rows_) , col_(m.col_), size(m.size) {
    this->data = new float[m.size];
    for (std::size_t i = 0; i < m.size; ++i) {
        this->data[i] = m.data[i];
    }
}

/**
 * dtor
 */

Matrix::~Matrix() {
    delete[] this->data;
}

int Matrix::GetRows() const {
    return this->rows_;
}

int Matrix::GetCols() const {
    return this->col_;
}

Matrix &Matrix::Vectorize() noexcept {
    this->col_ = 1;
    this->rows_ = this->size;
    return *this;
}

void Matrix::Print() const noexcept {
    int counter = 0;
    for (size_t i = 0; i < this->size; ++i) {
        if (i != 0 && counter == this->col_ - 1) {
            std::cout << this->data[i];
            std::cout << std::endl;
            counter = 0;
        } else {
            std::cout << this->data[i] << " ";
            counter++;
        }

    }
}

Matrix &Matrix::operator=(const Matrix &other) noexcept {
    if (this != &other) {
        delete[] this->data;
        this->rows_ = other.rows_;
        this->col_ = other.col_;
        this->size = other.size;
        this->data = new float[size];
        for (size_t i = 0; i < other.size; ++i) {
            this->data[i] = other.data[i];

        }
    }
    return *this;
}

Matrix Matrix::operator*(const Matrix &b) const noexcept(false) {
    if (this->col_ == b.rows_) {
        Matrix result(this->rows_, b.col_);
        for (int i = 0; i < this->rows_; i++) {
            for (int j = 0; j < b.col_; j++) {
                for (int k = 0; k < this->col_; k++) {
                    result.data[i * result.col_ + j] += this->data[i * this->col_ + k] * b.data[k * b.col_ + j];
                }
            }
        }
        return result;
    }
    throw MatrixException("Invalid matrix dimensions.\n");

}

Matrix Matrix::operator*(const float &rhs) const noexcept {
    Matrix result = *this;
    for (std::size_t i = 0; i < this->size; ++i) {
        result.data[i] = this->data[i] * rhs;
    }
    return result;
}

Matrix operator*(int c, const Matrix &m) noexcept {
    Matrix result = m;
    for (size_t i = 0; i < m.size; ++i) {
        result.data[i] = m.data[i] * c;
    }
    return result;
}

Matrix &Matrix::operator*=(const Matrix &b) noexcept(false) {
    if (this->col_ == b.rows_) {
        *this = *this * b;
        return *this;
    }
    throw MatrixException("Invalid matrix dimensions.\n");
}

Matrix &Matrix::operator*=(const float &c) noexcept {
    *this = *this * c;
    return *this;
}

Matrix Matrix::operator/(const float &c) const noexcept(false) {
    if (c != 0) {
        Matrix result = *this;
        for (size_t i = 0; i < this->size; ++i) {
            result.data[i] = this->data[i] / c;
        }
        return result;
    }
    throw MatrixException("Division by zero.\n");
}

Matrix &Matrix::operator/=(const float &c) noexcept(false) {
    if (c != 0) {
        *this = *this / c;
        return *this;
    }
    throw MatrixException("Division by zero.\n");
}

Matrix Matrix::operator+(const Matrix &b) const noexcept(false) {
    if (this->rows_ == b.rows_ && this->col_ == b.col_) {
        Matrix result = *this;
        for (size_t i = 0; i < this->size; ++i) {
            result.data[i] = this->data[i] + b.data[i];
        }
        return result;
    }
    throw MatrixException("Invalid matrix dimensions.\n");
}

Matrix &Matrix::operator+=(const Matrix &b) noexcept(false) {
    if (this->rows_ == b.rows_ && this->col_ == b.col_) {
        *this = *this + b;
        return *this;
    }
    throw MatrixException("Invalid matrix dimensions.\n");
}

Matrix &Matrix::operator+=(const float &c) noexcept {
    for (size_t i = 0; i < this->size; ++i) {
        this->data[i] = this->data[i] + c;
    }
    return *this;
}

float Matrix::operator()(int i, int j) const noexcept(false) {
    if (i < this->rows_ && j < this->col_ ) {
        return this->data[i * this->col_ + j];
    }
    throw MatrixException("Index out of range.\n");
}

float &Matrix::operator()(int i, int j) noexcept(false) {
    if (i < this->rows_ && j < this->col_ ) {
        return this->data[i * this->col_ + j];
    }
    throw MatrixException("Index out of range.\n");
}

float Matrix::operator[](size_t ind) const noexcept(false) {
    if (ind < this->size) {
        return this->data[ind];
    }
    throw MatrixException("Index out of range.\n");
}

float &Matrix::operator[](size_t ind) noexcept(false) {
    if (ind < this->size) {
        return this->data[ind];
    }
    throw MatrixException("Index out of range.\n");
}

bool Matrix::operator==(const Matrix &b) const noexcept {
    if (b.data == this->data) return true;
    if (b.col_ != this->col_ || b.rows_ != this->rows_) return false;
    for (size_t i = 0; i < this->size; ++i) {
        if (this->data[i] != b.data[i]) {
            return false;
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix &b) const noexcept {
    for (size_t i = 0; i < this->size; ++i) {
        if (this->data[i] != b.data[i]) {
            return true;
        }
    }
    return false;
}

std::istream &operator>>(std::istream &is, const Matrix &m) noexcept(false) {
    if (is.good()) {
        float x;
        int i = 0;
        while (is >> x) {
             {
                m.data[i] = x;
                i++;
            }
        }
    }else {
        throw MatrixException("Error loading from input stream.\n");
    }
    return is;
}

std::ostream &operator<<(std::ostream &os, const Matrix &m) noexcept(false) {
    int counter = 0;
    for (size_t i = 0; i < m.size; ++i) {
        if (i != 0 && counter == m.col_ - 1) {
            os << m.data[i];
            os << std::endl;
            counter = 0;
        } else {
            os << m.data[i] << " ";
            counter++;
        }

    }
    return os;
}
