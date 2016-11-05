// name: pool_test.cpp

#include "catch.hpp"

#include <px/common/pool.hpp>

#include <list>

TEST_CASE("pool", "[pool]")
{
	auto is_sequental = [](auto& pool) {
		char* last = nullptr;
		bool flag = true;
		pool.enumerate_active([&last, &flag](auto &element) {
			char* ptr = static_cast<char*>(&element)
				if (ptr <= last) flag = false; // next in memory
			last = ptr;
		});
		return flag;
	};

	auto count = [](auto& pool) {
		size_t counter = 0;
		pool.enumerate_active([&counter](auto) { ++counter; });
		return counter;
	};

	typedef int element;
	const size_t maximum = 100;

	px::pool<element, maximum> p;
	REQUIRE(p.size() == 0);
	REQUIRE(count(p) == 0);
	REQUIRE(p.empty() == true);

	element* zero = p.request();
	REQUIRE(zero != nullptr);
	REQUIRE(p.size() == 1);
	REQUIRE(count(p) == 1);
	REQUIRE(p.empty() == false);

	p.release(zero);
	REQUIRE(p.size() == 0);
	REQUIRE(count(p) == 0);
	REQUIRE(p.empty() == true);

	std::list<element*> list;

	for (int i = 0; i < maximum; ++i)
	{
		element* e = p.request();
		REQUIRE(e != nullptr);
		list.push_back(e);
	}
	REQUIRE(p.size() == maximum);
	REQUIRE(count(p) == maximum);
	REQUIRE(p.full() == true);
}