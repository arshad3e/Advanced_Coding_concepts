#include <iostream>
#include <thread>
#include <chrono>

void hardRealTimeTask() {
    auto start_time = std::chrono::steady_clock::now();
    
    // Task execution
    std::cout << "Executing Hard Real-Time Task\n";
    
    // Simulate processing
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Simulating work

    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    // Strict deadline enforcement (e.g., must finish within 20ms)
    if (duration > 20) {
        std::cerr << "Deadline Missed! System Failure!\n";
    } else {
        std::cout << "Task completed within deadline.\n";
    }
}

int main() {
    std::cout << "Starting Hard Real-Time Task...\n";
    hardRealTimeTask();
    return 0;
}
