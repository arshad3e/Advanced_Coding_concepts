/*mutable - Allows Modification in a const Object
Used when a class member needs to be changed inside a const function.
Common for caching mechanisms or debugging flags.
Example: Logging Access Count in a const Function*/

#include <iostream>

class Logger {
private:
    mutable int accessCount;  // Can be modified inside const functions

public:
    Logger() : accessCount(0) {}

    void showMessage() const {
        accessCount++;  // Allowed because it's mutable
        std::cout << "Hello, world! (Accessed " << accessCount << " times)\n";
    }
};

int main() {
    Logger log;
    log.showMessage(); // Access 1
    log.showMessage(); // Access 2
}

/*Why mutable?

showMessage() is a const function, meaning it can't modify any member variables—except for mutable ones.
This is useful for tracking non-critical state changes like debugging counters.*/
