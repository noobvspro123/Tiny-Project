#include "ECommerceSystem.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>

// ─── Constructor ──────────────────────────────────────────────────────────────

ECommerceSystem::ECommerceSystem(const std::string& productFile,
                                 const std::string& userFile,
                                 const std::string& orderFile)
    : mProductFile(productFile), mUserFile(userFile),
      mOrderFile(orderFile), mNextProductId(1), mNextUserId(1) {}

// ─── Helpers ──────────────────────────────────────────────────────────────────

Product* ECommerceSystem::FindProduct(int id) {
    for (auto& p : mProducts)
        if (p.GetId() == id) return &p;
    return nullptr;
}

User* ECommerceSystem::FindUser(int id) {
    for (auto& u : mUsers)
        if (u.GetId() == id) return &u;
    return nullptr;
}

User* ECommerceSystem::FindUserByName(const std::string& username) {
    for (auto& u : mUsers)
        if (u.GetUsername() == username) return &u;
    return nullptr;
}

void ECommerceSystem::EnsureCart(int userId) {
    if (mCarts.find(userId) == mCarts.end())
        mCarts.emplace(userId, ShoppingCart(userId));
}

// ─── Persistence ──────────────────────────────────────────────────────────────

void ECommerceSystem::LoadAll() {
    // Products
    {
        std::ifstream f(mProductFile);
        if (f.is_open()) {
            std::string line;
            int maxId = 0;
            while (std::getline(f, line)) {
                if (line.empty()) continue;
                try {
                    Product p = Product::Deserialize(line);
                    maxId = std::max(maxId, p.GetId());
                    mProducts.push_back(p);
                } catch (...) {}
            }
            mNextProductId = maxId + 1;
        }
    }
    // Users
    {
        std::ifstream f(mUserFile);
        if (f.is_open()) {
            std::string line;
            int maxId = 0;
            while (std::getline(f, line)) {
                if (line.empty()) continue;
                try {
                    User u = User::Deserialize(line);
                    maxId = std::max(maxId, u.GetId());
                    mUsers.push_back(u);
                } catch (...) {}
            }
            mNextUserId = maxId + 1;
        }
    }
    // Orders
    {
        std::ifstream f(mOrderFile);
        if (f.is_open()) {
            std::string line;
            int maxId = 0;
            while (std::getline(f, line)) {
                if (line.empty()) continue;
                try {
                    Order o = Order::Deserialize(line);
                    maxId = std::max(maxId, o.GetOrderId());
                    mOrders.push_back(o);
                } catch (...) {}
            }
            Order::SetNextId(maxId + 1);
        }
    }
}

void ECommerceSystem::SaveAll() const {
    auto writeFile = [](const std::string& path,
                        const std::vector<std::string>& lines) {
        std::ofstream f(path);
        for (auto& l : lines) f << l << "\n";
    };

    std::vector<std::string> pLines, uLines, oLines;
    for (auto& p : mProducts) pLines.push_back(p.Serialize());
    for (auto& u : mUsers)    uLines.push_back(u.Serialize());
    for (auto& o : mOrders)   oLines.push_back(o.Serialize());

    writeFile(mProductFile, pLines);
    writeFile(mUserFile,    uLines);
    writeFile(mOrderFile,   oLines);
}

// ─── Auth ─────────────────────────────────────────────────────────────────────

User* ECommerceSystem::Login(const std::string& username,
                             const std::string& password) {
    User* u = FindUserByName(username);
    if (u && u->GetPassword() == password) return u;
    return nullptr;
}

bool ECommerceSystem::RegisterUser(const std::string& username,
                                   const std::string& password,
                                   bool isAdmin) {
    if (FindUserByName(username)) return false;
    mUsers.emplace_back(mNextUserId++, username, password, isAdmin);
    return true;
}

// ─── Customer operations ──────────────────────────────────────────────────────

void ECommerceSystem::BrowseProducts(int /*userId*/) {
    std::cout << "\n--- Product Catalog -------------------------------\n";
    if (mProducts.empty()) { std::cout << "  No products available.\n"; return; }
    for (auto& p : mProducts) {
        if (p.GetStock() > 0) p.Print();
    }
    std::cout << "---------------------------------------------------\n";
}

void ECommerceSystem::ViewProduct(int userId, int productId) {
    Product* p = FindProduct(productId);
    if (!p) { std::cout << "Product not found.\n"; return; }

    p->IncrementView();
    User* u = FindUser(userId);
    if (u) u->RecordView(productId);

    std::cout << "\n-- Product Details --\n";
    p->Print();
}

void ECommerceSystem::AddToCart(int userId, int productId, int qty) {
    Product* p = FindProduct(productId);
    if (!p) { std::cout << "Product not found.\n"; return; }
    if (p->GetStock() < qty) {
        std::cout << "Not enough stock (available: " << p->GetStock() << ")\n";
        return;
    }
    EnsureCart(userId);
    mCarts.at(userId).AddItem(productId, p->GetName(), p->GetPrice(), qty);
    std::cout << "Added " << qty << "x [" << p->GetName() << "] to cart.\n";
}

