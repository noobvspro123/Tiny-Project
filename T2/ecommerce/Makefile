CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

SRCS = src/Product.cpp src/User.cpp src/ShoppingCart.cpp \
       src/Order.cpp src/RecommendationEngine.cpp \
       src/ECommerceSystem.cpp src/main.cpp

TARGET = ecommerce.exe

all: $(TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	@if exist $(TARGET) del $(TARGET)
	@if exist output.txt del output.txt

run: all
	@if exist output.txt del output.txt
	@if not exist data mkdir data
	@.\$(TARGET)
