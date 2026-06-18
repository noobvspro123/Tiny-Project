#include "ShoppingCart.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

// ─── CartItem ────────────────────────────────────────────────────────────────

void CartItem::Print() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  [" << productId << "] " << productName
              << " x" << quantity
              << "  @$" << price
               << "  = $" << Subtotal() << "\n" << std::flush;
}

// ─── ShoppingCart ─────────────────────────────────────────────────────────────

ShoppingCart::ShoppingCart(int userId) : mUserId(userId) {}

void ShoppingCart::AddItem(int productId, const std::string& name,
                           double price, int qty) {
    for (auto& item : mItems) {
        if (item.productId == productId) {
            item.quantity += qty;
            return;
        }
    }
    mItems.emplace_back(productId, name, price, qty);
}

void ShoppingCart::RemoveItem(int productId) {
    mItems.erase(
        std::remove_if(mItems.begin(), mItems.end(),
            [productId](const CartItem& i){ return i.productId == productId; }),
        mItems.end());
}

void ShoppingCart::UpdateQuantity(int productId, int newQty) {
    for (auto& item : mItems) {
        if (item.productId == productId) {
            if (newQty <= 0)
                RemoveItem(productId);
            else
                item.quantity = newQty;
            return;
        }
    }
}

void ShoppingCart::Clear() {
    mItems.clear();
}

double ShoppingCart::GetTotal() const {
    double total = 0.0;
    for (auto& item : mItems) total += item.Subtotal();
    return total;
}

void ShoppingCart::Print() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "+--- Shopping Cart (User " << mUserId << ") ---------------+\n";
    if (mItems.empty()) {
        std::cout << "  (empty)\n";
    } else {
        for (auto& item : mItems) item.Print();
        std::cout << "  -----------------------------------------\n";
        std::cout << "  TOTAL: $" << GetTotal() << "\n";
    }
    std::cout << "+---------------------------------------------+\n" << std::flush;
}
