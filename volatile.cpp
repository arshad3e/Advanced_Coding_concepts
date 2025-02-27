/*volatile - Prevents Compiler Optimizations
Used when a variable can change outside the program’s control, such as hardware registers, shared memory, or interrupt-driven changes.
Prevents the compiler from optimizing away repeated reads/writes.
Example: Reading a Hardware Register
cpp
Copy
Edit*/

#include <iostream>

volatile int sensorData = 10;  // Can change anytime (e.g., by hardware)

void readSensor() {
    while (sensorData != 20) {  // This loop won’t be optimized away
        std::cout << "Waiting for sensor update...\n";
    }
    std::cout << "Sensor updated to 20!\n";
}

int main() {
    readSensor();
}
/*🔹 Why volatile?
Without volatile, the compiler might assume sensorData never changes inside the loop and optimize away the repeated checks, causing an infinite loop.*/
