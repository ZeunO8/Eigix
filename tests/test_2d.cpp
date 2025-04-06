#include "tests.hpp"
auto test_2d = ([]() -> bool
{
    tests.push_back({"2D", []()->bool
    {
        return false;
    }});
    return true;
}
)();