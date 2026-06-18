#pragma once
#include "ShoppingCart.h"
#include <string>
#include <vector>
#include <ctime>

class Order {
private:
    int                   mOrderId;
    int                   mUserId;
    std::vector<CartItem> mItems;
    double                mTotal;
    std::string           mTimestamp;

    static int sNextId;

    static std::string CurrentTimestamp();

public:
    Order(int userId, const std::vector<CartItem>& items, double total);

    // Getters
    int         GetOrderId()   const { return mOrderId; }
    int         GetUserId()    const { return mUserId; }
    double      GetTotal()     const { return mTotal; }
    std::string GetTimestamp() const { return mTimestamp; }
    const std::vector<CartItem>& GetItems() const { return mItems; }

    // Serialization
    std::string Serialize()   const;
    static Order Deserialize(const std::string& line);

    static void SetNextId(int id) { sNextId = id; }
    static int  GetNextId()       { return sNextId; }

    void Print() const;
};
