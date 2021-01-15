/**
 * @file main.cpp
 * @author Hunter Sun (HunterSun2018@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-10-23
 * 
 * @copyright Copyright (c) 2020
 * 
 * build : g++ main.cpp -o demo -std=c++20
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>
//#include <format>
#include <coroutine>
#include <filesystem>
#include <cassert>
#include "generator.hpp"

using namespace std;
namespace fs = std::filesystem;

void run_range_demo();

generator<fs::directory_entry> list_directory(fs::path path);

int main(int argc, const char *argv[])
{
	try
	{
		run_range_demo();

		for (const auto &f : list_directory(fs::path("..")))
		        std::cout << f.path() << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "exception : " << e.what() << '\n';
	}
}

void run_range_demo()
{
	vector<int> data = {1, 2, 3, 4, 5, 10, 9, 8, 7, 6};
	
	ranges::sort(data);

	auto result = data |
				  views::filter([](int i) { return i % 2 == 1; }) |
				  views::transform([](int i) { return to_string(i); });

	for (auto v : result)
		cout << v << ", ";

	cout << endl;
}

generator<fs::directory_entry> list_directory(fs::path path)
{
    for (auto entry : fs::directory_iterator(path))
    {
        co_yield entry;
        if (is_directory(entry))
            co_yield list_directory(entry);
    }
}

// void run_formating_demo()
// {
// 	cout << format("Hello {}!\n", "world");
// }

uint64_t newton_sqrt(uint64_t x, uint64_t y)
{
    __uint128_t z = __uint128_t(x) * __uint128_t(y);

    auto z1 = z;
    auto s = 1; // shift bits

    // Initialize s
    if (z1 > (__uint128_t(1L) << 64) - 1)
    {
        s = s + 32;
        z1 = z1 >> 64;
    };
    if (z1 > (__uint64_t(1L) << 32) - 1)
    {
        s = s + 16;
        z1 = z1 >> 32;
    };
    if (z1 > 65535)
    {
        s = s + 8;
        z1 = z1 >> 16;
    };
    if (z1 > 255)
    {
        s = s + 4;
        z1 = z1 >> 8;
    };
    if (z1 > 15)
    {
        s = s + 2;
        z1 = z1 >> 4;
    };
    if (z1 > 3)
    {
        s = s + 1;
    }; //z1 = z1 >> 2;

    // iterate
    __uint128_t g0 = __uint128_t(1) << s;
    __uint128_t g1 = (g0 + (z >> s)) >> 1;
    int i = 0;

    while (g1 < g0)
    {
        g0 = g1;
        g1 = (g0 + z / g0) >> 1;

        i++;
    };

    cout << "loop " << i << " times" << endl;

    return g0;
}

void test_newton_sqrt()
{
	assert(newton_sqrt(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF) == 0xFFFFFFFFFFFFFFFF);             //0xFFFFFFFFFFFFFFFF
    assert(newton_sqrt(0xFFFFFFFFFFFFFFFF - 1, 0xFFFFFFFFFFFFFFFF - 1) == 0xFFFFFFFFFFFFFFFF - 1); //0xFFFFFFFFFFFFFFFF
    assert(newton_sqrt(0xFFFFFFFF, 0xFFFFFFFF) == 0xFFFFFFFF);                                     //0xFFFFFFFF
    assert(newton_sqrt(245893761, 245893761) == 245893761);                                        //245893761
    assert(newton_sqrt(3, 3) == 3);
    assert(newton_sqrt(1, 1) == 1);
}