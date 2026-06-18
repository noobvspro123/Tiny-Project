#include "Vector.hpp"
#include <cassert>

// Constructor khởi tạo
Vector::Vector(int size) {
    assert(size > 0);
    mSize = size;
    mData = new double[mSize]; // Cấp phát bộ nhớ động
    for (int i = 0; i < mSize; ++i) {
        mData[i] = 0.0;
    }
}

// Copy constructor (Sao chép sâu - Deep copy)
Vector::Vector(const Vector& other) {
    mSize = other.mSize;
    mData = new double[mSize];
    for (int i = 0; i < mSize; ++i) {
        mData[i] = other.mData[i];
    }
}

// Destructor (Giải phóng bộ nhớ)
Vector::~Vector() {
    delete[] mData;
}

// Getter lấy kích thước
int Vector::GetSize() const {
    return mSize;
}

// Toán tử gán (=)
Vector& Vector::operator=(const Vector& other) {
    if (this != &other) {
        delete[] mData; // Xóa dữ liệu cũ đi trước
        mSize = other.mSize;
        mData = new double[mSize];
        for (int i = 0; i < mSize; ++i) {
            mData[i] = other.mData[i];
        }
    }
    return *this;
}

// Toán tử [] (chỉ số bắt đầu từ 0)
double& Vector::operator[](int i) {
    assert(i >= 0 && i < mSize); // Kiểm tra giới hạn mảng
    return mData[i];
}

const double& Vector::operator[](int i) const {
    assert(i >= 0 && i < mSize);
    return mData[i];
}

// Toán tử () (chỉ số bắt đầu từ 1)
double& Vector::operator()(int i) {
    assert(i >= 1 && i <= mSize); // Kiểm tra i có hợp lệ không
    return mData[i - 1];          // Trừ 1 để khớp với index thực tế của C++
}

const double& Vector::operator()(int i) const {
    assert(i >= 1 && i <= mSize);
    return mData[i - 1];
}

// Toán tử một ngôi (+)
Vector Vector::operator+() const {
    return *this;
}

// Toán tử một ngôi (-)
Vector Vector::operator-() const {
    Vector result(mSize);
    for (int i = 0; i < mSize; ++i) {
        result[i] = -mData[i];
    }
    return result;
}

// Phép cộng 2 vector
Vector Vector::operator+(const Vector& v) const {
    assert(mSize == v.mSize); // Bắt buộc cùng kích thước
    Vector result(mSize);
    for (int i = 0; i < mSize; ++i) {
        result[i] = mData[i] + v.mData[i];
    }
    return result;
}

// Phép trừ 2 vector
Vector Vector::operator-(const Vector& v) const {
    assert(mSize == v.mSize); // Bắt buộc cùng kích thước
    Vector result(mSize);
    for (int i = 0; i < mSize; ++i) {
        result[i] = mData[i] - v.mData[i];
    }
    return result;
}

// Phép nhân: Vector * Số vô hướng
Vector Vector::operator*(double scalar) const {
    Vector result(mSize);
    for (int i = 0; i < mSize; ++i) {
        result[i] = mData[i] * scalar;
    }
    return result;
}

// Phép nhân: Số vô hướng * Vector (hàm ngoài)
Vector operator*(double scalar, const Vector& v) {
    return v * scalar; // Gọi lại hàm Vector * scalar đã viết ở trên
}