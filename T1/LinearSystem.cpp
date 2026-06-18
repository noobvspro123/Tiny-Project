#include "LinearSystem.hpp"
#include <iostream>
#include <cmath>
#include <cassert>

// ================= LỚP LINEAR SYSTEM =================

// Constructor: Khởi tạo hệ phương trình và tự động lấy kích thước [cite: 31]
LinearSystem::LinearSystem(const Matrix& A, const Vector& b) {
    assert(A.GetNumRows() == A.GetNumCols()); // A phải là ma trận vuông
    assert(A.GetNumRows() == b.GetSize());    // A và b phải tương thích kích thước [cite: 29]
    
    mSize = A.GetNumRows();
    mpA = new Matrix(A);
    mpb = new Vector(b);
}

// Destructor
LinearSystem::~LinearSystem() {
    delete mpA;
    delete mpb;
}

// Thuật toán khử Gauss với Pivoting 
Vector LinearSystem::Solve() {
    Matrix A(*mpA); // Tạo bản sao để không làm hỏng dữ liệu gốc
    Vector b(*mpb);
    int n = mSize;

    // Quá trình khử xuôi (Forward Elimination)
    for (int i = 1; i <= n; ++i) {
        int pivot = i;
        for (int j = i + 1; j <= n; ++j) {
            if (std::abs(A(j, i)) > std::abs(A(pivot, i))) pivot = j;
        }
        assert(std::abs(A(pivot, i)) > 1e-9); // Bắt lỗi ma trận suy biến

        // Đổi chỗ hàng
        if (pivot != i) {
            for (int k = 1; k <= n; ++k) std::swap(A(i, k), A(pivot, k));
            double tempB = b(i);
            b(i) = b(pivot);
            b(pivot) = tempB;
        }

        // Khử các phần tử dưới đường chéo chính
        for (int j = i + 1; j <= n; ++j) {
            double factor = A(j, i) / A(i, i);
            for (int k = i; k <= n; ++k) A(j, k) -= factor * A(i, k);
            b(j) -= factor * b(i);
        }
    }

    // Quá trình thế ngược (Backward Substitution)
    Vector x(n);
    for (int i = n; i >= 1; --i) {
        double sum = b(i);
        for (int j = i + 1; j <= n; ++j) sum -= A(i, j) * x(j);
        x(i) = sum / A(i, i);
    }
    return x; // Trả về vector chứa nghiệm [cite: 35]
}


// ================= LỚP POSITIVE SYMMETRIC LINEAR SYSTEM =================

PosSymLinSystem::PosSymLinSystem(const Matrix& A, const Vector& b) : LinearSystem(A, b) {
    // Kiểm tra tính đối xứng của ma trận (A = A^T) [cite: 39]
    for (int i = 1; i <= mSize; ++i) {
        for (int j = 1; j <= mSize; ++j) {
            assert(std::abs(A(i, j) - A(j, i)) < 1e-9); 
        }
    }
}

// Hàm phụ trợ: Tính tích vô hướng (Dot Product) của hai vector
double DotProduct(const Vector& v1, const Vector& v2) {
    double sum = 0.0;
    for (int i = 1; i <= v1.GetSize(); ++i) {
        sum += v1(i) * v2(i);
    }
    return sum;
}

// Giải hệ phương trình bằng Conjugate Gradient Method 
Vector PosSymLinSystem::Solve() {
    Vector x(mSize); // Khởi tạo vector nghiệm x0 = [0, 0, ..., 0]
    Vector r = (*mpb) - (*mpA) * x; // Vector thặng dư ban đầu
    Vector p = r;                   // Hướng tìm kiếm
    double rsold = DotProduct(r, r);

    for (int i = 0; i < mSize * 2; ++i) { // Giới hạn số vòng lặp tối đa
        Vector Ap = (*mpA) * p;
        double alpha = rsold / DotProduct(p, Ap);
        
        x = x + p * alpha; // Cập nhật nghiệm dự đoán
        r = r - Ap * alpha; // Cập nhật thặng dư mới
        
        double rsnew = DotProduct(r, r);
        if (std::sqrt(rsnew) < 1e-10) break; // Dừng lại nếu đã đạt độ hội tụ
        
        p = r + p * (rsnew / rsold); // Cập nhật hướng tìm kiếm mới
        rsold = rsnew;
    }
    return x;
}