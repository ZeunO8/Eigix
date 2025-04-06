#include "tests.hpp"
#include <iostream>
int main()
{
    for (auto& test : tests)
    {
        if (!test.function())
        {
            std::cout << "[FAIL] " << test.name << std::endl;
        }
        else
        {
            std::cout << "[PASS] " << test.name << std::endl;
        }
    }
}