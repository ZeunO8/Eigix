#pragma once
#include <vector>
#include <string>
#include <functional>
struct Test
{
    std::string name;
    std::function<bool()> function;
};
inline static std::vector<Test> tests = {};