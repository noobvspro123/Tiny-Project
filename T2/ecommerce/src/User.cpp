#include "User.h"
#include <sstream>
#include <iostream>
#include <stdexcept>

User::User() : mId(0), mUsername(""), mPassword(""), mIsAdmin(false) {}

User::User(int id, const std::string& username,
           const std::string& password, bool isAdmin)
    : mId(id), mUsername(username), mPassword(password), mIsAdmin(isAdmin) {}

void User::RecordView(int productId) {
    mHistory[productId].first++;
}

void User::RecordPurchase(int productId, int qty) {
    mHistory[productId].second += qty;
}

int User::GetViewCount(int productId) const {
    auto it = mHistory.find(productId);
    return (it != mHistory.end()) ? it->second.first : 0;
}

int User::GetPurchaseCount(int productId) const {
    auto it = mHistory.find(productId);
    return (it != mHistory.end()) ? it->second.second : 0;
}

std::vector<int> User::GetInteractedProducts() const {
    std::vector<int> ids;
    for (auto& kv : mHistory) ids.push_back(kv.first);
    return ids;
}

// Format: id|username|password|isAdmin|pid:v:p,pid:v:p,...
std::string User::Serialize() const {
    std::ostringstream oss;
    oss << mId << "|" << mUsername << "|" << mPassword << "|"
        << (mIsAdmin ? 1 : 0) << "|";
    bool first = true;
    for (auto& kv : mHistory) {
        if (!first) oss << ",";
        oss << kv.first << ":" << kv.second.first << ":" << kv.second.second;
        first = false;
    }
    return oss.str();
}

User User::Deserialize(const std::string& line) {
    std::istringstream iss(line);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(iss, token, '|')) tokens.push_back(token);
    if (tokens.size() < 5) throw std::runtime_error("Invalid user line: " + line);

    User u;
    u.mId       = std::stoi(tokens[0]);
    u.mUsername = tokens[1];
    u.mPassword = tokens[2];
    u.mIsAdmin  = (tokens[3] == "1");

    // Parse history: pid:v:p,pid:v:p
    if (!tokens[4].empty()) {
        std::istringstream hist(tokens[4]);
        std::string entry;
        while (std::getline(hist, entry, ',')) {
            std::istringstream es(entry);
            std::string part;
            std::vector<std::string> parts;
            while (std::getline(es, part, ':')) parts.push_back(part);
            if (parts.size() == 3)
                u.mHistory[std::stoi(parts[0])] = { std::stoi(parts[1]), std::stoi(parts[2]) };
        }
    }
    return u;
}

void User::Print() const {
    std::cout << "[" << mId << "] " << mUsername
              << (mIsAdmin ? " (Admin)" : " (Customer)")
              << " | Interactions: " << mHistory.size() << "\n";
}
