#include "Matrix.h"
#include <cmath>

#define MAT_SIZE 256
#define BLURR_FACTOR 16
#define SIDES 1
#define MIDDLE 2
#define BLURR_ROW_SIZE 3
#define BLURR_COL_SIZE 3
#define BLURR_SIZE BLURR_COL_SIZE*BLURR_ROW_SIZE
#define CENTER floor(BLURR_SIZE/2)
#define BLUR_COL_CENTER 1
#define BLUR_ROW_CENTER 1
#define CENTER_VAL 4
#define FIRST 0
#define SECOND 1
#define THIRD 2
#define FOURTH 3
#define FIFTH 4
#define SIXTH 5
#define SEVENTH 6
#define EIGHTH 7
#define NINETH 8
#define SOBEL_FACTOR 8



void CreatScal(int levels, int arr[]) {
    int range_of_colors = MAT_SIZE / levels;
    const int range = range_of_colors;
    int start_range = 0;
    for (int i = 0; i < levels; ++i) {
        arr[i] = floor((range_of_colors - 1 + start_range) / 2);
        start_range = range_of_colors;
        range_of_colors += range;
    }
}

Matrix MakeQuantiMatrix(const Matrix &img, const int arr[], size_t arr_size) {
    Matrix quanti_mat = img;
    int ind = 0;
    int img_size = img.GetCols() * img.GetRows();
    int lim = (MAT_SIZE / arr_size) - 1;
    for (int i = 0; i < img_size; ++i) {
        for (size_t j = 0; j < arr_size; ++j) {
                        if (quanti_mat[i] <= lim) {
                quanti_mat[i] = (float) arr[ind];
            } else {
                lim += MAT_SIZE / arr_size;
                ind++;
            }
            if (quanti_mat[i] >= MAT_SIZE) {
                quanti_mat[i] = MAT_SIZE - 1;
            }
        }
    }
    delete [] arr;
    return quanti_mat;
}

Matrix Quantization(const Matrix &image, int levels) {

    int* arr = new int[levels];
    CreatScal(levels, arr);
    return MakeQuantiMatrix(image, arr, levels);
}

Matrix CreatBlurrMat() {
    Matrix blurr_mat(BLURR_ROW_SIZE, BLURR_COL_SIZE);
    for (int i = 0; i < BLURR_ROW_SIZE * BLURR_COL_SIZE; ++i) {
        if (i % 2 != 0) {
            blurr_mat[i] = MIDDLE;
        } else if (i == CENTER) {
            blurr_mat[i] = CENTER_VAL;
        } else {
            blurr_mat[i] = SIDES;
        }
    }
    blurr_mat /= BLURR_FACTOR;
    return blurr_mat;
}

void DoConvilation(const Matrix &in, Matrix &result, Matrix &blur_mat) {
    for (int i = 0; i < result.GetRows(); ++i)                // rows
    {
        for (int j = 0; j < result.GetCols(); ++j)            // columns
        {
            float sum = 0;                            // init to 0 before sum
            for (int m = 0; m < blur_mat.GetRows(); ++m)      // blur_mat rows
            {
                int mm = blur_mat.GetRows() - 1 - m;       // row index of flipped blur_mat
                for (int n = 0; n < blur_mat.GetCols(); ++n)  // blur_mat columns
                {
                    int nn = blur_mat.GetCols() - 1 - n;   // column index of flipped blur_mat
                    // index of input signal, used for checking boundary
                    int row_index = i + (BLUR_ROW_CENTER - mm);
                    int col_index = j + (BLUR_COL_CENTER - nn);
                    // ignore input samples which are result of bound
                    if (row_index >= 0 && row_index < result.GetRows() && col_index >= 0 &&
                        col_index < result.GetCols())
                        sum += in[result.GetCols() * row_index + col_index] * blur_mat[blur_mat.GetCols() * mm + nn];
                }
            }
            result[result.GetCols() * i + j] = (sum);
        }
    }
}

Matrix Blur(const Matrix &image) {
    Matrix blurr_mat = CreatBlurrMat();
    Matrix result_mat = image;
    DoConvilation(image, result_mat, blurr_mat);
    int size = result_mat.GetRows()*result_mat.GetCols();
    for (int i = 0; i < size; ++i) {
        result_mat[i] = rintf(result_mat[i]);
    }
    return result_mat;
}

Matrix CreatSobMatA() {
    Matrix a(BLURR_ROW_SIZE, BLURR_ROW_SIZE);
    for (int i = 1; i < BLURR_SIZE; i += 3) {
        a[i] = 0;
    }
    a[THIRD] = 1;
    a[SIXTH] = 2;
    a[NINETH] = 1;
    a[FIRST] = -1;
    a[FOURTH] = -2;
    a[SEVENTH] = -1;
    return a /= SOBEL_FACTOR;
}

Matrix CreatSobMatB() {
    Matrix a(BLURR_ROW_SIZE, BLURR_ROW_SIZE);
    for (int i = FOURTH; i < SEVENTH; i++) {
        a[i] = 0;
    }
    a[SEVENTH] = 1;
    a[EIGHTH] = 2;
    a[NINETH] = 1;
    a[FIRST] = -1;
    a[SECOND] = -2;
    a[THIRD] = -1;
    return a /= SOBEL_FACTOR;
}

Matrix Sobel(const Matrix &image) {
    Matrix a = CreatSobMatA();
    Matrix result_mat_a = image;
    Matrix b = CreatSobMatB();
    Matrix result_mat_b = image;
    DoConvilation(image, result_mat_a, a);
    DoConvilation(image, result_mat_b, b);
    int size = image.GetCols()*image.GetRows();
    Matrix result_mat_c =  result_mat_a + result_mat_b;
    for (int i = 0; i < size; ++i) {
        if(result_mat_c[i] < 0){
            result_mat_c[i] = 0;
        } else if(result_mat_c[i] > MAT_SIZE -1){
            result_mat_c[i] = MAT_SIZE -1;
        }
        result_mat_c[i] = rintf(result_mat_c[i]);
    }
    return result_mat_c;
}

