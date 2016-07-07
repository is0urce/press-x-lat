// name: mass_test.cpp

#include "catch.hpp"

#include <px/rl/mass.hpp>
#include <px/rl/traverse.hpp>

using namespace px;
using namespace px::rl;

TEST_CASE("mass", "[mass]")
{
	mass<(unsigned int)traverse::max_value> tile;

	tile.make_transparent();
	REQUIRE(tile.transparent() == true);
	tile.make_opaque();
	REQUIRE(tile.transparent() == false);
}