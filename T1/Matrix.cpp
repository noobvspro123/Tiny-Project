#include "Matrix.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

// Constructor khởi tạo kích thước và gán toàn bộ bằng 0.0
Matrix::Matrix(int numRows, int numCols) {
    assert(numRows > 0 && numCols > 0);
    mNumRows = numRows;
    mNumCols = numCols;

    // Cấp phát động mảng các con trỏ dòng
    mData = new double*[mNumRows];
    for (int i = 0; i < mNumRows; ++i) {
        mData[i] = new double[mNumCols];
        for (int j = 0; j < mNumCols; ++j) {
            mData[i][j] = 0.0;
        }
    }
}

// Copy constructor (Deep copy cho mảng 2 chiều)
Matrix::Matrix(const Matrix& other) {
    mNumRows = other.mNumRows;
    mNumCols = other.mNumCols;

    mData = new double*[mNumRows];
    for (int i = 0; i < mNumRows; ++i) {
        mData[i] = new double[mNumCols];
        for (int j = 0; j < mNumCols; ++j) {
            mData[i][j] = other.mData[i][j];
        }
    }
}

// Destructor giải phóng bộ nhớ theo từng dòng trước khi xóa con trỏ quản lý
Matrix::~Matrix() {
    for (int i = 0; i < mNumRows; ++i) {
        delete[] mData[i];
    }
    delete[] mData;
}

int Matrix::GetNumRows() const { return mNumRows; }
int Matrix::GetNumCols() const { return mNumCols; }

// Toán tử () truy xuất phần tử bắt đầu từ 1 (1-based indexing)
double& Matrix::operator()(int i, int j) {
    assert(i >= 1 && i <= mNumRows);
    assert(j >= 1 && j <= mNumCols);
    return mData[i - 1][j - 1];
}

const double& Matrix::operator()(int i, int j) const {
    assert(i >= 1 && i <= mNumRows);
    assert(j >= 1 && j <= mNumCols);
    return mData[i - 1][j - 1];
}

// Toán tử gán (=)
Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        // Giải phóng bộ nhớ ma trận cũ
        for (int i = 0; i < mNumRows; ++i) {
            delete[] mData[i];
        }
        delete[] mData;

        // Cấp phát và sao chép dữ liệu mới
        mNumRows = other.mNumRows;
        mNumCols = other.mNumCols;
        mData = new double*[mNumRows];
        for (int i = 0; i < mNumRows; ++i) {
            mData[i] = new double[mNumCols];
            for (int j = 0; j < mNumCols; ++j) {
                mData[i][j] = other.mData[i][j];
            }
        }
    }
    return *this;
}

// Toán tử một ngôi Unary (+, -)
Matrix Matrix::operator+() const { return *this; }

Matrix Matrix::operator-() const {
    Matrix result(mNumRows, mNumCols);
    for (int i = 1; i <= mNumRows; ++i) {
        for (int j = 1; j <= mNumCols; ++j) {
            result(i, j) = -(*this)(i, j);
        }
    }
    return result;
}

// Phép cộng hai ma trận cùng kích thước
Matrix Matrix::operator+(const Matrix& m) const {
    assert(mNumRows == m.mNumRows && mNumCols == m.mNumCols);
    Matrix result(mNumRows, mNumCols);
    for (int i = 1; i <= mNumRows; ++i) {
        for (int j = 1; j <= mNumCols; ++j) {
            result(i, j) = (*this)(i, j) + m(i, j);
        }
    }
    return result;
}

// Phép trừ hai ma trận cùng kích thước
Matrix Matrix::operator-(const Matrix& m) const {
    assert(mNumRows == m.mNumRows && mNumCols == m.mNumCols);
    Matrix result(mNumRows, mNumCols);
    for (int i = 1; i <= mNumRows; ++i) {
        for (int j = 1; j <= mNumCols; ++j) {
            result(i, j) = (*this)(i, j) - m(i, j);
        }
    }
    return result;
}

// Phép nhân Ma trận với Số vô hướng
Matrix Matrix::operator*(double scalar) const {
    Matrix result(mNumRows, mNumCols);
    for (int i = 1; i <= mNumRows; ++i) {
        for (int j = 1; j <= mNumCols; ++j) {
            result(i, j) = (*this)(i, j) * scalar;
        }
    }
    return result;
}

Matrix operator*(double scalar, const Matrix& m) {
    return m * scalar;
}

// Phép nhân Ma trận với Ma trận: C(i, j) = tổng_k (A(i, k) * B(k, j))
Matrix Matrix::operator*(const Matrix& m) const {
    assert(mNumCols == m.mNumRows); // Số cột ma trận trước phải bằng số hàng ma trận sau
    Matrix result(mNumRows, m.mNumCols);
    for (int i = 1; i <= mNumRows; ++i) {
        for (int j = 1; j <= m.mNumCols; ++j) {
            double sum = 0.0;
            for (int k = 1; k <= mNumCols; ++k) {
                sum += (*this)(i, k) * m(k, j);
            }
            result(i, j) = sum;
        }
    }
    return result;
}

