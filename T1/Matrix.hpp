#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "Vector.hpp"

class Matrix {
private:
    int mNumRows;    // Số hàng
    int mNumCols;    // Số cột
    double** mData;  // Con trỏ trỏ đến mảng các con trỏ (mảng 2 chiều)

public:
    // Các Constructor và Destructor
    Matrix(int numRows, int numCols);
    Matrix(const Matrix& other);
    ~Matrix();

    // Các phương thức public lấy số hàng, số cột
    int GetNumRows() const;
    int GetNumCols() const;

    // Nạp chồng toán tử ngoặc tròn () dùng chỉ số từ 1 (1-based indexing)
    double& operator()(int i, int j);
    const double& operator()(int i, int j) const;

    // Nạp chồng toán tử gán
    Matrix& operator=(const Matrix& other);

    // Nạp chồng toán tử một ngôi (Unary operators)
    Matrix operator+() const;
    Matrix operator-() const;

    // Nạp chồng toán tử hai ngôi giữa Ma trận với Ma trận
    Matrix operator+(const Matrix& m) const;
    Matrix operator-(const Matrix& m) const;
    
    // Nạp chồng toán tử nhân
    Matrix operator*(double scalar) const;       // Ma trận * Số vô hướng
    Matrix operator*(const Matrix& m) const;     // Ma trận * Ma trận
    Vector operator*(const Vector& v) const;     // Ma trận * Vector

    // Các phương thức tính toán nâng cao
    Matrix Transpose() const;                    // Ma trận chuyển vị
    double Determinant() const;                  // Tính định thức
    Matrix Inverse() const;                      // Ma trận nghịch đảo
    Matrix PseudoInverse() const;                // Giả nghịch đảo Moore-Penrose
};

// Hàm toán tử ngoài để hỗ trợ phép nhân: Số vô hướng * Ma trận
Matrix operator*(double scalar, const Matrix& m);

#endif // MATRIX_HPP