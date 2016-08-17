// name: builders_test.cpp

#include "catch.hpp"

#include <px/core/gen/house_builder.hpp>

#include <random>

TEST_CASE("builders_test", "[house_builder]")
{
	px::core::house_builder builder;

	std::mt19937 rng;

	auto result = builder.build(rng, { 117, 117 });
}