#include "Order.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <stdexcept>

int Order::sNextId = 1;

std::string Order::CurrentTimestamp() {
    time_t now = time(nullptr);
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return std::string(buf);
}

Order::Order(int userId, const std::vector<CartItem>& items, double total)
    : mOrderId(sNextId++), mUserId(userId),
      mItems(items), mTotal(total),
      mTimestamp(CurrentTimestamp()) {}

// Format: orderId|userId|total|timestamp|pid:name:price:qty,...
std::string Order::Serialize() const {
    std::ostringstream oss;
    oss << mOrderId << "|" << mUserId << "|"
        << std::fixed << std::setprecision(2) << mTotal << "|"
        << mTimestamp << "|";
    for (size_t i = 0; i < mItems.size(); ++i) {
        if (i > 0) oss << ",";
        oss << mItems[i].productId << ":"
            << mItems[i].productName << ":"
            << mItems[i].price << ":"
            << mItems[i].quantity;
    }
    return oss.str();
}

Order Order::Deserialize(const std::string& line) {
    std::istringstream iss(line);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(iss, token, '|')) tokens.push_back(token);
    if (tokens.size() < 5) throw std::runtime_error("Invalid order line");

    std::vector<CartItem> items;
    if (!tokens[4].empty()) {
        std::istringstream itemStream(tokens[4]);
        std::string entry;
        while (std::getline(itemStream, entry, ',')) {
            std::istringstream es(entry);
            std::string part;
            std::vector<std::string> parts;
            while (std::getline(es, part, ':')) parts.push_back(part);
            if (parts.size() == 4)
                items.emplace_back(std::stoi(parts[0]), parts[1],
                                   std::stod(parts[2]), std::stoi(parts[3]));
        }
    }

    double total = std::stod(tokens[2]);
    Order o(std::stoi(tokens[1]), items, total);
    o.mOrderId   = std::stoi(tokens[0]);
    o.mTimestamp = tokens[3];
    return o;
}

void Order::Print() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Order #" << mOrderId
              << " | User: " << mUserId
              << " | " << mTimestamp
              << " | Total: $" << mTotal << "\n";
    for (auto& item : mItems) item.Print();
    std::cout << std::flush;
}
