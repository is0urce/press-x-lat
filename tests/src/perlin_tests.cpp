// name: perlin_test.cpp

#include "catch.hpp"

#include <px/fn/perlin.hpp>

using namespace px;
using namespace px::fn;

TEST_CASE("perlin", "[perlin]")
{
	perlin<100, 100> noise;
	noise.fill(std::mt19937());

	auto v = noise.sample(0.3, 0.3, 2);
}