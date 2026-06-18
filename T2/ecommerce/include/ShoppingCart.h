#pragma once
#include <vector>
#include <string>

// ─── CartItem ─────────────────────────────────────────────────────────────────

struct CartItem {
    int    productId;
    std::string productName;
    double price;
    int    quantity;

    CartItem(int id, const std::string& name, double price, int qty)
        : productId(id), productName(name), price(price), quantity(qty) {}

    double Subtotal() const { return price * quantity; }
    void Print() const;
};

// ─── ShoppingCart ─────────────────────────────────────────────────────────────

class ShoppingCart {
private:
    int               mUserId;
    std::vector<CartItem> mItems;

public:
    explicit ShoppingCart(int userId);

    // Operations
    void AddItem(int productId, const std::string& name, double price, int qty = 1);
    void RemoveItem(int productId);
    void UpdateQuantity(int productId, int newQty);
    void Clear();

    // Queries
    bool   IsEmpty()     const { return mItems.empty(); }
    int    GetUserId()   const { return mUserId; }
    double GetTotal()    const;
    int    GetItemCount() const { return (int)mItems.size(); }
    const std::vector<CartItem>& GetItems() const { return mItems; }

    void Print() const;
};
