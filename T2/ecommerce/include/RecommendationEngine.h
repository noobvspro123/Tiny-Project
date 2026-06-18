#pragma once
#include "Product.h"
#include "User.h"
#include <vector>
#include <map>

// ─── RecommendationEngine ─────────────────────────────────────────────────────
// Input : user interaction history (views + purchases)
// Output: Top N recommended products
// Score : view_count*0.2 + purchase_count*0.8 + category_bonus

class RecommendationEngine {
private:
    // Category bonus: based on how much the user likes this category
    double CategoryBonus(const User& user,
                         const std::string& category,
                         const std::vector<Product>& allProducts) const;

public:
    // Returns top N products sorted by score (excludes already-purchased items)
    std::vector<Product> Recommend(const User&                user,
                                   const std::vector<Product>& allProducts,
                                   int topN = 5) const;

    void PrintRecommendations(const User&                user,
                              const std::vector<Product>& allProducts,
                              int topN = 5) const;
};
