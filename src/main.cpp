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
