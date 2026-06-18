#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>
#include <ios>
#ifdef _WIN32
#include <windows.h>
#endif
#include "ECommerceSystem.h"

// Hàm tiện ích để xóa bộ nhớ đệm nhập liệu
void clearInput() {
    std::cin.clear();
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}

void showHeader(const std::string& title) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    std::cout << "\n==================================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "==================================================\n";
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    ECommerceSystem sys("data/products.txt", "data/users.txt", "data/orders.txt");
    sys.LoadAll();

    // Tự động thêm admin nếu chưa có
    sys.RegisterUser("admin", "admin123");

    // Nếu kho hàng trống, thêm một số sản phẩm mẫu
    // (Điều này chỉ chạy 1 lần duy nhất khi file products.txt chưa có dữ liệu)
    // User* admin = sys.Login("admin", "admin123");
    // ... code seed data nếu cần ...

    User* currentUser = nullptr;
    bool running = true;

    while (running) {
        if (!currentUser) {
            showHeader("HE THONG TMDT - CHAO MUNG");
            std::cout << "1. Dang nhap\n";
            std::cout << "2. Dang ky tai khoan moi\n";
            std::cout << "0. Thoat\n";
            std::cout << "Chon: ";
            
            int choice;
            if (!(std::cin >> choice)) { clearInput(); continue; }

            if (choice == 1) {
                std::string user, pass;
                std::cout << "Username: "; std::cin >> user;
                std::cout << "Password: "; std::cin >> pass;
                currentUser = sys.Login(user, pass);
                if (!currentUser) std::cout << "![Loi] Sai tai khoan hoac mat khau.\n";
                else std::cout << "[OK] Chao mung " << currentUser->GetUsername() << "!\n";
            } else if (choice == 2) {
                std::string user, pass;
                std::cout << "Username moi: "; std::cin >> user;
                std::cout << "Password moi: "; std::cin >> pass;
                if (sys.RegisterUser(user, pass)) std::cout << "[OK] Dang ky thanh cong. Hay dang nhap.\n";
                else std::cout << "![Loi] Username da ton tai.\n";
            } else if (choice == 0) {
                running = false;
            }
        } 
        else {
            // Danh cho Admin
            if (currentUser->GetUsername() == "admin") {
                showHeader("MENU QUAN TRI (ADMIN)");
                std::cout << "1. Them san pham moi\n";
                std::cout << "2. Liet ke tat ca san pham\n";
                std::cout << "3. Thong ke ban chay/xem nhieu\n";
                std::cout << "4. Xem danh sach nguoi dung\n";
                std::cout << "0. Dang xuat\n";
                std::cout << "Chon: ";
                
                int choice;
                if (!(std::cin >> choice)) { clearInput(); continue; }

                if (choice == 1) {
                    std::string n, c; double p; int s;
                    std::cout << "Ten SP: "; clearInput(); std::getline(std::cin, n);
                    std::cout << "DanhMuc: "; std::getline(std::cin, c);
                    std::cout << "Gia: "; std::cin >> p;
                    std::cout << "Kho: "; std::cin >> s;
                    sys.AddProduct(n, c, p, s);
                } else if (choice == 2) {
                    sys.ListAllProducts();
                } else if (choice == 3) {
                    sys.ViewStatsMostViewed(5);
                    sys.ViewStatsBestSelling(5);
                } else if (choice == 4) {
                    sys.ListAllUsers();
                } else if (choice == 0) {
                    currentUser = nullptr;
                }
            } 
            // Danh cho Khach hang
            else {
                showHeader("MENU CUA HANG - " + currentUser->GetUsername());
                std::cout << "1. Xem danh muc san pham\n";
                std::cout << "2. Goi y hom nay (Dua tren so thich)\n";
                std::cout << "3. Them hang vao gio\n";
                std::cout << "4. Xem gio hang\n";
                std::cout << "5. Thanh toan (Checkout)\n";
                std::cout << "6. Xem lich su don hang\n";
                std::cout << "0. Dang xuat\n";
                std::cout << "Chon: ";

                int choice;
                if (!(std::cin >> choice)) { clearInput(); continue; }

                if (choice == 1) {
                    sys.BrowseProducts(currentUser->GetId());
                } else if (choice == 2) {
                    sys.GetRecommendations(currentUser->GetId(), 5);
                } else if (choice == 3) {
                    int pid, qty;
                    std::cout << "Nhap ID san pham: "; std::cin >> pid;
                    std::cout << "So luong: "; std::cin >> qty;
                    sys.AddToCart(currentUser->GetId(), pid, qty);
                } else if (choice == 4) {
                    sys.ViewCart(currentUser->GetId());
                } else if (choice == 5) {
                    sys.Checkout(currentUser->GetId());
                } else if (choice == 6) {
                    sys.ViewOrderHistory(currentUser->GetId());
                } else if (choice == 0) {
                    currentUser = nullptr;
                }
            }
        }
    }

    sys.SaveAll();
    std::cout << "\nCam on ban da su dung he thong!\n";
    return 0;
}
