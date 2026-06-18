#pragma once
#include "Product.h"
#include "User.h"
#include "ShoppingCart.h"
#include "Order.h"
#include "RecommendationEngine.h"
#include <vector>
#include <string>
#include <map>

// ─── ECommerceSystem ──────────────────────────────────────────────────────────
// Central system managing products, users, carts, orders, and recommendations.

class ECommerceSystem {
private:
    std::vector<Product>          mProducts;
    std::vector<User>             mUsers;
    std::vector<Order>            mOrders;
    std::map<int, ShoppingCart>   mCarts;    // userId -> cart
    RecommendationEngine          mEngine;

    // File paths for persistence
    std::string mProductFile;
    std::string mUserFile;
    std::string mOrderFile;

    // ID counters
    int mNextProductId;
    int mNextUserId;

    // Helpers
    Product* FindProduct(int id);
    User*    FindUser(int id);
    User*    FindUserByName(const std::string& username);
    void     EnsureCart(int userId);

public:
    ECommerceSystem(const std::string& productFile = "data/products.txt",
                    const std::string& userFile    = "data/users.txt",
                    const std::string& orderFile   = "data/orders.txt");

    // ── Persistence ───────────────────────────────────────────────────────────
    void LoadAll();
    void SaveAll() const;

    // ── Auth ──────────────────────────────────────────────────────────────────
    User* Login(const std::string& username, const std::string& password);
    bool  RegisterUser(const std::string& username, const std::string& password);

    // ── Customer operations ───────────────────────────────────────────────────
    void BrowseProducts(int userId);
    void ViewProduct(int userId, int productId);
    void AddToCart(int userId, int productId, int qty = 1);
    void RemoveFromCart(int userId, int productId);
    void UpdateCartQty(int userId, int productId, int newQty);
    void ViewCart(int userId);
    Order* Checkout(int userId);
    void ViewOrderHistory(int userId) const;
    void GetRecommendations(int userId, int topN = 5);

    // ── Admin operations ──────────────────────────────────────────────────────
    void AddProduct(const std::string& name, const std::string& category,
                    double price, int stock);
    void EditProduct(int productId, const std::string& name,
                     const std::string& category, double price, int stock);
    void DeleteProduct(int productId);
    void ViewStatsMostViewed(int topN = 5)    const;
    void ViewStatsBestSelling(int topN = 5)   const;
    void ViewStatsActiveUsers(int topN = 5)   const;
    void ListAllProducts()                    const;
    void ListAllUsers()                       const;
};
