#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include "Vector.hpp"
#include "Matrix.hpp"
#include "LinearSystem.hpp"

int main() {
    std::cout << "=== PROJECT 1 - PART B: LINEAR REGRESSION ===\n\n";

    int totalInstances = 209;
    int numFeatures = 6;
    
    // 80% for Train (167 instances), 20% for Test (42 instances)
    int trainSize = 167; 
    int testSize = totalInstances - trainSize;

    Matrix A_total(totalInstances, numFeatures);
    Vector b_total(totalInstances);

    // 1. Read data from machine.data file
    std::ifstream file("machine.data");
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open machine.data. Please make sure the file is in the same directory as the executable!\n";
        return 1;
    }

    std::string line;
    int row = 1;
    while (std::getline(file, line) && row <= totalInstances) {
        std::stringstream ss(line);
        std::string token;

        // Skip the first 2 columns (Vendor and Model)
        std::getline(ss, token, ',');
        std::getline(ss, token, ',');

        // Read 6 feature columns: MYCT, MMIN, MMAX, CACH, CHMIN, CHMAX
        for (int j = 1; j <= numFeatures; ++j) {
            std::getline(ss, token, ',');
            A_total(row, j) = std::stod(token);
        }

        // Read target column: PRP
        std::getline(ss, token, ',');
        b_total(row) = std::stod(token);

        // Skip the last column (ERP)
        std::getline(ss, token, ','); 
        row++;
    }
    file.close();
    std::cout << "[+] Successfully loaded " << totalInstances << " data instances.\n";

    // 2. Split into Train and Test sets
    Matrix A_train(trainSize, numFeatures);
    Vector b_train(trainSize);
    Matrix A_test(testSize, numFeatures);
    Vector b_test(testSize);

    for (int i = 1; i <= trainSize; ++i) {
        for (int j = 1; j <= numFeatures; ++j) A_train(i, j) = A_total(i, j);
        b_train(i) = b_total(i);
    }
    for (int i = 1; i <= testSize; ++i) {
        for (int j = 1; j <= numFeatures; ++j) A_test(i, j) = A_total(i + trainSize, j);
        b_test(i) = b_total(i + trainSize);
    }
    std::cout << "[+] Data split: Train (" << trainSize << "), Test (" << testSize << ").\n\n";

    // 3. Train the model (Find x = A_train^+ * b_train)
    std::cout << "Calculating Pseudo-inverse Matrix. Please wait...\n";
    Matrix A_train_pinv = A_train.PseudoInverse();
    Vector x_params = A_train_pinv * b_train;

    std::cout << "\n[+] Parameter Results (Weights):\n";
    std::cout << "x1 (MYCT)  = " << x_params(1) << "\n";
    std::cout << "x2 (MMIN)  = " << x_params(2) << "\n";
    std::cout << "x3 (MMAX)  = " << x_params(3) << "\n";
    std::cout << "x4 (CACH)  = " << x_params(4) << "\n";
    std::cout << "x5 (CHMIN) = " << x_params(5) << "\n";
    std::cout << "x6 (CHMAX) = " << x_params(6) << "\n\n";

    // 4. Evaluate the model on Test set (Calculate RMSE)
    Vector predictions = A_test * x_params;
    double mse = 0.0;
    
    for (int i = 1; i <= testSize; ++i) {
        double error = predictions(i) - b_test(i);
        mse += error * error;
    }
    double rmse = std::sqrt(mse / testSize);

    std::cout << "[+] Evaluation on Test set:\n";
    std::cout << "-> Root Mean Square Error (RMSE) = " << rmse << "\n";
    std::cout << "\n=== PROJECT 1 COMPLETED ===\n";

    return 0;
}