void ECommerceSystem::RemoveFromCart(int userId, int productId) {
    EnsureCart(userId);
    mCarts.at(userId).RemoveItem(productId);
    std::cout << "Item removed from cart.\n";
}

void ECommerceSystem::UpdateCartQty(int userId, int productId, int newQty) {
    EnsureCart(userId);
    mCarts.at(userId).UpdateQuantity(productId, newQty);
    std::cout << "Cart updated.\n";
}

void ECommerceSystem::ViewCart(int userId) {
    EnsureCart(userId);
    mCarts.at(userId).Print();
}

Order* ECommerceSystem::Checkout(int userId) {
    EnsureCart(userId);
    ShoppingCart& cart = mCarts.at(userId);
    if (cart.IsEmpty()) {
        std::cout << "Cart is empty!\n";
        return nullptr;
    }

    // Deduct stock and record purchases
    for (auto& item : cart.GetItems()) {
        Product* p = FindProduct(item.productId);
        if (p) p->IncrementPurchase(item.quantity);
        User*    u = FindUser(userId);
        if (u) u->RecordPurchase(item.productId, item.quantity);
    }

    mOrders.emplace_back(userId, cart.GetItems(), cart.GetTotal());
    Order* created = &mOrders.back();

    cart.Clear();
    std::cout << "\n[OK] Order placed successfully!\n";
    created->Print();
    return created;
}

void ECommerceSystem::ViewOrderHistory(int userId) const {
    std::cout << "\n--- Order History (User " << userId << ") ---------------\n";
    bool found = false;
    for (auto& o : mOrders) {
        if (o.GetUserId() == userId) { o.Print(); found = true; }
    }
    if (!found) std::cout << "  No orders yet.\n";
}

void ECommerceSystem::GetRecommendations(int userId, int topN) {
    User* u = FindUser(userId);
    if (!u) { std::cout << "User not found.\n"; return; }
    mEngine.PrintRecommendations(*u, mProducts, topN);
}

// ─── Admin operations ─────────────────────────────────────────────────────────

void ECommerceSystem::AddProduct(const std::string& name,
                                  const std::string& category,
                                  double price, int stock) {
    mProducts.emplace_back(mNextProductId++, name, category, price, stock);
    std::cout << "Product [" << name << "] added (ID=" << mNextProductId-1 << ").\n";
}

void ECommerceSystem::EditProduct(int productId, const std::string& name,
                                   const std::string& category,
                                   double price, int stock) {
    Product* p = FindProduct(productId);
    if (!p) { std::cout << "Product not found.\n"; return; }
    p->SetName(name);
    p->SetCategory(category);
    p->SetPrice(price);
    p->SetStock(stock);
    std::cout << "Product [" << productId << "] updated.\n";
}

void ECommerceSystem::DeleteProduct(int productId) {
    auto it = std::remove_if(mProducts.begin(), mProducts.end(),
        [productId](const Product& p){ return p.GetId() == productId; });
    if (it == mProducts.end()) { std::cout << "Product not found.\n"; return; }
    mProducts.erase(it, mProducts.end());
    std::cout << "Product [" << productId << "] deleted.\n";
}

void ECommerceSystem::ViewStatsMostViewed(int topN) const {
    std::cout << "\n--- Most Viewed Products (Top " << topN << ") -----------\n";
    auto sorted = mProducts;
    std::sort(sorted.begin(), sorted.end(),
        [](const Product& a, const Product& b){ return a.GetViewCount() > b.GetViewCount(); });
    for (int i = 0; i < topN && i < (int)sorted.size(); ++i) {
        std::cout << i+1 << ". "; sorted[i].Print();
    }
}

void ECommerceSystem::ViewStatsBestSelling(int topN) const {
    std::cout << "\n--- Best Selling Products (Top " << topN << ") ----------\n";
    auto sorted = mProducts;
    std::sort(sorted.begin(), sorted.end(),
        [](const Product& a, const Product& b){ return a.GetPurchaseCount() > b.GetPurchaseCount(); });
    for (int i = 0; i < topN && i < (int)sorted.size(); ++i) {
        std::cout << i+1 << ". "; sorted[i].Print();
    }
}

void ECommerceSystem::ViewStatsActiveUsers(int topN) const {
    std::cout << "\n--- Most Active Users (Top " << topN << ") --------------\n";
    auto sorted = mUsers;
    std::sort(sorted.begin(), sorted.end(),
        [](const User& a, const User& b){
            return a.GetInteractedProducts().size() > b.GetInteractedProducts().size();
        });
    for (int i = 0; i < topN && i < (int)sorted.size(); ++i) {
        std::cout << i+1 << ". "; sorted[i].Print();
    }
}

void ECommerceSystem::ListAllProducts() const {
    std::cout << "\n--- All Products -----------------------------------\n";
    for (auto& p : mProducts) p.Print();
}

void ECommerceSystem::ListAllUsers() const {
    std::cout << "\n--- All Users --------------------------------------\n";
    for (auto& u : mUsers) u.Print();
}
