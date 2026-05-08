#include <iostream>
#include <cassert>

void testHello() {
    assert(1 + 1 == 2);
}

int main() {
    testHello();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}