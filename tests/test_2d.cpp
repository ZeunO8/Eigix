#include "test_2d.hpp"
#include <eigix/Matrix2D.hpp>
bool test_2d()
{
    Matrix2D<int, 4, 1> vec;
    vec.populate(1, 2, 3, 4);
    Matrix2D<int, 4, 4> mat;
    mat.populate(1, 0, 0, 0,
                 0, 1, 0, 0,
                 0, 0, 1, 0,
                 0, 0, 0, 1);
    auto vecmod = mat * vec;
    std::cout << vecmod << std::endl;
    return false;
}