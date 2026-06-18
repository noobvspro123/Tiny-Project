#include "Product.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>

Product::Product()
    : mId(0), mName(""), mCategory(""), mPrice(0.0),
      mStock(0), mViewCount(0), mPurchaseCount(0) {}

Product::Product(int id, const std::string& name, const std::string& category,
                 double price, int stock)
    : mId(id), mName(name), mCategory(category), mPrice(price),
      mStock(stock), mViewCount(0), mPurchaseCount(0) {}

void Product::IncrementPurchase(int qty) {
    mPurchaseCount += qty;
    mStock         -= qty;
    if (mStock < 0) mStock = 0;
}

double Product::GetScore(double categoryBonus) const {
    return mViewCount * 0.2 + mPurchaseCount * 0.8 + categoryBonus;
}

// Format: id|name|category|price|stock|viewCount|purchaseCount
std::string Product::Serialize() const {
    std::ostringstream oss;
    oss << mId << "|" << mName << "|" << mCategory << "|"
        << std::fixed << std::setprecision(2) << mPrice << "|"
        << mStock << "|" << mViewCount << "|" << mPurchaseCount;
    return oss.str();
}

Product Product::Deserialize(const std::string& line) {
    std::istringstream iss(line);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(iss, token, '|')) tokens.push_back(token);
    if (tokens.size() < 7) throw std::runtime_error("Invalid product line: " + line);

    Product p;
    p.mId            = std::stoi(tokens[0]);
    p.mName          = tokens[1];
    p.mCategory      = tokens[2];
    p.mPrice         = std::stod(tokens[3]);
    p.mStock         = std::stoi(tokens[4]);
    p.mViewCount     = std::stoi(tokens[5]);
    p.mPurchaseCount = std::stoi(tokens[6]);
    return p;
}

void Product::Print() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "[" << mId << "] " << mName
              << " | Category: " << mCategory
              << " | Price: $" << mPrice
              << " | Stock: " << mStock
              << " | Views: " << mViewCount
              << " | Purchases: " << mPurchaseCount << "\n";
}
