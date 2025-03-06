#include <memory> // Required for smart pointers

void uniquePtrExample() {
    std::unique_ptr<int> ptr = std::make_unique<int>(10); // No need to delete
    std::cout << *ptr << std::endl;
} // `ptr` is automatically deleted when function ends


