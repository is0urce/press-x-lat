// name: bsp_test.cpp

#include "catch.hpp"

#include <px/fn/bsp.hpp>

#include <random>

TEST_CASE("bsp", "[bsp]")
{
	std::mt19937 rng;
	px::fn::bsp<std::mt19937> partitions(rng, { { 1, 1 }, { 100, 100} }, 20);
}