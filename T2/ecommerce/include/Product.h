#pragma once
#include <string>

class Product {
private:
    int         mId;
    std::string mName;
    std::string mCategory;
    double      mPrice;
    int         mStock;
    int         mViewCount;
    int         mPurchaseCount;

public:
    Product();
    Product(int id, const std::string& name, const std::string& category,
            double price, int stock);

    // Getters
    int         GetId()            const { return mId; }
    std::string GetName()          const { return mName; }
    std::string GetCategory()      const { return mCategory; }
    double      GetPrice()         const { return mPrice; }
    int         GetStock()         const { return mStock; }
    int         GetViewCount()     const { return mViewCount; }
    int         GetPurchaseCount() const { return mPurchaseCount; }

    // Setters
    void SetName(const std::string& name)         { mName = name; }
    void SetCategory(const std::string& category) { mCategory = category; }
    void SetPrice(double price)                    { mPrice = price; }
    void SetStock(int stock)                       { mStock = stock; }

    void IncrementView()     { mViewCount++; }
    void IncrementPurchase(int qty = 1);

    // Recommendation score: score = view*0.2 + purchase*0.8 + category_bonus
    double GetScore(double categoryBonus = 0.0) const;

    // Serialization
    std::string Serialize()   const;
    static Product Deserialize(const std::string& line);

    void Print() const;
};
