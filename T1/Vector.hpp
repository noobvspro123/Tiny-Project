#ifndef VECTOR_HPP
#define VECTOR_HPP

class Vector {
private:
    int mSize;       // Kích thước của mảng
    double* mData;   // Con trỏ trỏ tới phần tử dữ liệu của mảng

public:
    // Constructors & Destructor
    Vector(int size);                     
    Vector(const Vector& other);          
    ~Vector();                            

    // Lấy kích thước vector (phục vụ cho các phép tính sau này)
    int GetSize() const;

    // Nạp chồng toán tử gán
    Vector& operator=(const Vector& other);

    // Nạp chồng toán tử ngoặc vuông [] (0-based indexing)
    double& operator[](int i);
    const double& operator[](int i) const;
    
    // Nạp chồng toán tử ngoặc tròn () (1-based indexing)
    double& operator()(int i);
    const double& operator()(int i) const;

    // Nạp chồng toán tử một ngôi (Unary operators)
    Vector operator+() const;
    Vector operator-() const;

    // Nạp chồng toán tử hai ngôi với Vector (Binary operators)
    Vector operator+(const Vector& v) const;
    Vector operator-(const Vector& v) const;

    // Nạp chồng toán tử nhân với số vô hướng (Scalar multiplication)
    Vector operator*(double scalar) const;
};

// Hàm bạn (để hỗ trợ phép nhân: Số vô hướng * Vector)
Vector operator*(double scalar, const Vector& v);

#endif // VECTOR_HPP