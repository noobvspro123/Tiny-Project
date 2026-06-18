#  Tiny-Project: Programming 2

This repository contains the source code for our final project in the **Programming 2** course. The project focuses on learning and applying core C++ concepts like pointers, memory management, Object-Oriented Programming (OOP), and File I/O.

##  Project done by our group of 5 members:

| No. | Full Name | Student ID |
|:---:|:---|:---|
| 1 | Trinh Viet Long | 104240033 |
| 2 | Dang Thanh Tu | 104240761 |
| 3 | Bui Quang Minh | 104240606 |
| 4 | Le Vu Tien Minh | 104240689 |
| 5 | Le Minh Tri | 104240460 |

##  Repository Structure

* **`T1/` (Linear System & Regression):** We built everything from scratch without external math libraries. We created our own `Matrix` and `Vector` classes to practice pointer-based memory allocation, constructors/destructors, and operator overloading. We used these classes to calculate the Moore-Penrose pseudo-inverse and predict CPU performance.
* **`T2/` (Smart E-Commerce System):** A simple Object-Oriented E-Commerce app. We practiced designing OOP classes (`User`, `Product`, `Cart`, `Order`), built a basic scoring algorithm for product recommendations, and used standard C++ File I/O to save data into `.txt` files so it isn't lost after closing the program.

##  Default Accounts (Project 2)
To access the administrator features, use the following login credentials:
* **Username:** `admin`
* **Password:** `admin123`

*(Use this account to log into the system, add new products to the catalog, manage the item list, and view platform statistics).*

##  How to Compile and Run

###  Running Project 1 (T1)
Navigate to the `T1` directory, compile the C++ files, and execute:

cd T1
g++ *.cpp -o main
./main


 Running Project 2 (T2)
Navigate to the T2/ecommerce directory. Depending on your operating system, choose one of the methods below:
Method 1: Windows (Using CMD / PowerShell)
Open your terminal (cmd) and run the provided batch file:

T2/ecommerce
.\build.bat


Method 2: Linux / macOS / GitHub Codespaces (Using Makefile)
Open your terminal and run:

T2/ecommerce
make
./ecommerce


(Note: If you encounter a Permission denied error on Linux/Codespaces when running the executable, please grant execution permission by running chmod +x ecommerce first).
