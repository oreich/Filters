#ifndef MATRIX_H
#define MATRIX_H



#include <iostream>
#include <fstream>
class Matrix {
private:
    int rows_;
    int col_;
    size_t size = rows_*col_;
    float* data = nullptr;
public:
    /**
     * deafult ctor and ctor
     */
    Matrix(int rows = 1, int cols = 1)  ;
    /**
     * copy ctor
     */
    Matrix(const Matrix &m) ;

    /**
     * dtor
     */
    ~Matrix();
    /**
     * getter
     * @return the amount of rows
     */

    int GetRows() const;
    /**
     * getter
     * @return the amount of cols
     */
    int GetCols() const;
    Matrix& Vectorize() noexcept;
    void Print() const noexcept;
    /**
     * assignment operator
     */

    Matrix& operator=(const Matrix &b) noexcept;
    /**
     * mult two matrix and return third matrix
     */

    Matrix operator*(const Matrix& b) const;
    /**
     * scalar mult on the right
     */

    Matrix operator*(const float& rhs) const noexcept;
    /**
     * scalar mult on the left
     */

    friend Matrix operator*(int c, const Matrix& m) noexcept;
    /**
     *  mult two matrix and change the first
     */

    Matrix &operator*=(const Matrix& b) ;
    /**
     *     mult scalar with the matrix and change the matrix
     */
    Matrix &operator*=(const float& c) noexcept;
    /**
     *     scalar division on the right need to check that the scalar is not 0
     */
    Matrix operator/(const float& c) const;
    /**
     *     scalar division need to check that the scalar is not 0
     */
    Matrix &operator/=(const float& c);
    /**
     *     matrix addition and return third matrix
     */
    Matrix operator+(const Matrix& b) const;
    /**
     *     matrix accumulation
     */
    Matrix &operator+=(const Matrix& b);
    /**
     *     matrix scalar accumulation adding c to each cell of the matrix
     */
    Matrix &operator+=(const float& c) noexcept;
    /**
     *     parenthesis indexing , check if the indexes are in the right range
     */
    float operator()(int i , int j) const;
    float &operator()(int i, int j)  noexcept(false);
    /**
     *     brackets indexing
     */
    float &operator[](size_t ind);
    /**
     *     brackets indexing
     */
    float operator[](size_t ind) const;
    /**
     *     equality
     * @param b
     * @return bool true if are equal false else
     */
    bool operator==(const Matrix& b) const noexcept;
    /**
     *     not equal
     */
    bool operator!=(const Matrix& b) const noexcept;
    /**
     *     input stream , fill matrix elements . read from given input stream
     */
    friend std::istream &operator>>(std::istream &is , const Matrix& m);
    /**
     *     output stream cout or write to a file
     */
    friend std::ostream &operator<<(std::ostream &os, const Matrix &m);


};
#endif //MATRIX_H