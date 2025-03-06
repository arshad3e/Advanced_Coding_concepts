#include <memory> // Required for smart pointers

void uniquePtrExample() {
    std::unique_ptr<int> ptr = std::make_unique<int>(10); // No need to delete
    std::cout << *ptr << std::endl;
} // `ptr` is automatically deleted when function ends

#include <memory>

void sharedPtrExample() {
    std::shared_ptr<int> ptr1 = std::make_shared<int>(10);
    std::shared_ptr<int> ptr2 = ptr1; // Now both share ownership

    std::cout << "Count: " << ptr1.use_count() << std::endl; // 2
} // When the last shared_ptr goes out of scope, memory is freed

#include <memory>

void weakPtrExample() {
    std::shared_ptr<int> shared = std::make_shared<int>(20);
    std::weak_ptr<int> weak = shared; // Doesn't increase reference count
} // `shared` deletes memory; `weak` doesn't prevent deletion
