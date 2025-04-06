#include "tests.hpp"
#include "test_2d.hpp"
#include "test_3d.hpp"
#include <iostream>
void register_test(const Test& test);
int main()
{
    register_test({"3D", test_3d});
    register_test({"2D", test_2d});
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
void register_test(const Test& test)
{
    tests.push_back(test);
}