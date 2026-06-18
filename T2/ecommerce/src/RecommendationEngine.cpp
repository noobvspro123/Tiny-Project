#include "RecommendationEngine.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <map>

// ─── Category bonus ───────────────────────────────────────────────────────────
// Bonus = (total purchases in same category by user) * 0.5

double RecommendationEngine::CategoryBonus(
        const User& user,
        const std::string& category,
        const std::vector<Product>& allProducts) const {

    double bonus = 0.0;
    for (auto& p : allProducts) {
        if (p.GetCategory() == category) {
            int purchased = user.GetPurchaseCount(p.GetId());
            int viewed    = user.GetViewCount(p.GetId());
            bonus += purchased * 0.5 + viewed * 0.1;
        }
    }
    return bonus;
}

// ─── Recommend ────────────────────────────────────────────────────────────────

std::vector<Product> RecommendationEngine::Recommend(
        const User& user,
        const std::vector<Product>& allProducts,
        int topN) const {

    // Compute score for each product
    std::vector<std::pair<double, int>> scored;   // (score, index)
    for (size_t i = 0; i < allProducts.size(); ++i) {
        const Product& p = allProducts[i];
        if (p.GetStock() <= 0) continue;          // skip out-of-stock

        double catBonus = CategoryBonus(user, p.GetCategory(), allProducts);
        double userViews    = user.GetViewCount(p.GetId());
        double userPurchases = user.GetPurchaseCount(p.GetId());

        // Score based on global stats + user's own history in this category
        double score = p.GetViewCount()     * 0.2
                     + p.GetPurchaseCount() * 0.8
                     + catBonus
                     + userViews    * 0.3   // boost if user already viewed
                     + userPurchases * 0.1; // small boost for re-purchase

        scored.push_back({ score, (int)i });
    }

    // Sort descending by score
    std::sort(scored.begin(), scored.end(),
              [](const std::pair<double,int>& a, const std::pair<double,int>& b){
                  return a.first > b.first;
              });

    std::vector<Product> result;
    int count = 0;
    for (auto& s : scored) {
        if (count >= topN) break;
        result.push_back(allProducts[s.second]);
        count++;
    }
    return result;
}

void RecommendationEngine::PrintRecommendations(
        const User& user,
        const std::vector<Product>& allProducts,
        int topN) const {

    std::cout << "\n* Top " << topN << " recommendations for " << user.GetUsername() << ":\n";
    auto recs = Recommend(user, allProducts, topN);
    if (recs.empty()) {
        std::cout << "  No recommendations available.\n";
        return;
    }
    int rank = 1;
    for (auto& p : recs) {
        std::cout << "  " << rank++ << ". ";
        p.Print();
    }
}
