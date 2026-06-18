#include <iostream>
#include <string>
#include <limits>
#include "ECommerceSystem.h"

void clearInput() {
    std::cin.clear();
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}

void showHeader(const std::string& title) {
    std::cout << "\033[2J\033[1;1H";
    std::cout << "\n==================================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "==================================================\n" << std::flush;
}

void pause() {
    std::cout << std::flush;
    std::cout << "\nNhan Enter de tiep tuc..." << std::flush;
    clearInput();
    std::cin.get();
}

int getIntInput() {
    int val;
    if (!(std::cin >> val)) {
        clearInput();
        return -1;
    }
    return val;
}

int main() {
    ECommerceSystem sys("data/products.txt", "data/users.txt", "data/orders.txt");
    sys.LoadAll();

    if (sys.RegisterUser("admin", "admin123", true)) sys.SaveAll();

    User* currentUser = nullptr;
    bool running = true;

    while (running) {
        if (!currentUser) {
            showHeader("HE THONG TMDT - CHAO MUNG");
            std::cout << "1. Dang nhap\n";
            std::cout << "2. Dang ky tai khoan moi\n";
            std::cout << "0. Thoat\n";
            std::cout << "Chon: ";

            int choice = getIntInput();
            if (choice < 0) continue;

            if (choice == 1) {
                std::string user, pass;
                std::cout << "Username: "; std::cin >> user;
                std::cout << "Password: "; std::cin >> pass;
                currentUser = sys.Login(user, pass);
                if (!currentUser)
                    std::cout << "![Loi] Sai tai khoan hoac mat khau.\n";
                else
                    std::cout << "[OK] Chao mung " << currentUser->GetUsername() << "!\n";
                pause();
            } else if (choice == 2) {
                std::string user, pass;
                std::cout << "Username moi: "; std::cin >> user;
                std::cout << "Password moi: "; std::cin >> pass;
                if (sys.RegisterUser(user, pass)) {
                    sys.SaveAll();
                    std::cout << "[OK] Dang ky thanh cong. Hay dang nhap.\n";
                } else
                    std::cout << "![Loi] Username da ton tai.\n";
                pause();
            } else if (choice == 0) {
                running = false;
            }
        }
        else if (currentUser->IsAdmin()) {
            showHeader("MENU QUAN TRI (ADMIN)");
            std::cout << "1. Them san pham moi\n";
            std::cout << "2. Sua san pham\n";
            std::cout << "3. Xoa san pham\n";
            std::cout << "4. Liet ke tat ca san pham\n";
            std::cout << "5. Thong ke (xem nhieu, ban chay)\n";
            std::cout << "6. Xem nguoi dung hoat dong nhieu nhat\n";
            std::cout << "7. Xem danh sach nguoi dung\n";
            std::cout << "0. Dang xuat & Luu\n";
            std::cout << "Chon: ";

            int choice = getIntInput();
            if (choice < 0) continue;

            if (choice == 1) {
                std::string n, c; double p; int s;
                std::cout << "Ten SP: "; clearInput(); std::getline(std::cin, n);
                if (n.empty()) { std::cout << "Ten khong duoc rong.\n"; pause(); continue; }
                std::cout << "DanhMuc: "; std::getline(std::cin, c);
                std::cout << "Gia: "; std::cin >> p;
                if (p <= 0) { std::cout << "Gia phai > 0.\n"; pause(); continue; }
                std::cout << "Kho: "; std::cin >> s;
                if (s < 0) { std::cout << "Kho khong am.\n"; pause(); continue; }
                sys.AddProduct(n, c, p, s);
                sys.SaveAll();
                pause();
            } else if (choice == 2) {
                int pid;
                std::cout << "Nhap ID san pham can sua: "; std::cin >> pid;
                if (pid <= 0) { std::cout << "ID khong hop le.\n"; pause(); continue; }
                std::string n, c; double p; int s;
                std::cout << "Ten moi: "; clearInput(); std::getline(std::cin, n);
                std::cout << "DanhMuc moi: "; std::getline(std::cin, c);
                std::cout << "Gia moi: "; std::cin >> p;
                std::cout << "Kho moi: "; std::cin >> s;
                sys.EditProduct(pid, n, c, p, s);
                sys.SaveAll();
                pause();
            } else if (choice == 3) {
                int pid;
                std::cout << "Nhap ID san pham can xoa: "; std::cin >> pid;
                if (pid <= 0) { std::cout << "ID khong hop le.\n"; pause(); continue; }
                sys.DeleteProduct(pid);
                sys.SaveAll();
                pause();
            } else if (choice == 4) {
                sys.ListAllProducts();
                pause();
            } else if (choice == 5) {
                sys.ViewStatsMostViewed(5);
                sys.ViewStatsBestSelling(5);
                pause();
            } else if (choice == 6) {
                sys.ViewStatsActiveUsers(5);
                pause();
            } else if (choice == 7) {
                sys.ListAllUsers();
                pause();
            } else if (choice == 0) {
                sys.SaveAll();
                currentUser = nullptr;
                std::cout << "[OK] Da luu. Dang xuat.\n";
                pause();
            }
        }
        else {
            showHeader("MENU CUA HANG - " + currentUser->GetUsername());
            std::cout << "1. Xem danh muc san pham\n";
            std::cout << "2. Goi y hom nay (Dua tren so thich)\n";
            std::cout << "3. Them hang vao gio\n";
            std::cout << "4. Xem gio hang\n";
            std::cout << "5. Thanh toan (Checkout)\n";
            std::cout << "6. Xem lich su don hang\n";
            std::cout << "0. Dang xuat & Luu\n";
            std::cout << "Chon: ";

            int choice = getIntInput();
            if (choice < 0) continue;

            if (choice == 1) {
                sys.BrowseProducts(currentUser->GetId());
                pause();
            } else if (choice == 2) {
                sys.GetRecommendations(currentUser->GetId(), 5);
                pause();
            } else if (choice == 3) {
                int pid, qty;
                std::cout << "Nhap ID san pham: "; std::cin >> pid;
                if (pid <= 0) { std::cout << "ID khong hop le.\n"; pause(); continue; }
                std::cout << "So luong: "; std::cin >> qty;
                if (qty <= 0) { std::cout << "So luong phai > 0.\n"; pause(); continue; }
                sys.AddToCart(currentUser->GetId(), pid, qty);
                pause();
            } else if (choice == 4) {
                sys.ViewCart(currentUser->GetId());
                pause();
            } else if (choice == 5) {
                sys.Checkout(currentUser->GetId());
                sys.SaveAll();
                pause();
            } else if (choice == 6) {
                sys.ViewOrderHistory(currentUser->GetId());
                pause();
            } else if (choice == 0) {
                sys.SaveAll();
                currentUser = nullptr;
                std::cout << "[OK] Da luu. Dang xuat.\n";
                pause();
            }
        }
    }

    sys.SaveAll();
    std::cout << "\nCam on ban da su dung he thong!\n";
    return 0;
}
