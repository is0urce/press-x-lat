// name: manager_test.cpp

#include "catch.hpp"

#include <px/es/manager.hpp>

using namespace px;
using namespace px::es;

TEST_CASE("manager", "[manager]")
{
	manager<int, 100> sprites;
	auto sprite = sprites.make();
	REQUIRE(sprites.count() == 1);
	sprite.reset();
	REQUIRE(sprites.count() == 0);
}