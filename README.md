# Cafeteria-Management-System-Simulation
C++ console application simulating a cafeteria self-service system using advanced data structures (Priority Queue, BST) and Dijkstra's Algorithm for customer flow and inventory management.

# 🍴 Cafeteria Management System Simulation
          ![Self-service Tray canteen in cafeteria](https://www.shutterstock.com/image-photo/little-kids-eating-lunch-wooden-600nw-2671262569.jpg)
## 📝 Project Overview

This is a C++ console application that simulates a self-service cafeteria environment. The system manages customer queues (Students and Faculty), prioritizes service, tracks tray inventory, calculates wait times, and integrates graph algorithms (Dijkstra's) to model and optimize cafeteria physical flow.

The project demonstrates the practical application of core Data Structures and Algorithms (DSA) principles for solving real-world management challenges.

## ✨ Key Features Implemented

### 1. Priority-Based Service & Queuing
* **Mixed Queues:** Utilizes an STL `std::priority_queue` with a custom lambda comparator for **Faculty** (prioritizing high-priority and then arrival time) and a standard `std::queue` for **Students**.
* **Service Logic:** Faculty are always served before students.
* **Flexible Queue:** An `std::deque` is used to represent a conceptual "flexible" line, pushing faculty to the front and students to the back.

### 2. Inventory and Asset Management
* **Tray Tracking (BST):** A **Binary Search Tree (BST)** is implemented to store, manage, and efficiently search for individual assigned tray IDs.
* **Tray Returns (Stack):** An `std::stack` is used to model the physical collection point for returned trays ($\text{LIFO}$).
* **Capacity Management:** The system tracks the current count of available trays against a defined maximum capacity.

### 3. Optimization and Metrics
* **Dijkstra's Algorithm:** A fully implemented Graph structure allows the application to calculate the **Shortest Paths** from any starting node (e.g., entrance) to all other nodes (e.g., serving stations, exits) in the cafeteria layout.
* **Wait Time Analysis:** Calculates and displays the **moving average waiting time** of the last five customers to provide a real-time metric of service performance.

### 4. Robustness and Input Handling
* Custom input functions (`readInt`, `readString`) are implemented with built-in **input validation** and **exception handling** (`try-catch`) to prevent crashes and ensure system stability.

## 🛠️ Tech Stack
* **Language:** C++
* **Core Libraries:** Standard Template Library (STL)
* **Data Structures:** `std::queue`, `std::priority_queue`, `std::stack`, `std::deque`, `std::vector`, Custom Binary Search Tree (BST)
* **Algorithms:** Dijkstra's Shortest Path Algorithm
