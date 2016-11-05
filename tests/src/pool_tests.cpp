// name: pool_test.cpp

#include "catch.hpp"

#include <px/common/pool.hpp>

using namespace px;

TEST_CASE("pool", "[pool]")
{
	pool<int, 10> p;
	REQUIRE(p.size() == 0);

	int counter;

	counter = 0;
	p.enumerate_active([&counter](auto) { ++counter; });
	REQUIRE(counter == 0);

	int* zero = p.request();
	REQUIRE(p.size() == 1);
	REQUIRE(zero != nullptr);

	counter = 0;
	p.enumerate_active([&counter](auto) { ++counter; });
	REQUIRE(counter == 1);


	p.release(zero);
	REQUIRE(p.size() == 0);

	counter = 0;
	p.enumerate_active([&counter](auto) { ++counter; });
	REQUIRE(counter == 0);
}