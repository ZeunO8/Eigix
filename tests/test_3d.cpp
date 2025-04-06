#include "tests.hpp"
auto test_3d = ([]() -> bool
{
    tests.push_back({"3D", []()->bool
    {
        return false;
    }});
    return true;
}
)();