// Phép nhân Ma trận với Vector: kết quả trả về một Vector mới
Vector Matrix::operator*(const Vector& v) const {
    assert(mNumCols == v.GetSize());
    Vector result(mNumRows);
    for (int i = 1; i <= mNumRows; ++i) {
        double sum = 0.0;
        for (int j = 1; j <= mNumCols; ++j) {
            sum += (*this)(i, j) * v(j); // Dùng toán tử () bắt đầu từ 1 của lớp Vector
        }
        result(i) = sum;
    }
    return result;
}

// Hàm tính Ma trận chuyển vị (Transpose)
Matrix Matrix::Transpose() const {
    Matrix result(mNumCols, mNumRows);
    for (int i = 1; i <= mNumRows; ++i) {
        for (int j = 1; j <= mNumCols; ++j) {
            result(j, i) = (*this)(i, j);
        }
    }
    return result;
}

// Tính định thức bằng phương pháp khử Gauss đưa về ma trận tam giác trên
double Matrix::Determinant() const {
    assert(mNumRows == mNumCols); // Bắt buộc phải là ma trận vuông
    int n = mNumRows;
    Matrix temp(*this);
    double det = 1.0;

    for (int i = 1; i <= n; ++i) {
        // Tìm phần tử trội (pivoting) trên cột i từ hàng i trở xuống
        int pivot = i;
        for (int j = i + 1; j <= n; ++j) {
            if (std::abs(temp(j, i)) > std::abs(temp(pivot, i))) {
                pivot = j;
            }
        }
        // Nếu phần tử trội xấp xỉ bằng 0 thì định thức bằng 0
        if (std::abs(temp(pivot, i)) < 1e-9) return 0.0;

        // Đổi chỗ hàng i và hàng pivot nếu cần thiết
        if (pivot != i) {
            for (int k = 1; k <= n; ++k) {
                std::swap(temp(i, k), temp(pivot, k));
            }
            det *= -1.0; // Đổi chỗ 2 hàng thì định thức đổi dấu
        }

        det *= temp(i, i);

        // Khử các phần tử phía dưới cột i
        for (int j = i + 1; j <= n; ++j) {
            double factor = temp(j, i) / temp(i, i);
            for (int k = i; k <= n; ++k) {
                temp(j, k) -= factor * temp(i, k);
            }
        }
    }
    return det;
}

// Tìm ma trận nghịch đảo bằng phương pháp Gauss-Jordan biến đổi cùng ma trận đơn vị
Matrix Matrix::Inverse() const {
    assert(mNumRows == mNumCols);
    int n = mNumRows;
    Matrix temp(*this);
    Matrix inv(n, n);
    
    // Khởi tạo inv là ma trận đơn vị
    for (int i = 1; i <= n; ++i) inv(i, i) = 1.0;

    for (int i = 1; i <= n; ++i) {
        int pivot = i;
        for (int j = i + 1; j <= n; ++j) {
            if (std::abs(temp(j, i)) > std::abs(temp(pivot, i))) {
                pivot = j;
            }
        }
        assert(std::abs(temp(pivot, i)) > 1e-9); // Đảm bảo ma trận không suy biến

        if (pivot != i) {
            for (int k = 1; k <= n; ++k) {
                std::swap(temp(i, k), temp(pivot, k));
                std::swap(inv(i, k), inv(pivot, k));
            }
        }

        // Chuẩn hóa hàng i về có phần tử đầu cột bằng 1
        double divisor = temp(i, i);
        for (int k = 1; k <= n; ++k) {
            temp(i, k) /= divisor;
            inv(i, k) /= divisor;
        }

        // Khử toàn bộ các phần tử khác trên cột i (cả trên lẫn dưới hàng i)
        for (int j = 1; j <= n; ++j) {
            if (j != i) {
                double factor = temp(j, i);
                for (int k = 1; k <= n; ++k) {
                    temp(j, k) -= factor * temp(i, k);
                    inv(j, k) -= factor * inv(i, k);
                }
            }
        }
    }
    return inv;
}

// Tính Giả nghịch đảo Moore-Penrose phục vụ cho hệ không vuông ở Part B
// Công thức áp dụng cho ma trận đầy đủ hạng cột: A+ = (A^T * A)^-1 * A^T
Matrix Matrix::PseudoInverse() const {
    Matrix AT = this->Transpose();
    Matrix ATA = AT * (*this);
    return ATA.Inverse() * AT;
}