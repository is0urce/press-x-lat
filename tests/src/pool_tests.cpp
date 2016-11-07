// name: pool_tests.cpp

#include "catch.hpp"

#include <px/common/pool.hpp>

#include <algorithm>
#include <list>
#include <random>
#include <iostream>
#include <string>
#include <vector>

TEST_CASE("pool", "[pool]")
{
	auto is_sequental = [](auto& pool) {
		const char* last = nullptr;
		bool flag = true;
		pool.enumerate([&last, &flag](auto &e) {
			const char* ptr = reinterpret_cast<const char*>(&e);
			if (ptr <= last) flag = false; // next in memory
			last = ptr;
		});
		return flag;
	};

	auto count = [](auto& pool) {
		size_t counter = 0;
		pool.enumerate([&counter](auto) { ++counter; });
		return counter;
	};

	typedef int element;
	const size_t maximum = 100;

	px::pool<element, 10> a; // = a - error
	px::pool<element, 10> x = std::move(a); // move-copyable

	px::pool<element, maximum> p;
	REQUIRE(p.size() == 0);
	REQUIRE(count(p) == 0);
	REQUIRE(p.empty() == true);

	element* first = p.request();
	REQUIRE(first != nullptr);
	REQUIRE(p.size() == 1);
	REQUIRE(count(p) == 1);
	REQUIRE(p.empty() == false);
	REQUIRE(p.contains(first) == true);
	REQUIRE(p.contains(nullptr) == false);

	p.release(first);
	REQUIRE(p.size() == 0);
	REQUIRE(count(p) == 0);
	REQUIRE(p.empty() == true);

	std::list<element*> list;

	for (int i = 0; i < maximum; ++i)
	{
		list.push_back(p.request());
	}
	REQUIRE(p.size() == maximum);
	REQUIRE(count(p) == maximum);
	REQUIRE(is_sequental(p) == true);
	REQUIRE(p.full() == true);

	// everything else is nullptr
	REQUIRE(p.request() == nullptr);
	REQUIRE(p.size() == maximum);
	REQUIRE(count(p) == maximum);
	REQUIRE(p.full() == true);

	for (auto eptr : list)
	{
		p.release(eptr);
	}
	list.clear();
	REQUIRE(p.size() == 0);
	REQUIRE(count(p) == 0);
	REQUIRE(p.empty() == true);

	for (int i = 0; i < maximum; ++i)
	{
		list.push_back(p.request());
	}
	REQUIRE(p.size() == maximum);
	REQUIRE(count(p) == maximum);
	REQUIRE(is_sequental(p) == true);
	REQUIRE(p.full() == true);

	// clear
	p.clear();
	REQUIRE(p.size() == 0);
	REQUIRE(count(p) == 0);
	REQUIRE(p.empty() == true);


	////// RNG MADNESS SHOW //////
	std::mt19937 rng;
	p.clear();
	std::vector<element*> arr;
	arr.reserve(maximum);

	auto dump = [](auto &pool) {
		std::vector<element*> vec;
		pool.enumerate([&vec](auto &e) { vec.push_back(&e); });
		return vec;
	};

	for (int j = 0; j < 50; ++j)
	{
		arr.push_back(p.request());
	}
	for (int k = 0; k < 10; ++k)
	{
		for (int j = 0; j < 25; ++j)
		{
			arr.push_back(p.request());
		}
		for (int j = 0; j < 25; ++j)
		{
			size_t index = std::uniform_int_distribution<size_t>{ 0, arr.size() - 1 }(rng);
			p.release(arr[index]);
			arr.erase(std::begin(arr) + index);
		}
	}
	std::sort(std::begin(arr), std::end(arr));
	auto current = dump(p);

	REQUIRE(count(p) == p.size());
	REQUIRE(is_sequental(p) == true);
	REQUIRE(count(p) == arr.size());
	REQUIRE(std::equal(std::begin(arr), std::begin(arr) + arr.size(), std::begin(current)) == true);
}