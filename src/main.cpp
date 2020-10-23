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

using namespace std;

void run_range_demo();

int main(int argc, const char *argv[])
{
	try
	{
		run_range_demo();
	}
	catch (const std::exception &e)
	{
		std::cerr << "exception : " << e.what() << '\n';
	}
}

void run_range_demo()
{
	vector<int> data = {1, 2, 3, 4, 5, 10, 9, 8, 7, 6};
	
	sort(begin(data), end(data), [](int a, int b) { return a < b; }) ;

	auto result = data |				  
				  views::filter([](int i) { return i % 2 == 1; }) |
				  views::transform([](int i) { return to_string(i); });

	for (auto v : result)
		cout << v << ", ";

	cout << endl;
}