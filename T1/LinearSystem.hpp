#ifndef LINEARSYSTEM_HPP
#define LINEARSYSTEM_HPP

#include "Matrix.hpp"
#include "Vector.hpp"

class LinearSystem {
protected: 
    // Chuyển sang protected để lớp con (PosSymLinSystem) có thể kế thừa và sử dụng 
    int mSize;
    Matrix* mpA;
    Vector* mpb;

private:
    // Chặn Copy Constructor và Default Constructor theo yêu cầu 
    LinearSystem(const LinearSystem& other);
    LinearSystem();

public:
    // Constructor khởi tạo từ Ma trận A và Vector b [cite: 31]
    LinearSystem(const Matrix& A, const Vector& b);
    
    // Virtual destructor rất quan trọng khi dùng kế thừa
    virtual ~LinearSystem(); 

    // Hàm giải hệ phương trình (Virtual để lớp con có thể ghi đè) 
    virtual Vector Solve(); 
};

// Lớp giải hệ phương trình cho Ma trận đối xứng dương xác định 
class PosSymLinSystem : public LinearSystem {
public:
    PosSymLinSystem(const Matrix& A, const Vector& b);
    
    // Ghi đè hàm Solve bằng phương pháp Conjugate Gradient 
    Vector Solve() override; 
};

#endif // LINEARSYSTEM_HPP