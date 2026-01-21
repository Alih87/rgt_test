# 로봇 응용 SW 개발자 직무 면접 전 과제

## Overview
This repository contains the implementation for **Problem 1: Resource Management Using Smart Pointers**.
The project implements a `LogFileManager` class that manages multiple log files and records messages with timestamps using modern C++ smart pointers and RAII principles.

Only the problems attempted are included, as per the assignment instructions.

---

## Problems Description
**Problem 1: Resource Management Using Smart Pointers**

Implemented a `LogFileManager` class that:
- Manages multiple log files
- Writes log messages with timestamps
- Uses smart pointers for safe and automatic resource management

**Problem 2:  Container Design Using Templates and STL**

Implemented a `CircularBuffer` class that:
- Has a fixed size Circular Buffer
- Contains methods begin(), end(), size(), capacity(), empty(), push_back() etc.
- const / non-const versions


**Problem 3:  Multithreading and Functional Programming**

Implemented a `ParallelProcessor` class that:
- Compares large size pixel data handling
- Compares sequential and parallel processing times


**Problem 4:  Python Problem - RESTful API Backend using FastAPI**

Implemented a FAST API backend that:
- Supports Borrow/Return System
- Uses REST endpoints (POST, GET, DELETE)
- Supports Book Management
---

## Development Environment
- **Operating System:** Windows 11
- **IDE:** Visual Studio Code
- **Compiler:** MSVC (Visual Studio Build Tools)
- **C++ Standard:** C++17

---

## Installation & Build Instructions
1. Clone the repository:
   ```bash
   git clone https://github.com/Alih87/rgt_test.git

2. Install Visual Studio Code and MSVC compiler. Guide: https://code.visualstudio.com/docs/cpp/config-msvc

3. In the `lib_mgmt_sys` directory run the following command to install python dependancies ```bash
   pip install -r requirements.txt

4. Make sure to add `/std:c++17` inside `.vscode/task.json` args list.