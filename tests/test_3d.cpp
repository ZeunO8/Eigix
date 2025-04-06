#include "test_3d.hpp"
#include <eigix/Tensor3D.hpp>
bool test_3d()
{
    Tensor3D<int, 1, 1, 1> point;
    point.populate(5);
    Tensor3D<int, 1, 1, 1> point2;
    point2.populate(5);
    auto pointmod = point2 * point;
    std::cout << pointmod << std::endl;
    return false;
}