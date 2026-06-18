#pragma once
#include <string>
#include <vector>
#include <map>

class User {
private:
    int         mId;
    std::string mUsername;
    std::string mPassword;   // stored as plain text for demo purposes
    bool        mIsAdmin;

    // interaction history: productId -> {viewCount, purchaseCount}
    std::map<int, std::pair<int,int>> mHistory;

public:
    User();
    User(int id, const std::string& username,
         const std::string& password, bool isAdmin = false);

    // Getters
    int         GetId()       const { return mId; }
    std::string GetUsername() const { return mUsername; }
    std::string GetPassword() const { return mPassword; }
    bool        IsAdmin()     const { return mIsAdmin; }

    // Interaction tracking
    void RecordView(int productId);
    void RecordPurchase(int productId, int qty = 1);
    int  GetViewCount(int productId)     const;
    int  GetPurchaseCount(int productId) const;

    // Returns all product IDs the user interacted with
    std::vector<int> GetInteractedProducts() const;

    // Serialization
    std::string Serialize()   const;
    static User Deserialize(const std::string& line);

    void Print() const;
};
